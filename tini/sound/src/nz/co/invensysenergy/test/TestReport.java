/**
 * TiniUnit Unit Testing Framework for the Dallas Semiconductors TINI Board
 * http://tiniunit.sourceforge.net/
 *
 * Invensys Energy Systems (NZ) Ltd
 * Christchurch, New Zealand
 * http://www.invensys-energy.com/new_zealand/
 * Contact: Gavin Kingsley <gavin.kingsley@energy.invensys.com>
 *
 * Copyright (c) 2000, 2001, Invensys Energy Systems (NZ) Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of Invensys Energy Systems (NZ) Ltd nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL INVENSYS ENERGY SYSTEMS (NZ) LTD OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * http://www.opensource.org/licenses/bsd-license.html
 */
package nz.co.invensysenergy.test;

import java.io.ByteArrayOutputStream;
import java.io.PrintWriter;

/**
 * This class accumulates the results of tests as they are run.
 */
public class TestReport {

  private int totalTestsRun;
  private int testsFailed;
  private PrintWriter failureReports;
  private ByteArrayOutputStream underlyingReport;
  private final long startTime = System.currentTimeMillis();

  /**
   * Call this method every time a test case passes.
   */
  public void okay() {
    totalTestsRun++;
  }

  /**
   * Call this method every time a test case fails.
   *
   * @param test The test that failed.
   * @param e The exception that was thrown by the test when it failed.
   */
  public void failed(TestCase test, Throwable e) {
    if (failureReports == null) {
      underlyingReport = new ByteArrayOutputStream();
      failureReports = new PrintWriter(underlyingReport);
    }
    failureReports.println(test.toString());
    e.printStackTrace(failureReports);
    failureReports.println();
    totalTestsRun++;
    testsFailed++;
  }

  /**
   * Prints a report of the tests to System.out.
   */
  public void report() {
    System.out.println();
    System.out.println("All Tests Complete (Elapsed time: " + (System.currentTimeMillis() - startTime) / 1000 + " s).");
    System.out.println(totalTestsRun + " tests run.");
    System.out.println(testsFailed + " tests failed.");
    if (testsFailed > 0) {
      System.out.println();
      System.out.println("TEST FAILURE REPORT");
      System.out.println();
      if (failureReports.checkError()) {
        System.out.println("ERROR IN TEST REPORT MECHANISM! FAILURE REPORT UNAVAILABLE?");
      }
      System.out.println(underlyingReport.toString());
    }
  }

  /**
   * Determines the appropriate exit code for the program to use. Since the TINI
   * does not support deamon threads, and you never can tell what threads have
   * been set up during testing, even a successful run of tests should call
   * System.exit().
   */
  public int getExitCode() {
    if (testsFailed > 0) {
      return 1;
    }
    return 0;
  }
}