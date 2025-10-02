/*
 * Copyright (c) 2001, 2025, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_RUNTIME_PERFDATA_HPP
#define SHARE_RUNTIME_PERFDATA_HPP

#include "memory/allocation.hpp"
#include "runtime/atomic.hpp"
#include "runtime/perfDataTypes.hpp"
#include "runtime/perfMemory.hpp"
#include "runtime/timer.hpp"
#ifdef SVM
#include "svmGlobalData.hpp"
#endif // SVM

// NOTE (chaeubl): For SVM, all GC-related PerfData lives in the image heap as pre-allocated oops.
// The perf data entry classes below must not have virtual methods as we directly cast image heap oops to pointers.

/*
 * Classes to support access to production performance data
 *
 * The PerfData class structure is provided for creation, access, and update
 * of performance data (a.k.a. instrumentation) in a specific memory region
 * which is possibly accessible as shared memory. Although not explicitly
 * prevented from doing so, developers should not use the values returned
 * by accessor methods to make algorithmic decisions as they are potentially
 * extracted from a shared memory region. Although any shared memory region
 * created is with appropriate access restrictions, allowing read-write access
 * only to the principal that created the JVM, it is believed that a the
 * shared memory region facilitates an easier attack path than attacks
 * launched through mechanisms such as /proc. For this reason, it is
 * recommended that data returned by PerfData accessor methods be used
 * cautiously.
 *
 * There are three variability classifications of performance data
 *   Constants  -  value is written to the PerfData memory once, on creation
 *   Variables  -  value is modifiable, with no particular restrictions
 *   Counters   -  value is monotonically changing (increasing or decreasing)
 *
 * The performance data items can also have various types. The class
 * hierarchy and the structure of the memory region are designed to
 * accommodate new types as they are needed. Types are specified in
 * terms of Java basic types, which accommodates client applications
 * written in the Java programming language. The class hierarchy is:
 *
 * - PerfData (Abstract)
 *     - PerfLong (Abstract)
 *         - PerfLongConstant        (alias: PerfConstant)
 *         - PerfLongVariant (Abstract)
 *             - PerfLongVariable    (alias: PerfVariable)
 *             - PerfLongCounter     (alias: PerfCounter)
 *
 *     - PerfByteArray (Abstract)
 *         - PerfString (Abstract)
 *             - PerfStringVariable
 *             - PerfStringConstant
 *
 *
 * As seen in the class hierarchy, the initially supported types are:
 *
 *    Long      - performance data holds a Java long type
 *    ByteArray - performance data holds an array of Java bytes
 *                used for holding C++ char arrays.
 *
 * The String type is derived from the ByteArray type.
 *
 * A PerfData subtype is not required to provide an implementation for
 * each variability classification. For example, the String type provides
 * Variable and Constant variability classifications in the PerfStringVariable
 * and PerfStringConstant classes, but does not provide a counter type.
 *
 * Performance data are also described by a unit of measure. Units allow
 * client applications to make reasonable decisions on how to treat
 * performance data generically, preventing the need to hard-code the
 * specifics of a particular data item in client applications. The current
 * set of units are:
 *
 *   None        - the data has no units of measure
 *   Bytes       - data is measured in bytes
 *   Ticks       - data is measured in clock ticks
 *   Events      - data is measured in events. For example,
 *                 the number of garbage collection events or the
 *                 number of methods compiled.
 *   String      - data is not numerical. For example,
 *                 the java command line options
 *   Hertz       - data is a frequency
 *
 * The performance counters also provide a support attribute, indicating
 * the stability of the counter as a programmatic interface. The support
 * level is also implied by the name space in which the counter is created.
 * The counter name space support conventions follow the Java package, class,
 * and property support conventions:
 *
 *    java.*          - stable, supported interface
 *    com.sun.*       - unstable, supported interface
 *    sun.*           - unstable, unsupported interface
 *
 * In the above context, unstable is a measure of the interface support
 * level, not the implementation stability level.
 *
 * Currently, instances of PerfData subtypes are considered to have
 * a life time equal to that of the VM and are managed by the
 * PerfDataManager class. All constructors for the PerfData class and
 * its subtypes have protected constructors. Creation of PerfData
 * instances is performed by invoking various create methods on the
 * PerfDataManager class. Users should not attempt to delete these
 * instances as the PerfDataManager class expects to perform deletion
 * operations on exit of the VM.
 *
 * Examples:
 *
 * Creating performance counter that holds a monotonically increasing
 * long data value with units specified in U_Bytes in the "java.gc.*"
 * name space.
 *
 *   PerfLongCounter* foo_counter;
 *
 *   foo_counter = PerfDataManager::create_long_counter(JAVA_GC, "foo",
 *                                                       PerfData::U_Bytes,
 *                                                       optionalInitialValue,
 *                                                       CHECK);
 *   foo_counter->inc();
 *
 * Creating a performance counter that holds a variably change long
 * data value with units specified in U_Bytes in the "com.sun.ci
 * name space.
 *
 *   PerfLongVariable* bar_variable;
 *   bar_variable = PerfDataManager::create_long_variable(COM_CI, "bar",
.*                                                        PerfData::U_Bytes,
 *                                                        optionalInitialValue,
 *                                                        CHECK);
 *
 *   bar_variable->inc();
 *   bar_variable->set_value(0);
 *
 * Creating a performance counter that holds a constant string value in
 * the "sun.cls.*" name space.
 *
 *   PerfDataManager::create_string_constant(SUN_CLS, "foo", string, CHECK);
 *
 *   Although the create_string_constant() factory method returns a pointer
 *   to the PerfStringConstant object, it can safely be ignored. Developers
 *   are not encouraged to access the string constant's value via this
 *   pointer at this time due to security concerns.
 *
 * For additional uses of PerfData subtypes, see the utility classes
 * PerfTraceTime and PerfTraceTimedEvent below.
 *
 * Always-on counters can be created independent of
 * the UsePerfData flag. Counters will be created on the c-heap
 * if UsePerfData is false.
 *
 * Until further notice, all PerfData objects should be created and
 * manipulated within a guarded block. The guard variable is
 * UsePerfData, a product flag set to true by default. This flag may
 * be removed from the product in the future.
 *
 * There are possible shutdown races between counter uses and counter
 * destruction code. Normal shutdown happens with taking VM_Exit safepoint
 * operation, so in the vast majority of uses this is not an issue. On the
 * paths where a concurrent access can still happen when VM is at safepoint,
 * use the following pattern to coordinate with shutdown:
 *
 * {
 *   GlobalCounter::CriticalSection cs(Thread::current());
 *   if (PerfDataManager::has_PerfData()) {
 *     <update-counter>
 *   }
 * }
 */

namespace svm_gc {

class PerfData {
};


/*
 * PerfLong is the base class for the various Long PerfData subtypes.
 * it contains implementation details that are common among its derived
 * types.
 */
class PerfLong : public PerfData {
  public:
    // returns the value of the data portion of the item in the
    // PerfData memory region.
    inline jlong get_value() { return *(jlong*)valuep(); }

  protected:
    inline jlong* valuep() { return (jlong*)(((address)this) + SVMGlobalData::_offsets._perf_long._value); }
};

/*
 * The PerfLongVariant class, and its alias PerfVariant, implement
 * a PerfData subtype that holds a jlong data value that can be modified
 * in an unrestricted manner. This class provides the implementation details
 * for common functionality among its derived types.
 */
class PerfLongVariant : public PerfLong {
  public:
    inline void inc() { (*valuep())++; }
    inline void inc(jlong val) { (*valuep()) += val; }
    inline void dec(jlong val) { inc(-val); }
};

/*
 * The PerfLongCounter class, and its alias PerfCounter, implement
 * a PerfData subtype that holds a jlong data value that can (should)
 * be modified in a monotonic manner. The inc(jlong) and add(jlong)
 * methods can be passed negative values to implement a monotonically
 * decreasing value. However, we rely upon the programmer to honor
 * the notion that this counter always moves in the same direction -
 * either increasing or decreasing.
 */
class PerfLongCounter : public PerfLongVariant {
};

/*
 * The PerfLongVariable class, and its alias PerfVariable, implement
 * a PerfData subtype that holds a jlong data value that can
 * be modified in an unrestricted manner.
 */
class PerfLongVariable : public PerfLongVariant {
  public:
    inline void set_value(jlong val) { (*valuep()) = val; }
};

/*
 * The PerfByteArray provides a PerfData subtype that allows the creation
 * of a contiguous region of the PerfData memory region for storing a vector
 * of bytes. This class is currently intended to be a base class for
 * the PerfString class, and cannot be instantiated directly.
 */
class PerfByteArray : public PerfData {
};

class PerfString : public PerfByteArray {
};

/*
 * The PerfStringVariable class provides a PerfData sub class that
 * allows a null terminated string of single byte character data
 * to be stored in PerfData memory region. The string value can be reset
 * after initialization. If the string value is >= max_length, then
 * it will be truncated to max_length characters. The copied string
 * is always null terminated.
 */
class PerfStringVariable : public PerfString{
  public:
    void set_value(const char* val);
};

/*
 * this class will administer a PerfCounter used as a time accumulator
 * for a basic block much like the TraceTime class.
 *
 * Example:
 *
 *    static PerfCounter* my_time_counter = PerfDataManager::create_counter("my.time.counter", PerfData::U_Ticks, 0LL, CHECK);
 *
 *    {
 *      PerfTraceTime ptt(my_time_counter);
 *      // perform the operation you want to measure
 *    }
 *
 * Note: use of this class does not need to occur within a guarded
 * block. The UsePerfData guard is used with the implementation
 * of this class.
 */
class PerfTraceTime : public StackObj {

  protected:
    elapsedTimer _t;
    PerfLongCounter* _timerp;

  public:
    inline PerfTraceTime(PerfLongCounter* timerp) : _timerp(timerp) {
      if (!UsePerfData || timerp == nullptr) { return; }
      _t.start();
    }

    ~PerfTraceTime() {
      if (!UsePerfData || !_t.is_active()) { return; }
      _t.stop();
      _timerp->inc(_t.ticks());
    }
};

/* The PerfTraceTimedEvent class is responsible for counting the
 * occurrence of some event and measuring the the elapsed time of
 * the event in two separate PerfCounter instances.
 *
 * Example:
 *
 *    static PerfCounter* my_time_counter = PerfDataManager::create_counter("my.time.counter", PerfData::U_Ticks, CHECK);
 *    static PerfCounter* my_event_counter = PerfDataManager::create_counter("my.event.counter", PerfData::U_Events, CHECK);
 *
 *    {
 *      PerfTraceTimedEvent ptte(my_time_counter, my_event_counter);
 *      // perform the operation you want to count and measure
 *    }
 *
 * Note: use of this class does not need to occur within a guarded
 * block. The UsePerfData guard is used with the implementation
 * of this class.
 *
 */
class PerfTraceTimedEvent : public PerfTraceTime {

  protected:
    PerfLongCounter* _eventp;

  public:
    inline PerfTraceTimedEvent(PerfLongCounter* timerp, PerfLongCounter* eventp): PerfTraceTime(timerp), _eventp(eventp) {
      if (!UsePerfData || timerp == nullptr) { return; }
      _eventp->inc();
    }

};

} // namespace svm_gc

#endif // SHARE_RUNTIME_PERFDATA_HPP
