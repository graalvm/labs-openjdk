/*
 * Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
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
 */
import java.awt.print.PrinterJob;
import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttributeSet;
import javax.print.attribute.standard.DialogTypeSelection;

/**
 * @test
 * @bug 8039412
 * @key printer
 * @run main/manual PageDlgStackOverflowTest
 * @summary Calling pageDialog() after printDialog with
 *          DialogTypeSelection.NATIVE should not result in StackOverflowError
 */
public class PageDlgStackOverflowTest {

    public static void main(String args[]) {
        PrinterJob job = PrinterJob.getPrinterJob();
        if (job == null) {
            return;
        }
        PrintRequestAttributeSet pSet =
             new HashPrintRequestAttributeSet();
        pSet.add(DialogTypeSelection.NATIVE);
        job.printDialog(pSet);
        try {
            job.pageDialog(pSet);
        } catch (StackOverflowError e) {
            throw new RuntimeException("StackOverflowError is thrown");
        }
    }
}

