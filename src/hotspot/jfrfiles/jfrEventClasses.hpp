/*
 * Copyright (c) 2019, 2025, Oracle and/or its affiliates. All rights reserved.
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

/* AUTOMATICALLY GENERATED FILE - DO NOT EDIT */

#ifndef JFRFILES_JFREVENTCLASSES_HPP
#define JFRFILES_JFREVENTCLASSES_HPP

#include "jfrfiles/jfrTypes.hpp"
#include "jfr/utilities/jfrTypes.hpp"
#include "oops/klass.hpp"
#include "runtime/thread.hpp"
#include "utilities/macros.hpp"
#include "utilities/ticks.hpp"
#if INCLUDE_JFR
#include "jfr/recorder/service/jfrEvent.hpp"
/*
 * Each event class has an assert member function verify() which is invoked
 * just before the engine writes the event and its fields to the data stream.
 * The purpose of verify() is to ensure that all fields in the event are initialized
 * and set before attempting to commit.
 *
 * We enforce this requirement because events are generally stack allocated and therefore
 * *not* initialized to default values. This prevents us from inadvertently committing
 * uninitialized values to the data stream.
 *
 * The assert message contains both the index (zero based) as well as the name of the field.
 */

struct JfrStructVirtualSpace
{
 private:
  u8 _start;
  u8 _committedEnd;
  u8 _committedSize;
  u8 _reservedEnd;
  u8 _reservedSize;

 public:
  void set_start(u8 new_value) { this->_start = new_value; }
  void set_committedEnd(u8 new_value) { this->_committedEnd = new_value; }
  void set_committedSize(u8 new_value) { this->_committedSize = new_value; }
  void set_reservedEnd(u8 new_value) { this->_reservedEnd = new_value; }
  void set_reservedSize(u8 new_value) { this->_reservedSize = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_start);
    w.write(_committedEnd);
    w.write(_committedSize);
    w.write(_reservedEnd);
    w.write(_reservedSize);
  }
};


struct JfrStructObjectSpace
{
 private:
  u8 _start;
  u8 _end;
  u8 _used;
  u8 _size;

 public:
  void set_start(u8 new_value) { this->_start = new_value; }
  void set_end(u8 new_value) { this->_end = new_value; }
  void set_used(u8 new_value) { this->_used = new_value; }
  void set_size(u8 new_value) { this->_size = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_start);
    w.write(_end);
    w.write(_used);
    w.write(_size);
  }
};


struct JfrStructMetaspaceSizes
{
 private:
  u8 _committed;
  u8 _used;
  u8 _reserved;

 public:
  void set_committed(u8 new_value) { this->_committed = new_value; }
  void set_used(u8 new_value) { this->_used = new_value; }
  void set_reserved(u8 new_value) { this->_reserved = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_committed);
    w.write(_used);
    w.write(_reserved);
  }
};


struct JfrStructCopyFailed
{
 private:
  u8 _objectCount;
  u8 _firstSize;
  u8 _smallestSize;
  u8 _totalSize;

 public:
  void set_objectCount(u8 new_value) { this->_objectCount = new_value; }
  void set_firstSize(u8 new_value) { this->_firstSize = new_value; }
  void set_smallestSize(u8 new_value) { this->_smallestSize = new_value; }
  void set_totalSize(u8 new_value) { this->_totalSize = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_objectCount);
    w.write(_firstSize);
    w.write(_smallestSize);
    w.write(_totalSize);
  }
};


struct JfrStructG1EvacuationStatistics
{
 private:
  unsigned _gcId;
  u8 _allocated;
  u8 _wasted;
  u8 _used;
  u8 _undoWaste;
  u8 _regionEndWaste;
  unsigned _regionsRefilled;
  u8 _numPlabsFilled;
  u8 _directAllocated;
  u8 _numDirectAllocated;
  u8 _failureUsed;
  u8 _failureWaste;

 public:
  void set_gcId(unsigned new_value) { this->_gcId = new_value; }
  void set_allocated(u8 new_value) { this->_allocated = new_value; }
  void set_wasted(u8 new_value) { this->_wasted = new_value; }
  void set_used(u8 new_value) { this->_used = new_value; }
  void set_undoWaste(u8 new_value) { this->_undoWaste = new_value; }
  void set_regionEndWaste(u8 new_value) { this->_regionEndWaste = new_value; }
  void set_regionsRefilled(unsigned new_value) { this->_regionsRefilled = new_value; }
  void set_numPlabsFilled(u8 new_value) { this->_numPlabsFilled = new_value; }
  void set_directAllocated(u8 new_value) { this->_directAllocated = new_value; }
  void set_numDirectAllocated(u8 new_value) { this->_numDirectAllocated = new_value; }
  void set_failureUsed(u8 new_value) { this->_failureUsed = new_value; }
  void set_failureWaste(u8 new_value) { this->_failureWaste = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_allocated);
    w.write(_wasted);
    w.write(_used);
    w.write(_undoWaste);
    w.write(_regionEndWaste);
    w.write(_regionsRefilled);
    w.write(_numPlabsFilled);
    w.write(_directAllocated);
    w.write(_numDirectAllocated);
    w.write(_failureUsed);
    w.write(_failureWaste);
  }
};


struct JfrStructCalleeMethod
{
 private:
  const char* _type;
  const char* _name;
  const char* _descriptor;

 public:
  void set_type(const char* new_value) { this->_type = new_value; }
  void set_name(const char* new_value) { this->_name = new_value; }
  void set_descriptor(const char* new_value) { this->_descriptor = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_type);
    w.write(_name);
    w.write(_descriptor);
  }
};


struct JfrStructStackFrame
{
 private:
  const Method* _method;
  s4 _lineNumber;
  s4 _bytecodeIndex;
  u8 _type;

 public:
  void set_method(const Method* new_value) { this->_method = new_value; }
  void set_lineNumber(s4 new_value) { this->_lineNumber = new_value; }
  void set_bytecodeIndex(s4 new_value) { this->_bytecodeIndex = new_value; }
  void set_type(u8 new_value) { this->_type = new_value; }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_method);
    w.write(_lineNumber);
    w.write(_bytecodeIndex);
    w.write(_type);
  }
};


class EventDuration : public JfrEvent<EventDuration>
{
 private:

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrDurationEvent;

  EventDuration(EventStartTime timing=TIMED) : JfrEvent<EventDuration>(timing) {}


  template <typename Writer>
  void writeData(Writer& w) {
    JfrEventSetting::unhide_internal_types();
  }

  using JfrEvent<EventDuration>::commit; // else commit() is hidden by overloaded versions in this class

  EventDuration(
    ) : JfrEvent<EventDuration>(TIMED) {
    if (should_commit()) {
    }
  }

  void commit() {
    if (should_commit()) {
      commit();
    }
  }

#ifdef ASSERT
  void verify() const {
  }
#endif
};

class EventInstant : public JfrEvent<EventInstant>
{
 private:

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrInstantEvent;

  EventInstant(EventStartTime timing=TIMED) : JfrEvent<EventInstant>(timing) {}


  template <typename Writer>
  void writeData(Writer& w) {
    JfrEventSetting::unhide_internal_types();
  }

  using JfrEvent<EventInstant>::commit; // else commit() is hidden by overloaded versions in this class



#ifdef ASSERT
  void verify() const {
  }
#endif
};

class EventValue : public JfrEvent<EventValue>
{
 private:
  u8 _value;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrValueEvent;

  EventValue(EventStartTime timing=TIMED) : JfrEvent<EventValue>(timing) {}

  void set_value(u8 new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    JfrEventSetting::unhide_internal_types();
    w.write(_value);
  }

  using JfrEvent<EventValue>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 value) {
    EventValue me(UNTIMED);

    if (me.should_commit()) {
      me.set_value(value);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_value");
  }
#endif
};

class EventText : public JfrEvent<EventText>
{
 private:
  const char* _text;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrTextEvent;

  EventText(EventStartTime timing=TIMED) : JfrEvent<EventText>(timing) {}

  void set_text(const char* new_value) {
    this->_text = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    JfrEventSetting::unhide_internal_types();
    w.write(_text);
  }

  using JfrEvent<EventText>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* text) {
    EventText me(UNTIMED);

    if (me.should_commit()) {
      me.set_text(text);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_text");
  }
#endif
};

class EventZThreadDebug : public JfrEvent<EventZThreadDebug>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZThreadDebugEvent;

  EventZThreadDebug(EventStartTime timing=TIMED) : JfrEvent<EventZThreadDebug>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    JfrEventSetting::unhide_internal_types();
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventZThreadDebug>::commit; // else commit() is hidden by overloaded versions in this class

  EventZThreadDebug(
    unsigned gcId,
    const char* name) : JfrEvent<EventZThreadDebug>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventZThreadDebug me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventResidentSetSize : public JfrEvent<EventResidentSetSize>
{
 private:
  u8 _size;
  u8 _peak;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrResidentSetSizeEvent;

  EventResidentSetSize(EventStartTime timing=TIMED) : JfrEvent<EventResidentSetSize>(timing) {}

  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_peak(u8 new_value) {
    this->_peak = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_size);
    w.write(_peak);
  }

  using JfrEvent<EventResidentSetSize>::commit; // else commit() is hidden by overloaded versions in this class

  EventResidentSetSize(
    u8 size,
    u8 peak) : JfrEvent<EventResidentSetSize>(TIMED) {
    if (should_commit()) {
      set_size(size);
      set_peak(peak);
    }
  }

  void commit(u8 size,
              u8 peak) {
    if (should_commit()) {
      set_size(size);
      set_peak(peak);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 size,
                     u8 peak) {
    EventResidentSetSize me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_size(size);
      me.set_peak(peak);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_size");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_peak");
  }
#endif
};

class EventThreadStart : public JfrEvent<EventThreadStart>
{
 private:
  u8 _thread;
  u8 _parentThread;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrThreadStartEvent;

  EventThreadStart(EventStartTime timing=TIMED) : JfrEvent<EventThreadStart>(timing) {}

  void set_thread(u8 new_value) {
    this->_thread = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_parentThread(u8 new_value) {
    this->_parentThread = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_thread);
    w.write(_parentThread);
  }

  using JfrEvent<EventThreadStart>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 thread,
                     u8 parentThread) {
    EventThreadStart me(UNTIMED);

    if (me.should_commit()) {
      me.set_thread(thread);
      me.set_parentThread(parentThread);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_thread");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_parentThread");
  }
#endif
};

class EventThreadEnd : public JfrEvent<EventThreadEnd>
{
 private:
  u8 _thread;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrThreadEndEvent;

  EventThreadEnd(EventStartTime timing=TIMED) : JfrEvent<EventThreadEnd>(timing) {}

  void set_thread(u8 new_value) {
    this->_thread = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_thread);
  }

  using JfrEvent<EventThreadEnd>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 thread) {
    EventThreadEnd me(UNTIMED);

    if (me.should_commit()) {
      me.set_thread(thread);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_thread");
  }
#endif
};

class EventThreadPark : public JfrEvent<EventThreadPark>
{
 private:
  const Klass* _parkedClass;
  s8 _timeout;
  s8 _until;
  u8 _address;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrThreadParkEvent;

  EventThreadPark(EventStartTime timing=TIMED) : JfrEvent<EventThreadPark>(timing) {}

  void set_parkedClass(const Klass* new_value) {
    this->_parkedClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_timeout(s8 new_value) {
    this->_timeout = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_until(s8 new_value) {
    this->_until = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_address(u8 new_value) {
    this->_address = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_parkedClass);
    w.write(_timeout);
    w.write(_until);
    w.write(_address);
  }

  using JfrEvent<EventThreadPark>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadPark(
    const Klass* parkedClass,
    s8 timeout,
    s8 until,
    u8 address) : JfrEvent<EventThreadPark>(TIMED) {
    if (should_commit()) {
      set_parkedClass(parkedClass);
      set_timeout(timeout);
      set_until(until);
      set_address(address);
    }
  }

  void commit(const Klass* parkedClass,
              s8 timeout,
              s8 until,
              u8 address) {
    if (should_commit()) {
      set_parkedClass(parkedClass);
      set_timeout(timeout);
      set_until(until);
      set_address(address);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* parkedClass,
                     s8 timeout,
                     s8 until,
                     u8 address) {
    EventThreadPark me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_parkedClass(parkedClass);
      me.set_timeout(timeout);
      me.set_until(until);
      me.set_address(address);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_parkedClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_timeout");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_until");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_address");
  }
#endif
};

class EventJavaMonitorEnter : public JfrEvent<EventJavaMonitorEnter>
{
 private:
  const Klass* _monitorClass;
  u8 _previousOwner;
  u8 _address;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrJavaMonitorEnterEvent;

  EventJavaMonitorEnter(EventStartTime timing=TIMED) : JfrEvent<EventJavaMonitorEnter>(timing) {}

  void set_monitorClass(const Klass* new_value) {
    this->_monitorClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_previousOwner(u8 new_value) {
    this->_previousOwner = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_address(u8 new_value) {
    this->_address = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_monitorClass);
    w.write(_previousOwner);
    w.write(_address);
  }

  using JfrEvent<EventJavaMonitorEnter>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorEnter(
    const Klass* monitorClass,
    u8 previousOwner,
    u8 address) : JfrEvent<EventJavaMonitorEnter>(TIMED) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_previousOwner(previousOwner);
      set_address(address);
    }
  }

  void commit(const Klass* monitorClass,
              u8 previousOwner,
              u8 address) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_previousOwner(previousOwner);
      set_address(address);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 previousOwner,
                     u8 address) {
    EventJavaMonitorEnter me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_monitorClass(monitorClass);
      me.set_previousOwner(previousOwner);
      me.set_address(address);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_monitorClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_previousOwner");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_address");
  }
#endif
};

class EventJavaMonitorWait : public JfrEvent<EventJavaMonitorWait>
{
 private:
  const Klass* _monitorClass;
  u8 _notifier;
  s8 _timeout;
  bool _timedOut;
  u8 _address;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrJavaMonitorWaitEvent;

  EventJavaMonitorWait(EventStartTime timing=TIMED) : JfrEvent<EventJavaMonitorWait>(timing) {}

  void set_monitorClass(const Klass* new_value) {
    this->_monitorClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_notifier(u8 new_value) {
    this->_notifier = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_timeout(s8 new_value) {
    this->_timeout = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_timedOut(bool new_value) {
    this->_timedOut = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_address(u8 new_value) {
    this->_address = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_monitorClass);
    w.write(_notifier);
    w.write(_timeout);
    w.write(_timedOut);
    w.write(_address);
  }

  using JfrEvent<EventJavaMonitorWait>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorWait(
    const Klass* monitorClass,
    u8 notifier,
    s8 timeout,
    bool timedOut,
    u8 address) : JfrEvent<EventJavaMonitorWait>(TIMED) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_notifier(notifier);
      set_timeout(timeout);
      set_timedOut(timedOut);
      set_address(address);
    }
  }

  void commit(const Klass* monitorClass,
              u8 notifier,
              s8 timeout,
              bool timedOut,
              u8 address) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_notifier(notifier);
      set_timeout(timeout);
      set_timedOut(timedOut);
      set_address(address);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 notifier,
                     s8 timeout,
                     bool timedOut,
                     u8 address) {
    EventJavaMonitorWait me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_monitorClass(monitorClass);
      me.set_notifier(notifier);
      me.set_timeout(timeout);
      me.set_timedOut(timedOut);
      me.set_address(address);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_monitorClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_notifier");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_timeout");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_timedOut");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_address");
  }
#endif
};

class EventJavaMonitorNotify : public JfrEvent<EventJavaMonitorNotify>
{
 private:
  const Klass* _monitorClass;
  unsigned _notifiedCount;
  u8 _address;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrJavaMonitorNotifyEvent;

  EventJavaMonitorNotify(EventStartTime timing=TIMED) : JfrEvent<EventJavaMonitorNotify>(timing) {}

  void set_monitorClass(const Klass* new_value) {
    this->_monitorClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_notifiedCount(unsigned new_value) {
    this->_notifiedCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_address(u8 new_value) {
    this->_address = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_monitorClass);
    w.write(_notifiedCount);
    w.write(_address);
  }

  using JfrEvent<EventJavaMonitorNotify>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorNotify(
    const Klass* monitorClass,
    unsigned notifiedCount,
    u8 address) : JfrEvent<EventJavaMonitorNotify>(TIMED) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_notifiedCount(notifiedCount);
      set_address(address);
    }
  }

  void commit(const Klass* monitorClass,
              unsigned notifiedCount,
              u8 address) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_notifiedCount(notifiedCount);
      set_address(address);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     unsigned notifiedCount,
                     u8 address) {
    EventJavaMonitorNotify me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_monitorClass(monitorClass);
      me.set_notifiedCount(notifiedCount);
      me.set_address(address);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_monitorClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_notifiedCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_address");
  }
#endif
};

class EventJavaMonitorInflate : public JfrEvent<EventJavaMonitorInflate>
{
 private:
  const Klass* _monitorClass;
  u8 _address;
  u8 _cause;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrJavaMonitorInflateEvent;

  EventJavaMonitorInflate(EventStartTime timing=TIMED) : JfrEvent<EventJavaMonitorInflate>(timing) {}

  void set_monitorClass(const Klass* new_value) {
    this->_monitorClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_address(u8 new_value) {
    this->_address = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_cause(u8 new_value) {
    this->_cause = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_monitorClass);
    w.write(_address);
    w.write(_cause);
  }

  using JfrEvent<EventJavaMonitorInflate>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorInflate(
    const Klass* monitorClass,
    u8 address,
    u8 cause) : JfrEvent<EventJavaMonitorInflate>(TIMED) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_address(address);
      set_cause(cause);
    }
  }

  void commit(const Klass* monitorClass,
              u8 address,
              u8 cause) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_address(address);
      set_cause(cause);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 address,
                     u8 cause) {
    EventJavaMonitorInflate me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_monitorClass(monitorClass);
      me.set_address(address);
      me.set_cause(cause);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_monitorClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_address");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_cause");
  }
#endif
};

class EventJavaMonitorDeflate : public JfrEvent<EventJavaMonitorDeflate>
{
 private:
  const Klass* _monitorClass;
  u8 _address;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrJavaMonitorDeflateEvent;

  EventJavaMonitorDeflate(EventStartTime timing=TIMED) : JfrEvent<EventJavaMonitorDeflate>(timing) {}

  void set_monitorClass(const Klass* new_value) {
    this->_monitorClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_address(u8 new_value) {
    this->_address = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_monitorClass);
    w.write(_address);
  }

  using JfrEvent<EventJavaMonitorDeflate>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorDeflate(
    const Klass* monitorClass,
    u8 address) : JfrEvent<EventJavaMonitorDeflate>(TIMED) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_address(address);
    }
  }

  void commit(const Klass* monitorClass,
              u8 address) {
    if (should_commit()) {
      set_monitorClass(monitorClass);
      set_address(address);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 address) {
    EventJavaMonitorDeflate me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_monitorClass(monitorClass);
      me.set_address(address);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_monitorClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_address");
  }
#endif
};

class EventJavaMonitorStatistics : public JfrEvent<EventJavaMonitorStatistics>
{
 private:
  u8 _count;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrJavaMonitorStatisticsEvent;

  EventJavaMonitorStatistics(EventStartTime timing=TIMED) : JfrEvent<EventJavaMonitorStatistics>(timing) {}

  void set_count(u8 new_value) {
    this->_count = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_count);
  }

  using JfrEvent<EventJavaMonitorStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorStatistics(
    u8 count) : JfrEvent<EventJavaMonitorStatistics>(TIMED) {
    if (should_commit()) {
      set_count(count);
    }
  }

  void commit(u8 count) {
    if (should_commit()) {
      set_count(count);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 count) {
    EventJavaMonitorStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_count(count);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_count");
  }
#endif
};

class EventSyncOnValueBasedClass : public JfrEvent<EventSyncOnValueBasedClass>
{
 private:
  const Klass* _valueBasedClass;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrSyncOnValueBasedClassEvent;

  EventSyncOnValueBasedClass(EventStartTime timing=TIMED) : JfrEvent<EventSyncOnValueBasedClass>(timing) {}

  void set_valueBasedClass(const Klass* new_value) {
    this->_valueBasedClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_valueBasedClass);
  }

  using JfrEvent<EventSyncOnValueBasedClass>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* valueBasedClass) {
    EventSyncOnValueBasedClass me(UNTIMED);

    if (me.should_commit()) {
      me.set_valueBasedClass(valueBasedClass);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_valueBasedClass");
  }
#endif
};

class EventContinuationFreeze : public JfrEvent<EventContinuationFreeze>
{
 private:
  u8 _carrierThread;
  const Klass* _continuationClass;
  u2 _interpretedFrames;
  u2 _size;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrContinuationFreezeEvent;

  EventContinuationFreeze(EventStartTime timing=TIMED) : JfrEvent<EventContinuationFreeze>(timing) {}

  void set_carrierThread(u8 new_value) {
    this->_carrierThread = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_continuationClass(const Klass* new_value) {
    this->_continuationClass = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_interpretedFrames(u2 new_value) {
    this->_interpretedFrames = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_size(u2 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_carrierThread);
    w.write(_continuationClass);
    w.write(_interpretedFrames);
    w.write(_size);
  }

  using JfrEvent<EventContinuationFreeze>::commit; // else commit() is hidden by overloaded versions in this class

  EventContinuationFreeze(
    u8 carrierThread,
    const Klass* continuationClass,
    u2 interpretedFrames,
    u2 size) : JfrEvent<EventContinuationFreeze>(TIMED) {
    if (should_commit()) {
      set_carrierThread(carrierThread);
      set_continuationClass(continuationClass);
      set_interpretedFrames(interpretedFrames);
      set_size(size);
    }
  }

  void commit(u8 carrierThread,
              const Klass* continuationClass,
              u2 interpretedFrames,
              u2 size) {
    if (should_commit()) {
      set_carrierThread(carrierThread);
      set_continuationClass(continuationClass);
      set_interpretedFrames(interpretedFrames);
      set_size(size);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 carrierThread,
                     const Klass* continuationClass,
                     u2 interpretedFrames,
                     u2 size) {
    EventContinuationFreeze me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_carrierThread(carrierThread);
      me.set_continuationClass(continuationClass);
      me.set_interpretedFrames(interpretedFrames);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_carrierThread");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_continuationClass");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_interpretedFrames");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventContinuationThaw : public JfrEvent<EventContinuationThaw>
{
 private:
  u8 _carrierThread;
  const Klass* _continuationClass;
  u2 _interpretedFrames;
  u2 _size;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrContinuationThawEvent;

  EventContinuationThaw(EventStartTime timing=TIMED) : JfrEvent<EventContinuationThaw>(timing) {}

  void set_carrierThread(u8 new_value) {
    this->_carrierThread = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_continuationClass(const Klass* new_value) {
    this->_continuationClass = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_interpretedFrames(u2 new_value) {
    this->_interpretedFrames = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_size(u2 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_carrierThread);
    w.write(_continuationClass);
    w.write(_interpretedFrames);
    w.write(_size);
  }

  using JfrEvent<EventContinuationThaw>::commit; // else commit() is hidden by overloaded versions in this class

  EventContinuationThaw(
    u8 carrierThread,
    const Klass* continuationClass,
    u2 interpretedFrames,
    u2 size) : JfrEvent<EventContinuationThaw>(TIMED) {
    if (should_commit()) {
      set_carrierThread(carrierThread);
      set_continuationClass(continuationClass);
      set_interpretedFrames(interpretedFrames);
      set_size(size);
    }
  }

  void commit(u8 carrierThread,
              const Klass* continuationClass,
              u2 interpretedFrames,
              u2 size) {
    if (should_commit()) {
      set_carrierThread(carrierThread);
      set_continuationClass(continuationClass);
      set_interpretedFrames(interpretedFrames);
      set_size(size);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 carrierThread,
                     const Klass* continuationClass,
                     u2 interpretedFrames,
                     u2 size) {
    EventContinuationThaw me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_carrierThread(carrierThread);
      me.set_continuationClass(continuationClass);
      me.set_interpretedFrames(interpretedFrames);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_carrierThread");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_continuationClass");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_interpretedFrames");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventContinuationFreezeFast : public JfrEvent<EventContinuationFreezeFast>
{
 private:
  u8 _id;
  unsigned _size;
  bool _allocate;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrContinuationFreezeFastEvent;

  EventContinuationFreezeFast(EventStartTime timing=TIMED) : JfrEvent<EventContinuationFreezeFast>(timing) {}

  void set_id(u8 new_value) {
    this->_id = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_size(unsigned new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_allocate(bool new_value) {
    this->_allocate = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_id);
    w.write(_size);
    w.write(_allocate);
  }

  using JfrEvent<EventContinuationFreezeFast>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id,
                     unsigned size,
                     bool allocate) {
    EventContinuationFreezeFast me(UNTIMED);

    if (me.should_commit()) {
      me.set_id(id);
      me.set_size(size);
      me.set_allocate(allocate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_id");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_size");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_allocate");
  }
#endif
};

class EventContinuationFreezeSlow : public JfrEvent<EventContinuationFreezeSlow>
{
 private:
  u8 _id;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrContinuationFreezeSlowEvent;

  EventContinuationFreezeSlow(EventStartTime timing=TIMED) : JfrEvent<EventContinuationFreezeSlow>(timing) {}

  void set_id(u8 new_value) {
    this->_id = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_id);
  }

  using JfrEvent<EventContinuationFreezeSlow>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id) {
    EventContinuationFreezeSlow me(UNTIMED);

    if (me.should_commit()) {
      me.set_id(id);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_id");
  }
#endif
};

class EventContinuationThawFast : public JfrEvent<EventContinuationThawFast>
{
 private:
  u8 _id;
  bool _full;
  unsigned _size;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrContinuationThawFastEvent;

  EventContinuationThawFast(EventStartTime timing=TIMED) : JfrEvent<EventContinuationThawFast>(timing) {}

  void set_id(u8 new_value) {
    this->_id = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_full(bool new_value) {
    this->_full = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_size(unsigned new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_id);
    w.write(_full);
    w.write(_size);
  }

  using JfrEvent<EventContinuationThawFast>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id,
                     bool full,
                     unsigned size) {
    EventContinuationThawFast me(UNTIMED);

    if (me.should_commit()) {
      me.set_id(id);
      me.set_full(full);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_id");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_full");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventContinuationThawSlow : public JfrEvent<EventContinuationThawSlow>
{
 private:
  u8 _id;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrContinuationThawSlowEvent;

  EventContinuationThawSlow(EventStartTime timing=TIMED) : JfrEvent<EventContinuationThawSlow>(timing) {}

  void set_id(u8 new_value) {
    this->_id = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_id);
  }

  using JfrEvent<EventContinuationThawSlow>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id) {
    EventContinuationThawSlow me(UNTIMED);

    if (me.should_commit()) {
      me.set_id(id);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_id");
  }
#endif
};

class EventVirtualThreadPinned : public JfrEvent<EventVirtualThreadPinned>
{
 private:
  const char* _blockingOperation;
  const char* _pinnedReason;
  u8 _carrierThread;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrVirtualThreadPinnedEvent;

  EventVirtualThreadPinned(EventStartTime timing=TIMED) : JfrEvent<EventVirtualThreadPinned>(timing) {}

  void set_blockingOperation(const char* new_value) {
    this->_blockingOperation = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_pinnedReason(const char* new_value) {
    this->_pinnedReason = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_carrierThread(u8 new_value) {
    this->_carrierThread = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_blockingOperation);
    w.write(_pinnedReason);
    w.write(_carrierThread);
  }

  using JfrEvent<EventVirtualThreadPinned>::commit; // else commit() is hidden by overloaded versions in this class

  EventVirtualThreadPinned(
    const char* blockingOperation,
    const char* pinnedReason,
    u8 carrierThread) : JfrEvent<EventVirtualThreadPinned>(TIMED) {
    if (should_commit()) {
      set_blockingOperation(blockingOperation);
      set_pinnedReason(pinnedReason);
      set_carrierThread(carrierThread);
    }
  }

  void commit(const char* blockingOperation,
              const char* pinnedReason,
              u8 carrierThread) {
    if (should_commit()) {
      set_blockingOperation(blockingOperation);
      set_pinnedReason(pinnedReason);
      set_carrierThread(carrierThread);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* blockingOperation,
                     const char* pinnedReason,
                     u8 carrierThread) {
    EventVirtualThreadPinned me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_blockingOperation(blockingOperation);
      me.set_pinnedReason(pinnedReason);
      me.set_carrierThread(carrierThread);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_blockingOperation");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_pinnedReason");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_carrierThread");
  }
#endif
};

class EventReservedStackActivation : public JfrEvent<EventReservedStackActivation>
{
 private:
  const Method* _method;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrReservedStackActivationEvent;

  EventReservedStackActivation(EventStartTime timing=TIMED) : JfrEvent<EventReservedStackActivation>(timing) {}

  void set_method(const Method* new_value) {
    this->_method = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_method);
  }

  using JfrEvent<EventReservedStackActivation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Method* method) {
    EventReservedStackActivation me(UNTIMED);

    if (me.should_commit()) {
      me.set_method(method);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_method");
  }
#endif
};

class EventClassLoad : public JfrEvent<EventClassLoad>
{
 private:
  const Klass* _loadedClass;
  const ClassLoaderData* _definingClassLoader;
  const ClassLoaderData* _initiatingClassLoader;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrClassLoadEvent;

  EventClassLoad(EventStartTime timing=TIMED) : JfrEvent<EventClassLoad>(timing) {}

  void set_loadedClass(const Klass* new_value) {
    this->_loadedClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_definingClassLoader(const ClassLoaderData* new_value) {
    this->_definingClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_initiatingClassLoader(const ClassLoaderData* new_value) {
    this->_initiatingClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_loadedClass);
    w.write(_definingClassLoader);
    w.write(_initiatingClassLoader);
  }

  using JfrEvent<EventClassLoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventClassLoad(
    const Klass* loadedClass,
    const ClassLoaderData* definingClassLoader,
    const ClassLoaderData* initiatingClassLoader) : JfrEvent<EventClassLoad>(TIMED) {
    if (should_commit()) {
      set_loadedClass(loadedClass);
      set_definingClassLoader(definingClassLoader);
      set_initiatingClassLoader(initiatingClassLoader);
    }
  }

  void commit(const Klass* loadedClass,
              const ClassLoaderData* definingClassLoader,
              const ClassLoaderData* initiatingClassLoader) {
    if (should_commit()) {
      set_loadedClass(loadedClass);
      set_definingClassLoader(definingClassLoader);
      set_initiatingClassLoader(initiatingClassLoader);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* loadedClass,
                     const ClassLoaderData* definingClassLoader,
                     const ClassLoaderData* initiatingClassLoader) {
    EventClassLoad me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_loadedClass(loadedClass);
      me.set_definingClassLoader(definingClassLoader);
      me.set_initiatingClassLoader(initiatingClassLoader);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_loadedClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_definingClassLoader");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_initiatingClassLoader");
  }
#endif
};

class EventClassDefine : public JfrEvent<EventClassDefine>
{
 private:
  const Klass* _definedClass;
  const ClassLoaderData* _definingClassLoader;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrClassDefineEvent;

  EventClassDefine(EventStartTime timing=TIMED) : JfrEvent<EventClassDefine>(timing) {}

  void set_definedClass(const Klass* new_value) {
    this->_definedClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_definingClassLoader(const ClassLoaderData* new_value) {
    this->_definingClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_definedClass);
    w.write(_definingClassLoader);
  }

  using JfrEvent<EventClassDefine>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* definedClass,
                     const ClassLoaderData* definingClassLoader) {
    EventClassDefine me(UNTIMED);

    if (me.should_commit()) {
      me.set_definedClass(definedClass);
      me.set_definingClassLoader(definingClassLoader);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_definedClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_definingClassLoader");
  }
#endif
};

class EventClassRedefinition : public JfrEvent<EventClassRedefinition>
{
 private:
  const Klass* _redefinedClass;
  s4 _classModificationCount;
  u8 _redefinitionId;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrClassRedefinitionEvent;

  EventClassRedefinition(EventStartTime timing=TIMED) : JfrEvent<EventClassRedefinition>(timing) {}

  void set_redefinedClass(const Klass* new_value) {
    this->_redefinedClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_classModificationCount(s4 new_value) {
    this->_classModificationCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_redefinitionId(u8 new_value) {
    this->_redefinitionId = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_redefinedClass);
    w.write(_classModificationCount);
    w.write(_redefinitionId);
  }

  using JfrEvent<EventClassRedefinition>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* redefinedClass,
                     s4 classModificationCount,
                     u8 redefinitionId) {
    EventClassRedefinition me(UNTIMED);

    if (me.should_commit()) {
      me.set_redefinedClass(redefinedClass);
      me.set_classModificationCount(classModificationCount);
      me.set_redefinitionId(redefinitionId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_redefinedClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_classModificationCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_redefinitionId");
  }
#endif
};

class EventRedefineClasses : public JfrEvent<EventRedefineClasses>
{
 private:
  s4 _classCount;
  u8 _redefinitionId;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrRedefineClassesEvent;

  EventRedefineClasses(EventStartTime timing=TIMED) : JfrEvent<EventRedefineClasses>(timing) {}

  void set_classCount(s4 new_value) {
    this->_classCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_redefinitionId(u8 new_value) {
    this->_redefinitionId = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_classCount);
    w.write(_redefinitionId);
  }

  using JfrEvent<EventRedefineClasses>::commit; // else commit() is hidden by overloaded versions in this class

  EventRedefineClasses(
    s4 classCount,
    u8 redefinitionId) : JfrEvent<EventRedefineClasses>(TIMED) {
    if (should_commit()) {
      set_classCount(classCount);
      set_redefinitionId(redefinitionId);
    }
  }

  void commit(s4 classCount,
              u8 redefinitionId) {
    if (should_commit()) {
      set_classCount(classCount);
      set_redefinitionId(redefinitionId);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s4 classCount,
                     u8 redefinitionId) {
    EventRedefineClasses me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_classCount(classCount);
      me.set_redefinitionId(redefinitionId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_classCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_redefinitionId");
  }
#endif
};

class EventRetransformClasses : public JfrEvent<EventRetransformClasses>
{
 private:
  s4 _classCount;
  u8 _redefinitionId;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrRetransformClassesEvent;

  EventRetransformClasses(EventStartTime timing=TIMED) : JfrEvent<EventRetransformClasses>(timing) {}

  void set_classCount(s4 new_value) {
    this->_classCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_redefinitionId(u8 new_value) {
    this->_redefinitionId = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_classCount);
    w.write(_redefinitionId);
  }

  using JfrEvent<EventRetransformClasses>::commit; // else commit() is hidden by overloaded versions in this class

  EventRetransformClasses(
    s4 classCount,
    u8 redefinitionId) : JfrEvent<EventRetransformClasses>(TIMED) {
    if (should_commit()) {
      set_classCount(classCount);
      set_redefinitionId(redefinitionId);
    }
  }

  void commit(s4 classCount,
              u8 redefinitionId) {
    if (should_commit()) {
      set_classCount(classCount);
      set_redefinitionId(redefinitionId);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s4 classCount,
                     u8 redefinitionId) {
    EventRetransformClasses me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_classCount(classCount);
      me.set_redefinitionId(redefinitionId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_classCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_redefinitionId");
  }
#endif
};

class EventClassUnload : public JfrEvent<EventClassUnload>
{
 private:
  const Klass* _unloadedClass;
  const ClassLoaderData* _definingClassLoader;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrClassUnloadEvent;

  EventClassUnload(EventStartTime timing=TIMED) : JfrEvent<EventClassUnload>(timing) {}

  void set_unloadedClass(const Klass* new_value) {
    this->_unloadedClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_definingClassLoader(const ClassLoaderData* new_value) {
    this->_definingClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_unloadedClass);
    w.write(_definingClassLoader);
  }

  using JfrEvent<EventClassUnload>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* unloadedClass,
                     const ClassLoaderData* definingClassLoader) {
    EventClassUnload me(UNTIMED);

    if (me.should_commit()) {
      me.set_unloadedClass(unloadedClass);
      me.set_definingClassLoader(definingClassLoader);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_unloadedClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_definingClassLoader");
  }
#endif
};

class EventIntFlagChanged : public JfrEvent<EventIntFlagChanged>
{
 private:
  const char* _name;
  s4 _oldValue;
  s4 _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrIntFlagChangedEvent;

  EventIntFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventIntFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(s4 new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(s4 new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventIntFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     s4 oldValue,
                     s4 newValue,
                     u8 origin) {
    EventIntFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventUnsignedIntFlagChanged : public JfrEvent<EventUnsignedIntFlagChanged>
{
 private:
  const char* _name;
  unsigned _oldValue;
  unsigned _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrUnsignedIntFlagChangedEvent;

  EventUnsignedIntFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventUnsignedIntFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(unsigned new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(unsigned new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventUnsignedIntFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     unsigned oldValue,
                     unsigned newValue,
                     u8 origin) {
    EventUnsignedIntFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventLongFlagChanged : public JfrEvent<EventLongFlagChanged>
{
 private:
  const char* _name;
  s8 _oldValue;
  s8 _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrLongFlagChangedEvent;

  EventLongFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventLongFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(s8 new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(s8 new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventLongFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     s8 oldValue,
                     s8 newValue,
                     u8 origin) {
    EventLongFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventUnsignedLongFlagChanged : public JfrEvent<EventUnsignedLongFlagChanged>
{
 private:
  const char* _name;
  u8 _oldValue;
  u8 _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrUnsignedLongFlagChangedEvent;

  EventUnsignedLongFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventUnsignedLongFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(u8 new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(u8 new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventUnsignedLongFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     u8 oldValue,
                     u8 newValue,
                     u8 origin) {
    EventUnsignedLongFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventDoubleFlagChanged : public JfrEvent<EventDoubleFlagChanged>
{
 private:
  const char* _name;
  double _oldValue;
  double _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrDoubleFlagChangedEvent;

  EventDoubleFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventDoubleFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(double new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(double new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventDoubleFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     double oldValue,
                     double newValue,
                     u8 origin) {
    EventDoubleFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventBooleanFlagChanged : public JfrEvent<EventBooleanFlagChanged>
{
 private:
  const char* _name;
  bool _oldValue;
  bool _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrBooleanFlagChangedEvent;

  EventBooleanFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventBooleanFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(bool new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(bool new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventBooleanFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     bool oldValue,
                     bool newValue,
                     u8 origin) {
    EventBooleanFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventStringFlagChanged : public JfrEvent<EventStringFlagChanged>
{
 private:
  const char* _name;
  const char* _oldValue;
  const char* _newValue;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrStringFlagChangedEvent;

  EventStringFlagChanged(EventStartTime timing=TIMED) : JfrEvent<EventStringFlagChanged>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldValue(const char* new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newValue(const char* new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_origin);
  }

  using JfrEvent<EventStringFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     const char* oldValue,
                     const char* newValue,
                     u8 origin) {
    EventStringFlagChanged me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventGCHeapSummary : public JfrEvent<EventGCHeapSummary>
{
 private:
  unsigned _gcId;
  u8 _when;
  JfrStructVirtualSpace _heapSpace;
  u8 _heapUsed;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCHeapSummaryEvent;

  EventGCHeapSummary(EventStartTime timing=TIMED) : JfrEvent<EventGCHeapSummary>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_when(u8 new_value) {
    this->_when = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_heapSpace(const JfrStructVirtualSpace& value) {
    this->_heapSpace = value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_heapUsed(u8 new_value) {
    this->_heapUsed = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_when);
    _heapSpace.writeData(w);
    w.write(_heapUsed);
  }

  using JfrEvent<EventGCHeapSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     const JfrStructVirtualSpace& heapSpace,
                     u8 heapUsed) {
    EventGCHeapSummary me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_when(when);
      me.set_heapSpace(heapSpace);
      me.set_heapUsed(heapUsed);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_when");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_heapSpace");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_heapUsed");
  }
#endif
};

class EventGCHeapMemoryUsage : public JfrEvent<EventGCHeapMemoryUsage>
{
 private:
  u8 _used;
  u8 _committed;
  u8 _max;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrGCHeapMemoryUsageEvent;

  EventGCHeapMemoryUsage(EventStartTime timing=TIMED) : JfrEvent<EventGCHeapMemoryUsage>(timing) {}

  void set_used(u8 new_value) {
    this->_used = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_committed(u8 new_value) {
    this->_committed = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_max(u8 new_value) {
    this->_max = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_used);
    w.write(_committed);
    w.write(_max);
  }

  using JfrEvent<EventGCHeapMemoryUsage>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCHeapMemoryUsage(
    u8 used,
    u8 committed,
    u8 max) : JfrEvent<EventGCHeapMemoryUsage>(TIMED) {
    if (should_commit()) {
      set_used(used);
      set_committed(committed);
      set_max(max);
    }
  }

  void commit(u8 used,
              u8 committed,
              u8 max) {
    if (should_commit()) {
      set_used(used);
      set_committed(committed);
      set_max(max);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 used,
                     u8 committed,
                     u8 max) {
    EventGCHeapMemoryUsage me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_used(used);
      me.set_committed(committed);
      me.set_max(max);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_used");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_committed");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_max");
  }
#endif
};

class EventGCHeapMemoryPoolUsage : public JfrEvent<EventGCHeapMemoryPoolUsage>
{
 private:
  const char* _name;
  u8 _used;
  u8 _committed;
  u8 _max;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrGCHeapMemoryPoolUsageEvent;

  EventGCHeapMemoryPoolUsage(EventStartTime timing=TIMED) : JfrEvent<EventGCHeapMemoryPoolUsage>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_used(u8 new_value) {
    this->_used = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_committed(u8 new_value) {
    this->_committed = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_max(u8 new_value) {
    this->_max = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_used);
    w.write(_committed);
    w.write(_max);
  }

  using JfrEvent<EventGCHeapMemoryPoolUsage>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCHeapMemoryPoolUsage(
    const char* name,
    u8 used,
    u8 committed,
    u8 max) : JfrEvent<EventGCHeapMemoryPoolUsage>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_used(used);
      set_committed(committed);
      set_max(max);
    }
  }

  void commit(const char* name,
              u8 used,
              u8 committed,
              u8 max) {
    if (should_commit()) {
      set_name(name);
      set_used(used);
      set_committed(committed);
      set_max(max);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     u8 used,
                     u8 committed,
                     u8 max) {
    EventGCHeapMemoryPoolUsage me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_used(used);
      me.set_committed(committed);
      me.set_max(max);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_used");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_committed");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_max");
  }
#endif
};

class EventMetaspaceSummary : public JfrEvent<EventMetaspaceSummary>
{
 private:
  unsigned _gcId;
  u8 _when;
  u8 _gcThreshold;
  JfrStructMetaspaceSizes _metaspace;
  JfrStructMetaspaceSizes _dataSpace;
  JfrStructMetaspaceSizes _classSpace;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrMetaspaceSummaryEvent;

  EventMetaspaceSummary(EventStartTime timing=TIMED) : JfrEvent<EventMetaspaceSummary>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_when(u8 new_value) {
    this->_when = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_gcThreshold(u8 new_value) {
    this->_gcThreshold = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_metaspace(const JfrStructMetaspaceSizes& value) {
    this->_metaspace = value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_dataSpace(const JfrStructMetaspaceSizes& value) {
    this->_dataSpace = value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_classSpace(const JfrStructMetaspaceSizes& value) {
    this->_classSpace = value;
    DEBUG_ONLY(set_field_bit(5));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_when);
    w.write(_gcThreshold);
    _metaspace.writeData(w);
    _dataSpace.writeData(w);
    _classSpace.writeData(w);
  }

  using JfrEvent<EventMetaspaceSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     u8 gcThreshold,
                     const JfrStructMetaspaceSizes& metaspace,
                     const JfrStructMetaspaceSizes& dataSpace,
                     const JfrStructMetaspaceSizes& classSpace) {
    EventMetaspaceSummary me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_when(when);
      me.set_gcThreshold(gcThreshold);
      me.set_metaspace(metaspace);
      me.set_dataSpace(dataSpace);
      me.set_classSpace(classSpace);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_when");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_gcThreshold");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_metaspace");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_dataSpace");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_classSpace");
  }
#endif
};

class EventMetaspaceGCThreshold : public JfrEvent<EventMetaspaceGCThreshold>
{
 private:
  u8 _oldValue;
  u8 _newValue;
  u8 _updater;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrMetaspaceGCThresholdEvent;

  EventMetaspaceGCThreshold(EventStartTime timing=TIMED) : JfrEvent<EventMetaspaceGCThreshold>(timing) {}

  void set_oldValue(u8 new_value) {
    this->_oldValue = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_newValue(u8 new_value) {
    this->_newValue = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_updater(u8 new_value) {
    this->_updater = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_oldValue);
    w.write(_newValue);
    w.write(_updater);
  }

  using JfrEvent<EventMetaspaceGCThreshold>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 oldValue,
                     u8 newValue,
                     u8 updater) {
    EventMetaspaceGCThreshold me(UNTIMED);

    if (me.should_commit()) {
      me.set_oldValue(oldValue);
      me.set_newValue(newValue);
      me.set_updater(updater);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_oldValue");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_newValue");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_updater");
  }
#endif
};

class EventMetaspaceAllocationFailure : public JfrEvent<EventMetaspaceAllocationFailure>
{
 private:
  const ClassLoaderData* _classLoader;
  bool _hiddenClassLoader;
  u8 _size;
  u8 _metadataType;
  u8 _metaspaceObjectType;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrMetaspaceAllocationFailureEvent;

  EventMetaspaceAllocationFailure(EventStartTime timing=TIMED) : JfrEvent<EventMetaspaceAllocationFailure>(timing) {}

  void set_classLoader(const ClassLoaderData* new_value) {
    this->_classLoader = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_hiddenClassLoader(bool new_value) {
    this->_hiddenClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_metadataType(u8 new_value) {
    this->_metadataType = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_metaspaceObjectType(u8 new_value) {
    this->_metaspaceObjectType = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_classLoader);
    w.write(_hiddenClassLoader);
    w.write(_size);
    w.write(_metadataType);
    w.write(_metaspaceObjectType);
  }

  using JfrEvent<EventMetaspaceAllocationFailure>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const ClassLoaderData* classLoader,
                     bool hiddenClassLoader,
                     u8 size,
                     u8 metadataType,
                     u8 metaspaceObjectType) {
    EventMetaspaceAllocationFailure me(UNTIMED);

    if (me.should_commit()) {
      me.set_classLoader(classLoader);
      me.set_hiddenClassLoader(hiddenClassLoader);
      me.set_size(size);
      me.set_metadataType(metadataType);
      me.set_metaspaceObjectType(metaspaceObjectType);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_classLoader");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_hiddenClassLoader");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_size");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_metadataType");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_metaspaceObjectType");
  }
#endif
};

class EventMetaspaceOOM : public JfrEvent<EventMetaspaceOOM>
{
 private:
  const ClassLoaderData* _classLoader;
  bool _hiddenClassLoader;
  u8 _size;
  u8 _metadataType;
  u8 _metaspaceObjectType;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrMetaspaceOOMEvent;

  EventMetaspaceOOM(EventStartTime timing=TIMED) : JfrEvent<EventMetaspaceOOM>(timing) {}

  void set_classLoader(const ClassLoaderData* new_value) {
    this->_classLoader = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_hiddenClassLoader(bool new_value) {
    this->_hiddenClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_metadataType(u8 new_value) {
    this->_metadataType = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_metaspaceObjectType(u8 new_value) {
    this->_metaspaceObjectType = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_classLoader);
    w.write(_hiddenClassLoader);
    w.write(_size);
    w.write(_metadataType);
    w.write(_metaspaceObjectType);
  }

  using JfrEvent<EventMetaspaceOOM>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const ClassLoaderData* classLoader,
                     bool hiddenClassLoader,
                     u8 size,
                     u8 metadataType,
                     u8 metaspaceObjectType) {
    EventMetaspaceOOM me(UNTIMED);

    if (me.should_commit()) {
      me.set_classLoader(classLoader);
      me.set_hiddenClassLoader(hiddenClassLoader);
      me.set_size(size);
      me.set_metadataType(metadataType);
      me.set_metaspaceObjectType(metaspaceObjectType);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_classLoader");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_hiddenClassLoader");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_size");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_metadataType");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_metaspaceObjectType");
  }
#endif
};

class EventMetaspaceChunkFreeListSummary : public JfrEvent<EventMetaspaceChunkFreeListSummary>
{
 private:
  unsigned _gcId;
  u8 _when;
  u8 _metadataType;
  u8 _specializedChunks;
  u8 _specializedChunksTotalSize;
  u8 _smallChunks;
  u8 _smallChunksTotalSize;
  u8 _mediumChunks;
  u8 _mediumChunksTotalSize;
  u8 _humongousChunks;
  u8 _humongousChunksTotalSize;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrMetaspaceChunkFreeListSummaryEvent;

  EventMetaspaceChunkFreeListSummary(EventStartTime timing=TIMED) : JfrEvent<EventMetaspaceChunkFreeListSummary>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_when(u8 new_value) {
    this->_when = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_metadataType(u8 new_value) {
    this->_metadataType = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_specializedChunks(u8 new_value) {
    this->_specializedChunks = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_specializedChunksTotalSize(u8 new_value) {
    this->_specializedChunksTotalSize = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_smallChunks(u8 new_value) {
    this->_smallChunks = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_smallChunksTotalSize(u8 new_value) {
    this->_smallChunksTotalSize = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_mediumChunks(u8 new_value) {
    this->_mediumChunks = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_mediumChunksTotalSize(u8 new_value) {
    this->_mediumChunksTotalSize = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }
  void set_humongousChunks(u8 new_value) {
    this->_humongousChunks = new_value;
    DEBUG_ONLY(set_field_bit(9));
  }
  void set_humongousChunksTotalSize(u8 new_value) {
    this->_humongousChunksTotalSize = new_value;
    DEBUG_ONLY(set_field_bit(10));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_when);
    w.write(_metadataType);
    w.write(_specializedChunks);
    w.write(_specializedChunksTotalSize);
    w.write(_smallChunks);
    w.write(_smallChunksTotalSize);
    w.write(_mediumChunks);
    w.write(_mediumChunksTotalSize);
    w.write(_humongousChunks);
    w.write(_humongousChunksTotalSize);
  }

  using JfrEvent<EventMetaspaceChunkFreeListSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     u8 metadataType,
                     u8 specializedChunks,
                     u8 specializedChunksTotalSize,
                     u8 smallChunks,
                     u8 smallChunksTotalSize,
                     u8 mediumChunks,
                     u8 mediumChunksTotalSize,
                     u8 humongousChunks,
                     u8 humongousChunksTotalSize) {
    EventMetaspaceChunkFreeListSummary me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_when(when);
      me.set_metadataType(metadataType);
      me.set_specializedChunks(specializedChunks);
      me.set_specializedChunksTotalSize(specializedChunksTotalSize);
      me.set_smallChunks(smallChunks);
      me.set_smallChunksTotalSize(smallChunksTotalSize);
      me.set_mediumChunks(mediumChunks);
      me.set_mediumChunksTotalSize(mediumChunksTotalSize);
      me.set_humongousChunks(humongousChunks);
      me.set_humongousChunksTotalSize(humongousChunksTotalSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_when");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_metadataType");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_specializedChunks");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_specializedChunksTotalSize");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_smallChunks");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_smallChunksTotalSize");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_mediumChunks");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_mediumChunksTotalSize");
    assert(verify_field_bit(9), "Attempting to write an uninitialized event field: %s", "_humongousChunks");
    assert(verify_field_bit(10), "Attempting to write an uninitialized event field: %s", "_humongousChunksTotalSize");
  }
#endif
};

class EventPSHeapSummary : public JfrEvent<EventPSHeapSummary>
{
 private:
  unsigned _gcId;
  u8 _when;
  JfrStructVirtualSpace _oldSpace;
  JfrStructObjectSpace _oldObjectSpace;
  JfrStructVirtualSpace _youngSpace;
  JfrStructObjectSpace _edenSpace;
  JfrStructObjectSpace _fromSpace;
  JfrStructObjectSpace _toSpace;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrPSHeapSummaryEvent;

  EventPSHeapSummary(EventStartTime timing=TIMED) : JfrEvent<EventPSHeapSummary>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_when(u8 new_value) {
    this->_when = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_oldSpace(const JfrStructVirtualSpace& value) {
    this->_oldSpace = value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_oldObjectSpace(const JfrStructObjectSpace& value) {
    this->_oldObjectSpace = value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_youngSpace(const JfrStructVirtualSpace& value) {
    this->_youngSpace = value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_edenSpace(const JfrStructObjectSpace& value) {
    this->_edenSpace = value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_fromSpace(const JfrStructObjectSpace& value) {
    this->_fromSpace = value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_toSpace(const JfrStructObjectSpace& value) {
    this->_toSpace = value;
    DEBUG_ONLY(set_field_bit(7));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_when);
    _oldSpace.writeData(w);
    _oldObjectSpace.writeData(w);
    _youngSpace.writeData(w);
    _edenSpace.writeData(w);
    _fromSpace.writeData(w);
    _toSpace.writeData(w);
  }

  using JfrEvent<EventPSHeapSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     const JfrStructVirtualSpace& oldSpace,
                     const JfrStructObjectSpace& oldObjectSpace,
                     const JfrStructVirtualSpace& youngSpace,
                     const JfrStructObjectSpace& edenSpace,
                     const JfrStructObjectSpace& fromSpace,
                     const JfrStructObjectSpace& toSpace) {
    EventPSHeapSummary me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_when(when);
      me.set_oldSpace(oldSpace);
      me.set_oldObjectSpace(oldObjectSpace);
      me.set_youngSpace(youngSpace);
      me.set_edenSpace(edenSpace);
      me.set_fromSpace(fromSpace);
      me.set_toSpace(toSpace);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_when");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_oldSpace");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_oldObjectSpace");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_youngSpace");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_edenSpace");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_fromSpace");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_toSpace");
  }
#endif
};

class EventG1HeapSummary : public JfrEvent<EventG1HeapSummary>
{
 private:
  unsigned _gcId;
  u8 _when;
  u8 _edenUsedSize;
  u8 _edenTotalSize;
  u8 _survivorUsedSize;
  u8 _oldGenUsedSize;
  unsigned _numberOfRegions;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1HeapSummaryEvent;

  EventG1HeapSummary(EventStartTime timing=TIMED) : JfrEvent<EventG1HeapSummary>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_when(u8 new_value) {
    this->_when = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_edenUsedSize(u8 new_value) {
    this->_edenUsedSize = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_edenTotalSize(u8 new_value) {
    this->_edenTotalSize = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_survivorUsedSize(u8 new_value) {
    this->_survivorUsedSize = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_oldGenUsedSize(u8 new_value) {
    this->_oldGenUsedSize = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_numberOfRegions(unsigned new_value) {
    this->_numberOfRegions = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_when);
    w.write(_edenUsedSize);
    w.write(_edenTotalSize);
    w.write(_survivorUsedSize);
    w.write(_oldGenUsedSize);
    w.write(_numberOfRegions);
  }

  using JfrEvent<EventG1HeapSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     u8 edenUsedSize,
                     u8 edenTotalSize,
                     u8 survivorUsedSize,
                     u8 oldGenUsedSize,
                     unsigned numberOfRegions) {
    EventG1HeapSummary me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_when(when);
      me.set_edenUsedSize(edenUsedSize);
      me.set_edenTotalSize(edenTotalSize);
      me.set_survivorUsedSize(survivorUsedSize);
      me.set_oldGenUsedSize(oldGenUsedSize);
      me.set_numberOfRegions(numberOfRegions);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_when");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_edenUsedSize");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_edenTotalSize");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_survivorUsedSize");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_oldGenUsedSize");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_numberOfRegions");
  }
#endif
};

class EventGarbageCollection : public JfrEvent<EventGarbageCollection>
{
 private:
  unsigned _gcId;
  u8 _name;
  u8 _cause;
  Tickspan _sumOfPauses;
  Tickspan _longestPause;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGarbageCollectionEvent;

  EventGarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventGarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(u8 new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_cause(u8 new_value) {
    this->_cause = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_sumOfPauses(const Tickspan& new_value) {
    this->_sumOfPauses = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_longestPause(const Tickspan& new_value) {
    this->_longestPause = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
    w.write(_cause);
    w.write(_sumOfPauses);
    w.write(_longestPause);
  }

  using JfrEvent<EventGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventGarbageCollection(
    unsigned gcId,
    u8 name,
    u8 cause,
    const Tickspan& sumOfPauses,
    const Tickspan& longestPause) : JfrEvent<EventGarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      set_cause(cause);
      set_sumOfPauses(sumOfPauses);
      set_longestPause(longestPause);
    }
  }

  void commit(unsigned gcId,
              u8 name,
              u8 cause,
              const Tickspan& sumOfPauses,
              const Tickspan& longestPause) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      set_cause(cause);
      set_sumOfPauses(sumOfPauses);
      set_longestPause(longestPause);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     u8 name,
                     u8 cause,
                     const Tickspan& sumOfPauses,
                     const Tickspan& longestPause) {
    EventGarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.set_cause(cause);
      me.set_sumOfPauses(sumOfPauses);
      me.set_longestPause(longestPause);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_cause");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_sumOfPauses");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_longestPause");
  }
#endif
};

class EventSystemGC : public JfrEvent<EventSystemGC>
{
 private:
  bool _invokedConcurrent;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrSystemGCEvent;

  EventSystemGC(EventStartTime timing=TIMED) : JfrEvent<EventSystemGC>(timing) {}

  void set_invokedConcurrent(bool new_value) {
    this->_invokedConcurrent = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_invokedConcurrent);
  }

  using JfrEvent<EventSystemGC>::commit; // else commit() is hidden by overloaded versions in this class

  EventSystemGC(
    bool invokedConcurrent) : JfrEvent<EventSystemGC>(TIMED) {
    if (should_commit()) {
      set_invokedConcurrent(invokedConcurrent);
    }
  }

  void commit(bool invokedConcurrent) {
    if (should_commit()) {
      set_invokedConcurrent(invokedConcurrent);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     bool invokedConcurrent) {
    EventSystemGC me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_invokedConcurrent(invokedConcurrent);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_invokedConcurrent");
  }
#endif
};

class EventParallelOldGarbageCollection : public JfrEvent<EventParallelOldGarbageCollection>
{
 private:
  unsigned _gcId;
  u8 _densePrefix;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrParallelOldGarbageCollectionEvent;

  EventParallelOldGarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventParallelOldGarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_densePrefix(u8 new_value) {
    this->_densePrefix = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_densePrefix);
  }

  using JfrEvent<EventParallelOldGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventParallelOldGarbageCollection(
    unsigned gcId,
    u8 densePrefix) : JfrEvent<EventParallelOldGarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_densePrefix(densePrefix);
    }
  }

  void commit(unsigned gcId,
              u8 densePrefix) {
    if (should_commit()) {
      set_gcId(gcId);
      set_densePrefix(densePrefix);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     u8 densePrefix) {
    EventParallelOldGarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_densePrefix(densePrefix);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_densePrefix");
  }
#endif
};

class EventYoungGarbageCollection : public JfrEvent<EventYoungGarbageCollection>
{
 private:
  unsigned _gcId;
  unsigned _tenuringThreshold;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrYoungGarbageCollectionEvent;

  EventYoungGarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventYoungGarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_tenuringThreshold(unsigned new_value) {
    this->_tenuringThreshold = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_tenuringThreshold);
  }

  using JfrEvent<EventYoungGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventYoungGarbageCollection(
    unsigned gcId,
    unsigned tenuringThreshold) : JfrEvent<EventYoungGarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_tenuringThreshold(tenuringThreshold);
    }
  }

  void commit(unsigned gcId,
              unsigned tenuringThreshold) {
    if (should_commit()) {
      set_gcId(gcId);
      set_tenuringThreshold(tenuringThreshold);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     unsigned tenuringThreshold) {
    EventYoungGarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_tenuringThreshold(tenuringThreshold);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_tenuringThreshold");
  }
#endif
};

class EventOldGarbageCollection : public JfrEvent<EventOldGarbageCollection>
{
 private:
  unsigned _gcId;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrOldGarbageCollectionEvent;

  EventOldGarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventOldGarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
  }

  using JfrEvent<EventOldGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventOldGarbageCollection(
    unsigned gcId) : JfrEvent<EventOldGarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
    }
  }

  void commit(unsigned gcId) {
    if (should_commit()) {
      set_gcId(gcId);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId) {
    EventOldGarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
  }
#endif
};

class EventG1GarbageCollection : public JfrEvent<EventG1GarbageCollection>
{
 private:
  unsigned _gcId;
  u8 _type;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1GarbageCollectionEvent;

  EventG1GarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventG1GarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_type);
  }

  using JfrEvent<EventG1GarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventG1GarbageCollection(
    unsigned gcId,
    u8 type) : JfrEvent<EventG1GarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_type(type);
    }
  }

  void commit(unsigned gcId,
              u8 type) {
    if (should_commit()) {
      set_gcId(gcId);
      set_type(type);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     u8 type) {
    EventG1GarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_type(type);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_type");
  }
#endif
};

class EventG1MMU : public JfrEvent<EventG1MMU>
{
 private:
  unsigned _gcId;
  s8 _timeSlice;
  s8 _gcTime;
  s8 _pauseTarget;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1MMUEvent;

  EventG1MMU(EventStartTime timing=TIMED) : JfrEvent<EventG1MMU>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_timeSlice(s8 new_value) {
    this->_timeSlice = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_gcTime(s8 new_value) {
    this->_gcTime = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_pauseTarget(s8 new_value) {
    this->_pauseTarget = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_timeSlice);
    w.write(_gcTime);
    w.write(_pauseTarget);
  }

  using JfrEvent<EventG1MMU>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     s8 timeSlice,
                     s8 gcTime,
                     s8 pauseTarget) {
    EventG1MMU me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_timeSlice(timeSlice);
      me.set_gcTime(gcTime);
      me.set_pauseTarget(pauseTarget);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_timeSlice");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_gcTime");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_pauseTarget");
  }
#endif
};

class EventEvacuationInformation : public JfrEvent<EventEvacuationInformation>
{
 private:
  unsigned _gcId;
  unsigned _cSetRegions;
  u8 _cSetUsedBefore;
  u8 _cSetUsedAfter;
  unsigned _allocationRegions;
  u8 _allocationRegionsUsedBefore;
  u8 _allocationRegionsUsedAfter;
  u8 _bytesCopied;
  unsigned _regionsFreed;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrEvacuationInformationEvent;

  EventEvacuationInformation(EventStartTime timing=TIMED) : JfrEvent<EventEvacuationInformation>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_cSetRegions(unsigned new_value) {
    this->_cSetRegions = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_cSetUsedBefore(u8 new_value) {
    this->_cSetUsedBefore = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_cSetUsedAfter(u8 new_value) {
    this->_cSetUsedAfter = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_allocationRegions(unsigned new_value) {
    this->_allocationRegions = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_allocationRegionsUsedBefore(u8 new_value) {
    this->_allocationRegionsUsedBefore = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_allocationRegionsUsedAfter(u8 new_value) {
    this->_allocationRegionsUsedAfter = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_bytesCopied(u8 new_value) {
    this->_bytesCopied = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_regionsFreed(unsigned new_value) {
    this->_regionsFreed = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_cSetRegions);
    w.write(_cSetUsedBefore);
    w.write(_cSetUsedAfter);
    w.write(_allocationRegions);
    w.write(_allocationRegionsUsedBefore);
    w.write(_allocationRegionsUsedAfter);
    w.write(_bytesCopied);
    w.write(_regionsFreed);
  }

  using JfrEvent<EventEvacuationInformation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     unsigned cSetRegions,
                     u8 cSetUsedBefore,
                     u8 cSetUsedAfter,
                     unsigned allocationRegions,
                     u8 allocationRegionsUsedBefore,
                     u8 allocationRegionsUsedAfter,
                     u8 bytesCopied,
                     unsigned regionsFreed) {
    EventEvacuationInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_cSetRegions(cSetRegions);
      me.set_cSetUsedBefore(cSetUsedBefore);
      me.set_cSetUsedAfter(cSetUsedAfter);
      me.set_allocationRegions(allocationRegions);
      me.set_allocationRegionsUsedBefore(allocationRegionsUsedBefore);
      me.set_allocationRegionsUsedAfter(allocationRegionsUsedAfter);
      me.set_bytesCopied(bytesCopied);
      me.set_regionsFreed(regionsFreed);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_cSetRegions");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_cSetUsedBefore");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_cSetUsedAfter");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_allocationRegions");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_allocationRegionsUsedBefore");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_allocationRegionsUsedAfter");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_bytesCopied");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_regionsFreed");
  }
#endif
};

class EventGCReferenceStatistics : public JfrEvent<EventGCReferenceStatistics>
{
 private:
  unsigned _gcId;
  u8 _type;
  u8 _count;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCReferenceStatisticsEvent;

  EventGCReferenceStatistics(EventStartTime timing=TIMED) : JfrEvent<EventGCReferenceStatistics>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_count(u8 new_value) {
    this->_count = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_type);
    w.write(_count);
  }

  using JfrEvent<EventGCReferenceStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 type,
                     u8 count) {
    EventGCReferenceStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_type(type);
      me.set_count(count);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_type");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_count");
  }
#endif
};

class EventObjectCountAfterGC : public JfrEvent<EventObjectCountAfterGC>
{
 private:
  unsigned _gcId;
  const Klass* _objectClass;
  s8 _count;
  u8 _totalSize;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrObjectCountAfterGCEvent;

  EventObjectCountAfterGC(EventStartTime timing=TIMED) : JfrEvent<EventObjectCountAfterGC>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_count(s8 new_value) {
    this->_count = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_totalSize(u8 new_value) {
    this->_totalSize = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_objectClass);
    w.write(_count);
    w.write(_totalSize);
  }

  using JfrEvent<EventObjectCountAfterGC>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     s8 count,
                     u8 totalSize) {
    EventObjectCountAfterGC me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_objectClass(objectClass);
      me.set_count(count);
      me.set_totalSize(totalSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_count");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_totalSize");
  }
#endif
};

class EventG1EvacuationYoungStatistics : public JfrEvent<EventG1EvacuationYoungStatistics>
{
 private:
  JfrStructG1EvacuationStatistics _statistics;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1EvacuationYoungStatisticsEvent;

  EventG1EvacuationYoungStatistics(EventStartTime timing=TIMED) : JfrEvent<EventG1EvacuationYoungStatistics>(timing) {}

  void set_statistics(const JfrStructG1EvacuationStatistics& value) {
    this->_statistics = value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    _statistics.writeData(w);
  }

  using JfrEvent<EventG1EvacuationYoungStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const JfrStructG1EvacuationStatistics& statistics) {
    EventG1EvacuationYoungStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_statistics(statistics);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_statistics");
  }
#endif
};

class EventG1EvacuationOldStatistics : public JfrEvent<EventG1EvacuationOldStatistics>
{
 private:
  JfrStructG1EvacuationStatistics _statistics;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1EvacuationOldStatisticsEvent;

  EventG1EvacuationOldStatistics(EventStartTime timing=TIMED) : JfrEvent<EventG1EvacuationOldStatistics>(timing) {}

  void set_statistics(const JfrStructG1EvacuationStatistics& value) {
    this->_statistics = value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    _statistics.writeData(w);
  }

  using JfrEvent<EventG1EvacuationOldStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const JfrStructG1EvacuationStatistics& statistics) {
    EventG1EvacuationOldStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_statistics(statistics);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_statistics");
  }
#endif
};

class EventG1BasicIHOP : public JfrEvent<EventG1BasicIHOP>
{
 private:
  unsigned _gcId;
  u8 _threshold;
  float _thresholdPercentage;
  u8 _targetOccupancy;
  u8 _currentOccupancy;
  u8 _recentMutatorAllocationSize;
  s8 _recentMutatorDuration;
  double _recentAllocationRate;
  s8 _lastMarkingDuration;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1BasicIHOPEvent;

  EventG1BasicIHOP(EventStartTime timing=TIMED) : JfrEvent<EventG1BasicIHOP>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_threshold(u8 new_value) {
    this->_threshold = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_thresholdPercentage(float new_value) {
    this->_thresholdPercentage = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_targetOccupancy(u8 new_value) {
    this->_targetOccupancy = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_currentOccupancy(u8 new_value) {
    this->_currentOccupancy = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_recentMutatorAllocationSize(u8 new_value) {
    this->_recentMutatorAllocationSize = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_recentMutatorDuration(s8 new_value) {
    this->_recentMutatorDuration = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_recentAllocationRate(double new_value) {
    this->_recentAllocationRate = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_lastMarkingDuration(s8 new_value) {
    this->_lastMarkingDuration = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_threshold);
    w.write(_thresholdPercentage);
    w.write(_targetOccupancy);
    w.write(_currentOccupancy);
    w.write(_recentMutatorAllocationSize);
    w.write(_recentMutatorDuration);
    w.write(_recentAllocationRate);
    w.write(_lastMarkingDuration);
  }

  using JfrEvent<EventG1BasicIHOP>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 threshold,
                     float thresholdPercentage,
                     u8 targetOccupancy,
                     u8 currentOccupancy,
                     u8 recentMutatorAllocationSize,
                     s8 recentMutatorDuration,
                     double recentAllocationRate,
                     s8 lastMarkingDuration) {
    EventG1BasicIHOP me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_threshold(threshold);
      me.set_thresholdPercentage(thresholdPercentage);
      me.set_targetOccupancy(targetOccupancy);
      me.set_currentOccupancy(currentOccupancy);
      me.set_recentMutatorAllocationSize(recentMutatorAllocationSize);
      me.set_recentMutatorDuration(recentMutatorDuration);
      me.set_recentAllocationRate(recentAllocationRate);
      me.set_lastMarkingDuration(lastMarkingDuration);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_threshold");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_thresholdPercentage");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_targetOccupancy");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_currentOccupancy");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_recentMutatorAllocationSize");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_recentMutatorDuration");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_recentAllocationRate");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_lastMarkingDuration");
  }
#endif
};

class EventG1AdaptiveIHOP : public JfrEvent<EventG1AdaptiveIHOP>
{
 private:
  unsigned _gcId;
  u8 _threshold;
  float _thresholdPercentage;
  u8 _ihopTargetOccupancy;
  u8 _currentOccupancy;
  u8 _additionalBufferSize;
  double _predictedAllocationRate;
  s8 _predictedMarkingDuration;
  bool _predictionActive;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1AdaptiveIHOPEvent;

  EventG1AdaptiveIHOP(EventStartTime timing=TIMED) : JfrEvent<EventG1AdaptiveIHOP>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_threshold(u8 new_value) {
    this->_threshold = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_thresholdPercentage(float new_value) {
    this->_thresholdPercentage = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_ihopTargetOccupancy(u8 new_value) {
    this->_ihopTargetOccupancy = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_currentOccupancy(u8 new_value) {
    this->_currentOccupancy = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_additionalBufferSize(u8 new_value) {
    this->_additionalBufferSize = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_predictedAllocationRate(double new_value) {
    this->_predictedAllocationRate = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_predictedMarkingDuration(s8 new_value) {
    this->_predictedMarkingDuration = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_predictionActive(bool new_value) {
    this->_predictionActive = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_threshold);
    w.write(_thresholdPercentage);
    w.write(_ihopTargetOccupancy);
    w.write(_currentOccupancy);
    w.write(_additionalBufferSize);
    w.write(_predictedAllocationRate);
    w.write(_predictedMarkingDuration);
    w.write(_predictionActive);
  }

  using JfrEvent<EventG1AdaptiveIHOP>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 threshold,
                     float thresholdPercentage,
                     u8 ihopTargetOccupancy,
                     u8 currentOccupancy,
                     u8 additionalBufferSize,
                     double predictedAllocationRate,
                     s8 predictedMarkingDuration,
                     bool predictionActive) {
    EventG1AdaptiveIHOP me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_threshold(threshold);
      me.set_thresholdPercentage(thresholdPercentage);
      me.set_ihopTargetOccupancy(ihopTargetOccupancy);
      me.set_currentOccupancy(currentOccupancy);
      me.set_additionalBufferSize(additionalBufferSize);
      me.set_predictedAllocationRate(predictedAllocationRate);
      me.set_predictedMarkingDuration(predictedMarkingDuration);
      me.set_predictionActive(predictionActive);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_threshold");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_thresholdPercentage");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_ihopTargetOccupancy");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_currentOccupancy");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_additionalBufferSize");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_predictedAllocationRate");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_predictedMarkingDuration");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_predictionActive");
  }
#endif
};

class EventPromoteObjectInNewPLAB : public JfrEvent<EventPromoteObjectInNewPLAB>
{
 private:
  unsigned _gcId;
  const Klass* _objectClass;
  u8 _objectSize;
  unsigned _tenuringAge;
  bool _tenured;
  u8 _plabSize;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrPromoteObjectInNewPLABEvent;

  EventPromoteObjectInNewPLAB(EventStartTime timing=TIMED) : JfrEvent<EventPromoteObjectInNewPLAB>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_objectSize(u8 new_value) {
    this->_objectSize = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_tenuringAge(unsigned new_value) {
    this->_tenuringAge = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_tenured(bool new_value) {
    this->_tenured = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_plabSize(u8 new_value) {
    this->_plabSize = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_objectClass);
    w.write(_objectSize);
    w.write(_tenuringAge);
    w.write(_tenured);
    w.write(_plabSize);
  }

  using JfrEvent<EventPromoteObjectInNewPLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     u8 objectSize,
                     unsigned tenuringAge,
                     bool tenured,
                     u8 plabSize) {
    EventPromoteObjectInNewPLAB me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_objectClass(objectClass);
      me.set_objectSize(objectSize);
      me.set_tenuringAge(tenuringAge);
      me.set_tenured(tenured);
      me.set_plabSize(plabSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_objectSize");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_tenuringAge");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_tenured");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_plabSize");
  }
#endif
};

class EventPromoteObjectOutsidePLAB : public JfrEvent<EventPromoteObjectOutsidePLAB>
{
 private:
  unsigned _gcId;
  const Klass* _objectClass;
  u8 _objectSize;
  unsigned _tenuringAge;
  bool _tenured;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrPromoteObjectOutsidePLABEvent;

  EventPromoteObjectOutsidePLAB(EventStartTime timing=TIMED) : JfrEvent<EventPromoteObjectOutsidePLAB>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_objectSize(u8 new_value) {
    this->_objectSize = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_tenuringAge(unsigned new_value) {
    this->_tenuringAge = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_tenured(bool new_value) {
    this->_tenured = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_objectClass);
    w.write(_objectSize);
    w.write(_tenuringAge);
    w.write(_tenured);
  }

  using JfrEvent<EventPromoteObjectOutsidePLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     u8 objectSize,
                     unsigned tenuringAge,
                     bool tenured) {
    EventPromoteObjectOutsidePLAB me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_objectClass(objectClass);
      me.set_objectSize(objectSize);
      me.set_tenuringAge(tenuringAge);
      me.set_tenured(tenured);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_objectSize");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_tenuringAge");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_tenured");
  }
#endif
};

class EventPromotionFailed : public JfrEvent<EventPromotionFailed>
{
 private:
  unsigned _gcId;
  JfrStructCopyFailed _promotionFailed;
  u8 _thread;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrPromotionFailedEvent;

  EventPromotionFailed(EventStartTime timing=TIMED) : JfrEvent<EventPromotionFailed>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_promotionFailed(const JfrStructCopyFailed& value) {
    this->_promotionFailed = value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_thread(u8 new_value) {
    this->_thread = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    _promotionFailed.writeData(w);
    w.write(_thread);
  }

  using JfrEvent<EventPromotionFailed>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const JfrStructCopyFailed& promotionFailed,
                     u8 thread) {
    EventPromotionFailed me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_promotionFailed(promotionFailed);
      me.set_thread(thread);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_promotionFailed");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_thread");
  }
#endif
};

class EventEvacuationFailed : public JfrEvent<EventEvacuationFailed>
{
 private:
  unsigned _gcId;
  JfrStructCopyFailed _evacuationFailed;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrEvacuationFailedEvent;

  EventEvacuationFailed(EventStartTime timing=TIMED) : JfrEvent<EventEvacuationFailed>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_evacuationFailed(const JfrStructCopyFailed& value) {
    this->_evacuationFailed = value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    _evacuationFailed.writeData(w);
  }

  using JfrEvent<EventEvacuationFailed>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const JfrStructCopyFailed& evacuationFailed) {
    EventEvacuationFailed me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_evacuationFailed(evacuationFailed);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_evacuationFailed");
  }
#endif
};

class EventConcurrentModeFailure : public JfrEvent<EventConcurrentModeFailure>
{
 private:
  unsigned _gcId;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrConcurrentModeFailureEvent;

  EventConcurrentModeFailure(EventStartTime timing=TIMED) : JfrEvent<EventConcurrentModeFailure>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
  }

  using JfrEvent<EventConcurrentModeFailure>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId) {
    EventConcurrentModeFailure me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
  }
#endif
};

class EventGCCPUTime : public JfrEvent<EventGCCPUTime>
{
 private:
  unsigned _gcId;
  u8 _userTime;
  u8 _systemTime;
  u8 _realTime;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCCPUTimeEvent;

  EventGCCPUTime(EventStartTime timing=TIMED) : JfrEvent<EventGCCPUTime>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_userTime(u8 new_value) {
    this->_userTime = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_systemTime(u8 new_value) {
    this->_systemTime = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_realTime(u8 new_value) {
    this->_realTime = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_userTime);
    w.write(_systemTime);
    w.write(_realTime);
  }

  using JfrEvent<EventGCCPUTime>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 userTime,
                     u8 systemTime,
                     u8 realTime) {
    EventGCCPUTime me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_userTime(userTime);
      me.set_systemTime(systemTime);
      me.set_realTime(realTime);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_userTime");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_systemTime");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_realTime");
  }
#endif
};

class EventGCPhasePause : public JfrEvent<EventGCPhasePause>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhasePauseEvent;

  EventGCPhasePause(EventStartTime timing=TIMED) : JfrEvent<EventGCPhasePause>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhasePause>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePause(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhasePause>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhasePause me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhasePauseLevel1 : public JfrEvent<EventGCPhasePauseLevel1>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhasePauseLevel1Event;

  EventGCPhasePauseLevel1(EventStartTime timing=TIMED) : JfrEvent<EventGCPhasePauseLevel1>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhasePauseLevel1>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel1(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhasePauseLevel1>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhasePauseLevel1 me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhasePauseLevel2 : public JfrEvent<EventGCPhasePauseLevel2>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhasePauseLevel2Event;

  EventGCPhasePauseLevel2(EventStartTime timing=TIMED) : JfrEvent<EventGCPhasePauseLevel2>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhasePauseLevel2>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel2(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhasePauseLevel2>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhasePauseLevel2 me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhasePauseLevel3 : public JfrEvent<EventGCPhasePauseLevel3>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhasePauseLevel3Event;

  EventGCPhasePauseLevel3(EventStartTime timing=TIMED) : JfrEvent<EventGCPhasePauseLevel3>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhasePauseLevel3>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel3(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhasePauseLevel3>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhasePauseLevel3 me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhasePauseLevel4 : public JfrEvent<EventGCPhasePauseLevel4>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhasePauseLevel4Event;

  EventGCPhasePauseLevel4(EventStartTime timing=TIMED) : JfrEvent<EventGCPhasePauseLevel4>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhasePauseLevel4>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel4(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhasePauseLevel4>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhasePauseLevel4 me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhaseConcurrent : public JfrEvent<EventGCPhaseConcurrent>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhaseConcurrentEvent;

  EventGCPhaseConcurrent(EventStartTime timing=TIMED) : JfrEvent<EventGCPhaseConcurrent>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhaseConcurrent>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseConcurrent(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhaseConcurrent>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhaseConcurrent me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhaseConcurrentLevel1 : public JfrEvent<EventGCPhaseConcurrentLevel1>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhaseConcurrentLevel1Event;

  EventGCPhaseConcurrentLevel1(EventStartTime timing=TIMED) : JfrEvent<EventGCPhaseConcurrentLevel1>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhaseConcurrentLevel1>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseConcurrentLevel1(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhaseConcurrentLevel1>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhaseConcurrentLevel1 me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhaseConcurrentLevel2 : public JfrEvent<EventGCPhaseConcurrentLevel2>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhaseConcurrentLevel2Event;

  EventGCPhaseConcurrentLevel2(EventStartTime timing=TIMED) : JfrEvent<EventGCPhaseConcurrentLevel2>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhaseConcurrentLevel2>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseConcurrentLevel2(
    unsigned gcId,
    const char* name) : JfrEvent<EventGCPhaseConcurrentLevel2>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventGCPhaseConcurrentLevel2 me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventGCPhaseParallel : public JfrEvent<EventGCPhaseParallel>
{
 private:
  unsigned _gcId;
  unsigned _gcWorkerId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrGCPhaseParallelEvent;

  EventGCPhaseParallel(EventStartTime timing=TIMED) : JfrEvent<EventGCPhaseParallel>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_gcWorkerId(unsigned new_value) {
    this->_gcWorkerId = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_gcWorkerId);
    w.write(_name);
  }

  using JfrEvent<EventGCPhaseParallel>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseParallel(
    unsigned gcId,
    unsigned gcWorkerId,
    const char* name) : JfrEvent<EventGCPhaseParallel>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_gcWorkerId(gcWorkerId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              unsigned gcWorkerId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_gcWorkerId(gcWorkerId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     unsigned gcWorkerId,
                     const char* name) {
    EventGCPhaseParallel me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_gcWorkerId(gcWorkerId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_gcWorkerId");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventAllocationRequiringGC : public JfrEvent<EventAllocationRequiringGC>
{
 private:
  unsigned _gcId;
  u8 _size;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrAllocationRequiringGCEvent;

  EventAllocationRequiringGC(EventStartTime timing=TIMED) : JfrEvent<EventAllocationRequiringGC>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_size);
  }

  using JfrEvent<EventAllocationRequiringGC>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 size) {
    EventAllocationRequiringGC me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventTenuringDistribution : public JfrEvent<EventTenuringDistribution>
{
 private:
  unsigned _gcId;
  unsigned _age;
  u8 _size;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrTenuringDistributionEvent;

  EventTenuringDistribution(EventStartTime timing=TIMED) : JfrEvent<EventTenuringDistribution>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_age(unsigned new_value) {
    this->_age = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_age);
    w.write(_size);
  }

  using JfrEvent<EventTenuringDistribution>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     unsigned age,
                     u8 size) {
    EventTenuringDistribution me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_age(age);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_age");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventG1HeapRegionTypeChange : public JfrEvent<EventG1HeapRegionTypeChange>
{
 private:
  unsigned _index;
  u8 _from;
  u8 _to;
  u8 _start;
  u8 _used;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrG1HeapRegionTypeChangeEvent;

  EventG1HeapRegionTypeChange(EventStartTime timing=TIMED) : JfrEvent<EventG1HeapRegionTypeChange>(timing) {}

  void set_index(unsigned new_value) {
    this->_index = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_from(u8 new_value) {
    this->_from = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_to(u8 new_value) {
    this->_to = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_start(u8 new_value) {
    this->_start = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_used(u8 new_value) {
    this->_used = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_index);
    w.write(_from);
    w.write(_to);
    w.write(_start);
    w.write(_used);
  }

  using JfrEvent<EventG1HeapRegionTypeChange>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned index,
                     u8 from,
                     u8 to,
                     u8 start,
                     u8 used) {
    EventG1HeapRegionTypeChange me(UNTIMED);

    if (me.should_commit()) {
      me.set_index(index);
      me.set_from(from);
      me.set_to(to);
      me.set_start(start);
      me.set_used(used);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_index");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_from");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_to");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_start");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_used");
  }
#endif
};

class EventJITRestart : public JfrEvent<EventJITRestart>
{
 private:
  u8 _freedMemory;
  u8 _codeCacheMaxCapacity;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrJITRestartEvent;

  EventJITRestart(EventStartTime timing=TIMED) : JfrEvent<EventJITRestart>(timing) {}

  void set_freedMemory(u8 new_value) {
    this->_freedMemory = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_codeCacheMaxCapacity(u8 new_value) {
    this->_codeCacheMaxCapacity = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_freedMemory);
    w.write(_codeCacheMaxCapacity);
  }

  using JfrEvent<EventJITRestart>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 freedMemory,
                     u8 codeCacheMaxCapacity) {
    EventJITRestart me(UNTIMED);

    if (me.should_commit()) {
      me.set_freedMemory(freedMemory);
      me.set_codeCacheMaxCapacity(codeCacheMaxCapacity);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_freedMemory");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_codeCacheMaxCapacity");
  }
#endif
};

class EventCompilation : public JfrEvent<EventCompilation>
{
 private:
  s4 _compileId;
  u8 _compiler;
  const Method* _method;
  u2 _compileLevel;
  bool _succeded;
  bool _isOsr;
  u8 _codeSize;
  u8 _inlinedBytes;
  u8 _arenaBytes;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCompilationEvent;

  EventCompilation(EventStartTime timing=TIMED) : JfrEvent<EventCompilation>(timing) {}

  void set_compileId(s4 new_value) {
    this->_compileId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_compiler(u8 new_value) {
    this->_compiler = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_method(const Method* new_value) {
    this->_method = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_compileLevel(u2 new_value) {
    this->_compileLevel = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_succeded(bool new_value) {
    this->_succeded = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_isOsr(bool new_value) {
    this->_isOsr = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_codeSize(u8 new_value) {
    this->_codeSize = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_inlinedBytes(u8 new_value) {
    this->_inlinedBytes = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_arenaBytes(u8 new_value) {
    this->_arenaBytes = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_compileId);
    w.write(_compiler);
    w.write(_method);
    w.write(_compileLevel);
    w.write(_succeded);
    w.write(_isOsr);
    w.write(_codeSize);
    w.write(_inlinedBytes);
    w.write(_arenaBytes);
  }

  using JfrEvent<EventCompilation>::commit; // else commit() is hidden by overloaded versions in this class

  EventCompilation(
    s4 compileId,
    u8 compiler,
    const Method* method,
    u2 compileLevel,
    bool succeded,
    bool isOsr,
    u8 codeSize,
    u8 inlinedBytes,
    u8 arenaBytes) : JfrEvent<EventCompilation>(TIMED) {
    if (should_commit()) {
      set_compileId(compileId);
      set_compiler(compiler);
      set_method(method);
      set_compileLevel(compileLevel);
      set_succeded(succeded);
      set_isOsr(isOsr);
      set_codeSize(codeSize);
      set_inlinedBytes(inlinedBytes);
      set_arenaBytes(arenaBytes);
    }
  }

  void commit(s4 compileId,
              u8 compiler,
              const Method* method,
              u2 compileLevel,
              bool succeded,
              bool isOsr,
              u8 codeSize,
              u8 inlinedBytes,
              u8 arenaBytes) {
    if (should_commit()) {
      set_compileId(compileId);
      set_compiler(compiler);
      set_method(method);
      set_compileLevel(compileLevel);
      set_succeded(succeded);
      set_isOsr(isOsr);
      set_codeSize(codeSize);
      set_inlinedBytes(inlinedBytes);
      set_arenaBytes(arenaBytes);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s4 compileId,
                     u8 compiler,
                     const Method* method,
                     u2 compileLevel,
                     bool succeded,
                     bool isOsr,
                     u8 codeSize,
                     u8 inlinedBytes,
                     u8 arenaBytes) {
    EventCompilation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_compileId(compileId);
      me.set_compiler(compiler);
      me.set_method(method);
      me.set_compileLevel(compileLevel);
      me.set_succeded(succeded);
      me.set_isOsr(isOsr);
      me.set_codeSize(codeSize);
      me.set_inlinedBytes(inlinedBytes);
      me.set_arenaBytes(arenaBytes);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_compileId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_compiler");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_method");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_compileLevel");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_succeded");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_isOsr");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_codeSize");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_inlinedBytes");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_arenaBytes");
  }
#endif
};

class EventCompilerPhase : public JfrEvent<EventCompilerPhase>
{
 private:
  u8 _phase;
  s4 _compileId;
  u2 _phaseLevel;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCompilerPhaseEvent;

  EventCompilerPhase(EventStartTime timing=TIMED) : JfrEvent<EventCompilerPhase>(timing) {}

  void set_phase(u8 new_value) {
    this->_phase = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_compileId(s4 new_value) {
    this->_compileId = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_phaseLevel(u2 new_value) {
    this->_phaseLevel = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_phase);
    w.write(_compileId);
    w.write(_phaseLevel);
  }

  using JfrEvent<EventCompilerPhase>::commit; // else commit() is hidden by overloaded versions in this class

  EventCompilerPhase(
    u8 phase,
    s4 compileId,
    u2 phaseLevel) : JfrEvent<EventCompilerPhase>(TIMED) {
    if (should_commit()) {
      set_phase(phase);
      set_compileId(compileId);
      set_phaseLevel(phaseLevel);
    }
  }

  void commit(u8 phase,
              s4 compileId,
              u2 phaseLevel) {
    if (should_commit()) {
      set_phase(phase);
      set_compileId(compileId);
      set_phaseLevel(phaseLevel);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 phase,
                     s4 compileId,
                     u2 phaseLevel) {
    EventCompilerPhase me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_phase(phase);
      me.set_compileId(compileId);
      me.set_phaseLevel(phaseLevel);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_phase");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_compileId");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_phaseLevel");
  }
#endif
};

class EventCompilationFailure : public JfrEvent<EventCompilationFailure>
{
 private:
  const char* _failureMessage;
  s4 _compileId;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCompilationFailureEvent;

  EventCompilationFailure(EventStartTime timing=TIMED) : JfrEvent<EventCompilationFailure>(timing) {}

  void set_failureMessage(const char* new_value) {
    this->_failureMessage = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_compileId(s4 new_value) {
    this->_compileId = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_failureMessage);
    w.write(_compileId);
  }

  using JfrEvent<EventCompilationFailure>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* failureMessage,
                     s4 compileId) {
    EventCompilationFailure me(UNTIMED);

    if (me.should_commit()) {
      me.set_failureMessage(failureMessage);
      me.set_compileId(compileId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_failureMessage");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_compileId");
  }
#endif
};

class EventCompilerInlining : public JfrEvent<EventCompilerInlining>
{
 private:
  s4 _compileId;
  const Method* _caller;
  JfrStructCalleeMethod _callee;
  bool _succeeded;
  const char* _message;
  s4 _bci;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCompilerInliningEvent;

  EventCompilerInlining(EventStartTime timing=TIMED) : JfrEvent<EventCompilerInlining>(timing) {}

  void set_compileId(s4 new_value) {
    this->_compileId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_caller(const Method* new_value) {
    this->_caller = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_callee(const JfrStructCalleeMethod& value) {
    this->_callee = value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_succeeded(bool new_value) {
    this->_succeeded = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_message(const char* new_value) {
    this->_message = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_bci(s4 new_value) {
    this->_bci = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_compileId);
    w.write(_caller);
    _callee.writeData(w);
    w.write(_succeeded);
    w.write(_message);
    w.write(_bci);
  }

  using JfrEvent<EventCompilerInlining>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 compileId,
                     const Method* caller,
                     const JfrStructCalleeMethod& callee,
                     bool succeeded,
                     const char* message,
                     s4 bci) {
    EventCompilerInlining me(UNTIMED);

    if (me.should_commit()) {
      me.set_compileId(compileId);
      me.set_caller(caller);
      me.set_callee(callee);
      me.set_succeeded(succeeded);
      me.set_message(message);
      me.set_bci(bci);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_compileId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_caller");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_callee");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_succeeded");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_message");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_bci");
  }
#endif
};

class EventCodeCacheFull : public JfrEvent<EventCodeCacheFull>
{
 private:
  u8 _codeBlobType;
  u8 _startAddress;
  u8 _commitedTopAddress;
  u8 _reservedTopAddress;
  s4 _entryCount;
  s4 _methodCount;
  s4 _adaptorCount;
  u8 _unallocatedCapacity;
  s4 _fullCount;
  u8 _codeCacheMaxCapacity;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCodeCacheFullEvent;

  EventCodeCacheFull(EventStartTime timing=TIMED) : JfrEvent<EventCodeCacheFull>(timing) {}

  void set_codeBlobType(u8 new_value) {
    this->_codeBlobType = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_startAddress(u8 new_value) {
    this->_startAddress = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_commitedTopAddress(u8 new_value) {
    this->_commitedTopAddress = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_reservedTopAddress(u8 new_value) {
    this->_reservedTopAddress = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_entryCount(s4 new_value) {
    this->_entryCount = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_methodCount(s4 new_value) {
    this->_methodCount = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_adaptorCount(s4 new_value) {
    this->_adaptorCount = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_unallocatedCapacity(u8 new_value) {
    this->_unallocatedCapacity = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_fullCount(s4 new_value) {
    this->_fullCount = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }
  void set_codeCacheMaxCapacity(u8 new_value) {
    this->_codeCacheMaxCapacity = new_value;
    DEBUG_ONLY(set_field_bit(9));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_codeBlobType);
    w.write(_startAddress);
    w.write(_commitedTopAddress);
    w.write(_reservedTopAddress);
    w.write(_entryCount);
    w.write(_methodCount);
    w.write(_adaptorCount);
    w.write(_unallocatedCapacity);
    w.write(_fullCount);
    w.write(_codeCacheMaxCapacity);
  }

  using JfrEvent<EventCodeCacheFull>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 codeBlobType,
                     u8 startAddress,
                     u8 commitedTopAddress,
                     u8 reservedTopAddress,
                     s4 entryCount,
                     s4 methodCount,
                     s4 adaptorCount,
                     u8 unallocatedCapacity,
                     s4 fullCount,
                     u8 codeCacheMaxCapacity) {
    EventCodeCacheFull me(UNTIMED);

    if (me.should_commit()) {
      me.set_codeBlobType(codeBlobType);
      me.set_startAddress(startAddress);
      me.set_commitedTopAddress(commitedTopAddress);
      me.set_reservedTopAddress(reservedTopAddress);
      me.set_entryCount(entryCount);
      me.set_methodCount(methodCount);
      me.set_adaptorCount(adaptorCount);
      me.set_unallocatedCapacity(unallocatedCapacity);
      me.set_fullCount(fullCount);
      me.set_codeCacheMaxCapacity(codeCacheMaxCapacity);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_codeBlobType");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_startAddress");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_commitedTopAddress");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_reservedTopAddress");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_entryCount");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_methodCount");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_adaptorCount");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_unallocatedCapacity");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_fullCount");
    assert(verify_field_bit(9), "Attempting to write an uninitialized event field: %s", "_codeCacheMaxCapacity");
  }
#endif
};

class EventDeoptimization : public JfrEvent<EventDeoptimization>
{
 private:
  s4 _compileId;
  u8 _compiler;
  const Method* _method;
  s4 _lineNumber;
  s4 _bci;
  u8 _instruction;
  u8 _reason;
  u8 _action;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrDeoptimizationEvent;

  EventDeoptimization(EventStartTime timing=TIMED) : JfrEvent<EventDeoptimization>(timing) {}

  void set_compileId(s4 new_value) {
    this->_compileId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_compiler(u8 new_value) {
    this->_compiler = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_method(const Method* new_value) {
    this->_method = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_lineNumber(s4 new_value) {
    this->_lineNumber = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_bci(s4 new_value) {
    this->_bci = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_instruction(u8 new_value) {
    this->_instruction = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_reason(u8 new_value) {
    this->_reason = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_action(u8 new_value) {
    this->_action = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_compileId);
    w.write(_compiler);
    w.write(_method);
    w.write(_lineNumber);
    w.write(_bci);
    w.write(_instruction);
    w.write(_reason);
    w.write(_action);
  }

  using JfrEvent<EventDeoptimization>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 compileId,
                     u8 compiler,
                     const Method* method,
                     s4 lineNumber,
                     s4 bci,
                     u8 instruction,
                     u8 reason,
                     u8 action) {
    EventDeoptimization me(UNTIMED);

    if (me.should_commit()) {
      me.set_compileId(compileId);
      me.set_compiler(compiler);
      me.set_method(method);
      me.set_lineNumber(lineNumber);
      me.set_bci(bci);
      me.set_instruction(instruction);
      me.set_reason(reason);
      me.set_action(action);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_compileId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_compiler");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_method");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_lineNumber");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_bci");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_instruction");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_reason");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_action");
  }
#endif
};

class EventSafepointBegin : public JfrEvent<EventSafepointBegin>
{
 private:
  u8 _safepointId;
  s4 _totalThreadCount;
  s4 _jniCriticalThreadCount;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrSafepointBeginEvent;

  EventSafepointBegin(EventStartTime timing=TIMED) : JfrEvent<EventSafepointBegin>(timing) {}

  void set_safepointId(u8 new_value) {
    this->_safepointId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_totalThreadCount(s4 new_value) {
    this->_totalThreadCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_jniCriticalThreadCount(s4 new_value) {
    this->_jniCriticalThreadCount = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_safepointId);
    w.write(_totalThreadCount);
    w.write(_jniCriticalThreadCount);
  }

  using JfrEvent<EventSafepointBegin>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointBegin(
    u8 safepointId,
    s4 totalThreadCount,
    s4 jniCriticalThreadCount) : JfrEvent<EventSafepointBegin>(TIMED) {
    if (should_commit()) {
      set_safepointId(safepointId);
      set_totalThreadCount(totalThreadCount);
      set_jniCriticalThreadCount(jniCriticalThreadCount);
    }
  }

  void commit(u8 safepointId,
              s4 totalThreadCount,
              s4 jniCriticalThreadCount) {
    if (should_commit()) {
      set_safepointId(safepointId);
      set_totalThreadCount(totalThreadCount);
      set_jniCriticalThreadCount(jniCriticalThreadCount);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 safepointId,
                     s4 totalThreadCount,
                     s4 jniCriticalThreadCount) {
    EventSafepointBegin me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_safepointId(safepointId);
      me.set_totalThreadCount(totalThreadCount);
      me.set_jniCriticalThreadCount(jniCriticalThreadCount);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_safepointId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_totalThreadCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_jniCriticalThreadCount");
  }
#endif
};

class EventSafepointStateSynchronization : public JfrEvent<EventSafepointStateSynchronization>
{
 private:
  u8 _safepointId;
  s4 _initialThreadCount;
  s4 _runningThreadCount;
  s4 _iterations;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrSafepointStateSynchronizationEvent;

  EventSafepointStateSynchronization(EventStartTime timing=TIMED) : JfrEvent<EventSafepointStateSynchronization>(timing) {}

  void set_safepointId(u8 new_value) {
    this->_safepointId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_initialThreadCount(s4 new_value) {
    this->_initialThreadCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_runningThreadCount(s4 new_value) {
    this->_runningThreadCount = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_iterations(s4 new_value) {
    this->_iterations = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_safepointId);
    w.write(_initialThreadCount);
    w.write(_runningThreadCount);
    w.write(_iterations);
  }

  using JfrEvent<EventSafepointStateSynchronization>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointStateSynchronization(
    u8 safepointId,
    s4 initialThreadCount,
    s4 runningThreadCount,
    s4 iterations) : JfrEvent<EventSafepointStateSynchronization>(TIMED) {
    if (should_commit()) {
      set_safepointId(safepointId);
      set_initialThreadCount(initialThreadCount);
      set_runningThreadCount(runningThreadCount);
      set_iterations(iterations);
    }
  }

  void commit(u8 safepointId,
              s4 initialThreadCount,
              s4 runningThreadCount,
              s4 iterations) {
    if (should_commit()) {
      set_safepointId(safepointId);
      set_initialThreadCount(initialThreadCount);
      set_runningThreadCount(runningThreadCount);
      set_iterations(iterations);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 safepointId,
                     s4 initialThreadCount,
                     s4 runningThreadCount,
                     s4 iterations) {
    EventSafepointStateSynchronization me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_safepointId(safepointId);
      me.set_initialThreadCount(initialThreadCount);
      me.set_runningThreadCount(runningThreadCount);
      me.set_iterations(iterations);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_safepointId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_initialThreadCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_runningThreadCount");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_iterations");
  }
#endif
};

class EventSafepointEnd : public JfrEvent<EventSafepointEnd>
{
 private:
  u8 _safepointId;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrSafepointEndEvent;

  EventSafepointEnd(EventStartTime timing=TIMED) : JfrEvent<EventSafepointEnd>(timing) {}

  void set_safepointId(u8 new_value) {
    this->_safepointId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_safepointId);
  }

  using JfrEvent<EventSafepointEnd>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointEnd(
    u8 safepointId) : JfrEvent<EventSafepointEnd>(TIMED) {
    if (should_commit()) {
      set_safepointId(safepointId);
    }
  }

  void commit(u8 safepointId) {
    if (should_commit()) {
      set_safepointId(safepointId);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 safepointId) {
    EventSafepointEnd me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_safepointId(safepointId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_safepointId");
  }
#endif
};

class EventSafepointLatency : public JfrEvent<EventSafepointLatency>
{
 private:
  u8 _threadState;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = true;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrSafepointLatencyEvent;

  EventSafepointLatency(EventStartTime timing=TIMED) : JfrEvent<EventSafepointLatency>(timing) {}

  void set_threadState(u8 new_value) {
    this->_threadState = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_threadState);
  }

  using JfrEvent<EventSafepointLatency>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointLatency(
    u8 threadState) : JfrEvent<EventSafepointLatency>(TIMED) {
    if (should_commit()) {
      set_threadState(threadState);
    }
  }

  void commit(u8 threadState) {
    if (should_commit()) {
      set_threadState(threadState);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 threadState) {
    EventSafepointLatency me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_threadState(threadState);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_threadState");
  }
#endif
};

class EventExecuteVMOperation : public JfrEvent<EventExecuteVMOperation>
{
 private:
  u8 _operation;
  bool _safepoint;
  bool _blocking;
  u8 _caller;
  u8 _safepointId;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrExecuteVMOperationEvent;

  EventExecuteVMOperation(EventStartTime timing=TIMED) : JfrEvent<EventExecuteVMOperation>(timing) {}

  void set_operation(u8 new_value) {
    this->_operation = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_safepoint(bool new_value) {
    this->_safepoint = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_blocking(bool new_value) {
    this->_blocking = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_caller(u8 new_value) {
    this->_caller = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_safepointId(u8 new_value) {
    this->_safepointId = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_operation);
    w.write(_safepoint);
    w.write(_blocking);
    w.write(_caller);
    w.write(_safepointId);
  }

  using JfrEvent<EventExecuteVMOperation>::commit; // else commit() is hidden by overloaded versions in this class

  EventExecuteVMOperation(
    u8 operation,
    bool safepoint,
    bool blocking,
    u8 caller,
    u8 safepointId) : JfrEvent<EventExecuteVMOperation>(TIMED) {
    if (should_commit()) {
      set_operation(operation);
      set_safepoint(safepoint);
      set_blocking(blocking);
      set_caller(caller);
      set_safepointId(safepointId);
    }
  }

  void commit(u8 operation,
              bool safepoint,
              bool blocking,
              u8 caller,
              u8 safepointId) {
    if (should_commit()) {
      set_operation(operation);
      set_safepoint(safepoint);
      set_blocking(blocking);
      set_caller(caller);
      set_safepointId(safepointId);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 operation,
                     bool safepoint,
                     bool blocking,
                     u8 caller,
                     u8 safepointId) {
    EventExecuteVMOperation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_operation(operation);
      me.set_safepoint(safepoint);
      me.set_blocking(blocking);
      me.set_caller(caller);
      me.set_safepointId(safepointId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_operation");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_safepoint");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_blocking");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_caller");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_safepointId");
  }
#endif
};

class EventShutdown : public JfrEvent<EventShutdown>
{
 private:
  const char* _reason;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrShutdownEvent;

  EventShutdown(EventStartTime timing=TIMED) : JfrEvent<EventShutdown>(timing) {}

  void set_reason(const char* new_value) {
    this->_reason = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_reason);
  }

  using JfrEvent<EventShutdown>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* reason) {
    EventShutdown me(UNTIMED);

    if (me.should_commit()) {
      me.set_reason(reason);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_reason");
  }
#endif
};

class EventObjectAllocationInNewTLAB : public JfrEvent<EventObjectAllocationInNewTLAB>
{
 private:
  const Klass* _objectClass;
  u8 _allocationSize;
  u8 _tlabSize;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrObjectAllocationInNewTLABEvent;

  EventObjectAllocationInNewTLAB(EventStartTime timing=TIMED) : JfrEvent<EventObjectAllocationInNewTLAB>(timing) {}

  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_allocationSize(u8 new_value) {
    this->_allocationSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_tlabSize(u8 new_value) {
    this->_tlabSize = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_objectClass);
    w.write(_allocationSize);
    w.write(_tlabSize);
  }

  using JfrEvent<EventObjectAllocationInNewTLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* objectClass,
                     u8 allocationSize,
                     u8 tlabSize) {
    EventObjectAllocationInNewTLAB me(UNTIMED);

    if (me.should_commit()) {
      me.set_objectClass(objectClass);
      me.set_allocationSize(allocationSize);
      me.set_tlabSize(tlabSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_allocationSize");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_tlabSize");
  }
#endif
};

class EventObjectAllocationOutsideTLAB : public JfrEvent<EventObjectAllocationOutsideTLAB>
{
 private:
  const Klass* _objectClass;
  u8 _allocationSize;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrObjectAllocationOutsideTLABEvent;

  EventObjectAllocationOutsideTLAB(EventStartTime timing=TIMED) : JfrEvent<EventObjectAllocationOutsideTLAB>(timing) {}

  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_allocationSize(u8 new_value) {
    this->_allocationSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_objectClass);
    w.write(_allocationSize);
  }

  using JfrEvent<EventObjectAllocationOutsideTLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* objectClass,
                     u8 allocationSize) {
    EventObjectAllocationOutsideTLAB me(UNTIMED);

    if (me.should_commit()) {
      me.set_objectClass(objectClass);
      me.set_allocationSize(allocationSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_allocationSize");
  }
#endif
};

class EventObjectAllocationSample : public JfrEvent<EventObjectAllocationSample>
{
 private:
  const Klass* _objectClass;
  s8 _weight;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = true;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrObjectAllocationSampleEvent;

  EventObjectAllocationSample(EventStartTime timing=TIMED) : JfrEvent<EventObjectAllocationSample>(timing) {}

  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_weight(s8 new_value) {
    this->_weight = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_objectClass);
    w.write(_weight);
  }

  using JfrEvent<EventObjectAllocationSample>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* objectClass,
                     s8 weight) {
    EventObjectAllocationSample me(UNTIMED);

    if (me.should_commit()) {
      me.set_objectClass(objectClass);
      me.set_weight(weight);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_weight");
  }
#endif
};

class EventOldObjectSample : public JfrEvent<EventOldObjectSample>
{
 private:
  Ticks _allocationTime;
  u8 _objectSize;
  Tickspan _objectAge;
  u8 _lastKnownHeapUsage;
  u8 _object;
  s4 _arrayElements;
  u8 _root;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = true;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrOldObjectSampleEvent;

  EventOldObjectSample(EventStartTime timing=TIMED) : JfrEvent<EventOldObjectSample>(timing) {}

  void set_allocationTime(const Ticks& new_value) {
    this->_allocationTime = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_objectSize(u8 new_value) {
    this->_objectSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_objectAge(const Tickspan& new_value) {
    this->_objectAge = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_lastKnownHeapUsage(u8 new_value) {
    this->_lastKnownHeapUsage = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_object(u8 new_value) {
    this->_object = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_arrayElements(s4 new_value) {
    this->_arrayElements = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_root(u8 new_value) {
    this->_root = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_allocationTime);
    w.write(_objectSize);
    w.write(_objectAge);
    w.write(_lastKnownHeapUsage);
    w.write(_object);
    w.write(_arrayElements);
    w.write(_root);
  }

  using JfrEvent<EventOldObjectSample>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Ticks& allocationTime,
                     u8 objectSize,
                     const Tickspan& objectAge,
                     u8 lastKnownHeapUsage,
                     u8 object,
                     s4 arrayElements,
                     u8 root) {
    EventOldObjectSample me(UNTIMED);

    if (me.should_commit()) {
      me.set_allocationTime(allocationTime);
      me.set_objectSize(objectSize);
      me.set_objectAge(objectAge);
      me.set_lastKnownHeapUsage(lastKnownHeapUsage);
      me.set_object(object);
      me.set_arrayElements(arrayElements);
      me.set_root(root);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_allocationTime");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_objectSize");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_objectAge");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_lastKnownHeapUsage");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_object");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_arrayElements");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_root");
  }
#endif
};

class EventNativeMemoryUsage : public JfrEvent<EventNativeMemoryUsage>
{
 private:
  u8 _type;
  u8 _reserved;
  u8 _committed;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrNativeMemoryUsageEvent;

  EventNativeMemoryUsage(EventStartTime timing=TIMED) : JfrEvent<EventNativeMemoryUsage>(timing) {}

  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_reserved(u8 new_value) {
    this->_reserved = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_committed(u8 new_value) {
    this->_committed = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_type);
    w.write(_reserved);
    w.write(_committed);
  }

  using JfrEvent<EventNativeMemoryUsage>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeMemoryUsage(
    u8 type,
    u8 reserved,
    u8 committed) : JfrEvent<EventNativeMemoryUsage>(TIMED) {
    if (should_commit()) {
      set_type(type);
      set_reserved(reserved);
      set_committed(committed);
    }
  }

  void commit(u8 type,
              u8 reserved,
              u8 committed) {
    if (should_commit()) {
      set_type(type);
      set_reserved(reserved);
      set_committed(committed);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 reserved,
                     u8 committed) {
    EventNativeMemoryUsage me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_type(type);
      me.set_reserved(reserved);
      me.set_committed(committed);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_type");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_reserved");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_committed");
  }
#endif
};

class EventNativeMemoryUsageTotal : public JfrEvent<EventNativeMemoryUsageTotal>
{
 private:
  u8 _reserved;
  u8 _committed;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrNativeMemoryUsageTotalEvent;

  EventNativeMemoryUsageTotal(EventStartTime timing=TIMED) : JfrEvent<EventNativeMemoryUsageTotal>(timing) {}

  void set_reserved(u8 new_value) {
    this->_reserved = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_committed(u8 new_value) {
    this->_committed = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_reserved);
    w.write(_committed);
  }

  using JfrEvent<EventNativeMemoryUsageTotal>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeMemoryUsageTotal(
    u8 reserved,
    u8 committed) : JfrEvent<EventNativeMemoryUsageTotal>(TIMED) {
    if (should_commit()) {
      set_reserved(reserved);
      set_committed(committed);
    }
  }

  void commit(u8 reserved,
              u8 committed) {
    if (should_commit()) {
      set_reserved(reserved);
      set_committed(committed);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 reserved,
                     u8 committed) {
    EventNativeMemoryUsageTotal me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_reserved(reserved);
      me.set_committed(committed);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_reserved");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_committed");
  }
#endif
};

class EventDumpReason : public JfrEvent<EventDumpReason>
{
 private:
  const char* _reason;
  s4 _recordingId;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrDumpReasonEvent;

  EventDumpReason(EventStartTime timing=TIMED) : JfrEvent<EventDumpReason>(timing) {}

  void set_reason(const char* new_value) {
    this->_reason = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_recordingId(s4 new_value) {
    this->_recordingId = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_reason);
    w.write(_recordingId);
  }

  using JfrEvent<EventDumpReason>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* reason,
                     s4 recordingId) {
    EventDumpReason me(UNTIMED);

    if (me.should_commit()) {
      me.set_reason(reason);
      me.set_recordingId(recordingId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_reason");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_recordingId");
  }
#endif
};

class EventDataLoss : public JfrEvent<EventDataLoss>
{
 private:
  u8 _amount;
  u8 _total;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrDataLossEvent;

  EventDataLoss(EventStartTime timing=TIMED) : JfrEvent<EventDataLoss>(timing) {}

  void set_amount(u8 new_value) {
    this->_amount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_total(u8 new_value) {
    this->_total = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_amount);
    w.write(_total);
  }

  using JfrEvent<EventDataLoss>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 amount,
                     u8 total) {
    EventDataLoss me(UNTIMED);

    if (me.should_commit()) {
      me.set_amount(amount);
      me.set_total(total);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_amount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_total");
  }
#endif
};

class EventJVMInformation : public JfrEvent<EventJVMInformation>
{
 private:
  const char* _jvmName;
  const char* _jvmVersion;
  const char* _jvmArguments;
  const char* _jvmFlags;
  const char* _javaArguments;
  s8 _jvmStartTime;
  s8 _pid;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrJVMInformationEvent;

  EventJVMInformation(EventStartTime timing=TIMED) : JfrEvent<EventJVMInformation>(timing) {}

  void set_jvmName(const char* new_value) {
    this->_jvmName = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_jvmVersion(const char* new_value) {
    this->_jvmVersion = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_jvmArguments(const char* new_value) {
    this->_jvmArguments = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_jvmFlags(const char* new_value) {
    this->_jvmFlags = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_javaArguments(const char* new_value) {
    this->_javaArguments = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_jvmStartTime(s8 new_value) {
    this->_jvmStartTime = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_pid(s8 new_value) {
    this->_pid = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_jvmName);
    w.write(_jvmVersion);
    w.write(_jvmArguments);
    w.write(_jvmFlags);
    w.write(_javaArguments);
    w.write(_jvmStartTime);
    w.write(_pid);
  }

  using JfrEvent<EventJVMInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventJVMInformation(
    const char* jvmName,
    const char* jvmVersion,
    const char* jvmArguments,
    const char* jvmFlags,
    const char* javaArguments,
    s8 jvmStartTime,
    s8 pid) : JfrEvent<EventJVMInformation>(TIMED) {
    if (should_commit()) {
      set_jvmName(jvmName);
      set_jvmVersion(jvmVersion);
      set_jvmArguments(jvmArguments);
      set_jvmFlags(jvmFlags);
      set_javaArguments(javaArguments);
      set_jvmStartTime(jvmStartTime);
      set_pid(pid);
    }
  }

  void commit(const char* jvmName,
              const char* jvmVersion,
              const char* jvmArguments,
              const char* jvmFlags,
              const char* javaArguments,
              s8 jvmStartTime,
              s8 pid) {
    if (should_commit()) {
      set_jvmName(jvmName);
      set_jvmVersion(jvmVersion);
      set_jvmArguments(jvmArguments);
      set_jvmFlags(jvmFlags);
      set_javaArguments(javaArguments);
      set_jvmStartTime(jvmStartTime);
      set_pid(pid);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* jvmName,
                     const char* jvmVersion,
                     const char* jvmArguments,
                     const char* jvmFlags,
                     const char* javaArguments,
                     s8 jvmStartTime,
                     s8 pid) {
    EventJVMInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_jvmName(jvmName);
      me.set_jvmVersion(jvmVersion);
      me.set_jvmArguments(jvmArguments);
      me.set_jvmFlags(jvmFlags);
      me.set_javaArguments(javaArguments);
      me.set_jvmStartTime(jvmStartTime);
      me.set_pid(pid);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_jvmName");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_jvmVersion");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_jvmArguments");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_jvmFlags");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_javaArguments");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_jvmStartTime");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_pid");
  }
#endif
};

class EventOSInformation : public JfrEvent<EventOSInformation>
{
 private:
  const char* _osVersion;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrOSInformationEvent;

  EventOSInformation(EventStartTime timing=TIMED) : JfrEvent<EventOSInformation>(timing) {}

  void set_osVersion(const char* new_value) {
    this->_osVersion = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_osVersion);
  }

  using JfrEvent<EventOSInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventOSInformation(
    const char* osVersion) : JfrEvent<EventOSInformation>(TIMED) {
    if (should_commit()) {
      set_osVersion(osVersion);
    }
  }

  void commit(const char* osVersion) {
    if (should_commit()) {
      set_osVersion(osVersion);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* osVersion) {
    EventOSInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_osVersion(osVersion);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_osVersion");
  }
#endif
};

class EventVirtualizationInformation : public JfrEvent<EventVirtualizationInformation>
{
 private:
  const char* _name;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrVirtualizationInformationEvent;

  EventVirtualizationInformation(EventStartTime timing=TIMED) : JfrEvent<EventVirtualizationInformation>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
  }

  using JfrEvent<EventVirtualizationInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventVirtualizationInformation(
    const char* name) : JfrEvent<EventVirtualizationInformation>(TIMED) {
    if (should_commit()) {
      set_name(name);
    }
  }

  void commit(const char* name) {
    if (should_commit()) {
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name) {
    EventVirtualizationInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventInitialSystemProperty : public JfrEvent<EventInitialSystemProperty>
{
 private:
  const char* _key;
  const char* _value;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrInitialSystemPropertyEvent;

  EventInitialSystemProperty(EventStartTime timing=TIMED) : JfrEvent<EventInitialSystemProperty>(timing) {}

  void set_key(const char* new_value) {
    this->_key = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(const char* new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_key);
    w.write(_value);
  }

  using JfrEvent<EventInitialSystemProperty>::commit; // else commit() is hidden by overloaded versions in this class

  EventInitialSystemProperty(
    const char* key,
    const char* value) : JfrEvent<EventInitialSystemProperty>(TIMED) {
    if (should_commit()) {
      set_key(key);
      set_value(value);
    }
  }

  void commit(const char* key,
              const char* value) {
    if (should_commit()) {
      set_key(key);
      set_value(value);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* key,
                     const char* value) {
    EventInitialSystemProperty me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_key(key);
      me.set_value(value);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_key");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
  }
#endif
};

class EventInitialEnvironmentVariable : public JfrEvent<EventInitialEnvironmentVariable>
{
 private:
  const char* _key;
  const char* _value;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrInitialEnvironmentVariableEvent;

  EventInitialEnvironmentVariable(EventStartTime timing=TIMED) : JfrEvent<EventInitialEnvironmentVariable>(timing) {}

  void set_key(const char* new_value) {
    this->_key = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(const char* new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_key);
    w.write(_value);
  }

  using JfrEvent<EventInitialEnvironmentVariable>::commit; // else commit() is hidden by overloaded versions in this class

  EventInitialEnvironmentVariable(
    const char* key,
    const char* value) : JfrEvent<EventInitialEnvironmentVariable>(TIMED) {
    if (should_commit()) {
      set_key(key);
      set_value(value);
    }
  }

  void commit(const char* key,
              const char* value) {
    if (should_commit()) {
      set_key(key);
      set_value(value);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* key,
                     const char* value) {
    EventInitialEnvironmentVariable me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_key(key);
      me.set_value(value);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_key");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
  }
#endif
};

class EventSystemProcess : public JfrEvent<EventSystemProcess>
{
 private:
  const char* _pid;
  const char* _commandLine;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrSystemProcessEvent;

  EventSystemProcess(EventStartTime timing=TIMED) : JfrEvent<EventSystemProcess>(timing) {}

  void set_pid(const char* new_value) {
    this->_pid = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_commandLine(const char* new_value) {
    this->_commandLine = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_pid);
    w.write(_commandLine);
  }

  using JfrEvent<EventSystemProcess>::commit; // else commit() is hidden by overloaded versions in this class

  EventSystemProcess(
    const char* pid,
    const char* commandLine) : JfrEvent<EventSystemProcess>(TIMED) {
    if (should_commit()) {
      set_pid(pid);
      set_commandLine(commandLine);
    }
  }

  void commit(const char* pid,
              const char* commandLine) {
    if (should_commit()) {
      set_pid(pid);
      set_commandLine(commandLine);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* pid,
                     const char* commandLine) {
    EventSystemProcess me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_pid(pid);
      me.set_commandLine(commandLine);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_pid");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_commandLine");
  }
#endif
};

class EventCPUInformation : public JfrEvent<EventCPUInformation>
{
 private:
  const char* _cpu;
  const char* _description;
  unsigned _sockets;
  unsigned _cores;
  unsigned _hwThreads;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCPUInformationEvent;

  EventCPUInformation(EventStartTime timing=TIMED) : JfrEvent<EventCPUInformation>(timing) {}

  void set_cpu(const char* new_value) {
    this->_cpu = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_description(const char* new_value) {
    this->_description = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_sockets(unsigned new_value) {
    this->_sockets = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_cores(unsigned new_value) {
    this->_cores = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_hwThreads(unsigned new_value) {
    this->_hwThreads = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_cpu);
    w.write(_description);
    w.write(_sockets);
    w.write(_cores);
    w.write(_hwThreads);
  }

  using JfrEvent<EventCPUInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventCPUInformation(
    const char* cpu,
    const char* description,
    unsigned sockets,
    unsigned cores,
    unsigned hwThreads) : JfrEvent<EventCPUInformation>(TIMED) {
    if (should_commit()) {
      set_cpu(cpu);
      set_description(description);
      set_sockets(sockets);
      set_cores(cores);
      set_hwThreads(hwThreads);
    }
  }

  void commit(const char* cpu,
              const char* description,
              unsigned sockets,
              unsigned cores,
              unsigned hwThreads) {
    if (should_commit()) {
      set_cpu(cpu);
      set_description(description);
      set_sockets(sockets);
      set_cores(cores);
      set_hwThreads(hwThreads);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* cpu,
                     const char* description,
                     unsigned sockets,
                     unsigned cores,
                     unsigned hwThreads) {
    EventCPUInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_cpu(cpu);
      me.set_description(description);
      me.set_sockets(sockets);
      me.set_cores(cores);
      me.set_hwThreads(hwThreads);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_cpu");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_description");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_sockets");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_cores");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_hwThreads");
  }
#endif
};

class EventCPUTimeStampCounter : public JfrEvent<EventCPUTimeStampCounter>
{
 private:
  bool _fastTimeEnabled;
  bool _fastTimeAutoEnabled;
  s8 _osFrequency;
  s8 _fastTimeFrequency;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCPUTimeStampCounterEvent;

  EventCPUTimeStampCounter(EventStartTime timing=TIMED) : JfrEvent<EventCPUTimeStampCounter>(timing) {}

  void set_fastTimeEnabled(bool new_value) {
    this->_fastTimeEnabled = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_fastTimeAutoEnabled(bool new_value) {
    this->_fastTimeAutoEnabled = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_osFrequency(s8 new_value) {
    this->_osFrequency = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_fastTimeFrequency(s8 new_value) {
    this->_fastTimeFrequency = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_fastTimeEnabled);
    w.write(_fastTimeAutoEnabled);
    w.write(_osFrequency);
    w.write(_fastTimeFrequency);
  }

  using JfrEvent<EventCPUTimeStampCounter>::commit; // else commit() is hidden by overloaded versions in this class

  EventCPUTimeStampCounter(
    bool fastTimeEnabled,
    bool fastTimeAutoEnabled,
    s8 osFrequency,
    s8 fastTimeFrequency) : JfrEvent<EventCPUTimeStampCounter>(TIMED) {
    if (should_commit()) {
      set_fastTimeEnabled(fastTimeEnabled);
      set_fastTimeAutoEnabled(fastTimeAutoEnabled);
      set_osFrequency(osFrequency);
      set_fastTimeFrequency(fastTimeFrequency);
    }
  }

  void commit(bool fastTimeEnabled,
              bool fastTimeAutoEnabled,
              s8 osFrequency,
              s8 fastTimeFrequency) {
    if (should_commit()) {
      set_fastTimeEnabled(fastTimeEnabled);
      set_fastTimeAutoEnabled(fastTimeAutoEnabled);
      set_osFrequency(osFrequency);
      set_fastTimeFrequency(fastTimeFrequency);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     bool fastTimeEnabled,
                     bool fastTimeAutoEnabled,
                     s8 osFrequency,
                     s8 fastTimeFrequency) {
    EventCPUTimeStampCounter me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_fastTimeEnabled(fastTimeEnabled);
      me.set_fastTimeAutoEnabled(fastTimeAutoEnabled);
      me.set_osFrequency(osFrequency);
      me.set_fastTimeFrequency(fastTimeFrequency);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_fastTimeEnabled");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_fastTimeAutoEnabled");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_osFrequency");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_fastTimeFrequency");
  }
#endif
};

class EventCPULoad : public JfrEvent<EventCPULoad>
{
 private:
  float _jvmUser;
  float _jvmSystem;
  float _machineTotal;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCPULoadEvent;

  EventCPULoad(EventStartTime timing=TIMED) : JfrEvent<EventCPULoad>(timing) {}

  void set_jvmUser(float new_value) {
    this->_jvmUser = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_jvmSystem(float new_value) {
    this->_jvmSystem = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_machineTotal(float new_value) {
    this->_machineTotal = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_jvmUser);
    w.write(_jvmSystem);
    w.write(_machineTotal);
  }

  using JfrEvent<EventCPULoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventCPULoad(
    float jvmUser,
    float jvmSystem,
    float machineTotal) : JfrEvent<EventCPULoad>(TIMED) {
    if (should_commit()) {
      set_jvmUser(jvmUser);
      set_jvmSystem(jvmSystem);
      set_machineTotal(machineTotal);
    }
  }

  void commit(float jvmUser,
              float jvmSystem,
              float machineTotal) {
    if (should_commit()) {
      set_jvmUser(jvmUser);
      set_jvmSystem(jvmSystem);
      set_machineTotal(machineTotal);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     float jvmUser,
                     float jvmSystem,
                     float machineTotal) {
    EventCPULoad me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_jvmUser(jvmUser);
      me.set_jvmSystem(jvmSystem);
      me.set_machineTotal(machineTotal);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_jvmUser");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_jvmSystem");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_machineTotal");
  }
#endif
};

class EventThreadCPULoad : public JfrEvent<EventThreadCPULoad>
{
 private:
  float _user;
  float _system;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrThreadCPULoadEvent;

  EventThreadCPULoad(EventStartTime timing=TIMED) : JfrEvent<EventThreadCPULoad>(timing) {}

  void set_user(float new_value) {
    this->_user = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_system(float new_value) {
    this->_system = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_user);
    w.write(_system);
  }

  using JfrEvent<EventThreadCPULoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadCPULoad(
    float user,
    float system) : JfrEvent<EventThreadCPULoad>(TIMED) {
    if (should_commit()) {
      set_user(user);
      set_system(system);
    }
  }

  void commit(float user,
              float system) {
    if (should_commit()) {
      set_user(user);
      set_system(system);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     float user,
                     float system) {
    EventThreadCPULoad me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_user(user);
      me.set_system(system);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_user");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_system");
  }
#endif
};

class EventThreadContextSwitchRate : public JfrEvent<EventThreadContextSwitchRate>
{
 private:
  float _switchRate;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrThreadContextSwitchRateEvent;

  EventThreadContextSwitchRate(EventStartTime timing=TIMED) : JfrEvent<EventThreadContextSwitchRate>(timing) {}

  void set_switchRate(float new_value) {
    this->_switchRate = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_switchRate);
  }

  using JfrEvent<EventThreadContextSwitchRate>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadContextSwitchRate(
    float switchRate) : JfrEvent<EventThreadContextSwitchRate>(TIMED) {
    if (should_commit()) {
      set_switchRate(switchRate);
    }
  }

  void commit(float switchRate) {
    if (should_commit()) {
      set_switchRate(switchRate);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     float switchRate) {
    EventThreadContextSwitchRate me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_switchRate(switchRate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_switchRate");
  }
#endif
};

class EventCompilerQueueUtilization : public JfrEvent<EventCompilerQueueUtilization>
{
 private:
  u8 _compiler;
  s8 _addedRate;
  s8 _removedRate;
  s8 _queueSize;
  s8 _peakQueueSize;
  s8 _addedCount;
  s8 _removedCount;
  s8 _totalAddedCount;
  s8 _totalRemovedCount;
  s4 _compilerThreadCount;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCompilerQueueUtilizationEvent;

  EventCompilerQueueUtilization(EventStartTime timing=TIMED) : JfrEvent<EventCompilerQueueUtilization>(timing) {}

  void set_compiler(u8 new_value) {
    this->_compiler = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_addedRate(s8 new_value) {
    this->_addedRate = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_removedRate(s8 new_value) {
    this->_removedRate = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_queueSize(s8 new_value) {
    this->_queueSize = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_peakQueueSize(s8 new_value) {
    this->_peakQueueSize = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_addedCount(s8 new_value) {
    this->_addedCount = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_removedCount(s8 new_value) {
    this->_removedCount = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_totalAddedCount(s8 new_value) {
    this->_totalAddedCount = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_totalRemovedCount(s8 new_value) {
    this->_totalRemovedCount = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }
  void set_compilerThreadCount(s4 new_value) {
    this->_compilerThreadCount = new_value;
    DEBUG_ONLY(set_field_bit(9));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_compiler);
    w.write(_addedRate);
    w.write(_removedRate);
    w.write(_queueSize);
    w.write(_peakQueueSize);
    w.write(_addedCount);
    w.write(_removedCount);
    w.write(_totalAddedCount);
    w.write(_totalRemovedCount);
    w.write(_compilerThreadCount);
  }

  using JfrEvent<EventCompilerQueueUtilization>::commit; // else commit() is hidden by overloaded versions in this class

  EventCompilerQueueUtilization(
    u8 compiler,
    s8 addedRate,
    s8 removedRate,
    s8 queueSize,
    s8 peakQueueSize,
    s8 addedCount,
    s8 removedCount,
    s8 totalAddedCount,
    s8 totalRemovedCount,
    s4 compilerThreadCount) : JfrEvent<EventCompilerQueueUtilization>(TIMED) {
    if (should_commit()) {
      set_compiler(compiler);
      set_addedRate(addedRate);
      set_removedRate(removedRate);
      set_queueSize(queueSize);
      set_peakQueueSize(peakQueueSize);
      set_addedCount(addedCount);
      set_removedCount(removedCount);
      set_totalAddedCount(totalAddedCount);
      set_totalRemovedCount(totalRemovedCount);
      set_compilerThreadCount(compilerThreadCount);
    }
  }

  void commit(u8 compiler,
              s8 addedRate,
              s8 removedRate,
              s8 queueSize,
              s8 peakQueueSize,
              s8 addedCount,
              s8 removedCount,
              s8 totalAddedCount,
              s8 totalRemovedCount,
              s4 compilerThreadCount) {
    if (should_commit()) {
      set_compiler(compiler);
      set_addedRate(addedRate);
      set_removedRate(removedRate);
      set_queueSize(queueSize);
      set_peakQueueSize(peakQueueSize);
      set_addedCount(addedCount);
      set_removedCount(removedCount);
      set_totalAddedCount(totalAddedCount);
      set_totalRemovedCount(totalRemovedCount);
      set_compilerThreadCount(compilerThreadCount);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 compiler,
                     s8 addedRate,
                     s8 removedRate,
                     s8 queueSize,
                     s8 peakQueueSize,
                     s8 addedCount,
                     s8 removedCount,
                     s8 totalAddedCount,
                     s8 totalRemovedCount,
                     s4 compilerThreadCount) {
    EventCompilerQueueUtilization me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_compiler(compiler);
      me.set_addedRate(addedRate);
      me.set_removedRate(removedRate);
      me.set_queueSize(queueSize);
      me.set_peakQueueSize(peakQueueSize);
      me.set_addedCount(addedCount);
      me.set_removedCount(removedCount);
      me.set_totalAddedCount(totalAddedCount);
      me.set_totalRemovedCount(totalRemovedCount);
      me.set_compilerThreadCount(compilerThreadCount);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_compiler");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_addedRate");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_removedRate");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_queueSize");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_peakQueueSize");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_addedCount");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_removedCount");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_totalAddedCount");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_totalRemovedCount");
    assert(verify_field_bit(9), "Attempting to write an uninitialized event field: %s", "_compilerThreadCount");
  }
#endif
};

class EventNetworkUtilization : public JfrEvent<EventNetworkUtilization>
{
 private:
  u8 _networkInterface;
  s8 _readRate;
  s8 _writeRate;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrNetworkUtilizationEvent;

  EventNetworkUtilization(EventStartTime timing=TIMED) : JfrEvent<EventNetworkUtilization>(timing) {}

  void set_networkInterface(u8 new_value) {
    this->_networkInterface = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_readRate(s8 new_value) {
    this->_readRate = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_writeRate(s8 new_value) {
    this->_writeRate = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_networkInterface);
    w.write(_readRate);
    w.write(_writeRate);
  }

  using JfrEvent<EventNetworkUtilization>::commit; // else commit() is hidden by overloaded versions in this class

  EventNetworkUtilization(
    u8 networkInterface,
    s8 readRate,
    s8 writeRate) : JfrEvent<EventNetworkUtilization>(TIMED) {
    if (should_commit()) {
      set_networkInterface(networkInterface);
      set_readRate(readRate);
      set_writeRate(writeRate);
    }
  }

  void commit(u8 networkInterface,
              s8 readRate,
              s8 writeRate) {
    if (should_commit()) {
      set_networkInterface(networkInterface);
      set_readRate(readRate);
      set_writeRate(writeRate);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 networkInterface,
                     s8 readRate,
                     s8 writeRate) {
    EventNetworkUtilization me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_networkInterface(networkInterface);
      me.set_readRate(readRate);
      me.set_writeRate(writeRate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_networkInterface");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_readRate");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_writeRate");
  }
#endif
};

class EventJavaThreadStatistics : public JfrEvent<EventJavaThreadStatistics>
{
 private:
  s8 _activeCount;
  s8 _daemonCount;
  s8 _accumulatedCount;
  s8 _peakCount;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrJavaThreadStatisticsEvent;

  EventJavaThreadStatistics(EventStartTime timing=TIMED) : JfrEvent<EventJavaThreadStatistics>(timing) {}

  void set_activeCount(s8 new_value) {
    this->_activeCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_daemonCount(s8 new_value) {
    this->_daemonCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_accumulatedCount(s8 new_value) {
    this->_accumulatedCount = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_peakCount(s8 new_value) {
    this->_peakCount = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_activeCount);
    w.write(_daemonCount);
    w.write(_accumulatedCount);
    w.write(_peakCount);
  }

  using JfrEvent<EventJavaThreadStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaThreadStatistics(
    s8 activeCount,
    s8 daemonCount,
    s8 accumulatedCount,
    s8 peakCount) : JfrEvent<EventJavaThreadStatistics>(TIMED) {
    if (should_commit()) {
      set_activeCount(activeCount);
      set_daemonCount(daemonCount);
      set_accumulatedCount(accumulatedCount);
      set_peakCount(peakCount);
    }
  }

  void commit(s8 activeCount,
              s8 daemonCount,
              s8 accumulatedCount,
              s8 peakCount) {
    if (should_commit()) {
      set_activeCount(activeCount);
      set_daemonCount(daemonCount);
      set_accumulatedCount(accumulatedCount);
      set_peakCount(peakCount);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s8 activeCount,
                     s8 daemonCount,
                     s8 accumulatedCount,
                     s8 peakCount) {
    EventJavaThreadStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_activeCount(activeCount);
      me.set_daemonCount(daemonCount);
      me.set_accumulatedCount(accumulatedCount);
      me.set_peakCount(peakCount);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_activeCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_daemonCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_accumulatedCount");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_peakCount");
  }
#endif
};

class EventClassLoadingStatistics : public JfrEvent<EventClassLoadingStatistics>
{
 private:
  s8 _loadedClassCount;
  s8 _unloadedClassCount;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrClassLoadingStatisticsEvent;

  EventClassLoadingStatistics(EventStartTime timing=TIMED) : JfrEvent<EventClassLoadingStatistics>(timing) {}

  void set_loadedClassCount(s8 new_value) {
    this->_loadedClassCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_unloadedClassCount(s8 new_value) {
    this->_unloadedClassCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_loadedClassCount);
    w.write(_unloadedClassCount);
  }

  using JfrEvent<EventClassLoadingStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventClassLoadingStatistics(
    s8 loadedClassCount,
    s8 unloadedClassCount) : JfrEvent<EventClassLoadingStatistics>(TIMED) {
    if (should_commit()) {
      set_loadedClassCount(loadedClassCount);
      set_unloadedClassCount(unloadedClassCount);
    }
  }

  void commit(s8 loadedClassCount,
              s8 unloadedClassCount) {
    if (should_commit()) {
      set_loadedClassCount(loadedClassCount);
      set_unloadedClassCount(unloadedClassCount);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s8 loadedClassCount,
                     s8 unloadedClassCount) {
    EventClassLoadingStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_loadedClassCount(loadedClassCount);
      me.set_unloadedClassCount(unloadedClassCount);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_loadedClassCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_unloadedClassCount");
  }
#endif
};

class EventClassLoaderStatistics : public JfrEvent<EventClassLoaderStatistics>
{
 private:
  const ClassLoaderData* _classLoader;
  const ClassLoaderData* _parentClassLoader;
  u8 _classLoaderData;
  s8 _classCount;
  u8 _chunkSize;
  u8 _blockSize;
  s8 _hiddenClassCount;
  u8 _hiddenChunkSize;
  u8 _hiddenBlockSize;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrClassLoaderStatisticsEvent;

  EventClassLoaderStatistics(EventStartTime timing=TIMED) : JfrEvent<EventClassLoaderStatistics>(timing) {}

  void set_classLoader(const ClassLoaderData* new_value) {
    this->_classLoader = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_parentClassLoader(const ClassLoaderData* new_value) {
    this->_parentClassLoader = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_classLoaderData(u8 new_value) {
    this->_classLoaderData = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_classCount(s8 new_value) {
    this->_classCount = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_chunkSize(u8 new_value) {
    this->_chunkSize = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_blockSize(u8 new_value) {
    this->_blockSize = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_hiddenClassCount(s8 new_value) {
    this->_hiddenClassCount = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_hiddenChunkSize(u8 new_value) {
    this->_hiddenChunkSize = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_hiddenBlockSize(u8 new_value) {
    this->_hiddenBlockSize = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_classLoader);
    w.write(_parentClassLoader);
    w.write(_classLoaderData);
    w.write(_classCount);
    w.write(_chunkSize);
    w.write(_blockSize);
    w.write(_hiddenClassCount);
    w.write(_hiddenChunkSize);
    w.write(_hiddenBlockSize);
  }

  using JfrEvent<EventClassLoaderStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventClassLoaderStatistics(
    const ClassLoaderData* classLoader,
    const ClassLoaderData* parentClassLoader,
    u8 classLoaderData,
    s8 classCount,
    u8 chunkSize,
    u8 blockSize,
    s8 hiddenClassCount,
    u8 hiddenChunkSize,
    u8 hiddenBlockSize) : JfrEvent<EventClassLoaderStatistics>(TIMED) {
    if (should_commit()) {
      set_classLoader(classLoader);
      set_parentClassLoader(parentClassLoader);
      set_classLoaderData(classLoaderData);
      set_classCount(classCount);
      set_chunkSize(chunkSize);
      set_blockSize(blockSize);
      set_hiddenClassCount(hiddenClassCount);
      set_hiddenChunkSize(hiddenChunkSize);
      set_hiddenBlockSize(hiddenBlockSize);
    }
  }

  void commit(const ClassLoaderData* classLoader,
              const ClassLoaderData* parentClassLoader,
              u8 classLoaderData,
              s8 classCount,
              u8 chunkSize,
              u8 blockSize,
              s8 hiddenClassCount,
              u8 hiddenChunkSize,
              u8 hiddenBlockSize) {
    if (should_commit()) {
      set_classLoader(classLoader);
      set_parentClassLoader(parentClassLoader);
      set_classLoaderData(classLoaderData);
      set_classCount(classCount);
      set_chunkSize(chunkSize);
      set_blockSize(blockSize);
      set_hiddenClassCount(hiddenClassCount);
      set_hiddenChunkSize(hiddenChunkSize);
      set_hiddenBlockSize(hiddenBlockSize);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const ClassLoaderData* classLoader,
                     const ClassLoaderData* parentClassLoader,
                     u8 classLoaderData,
                     s8 classCount,
                     u8 chunkSize,
                     u8 blockSize,
                     s8 hiddenClassCount,
                     u8 hiddenChunkSize,
                     u8 hiddenBlockSize) {
    EventClassLoaderStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_classLoader(classLoader);
      me.set_parentClassLoader(parentClassLoader);
      me.set_classLoaderData(classLoaderData);
      me.set_classCount(classCount);
      me.set_chunkSize(chunkSize);
      me.set_blockSize(blockSize);
      me.set_hiddenClassCount(hiddenClassCount);
      me.set_hiddenChunkSize(hiddenChunkSize);
      me.set_hiddenBlockSize(hiddenBlockSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_classLoader");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_parentClassLoader");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_classLoaderData");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_classCount");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_chunkSize");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_blockSize");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_hiddenClassCount");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_hiddenChunkSize");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_hiddenBlockSize");
  }
#endif
};

class EventSymbolTableStatistics : public JfrEvent<EventSymbolTableStatistics>
{
 private:
  u8 _bucketCount;
  u8 _entryCount;
  u8 _totalFootprint;
  u8 _bucketCountMaximum;
  float _bucketCountAverage;
  float _bucketCountVariance;
  float _bucketCountStandardDeviation;
  float _insertionRate;
  float _removalRate;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrSymbolTableStatisticsEvent;

  EventSymbolTableStatistics(EventStartTime timing=TIMED) : JfrEvent<EventSymbolTableStatistics>(timing) {}

  void set_bucketCount(u8 new_value) {
    this->_bucketCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_entryCount(u8 new_value) {
    this->_entryCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_totalFootprint(u8 new_value) {
    this->_totalFootprint = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_bucketCountMaximum(u8 new_value) {
    this->_bucketCountMaximum = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_bucketCountAverage(float new_value) {
    this->_bucketCountAverage = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_bucketCountVariance(float new_value) {
    this->_bucketCountVariance = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_bucketCountStandardDeviation(float new_value) {
    this->_bucketCountStandardDeviation = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_insertionRate(float new_value) {
    this->_insertionRate = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_removalRate(float new_value) {
    this->_removalRate = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_bucketCount);
    w.write(_entryCount);
    w.write(_totalFootprint);
    w.write(_bucketCountMaximum);
    w.write(_bucketCountAverage);
    w.write(_bucketCountVariance);
    w.write(_bucketCountStandardDeviation);
    w.write(_insertionRate);
    w.write(_removalRate);
  }

  using JfrEvent<EventSymbolTableStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventSymbolTableStatistics(
    u8 bucketCount,
    u8 entryCount,
    u8 totalFootprint,
    u8 bucketCountMaximum,
    float bucketCountAverage,
    float bucketCountVariance,
    float bucketCountStandardDeviation,
    float insertionRate,
    float removalRate) : JfrEvent<EventSymbolTableStatistics>(TIMED) {
    if (should_commit()) {
      set_bucketCount(bucketCount);
      set_entryCount(entryCount);
      set_totalFootprint(totalFootprint);
      set_bucketCountMaximum(bucketCountMaximum);
      set_bucketCountAverage(bucketCountAverage);
      set_bucketCountVariance(bucketCountVariance);
      set_bucketCountStandardDeviation(bucketCountStandardDeviation);
      set_insertionRate(insertionRate);
      set_removalRate(removalRate);
    }
  }

  void commit(u8 bucketCount,
              u8 entryCount,
              u8 totalFootprint,
              u8 bucketCountMaximum,
              float bucketCountAverage,
              float bucketCountVariance,
              float bucketCountStandardDeviation,
              float insertionRate,
              float removalRate) {
    if (should_commit()) {
      set_bucketCount(bucketCount);
      set_entryCount(entryCount);
      set_totalFootprint(totalFootprint);
      set_bucketCountMaximum(bucketCountMaximum);
      set_bucketCountAverage(bucketCountAverage);
      set_bucketCountVariance(bucketCountVariance);
      set_bucketCountStandardDeviation(bucketCountStandardDeviation);
      set_insertionRate(insertionRate);
      set_removalRate(removalRate);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 bucketCount,
                     u8 entryCount,
                     u8 totalFootprint,
                     u8 bucketCountMaximum,
                     float bucketCountAverage,
                     float bucketCountVariance,
                     float bucketCountStandardDeviation,
                     float insertionRate,
                     float removalRate) {
    EventSymbolTableStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_bucketCount(bucketCount);
      me.set_entryCount(entryCount);
      me.set_totalFootprint(totalFootprint);
      me.set_bucketCountMaximum(bucketCountMaximum);
      me.set_bucketCountAverage(bucketCountAverage);
      me.set_bucketCountVariance(bucketCountVariance);
      me.set_bucketCountStandardDeviation(bucketCountStandardDeviation);
      me.set_insertionRate(insertionRate);
      me.set_removalRate(removalRate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_bucketCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_entryCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_totalFootprint");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_bucketCountMaximum");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_bucketCountAverage");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_bucketCountVariance");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_bucketCountStandardDeviation");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_insertionRate");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_removalRate");
  }
#endif
};

class EventStringTableStatistics : public JfrEvent<EventStringTableStatistics>
{
 private:
  u8 _bucketCount;
  u8 _entryCount;
  u8 _totalFootprint;
  u8 _bucketCountMaximum;
  float _bucketCountAverage;
  float _bucketCountVariance;
  float _bucketCountStandardDeviation;
  float _insertionRate;
  float _removalRate;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrStringTableStatisticsEvent;

  EventStringTableStatistics(EventStartTime timing=TIMED) : JfrEvent<EventStringTableStatistics>(timing) {}

  void set_bucketCount(u8 new_value) {
    this->_bucketCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_entryCount(u8 new_value) {
    this->_entryCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_totalFootprint(u8 new_value) {
    this->_totalFootprint = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_bucketCountMaximum(u8 new_value) {
    this->_bucketCountMaximum = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_bucketCountAverage(float new_value) {
    this->_bucketCountAverage = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_bucketCountVariance(float new_value) {
    this->_bucketCountVariance = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_bucketCountStandardDeviation(float new_value) {
    this->_bucketCountStandardDeviation = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_insertionRate(float new_value) {
    this->_insertionRate = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_removalRate(float new_value) {
    this->_removalRate = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_bucketCount);
    w.write(_entryCount);
    w.write(_totalFootprint);
    w.write(_bucketCountMaximum);
    w.write(_bucketCountAverage);
    w.write(_bucketCountVariance);
    w.write(_bucketCountStandardDeviation);
    w.write(_insertionRate);
    w.write(_removalRate);
  }

  using JfrEvent<EventStringTableStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventStringTableStatistics(
    u8 bucketCount,
    u8 entryCount,
    u8 totalFootprint,
    u8 bucketCountMaximum,
    float bucketCountAverage,
    float bucketCountVariance,
    float bucketCountStandardDeviation,
    float insertionRate,
    float removalRate) : JfrEvent<EventStringTableStatistics>(TIMED) {
    if (should_commit()) {
      set_bucketCount(bucketCount);
      set_entryCount(entryCount);
      set_totalFootprint(totalFootprint);
      set_bucketCountMaximum(bucketCountMaximum);
      set_bucketCountAverage(bucketCountAverage);
      set_bucketCountVariance(bucketCountVariance);
      set_bucketCountStandardDeviation(bucketCountStandardDeviation);
      set_insertionRate(insertionRate);
      set_removalRate(removalRate);
    }
  }

  void commit(u8 bucketCount,
              u8 entryCount,
              u8 totalFootprint,
              u8 bucketCountMaximum,
              float bucketCountAverage,
              float bucketCountVariance,
              float bucketCountStandardDeviation,
              float insertionRate,
              float removalRate) {
    if (should_commit()) {
      set_bucketCount(bucketCount);
      set_entryCount(entryCount);
      set_totalFootprint(totalFootprint);
      set_bucketCountMaximum(bucketCountMaximum);
      set_bucketCountAverage(bucketCountAverage);
      set_bucketCountVariance(bucketCountVariance);
      set_bucketCountStandardDeviation(bucketCountStandardDeviation);
      set_insertionRate(insertionRate);
      set_removalRate(removalRate);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 bucketCount,
                     u8 entryCount,
                     u8 totalFootprint,
                     u8 bucketCountMaximum,
                     float bucketCountAverage,
                     float bucketCountVariance,
                     float bucketCountStandardDeviation,
                     float insertionRate,
                     float removalRate) {
    EventStringTableStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_bucketCount(bucketCount);
      me.set_entryCount(entryCount);
      me.set_totalFootprint(totalFootprint);
      me.set_bucketCountMaximum(bucketCountMaximum);
      me.set_bucketCountAverage(bucketCountAverage);
      me.set_bucketCountVariance(bucketCountVariance);
      me.set_bucketCountStandardDeviation(bucketCountStandardDeviation);
      me.set_insertionRate(insertionRate);
      me.set_removalRate(removalRate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_bucketCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_entryCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_totalFootprint");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_bucketCountMaximum");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_bucketCountAverage");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_bucketCountVariance");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_bucketCountStandardDeviation");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_insertionRate");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_removalRate");
  }
#endif
};

class EventThreadAllocationStatistics : public JfrEvent<EventThreadAllocationStatistics>
{
 private:
  u8 _allocated;
  u8 _thread;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrThreadAllocationStatisticsEvent;

  EventThreadAllocationStatistics(EventStartTime timing=TIMED) : JfrEvent<EventThreadAllocationStatistics>(timing) {}

  void set_allocated(u8 new_value) {
    this->_allocated = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_thread(u8 new_value) {
    this->_thread = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_allocated);
    w.write(_thread);
  }

  using JfrEvent<EventThreadAllocationStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadAllocationStatistics(
    u8 allocated,
    u8 thread) : JfrEvent<EventThreadAllocationStatistics>(TIMED) {
    if (should_commit()) {
      set_allocated(allocated);
      set_thread(thread);
    }
  }

  void commit(u8 allocated,
              u8 thread) {
    if (should_commit()) {
      set_allocated(allocated);
      set_thread(thread);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 allocated,
                     u8 thread) {
    EventThreadAllocationStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_allocated(allocated);
      me.set_thread(thread);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_allocated");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_thread");
  }
#endif
};

class EventPhysicalMemory : public JfrEvent<EventPhysicalMemory>
{
 private:
  u8 _totalSize;
  u8 _usedSize;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrPhysicalMemoryEvent;

  EventPhysicalMemory(EventStartTime timing=TIMED) : JfrEvent<EventPhysicalMemory>(timing) {}

  void set_totalSize(u8 new_value) {
    this->_totalSize = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_usedSize(u8 new_value) {
    this->_usedSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_totalSize);
    w.write(_usedSize);
  }

  using JfrEvent<EventPhysicalMemory>::commit; // else commit() is hidden by overloaded versions in this class

  EventPhysicalMemory(
    u8 totalSize,
    u8 usedSize) : JfrEvent<EventPhysicalMemory>(TIMED) {
    if (should_commit()) {
      set_totalSize(totalSize);
      set_usedSize(usedSize);
    }
  }

  void commit(u8 totalSize,
              u8 usedSize) {
    if (should_commit()) {
      set_totalSize(totalSize);
      set_usedSize(usedSize);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 totalSize,
                     u8 usedSize) {
    EventPhysicalMemory me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_totalSize(totalSize);
      me.set_usedSize(usedSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_totalSize");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_usedSize");
  }
#endif
};

class EventSwapSpace : public JfrEvent<EventSwapSpace>
{
 private:
  s8 _totalSize;
  s8 _freeSize;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrSwapSpaceEvent;

  EventSwapSpace(EventStartTime timing=TIMED) : JfrEvent<EventSwapSpace>(timing) {}

  void set_totalSize(s8 new_value) {
    this->_totalSize = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_freeSize(s8 new_value) {
    this->_freeSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_totalSize);
    w.write(_freeSize);
  }

  using JfrEvent<EventSwapSpace>::commit; // else commit() is hidden by overloaded versions in this class

  EventSwapSpace(
    s8 totalSize,
    s8 freeSize) : JfrEvent<EventSwapSpace>(TIMED) {
    if (should_commit()) {
      set_totalSize(totalSize);
      set_freeSize(freeSize);
    }
  }

  void commit(s8 totalSize,
              s8 freeSize) {
    if (should_commit()) {
      set_totalSize(totalSize);
      set_freeSize(freeSize);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s8 totalSize,
                     s8 freeSize) {
    EventSwapSpace me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_totalSize(totalSize);
      me.set_freeSize(freeSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_totalSize");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_freeSize");
  }
#endif
};

class EventExecutionSample : public JfrEvent<EventExecutionSample>
{
 private:
  u8 _sampledThread;
  u8 _stackTrace;
  u8 _state;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrExecutionSampleEvent;

  EventExecutionSample(EventStartTime timing=TIMED) : JfrEvent<EventExecutionSample>(timing) {}

  void set_sampledThread(u8 new_value) {
    this->_sampledThread = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_stackTrace(u8 new_value) {
    this->_stackTrace = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_state(u8 new_value) {
    this->_state = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_sampledThread);
    w.write(_stackTrace);
    w.write(_state);
  }

  using JfrEvent<EventExecutionSample>::commit; // else commit() is hidden by overloaded versions in this class

  EventExecutionSample(
    u8 sampledThread,
    u8 stackTrace,
    u8 state) : JfrEvent<EventExecutionSample>(TIMED) {
    if (should_commit()) {
      set_sampledThread(sampledThread);
      set_stackTrace(stackTrace);
      set_state(state);
    }
  }

  void commit(u8 sampledThread,
              u8 stackTrace,
              u8 state) {
    if (should_commit()) {
      set_sampledThread(sampledThread);
      set_stackTrace(stackTrace);
      set_state(state);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 sampledThread,
                     u8 stackTrace,
                     u8 state) {
    EventExecutionSample me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_sampledThread(sampledThread);
      me.set_stackTrace(stackTrace);
      me.set_state(state);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_sampledThread");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_stackTrace");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_state");
  }
#endif
};

class EventNativeMethodSample : public JfrEvent<EventNativeMethodSample>
{
 private:
  u8 _sampledThread;
  u8 _stackTrace;
  u8 _state;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrNativeMethodSampleEvent;

  EventNativeMethodSample(EventStartTime timing=TIMED) : JfrEvent<EventNativeMethodSample>(timing) {}

  void set_sampledThread(u8 new_value) {
    this->_sampledThread = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_stackTrace(u8 new_value) {
    this->_stackTrace = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_state(u8 new_value) {
    this->_state = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_sampledThread);
    w.write(_stackTrace);
    w.write(_state);
  }

  using JfrEvent<EventNativeMethodSample>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeMethodSample(
    u8 sampledThread,
    u8 stackTrace,
    u8 state) : JfrEvent<EventNativeMethodSample>(TIMED) {
    if (should_commit()) {
      set_sampledThread(sampledThread);
      set_stackTrace(stackTrace);
      set_state(state);
    }
  }

  void commit(u8 sampledThread,
              u8 stackTrace,
              u8 state) {
    if (should_commit()) {
      set_sampledThread(sampledThread);
      set_stackTrace(stackTrace);
      set_state(state);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 sampledThread,
                     u8 stackTrace,
                     u8 state) {
    EventNativeMethodSample me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_sampledThread(sampledThread);
      me.set_stackTrace(stackTrace);
      me.set_state(state);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_sampledThread");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_stackTrace");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_state");
  }
#endif
};

class EventCPUTimeSample : public JfrEvent<EventCPUTimeSample>
{
 private:
  u8 _stackTrace;
  u8 _eventThread;
  bool _failed;
  Tickspan _samplingPeriod;
  bool _biased;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = true;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCPUTimeSampleEvent;

  EventCPUTimeSample(EventStartTime timing=TIMED) : JfrEvent<EventCPUTimeSample>(timing) {}

  void set_stackTrace(u8 new_value) {
    this->_stackTrace = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_eventThread(u8 new_value) {
    this->_eventThread = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_failed(bool new_value) {
    this->_failed = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_samplingPeriod(const Tickspan& new_value) {
    this->_samplingPeriod = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_biased(bool new_value) {
    this->_biased = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_stackTrace);
    w.write(_eventThread);
    w.write(_failed);
    w.write(_samplingPeriod);
    w.write(_biased);
  }

  using JfrEvent<EventCPUTimeSample>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 stackTrace,
                     u8 eventThread,
                     bool failed,
                     const Tickspan& samplingPeriod,
                     bool biased) {
    EventCPUTimeSample me(UNTIMED);

    if (me.should_commit()) {
      me.set_stackTrace(stackTrace);
      me.set_eventThread(eventThread);
      me.set_failed(failed);
      me.set_samplingPeriod(samplingPeriod);
      me.set_biased(biased);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_stackTrace");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_eventThread");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_failed");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_samplingPeriod");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_biased");
  }
#endif
};

class EventCPUTimeSamplesLost : public JfrEvent<EventCPUTimeSamplesLost>
{
 private:
  s4 _lostSamples;
  u8 _eventThread;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrCPUTimeSamplesLostEvent;

  EventCPUTimeSamplesLost(EventStartTime timing=TIMED) : JfrEvent<EventCPUTimeSamplesLost>(timing) {}

  void set_lostSamples(s4 new_value) {
    this->_lostSamples = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_eventThread(u8 new_value) {
    this->_eventThread = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_lostSamples);
    w.write(_eventThread);
  }

  using JfrEvent<EventCPUTimeSamplesLost>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 lostSamples,
                     u8 eventThread) {
    EventCPUTimeSamplesLost me(UNTIMED);

    if (me.should_commit()) {
      me.set_lostSamples(lostSamples);
      me.set_eventThread(eventThread);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_lostSamples");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_eventThread");
  }
#endif
};

class EventThreadDump : public JfrEvent<EventThreadDump>
{
 private:
  const char* _result;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrThreadDumpEvent;

  EventThreadDump(EventStartTime timing=TIMED) : JfrEvent<EventThreadDump>(timing) {}

  void set_result(const char* new_value) {
    this->_result = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_result);
  }

  using JfrEvent<EventThreadDump>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadDump(
    const char* result) : JfrEvent<EventThreadDump>(TIMED) {
    if (should_commit()) {
      set_result(result);
    }
  }

  void commit(const char* result) {
    if (should_commit()) {
      set_result(result);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* result) {
    EventThreadDump me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_result(result);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_result");
  }
#endif
};

class EventNativeLibrary : public JfrEvent<EventNativeLibrary>
{
 private:
  const char* _name;
  u8 _baseAddress;
  u8 _topAddress;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrNativeLibraryEvent;

  EventNativeLibrary(EventStartTime timing=TIMED) : JfrEvent<EventNativeLibrary>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_baseAddress(u8 new_value) {
    this->_baseAddress = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_topAddress(u8 new_value) {
    this->_topAddress = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_baseAddress);
    w.write(_topAddress);
  }

  using JfrEvent<EventNativeLibrary>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeLibrary(
    const char* name,
    u8 baseAddress,
    u8 topAddress) : JfrEvent<EventNativeLibrary>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_baseAddress(baseAddress);
      set_topAddress(topAddress);
    }
  }

  void commit(const char* name,
              u8 baseAddress,
              u8 topAddress) {
    if (should_commit()) {
      set_name(name);
      set_baseAddress(baseAddress);
      set_topAddress(topAddress);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     u8 baseAddress,
                     u8 topAddress) {
    EventNativeLibrary me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_baseAddress(baseAddress);
      me.set_topAddress(topAddress);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_baseAddress");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_topAddress");
  }
#endif
};

class EventNativeLibraryLoad : public JfrEvent<EventNativeLibraryLoad>
{
 private:
  const char* _name;
  bool _success;
  const char* _errorMessage;
  bool _fpuCorrectionAttempt;
  bool _fpuCorrectionSuccess;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrNativeLibraryLoadEvent;

  EventNativeLibraryLoad(EventStartTime timing=TIMED) : JfrEvent<EventNativeLibraryLoad>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_success(bool new_value) {
    this->_success = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_errorMessage(const char* new_value) {
    this->_errorMessage = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_fpuCorrectionAttempt(bool new_value) {
    this->_fpuCorrectionAttempt = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_fpuCorrectionSuccess(bool new_value) {
    this->_fpuCorrectionSuccess = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_success);
    w.write(_errorMessage);
    w.write(_fpuCorrectionAttempt);
    w.write(_fpuCorrectionSuccess);
  }

  using JfrEvent<EventNativeLibraryLoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeLibraryLoad(
    const char* name,
    bool success,
    const char* errorMessage,
    bool fpuCorrectionAttempt,
    bool fpuCorrectionSuccess) : JfrEvent<EventNativeLibraryLoad>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_success(success);
      set_errorMessage(errorMessage);
      set_fpuCorrectionAttempt(fpuCorrectionAttempt);
      set_fpuCorrectionSuccess(fpuCorrectionSuccess);
    }
  }

  void commit(const char* name,
              bool success,
              const char* errorMessage,
              bool fpuCorrectionAttempt,
              bool fpuCorrectionSuccess) {
    if (should_commit()) {
      set_name(name);
      set_success(success);
      set_errorMessage(errorMessage);
      set_fpuCorrectionAttempt(fpuCorrectionAttempt);
      set_fpuCorrectionSuccess(fpuCorrectionSuccess);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     bool success,
                     const char* errorMessage,
                     bool fpuCorrectionAttempt,
                     bool fpuCorrectionSuccess) {
    EventNativeLibraryLoad me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_success(success);
      me.set_errorMessage(errorMessage);
      me.set_fpuCorrectionAttempt(fpuCorrectionAttempt);
      me.set_fpuCorrectionSuccess(fpuCorrectionSuccess);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_success");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_errorMessage");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_fpuCorrectionAttempt");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_fpuCorrectionSuccess");
  }
#endif
};

class EventNativeLibraryUnload : public JfrEvent<EventNativeLibraryUnload>
{
 private:
  const char* _name;
  bool _success;
  const char* _errorMessage;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrNativeLibraryUnloadEvent;

  EventNativeLibraryUnload(EventStartTime timing=TIMED) : JfrEvent<EventNativeLibraryUnload>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_success(bool new_value) {
    this->_success = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_errorMessage(const char* new_value) {
    this->_errorMessage = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_success);
    w.write(_errorMessage);
  }

  using JfrEvent<EventNativeLibraryUnload>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeLibraryUnload(
    const char* name,
    bool success,
    const char* errorMessage) : JfrEvent<EventNativeLibraryUnload>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_success(success);
      set_errorMessage(errorMessage);
    }
  }

  void commit(const char* name,
              bool success,
              const char* errorMessage) {
    if (should_commit()) {
      set_name(name);
      set_success(success);
      set_errorMessage(errorMessage);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     bool success,
                     const char* errorMessage) {
    EventNativeLibraryUnload me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_success(success);
      me.set_errorMessage(errorMessage);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_success");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_errorMessage");
  }
#endif
};

class EventModuleRequire : public JfrEvent<EventModuleRequire>
{
 private:
  const ModuleEntry* _source;
  const ModuleEntry* _requiredModule;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrModuleRequireEvent;

  EventModuleRequire(EventStartTime timing=TIMED) : JfrEvent<EventModuleRequire>(timing) {}

  void set_source(const ModuleEntry* new_value) {
    this->_source = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_requiredModule(const ModuleEntry* new_value) {
    this->_requiredModule = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_source);
    w.write(_requiredModule);
  }

  using JfrEvent<EventModuleRequire>::commit; // else commit() is hidden by overloaded versions in this class

  EventModuleRequire(
    const ModuleEntry* source,
    const ModuleEntry* requiredModule) : JfrEvent<EventModuleRequire>(TIMED) {
    if (should_commit()) {
      set_source(source);
      set_requiredModule(requiredModule);
    }
  }

  void commit(const ModuleEntry* source,
              const ModuleEntry* requiredModule) {
    if (should_commit()) {
      set_source(source);
      set_requiredModule(requiredModule);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const ModuleEntry* source,
                     const ModuleEntry* requiredModule) {
    EventModuleRequire me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_source(source);
      me.set_requiredModule(requiredModule);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_source");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_requiredModule");
  }
#endif
};

class EventModuleExport : public JfrEvent<EventModuleExport>
{
 private:
  const PackageEntry* _exportedPackage;
  const ModuleEntry* _targetModule;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrModuleExportEvent;

  EventModuleExport(EventStartTime timing=TIMED) : JfrEvent<EventModuleExport>(timing) {}

  void set_exportedPackage(const PackageEntry* new_value) {
    this->_exportedPackage = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_targetModule(const ModuleEntry* new_value) {
    this->_targetModule = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_exportedPackage);
    w.write(_targetModule);
  }

  using JfrEvent<EventModuleExport>::commit; // else commit() is hidden by overloaded versions in this class

  EventModuleExport(
    const PackageEntry* exportedPackage,
    const ModuleEntry* targetModule) : JfrEvent<EventModuleExport>(TIMED) {
    if (should_commit()) {
      set_exportedPackage(exportedPackage);
      set_targetModule(targetModule);
    }
  }

  void commit(const PackageEntry* exportedPackage,
              const ModuleEntry* targetModule) {
    if (should_commit()) {
      set_exportedPackage(exportedPackage);
      set_targetModule(targetModule);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const PackageEntry* exportedPackage,
                     const ModuleEntry* targetModule) {
    EventModuleExport me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_exportedPackage(exportedPackage);
      me.set_targetModule(targetModule);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_exportedPackage");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_targetModule");
  }
#endif
};

class EventCompilerStatistics : public JfrEvent<EventCompilerStatistics>
{
 private:
  s4 _compileCount;
  s4 _bailoutCount;
  s4 _invalidatedCount;
  s4 _osrCompileCount;
  s4 _standardCompileCount;
  u8 _osrBytesCompiled;
  u8 _standardBytesCompiled;
  u8 _nmethodsSize;
  u8 _nmethodCodeSize;
  s8 _peakTimeSpent;
  s8 _totalTimeSpent;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCompilerStatisticsEvent;

  EventCompilerStatistics(EventStartTime timing=TIMED) : JfrEvent<EventCompilerStatistics>(timing) {}

  void set_compileCount(s4 new_value) {
    this->_compileCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_bailoutCount(s4 new_value) {
    this->_bailoutCount = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_invalidatedCount(s4 new_value) {
    this->_invalidatedCount = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_osrCompileCount(s4 new_value) {
    this->_osrCompileCount = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_standardCompileCount(s4 new_value) {
    this->_standardCompileCount = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_osrBytesCompiled(u8 new_value) {
    this->_osrBytesCompiled = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_standardBytesCompiled(u8 new_value) {
    this->_standardBytesCompiled = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_nmethodsSize(u8 new_value) {
    this->_nmethodsSize = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_nmethodCodeSize(u8 new_value) {
    this->_nmethodCodeSize = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }
  void set_peakTimeSpent(s8 new_value) {
    this->_peakTimeSpent = new_value;
    DEBUG_ONLY(set_field_bit(9));
  }
  void set_totalTimeSpent(s8 new_value) {
    this->_totalTimeSpent = new_value;
    DEBUG_ONLY(set_field_bit(10));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_compileCount);
    w.write(_bailoutCount);
    w.write(_invalidatedCount);
    w.write(_osrCompileCount);
    w.write(_standardCompileCount);
    w.write(_osrBytesCompiled);
    w.write(_standardBytesCompiled);
    w.write(_nmethodsSize);
    w.write(_nmethodCodeSize);
    w.write(_peakTimeSpent);
    w.write(_totalTimeSpent);
  }

  using JfrEvent<EventCompilerStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 compileCount,
                     s4 bailoutCount,
                     s4 invalidatedCount,
                     s4 osrCompileCount,
                     s4 standardCompileCount,
                     u8 osrBytesCompiled,
                     u8 standardBytesCompiled,
                     u8 nmethodsSize,
                     u8 nmethodCodeSize,
                     s8 peakTimeSpent,
                     s8 totalTimeSpent) {
    EventCompilerStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_compileCount(compileCount);
      me.set_bailoutCount(bailoutCount);
      me.set_invalidatedCount(invalidatedCount);
      me.set_osrCompileCount(osrCompileCount);
      me.set_standardCompileCount(standardCompileCount);
      me.set_osrBytesCompiled(osrBytesCompiled);
      me.set_standardBytesCompiled(standardBytesCompiled);
      me.set_nmethodsSize(nmethodsSize);
      me.set_nmethodCodeSize(nmethodCodeSize);
      me.set_peakTimeSpent(peakTimeSpent);
      me.set_totalTimeSpent(totalTimeSpent);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_compileCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_bailoutCount");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_invalidatedCount");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_osrCompileCount");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_standardCompileCount");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_osrBytesCompiled");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_standardBytesCompiled");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_nmethodsSize");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_nmethodCodeSize");
    assert(verify_field_bit(9), "Attempting to write an uninitialized event field: %s", "_peakTimeSpent");
    assert(verify_field_bit(10), "Attempting to write an uninitialized event field: %s", "_totalTimeSpent");
  }
#endif
};

class EventCompilerConfiguration : public JfrEvent<EventCompilerConfiguration>
{
 private:
  s4 _threadCount;
  bool _tieredCompilation;
  bool _dynamicCompilerThreadCount;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCompilerConfigurationEvent;

  EventCompilerConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventCompilerConfiguration>(timing) {}

  void set_threadCount(s4 new_value) {
    this->_threadCount = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_tieredCompilation(bool new_value) {
    this->_tieredCompilation = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_dynamicCompilerThreadCount(bool new_value) {
    this->_dynamicCompilerThreadCount = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_threadCount);
    w.write(_tieredCompilation);
    w.write(_dynamicCompilerThreadCount);
  }

  using JfrEvent<EventCompilerConfiguration>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 threadCount,
                     bool tieredCompilation,
                     bool dynamicCompilerThreadCount) {
    EventCompilerConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_threadCount(threadCount);
      me.set_tieredCompilation(tieredCompilation);
      me.set_dynamicCompilerThreadCount(dynamicCompilerThreadCount);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_threadCount");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_tieredCompilation");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_dynamicCompilerThreadCount");
  }
#endif
};

class EventCodeCacheStatistics : public JfrEvent<EventCodeCacheStatistics>
{
 private:
  u8 _codeBlobType;
  u8 _startAddress;
  u8 _reservedTopAddress;
  s4 _entryCount;
  s4 _methodCount;
  s4 _adaptorCount;
  u8 _unallocatedCapacity;
  s4 _fullCount;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCodeCacheStatisticsEvent;

  EventCodeCacheStatistics(EventStartTime timing=TIMED) : JfrEvent<EventCodeCacheStatistics>(timing) {}

  void set_codeBlobType(u8 new_value) {
    this->_codeBlobType = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_startAddress(u8 new_value) {
    this->_startAddress = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_reservedTopAddress(u8 new_value) {
    this->_reservedTopAddress = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_entryCount(s4 new_value) {
    this->_entryCount = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_methodCount(s4 new_value) {
    this->_methodCount = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_adaptorCount(s4 new_value) {
    this->_adaptorCount = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_unallocatedCapacity(u8 new_value) {
    this->_unallocatedCapacity = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_fullCount(s4 new_value) {
    this->_fullCount = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_codeBlobType);
    w.write(_startAddress);
    w.write(_reservedTopAddress);
    w.write(_entryCount);
    w.write(_methodCount);
    w.write(_adaptorCount);
    w.write(_unallocatedCapacity);
    w.write(_fullCount);
  }

  using JfrEvent<EventCodeCacheStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 codeBlobType,
                     u8 startAddress,
                     u8 reservedTopAddress,
                     s4 entryCount,
                     s4 methodCount,
                     s4 adaptorCount,
                     u8 unallocatedCapacity,
                     s4 fullCount) {
    EventCodeCacheStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_codeBlobType(codeBlobType);
      me.set_startAddress(startAddress);
      me.set_reservedTopAddress(reservedTopAddress);
      me.set_entryCount(entryCount);
      me.set_methodCount(methodCount);
      me.set_adaptorCount(adaptorCount);
      me.set_unallocatedCapacity(unallocatedCapacity);
      me.set_fullCount(fullCount);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_codeBlobType");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_startAddress");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_reservedTopAddress");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_entryCount");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_methodCount");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_adaptorCount");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_unallocatedCapacity");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_fullCount");
  }
#endif
};

class EventCodeCacheConfiguration : public JfrEvent<EventCodeCacheConfiguration>
{
 private:
  u8 _initialSize;
  u8 _reservedSize;
  u8 _nonNMethodSize;
  u8 _profiledSize;
  u8 _nonProfiledSize;
  u8 _expansionSize;
  u8 _minBlockLength;
  u8 _startAddress;
  u8 _reservedTopAddress;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrCodeCacheConfigurationEvent;

  EventCodeCacheConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventCodeCacheConfiguration>(timing) {}

  void set_initialSize(u8 new_value) {
    this->_initialSize = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_reservedSize(u8 new_value) {
    this->_reservedSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_nonNMethodSize(u8 new_value) {
    this->_nonNMethodSize = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_profiledSize(u8 new_value) {
    this->_profiledSize = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_nonProfiledSize(u8 new_value) {
    this->_nonProfiledSize = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_expansionSize(u8 new_value) {
    this->_expansionSize = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_minBlockLength(u8 new_value) {
    this->_minBlockLength = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_startAddress(u8 new_value) {
    this->_startAddress = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_reservedTopAddress(u8 new_value) {
    this->_reservedTopAddress = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_initialSize);
    w.write(_reservedSize);
    w.write(_nonNMethodSize);
    w.write(_profiledSize);
    w.write(_nonProfiledSize);
    w.write(_expansionSize);
    w.write(_minBlockLength);
    w.write(_startAddress);
    w.write(_reservedTopAddress);
  }

  using JfrEvent<EventCodeCacheConfiguration>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 initialSize,
                     u8 reservedSize,
                     u8 nonNMethodSize,
                     u8 profiledSize,
                     u8 nonProfiledSize,
                     u8 expansionSize,
                     u8 minBlockLength,
                     u8 startAddress,
                     u8 reservedTopAddress) {
    EventCodeCacheConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_initialSize(initialSize);
      me.set_reservedSize(reservedSize);
      me.set_nonNMethodSize(nonNMethodSize);
      me.set_profiledSize(profiledSize);
      me.set_nonProfiledSize(nonProfiledSize);
      me.set_expansionSize(expansionSize);
      me.set_minBlockLength(minBlockLength);
      me.set_startAddress(startAddress);
      me.set_reservedTopAddress(reservedTopAddress);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_initialSize");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_reservedSize");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_nonNMethodSize");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_profiledSize");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_nonProfiledSize");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_expansionSize");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_minBlockLength");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_startAddress");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_reservedTopAddress");
  }
#endif
};

class EventIntFlag : public JfrEvent<EventIntFlag>
{
 private:
  const char* _name;
  s4 _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrIntFlagEvent;

  EventIntFlag(EventStartTime timing=TIMED) : JfrEvent<EventIntFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(s4 new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventIntFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventIntFlag(
    const char* name,
    s4 value,
    u8 origin) : JfrEvent<EventIntFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              s4 value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     s4 value,
                     u8 origin) {
    EventIntFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventUnsignedIntFlag : public JfrEvent<EventUnsignedIntFlag>
{
 private:
  const char* _name;
  unsigned _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrUnsignedIntFlagEvent;

  EventUnsignedIntFlag(EventStartTime timing=TIMED) : JfrEvent<EventUnsignedIntFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(unsigned new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventUnsignedIntFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventUnsignedIntFlag(
    const char* name,
    unsigned value,
    u8 origin) : JfrEvent<EventUnsignedIntFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              unsigned value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     unsigned value,
                     u8 origin) {
    EventUnsignedIntFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventLongFlag : public JfrEvent<EventLongFlag>
{
 private:
  const char* _name;
  s8 _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrLongFlagEvent;

  EventLongFlag(EventStartTime timing=TIMED) : JfrEvent<EventLongFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(s8 new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventLongFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventLongFlag(
    const char* name,
    s8 value,
    u8 origin) : JfrEvent<EventLongFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              s8 value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     s8 value,
                     u8 origin) {
    EventLongFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventUnsignedLongFlag : public JfrEvent<EventUnsignedLongFlag>
{
 private:
  const char* _name;
  u8 _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrUnsignedLongFlagEvent;

  EventUnsignedLongFlag(EventStartTime timing=TIMED) : JfrEvent<EventUnsignedLongFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(u8 new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventUnsignedLongFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventUnsignedLongFlag(
    const char* name,
    u8 value,
    u8 origin) : JfrEvent<EventUnsignedLongFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              u8 value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     u8 value,
                     u8 origin) {
    EventUnsignedLongFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventDoubleFlag : public JfrEvent<EventDoubleFlag>
{
 private:
  const char* _name;
  double _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrDoubleFlagEvent;

  EventDoubleFlag(EventStartTime timing=TIMED) : JfrEvent<EventDoubleFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(double new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventDoubleFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventDoubleFlag(
    const char* name,
    double value,
    u8 origin) : JfrEvent<EventDoubleFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              double value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     double value,
                     u8 origin) {
    EventDoubleFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventBooleanFlag : public JfrEvent<EventBooleanFlag>
{
 private:
  const char* _name;
  bool _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrBooleanFlagEvent;

  EventBooleanFlag(EventStartTime timing=TIMED) : JfrEvent<EventBooleanFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(bool new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventBooleanFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventBooleanFlag(
    const char* name,
    bool value,
    u8 origin) : JfrEvent<EventBooleanFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              bool value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     bool value,
                     u8 origin) {
    EventBooleanFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventStringFlag : public JfrEvent<EventStringFlag>
{
 private:
  const char* _name;
  const char* _value;
  u8 _origin;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrStringFlagEvent;

  EventStringFlag(EventStartTime timing=TIMED) : JfrEvent<EventStringFlag>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(const char* new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_origin(u8 new_value) {
    this->_origin = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_value);
    w.write(_origin);
  }

  using JfrEvent<EventStringFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventStringFlag(
    const char* name,
    const char* value,
    u8 origin) : JfrEvent<EventStringFlag>(TIMED) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
    }
  }

  void commit(const char* name,
              const char* value,
              u8 origin) {
    if (should_commit()) {
      set_name(name);
      set_value(value);
      set_origin(origin);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     const char* value,
                     u8 origin) {
    EventStringFlag me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_name(name);
      me.set_value(value);
      me.set_origin(origin);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_origin");
  }
#endif
};

class EventObjectCount : public JfrEvent<EventObjectCount>
{
 private:
  unsigned _gcId;
  const Klass* _objectClass;
  s8 _count;
  u8 _totalSize;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrObjectCountEvent;

  EventObjectCount(EventStartTime timing=TIMED) : JfrEvent<EventObjectCount>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_objectClass(const Klass* new_value) {
    this->_objectClass = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_count(s8 new_value) {
    this->_count = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_totalSize(u8 new_value) {
    this->_totalSize = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_objectClass);
    w.write(_count);
    w.write(_totalSize);
  }

  using JfrEvent<EventObjectCount>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     s8 count,
                     u8 totalSize) {
    EventObjectCount me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_objectClass(objectClass);
      me.set_count(count);
      me.set_totalSize(totalSize);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_objectClass");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_count");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_totalSize");
  }
#endif
};

class EventG1HeapRegionInformation : public JfrEvent<EventG1HeapRegionInformation>
{
 private:
  unsigned _index;
  u8 _type;
  u8 _start;
  u8 _used;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrG1HeapRegionInformationEvent;

  EventG1HeapRegionInformation(EventStartTime timing=TIMED) : JfrEvent<EventG1HeapRegionInformation>(timing) {}

  void set_index(unsigned new_value) {
    this->_index = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_start(u8 new_value) {
    this->_start = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_used(u8 new_value) {
    this->_used = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_index);
    w.write(_type);
    w.write(_start);
    w.write(_used);
  }

  using JfrEvent<EventG1HeapRegionInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventG1HeapRegionInformation(
    unsigned index,
    u8 type,
    u8 start,
    u8 used) : JfrEvent<EventG1HeapRegionInformation>(TIMED) {
    if (should_commit()) {
      set_index(index);
      set_type(type);
      set_start(start);
      set_used(used);
    }
  }

  void commit(unsigned index,
              u8 type,
              u8 start,
              u8 used) {
    if (should_commit()) {
      set_index(index);
      set_type(type);
      set_start(start);
      set_used(used);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned index,
                     u8 type,
                     u8 start,
                     u8 used) {
    EventG1HeapRegionInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_index(index);
      me.set_type(type);
      me.set_start(start);
      me.set_used(used);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_index");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_type");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_start");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_used");
  }
#endif
};

class EventGCConfiguration : public JfrEvent<EventGCConfiguration>
{
 private:
  u8 _youngCollector;
  u8 _oldCollector;
  unsigned _parallelGCThreads;
  unsigned _concurrentGCThreads;
  bool _usesDynamicGCThreads;
  bool _isExplicitGCConcurrent;
  bool _isExplicitGCDisabled;
  s8 _pauseTarget;
  unsigned _gcTimeRatio;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrGCConfigurationEvent;

  EventGCConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventGCConfiguration>(timing) {}

  void set_youngCollector(u8 new_value) {
    this->_youngCollector = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_oldCollector(u8 new_value) {
    this->_oldCollector = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_parallelGCThreads(unsigned new_value) {
    this->_parallelGCThreads = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_concurrentGCThreads(unsigned new_value) {
    this->_concurrentGCThreads = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_usesDynamicGCThreads(bool new_value) {
    this->_usesDynamicGCThreads = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_isExplicitGCConcurrent(bool new_value) {
    this->_isExplicitGCConcurrent = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_isExplicitGCDisabled(bool new_value) {
    this->_isExplicitGCDisabled = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_pauseTarget(s8 new_value) {
    this->_pauseTarget = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_gcTimeRatio(unsigned new_value) {
    this->_gcTimeRatio = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_youngCollector);
    w.write(_oldCollector);
    w.write(_parallelGCThreads);
    w.write(_concurrentGCThreads);
    w.write(_usesDynamicGCThreads);
    w.write(_isExplicitGCConcurrent);
    w.write(_isExplicitGCDisabled);
    w.write(_pauseTarget);
    w.write(_gcTimeRatio);
  }

  using JfrEvent<EventGCConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCConfiguration(
    u8 youngCollector,
    u8 oldCollector,
    unsigned parallelGCThreads,
    unsigned concurrentGCThreads,
    bool usesDynamicGCThreads,
    bool isExplicitGCConcurrent,
    bool isExplicitGCDisabled,
    s8 pauseTarget,
    unsigned gcTimeRatio) : JfrEvent<EventGCConfiguration>(TIMED) {
    if (should_commit()) {
      set_youngCollector(youngCollector);
      set_oldCollector(oldCollector);
      set_parallelGCThreads(parallelGCThreads);
      set_concurrentGCThreads(concurrentGCThreads);
      set_usesDynamicGCThreads(usesDynamicGCThreads);
      set_isExplicitGCConcurrent(isExplicitGCConcurrent);
      set_isExplicitGCDisabled(isExplicitGCDisabled);
      set_pauseTarget(pauseTarget);
      set_gcTimeRatio(gcTimeRatio);
    }
  }

  void commit(u8 youngCollector,
              u8 oldCollector,
              unsigned parallelGCThreads,
              unsigned concurrentGCThreads,
              bool usesDynamicGCThreads,
              bool isExplicitGCConcurrent,
              bool isExplicitGCDisabled,
              s8 pauseTarget,
              unsigned gcTimeRatio) {
    if (should_commit()) {
      set_youngCollector(youngCollector);
      set_oldCollector(oldCollector);
      set_parallelGCThreads(parallelGCThreads);
      set_concurrentGCThreads(concurrentGCThreads);
      set_usesDynamicGCThreads(usesDynamicGCThreads);
      set_isExplicitGCConcurrent(isExplicitGCConcurrent);
      set_isExplicitGCDisabled(isExplicitGCDisabled);
      set_pauseTarget(pauseTarget);
      set_gcTimeRatio(gcTimeRatio);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 youngCollector,
                     u8 oldCollector,
                     unsigned parallelGCThreads,
                     unsigned concurrentGCThreads,
                     bool usesDynamicGCThreads,
                     bool isExplicitGCConcurrent,
                     bool isExplicitGCDisabled,
                     s8 pauseTarget,
                     unsigned gcTimeRatio) {
    EventGCConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_youngCollector(youngCollector);
      me.set_oldCollector(oldCollector);
      me.set_parallelGCThreads(parallelGCThreads);
      me.set_concurrentGCThreads(concurrentGCThreads);
      me.set_usesDynamicGCThreads(usesDynamicGCThreads);
      me.set_isExplicitGCConcurrent(isExplicitGCConcurrent);
      me.set_isExplicitGCDisabled(isExplicitGCDisabled);
      me.set_pauseTarget(pauseTarget);
      me.set_gcTimeRatio(gcTimeRatio);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_youngCollector");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_oldCollector");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_parallelGCThreads");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_concurrentGCThreads");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_usesDynamicGCThreads");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_isExplicitGCConcurrent");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_isExplicitGCDisabled");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_pauseTarget");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_gcTimeRatio");
  }
#endif
};

class EventGCSurvivorConfiguration : public JfrEvent<EventGCSurvivorConfiguration>
{
 private:
  u1 _maxTenuringThreshold;
  u1 _initialTenuringThreshold;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrGCSurvivorConfigurationEvent;

  EventGCSurvivorConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventGCSurvivorConfiguration>(timing) {}

  void set_maxTenuringThreshold(u1 new_value) {
    this->_maxTenuringThreshold = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_initialTenuringThreshold(u1 new_value) {
    this->_initialTenuringThreshold = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_maxTenuringThreshold);
    w.write(_initialTenuringThreshold);
  }

  using JfrEvent<EventGCSurvivorConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCSurvivorConfiguration(
    u1 maxTenuringThreshold,
    u1 initialTenuringThreshold) : JfrEvent<EventGCSurvivorConfiguration>(TIMED) {
    if (should_commit()) {
      set_maxTenuringThreshold(maxTenuringThreshold);
      set_initialTenuringThreshold(initialTenuringThreshold);
    }
  }

  void commit(u1 maxTenuringThreshold,
              u1 initialTenuringThreshold) {
    if (should_commit()) {
      set_maxTenuringThreshold(maxTenuringThreshold);
      set_initialTenuringThreshold(initialTenuringThreshold);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u1 maxTenuringThreshold,
                     u1 initialTenuringThreshold) {
    EventGCSurvivorConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_maxTenuringThreshold(maxTenuringThreshold);
      me.set_initialTenuringThreshold(initialTenuringThreshold);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_maxTenuringThreshold");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_initialTenuringThreshold");
  }
#endif
};

class EventGCTLABConfiguration : public JfrEvent<EventGCTLABConfiguration>
{
 private:
  bool _usesTLABs;
  u8 _minTLABSize;
  u8 _tlabRefillWasteLimit;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrGCTLABConfigurationEvent;

  EventGCTLABConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventGCTLABConfiguration>(timing) {}

  void set_usesTLABs(bool new_value) {
    this->_usesTLABs = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_minTLABSize(u8 new_value) {
    this->_minTLABSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_tlabRefillWasteLimit(u8 new_value) {
    this->_tlabRefillWasteLimit = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_usesTLABs);
    w.write(_minTLABSize);
    w.write(_tlabRefillWasteLimit);
  }

  using JfrEvent<EventGCTLABConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCTLABConfiguration(
    bool usesTLABs,
    u8 minTLABSize,
    u8 tlabRefillWasteLimit) : JfrEvent<EventGCTLABConfiguration>(TIMED) {
    if (should_commit()) {
      set_usesTLABs(usesTLABs);
      set_minTLABSize(minTLABSize);
      set_tlabRefillWasteLimit(tlabRefillWasteLimit);
    }
  }

  void commit(bool usesTLABs,
              u8 minTLABSize,
              u8 tlabRefillWasteLimit) {
    if (should_commit()) {
      set_usesTLABs(usesTLABs);
      set_minTLABSize(minTLABSize);
      set_tlabRefillWasteLimit(tlabRefillWasteLimit);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     bool usesTLABs,
                     u8 minTLABSize,
                     u8 tlabRefillWasteLimit) {
    EventGCTLABConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_usesTLABs(usesTLABs);
      me.set_minTLABSize(minTLABSize);
      me.set_tlabRefillWasteLimit(tlabRefillWasteLimit);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_usesTLABs");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_minTLABSize");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_tlabRefillWasteLimit");
  }
#endif
};

class EventGCHeapConfiguration : public JfrEvent<EventGCHeapConfiguration>
{
 private:
  u8 _minSize;
  u8 _maxSize;
  u8 _initialSize;
  bool _usesCompressedOops;
  u8 _compressedOopsMode;
  u8 _objectAlignment;
  u1 _heapAddressBits;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrGCHeapConfigurationEvent;

  EventGCHeapConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventGCHeapConfiguration>(timing) {}

  void set_minSize(u8 new_value) {
    this->_minSize = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_maxSize(u8 new_value) {
    this->_maxSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_initialSize(u8 new_value) {
    this->_initialSize = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_usesCompressedOops(bool new_value) {
    this->_usesCompressedOops = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_compressedOopsMode(u8 new_value) {
    this->_compressedOopsMode = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_objectAlignment(u8 new_value) {
    this->_objectAlignment = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_heapAddressBits(u1 new_value) {
    this->_heapAddressBits = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_minSize);
    w.write(_maxSize);
    w.write(_initialSize);
    w.write(_usesCompressedOops);
    w.write(_compressedOopsMode);
    w.write(_objectAlignment);
    w.write(_heapAddressBits);
  }

  using JfrEvent<EventGCHeapConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCHeapConfiguration(
    u8 minSize,
    u8 maxSize,
    u8 initialSize,
    bool usesCompressedOops,
    u8 compressedOopsMode,
    u8 objectAlignment,
    u1 heapAddressBits) : JfrEvent<EventGCHeapConfiguration>(TIMED) {
    if (should_commit()) {
      set_minSize(minSize);
      set_maxSize(maxSize);
      set_initialSize(initialSize);
      set_usesCompressedOops(usesCompressedOops);
      set_compressedOopsMode(compressedOopsMode);
      set_objectAlignment(objectAlignment);
      set_heapAddressBits(heapAddressBits);
    }
  }

  void commit(u8 minSize,
              u8 maxSize,
              u8 initialSize,
              bool usesCompressedOops,
              u8 compressedOopsMode,
              u8 objectAlignment,
              u1 heapAddressBits) {
    if (should_commit()) {
      set_minSize(minSize);
      set_maxSize(maxSize);
      set_initialSize(initialSize);
      set_usesCompressedOops(usesCompressedOops);
      set_compressedOopsMode(compressedOopsMode);
      set_objectAlignment(objectAlignment);
      set_heapAddressBits(heapAddressBits);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 minSize,
                     u8 maxSize,
                     u8 initialSize,
                     bool usesCompressedOops,
                     u8 compressedOopsMode,
                     u8 objectAlignment,
                     u1 heapAddressBits) {
    EventGCHeapConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_minSize(minSize);
      me.set_maxSize(maxSize);
      me.set_initialSize(initialSize);
      me.set_usesCompressedOops(usesCompressedOops);
      me.set_compressedOopsMode(compressedOopsMode);
      me.set_objectAlignment(objectAlignment);
      me.set_heapAddressBits(heapAddressBits);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_minSize");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_maxSize");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_initialSize");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_usesCompressedOops");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_compressedOopsMode");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_objectAlignment");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_heapAddressBits");
  }
#endif
};

class EventYoungGenerationConfiguration : public JfrEvent<EventYoungGenerationConfiguration>
{
 private:
  u8 _minSize;
  u8 _maxSize;
  unsigned _newRatio;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrYoungGenerationConfigurationEvent;

  EventYoungGenerationConfiguration(EventStartTime timing=TIMED) : JfrEvent<EventYoungGenerationConfiguration>(timing) {}

  void set_minSize(u8 new_value) {
    this->_minSize = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_maxSize(u8 new_value) {
    this->_maxSize = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_newRatio(unsigned new_value) {
    this->_newRatio = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_minSize);
    w.write(_maxSize);
    w.write(_newRatio);
  }

  using JfrEvent<EventYoungGenerationConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventYoungGenerationConfiguration(
    u8 minSize,
    u8 maxSize,
    unsigned newRatio) : JfrEvent<EventYoungGenerationConfiguration>(TIMED) {
    if (should_commit()) {
      set_minSize(minSize);
      set_maxSize(maxSize);
      set_newRatio(newRatio);
    }
  }

  void commit(u8 minSize,
              u8 maxSize,
              unsigned newRatio) {
    if (should_commit()) {
      set_minSize(minSize);
      set_maxSize(maxSize);
      set_newRatio(newRatio);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 minSize,
                     u8 maxSize,
                     unsigned newRatio) {
    EventYoungGenerationConfiguration me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_minSize(minSize);
      me.set_maxSize(maxSize);
      me.set_newRatio(newRatio);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_minSize");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_maxSize");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_newRatio");
  }
#endif
};

class EventZYoungGarbageCollection : public JfrEvent<EventZYoungGarbageCollection>
{
 private:
  unsigned _gcId;
  unsigned _tenuringThreshold;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZYoungGarbageCollectionEvent;

  EventZYoungGarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventZYoungGarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_tenuringThreshold(unsigned new_value) {
    this->_tenuringThreshold = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_tenuringThreshold);
  }

  using JfrEvent<EventZYoungGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventZYoungGarbageCollection(
    unsigned gcId,
    unsigned tenuringThreshold) : JfrEvent<EventZYoungGarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_tenuringThreshold(tenuringThreshold);
    }
  }

  void commit(unsigned gcId,
              unsigned tenuringThreshold) {
    if (should_commit()) {
      set_gcId(gcId);
      set_tenuringThreshold(tenuringThreshold);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     unsigned tenuringThreshold) {
    EventZYoungGarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_tenuringThreshold(tenuringThreshold);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_tenuringThreshold");
  }
#endif
};

class EventZOldGarbageCollection : public JfrEvent<EventZOldGarbageCollection>
{
 private:
  unsigned _gcId;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZOldGarbageCollectionEvent;

  EventZOldGarbageCollection(EventStartTime timing=TIMED) : JfrEvent<EventZOldGarbageCollection>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
  }

  using JfrEvent<EventZOldGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventZOldGarbageCollection(
    unsigned gcId) : JfrEvent<EventZOldGarbageCollection>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
    }
  }

  void commit(unsigned gcId) {
    if (should_commit()) {
      set_gcId(gcId);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId) {
    EventZOldGarbageCollection me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
  }
#endif
};

class EventZAllocationStall : public JfrEvent<EventZAllocationStall>
{
 private:
  u8 _type;
  u8 _size;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZAllocationStallEvent;

  EventZAllocationStall(EventStartTime timing=TIMED) : JfrEvent<EventZAllocationStall>(timing) {}

  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_type);
    w.write(_size);
  }

  using JfrEvent<EventZAllocationStall>::commit; // else commit() is hidden by overloaded versions in this class

  EventZAllocationStall(
    u8 type,
    u8 size) : JfrEvent<EventZAllocationStall>(TIMED) {
    if (should_commit()) {
      set_type(type);
      set_size(size);
    }
  }

  void commit(u8 type,
              u8 size) {
    if (should_commit()) {
      set_type(type);
      set_size(size);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 size) {
    EventZAllocationStall me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_type(type);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_type");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventZPageAllocation : public JfrEvent<EventZPageAllocation>
{
 private:
  u8 _type;
  u8 _size;
  u8 _harvested;
  u8 _committed;
  unsigned _harvestedRanges;
  bool _multiPartition;
  bool _successful;
  bool _nonBlocking;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZPageAllocationEvent;

  EventZPageAllocation(EventStartTime timing=TIMED) : JfrEvent<EventZPageAllocation>(timing) {}

  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_harvested(u8 new_value) {
    this->_harvested = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_committed(u8 new_value) {
    this->_committed = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_harvestedRanges(unsigned new_value) {
    this->_harvestedRanges = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_multiPartition(bool new_value) {
    this->_multiPartition = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_successful(bool new_value) {
    this->_successful = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_nonBlocking(bool new_value) {
    this->_nonBlocking = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_type);
    w.write(_size);
    w.write(_harvested);
    w.write(_committed);
    w.write(_harvestedRanges);
    w.write(_multiPartition);
    w.write(_successful);
    w.write(_nonBlocking);
  }

  using JfrEvent<EventZPageAllocation>::commit; // else commit() is hidden by overloaded versions in this class

  EventZPageAllocation(
    u8 type,
    u8 size,
    u8 harvested,
    u8 committed,
    unsigned harvestedRanges,
    bool multiPartition,
    bool successful,
    bool nonBlocking) : JfrEvent<EventZPageAllocation>(TIMED) {
    if (should_commit()) {
      set_type(type);
      set_size(size);
      set_harvested(harvested);
      set_committed(committed);
      set_harvestedRanges(harvestedRanges);
      set_multiPartition(multiPartition);
      set_successful(successful);
      set_nonBlocking(nonBlocking);
    }
  }

  void commit(u8 type,
              u8 size,
              u8 harvested,
              u8 committed,
              unsigned harvestedRanges,
              bool multiPartition,
              bool successful,
              bool nonBlocking) {
    if (should_commit()) {
      set_type(type);
      set_size(size);
      set_harvested(harvested);
      set_committed(committed);
      set_harvestedRanges(harvestedRanges);
      set_multiPartition(multiPartition);
      set_successful(successful);
      set_nonBlocking(nonBlocking);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 size,
                     u8 harvested,
                     u8 committed,
                     unsigned harvestedRanges,
                     bool multiPartition,
                     bool successful,
                     bool nonBlocking) {
    EventZPageAllocation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_type(type);
      me.set_size(size);
      me.set_harvested(harvested);
      me.set_committed(committed);
      me.set_harvestedRanges(harvestedRanges);
      me.set_multiPartition(multiPartition);
      me.set_successful(successful);
      me.set_nonBlocking(nonBlocking);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_type");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_size");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_harvested");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_committed");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_harvestedRanges");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_multiPartition");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_successful");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_nonBlocking");
  }
#endif
};

class EventZRelocationSet : public JfrEvent<EventZRelocationSet>
{
 private:
  u8 _total;
  u8 _empty;
  u8 _relocate;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZRelocationSetEvent;

  EventZRelocationSet(EventStartTime timing=TIMED) : JfrEvent<EventZRelocationSet>(timing) {}

  void set_total(u8 new_value) {
    this->_total = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_empty(u8 new_value) {
    this->_empty = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_relocate(u8 new_value) {
    this->_relocate = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_total);
    w.write(_empty);
    w.write(_relocate);
  }

  using JfrEvent<EventZRelocationSet>::commit; // else commit() is hidden by overloaded versions in this class

  EventZRelocationSet(
    u8 total,
    u8 empty,
    u8 relocate) : JfrEvent<EventZRelocationSet>(TIMED) {
    if (should_commit()) {
      set_total(total);
      set_empty(empty);
      set_relocate(relocate);
    }
  }

  void commit(u8 total,
              u8 empty,
              u8 relocate) {
    if (should_commit()) {
      set_total(total);
      set_empty(empty);
      set_relocate(relocate);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 total,
                     u8 empty,
                     u8 relocate) {
    EventZRelocationSet me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_total(total);
      me.set_empty(empty);
      me.set_relocate(relocate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_total");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_empty");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_relocate");
  }
#endif
};

class EventZRelocationSetGroup : public JfrEvent<EventZRelocationSetGroup>
{
 private:
  u8 _type;
  u8 _candidatePages;
  u8 _total;
  u8 _empty;
  u8 _selectedPages;
  u8 _relocate;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZRelocationSetGroupEvent;

  EventZRelocationSetGroup(EventStartTime timing=TIMED) : JfrEvent<EventZRelocationSetGroup>(timing) {}

  void set_type(u8 new_value) {
    this->_type = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_candidatePages(u8 new_value) {
    this->_candidatePages = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_total(u8 new_value) {
    this->_total = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_empty(u8 new_value) {
    this->_empty = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_selectedPages(u8 new_value) {
    this->_selectedPages = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_relocate(u8 new_value) {
    this->_relocate = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_type);
    w.write(_candidatePages);
    w.write(_total);
    w.write(_empty);
    w.write(_selectedPages);
    w.write(_relocate);
  }

  using JfrEvent<EventZRelocationSetGroup>::commit; // else commit() is hidden by overloaded versions in this class

  EventZRelocationSetGroup(
    u8 type,
    u8 candidatePages,
    u8 total,
    u8 empty,
    u8 selectedPages,
    u8 relocate) : JfrEvent<EventZRelocationSetGroup>(TIMED) {
    if (should_commit()) {
      set_type(type);
      set_candidatePages(candidatePages);
      set_total(total);
      set_empty(empty);
      set_selectedPages(selectedPages);
      set_relocate(relocate);
    }
  }

  void commit(u8 type,
              u8 candidatePages,
              u8 total,
              u8 empty,
              u8 selectedPages,
              u8 relocate) {
    if (should_commit()) {
      set_type(type);
      set_candidatePages(candidatePages);
      set_total(total);
      set_empty(empty);
      set_selectedPages(selectedPages);
      set_relocate(relocate);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 candidatePages,
                     u8 total,
                     u8 empty,
                     u8 selectedPages,
                     u8 relocate) {
    EventZRelocationSetGroup me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_type(type);
      me.set_candidatePages(candidatePages);
      me.set_total(total);
      me.set_empty(empty);
      me.set_selectedPages(selectedPages);
      me.set_relocate(relocate);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_type");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_candidatePages");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_total");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_empty");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_selectedPages");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_relocate");
  }
#endif
};

class EventZStatisticsCounter : public JfrEvent<EventZStatisticsCounter>
{
 private:
  u8 _id;
  u8 _increment;
  u8 _value;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZStatisticsCounterEvent;

  EventZStatisticsCounter(EventStartTime timing=TIMED) : JfrEvent<EventZStatisticsCounter>(timing) {}

  void set_id(u8 new_value) {
    this->_id = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_increment(u8 new_value) {
    this->_increment = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_value(u8 new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_id);
    w.write(_increment);
    w.write(_value);
  }

  using JfrEvent<EventZStatisticsCounter>::commit; // else commit() is hidden by overloaded versions in this class

  EventZStatisticsCounter(
    u8 id,
    u8 increment,
    u8 value) : JfrEvent<EventZStatisticsCounter>(TIMED) {
    if (should_commit()) {
      set_id(id);
      set_increment(increment);
      set_value(value);
    }
  }

  void commit(u8 id,
              u8 increment,
              u8 value) {
    if (should_commit()) {
      set_id(id);
      set_increment(increment);
      set_value(value);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 id,
                     u8 increment,
                     u8 value) {
    EventZStatisticsCounter me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_id(id);
      me.set_increment(increment);
      me.set_value(value);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_id");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_increment");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_value");
  }
#endif
};

class EventZStatisticsSampler : public JfrEvent<EventZStatisticsSampler>
{
 private:
  u8 _id;
  u8 _value;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZStatisticsSamplerEvent;

  EventZStatisticsSampler(EventStartTime timing=TIMED) : JfrEvent<EventZStatisticsSampler>(timing) {}

  void set_id(u8 new_value) {
    this->_id = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_value(u8 new_value) {
    this->_value = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_id);
    w.write(_value);
  }

  using JfrEvent<EventZStatisticsSampler>::commit; // else commit() is hidden by overloaded versions in this class

  EventZStatisticsSampler(
    u8 id,
    u8 value) : JfrEvent<EventZStatisticsSampler>(TIMED) {
    if (should_commit()) {
      set_id(id);
      set_value(value);
    }
  }

  void commit(u8 id,
              u8 value) {
    if (should_commit()) {
      set_id(id);
      set_value(value);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 id,
                     u8 value) {
    EventZStatisticsSampler me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_id(id);
      me.set_value(value);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_id");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_value");
  }
#endif
};

class EventZThreadPhase : public JfrEvent<EventZThreadPhase>
{
 private:
  unsigned _gcId;
  const char* _name;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZThreadPhaseEvent;

  EventZThreadPhase(EventStartTime timing=TIMED) : JfrEvent<EventZThreadPhase>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_name);
  }

  using JfrEvent<EventZThreadPhase>::commit; // else commit() is hidden by overloaded versions in this class

  EventZThreadPhase(
    unsigned gcId,
    const char* name) : JfrEvent<EventZThreadPhase>(TIMED) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
    }
  }

  void commit(unsigned gcId,
              const char* name) {
    if (should_commit()) {
      set_gcId(gcId);
      set_name(name);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
    EventZThreadPhase me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_gcId(gcId);
      me.set_name(name);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_name");
  }
#endif
};

class EventZUncommit : public JfrEvent<EventZUncommit>
{
 private:
  u8 _uncommitted;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrZUncommitEvent;

  EventZUncommit(EventStartTime timing=TIMED) : JfrEvent<EventZUncommit>(timing) {}

  void set_uncommitted(u8 new_value) {
    this->_uncommitted = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_uncommitted);
  }

  using JfrEvent<EventZUncommit>::commit; // else commit() is hidden by overloaded versions in this class

  EventZUncommit(
    u8 uncommitted) : JfrEvent<EventZUncommit>(TIMED) {
    if (should_commit()) {
      set_uncommitted(uncommitted);
    }
  }

  void commit(u8 uncommitted) {
    if (should_commit()) {
      set_uncommitted(uncommitted);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 uncommitted) {
    EventZUncommit me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_uncommitted(uncommitted);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_uncommitted");
  }
#endif
};

class EventShenandoahHeapRegionStateChange : public JfrEvent<EventShenandoahHeapRegionStateChange>
{
 private:
  unsigned _index;
  u8 _from;
  u8 _to;
  u8 _start;
  u8 _used;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrShenandoahHeapRegionStateChangeEvent;

  EventShenandoahHeapRegionStateChange(EventStartTime timing=TIMED) : JfrEvent<EventShenandoahHeapRegionStateChange>(timing) {}

  void set_index(unsigned new_value) {
    this->_index = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_from(u8 new_value) {
    this->_from = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_to(u8 new_value) {
    this->_to = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_start(u8 new_value) {
    this->_start = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_used(u8 new_value) {
    this->_used = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_index);
    w.write(_from);
    w.write(_to);
    w.write(_start);
    w.write(_used);
  }

  using JfrEvent<EventShenandoahHeapRegionStateChange>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned index,
                     u8 from,
                     u8 to,
                     u8 start,
                     u8 used) {
    EventShenandoahHeapRegionStateChange me(UNTIMED);

    if (me.should_commit()) {
      me.set_index(index);
      me.set_from(from);
      me.set_to(to);
      me.set_start(start);
      me.set_used(used);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_index");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_from");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_to");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_start");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_used");
  }
#endif
};

class EventShenandoahHeapRegionInformation : public JfrEvent<EventShenandoahHeapRegionInformation>
{
 private:
  unsigned _index;
  u8 _state;
  u8 _start;
  u8 _used;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrShenandoahHeapRegionInformationEvent;

  EventShenandoahHeapRegionInformation(EventStartTime timing=TIMED) : JfrEvent<EventShenandoahHeapRegionInformation>(timing) {}

  void set_index(unsigned new_value) {
    this->_index = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_state(u8 new_value) {
    this->_state = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_start(u8 new_value) {
    this->_start = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_used(u8 new_value) {
    this->_used = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_index);
    w.write(_state);
    w.write(_start);
    w.write(_used);
  }

  using JfrEvent<EventShenandoahHeapRegionInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventShenandoahHeapRegionInformation(
    unsigned index,
    u8 state,
    u8 start,
    u8 used) : JfrEvent<EventShenandoahHeapRegionInformation>(TIMED) {
    if (should_commit()) {
      set_index(index);
      set_state(state);
      set_start(start);
      set_used(used);
    }
  }

  void commit(unsigned index,
              u8 state,
              u8 start,
              u8 used) {
    if (should_commit()) {
      set_index(index);
      set_state(state);
      set_start(start);
      set_used(used);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned index,
                     u8 state,
                     u8 start,
                     u8 used) {
    EventShenandoahHeapRegionInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_index(index);
      me.set_state(state);
      me.set_start(start);
      me.set_used(used);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_index");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_state");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_start");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_used");
  }
#endif
};

class EventShenandoahEvacuationInformation : public JfrEvent<EventShenandoahEvacuationInformation>
{
 private:
  unsigned _gcId;
  u8 _cSetRegions;
  u8 _cSetUsedBefore;
  u8 _cSetUsedAfter;
  u8 _collectedOld;
  u8 _collectedPromoted;
  u8 _collectedYoung;
  u8 _regionsPromotedHumongous;
  u8 _regionsPromotedRegular;
  u8 _regularPromotedGarbage;
  u8 _regularPromotedFree;
  u8 _freeRegions;
  u8 _regionsImmediate;
  u8 _immediateBytes;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrShenandoahEvacuationInformationEvent;

  EventShenandoahEvacuationInformation(EventStartTime timing=TIMED) : JfrEvent<EventShenandoahEvacuationInformation>(timing) {}

  void set_gcId(unsigned new_value) {
    this->_gcId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_cSetRegions(u8 new_value) {
    this->_cSetRegions = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_cSetUsedBefore(u8 new_value) {
    this->_cSetUsedBefore = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_cSetUsedAfter(u8 new_value) {
    this->_cSetUsedAfter = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_collectedOld(u8 new_value) {
    this->_collectedOld = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_collectedPromoted(u8 new_value) {
    this->_collectedPromoted = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }
  void set_collectedYoung(u8 new_value) {
    this->_collectedYoung = new_value;
    DEBUG_ONLY(set_field_bit(6));
  }
  void set_regionsPromotedHumongous(u8 new_value) {
    this->_regionsPromotedHumongous = new_value;
    DEBUG_ONLY(set_field_bit(7));
  }
  void set_regionsPromotedRegular(u8 new_value) {
    this->_regionsPromotedRegular = new_value;
    DEBUG_ONLY(set_field_bit(8));
  }
  void set_regularPromotedGarbage(u8 new_value) {
    this->_regularPromotedGarbage = new_value;
    DEBUG_ONLY(set_field_bit(9));
  }
  void set_regularPromotedFree(u8 new_value) {
    this->_regularPromotedFree = new_value;
    DEBUG_ONLY(set_field_bit(10));
  }
  void set_freeRegions(u8 new_value) {
    this->_freeRegions = new_value;
    DEBUG_ONLY(set_field_bit(11));
  }
  void set_regionsImmediate(u8 new_value) {
    this->_regionsImmediate = new_value;
    DEBUG_ONLY(set_field_bit(12));
  }
  void set_immediateBytes(u8 new_value) {
    this->_immediateBytes = new_value;
    DEBUG_ONLY(set_field_bit(13));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_gcId);
    w.write(_cSetRegions);
    w.write(_cSetUsedBefore);
    w.write(_cSetUsedAfter);
    w.write(_collectedOld);
    w.write(_collectedPromoted);
    w.write(_collectedYoung);
    w.write(_regionsPromotedHumongous);
    w.write(_regionsPromotedRegular);
    w.write(_regularPromotedGarbage);
    w.write(_regularPromotedFree);
    w.write(_freeRegions);
    w.write(_regionsImmediate);
    w.write(_immediateBytes);
  }

  using JfrEvent<EventShenandoahEvacuationInformation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 cSetRegions,
                     u8 cSetUsedBefore,
                     u8 cSetUsedAfter,
                     u8 collectedOld,
                     u8 collectedPromoted,
                     u8 collectedYoung,
                     u8 regionsPromotedHumongous,
                     u8 regionsPromotedRegular,
                     u8 regularPromotedGarbage,
                     u8 regularPromotedFree,
                     u8 freeRegions,
                     u8 regionsImmediate,
                     u8 immediateBytes) {
    EventShenandoahEvacuationInformation me(UNTIMED);

    if (me.should_commit()) {
      me.set_gcId(gcId);
      me.set_cSetRegions(cSetRegions);
      me.set_cSetUsedBefore(cSetUsedBefore);
      me.set_cSetUsedAfter(cSetUsedAfter);
      me.set_collectedOld(collectedOld);
      me.set_collectedPromoted(collectedPromoted);
      me.set_collectedYoung(collectedYoung);
      me.set_regionsPromotedHumongous(regionsPromotedHumongous);
      me.set_regionsPromotedRegular(regionsPromotedRegular);
      me.set_regularPromotedGarbage(regularPromotedGarbage);
      me.set_regularPromotedFree(regularPromotedFree);
      me.set_freeRegions(freeRegions);
      me.set_regionsImmediate(regionsImmediate);
      me.set_immediateBytes(immediateBytes);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_gcId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_cSetRegions");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_cSetUsedBefore");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_cSetUsedAfter");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_collectedOld");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_collectedPromoted");
    assert(verify_field_bit(6), "Attempting to write an uninitialized event field: %s", "_collectedYoung");
    assert(verify_field_bit(7), "Attempting to write an uninitialized event field: %s", "_regionsPromotedHumongous");
    assert(verify_field_bit(8), "Attempting to write an uninitialized event field: %s", "_regionsPromotedRegular");
    assert(verify_field_bit(9), "Attempting to write an uninitialized event field: %s", "_regularPromotedGarbage");
    assert(verify_field_bit(10), "Attempting to write an uninitialized event field: %s", "_regularPromotedFree");
    assert(verify_field_bit(11), "Attempting to write an uninitialized event field: %s", "_freeRegions");
    assert(verify_field_bit(12), "Attempting to write an uninitialized event field: %s", "_regionsImmediate");
    assert(verify_field_bit(13), "Attempting to write an uninitialized event field: %s", "_immediateBytes");
  }
#endif
};

class EventFlush : public JfrEvent<EventFlush>
{
 private:
  u8 _flushId;
  u8 _elements;
  u8 _size;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrFlushEvent;

  EventFlush(EventStartTime timing=TIMED) : JfrEvent<EventFlush>(timing) {}

  void set_flushId(u8 new_value) {
    this->_flushId = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_elements(u8 new_value) {
    this->_elements = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_size(u8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_flushId);
    w.write(_elements);
    w.write(_size);
  }

  using JfrEvent<EventFlush>::commit; // else commit() is hidden by overloaded versions in this class

  EventFlush(
    u8 flushId,
    u8 elements,
    u8 size) : JfrEvent<EventFlush>(TIMED) {
    if (should_commit()) {
      set_flushId(flushId);
      set_elements(elements);
      set_size(size);
    }
  }

  void commit(u8 flushId,
              u8 elements,
              u8 size) {
    if (should_commit()) {
      set_flushId(flushId);
      set_elements(elements);
      set_size(size);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 flushId,
                     u8 elements,
                     u8 size) {
    EventFlush me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_flushId(flushId);
      me.set_elements(elements);
      me.set_size(size);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_flushId");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_elements");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_size");
  }
#endif
};

class EventHeapDump : public JfrEvent<EventHeapDump>
{
 private:
  const char* _destination;
  s8 _size;
  bool _gcBeforeDump;
  bool _onOutOfMemoryError;
  bool _overwrite;
  s4 _compression;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrHeapDumpEvent;

  EventHeapDump(EventStartTime timing=TIMED) : JfrEvent<EventHeapDump>(timing) {}

  void set_destination(const char* new_value) {
    this->_destination = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_size(s8 new_value) {
    this->_size = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_gcBeforeDump(bool new_value) {
    this->_gcBeforeDump = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_onOutOfMemoryError(bool new_value) {
    this->_onOutOfMemoryError = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_overwrite(bool new_value) {
    this->_overwrite = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_compression(s4 new_value) {
    this->_compression = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_destination);
    w.write(_size);
    w.write(_gcBeforeDump);
    w.write(_onOutOfMemoryError);
    w.write(_overwrite);
    w.write(_compression);
  }

  using JfrEvent<EventHeapDump>::commit; // else commit() is hidden by overloaded versions in this class

  EventHeapDump(
    const char* destination,
    s8 size,
    bool gcBeforeDump,
    bool onOutOfMemoryError,
    bool overwrite,
    s4 compression) : JfrEvent<EventHeapDump>(TIMED) {
    if (should_commit()) {
      set_destination(destination);
      set_size(size);
      set_gcBeforeDump(gcBeforeDump);
      set_onOutOfMemoryError(onOutOfMemoryError);
      set_overwrite(overwrite);
      set_compression(compression);
    }
  }

  void commit(const char* destination,
              s8 size,
              bool gcBeforeDump,
              bool onOutOfMemoryError,
              bool overwrite,
              s4 compression) {
    if (should_commit()) {
      set_destination(destination);
      set_size(size);
      set_gcBeforeDump(gcBeforeDump);
      set_onOutOfMemoryError(onOutOfMemoryError);
      set_overwrite(overwrite);
      set_compression(compression);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* destination,
                     s8 size,
                     bool gcBeforeDump,
                     bool onOutOfMemoryError,
                     bool overwrite,
                     s4 compression) {
    EventHeapDump me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_destination(destination);
      me.set_size(size);
      me.set_gcBeforeDump(gcBeforeDump);
      me.set_onOutOfMemoryError(onOutOfMemoryError);
      me.set_overwrite(overwrite);
      me.set_compression(compression);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_destination");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_size");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_gcBeforeDump");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_onOutOfMemoryError");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_overwrite");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_compression");
  }
#endif
};

class EventFinalizerStatistics : public JfrEvent<EventFinalizerStatistics>
{
 private:
  const Klass* _finalizableClass;
  u8 _codeSource;
  u8 _objects;
  u8 _totalFinalizersRun;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrFinalizerStatisticsEvent;

  EventFinalizerStatistics(EventStartTime timing=TIMED) : JfrEvent<EventFinalizerStatistics>(timing) {}

  void set_finalizableClass(const Klass* new_value) {
    this->_finalizableClass = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_codeSource(u8 new_value) {
    this->_codeSource = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_objects(u8 new_value) {
    this->_objects = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_totalFinalizersRun(u8 new_value) {
    this->_totalFinalizersRun = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_finalizableClass);
    w.write(_codeSource);
    w.write(_objects);
    w.write(_totalFinalizersRun);
  }

  using JfrEvent<EventFinalizerStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* finalizableClass,
                     u8 codeSource,
                     u8 objects,
                     u8 totalFinalizersRun) {
    EventFinalizerStatistics me(UNTIMED);

    if (me.should_commit()) {
      me.set_finalizableClass(finalizableClass);
      me.set_codeSource(codeSource);
      me.set_objects(objects);
      me.set_totalFinalizersRun(totalFinalizersRun);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_finalizableClass");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_codeSource");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_objects");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_totalFinalizersRun");
  }
#endif
};

class EventJavaAgent : public JfrEvent<EventJavaAgent>
{
 private:
  const char* _name;
  const char* _options;
  bool _dynamic;
  Ticks _initializationTime;
  Tickspan _initializationDuration;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrJavaAgentEvent;

  EventJavaAgent(EventStartTime timing=TIMED) : JfrEvent<EventJavaAgent>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_options(const char* new_value) {
    this->_options = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_dynamic(bool new_value) {
    this->_dynamic = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_initializationTime(const Ticks& new_value) {
    this->_initializationTime = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_initializationDuration(const Tickspan& new_value) {
    this->_initializationDuration = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_options);
    w.write(_dynamic);
    w.write(_initializationTime);
    w.write(_initializationDuration);
  }

  using JfrEvent<EventJavaAgent>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     const char* options,
                     bool dynamic,
                     const Ticks& initializationTime,
                     const Tickspan& initializationDuration) {
    EventJavaAgent me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_options(options);
      me.set_dynamic(dynamic);
      me.set_initializationTime(initializationTime);
      me.set_initializationDuration(initializationDuration);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_options");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_dynamic");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_initializationTime");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_initializationDuration");
  }
#endif
};

class EventNativeAgent : public JfrEvent<EventNativeAgent>
{
 private:
  const char* _name;
  const char* _options;
  bool _dynamic;
  Ticks _initializationTime;
  Tickspan _initializationDuration;
  const char* _path;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrNativeAgentEvent;

  EventNativeAgent(EventStartTime timing=TIMED) : JfrEvent<EventNativeAgent>(timing) {}

  void set_name(const char* new_value) {
    this->_name = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_options(const char* new_value) {
    this->_options = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_dynamic(bool new_value) {
    this->_dynamic = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_initializationTime(const Ticks& new_value) {
    this->_initializationTime = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_initializationDuration(const Tickspan& new_value) {
    this->_initializationDuration = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }
  void set_path(const char* new_value) {
    this->_path = new_value;
    DEBUG_ONLY(set_field_bit(5));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_name);
    w.write(_options);
    w.write(_dynamic);
    w.write(_initializationTime);
    w.write(_initializationDuration);
    w.write(_path);
  }

  using JfrEvent<EventNativeAgent>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     const char* options,
                     bool dynamic,
                     const Ticks& initializationTime,
                     const Tickspan& initializationDuration,
                     const char* path) {
    EventNativeAgent me(UNTIMED);

    if (me.should_commit()) {
      me.set_name(name);
      me.set_options(options);
      me.set_dynamic(dynamic);
      me.set_initializationTime(initializationTime);
      me.set_initializationDuration(initializationDuration);
      me.set_path(path);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_name");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_options");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_dynamic");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_initializationTime");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_initializationDuration");
    assert(verify_field_bit(5), "Attempting to write an uninitialized event field: %s", "_path");
  }
#endif
};

class EventDeprecatedInvocation : public JfrEvent<EventDeprecatedInvocation>
{
 private:
  const Method* _method;
  Ticks _invocationTime;
  bool _forRemoval;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = true;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrDeprecatedInvocationEvent;

  EventDeprecatedInvocation(EventStartTime timing=TIMED) : JfrEvent<EventDeprecatedInvocation>(timing) {}

  void set_method(const Method* new_value) {
    this->_method = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_invocationTime(const Ticks& new_value) {
    this->_invocationTime = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_forRemoval(bool new_value) {
    this->_forRemoval = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_method);
    w.write(_invocationTime);
    w.write(_forRemoval);
  }

  using JfrEvent<EventDeprecatedInvocation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Method* method,
                     const Ticks& invocationTime,
                     bool forRemoval) {
    EventDeprecatedInvocation me(UNTIMED);

    if (me.should_commit()) {
      me.set_method(method);
      me.set_invocationTime(invocationTime);
      me.set_forRemoval(forRemoval);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_method");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_invocationTime");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_forRemoval");
  }
#endif
};

class EventMethodTrace : public JfrEvent<EventMethodTrace>
{
 private:
  const Method* _method;

 public:
  static const bool hasThread = true;
  static const bool hasStackTrace = true;
  static const bool isInstant = false;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = false;
  static const JfrEventId eventId = JfrMethodTraceEvent;

  EventMethodTrace(EventStartTime timing=TIMED) : JfrEvent<EventMethodTrace>(timing) {}

  void set_method(const Method* new_value) {
    this->_method = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_method);
  }

  using JfrEvent<EventMethodTrace>::commit; // else commit() is hidden by overloaded versions in this class

  EventMethodTrace(
    const Method* method) : JfrEvent<EventMethodTrace>(TIMED) {
    if (should_commit()) {
      set_method(method);
    }
  }

  void commit(const Method* method) {
    if (should_commit()) {
      set_method(method);
      commit();
    }
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Method* method) {
    EventMethodTrace me(UNTIMED);

    if (me.should_commit()) {
      me.set_starttime(startTicks);
      me.set_endtime(endTicks);
      me.set_method(method);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_method");
  }
#endif
};

class EventMethodTiming : public JfrEvent<EventMethodTiming>
{
 private:
  const Method* _method;
  s8 _invocations;
  Tickspan _minimum;
  Tickspan _average;
  Tickspan _maximum;

 public:
  static const bool hasThread = false;
  static const bool hasStackTrace = false;
  static const bool isInstant = true;
  static const bool hasCutoff = false;
  static const bool hasThrottle = false;
  static const bool isRequestable = true;
  static const JfrEventId eventId = JfrMethodTimingEvent;

  EventMethodTiming(EventStartTime timing=TIMED) : JfrEvent<EventMethodTiming>(timing) {}

  void set_method(const Method* new_value) {
    this->_method = new_value;
    DEBUG_ONLY(set_field_bit(0));
  }
  void set_invocations(s8 new_value) {
    this->_invocations = new_value;
    DEBUG_ONLY(set_field_bit(1));
  }
  void set_minimum(const Tickspan& new_value) {
    this->_minimum = new_value;
    DEBUG_ONLY(set_field_bit(2));
  }
  void set_average(const Tickspan& new_value) {
    this->_average = new_value;
    DEBUG_ONLY(set_field_bit(3));
  }
  void set_maximum(const Tickspan& new_value) {
    this->_maximum = new_value;
    DEBUG_ONLY(set_field_bit(4));
  }

  template <typename Writer>
  void writeData(Writer& w) {
    w.write(_method);
    w.write(_invocations);
    w.write(_minimum);
    w.write(_average);
    w.write(_maximum);
  }

  using JfrEvent<EventMethodTiming>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Method* method,
                     s8 invocations,
                     const Tickspan& minimum,
                     const Tickspan& average,
                     const Tickspan& maximum) {
    EventMethodTiming me(UNTIMED);

    if (me.should_commit()) {
      me.set_method(method);
      me.set_invocations(invocations);
      me.set_minimum(minimum);
      me.set_average(average);
      me.set_maximum(maximum);
      me.commit();
    }
  }

#ifdef ASSERT
  void verify() const {
    assert(verify_field_bit(0), "Attempting to write an uninitialized event field: %s", "_method");
    assert(verify_field_bit(1), "Attempting to write an uninitialized event field: %s", "_invocations");
    assert(verify_field_bit(2), "Attempting to write an uninitialized event field: %s", "_minimum");
    assert(verify_field_bit(3), "Attempting to write an uninitialized event field: %s", "_average");
    assert(verify_field_bit(4), "Attempting to write an uninitialized event field: %s", "_maximum");
  }
#endif
};

template <typename EventType>
class JfrNonReentrant : public EventType {
 private:
  Thread* const _thread;
  int32_t _previous_nesting;
 public:
  JfrNonReentrant(EventStartTime timing = TIMED)
    : EventType(timing), _thread(Thread::current()), _previous_nesting(JfrThreadLocal::make_non_reentrant(_thread)) {}

  JfrNonReentrant(Thread* thread, EventStartTime timing = TIMED)
    : EventType(timing), _thread(thread), _previous_nesting(JfrThreadLocal::make_non_reentrant(_thread)) {}

  ~JfrNonReentrant() {
    if (_previous_nesting != -1) {
      JfrThreadLocal::make_reentrant(_thread, _previous_nesting);
    }
  }
}; 


#else // !INCLUDE_JFR

template <typename T>
class JfrEvent {
 public:
  JfrEvent() {}
  void set_starttime(const Ticks&) const {}
  void set_endtime(const Ticks&) const {}
  bool should_commit() const { return false; }
  bool is_started() const { return false; }
  static bool is_enabled() { return false; }
  void commit() {}
};

struct JfrStructVirtualSpace
{
 public:
  void set_start(u8 new_value) { }
  void set_committedEnd(u8 new_value) { }
  void set_committedSize(u8 new_value) { }
  void set_reservedEnd(u8 new_value) { }
  void set_reservedSize(u8 new_value) { }

};


struct JfrStructObjectSpace
{
 public:
  void set_start(u8 new_value) { }
  void set_end(u8 new_value) { }
  void set_used(u8 new_value) { }
  void set_size(u8 new_value) { }

};


struct JfrStructMetaspaceSizes
{
 public:
  void set_committed(u8 new_value) { }
  void set_used(u8 new_value) { }
  void set_reserved(u8 new_value) { }

};


struct JfrStructCopyFailed
{
 public:
  void set_objectCount(u8 new_value) { }
  void set_firstSize(u8 new_value) { }
  void set_smallestSize(u8 new_value) { }
  void set_totalSize(u8 new_value) { }

};


struct JfrStructG1EvacuationStatistics
{
 public:
  void set_gcId(unsigned new_value) { }
  void set_allocated(u8 new_value) { }
  void set_wasted(u8 new_value) { }
  void set_used(u8 new_value) { }
  void set_undoWaste(u8 new_value) { }
  void set_regionEndWaste(u8 new_value) { }
  void set_regionsRefilled(unsigned new_value) { }
  void set_numPlabsFilled(u8 new_value) { }
  void set_directAllocated(u8 new_value) { }
  void set_numDirectAllocated(u8 new_value) { }
  void set_failureUsed(u8 new_value) { }
  void set_failureWaste(u8 new_value) { }

};


struct JfrStructCalleeMethod
{
 public:
  void set_type(const char* new_value) { }
  void set_name(const char* new_value) { }
  void set_descriptor(const char* new_value) { }

};


struct JfrStructStackFrame
{
 public:
  void set_method(const Method* new_value) { }
  void set_lineNumber(s4 new_value) { }
  void set_bytecodeIndex(s4 new_value) { }
  void set_type(u8 new_value) { }

};


class EventDuration : public JfrEvent<EventDuration>
{
 public:
  EventDuration(EventStartTime timing=TIMED) {}


  using JfrEvent<EventDuration>::commit; // else commit() is hidden by overloaded versions in this class

  EventDuration(
    ) {
  }

  void commit() {
  }
};

class EventInstant : public JfrEvent<EventInstant>
{
 public:
  EventInstant(EventStartTime timing=TIMED) {}


  using JfrEvent<EventInstant>::commit; // else commit() is hidden by overloaded versions in this class


};

class EventValue : public JfrEvent<EventValue>
{
 public:
  EventValue(EventStartTime timing=TIMED) {}

  void set_value(u8 new_value) {
  }

  using JfrEvent<EventValue>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 value) {
  }
};

class EventText : public JfrEvent<EventText>
{
 public:
  EventText(EventStartTime timing=TIMED) {}

  void set_text(const char* new_value) {
  }

  using JfrEvent<EventText>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* text) {
  }
};

class EventZThreadDebug : public JfrEvent<EventZThreadDebug>
{
 public:
  EventZThreadDebug(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventZThreadDebug>::commit; // else commit() is hidden by overloaded versions in this class

  EventZThreadDebug(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventResidentSetSize : public JfrEvent<EventResidentSetSize>
{
 public:
  EventResidentSetSize(EventStartTime timing=TIMED) {}

  void set_size(u8 new_value) {
  }
  void set_peak(u8 new_value) {
  }

  using JfrEvent<EventResidentSetSize>::commit; // else commit() is hidden by overloaded versions in this class

  EventResidentSetSize(
    u8 size,
    u8 peak) {
  }

  void commit(u8 size,
              u8 peak) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 size,
                     u8 peak) {
  }
};

class EventThreadStart : public JfrEvent<EventThreadStart>
{
 public:
  EventThreadStart(EventStartTime timing=TIMED) {}

  void set_thread(u8 new_value) {
  }
  void set_parentThread(u8 new_value) {
  }

  using JfrEvent<EventThreadStart>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 thread,
                     u8 parentThread) {
  }
};

class EventThreadEnd : public JfrEvent<EventThreadEnd>
{
 public:
  EventThreadEnd(EventStartTime timing=TIMED) {}

  void set_thread(u8 new_value) {
  }

  using JfrEvent<EventThreadEnd>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 thread) {
  }
};

class EventThreadPark : public JfrEvent<EventThreadPark>
{
 public:
  EventThreadPark(EventStartTime timing=TIMED) {}

  void set_parkedClass(const Klass* new_value) {
  }
  void set_timeout(s8 new_value) {
  }
  void set_until(s8 new_value) {
  }
  void set_address(u8 new_value) {
  }

  using JfrEvent<EventThreadPark>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadPark(
    const Klass* parkedClass,
    s8 timeout,
    s8 until,
    u8 address) {
  }

  void commit(const Klass* parkedClass,
              s8 timeout,
              s8 until,
              u8 address) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* parkedClass,
                     s8 timeout,
                     s8 until,
                     u8 address) {
  }
};

class EventJavaMonitorEnter : public JfrEvent<EventJavaMonitorEnter>
{
 public:
  EventJavaMonitorEnter(EventStartTime timing=TIMED) {}

  void set_monitorClass(const Klass* new_value) {
  }
  void set_previousOwner(u8 new_value) {
  }
  void set_address(u8 new_value) {
  }

  using JfrEvent<EventJavaMonitorEnter>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorEnter(
    const Klass* monitorClass,
    u8 previousOwner,
    u8 address) {
  }

  void commit(const Klass* monitorClass,
              u8 previousOwner,
              u8 address) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 previousOwner,
                     u8 address) {
  }
};

class EventJavaMonitorWait : public JfrEvent<EventJavaMonitorWait>
{
 public:
  EventJavaMonitorWait(EventStartTime timing=TIMED) {}

  void set_monitorClass(const Klass* new_value) {
  }
  void set_notifier(u8 new_value) {
  }
  void set_timeout(s8 new_value) {
  }
  void set_timedOut(bool new_value) {
  }
  void set_address(u8 new_value) {
  }

  using JfrEvent<EventJavaMonitorWait>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorWait(
    const Klass* monitorClass,
    u8 notifier,
    s8 timeout,
    bool timedOut,
    u8 address) {
  }

  void commit(const Klass* monitorClass,
              u8 notifier,
              s8 timeout,
              bool timedOut,
              u8 address) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 notifier,
                     s8 timeout,
                     bool timedOut,
                     u8 address) {
  }
};

class EventJavaMonitorNotify : public JfrEvent<EventJavaMonitorNotify>
{
 public:
  EventJavaMonitorNotify(EventStartTime timing=TIMED) {}

  void set_monitorClass(const Klass* new_value) {
  }
  void set_notifiedCount(unsigned new_value) {
  }
  void set_address(u8 new_value) {
  }

  using JfrEvent<EventJavaMonitorNotify>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorNotify(
    const Klass* monitorClass,
    unsigned notifiedCount,
    u8 address) {
  }

  void commit(const Klass* monitorClass,
              unsigned notifiedCount,
              u8 address) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     unsigned notifiedCount,
                     u8 address) {
  }
};

class EventJavaMonitorInflate : public JfrEvent<EventJavaMonitorInflate>
{
 public:
  EventJavaMonitorInflate(EventStartTime timing=TIMED) {}

  void set_monitorClass(const Klass* new_value) {
  }
  void set_address(u8 new_value) {
  }
  void set_cause(u8 new_value) {
  }

  using JfrEvent<EventJavaMonitorInflate>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorInflate(
    const Klass* monitorClass,
    u8 address,
    u8 cause) {
  }

  void commit(const Klass* monitorClass,
              u8 address,
              u8 cause) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 address,
                     u8 cause) {
  }
};

class EventJavaMonitorDeflate : public JfrEvent<EventJavaMonitorDeflate>
{
 public:
  EventJavaMonitorDeflate(EventStartTime timing=TIMED) {}

  void set_monitorClass(const Klass* new_value) {
  }
  void set_address(u8 new_value) {
  }

  using JfrEvent<EventJavaMonitorDeflate>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorDeflate(
    const Klass* monitorClass,
    u8 address) {
  }

  void commit(const Klass* monitorClass,
              u8 address) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* monitorClass,
                     u8 address) {
  }
};

class EventJavaMonitorStatistics : public JfrEvent<EventJavaMonitorStatistics>
{
 public:
  EventJavaMonitorStatistics(EventStartTime timing=TIMED) {}

  void set_count(u8 new_value) {
  }

  using JfrEvent<EventJavaMonitorStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaMonitorStatistics(
    u8 count) {
  }

  void commit(u8 count) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 count) {
  }
};

class EventSyncOnValueBasedClass : public JfrEvent<EventSyncOnValueBasedClass>
{
 public:
  EventSyncOnValueBasedClass(EventStartTime timing=TIMED) {}

  void set_valueBasedClass(const Klass* new_value) {
  }

  using JfrEvent<EventSyncOnValueBasedClass>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* valueBasedClass) {
  }
};

class EventContinuationFreeze : public JfrEvent<EventContinuationFreeze>
{
 public:
  EventContinuationFreeze(EventStartTime timing=TIMED) {}

  void set_carrierThread(u8 new_value) {
  }
  void set_continuationClass(const Klass* new_value) {
  }
  void set_interpretedFrames(u2 new_value) {
  }
  void set_size(u2 new_value) {
  }

  using JfrEvent<EventContinuationFreeze>::commit; // else commit() is hidden by overloaded versions in this class

  EventContinuationFreeze(
    u8 carrierThread,
    const Klass* continuationClass,
    u2 interpretedFrames,
    u2 size) {
  }

  void commit(u8 carrierThread,
              const Klass* continuationClass,
              u2 interpretedFrames,
              u2 size) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 carrierThread,
                     const Klass* continuationClass,
                     u2 interpretedFrames,
                     u2 size) {
  }
};

class EventContinuationThaw : public JfrEvent<EventContinuationThaw>
{
 public:
  EventContinuationThaw(EventStartTime timing=TIMED) {}

  void set_carrierThread(u8 new_value) {
  }
  void set_continuationClass(const Klass* new_value) {
  }
  void set_interpretedFrames(u2 new_value) {
  }
  void set_size(u2 new_value) {
  }

  using JfrEvent<EventContinuationThaw>::commit; // else commit() is hidden by overloaded versions in this class

  EventContinuationThaw(
    u8 carrierThread,
    const Klass* continuationClass,
    u2 interpretedFrames,
    u2 size) {
  }

  void commit(u8 carrierThread,
              const Klass* continuationClass,
              u2 interpretedFrames,
              u2 size) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 carrierThread,
                     const Klass* continuationClass,
                     u2 interpretedFrames,
                     u2 size) {
  }
};

class EventContinuationFreezeFast : public JfrEvent<EventContinuationFreezeFast>
{
 public:
  EventContinuationFreezeFast(EventStartTime timing=TIMED) {}

  void set_id(u8 new_value) {
  }
  void set_size(unsigned new_value) {
  }
  void set_allocate(bool new_value) {
  }

  using JfrEvent<EventContinuationFreezeFast>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id,
                     unsigned size,
                     bool allocate) {
  }
};

class EventContinuationFreezeSlow : public JfrEvent<EventContinuationFreezeSlow>
{
 public:
  EventContinuationFreezeSlow(EventStartTime timing=TIMED) {}

  void set_id(u8 new_value) {
  }

  using JfrEvent<EventContinuationFreezeSlow>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id) {
  }
};

class EventContinuationThawFast : public JfrEvent<EventContinuationThawFast>
{
 public:
  EventContinuationThawFast(EventStartTime timing=TIMED) {}

  void set_id(u8 new_value) {
  }
  void set_full(bool new_value) {
  }
  void set_size(unsigned new_value) {
  }

  using JfrEvent<EventContinuationThawFast>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id,
                     bool full,
                     unsigned size) {
  }
};

class EventContinuationThawSlow : public JfrEvent<EventContinuationThawSlow>
{
 public:
  EventContinuationThawSlow(EventStartTime timing=TIMED) {}

  void set_id(u8 new_value) {
  }

  using JfrEvent<EventContinuationThawSlow>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 id) {
  }
};

class EventVirtualThreadPinned : public JfrEvent<EventVirtualThreadPinned>
{
 public:
  EventVirtualThreadPinned(EventStartTime timing=TIMED) {}

  void set_blockingOperation(const char* new_value) {
  }
  void set_pinnedReason(const char* new_value) {
  }
  void set_carrierThread(u8 new_value) {
  }

  using JfrEvent<EventVirtualThreadPinned>::commit; // else commit() is hidden by overloaded versions in this class

  EventVirtualThreadPinned(
    const char* blockingOperation,
    const char* pinnedReason,
    u8 carrierThread) {
  }

  void commit(const char* blockingOperation,
              const char* pinnedReason,
              u8 carrierThread) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* blockingOperation,
                     const char* pinnedReason,
                     u8 carrierThread) {
  }
};

class EventReservedStackActivation : public JfrEvent<EventReservedStackActivation>
{
 public:
  EventReservedStackActivation(EventStartTime timing=TIMED) {}

  void set_method(const Method* new_value) {
  }

  using JfrEvent<EventReservedStackActivation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Method* method) {
  }
};

class EventClassLoad : public JfrEvent<EventClassLoad>
{
 public:
  EventClassLoad(EventStartTime timing=TIMED) {}

  void set_loadedClass(const Klass* new_value) {
  }
  void set_definingClassLoader(const ClassLoaderData* new_value) {
  }
  void set_initiatingClassLoader(const ClassLoaderData* new_value) {
  }

  using JfrEvent<EventClassLoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventClassLoad(
    const Klass* loadedClass,
    const ClassLoaderData* definingClassLoader,
    const ClassLoaderData* initiatingClassLoader) {
  }

  void commit(const Klass* loadedClass,
              const ClassLoaderData* definingClassLoader,
              const ClassLoaderData* initiatingClassLoader) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Klass* loadedClass,
                     const ClassLoaderData* definingClassLoader,
                     const ClassLoaderData* initiatingClassLoader) {
  }
};

class EventClassDefine : public JfrEvent<EventClassDefine>
{
 public:
  EventClassDefine(EventStartTime timing=TIMED) {}

  void set_definedClass(const Klass* new_value) {
  }
  void set_definingClassLoader(const ClassLoaderData* new_value) {
  }

  using JfrEvent<EventClassDefine>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* definedClass,
                     const ClassLoaderData* definingClassLoader) {
  }
};

class EventClassRedefinition : public JfrEvent<EventClassRedefinition>
{
 public:
  EventClassRedefinition(EventStartTime timing=TIMED) {}

  void set_redefinedClass(const Klass* new_value) {
  }
  void set_classModificationCount(s4 new_value) {
  }
  void set_redefinitionId(u8 new_value) {
  }

  using JfrEvent<EventClassRedefinition>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* redefinedClass,
                     s4 classModificationCount,
                     u8 redefinitionId) {
  }
};

class EventRedefineClasses : public JfrEvent<EventRedefineClasses>
{
 public:
  EventRedefineClasses(EventStartTime timing=TIMED) {}

  void set_classCount(s4 new_value) {
  }
  void set_redefinitionId(u8 new_value) {
  }

  using JfrEvent<EventRedefineClasses>::commit; // else commit() is hidden by overloaded versions in this class

  EventRedefineClasses(
    s4 classCount,
    u8 redefinitionId) {
  }

  void commit(s4 classCount,
              u8 redefinitionId) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s4 classCount,
                     u8 redefinitionId) {
  }
};

class EventRetransformClasses : public JfrEvent<EventRetransformClasses>
{
 public:
  EventRetransformClasses(EventStartTime timing=TIMED) {}

  void set_classCount(s4 new_value) {
  }
  void set_redefinitionId(u8 new_value) {
  }

  using JfrEvent<EventRetransformClasses>::commit; // else commit() is hidden by overloaded versions in this class

  EventRetransformClasses(
    s4 classCount,
    u8 redefinitionId) {
  }

  void commit(s4 classCount,
              u8 redefinitionId) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s4 classCount,
                     u8 redefinitionId) {
  }
};

class EventClassUnload : public JfrEvent<EventClassUnload>
{
 public:
  EventClassUnload(EventStartTime timing=TIMED) {}

  void set_unloadedClass(const Klass* new_value) {
  }
  void set_definingClassLoader(const ClassLoaderData* new_value) {
  }

  using JfrEvent<EventClassUnload>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* unloadedClass,
                     const ClassLoaderData* definingClassLoader) {
  }
};

class EventIntFlagChanged : public JfrEvent<EventIntFlagChanged>
{
 public:
  EventIntFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(s4 new_value) {
  }
  void set_newValue(s4 new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventIntFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     s4 oldValue,
                     s4 newValue,
                     u8 origin) {
  }
};

class EventUnsignedIntFlagChanged : public JfrEvent<EventUnsignedIntFlagChanged>
{
 public:
  EventUnsignedIntFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(unsigned new_value) {
  }
  void set_newValue(unsigned new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventUnsignedIntFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     unsigned oldValue,
                     unsigned newValue,
                     u8 origin) {
  }
};

class EventLongFlagChanged : public JfrEvent<EventLongFlagChanged>
{
 public:
  EventLongFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(s8 new_value) {
  }
  void set_newValue(s8 new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventLongFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     s8 oldValue,
                     s8 newValue,
                     u8 origin) {
  }
};

class EventUnsignedLongFlagChanged : public JfrEvent<EventUnsignedLongFlagChanged>
{
 public:
  EventUnsignedLongFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(u8 new_value) {
  }
  void set_newValue(u8 new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventUnsignedLongFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     u8 oldValue,
                     u8 newValue,
                     u8 origin) {
  }
};

class EventDoubleFlagChanged : public JfrEvent<EventDoubleFlagChanged>
{
 public:
  EventDoubleFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(double new_value) {
  }
  void set_newValue(double new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventDoubleFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     double oldValue,
                     double newValue,
                     u8 origin) {
  }
};

class EventBooleanFlagChanged : public JfrEvent<EventBooleanFlagChanged>
{
 public:
  EventBooleanFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(bool new_value) {
  }
  void set_newValue(bool new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventBooleanFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     bool oldValue,
                     bool newValue,
                     u8 origin) {
  }
};

class EventStringFlagChanged : public JfrEvent<EventStringFlagChanged>
{
 public:
  EventStringFlagChanged(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_oldValue(const char* new_value) {
  }
  void set_newValue(const char* new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventStringFlagChanged>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     const char* oldValue,
                     const char* newValue,
                     u8 origin) {
  }
};

class EventGCHeapSummary : public JfrEvent<EventGCHeapSummary>
{
 public:
  EventGCHeapSummary(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_when(u8 new_value) {
  }
  void set_heapSpace(const JfrStructVirtualSpace& value) {
  }
  void set_heapUsed(u8 new_value) {
  }

  using JfrEvent<EventGCHeapSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     const JfrStructVirtualSpace& heapSpace,
                     u8 heapUsed) {
  }
};

class EventGCHeapMemoryUsage : public JfrEvent<EventGCHeapMemoryUsage>
{
 public:
  EventGCHeapMemoryUsage(EventStartTime timing=TIMED) {}

  void set_used(u8 new_value) {
  }
  void set_committed(u8 new_value) {
  }
  void set_max(u8 new_value) {
  }

  using JfrEvent<EventGCHeapMemoryUsage>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCHeapMemoryUsage(
    u8 used,
    u8 committed,
    u8 max) {
  }

  void commit(u8 used,
              u8 committed,
              u8 max) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 used,
                     u8 committed,
                     u8 max) {
  }
};

class EventGCHeapMemoryPoolUsage : public JfrEvent<EventGCHeapMemoryPoolUsage>
{
 public:
  EventGCHeapMemoryPoolUsage(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_used(u8 new_value) {
  }
  void set_committed(u8 new_value) {
  }
  void set_max(u8 new_value) {
  }

  using JfrEvent<EventGCHeapMemoryPoolUsage>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCHeapMemoryPoolUsage(
    const char* name,
    u8 used,
    u8 committed,
    u8 max) {
  }

  void commit(const char* name,
              u8 used,
              u8 committed,
              u8 max) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     u8 used,
                     u8 committed,
                     u8 max) {
  }
};

class EventMetaspaceSummary : public JfrEvent<EventMetaspaceSummary>
{
 public:
  EventMetaspaceSummary(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_when(u8 new_value) {
  }
  void set_gcThreshold(u8 new_value) {
  }
  void set_metaspace(const JfrStructMetaspaceSizes& value) {
  }
  void set_dataSpace(const JfrStructMetaspaceSizes& value) {
  }
  void set_classSpace(const JfrStructMetaspaceSizes& value) {
  }

  using JfrEvent<EventMetaspaceSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     u8 gcThreshold,
                     const JfrStructMetaspaceSizes& metaspace,
                     const JfrStructMetaspaceSizes& dataSpace,
                     const JfrStructMetaspaceSizes& classSpace) {
  }
};

class EventMetaspaceGCThreshold : public JfrEvent<EventMetaspaceGCThreshold>
{
 public:
  EventMetaspaceGCThreshold(EventStartTime timing=TIMED) {}

  void set_oldValue(u8 new_value) {
  }
  void set_newValue(u8 new_value) {
  }
  void set_updater(u8 new_value) {
  }

  using JfrEvent<EventMetaspaceGCThreshold>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 oldValue,
                     u8 newValue,
                     u8 updater) {
  }
};

class EventMetaspaceAllocationFailure : public JfrEvent<EventMetaspaceAllocationFailure>
{
 public:
  EventMetaspaceAllocationFailure(EventStartTime timing=TIMED) {}

  void set_classLoader(const ClassLoaderData* new_value) {
  }
  void set_hiddenClassLoader(bool new_value) {
  }
  void set_size(u8 new_value) {
  }
  void set_metadataType(u8 new_value) {
  }
  void set_metaspaceObjectType(u8 new_value) {
  }

  using JfrEvent<EventMetaspaceAllocationFailure>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const ClassLoaderData* classLoader,
                     bool hiddenClassLoader,
                     u8 size,
                     u8 metadataType,
                     u8 metaspaceObjectType) {
  }
};

class EventMetaspaceOOM : public JfrEvent<EventMetaspaceOOM>
{
 public:
  EventMetaspaceOOM(EventStartTime timing=TIMED) {}

  void set_classLoader(const ClassLoaderData* new_value) {
  }
  void set_hiddenClassLoader(bool new_value) {
  }
  void set_size(u8 new_value) {
  }
  void set_metadataType(u8 new_value) {
  }
  void set_metaspaceObjectType(u8 new_value) {
  }

  using JfrEvent<EventMetaspaceOOM>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const ClassLoaderData* classLoader,
                     bool hiddenClassLoader,
                     u8 size,
                     u8 metadataType,
                     u8 metaspaceObjectType) {
  }
};

class EventMetaspaceChunkFreeListSummary : public JfrEvent<EventMetaspaceChunkFreeListSummary>
{
 public:
  EventMetaspaceChunkFreeListSummary(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_when(u8 new_value) {
  }
  void set_metadataType(u8 new_value) {
  }
  void set_specializedChunks(u8 new_value) {
  }
  void set_specializedChunksTotalSize(u8 new_value) {
  }
  void set_smallChunks(u8 new_value) {
  }
  void set_smallChunksTotalSize(u8 new_value) {
  }
  void set_mediumChunks(u8 new_value) {
  }
  void set_mediumChunksTotalSize(u8 new_value) {
  }
  void set_humongousChunks(u8 new_value) {
  }
  void set_humongousChunksTotalSize(u8 new_value) {
  }

  using JfrEvent<EventMetaspaceChunkFreeListSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     u8 metadataType,
                     u8 specializedChunks,
                     u8 specializedChunksTotalSize,
                     u8 smallChunks,
                     u8 smallChunksTotalSize,
                     u8 mediumChunks,
                     u8 mediumChunksTotalSize,
                     u8 humongousChunks,
                     u8 humongousChunksTotalSize) {
  }
};

class EventPSHeapSummary : public JfrEvent<EventPSHeapSummary>
{
 public:
  EventPSHeapSummary(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_when(u8 new_value) {
  }
  void set_oldSpace(const JfrStructVirtualSpace& value) {
  }
  void set_oldObjectSpace(const JfrStructObjectSpace& value) {
  }
  void set_youngSpace(const JfrStructVirtualSpace& value) {
  }
  void set_edenSpace(const JfrStructObjectSpace& value) {
  }
  void set_fromSpace(const JfrStructObjectSpace& value) {
  }
  void set_toSpace(const JfrStructObjectSpace& value) {
  }

  using JfrEvent<EventPSHeapSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     const JfrStructVirtualSpace& oldSpace,
                     const JfrStructObjectSpace& oldObjectSpace,
                     const JfrStructVirtualSpace& youngSpace,
                     const JfrStructObjectSpace& edenSpace,
                     const JfrStructObjectSpace& fromSpace,
                     const JfrStructObjectSpace& toSpace) {
  }
};

class EventG1HeapSummary : public JfrEvent<EventG1HeapSummary>
{
 public:
  EventG1HeapSummary(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_when(u8 new_value) {
  }
  void set_edenUsedSize(u8 new_value) {
  }
  void set_edenTotalSize(u8 new_value) {
  }
  void set_survivorUsedSize(u8 new_value) {
  }
  void set_oldGenUsedSize(u8 new_value) {
  }
  void set_numberOfRegions(unsigned new_value) {
  }

  using JfrEvent<EventG1HeapSummary>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 when,
                     u8 edenUsedSize,
                     u8 edenTotalSize,
                     u8 survivorUsedSize,
                     u8 oldGenUsedSize,
                     unsigned numberOfRegions) {
  }
};

class EventGarbageCollection : public JfrEvent<EventGarbageCollection>
{
 public:
  EventGarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(u8 new_value) {
  }
  void set_cause(u8 new_value) {
  }
  void set_sumOfPauses(const Tickspan& new_value) {
  }
  void set_longestPause(const Tickspan& new_value) {
  }

  using JfrEvent<EventGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventGarbageCollection(
    unsigned gcId,
    u8 name,
    u8 cause,
    const Tickspan& sumOfPauses,
    const Tickspan& longestPause) {
  }

  void commit(unsigned gcId,
              u8 name,
              u8 cause,
              const Tickspan& sumOfPauses,
              const Tickspan& longestPause) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     u8 name,
                     u8 cause,
                     const Tickspan& sumOfPauses,
                     const Tickspan& longestPause) {
  }
};

class EventSystemGC : public JfrEvent<EventSystemGC>
{
 public:
  EventSystemGC(EventStartTime timing=TIMED) {}

  void set_invokedConcurrent(bool new_value) {
  }

  using JfrEvent<EventSystemGC>::commit; // else commit() is hidden by overloaded versions in this class

  EventSystemGC(
    bool invokedConcurrent) {
  }

  void commit(bool invokedConcurrent) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     bool invokedConcurrent) {
  }
};

class EventParallelOldGarbageCollection : public JfrEvent<EventParallelOldGarbageCollection>
{
 public:
  EventParallelOldGarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_densePrefix(u8 new_value) {
  }

  using JfrEvent<EventParallelOldGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventParallelOldGarbageCollection(
    unsigned gcId,
    u8 densePrefix) {
  }

  void commit(unsigned gcId,
              u8 densePrefix) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     u8 densePrefix) {
  }
};

class EventYoungGarbageCollection : public JfrEvent<EventYoungGarbageCollection>
{
 public:
  EventYoungGarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_tenuringThreshold(unsigned new_value) {
  }

  using JfrEvent<EventYoungGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventYoungGarbageCollection(
    unsigned gcId,
    unsigned tenuringThreshold) {
  }

  void commit(unsigned gcId,
              unsigned tenuringThreshold) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     unsigned tenuringThreshold) {
  }
};

class EventOldGarbageCollection : public JfrEvent<EventOldGarbageCollection>
{
 public:
  EventOldGarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }

  using JfrEvent<EventOldGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventOldGarbageCollection(
    unsigned gcId) {
  }

  void commit(unsigned gcId) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId) {
  }
};

class EventG1GarbageCollection : public JfrEvent<EventG1GarbageCollection>
{
 public:
  EventG1GarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_type(u8 new_value) {
  }

  using JfrEvent<EventG1GarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventG1GarbageCollection(
    unsigned gcId,
    u8 type) {
  }

  void commit(unsigned gcId,
              u8 type) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     u8 type) {
  }
};

class EventG1MMU : public JfrEvent<EventG1MMU>
{
 public:
  EventG1MMU(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_timeSlice(s8 new_value) {
  }
  void set_gcTime(s8 new_value) {
  }
  void set_pauseTarget(s8 new_value) {
  }

  using JfrEvent<EventG1MMU>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     s8 timeSlice,
                     s8 gcTime,
                     s8 pauseTarget) {
  }
};

class EventEvacuationInformation : public JfrEvent<EventEvacuationInformation>
{
 public:
  EventEvacuationInformation(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_cSetRegions(unsigned new_value) {
  }
  void set_cSetUsedBefore(u8 new_value) {
  }
  void set_cSetUsedAfter(u8 new_value) {
  }
  void set_allocationRegions(unsigned new_value) {
  }
  void set_allocationRegionsUsedBefore(u8 new_value) {
  }
  void set_allocationRegionsUsedAfter(u8 new_value) {
  }
  void set_bytesCopied(u8 new_value) {
  }
  void set_regionsFreed(unsigned new_value) {
  }

  using JfrEvent<EventEvacuationInformation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     unsigned cSetRegions,
                     u8 cSetUsedBefore,
                     u8 cSetUsedAfter,
                     unsigned allocationRegions,
                     u8 allocationRegionsUsedBefore,
                     u8 allocationRegionsUsedAfter,
                     u8 bytesCopied,
                     unsigned regionsFreed) {
  }
};

class EventGCReferenceStatistics : public JfrEvent<EventGCReferenceStatistics>
{
 public:
  EventGCReferenceStatistics(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_type(u8 new_value) {
  }
  void set_count(u8 new_value) {
  }

  using JfrEvent<EventGCReferenceStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 type,
                     u8 count) {
  }
};

class EventObjectCountAfterGC : public JfrEvent<EventObjectCountAfterGC>
{
 public:
  EventObjectCountAfterGC(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_objectClass(const Klass* new_value) {
  }
  void set_count(s8 new_value) {
  }
  void set_totalSize(u8 new_value) {
  }

  using JfrEvent<EventObjectCountAfterGC>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     s8 count,
                     u8 totalSize) {
  }
};

class EventG1EvacuationYoungStatistics : public JfrEvent<EventG1EvacuationYoungStatistics>
{
 public:
  EventG1EvacuationYoungStatistics(EventStartTime timing=TIMED) {}

  void set_statistics(const JfrStructG1EvacuationStatistics& value) {
  }

  using JfrEvent<EventG1EvacuationYoungStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const JfrStructG1EvacuationStatistics& statistics) {
  }
};

class EventG1EvacuationOldStatistics : public JfrEvent<EventG1EvacuationOldStatistics>
{
 public:
  EventG1EvacuationOldStatistics(EventStartTime timing=TIMED) {}

  void set_statistics(const JfrStructG1EvacuationStatistics& value) {
  }

  using JfrEvent<EventG1EvacuationOldStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const JfrStructG1EvacuationStatistics& statistics) {
  }
};

class EventG1BasicIHOP : public JfrEvent<EventG1BasicIHOP>
{
 public:
  EventG1BasicIHOP(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_threshold(u8 new_value) {
  }
  void set_thresholdPercentage(float new_value) {
  }
  void set_targetOccupancy(u8 new_value) {
  }
  void set_currentOccupancy(u8 new_value) {
  }
  void set_recentMutatorAllocationSize(u8 new_value) {
  }
  void set_recentMutatorDuration(s8 new_value) {
  }
  void set_recentAllocationRate(double new_value) {
  }
  void set_lastMarkingDuration(s8 new_value) {
  }

  using JfrEvent<EventG1BasicIHOP>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 threshold,
                     float thresholdPercentage,
                     u8 targetOccupancy,
                     u8 currentOccupancy,
                     u8 recentMutatorAllocationSize,
                     s8 recentMutatorDuration,
                     double recentAllocationRate,
                     s8 lastMarkingDuration) {
  }
};

class EventG1AdaptiveIHOP : public JfrEvent<EventG1AdaptiveIHOP>
{
 public:
  EventG1AdaptiveIHOP(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_threshold(u8 new_value) {
  }
  void set_thresholdPercentage(float new_value) {
  }
  void set_ihopTargetOccupancy(u8 new_value) {
  }
  void set_currentOccupancy(u8 new_value) {
  }
  void set_additionalBufferSize(u8 new_value) {
  }
  void set_predictedAllocationRate(double new_value) {
  }
  void set_predictedMarkingDuration(s8 new_value) {
  }
  void set_predictionActive(bool new_value) {
  }

  using JfrEvent<EventG1AdaptiveIHOP>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 threshold,
                     float thresholdPercentage,
                     u8 ihopTargetOccupancy,
                     u8 currentOccupancy,
                     u8 additionalBufferSize,
                     double predictedAllocationRate,
                     s8 predictedMarkingDuration,
                     bool predictionActive) {
  }
};

class EventPromoteObjectInNewPLAB : public JfrEvent<EventPromoteObjectInNewPLAB>
{
 public:
  EventPromoteObjectInNewPLAB(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_objectClass(const Klass* new_value) {
  }
  void set_objectSize(u8 new_value) {
  }
  void set_tenuringAge(unsigned new_value) {
  }
  void set_tenured(bool new_value) {
  }
  void set_plabSize(u8 new_value) {
  }

  using JfrEvent<EventPromoteObjectInNewPLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     u8 objectSize,
                     unsigned tenuringAge,
                     bool tenured,
                     u8 plabSize) {
  }
};

class EventPromoteObjectOutsidePLAB : public JfrEvent<EventPromoteObjectOutsidePLAB>
{
 public:
  EventPromoteObjectOutsidePLAB(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_objectClass(const Klass* new_value) {
  }
  void set_objectSize(u8 new_value) {
  }
  void set_tenuringAge(unsigned new_value) {
  }
  void set_tenured(bool new_value) {
  }

  using JfrEvent<EventPromoteObjectOutsidePLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     u8 objectSize,
                     unsigned tenuringAge,
                     bool tenured) {
  }
};

class EventPromotionFailed : public JfrEvent<EventPromotionFailed>
{
 public:
  EventPromotionFailed(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_promotionFailed(const JfrStructCopyFailed& value) {
  }
  void set_thread(u8 new_value) {
  }

  using JfrEvent<EventPromotionFailed>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const JfrStructCopyFailed& promotionFailed,
                     u8 thread) {
  }
};

class EventEvacuationFailed : public JfrEvent<EventEvacuationFailed>
{
 public:
  EventEvacuationFailed(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_evacuationFailed(const JfrStructCopyFailed& value) {
  }

  using JfrEvent<EventEvacuationFailed>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const JfrStructCopyFailed& evacuationFailed) {
  }
};

class EventConcurrentModeFailure : public JfrEvent<EventConcurrentModeFailure>
{
 public:
  EventConcurrentModeFailure(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }

  using JfrEvent<EventConcurrentModeFailure>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId) {
  }
};

class EventGCCPUTime : public JfrEvent<EventGCCPUTime>
{
 public:
  EventGCCPUTime(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_userTime(u8 new_value) {
  }
  void set_systemTime(u8 new_value) {
  }
  void set_realTime(u8 new_value) {
  }

  using JfrEvent<EventGCCPUTime>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 userTime,
                     u8 systemTime,
                     u8 realTime) {
  }
};

class EventGCPhasePause : public JfrEvent<EventGCPhasePause>
{
 public:
  EventGCPhasePause(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhasePause>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePause(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhasePauseLevel1 : public JfrEvent<EventGCPhasePauseLevel1>
{
 public:
  EventGCPhasePauseLevel1(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhasePauseLevel1>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel1(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhasePauseLevel2 : public JfrEvent<EventGCPhasePauseLevel2>
{
 public:
  EventGCPhasePauseLevel2(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhasePauseLevel2>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel2(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhasePauseLevel3 : public JfrEvent<EventGCPhasePauseLevel3>
{
 public:
  EventGCPhasePauseLevel3(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhasePauseLevel3>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel3(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhasePauseLevel4 : public JfrEvent<EventGCPhasePauseLevel4>
{
 public:
  EventGCPhasePauseLevel4(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhasePauseLevel4>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhasePauseLevel4(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhaseConcurrent : public JfrEvent<EventGCPhaseConcurrent>
{
 public:
  EventGCPhaseConcurrent(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhaseConcurrent>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseConcurrent(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhaseConcurrentLevel1 : public JfrEvent<EventGCPhaseConcurrentLevel1>
{
 public:
  EventGCPhaseConcurrentLevel1(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhaseConcurrentLevel1>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseConcurrentLevel1(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhaseConcurrentLevel2 : public JfrEvent<EventGCPhaseConcurrentLevel2>
{
 public:
  EventGCPhaseConcurrentLevel2(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhaseConcurrentLevel2>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseConcurrentLevel2(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventGCPhaseParallel : public JfrEvent<EventGCPhaseParallel>
{
 public:
  EventGCPhaseParallel(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_gcWorkerId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventGCPhaseParallel>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCPhaseParallel(
    unsigned gcId,
    unsigned gcWorkerId,
    const char* name) {
  }

  void commit(unsigned gcId,
              unsigned gcWorkerId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     unsigned gcWorkerId,
                     const char* name) {
  }
};

class EventAllocationRequiringGC : public JfrEvent<EventAllocationRequiringGC>
{
 public:
  EventAllocationRequiringGC(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_size(u8 new_value) {
  }

  using JfrEvent<EventAllocationRequiringGC>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 size) {
  }
};

class EventTenuringDistribution : public JfrEvent<EventTenuringDistribution>
{
 public:
  EventTenuringDistribution(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_age(unsigned new_value) {
  }
  void set_size(u8 new_value) {
  }

  using JfrEvent<EventTenuringDistribution>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     unsigned age,
                     u8 size) {
  }
};

class EventG1HeapRegionTypeChange : public JfrEvent<EventG1HeapRegionTypeChange>
{
 public:
  EventG1HeapRegionTypeChange(EventStartTime timing=TIMED) {}

  void set_index(unsigned new_value) {
  }
  void set_from(u8 new_value) {
  }
  void set_to(u8 new_value) {
  }
  void set_start(u8 new_value) {
  }
  void set_used(u8 new_value) {
  }

  using JfrEvent<EventG1HeapRegionTypeChange>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned index,
                     u8 from,
                     u8 to,
                     u8 start,
                     u8 used) {
  }
};

class EventJITRestart : public JfrEvent<EventJITRestart>
{
 public:
  EventJITRestart(EventStartTime timing=TIMED) {}

  void set_freedMemory(u8 new_value) {
  }
  void set_codeCacheMaxCapacity(u8 new_value) {
  }

  using JfrEvent<EventJITRestart>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 freedMemory,
                     u8 codeCacheMaxCapacity) {
  }
};

class EventCompilation : public JfrEvent<EventCompilation>
{
 public:
  EventCompilation(EventStartTime timing=TIMED) {}

  void set_compileId(s4 new_value) {
  }
  void set_compiler(u8 new_value) {
  }
  void set_method(const Method* new_value) {
  }
  void set_compileLevel(u2 new_value) {
  }
  void set_succeded(bool new_value) {
  }
  void set_isOsr(bool new_value) {
  }
  void set_codeSize(u8 new_value) {
  }
  void set_inlinedBytes(u8 new_value) {
  }
  void set_arenaBytes(u8 new_value) {
  }

  using JfrEvent<EventCompilation>::commit; // else commit() is hidden by overloaded versions in this class

  EventCompilation(
    s4 compileId,
    u8 compiler,
    const Method* method,
    u2 compileLevel,
    bool succeded,
    bool isOsr,
    u8 codeSize,
    u8 inlinedBytes,
    u8 arenaBytes) {
  }

  void commit(s4 compileId,
              u8 compiler,
              const Method* method,
              u2 compileLevel,
              bool succeded,
              bool isOsr,
              u8 codeSize,
              u8 inlinedBytes,
              u8 arenaBytes) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s4 compileId,
                     u8 compiler,
                     const Method* method,
                     u2 compileLevel,
                     bool succeded,
                     bool isOsr,
                     u8 codeSize,
                     u8 inlinedBytes,
                     u8 arenaBytes) {
  }
};

class EventCompilerPhase : public JfrEvent<EventCompilerPhase>
{
 public:
  EventCompilerPhase(EventStartTime timing=TIMED) {}

  void set_phase(u8 new_value) {
  }
  void set_compileId(s4 new_value) {
  }
  void set_phaseLevel(u2 new_value) {
  }

  using JfrEvent<EventCompilerPhase>::commit; // else commit() is hidden by overloaded versions in this class

  EventCompilerPhase(
    u8 phase,
    s4 compileId,
    u2 phaseLevel) {
  }

  void commit(u8 phase,
              s4 compileId,
              u2 phaseLevel) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 phase,
                     s4 compileId,
                     u2 phaseLevel) {
  }
};

class EventCompilationFailure : public JfrEvent<EventCompilationFailure>
{
 public:
  EventCompilationFailure(EventStartTime timing=TIMED) {}

  void set_failureMessage(const char* new_value) {
  }
  void set_compileId(s4 new_value) {
  }

  using JfrEvent<EventCompilationFailure>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* failureMessage,
                     s4 compileId) {
  }
};

class EventCompilerInlining : public JfrEvent<EventCompilerInlining>
{
 public:
  EventCompilerInlining(EventStartTime timing=TIMED) {}

  void set_compileId(s4 new_value) {
  }
  void set_caller(const Method* new_value) {
  }
  void set_callee(const JfrStructCalleeMethod& value) {
  }
  void set_succeeded(bool new_value) {
  }
  void set_message(const char* new_value) {
  }
  void set_bci(s4 new_value) {
  }

  using JfrEvent<EventCompilerInlining>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 compileId,
                     const Method* caller,
                     const JfrStructCalleeMethod& callee,
                     bool succeeded,
                     const char* message,
                     s4 bci) {
  }
};

class EventCodeCacheFull : public JfrEvent<EventCodeCacheFull>
{
 public:
  EventCodeCacheFull(EventStartTime timing=TIMED) {}

  void set_codeBlobType(u8 new_value) {
  }
  void set_startAddress(u8 new_value) {
  }
  void set_commitedTopAddress(u8 new_value) {
  }
  void set_reservedTopAddress(u8 new_value) {
  }
  void set_entryCount(s4 new_value) {
  }
  void set_methodCount(s4 new_value) {
  }
  void set_adaptorCount(s4 new_value) {
  }
  void set_unallocatedCapacity(u8 new_value) {
  }
  void set_fullCount(s4 new_value) {
  }
  void set_codeCacheMaxCapacity(u8 new_value) {
  }

  using JfrEvent<EventCodeCacheFull>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 codeBlobType,
                     u8 startAddress,
                     u8 commitedTopAddress,
                     u8 reservedTopAddress,
                     s4 entryCount,
                     s4 methodCount,
                     s4 adaptorCount,
                     u8 unallocatedCapacity,
                     s4 fullCount,
                     u8 codeCacheMaxCapacity) {
  }
};

class EventDeoptimization : public JfrEvent<EventDeoptimization>
{
 public:
  EventDeoptimization(EventStartTime timing=TIMED) {}

  void set_compileId(s4 new_value) {
  }
  void set_compiler(u8 new_value) {
  }
  void set_method(const Method* new_value) {
  }
  void set_lineNumber(s4 new_value) {
  }
  void set_bci(s4 new_value) {
  }
  void set_instruction(u8 new_value) {
  }
  void set_reason(u8 new_value) {
  }
  void set_action(u8 new_value) {
  }

  using JfrEvent<EventDeoptimization>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 compileId,
                     u8 compiler,
                     const Method* method,
                     s4 lineNumber,
                     s4 bci,
                     u8 instruction,
                     u8 reason,
                     u8 action) {
  }
};

class EventSafepointBegin : public JfrEvent<EventSafepointBegin>
{
 public:
  EventSafepointBegin(EventStartTime timing=TIMED) {}

  void set_safepointId(u8 new_value) {
  }
  void set_totalThreadCount(s4 new_value) {
  }
  void set_jniCriticalThreadCount(s4 new_value) {
  }

  using JfrEvent<EventSafepointBegin>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointBegin(
    u8 safepointId,
    s4 totalThreadCount,
    s4 jniCriticalThreadCount) {
  }

  void commit(u8 safepointId,
              s4 totalThreadCount,
              s4 jniCriticalThreadCount) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 safepointId,
                     s4 totalThreadCount,
                     s4 jniCriticalThreadCount) {
  }
};

class EventSafepointStateSynchronization : public JfrEvent<EventSafepointStateSynchronization>
{
 public:
  EventSafepointStateSynchronization(EventStartTime timing=TIMED) {}

  void set_safepointId(u8 new_value) {
  }
  void set_initialThreadCount(s4 new_value) {
  }
  void set_runningThreadCount(s4 new_value) {
  }
  void set_iterations(s4 new_value) {
  }

  using JfrEvent<EventSafepointStateSynchronization>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointStateSynchronization(
    u8 safepointId,
    s4 initialThreadCount,
    s4 runningThreadCount,
    s4 iterations) {
  }

  void commit(u8 safepointId,
              s4 initialThreadCount,
              s4 runningThreadCount,
              s4 iterations) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 safepointId,
                     s4 initialThreadCount,
                     s4 runningThreadCount,
                     s4 iterations) {
  }
};

class EventSafepointEnd : public JfrEvent<EventSafepointEnd>
{
 public:
  EventSafepointEnd(EventStartTime timing=TIMED) {}

  void set_safepointId(u8 new_value) {
  }

  using JfrEvent<EventSafepointEnd>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointEnd(
    u8 safepointId) {
  }

  void commit(u8 safepointId) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 safepointId) {
  }
};

class EventSafepointLatency : public JfrEvent<EventSafepointLatency>
{
 public:
  EventSafepointLatency(EventStartTime timing=TIMED) {}

  void set_threadState(u8 new_value) {
  }

  using JfrEvent<EventSafepointLatency>::commit; // else commit() is hidden by overloaded versions in this class

  EventSafepointLatency(
    u8 threadState) {
  }

  void commit(u8 threadState) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 threadState) {
  }
};

class EventExecuteVMOperation : public JfrEvent<EventExecuteVMOperation>
{
 public:
  EventExecuteVMOperation(EventStartTime timing=TIMED) {}

  void set_operation(u8 new_value) {
  }
  void set_safepoint(bool new_value) {
  }
  void set_blocking(bool new_value) {
  }
  void set_caller(u8 new_value) {
  }
  void set_safepointId(u8 new_value) {
  }

  using JfrEvent<EventExecuteVMOperation>::commit; // else commit() is hidden by overloaded versions in this class

  EventExecuteVMOperation(
    u8 operation,
    bool safepoint,
    bool blocking,
    u8 caller,
    u8 safepointId) {
  }

  void commit(u8 operation,
              bool safepoint,
              bool blocking,
              u8 caller,
              u8 safepointId) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 operation,
                     bool safepoint,
                     bool blocking,
                     u8 caller,
                     u8 safepointId) {
  }
};

class EventShutdown : public JfrEvent<EventShutdown>
{
 public:
  EventShutdown(EventStartTime timing=TIMED) {}

  void set_reason(const char* new_value) {
  }

  using JfrEvent<EventShutdown>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* reason) {
  }
};

class EventObjectAllocationInNewTLAB : public JfrEvent<EventObjectAllocationInNewTLAB>
{
 public:
  EventObjectAllocationInNewTLAB(EventStartTime timing=TIMED) {}

  void set_objectClass(const Klass* new_value) {
  }
  void set_allocationSize(u8 new_value) {
  }
  void set_tlabSize(u8 new_value) {
  }

  using JfrEvent<EventObjectAllocationInNewTLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* objectClass,
                     u8 allocationSize,
                     u8 tlabSize) {
  }
};

class EventObjectAllocationOutsideTLAB : public JfrEvent<EventObjectAllocationOutsideTLAB>
{
 public:
  EventObjectAllocationOutsideTLAB(EventStartTime timing=TIMED) {}

  void set_objectClass(const Klass* new_value) {
  }
  void set_allocationSize(u8 new_value) {
  }

  using JfrEvent<EventObjectAllocationOutsideTLAB>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* objectClass,
                     u8 allocationSize) {
  }
};

class EventObjectAllocationSample : public JfrEvent<EventObjectAllocationSample>
{
 public:
  EventObjectAllocationSample(EventStartTime timing=TIMED) {}

  void set_objectClass(const Klass* new_value) {
  }
  void set_weight(s8 new_value) {
  }

  using JfrEvent<EventObjectAllocationSample>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* objectClass,
                     s8 weight) {
  }
};

class EventOldObjectSample : public JfrEvent<EventOldObjectSample>
{
 public:
  EventOldObjectSample(EventStartTime timing=TIMED) {}

  void set_allocationTime(const Ticks& new_value) {
  }
  void set_objectSize(u8 new_value) {
  }
  void set_objectAge(const Tickspan& new_value) {
  }
  void set_lastKnownHeapUsage(u8 new_value) {
  }
  void set_object(u8 new_value) {
  }
  void set_arrayElements(s4 new_value) {
  }
  void set_root(u8 new_value) {
  }

  using JfrEvent<EventOldObjectSample>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Ticks& allocationTime,
                     u8 objectSize,
                     const Tickspan& objectAge,
                     u8 lastKnownHeapUsage,
                     u8 object,
                     s4 arrayElements,
                     u8 root) {
  }
};

class EventNativeMemoryUsage : public JfrEvent<EventNativeMemoryUsage>
{
 public:
  EventNativeMemoryUsage(EventStartTime timing=TIMED) {}

  void set_type(u8 new_value) {
  }
  void set_reserved(u8 new_value) {
  }
  void set_committed(u8 new_value) {
  }

  using JfrEvent<EventNativeMemoryUsage>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeMemoryUsage(
    u8 type,
    u8 reserved,
    u8 committed) {
  }

  void commit(u8 type,
              u8 reserved,
              u8 committed) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 reserved,
                     u8 committed) {
  }
};

class EventNativeMemoryUsageTotal : public JfrEvent<EventNativeMemoryUsageTotal>
{
 public:
  EventNativeMemoryUsageTotal(EventStartTime timing=TIMED) {}

  void set_reserved(u8 new_value) {
  }
  void set_committed(u8 new_value) {
  }

  using JfrEvent<EventNativeMemoryUsageTotal>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeMemoryUsageTotal(
    u8 reserved,
    u8 committed) {
  }

  void commit(u8 reserved,
              u8 committed) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 reserved,
                     u8 committed) {
  }
};

class EventDumpReason : public JfrEvent<EventDumpReason>
{
 public:
  EventDumpReason(EventStartTime timing=TIMED) {}

  void set_reason(const char* new_value) {
  }
  void set_recordingId(s4 new_value) {
  }

  using JfrEvent<EventDumpReason>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* reason,
                     s4 recordingId) {
  }
};

class EventDataLoss : public JfrEvent<EventDataLoss>
{
 public:
  EventDataLoss(EventStartTime timing=TIMED) {}

  void set_amount(u8 new_value) {
  }
  void set_total(u8 new_value) {
  }

  using JfrEvent<EventDataLoss>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 amount,
                     u8 total) {
  }
};

class EventJVMInformation : public JfrEvent<EventJVMInformation>
{
 public:
  EventJVMInformation(EventStartTime timing=TIMED) {}

  void set_jvmName(const char* new_value) {
  }
  void set_jvmVersion(const char* new_value) {
  }
  void set_jvmArguments(const char* new_value) {
  }
  void set_jvmFlags(const char* new_value) {
  }
  void set_javaArguments(const char* new_value) {
  }
  void set_jvmStartTime(s8 new_value) {
  }
  void set_pid(s8 new_value) {
  }

  using JfrEvent<EventJVMInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventJVMInformation(
    const char* jvmName,
    const char* jvmVersion,
    const char* jvmArguments,
    const char* jvmFlags,
    const char* javaArguments,
    s8 jvmStartTime,
    s8 pid) {
  }

  void commit(const char* jvmName,
              const char* jvmVersion,
              const char* jvmArguments,
              const char* jvmFlags,
              const char* javaArguments,
              s8 jvmStartTime,
              s8 pid) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* jvmName,
                     const char* jvmVersion,
                     const char* jvmArguments,
                     const char* jvmFlags,
                     const char* javaArguments,
                     s8 jvmStartTime,
                     s8 pid) {
  }
};

class EventOSInformation : public JfrEvent<EventOSInformation>
{
 public:
  EventOSInformation(EventStartTime timing=TIMED) {}

  void set_osVersion(const char* new_value) {
  }

  using JfrEvent<EventOSInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventOSInformation(
    const char* osVersion) {
  }

  void commit(const char* osVersion) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* osVersion) {
  }
};

class EventVirtualizationInformation : public JfrEvent<EventVirtualizationInformation>
{
 public:
  EventVirtualizationInformation(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }

  using JfrEvent<EventVirtualizationInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventVirtualizationInformation(
    const char* name) {
  }

  void commit(const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name) {
  }
};

class EventInitialSystemProperty : public JfrEvent<EventInitialSystemProperty>
{
 public:
  EventInitialSystemProperty(EventStartTime timing=TIMED) {}

  void set_key(const char* new_value) {
  }
  void set_value(const char* new_value) {
  }

  using JfrEvent<EventInitialSystemProperty>::commit; // else commit() is hidden by overloaded versions in this class

  EventInitialSystemProperty(
    const char* key,
    const char* value) {
  }

  void commit(const char* key,
              const char* value) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* key,
                     const char* value) {
  }
};

class EventInitialEnvironmentVariable : public JfrEvent<EventInitialEnvironmentVariable>
{
 public:
  EventInitialEnvironmentVariable(EventStartTime timing=TIMED) {}

  void set_key(const char* new_value) {
  }
  void set_value(const char* new_value) {
  }

  using JfrEvent<EventInitialEnvironmentVariable>::commit; // else commit() is hidden by overloaded versions in this class

  EventInitialEnvironmentVariable(
    const char* key,
    const char* value) {
  }

  void commit(const char* key,
              const char* value) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* key,
                     const char* value) {
  }
};

class EventSystemProcess : public JfrEvent<EventSystemProcess>
{
 public:
  EventSystemProcess(EventStartTime timing=TIMED) {}

  void set_pid(const char* new_value) {
  }
  void set_commandLine(const char* new_value) {
  }

  using JfrEvent<EventSystemProcess>::commit; // else commit() is hidden by overloaded versions in this class

  EventSystemProcess(
    const char* pid,
    const char* commandLine) {
  }

  void commit(const char* pid,
              const char* commandLine) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* pid,
                     const char* commandLine) {
  }
};

class EventCPUInformation : public JfrEvent<EventCPUInformation>
{
 public:
  EventCPUInformation(EventStartTime timing=TIMED) {}

  void set_cpu(const char* new_value) {
  }
  void set_description(const char* new_value) {
  }
  void set_sockets(unsigned new_value) {
  }
  void set_cores(unsigned new_value) {
  }
  void set_hwThreads(unsigned new_value) {
  }

  using JfrEvent<EventCPUInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventCPUInformation(
    const char* cpu,
    const char* description,
    unsigned sockets,
    unsigned cores,
    unsigned hwThreads) {
  }

  void commit(const char* cpu,
              const char* description,
              unsigned sockets,
              unsigned cores,
              unsigned hwThreads) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* cpu,
                     const char* description,
                     unsigned sockets,
                     unsigned cores,
                     unsigned hwThreads) {
  }
};

class EventCPUTimeStampCounter : public JfrEvent<EventCPUTimeStampCounter>
{
 public:
  EventCPUTimeStampCounter(EventStartTime timing=TIMED) {}

  void set_fastTimeEnabled(bool new_value) {
  }
  void set_fastTimeAutoEnabled(bool new_value) {
  }
  void set_osFrequency(s8 new_value) {
  }
  void set_fastTimeFrequency(s8 new_value) {
  }

  using JfrEvent<EventCPUTimeStampCounter>::commit; // else commit() is hidden by overloaded versions in this class

  EventCPUTimeStampCounter(
    bool fastTimeEnabled,
    bool fastTimeAutoEnabled,
    s8 osFrequency,
    s8 fastTimeFrequency) {
  }

  void commit(bool fastTimeEnabled,
              bool fastTimeAutoEnabled,
              s8 osFrequency,
              s8 fastTimeFrequency) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     bool fastTimeEnabled,
                     bool fastTimeAutoEnabled,
                     s8 osFrequency,
                     s8 fastTimeFrequency) {
  }
};

class EventCPULoad : public JfrEvent<EventCPULoad>
{
 public:
  EventCPULoad(EventStartTime timing=TIMED) {}

  void set_jvmUser(float new_value) {
  }
  void set_jvmSystem(float new_value) {
  }
  void set_machineTotal(float new_value) {
  }

  using JfrEvent<EventCPULoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventCPULoad(
    float jvmUser,
    float jvmSystem,
    float machineTotal) {
  }

  void commit(float jvmUser,
              float jvmSystem,
              float machineTotal) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     float jvmUser,
                     float jvmSystem,
                     float machineTotal) {
  }
};

class EventThreadCPULoad : public JfrEvent<EventThreadCPULoad>
{
 public:
  EventThreadCPULoad(EventStartTime timing=TIMED) {}

  void set_user(float new_value) {
  }
  void set_system(float new_value) {
  }

  using JfrEvent<EventThreadCPULoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadCPULoad(
    float user,
    float system) {
  }

  void commit(float user,
              float system) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     float user,
                     float system) {
  }
};

class EventThreadContextSwitchRate : public JfrEvent<EventThreadContextSwitchRate>
{
 public:
  EventThreadContextSwitchRate(EventStartTime timing=TIMED) {}

  void set_switchRate(float new_value) {
  }

  using JfrEvent<EventThreadContextSwitchRate>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadContextSwitchRate(
    float switchRate) {
  }

  void commit(float switchRate) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     float switchRate) {
  }
};

class EventCompilerQueueUtilization : public JfrEvent<EventCompilerQueueUtilization>
{
 public:
  EventCompilerQueueUtilization(EventStartTime timing=TIMED) {}

  void set_compiler(u8 new_value) {
  }
  void set_addedRate(s8 new_value) {
  }
  void set_removedRate(s8 new_value) {
  }
  void set_queueSize(s8 new_value) {
  }
  void set_peakQueueSize(s8 new_value) {
  }
  void set_addedCount(s8 new_value) {
  }
  void set_removedCount(s8 new_value) {
  }
  void set_totalAddedCount(s8 new_value) {
  }
  void set_totalRemovedCount(s8 new_value) {
  }
  void set_compilerThreadCount(s4 new_value) {
  }

  using JfrEvent<EventCompilerQueueUtilization>::commit; // else commit() is hidden by overloaded versions in this class

  EventCompilerQueueUtilization(
    u8 compiler,
    s8 addedRate,
    s8 removedRate,
    s8 queueSize,
    s8 peakQueueSize,
    s8 addedCount,
    s8 removedCount,
    s8 totalAddedCount,
    s8 totalRemovedCount,
    s4 compilerThreadCount) {
  }

  void commit(u8 compiler,
              s8 addedRate,
              s8 removedRate,
              s8 queueSize,
              s8 peakQueueSize,
              s8 addedCount,
              s8 removedCount,
              s8 totalAddedCount,
              s8 totalRemovedCount,
              s4 compilerThreadCount) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 compiler,
                     s8 addedRate,
                     s8 removedRate,
                     s8 queueSize,
                     s8 peakQueueSize,
                     s8 addedCount,
                     s8 removedCount,
                     s8 totalAddedCount,
                     s8 totalRemovedCount,
                     s4 compilerThreadCount) {
  }
};

class EventNetworkUtilization : public JfrEvent<EventNetworkUtilization>
{
 public:
  EventNetworkUtilization(EventStartTime timing=TIMED) {}

  void set_networkInterface(u8 new_value) {
  }
  void set_readRate(s8 new_value) {
  }
  void set_writeRate(s8 new_value) {
  }

  using JfrEvent<EventNetworkUtilization>::commit; // else commit() is hidden by overloaded versions in this class

  EventNetworkUtilization(
    u8 networkInterface,
    s8 readRate,
    s8 writeRate) {
  }

  void commit(u8 networkInterface,
              s8 readRate,
              s8 writeRate) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 networkInterface,
                     s8 readRate,
                     s8 writeRate) {
  }
};

class EventJavaThreadStatistics : public JfrEvent<EventJavaThreadStatistics>
{
 public:
  EventJavaThreadStatistics(EventStartTime timing=TIMED) {}

  void set_activeCount(s8 new_value) {
  }
  void set_daemonCount(s8 new_value) {
  }
  void set_accumulatedCount(s8 new_value) {
  }
  void set_peakCount(s8 new_value) {
  }

  using JfrEvent<EventJavaThreadStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventJavaThreadStatistics(
    s8 activeCount,
    s8 daemonCount,
    s8 accumulatedCount,
    s8 peakCount) {
  }

  void commit(s8 activeCount,
              s8 daemonCount,
              s8 accumulatedCount,
              s8 peakCount) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s8 activeCount,
                     s8 daemonCount,
                     s8 accumulatedCount,
                     s8 peakCount) {
  }
};

class EventClassLoadingStatistics : public JfrEvent<EventClassLoadingStatistics>
{
 public:
  EventClassLoadingStatistics(EventStartTime timing=TIMED) {}

  void set_loadedClassCount(s8 new_value) {
  }
  void set_unloadedClassCount(s8 new_value) {
  }

  using JfrEvent<EventClassLoadingStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventClassLoadingStatistics(
    s8 loadedClassCount,
    s8 unloadedClassCount) {
  }

  void commit(s8 loadedClassCount,
              s8 unloadedClassCount) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s8 loadedClassCount,
                     s8 unloadedClassCount) {
  }
};

class EventClassLoaderStatistics : public JfrEvent<EventClassLoaderStatistics>
{
 public:
  EventClassLoaderStatistics(EventStartTime timing=TIMED) {}

  void set_classLoader(const ClassLoaderData* new_value) {
  }
  void set_parentClassLoader(const ClassLoaderData* new_value) {
  }
  void set_classLoaderData(u8 new_value) {
  }
  void set_classCount(s8 new_value) {
  }
  void set_chunkSize(u8 new_value) {
  }
  void set_blockSize(u8 new_value) {
  }
  void set_hiddenClassCount(s8 new_value) {
  }
  void set_hiddenChunkSize(u8 new_value) {
  }
  void set_hiddenBlockSize(u8 new_value) {
  }

  using JfrEvent<EventClassLoaderStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventClassLoaderStatistics(
    const ClassLoaderData* classLoader,
    const ClassLoaderData* parentClassLoader,
    u8 classLoaderData,
    s8 classCount,
    u8 chunkSize,
    u8 blockSize,
    s8 hiddenClassCount,
    u8 hiddenChunkSize,
    u8 hiddenBlockSize) {
  }

  void commit(const ClassLoaderData* classLoader,
              const ClassLoaderData* parentClassLoader,
              u8 classLoaderData,
              s8 classCount,
              u8 chunkSize,
              u8 blockSize,
              s8 hiddenClassCount,
              u8 hiddenChunkSize,
              u8 hiddenBlockSize) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const ClassLoaderData* classLoader,
                     const ClassLoaderData* parentClassLoader,
                     u8 classLoaderData,
                     s8 classCount,
                     u8 chunkSize,
                     u8 blockSize,
                     s8 hiddenClassCount,
                     u8 hiddenChunkSize,
                     u8 hiddenBlockSize) {
  }
};

class EventSymbolTableStatistics : public JfrEvent<EventSymbolTableStatistics>
{
 public:
  EventSymbolTableStatistics(EventStartTime timing=TIMED) {}

  void set_bucketCount(u8 new_value) {
  }
  void set_entryCount(u8 new_value) {
  }
  void set_totalFootprint(u8 new_value) {
  }
  void set_bucketCountMaximum(u8 new_value) {
  }
  void set_bucketCountAverage(float new_value) {
  }
  void set_bucketCountVariance(float new_value) {
  }
  void set_bucketCountStandardDeviation(float new_value) {
  }
  void set_insertionRate(float new_value) {
  }
  void set_removalRate(float new_value) {
  }

  using JfrEvent<EventSymbolTableStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventSymbolTableStatistics(
    u8 bucketCount,
    u8 entryCount,
    u8 totalFootprint,
    u8 bucketCountMaximum,
    float bucketCountAverage,
    float bucketCountVariance,
    float bucketCountStandardDeviation,
    float insertionRate,
    float removalRate) {
  }

  void commit(u8 bucketCount,
              u8 entryCount,
              u8 totalFootprint,
              u8 bucketCountMaximum,
              float bucketCountAverage,
              float bucketCountVariance,
              float bucketCountStandardDeviation,
              float insertionRate,
              float removalRate) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 bucketCount,
                     u8 entryCount,
                     u8 totalFootprint,
                     u8 bucketCountMaximum,
                     float bucketCountAverage,
                     float bucketCountVariance,
                     float bucketCountStandardDeviation,
                     float insertionRate,
                     float removalRate) {
  }
};

class EventStringTableStatistics : public JfrEvent<EventStringTableStatistics>
{
 public:
  EventStringTableStatistics(EventStartTime timing=TIMED) {}

  void set_bucketCount(u8 new_value) {
  }
  void set_entryCount(u8 new_value) {
  }
  void set_totalFootprint(u8 new_value) {
  }
  void set_bucketCountMaximum(u8 new_value) {
  }
  void set_bucketCountAverage(float new_value) {
  }
  void set_bucketCountVariance(float new_value) {
  }
  void set_bucketCountStandardDeviation(float new_value) {
  }
  void set_insertionRate(float new_value) {
  }
  void set_removalRate(float new_value) {
  }

  using JfrEvent<EventStringTableStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventStringTableStatistics(
    u8 bucketCount,
    u8 entryCount,
    u8 totalFootprint,
    u8 bucketCountMaximum,
    float bucketCountAverage,
    float bucketCountVariance,
    float bucketCountStandardDeviation,
    float insertionRate,
    float removalRate) {
  }

  void commit(u8 bucketCount,
              u8 entryCount,
              u8 totalFootprint,
              u8 bucketCountMaximum,
              float bucketCountAverage,
              float bucketCountVariance,
              float bucketCountStandardDeviation,
              float insertionRate,
              float removalRate) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 bucketCount,
                     u8 entryCount,
                     u8 totalFootprint,
                     u8 bucketCountMaximum,
                     float bucketCountAverage,
                     float bucketCountVariance,
                     float bucketCountStandardDeviation,
                     float insertionRate,
                     float removalRate) {
  }
};

class EventThreadAllocationStatistics : public JfrEvent<EventThreadAllocationStatistics>
{
 public:
  EventThreadAllocationStatistics(EventStartTime timing=TIMED) {}

  void set_allocated(u8 new_value) {
  }
  void set_thread(u8 new_value) {
  }

  using JfrEvent<EventThreadAllocationStatistics>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadAllocationStatistics(
    u8 allocated,
    u8 thread) {
  }

  void commit(u8 allocated,
              u8 thread) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 allocated,
                     u8 thread) {
  }
};

class EventPhysicalMemory : public JfrEvent<EventPhysicalMemory>
{
 public:
  EventPhysicalMemory(EventStartTime timing=TIMED) {}

  void set_totalSize(u8 new_value) {
  }
  void set_usedSize(u8 new_value) {
  }

  using JfrEvent<EventPhysicalMemory>::commit; // else commit() is hidden by overloaded versions in this class

  EventPhysicalMemory(
    u8 totalSize,
    u8 usedSize) {
  }

  void commit(u8 totalSize,
              u8 usedSize) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 totalSize,
                     u8 usedSize) {
  }
};

class EventSwapSpace : public JfrEvent<EventSwapSpace>
{
 public:
  EventSwapSpace(EventStartTime timing=TIMED) {}

  void set_totalSize(s8 new_value) {
  }
  void set_freeSize(s8 new_value) {
  }

  using JfrEvent<EventSwapSpace>::commit; // else commit() is hidden by overloaded versions in this class

  EventSwapSpace(
    s8 totalSize,
    s8 freeSize) {
  }

  void commit(s8 totalSize,
              s8 freeSize) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     s8 totalSize,
                     s8 freeSize) {
  }
};

class EventExecutionSample : public JfrEvent<EventExecutionSample>
{
 public:
  EventExecutionSample(EventStartTime timing=TIMED) {}

  void set_sampledThread(u8 new_value) {
  }
  void set_stackTrace(u8 new_value) {
  }
  void set_state(u8 new_value) {
  }

  using JfrEvent<EventExecutionSample>::commit; // else commit() is hidden by overloaded versions in this class

  EventExecutionSample(
    u8 sampledThread,
    u8 stackTrace,
    u8 state) {
  }

  void commit(u8 sampledThread,
              u8 stackTrace,
              u8 state) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 sampledThread,
                     u8 stackTrace,
                     u8 state) {
  }
};

class EventNativeMethodSample : public JfrEvent<EventNativeMethodSample>
{
 public:
  EventNativeMethodSample(EventStartTime timing=TIMED) {}

  void set_sampledThread(u8 new_value) {
  }
  void set_stackTrace(u8 new_value) {
  }
  void set_state(u8 new_value) {
  }

  using JfrEvent<EventNativeMethodSample>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeMethodSample(
    u8 sampledThread,
    u8 stackTrace,
    u8 state) {
  }

  void commit(u8 sampledThread,
              u8 stackTrace,
              u8 state) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 sampledThread,
                     u8 stackTrace,
                     u8 state) {
  }
};

class EventCPUTimeSample : public JfrEvent<EventCPUTimeSample>
{
 public:
  EventCPUTimeSample(EventStartTime timing=TIMED) {}

  void set_stackTrace(u8 new_value) {
  }
  void set_eventThread(u8 new_value) {
  }
  void set_failed(bool new_value) {
  }
  void set_samplingPeriod(const Tickspan& new_value) {
  }
  void set_biased(bool new_value) {
  }

  using JfrEvent<EventCPUTimeSample>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 stackTrace,
                     u8 eventThread,
                     bool failed,
                     const Tickspan& samplingPeriod,
                     bool biased) {
  }
};

class EventCPUTimeSamplesLost : public JfrEvent<EventCPUTimeSamplesLost>
{
 public:
  EventCPUTimeSamplesLost(EventStartTime timing=TIMED) {}

  void set_lostSamples(s4 new_value) {
  }
  void set_eventThread(u8 new_value) {
  }

  using JfrEvent<EventCPUTimeSamplesLost>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 lostSamples,
                     u8 eventThread) {
  }
};

class EventThreadDump : public JfrEvent<EventThreadDump>
{
 public:
  EventThreadDump(EventStartTime timing=TIMED) {}

  void set_result(const char* new_value) {
  }

  using JfrEvent<EventThreadDump>::commit; // else commit() is hidden by overloaded versions in this class

  EventThreadDump(
    const char* result) {
  }

  void commit(const char* result) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* result) {
  }
};

class EventNativeLibrary : public JfrEvent<EventNativeLibrary>
{
 public:
  EventNativeLibrary(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_baseAddress(u8 new_value) {
  }
  void set_topAddress(u8 new_value) {
  }

  using JfrEvent<EventNativeLibrary>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeLibrary(
    const char* name,
    u8 baseAddress,
    u8 topAddress) {
  }

  void commit(const char* name,
              u8 baseAddress,
              u8 topAddress) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     u8 baseAddress,
                     u8 topAddress) {
  }
};

class EventNativeLibraryLoad : public JfrEvent<EventNativeLibraryLoad>
{
 public:
  EventNativeLibraryLoad(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_success(bool new_value) {
  }
  void set_errorMessage(const char* new_value) {
  }
  void set_fpuCorrectionAttempt(bool new_value) {
  }
  void set_fpuCorrectionSuccess(bool new_value) {
  }

  using JfrEvent<EventNativeLibraryLoad>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeLibraryLoad(
    const char* name,
    bool success,
    const char* errorMessage,
    bool fpuCorrectionAttempt,
    bool fpuCorrectionSuccess) {
  }

  void commit(const char* name,
              bool success,
              const char* errorMessage,
              bool fpuCorrectionAttempt,
              bool fpuCorrectionSuccess) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     bool success,
                     const char* errorMessage,
                     bool fpuCorrectionAttempt,
                     bool fpuCorrectionSuccess) {
  }
};

class EventNativeLibraryUnload : public JfrEvent<EventNativeLibraryUnload>
{
 public:
  EventNativeLibraryUnload(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_success(bool new_value) {
  }
  void set_errorMessage(const char* new_value) {
  }

  using JfrEvent<EventNativeLibraryUnload>::commit; // else commit() is hidden by overloaded versions in this class

  EventNativeLibraryUnload(
    const char* name,
    bool success,
    const char* errorMessage) {
  }

  void commit(const char* name,
              bool success,
              const char* errorMessage) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     bool success,
                     const char* errorMessage) {
  }
};

class EventModuleRequire : public JfrEvent<EventModuleRequire>
{
 public:
  EventModuleRequire(EventStartTime timing=TIMED) {}

  void set_source(const ModuleEntry* new_value) {
  }
  void set_requiredModule(const ModuleEntry* new_value) {
  }

  using JfrEvent<EventModuleRequire>::commit; // else commit() is hidden by overloaded versions in this class

  EventModuleRequire(
    const ModuleEntry* source,
    const ModuleEntry* requiredModule) {
  }

  void commit(const ModuleEntry* source,
              const ModuleEntry* requiredModule) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const ModuleEntry* source,
                     const ModuleEntry* requiredModule) {
  }
};

class EventModuleExport : public JfrEvent<EventModuleExport>
{
 public:
  EventModuleExport(EventStartTime timing=TIMED) {}

  void set_exportedPackage(const PackageEntry* new_value) {
  }
  void set_targetModule(const ModuleEntry* new_value) {
  }

  using JfrEvent<EventModuleExport>::commit; // else commit() is hidden by overloaded versions in this class

  EventModuleExport(
    const PackageEntry* exportedPackage,
    const ModuleEntry* targetModule) {
  }

  void commit(const PackageEntry* exportedPackage,
              const ModuleEntry* targetModule) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const PackageEntry* exportedPackage,
                     const ModuleEntry* targetModule) {
  }
};

class EventCompilerStatistics : public JfrEvent<EventCompilerStatistics>
{
 public:
  EventCompilerStatistics(EventStartTime timing=TIMED) {}

  void set_compileCount(s4 new_value) {
  }
  void set_bailoutCount(s4 new_value) {
  }
  void set_invalidatedCount(s4 new_value) {
  }
  void set_osrCompileCount(s4 new_value) {
  }
  void set_standardCompileCount(s4 new_value) {
  }
  void set_osrBytesCompiled(u8 new_value) {
  }
  void set_standardBytesCompiled(u8 new_value) {
  }
  void set_nmethodsSize(u8 new_value) {
  }
  void set_nmethodCodeSize(u8 new_value) {
  }
  void set_peakTimeSpent(s8 new_value) {
  }
  void set_totalTimeSpent(s8 new_value) {
  }

  using JfrEvent<EventCompilerStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 compileCount,
                     s4 bailoutCount,
                     s4 invalidatedCount,
                     s4 osrCompileCount,
                     s4 standardCompileCount,
                     u8 osrBytesCompiled,
                     u8 standardBytesCompiled,
                     u8 nmethodsSize,
                     u8 nmethodCodeSize,
                     s8 peakTimeSpent,
                     s8 totalTimeSpent) {
  }
};

class EventCompilerConfiguration : public JfrEvent<EventCompilerConfiguration>
{
 public:
  EventCompilerConfiguration(EventStartTime timing=TIMED) {}

  void set_threadCount(s4 new_value) {
  }
  void set_tieredCompilation(bool new_value) {
  }
  void set_dynamicCompilerThreadCount(bool new_value) {
  }

  using JfrEvent<EventCompilerConfiguration>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(s4 threadCount,
                     bool tieredCompilation,
                     bool dynamicCompilerThreadCount) {
  }
};

class EventCodeCacheStatistics : public JfrEvent<EventCodeCacheStatistics>
{
 public:
  EventCodeCacheStatistics(EventStartTime timing=TIMED) {}

  void set_codeBlobType(u8 new_value) {
  }
  void set_startAddress(u8 new_value) {
  }
  void set_reservedTopAddress(u8 new_value) {
  }
  void set_entryCount(s4 new_value) {
  }
  void set_methodCount(s4 new_value) {
  }
  void set_adaptorCount(s4 new_value) {
  }
  void set_unallocatedCapacity(u8 new_value) {
  }
  void set_fullCount(s4 new_value) {
  }

  using JfrEvent<EventCodeCacheStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 codeBlobType,
                     u8 startAddress,
                     u8 reservedTopAddress,
                     s4 entryCount,
                     s4 methodCount,
                     s4 adaptorCount,
                     u8 unallocatedCapacity,
                     s4 fullCount) {
  }
};

class EventCodeCacheConfiguration : public JfrEvent<EventCodeCacheConfiguration>
{
 public:
  EventCodeCacheConfiguration(EventStartTime timing=TIMED) {}

  void set_initialSize(u8 new_value) {
  }
  void set_reservedSize(u8 new_value) {
  }
  void set_nonNMethodSize(u8 new_value) {
  }
  void set_profiledSize(u8 new_value) {
  }
  void set_nonProfiledSize(u8 new_value) {
  }
  void set_expansionSize(u8 new_value) {
  }
  void set_minBlockLength(u8 new_value) {
  }
  void set_startAddress(u8 new_value) {
  }
  void set_reservedTopAddress(u8 new_value) {
  }

  using JfrEvent<EventCodeCacheConfiguration>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(u8 initialSize,
                     u8 reservedSize,
                     u8 nonNMethodSize,
                     u8 profiledSize,
                     u8 nonProfiledSize,
                     u8 expansionSize,
                     u8 minBlockLength,
                     u8 startAddress,
                     u8 reservedTopAddress) {
  }
};

class EventIntFlag : public JfrEvent<EventIntFlag>
{
 public:
  EventIntFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(s4 new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventIntFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventIntFlag(
    const char* name,
    s4 value,
    u8 origin) {
  }

  void commit(const char* name,
              s4 value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     s4 value,
                     u8 origin) {
  }
};

class EventUnsignedIntFlag : public JfrEvent<EventUnsignedIntFlag>
{
 public:
  EventUnsignedIntFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(unsigned new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventUnsignedIntFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventUnsignedIntFlag(
    const char* name,
    unsigned value,
    u8 origin) {
  }

  void commit(const char* name,
              unsigned value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     unsigned value,
                     u8 origin) {
  }
};

class EventLongFlag : public JfrEvent<EventLongFlag>
{
 public:
  EventLongFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(s8 new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventLongFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventLongFlag(
    const char* name,
    s8 value,
    u8 origin) {
  }

  void commit(const char* name,
              s8 value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     s8 value,
                     u8 origin) {
  }
};

class EventUnsignedLongFlag : public JfrEvent<EventUnsignedLongFlag>
{
 public:
  EventUnsignedLongFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(u8 new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventUnsignedLongFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventUnsignedLongFlag(
    const char* name,
    u8 value,
    u8 origin) {
  }

  void commit(const char* name,
              u8 value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     u8 value,
                     u8 origin) {
  }
};

class EventDoubleFlag : public JfrEvent<EventDoubleFlag>
{
 public:
  EventDoubleFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(double new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventDoubleFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventDoubleFlag(
    const char* name,
    double value,
    u8 origin) {
  }

  void commit(const char* name,
              double value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     double value,
                     u8 origin) {
  }
};

class EventBooleanFlag : public JfrEvent<EventBooleanFlag>
{
 public:
  EventBooleanFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(bool new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventBooleanFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventBooleanFlag(
    const char* name,
    bool value,
    u8 origin) {
  }

  void commit(const char* name,
              bool value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     bool value,
                     u8 origin) {
  }
};

class EventStringFlag : public JfrEvent<EventStringFlag>
{
 public:
  EventStringFlag(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_value(const char* new_value) {
  }
  void set_origin(u8 new_value) {
  }

  using JfrEvent<EventStringFlag>::commit; // else commit() is hidden by overloaded versions in this class

  EventStringFlag(
    const char* name,
    const char* value,
    u8 origin) {
  }

  void commit(const char* name,
              const char* value,
              u8 origin) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* name,
                     const char* value,
                     u8 origin) {
  }
};

class EventObjectCount : public JfrEvent<EventObjectCount>
{
 public:
  EventObjectCount(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_objectClass(const Klass* new_value) {
  }
  void set_count(s8 new_value) {
  }
  void set_totalSize(u8 new_value) {
  }

  using JfrEvent<EventObjectCount>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     const Klass* objectClass,
                     s8 count,
                     u8 totalSize) {
  }
};

class EventG1HeapRegionInformation : public JfrEvent<EventG1HeapRegionInformation>
{
 public:
  EventG1HeapRegionInformation(EventStartTime timing=TIMED) {}

  void set_index(unsigned new_value) {
  }
  void set_type(u8 new_value) {
  }
  void set_start(u8 new_value) {
  }
  void set_used(u8 new_value) {
  }

  using JfrEvent<EventG1HeapRegionInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventG1HeapRegionInformation(
    unsigned index,
    u8 type,
    u8 start,
    u8 used) {
  }

  void commit(unsigned index,
              u8 type,
              u8 start,
              u8 used) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned index,
                     u8 type,
                     u8 start,
                     u8 used) {
  }
};

class EventGCConfiguration : public JfrEvent<EventGCConfiguration>
{
 public:
  EventGCConfiguration(EventStartTime timing=TIMED) {}

  void set_youngCollector(u8 new_value) {
  }
  void set_oldCollector(u8 new_value) {
  }
  void set_parallelGCThreads(unsigned new_value) {
  }
  void set_concurrentGCThreads(unsigned new_value) {
  }
  void set_usesDynamicGCThreads(bool new_value) {
  }
  void set_isExplicitGCConcurrent(bool new_value) {
  }
  void set_isExplicitGCDisabled(bool new_value) {
  }
  void set_pauseTarget(s8 new_value) {
  }
  void set_gcTimeRatio(unsigned new_value) {
  }

  using JfrEvent<EventGCConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCConfiguration(
    u8 youngCollector,
    u8 oldCollector,
    unsigned parallelGCThreads,
    unsigned concurrentGCThreads,
    bool usesDynamicGCThreads,
    bool isExplicitGCConcurrent,
    bool isExplicitGCDisabled,
    s8 pauseTarget,
    unsigned gcTimeRatio) {
  }

  void commit(u8 youngCollector,
              u8 oldCollector,
              unsigned parallelGCThreads,
              unsigned concurrentGCThreads,
              bool usesDynamicGCThreads,
              bool isExplicitGCConcurrent,
              bool isExplicitGCDisabled,
              s8 pauseTarget,
              unsigned gcTimeRatio) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 youngCollector,
                     u8 oldCollector,
                     unsigned parallelGCThreads,
                     unsigned concurrentGCThreads,
                     bool usesDynamicGCThreads,
                     bool isExplicitGCConcurrent,
                     bool isExplicitGCDisabled,
                     s8 pauseTarget,
                     unsigned gcTimeRatio) {
  }
};

class EventGCSurvivorConfiguration : public JfrEvent<EventGCSurvivorConfiguration>
{
 public:
  EventGCSurvivorConfiguration(EventStartTime timing=TIMED) {}

  void set_maxTenuringThreshold(u1 new_value) {
  }
  void set_initialTenuringThreshold(u1 new_value) {
  }

  using JfrEvent<EventGCSurvivorConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCSurvivorConfiguration(
    u1 maxTenuringThreshold,
    u1 initialTenuringThreshold) {
  }

  void commit(u1 maxTenuringThreshold,
              u1 initialTenuringThreshold) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u1 maxTenuringThreshold,
                     u1 initialTenuringThreshold) {
  }
};

class EventGCTLABConfiguration : public JfrEvent<EventGCTLABConfiguration>
{
 public:
  EventGCTLABConfiguration(EventStartTime timing=TIMED) {}

  void set_usesTLABs(bool new_value) {
  }
  void set_minTLABSize(u8 new_value) {
  }
  void set_tlabRefillWasteLimit(u8 new_value) {
  }

  using JfrEvent<EventGCTLABConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCTLABConfiguration(
    bool usesTLABs,
    u8 minTLABSize,
    u8 tlabRefillWasteLimit) {
  }

  void commit(bool usesTLABs,
              u8 minTLABSize,
              u8 tlabRefillWasteLimit) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     bool usesTLABs,
                     u8 minTLABSize,
                     u8 tlabRefillWasteLimit) {
  }
};

class EventGCHeapConfiguration : public JfrEvent<EventGCHeapConfiguration>
{
 public:
  EventGCHeapConfiguration(EventStartTime timing=TIMED) {}

  void set_minSize(u8 new_value) {
  }
  void set_maxSize(u8 new_value) {
  }
  void set_initialSize(u8 new_value) {
  }
  void set_usesCompressedOops(bool new_value) {
  }
  void set_compressedOopsMode(u8 new_value) {
  }
  void set_objectAlignment(u8 new_value) {
  }
  void set_heapAddressBits(u1 new_value) {
  }

  using JfrEvent<EventGCHeapConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventGCHeapConfiguration(
    u8 minSize,
    u8 maxSize,
    u8 initialSize,
    bool usesCompressedOops,
    u8 compressedOopsMode,
    u8 objectAlignment,
    u1 heapAddressBits) {
  }

  void commit(u8 minSize,
              u8 maxSize,
              u8 initialSize,
              bool usesCompressedOops,
              u8 compressedOopsMode,
              u8 objectAlignment,
              u1 heapAddressBits) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 minSize,
                     u8 maxSize,
                     u8 initialSize,
                     bool usesCompressedOops,
                     u8 compressedOopsMode,
                     u8 objectAlignment,
                     u1 heapAddressBits) {
  }
};

class EventYoungGenerationConfiguration : public JfrEvent<EventYoungGenerationConfiguration>
{
 public:
  EventYoungGenerationConfiguration(EventStartTime timing=TIMED) {}

  void set_minSize(u8 new_value) {
  }
  void set_maxSize(u8 new_value) {
  }
  void set_newRatio(unsigned new_value) {
  }

  using JfrEvent<EventYoungGenerationConfiguration>::commit; // else commit() is hidden by overloaded versions in this class

  EventYoungGenerationConfiguration(
    u8 minSize,
    u8 maxSize,
    unsigned newRatio) {
  }

  void commit(u8 minSize,
              u8 maxSize,
              unsigned newRatio) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 minSize,
                     u8 maxSize,
                     unsigned newRatio) {
  }
};

class EventZYoungGarbageCollection : public JfrEvent<EventZYoungGarbageCollection>
{
 public:
  EventZYoungGarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_tenuringThreshold(unsigned new_value) {
  }

  using JfrEvent<EventZYoungGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventZYoungGarbageCollection(
    unsigned gcId,
    unsigned tenuringThreshold) {
  }

  void commit(unsigned gcId,
              unsigned tenuringThreshold) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     unsigned tenuringThreshold) {
  }
};

class EventZOldGarbageCollection : public JfrEvent<EventZOldGarbageCollection>
{
 public:
  EventZOldGarbageCollection(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }

  using JfrEvent<EventZOldGarbageCollection>::commit; // else commit() is hidden by overloaded versions in this class

  EventZOldGarbageCollection(
    unsigned gcId) {
  }

  void commit(unsigned gcId) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId) {
  }
};

class EventZAllocationStall : public JfrEvent<EventZAllocationStall>
{
 public:
  EventZAllocationStall(EventStartTime timing=TIMED) {}

  void set_type(u8 new_value) {
  }
  void set_size(u8 new_value) {
  }

  using JfrEvent<EventZAllocationStall>::commit; // else commit() is hidden by overloaded versions in this class

  EventZAllocationStall(
    u8 type,
    u8 size) {
  }

  void commit(u8 type,
              u8 size) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 size) {
  }
};

class EventZPageAllocation : public JfrEvent<EventZPageAllocation>
{
 public:
  EventZPageAllocation(EventStartTime timing=TIMED) {}

  void set_type(u8 new_value) {
  }
  void set_size(u8 new_value) {
  }
  void set_harvested(u8 new_value) {
  }
  void set_committed(u8 new_value) {
  }
  void set_harvestedRanges(unsigned new_value) {
  }
  void set_multiPartition(bool new_value) {
  }
  void set_successful(bool new_value) {
  }
  void set_nonBlocking(bool new_value) {
  }

  using JfrEvent<EventZPageAllocation>::commit; // else commit() is hidden by overloaded versions in this class

  EventZPageAllocation(
    u8 type,
    u8 size,
    u8 harvested,
    u8 committed,
    unsigned harvestedRanges,
    bool multiPartition,
    bool successful,
    bool nonBlocking) {
  }

  void commit(u8 type,
              u8 size,
              u8 harvested,
              u8 committed,
              unsigned harvestedRanges,
              bool multiPartition,
              bool successful,
              bool nonBlocking) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 size,
                     u8 harvested,
                     u8 committed,
                     unsigned harvestedRanges,
                     bool multiPartition,
                     bool successful,
                     bool nonBlocking) {
  }
};

class EventZRelocationSet : public JfrEvent<EventZRelocationSet>
{
 public:
  EventZRelocationSet(EventStartTime timing=TIMED) {}

  void set_total(u8 new_value) {
  }
  void set_empty(u8 new_value) {
  }
  void set_relocate(u8 new_value) {
  }

  using JfrEvent<EventZRelocationSet>::commit; // else commit() is hidden by overloaded versions in this class

  EventZRelocationSet(
    u8 total,
    u8 empty,
    u8 relocate) {
  }

  void commit(u8 total,
              u8 empty,
              u8 relocate) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 total,
                     u8 empty,
                     u8 relocate) {
  }
};

class EventZRelocationSetGroup : public JfrEvent<EventZRelocationSetGroup>
{
 public:
  EventZRelocationSetGroup(EventStartTime timing=TIMED) {}

  void set_type(u8 new_value) {
  }
  void set_candidatePages(u8 new_value) {
  }
  void set_total(u8 new_value) {
  }
  void set_empty(u8 new_value) {
  }
  void set_selectedPages(u8 new_value) {
  }
  void set_relocate(u8 new_value) {
  }

  using JfrEvent<EventZRelocationSetGroup>::commit; // else commit() is hidden by overloaded versions in this class

  EventZRelocationSetGroup(
    u8 type,
    u8 candidatePages,
    u8 total,
    u8 empty,
    u8 selectedPages,
    u8 relocate) {
  }

  void commit(u8 type,
              u8 candidatePages,
              u8 total,
              u8 empty,
              u8 selectedPages,
              u8 relocate) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 type,
                     u8 candidatePages,
                     u8 total,
                     u8 empty,
                     u8 selectedPages,
                     u8 relocate) {
  }
};

class EventZStatisticsCounter : public JfrEvent<EventZStatisticsCounter>
{
 public:
  EventZStatisticsCounter(EventStartTime timing=TIMED) {}

  void set_id(u8 new_value) {
  }
  void set_increment(u8 new_value) {
  }
  void set_value(u8 new_value) {
  }

  using JfrEvent<EventZStatisticsCounter>::commit; // else commit() is hidden by overloaded versions in this class

  EventZStatisticsCounter(
    u8 id,
    u8 increment,
    u8 value) {
  }

  void commit(u8 id,
              u8 increment,
              u8 value) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 id,
                     u8 increment,
                     u8 value) {
  }
};

class EventZStatisticsSampler : public JfrEvent<EventZStatisticsSampler>
{
 public:
  EventZStatisticsSampler(EventStartTime timing=TIMED) {}

  void set_id(u8 new_value) {
  }
  void set_value(u8 new_value) {
  }

  using JfrEvent<EventZStatisticsSampler>::commit; // else commit() is hidden by overloaded versions in this class

  EventZStatisticsSampler(
    u8 id,
    u8 value) {
  }

  void commit(u8 id,
              u8 value) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 id,
                     u8 value) {
  }
};

class EventZThreadPhase : public JfrEvent<EventZThreadPhase>
{
 public:
  EventZThreadPhase(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_name(const char* new_value) {
  }

  using JfrEvent<EventZThreadPhase>::commit; // else commit() is hidden by overloaded versions in this class

  EventZThreadPhase(
    unsigned gcId,
    const char* name) {
  }

  void commit(unsigned gcId,
              const char* name) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned gcId,
                     const char* name) {
  }
};

class EventZUncommit : public JfrEvent<EventZUncommit>
{
 public:
  EventZUncommit(EventStartTime timing=TIMED) {}

  void set_uncommitted(u8 new_value) {
  }

  using JfrEvent<EventZUncommit>::commit; // else commit() is hidden by overloaded versions in this class

  EventZUncommit(
    u8 uncommitted) {
  }

  void commit(u8 uncommitted) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 uncommitted) {
  }
};

class EventShenandoahHeapRegionStateChange : public JfrEvent<EventShenandoahHeapRegionStateChange>
{
 public:
  EventShenandoahHeapRegionStateChange(EventStartTime timing=TIMED) {}

  void set_index(unsigned new_value) {
  }
  void set_from(u8 new_value) {
  }
  void set_to(u8 new_value) {
  }
  void set_start(u8 new_value) {
  }
  void set_used(u8 new_value) {
  }

  using JfrEvent<EventShenandoahHeapRegionStateChange>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned index,
                     u8 from,
                     u8 to,
                     u8 start,
                     u8 used) {
  }
};

class EventShenandoahHeapRegionInformation : public JfrEvent<EventShenandoahHeapRegionInformation>
{
 public:
  EventShenandoahHeapRegionInformation(EventStartTime timing=TIMED) {}

  void set_index(unsigned new_value) {
  }
  void set_state(u8 new_value) {
  }
  void set_start(u8 new_value) {
  }
  void set_used(u8 new_value) {
  }

  using JfrEvent<EventShenandoahHeapRegionInformation>::commit; // else commit() is hidden by overloaded versions in this class

  EventShenandoahHeapRegionInformation(
    unsigned index,
    u8 state,
    u8 start,
    u8 used) {
  }

  void commit(unsigned index,
              u8 state,
              u8 start,
              u8 used) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     unsigned index,
                     u8 state,
                     u8 start,
                     u8 used) {
  }
};

class EventShenandoahEvacuationInformation : public JfrEvent<EventShenandoahEvacuationInformation>
{
 public:
  EventShenandoahEvacuationInformation(EventStartTime timing=TIMED) {}

  void set_gcId(unsigned new_value) {
  }
  void set_cSetRegions(u8 new_value) {
  }
  void set_cSetUsedBefore(u8 new_value) {
  }
  void set_cSetUsedAfter(u8 new_value) {
  }
  void set_collectedOld(u8 new_value) {
  }
  void set_collectedPromoted(u8 new_value) {
  }
  void set_collectedYoung(u8 new_value) {
  }
  void set_regionsPromotedHumongous(u8 new_value) {
  }
  void set_regionsPromotedRegular(u8 new_value) {
  }
  void set_regularPromotedGarbage(u8 new_value) {
  }
  void set_regularPromotedFree(u8 new_value) {
  }
  void set_freeRegions(u8 new_value) {
  }
  void set_regionsImmediate(u8 new_value) {
  }
  void set_immediateBytes(u8 new_value) {
  }

  using JfrEvent<EventShenandoahEvacuationInformation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(unsigned gcId,
                     u8 cSetRegions,
                     u8 cSetUsedBefore,
                     u8 cSetUsedAfter,
                     u8 collectedOld,
                     u8 collectedPromoted,
                     u8 collectedYoung,
                     u8 regionsPromotedHumongous,
                     u8 regionsPromotedRegular,
                     u8 regularPromotedGarbage,
                     u8 regularPromotedFree,
                     u8 freeRegions,
                     u8 regionsImmediate,
                     u8 immediateBytes) {
  }
};

class EventFlush : public JfrEvent<EventFlush>
{
 public:
  EventFlush(EventStartTime timing=TIMED) {}

  void set_flushId(u8 new_value) {
  }
  void set_elements(u8 new_value) {
  }
  void set_size(u8 new_value) {
  }

  using JfrEvent<EventFlush>::commit; // else commit() is hidden by overloaded versions in this class

  EventFlush(
    u8 flushId,
    u8 elements,
    u8 size) {
  }

  void commit(u8 flushId,
              u8 elements,
              u8 size) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     u8 flushId,
                     u8 elements,
                     u8 size) {
  }
};

class EventHeapDump : public JfrEvent<EventHeapDump>
{
 public:
  EventHeapDump(EventStartTime timing=TIMED) {}

  void set_destination(const char* new_value) {
  }
  void set_size(s8 new_value) {
  }
  void set_gcBeforeDump(bool new_value) {
  }
  void set_onOutOfMemoryError(bool new_value) {
  }
  void set_overwrite(bool new_value) {
  }
  void set_compression(s4 new_value) {
  }

  using JfrEvent<EventHeapDump>::commit; // else commit() is hidden by overloaded versions in this class

  EventHeapDump(
    const char* destination,
    s8 size,
    bool gcBeforeDump,
    bool onOutOfMemoryError,
    bool overwrite,
    s4 compression) {
  }

  void commit(const char* destination,
              s8 size,
              bool gcBeforeDump,
              bool onOutOfMemoryError,
              bool overwrite,
              s4 compression) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const char* destination,
                     s8 size,
                     bool gcBeforeDump,
                     bool onOutOfMemoryError,
                     bool overwrite,
                     s4 compression) {
  }
};

class EventFinalizerStatistics : public JfrEvent<EventFinalizerStatistics>
{
 public:
  EventFinalizerStatistics(EventStartTime timing=TIMED) {}

  void set_finalizableClass(const Klass* new_value) {
  }
  void set_codeSource(u8 new_value) {
  }
  void set_objects(u8 new_value) {
  }
  void set_totalFinalizersRun(u8 new_value) {
  }

  using JfrEvent<EventFinalizerStatistics>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Klass* finalizableClass,
                     u8 codeSource,
                     u8 objects,
                     u8 totalFinalizersRun) {
  }
};

class EventJavaAgent : public JfrEvent<EventJavaAgent>
{
 public:
  EventJavaAgent(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_options(const char* new_value) {
  }
  void set_dynamic(bool new_value) {
  }
  void set_initializationTime(const Ticks& new_value) {
  }
  void set_initializationDuration(const Tickspan& new_value) {
  }

  using JfrEvent<EventJavaAgent>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     const char* options,
                     bool dynamic,
                     const Ticks& initializationTime,
                     const Tickspan& initializationDuration) {
  }
};

class EventNativeAgent : public JfrEvent<EventNativeAgent>
{
 public:
  EventNativeAgent(EventStartTime timing=TIMED) {}

  void set_name(const char* new_value) {
  }
  void set_options(const char* new_value) {
  }
  void set_dynamic(bool new_value) {
  }
  void set_initializationTime(const Ticks& new_value) {
  }
  void set_initializationDuration(const Tickspan& new_value) {
  }
  void set_path(const char* new_value) {
  }

  using JfrEvent<EventNativeAgent>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const char* name,
                     const char* options,
                     bool dynamic,
                     const Ticks& initializationTime,
                     const Tickspan& initializationDuration,
                     const char* path) {
  }
};

class EventDeprecatedInvocation : public JfrEvent<EventDeprecatedInvocation>
{
 public:
  EventDeprecatedInvocation(EventStartTime timing=TIMED) {}

  void set_method(const Method* new_value) {
  }
  void set_invocationTime(const Ticks& new_value) {
  }
  void set_forRemoval(bool new_value) {
  }

  using JfrEvent<EventDeprecatedInvocation>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Method* method,
                     const Ticks& invocationTime,
                     bool forRemoval) {
  }
};

class EventMethodTrace : public JfrEvent<EventMethodTrace>
{
 public:
  EventMethodTrace(EventStartTime timing=TIMED) {}

  void set_method(const Method* new_value) {
  }

  using JfrEvent<EventMethodTrace>::commit; // else commit() is hidden by overloaded versions in this class

  EventMethodTrace(
    const Method* method) {
  }

  void commit(const Method* method) {
  }

  static void commit(const Ticks& startTicks,
                     const Ticks& endTicks,
                     const Method* method) {
  }
};

class EventMethodTiming : public JfrEvent<EventMethodTiming>
{
 public:
  EventMethodTiming(EventStartTime timing=TIMED) {}

  void set_method(const Method* new_value) {
  }
  void set_invocations(s8 new_value) {
  }
  void set_minimum(const Tickspan& new_value) {
  }
  void set_average(const Tickspan& new_value) {
  }
  void set_maximum(const Tickspan& new_value) {
  }

  using JfrEvent<EventMethodTiming>::commit; // else commit() is hidden by overloaded versions in this class



  static void commit(const Method* method,
                     s8 invocations,
                     const Tickspan& minimum,
                     const Tickspan& average,
                     const Tickspan& maximum) {
  }
};

template <typename EventType>
class JfrNonReentrant : public EventType {
 public:
  JfrNonReentrant(EventStartTime timing = TIMED)
  {}
}; 


#endif // INCLUDE_JFR
#endif // JFRFILES_JFREVENTCLASSES_HPP
