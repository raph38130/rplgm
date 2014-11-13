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

import java.util.Vector;

/**
 * This class allows collections of {@link TestCase} to be grouped together and
 * treated as a single TestCase in accordance with the Composite design pattern.<p>
 *
 * Note that the concept of testSet does not apply to a TestSuite as a whole.
 * Only individual TestCases have membership (or not) in a particular testSet.
 */
public class TestSuite extends TestCase {

  /**
   * This is a Vector of {@link TestCase}.
   */
  private Vector children = new Vector();

  /**
   * This method runs all the tests in this TestSuite.
   */
  protected void runTest() throws Exception {
    fail("Cannot call runTest() on a TestSuite. Use go() instead");
  }

  /**
   * This method adds a TestCase to this TestSuite.
   */
  public void addTest(TestCase child) {
    check(child != null, this+".add but child null");
    check(children != null, this+".add but children null");
    children.addElement(child);
    child.setParent(this);
  }

  /**
   * This method removes a TestCase from this TestSuite.
   */
  public void removeTest(TestCase child) {
    check(child != null, this+".remove but child null");
    check(children != null, this+".remove but children null");
    children.removeElement(child);
  }

  /**
   * TestSuites do not have individual membership in testSets. Only {@link TestCase}
   * do.
   */
  public void addTestSet(String testSet) {
    fail("TestSuites do not support testSets");
  }

  /**
   * Any TestSuite can be used as the main class for an application. Simply
   * instantiate the class in main() and then call this flavour of go().
   */
  public void go(String[] args, TestReport report) {
    check(args != null);
    check(children != null, this+"runTest, but children null");
    System.out.println("Starting Test Suite " + this);
    TestCase thisTest;
    final int size = children.size();
    while (children.size() > 0) {
      thisTest = (TestCase) children.firstElement();
      thisTest.go(args, report);
      children.removeElementAt(0);
    }
    children = null;
    freeMemory();
  }

  /**
   * This test case is included to allow the TINI board time to breath and run
   * its garbage collector. It might be useful to run this test intermittently
   * in long test suites. This is not really a pass/fail test itself, but it is
   * a TestCase just to allow it to be run as a test. Because memory is only a
   * problem on the TINI, this "test" belongs to the TINI test set.
   */
  public static class FreeMemory extends TestCase {
    public FreeMemory() {
      this.addTestSet("tini");
    }

    protected void runTest() {
      freeMemory();
    }
  }

  /**
   * Any TestSuite can be used as the main class for an application. Simply
   * instantiate the class in main() and then call this flavour of go().
   */
  public void goMain(String[] args) {
    int count;
    System.out.println(this+" started. Must see 'Test Complete' to have passed all specified test sets.");
    String testSet = "";
    System.out.print("Running test set(s): ");
    if (args.length == 0) {
      System.out.print("[AllTests]");
    } else {
      for (int i = 0; i < args.length; i++) {
        System.out.print(args[i]);
        System.out.print(" ");
      }
    }
    System.out.println();
    System.out.println();
    TestReport report = new TestReport();
    go(args, report);
    report.report();
    System.exit(report.getExitCode());
  }

}