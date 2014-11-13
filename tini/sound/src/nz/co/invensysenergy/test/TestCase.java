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
import com.dalsemi.system.ArrayUtils;

/**
 * This class provides the base framework to write a unit test. Simply extend
 * this class and provide an implementation for {@link #runTest}.<p>
 *
 * In general, collections of TestCases can be placed in a {@link TestSuite}
 * which is a subclass of TestCase in accordance with the Composite design
 * pattern.<p>
 *
 * A spin on a straight unit test is the concept of "test sets". A particular
 * TestCase may not be suitable for all environments. For example, a test may
 * only be intended to run on the Dallas Semiconductor's TINI board, or a comms
 * test may require a certain test set up. In that case, the TestCase may be
 * declared to belong to one or more "test sets", using {@link #addTestSet}. By
 * default, a TestCase belongs to all test sets. The null test set includes all
 * TestCases.
 */
public abstract class TestCase extends Asserts {

  /**
   * Since TestCases are composites, they can have parents, although I would
   * expect only one parent.
   */
  private TestCase parent;

  /**
   * We save the String once it has been determined to save time.
   */
  private String testName;

  /**
   * This is a vector of String naming all testSets supported by this TestCase.
   * If testSets is null then the TestCase belongs to all testSets.
   */
  private Vector testSets;

  /**
   * This function determines whether the TestCase belongs to this testSet.
   *
   * @param testSet The testSet that the TestCase may (or may not) belong to.
   * @return If testSets is null (ie the TestCase belongs to all testSets), or
   * testSet is "" (ie the testSet is the null case) or the TestCase supports
   * the specified testSet then true. Otherwise false.
   */
  private boolean testSetsSupported(final String[] targetTestSets) {
    check(targetTestSets != null);
    if ((targetTestSets.length > 0) && (testSets != null)) {
      for (int i = 0; i < targetTestSets.length; i++) {
        String thisTestSet;
        final int size = testSets.size();
        for (int j = 0; j < size; j++) {
          thisTestSet = (String) testSets.elementAt(j);
          if (thisTestSet.equalsIgnoreCase(targetTestSets[i])) {
            return true;
          }
        }
      }
      return false;
    } else {
      return true;
    }
  }

  /**
   * This command adds the specified testSet to the list of testSets supported
   * by this TestCase. If this method is not called for a TestCase, then the
   * TestCase belongs to all testSets.
   */
  public void addTestSet(String testSet) {
    check(testSet != null);
    check(!"".equals(testSet));

    if (testSets == null) {
      testSets = new Vector();
    }
    testSets.addElement(testSet);
  }

  /**
   * This method executes the test, provided it belongs to one of the specified testSets.
   *
   * @param testSet The test will only be executed if it belongs to this testSet
   * (or it belongs to all testSets). If testSet is the empty string then the
   * TestCase will execute.
   */
  public void go(String[] targetTestSets, TestReport report) {
    if (testSetsSupported(targetTestSets)) {
      System.out.println(this + " test started.");
      try {
        try {
          runTest();
        }
        catch (VirtualMachineError e) {
          System.err.println("Unexpected VirtualMachineError: " + e);
          freeMemory();
          System.err.println("Trying test again.");
          runTest();
        }
        report.okay();
      }
      catch (Exception e) {
        System.err.println("TEST FAILED: " + e);
        report.failed(this, e);
      }
    }
  }

  /**
   * This method must be overriden to provide the implementation of the TestCase.
   *
   * @throws Exception If the TestCase fails.
   */
  protected abstract void runTest() throws Exception;

  /**
   * This utility method can be called by any test to ensure it has enough headroom
   * on the TINI. In theory, it should pause long enough for any rubbish to be
   * garbage collected.
   */
  protected void freeMemory() {
    if ("TINI".equals(System.getProperty("os.arch"))) {
      Runtime r = Runtime.getRuntime();
      long oldMem;
      System.gc();
      try {
        do {
          oldMem = r.freeMemory();
          Thread.sleep(1000);
        } while (oldMem < r.freeMemory());
      }
      catch (InterruptedException burn) {
        // Do nothing.
      }
    }
  }

  protected void setParent(TestCase newParent) {
    check(newParent != null, "setParent, newParent is null");
    check(parent == null, "setParent, parent is not null");

    parent = newParent;

    // 29/08/01.GK: At this point, we could set testName to null to force it
    // to be recalculated to include the new parent. However, in practice, this
    // seems to be fine: ie test names include enough detail to be identifiable.
  }

  public String toString() {
    if (testName == null) {
      String classname = getClass().getName();
      int lastFullStop = classname.lastIndexOf('.');
      StringBuffer myName = new StringBuffer(classname.substring(lastFullStop + 1));
      if (parent != null) {
        myName.insert(0, '/');
        myName.insert(0, parent.toString());
      }
      testName = myName.toString();
    }
    return testName;
  }

}