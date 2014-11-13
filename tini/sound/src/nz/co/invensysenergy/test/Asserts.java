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

import com.dalsemi.system.ArrayUtils;

/**
 * Provides some convenience methods.
 */
public class Asserts {

  public static void check(boolean condition, String message) {
    if (!condition) {
      fail(message);
    }
  }

  public static void check(boolean condition) {
    if (!condition) {
      throw new IllegalStateException();
    }
  }

  public static void fail(String message) {
    throw new IllegalStateException(message);
  }

  /**
   * Tests that two objects are equal.
   */
  public static void assertEquals(Object expected, Object actual) {
    check(((expected == null) && (actual == null)) || (expected != null), "Expected is " + expected + ", but actual is " + actual);

    if ((expected != null) && (expected instanceof byte[])) {
      assertEquals((byte[]) expected, (byte[]) actual);
    } else if ((expected != null) && (expected instanceof Object[])) {
        assertEquals((Object[]) expected, (Object[]) actual);
    } else {
      check((expected == null) || expected.equals(actual), "Expected: \"" + expected + "\", Actual: \"" + actual + "\"");
    }
  }

  /**
   * Tests two byte arrays are equal without using java.util.Arrays (introduced
   * in Java 1.2).
   */
  public static void assertEquals(byte[] expected, final int expectedLength, byte[] actual, final int actualLength) {
    check(expected != null, "expected byte[] null");
    check(actual != null, "actual byte[] null");
    check(expected.length >= expectedLength, "expected.length=" + expected.length + ", but expectedLength=" + expectedLength);
    check(actual.length >= actualLength, "actual.length=" + actual.length + ", but actualLength=" + actualLength);
    // We cannot afford the time to calculate the string equivalents for the byte arrays
    // for large arrays (the check method needs the String as a parameter even if the
    // check passes. Therefore perform the tests without messages, catch the exception
    // and generate the message only if needed.
    try {
      check(expectedLength == actualLength);
      check(ArrayUtils.arrayComp(expected, 0, actual, 0, actualLength));
    }
    catch (IllegalStateException e) {
      fail("Expected: [" + convertByteArrayToString(expected) + "], actual: [" + convertByteArrayToString(actual, actualLength) + "] (length=" + actualLength + ")");
    }
  }

  public static void assertEquals(byte[] expected, byte[] actual, final int actualLength) {
    assertEquals(expected, expected.length, actual, actualLength);
  }

  public static void assertEquals(byte[] expected, byte[] actual) {
    check(actual != null, "actual byte[] null");
    assertEquals(expected, expected.length, actual, actual.length);
  }

  public static void assertEquals(Object[] expected, Object[] actual) {
    check(expected != null, "assertEquals(Object[]), but expected null");
    check(actual != null, "assertEquals(Object[]), but actual null");
    check(expected.length == actual.length, "assertEquals, but Object[] lengths do not match");

    for (int i = 0; i < expected.length; i++) {
      assertEquals(expected[i], actual[i]);
    }
  }

  public static void assertEquals(int expected, int actual) {
    check(expected == actual, "Expected: " + expected + ", Actual: " + actual);
  }

  public static void assertEquals(long expected, long actual) {
    check(expected == actual, "Expected: " + expected + ", Actual: " + actual);
  }

  public static void assertEquals(float expected, float actual) {
    check((expected == actual) || (Float.isNaN(expected) && Float.isNaN(actual)), "Expected: " + expected + ", Actual: " + actual);
  }

  public static void assertEquals(boolean expected, boolean actual) {
    check(expected == actual, "Expected: " + expected + ", Actual: " + actual);
  }

  public static String convertByteArrayToString(byte[] array) {
    return convertByteArrayToString(array, array.length);
  }

  public static String convertByteArrayToString(byte[] array, int length) {
    if (array != null) {
      check(length >= 0);
      StringBuffer msg = new StringBuffer(length * 4);
      for (int i = 0; i < length; i++) {
        if (i != 0) {
          msg.append(' ');
        }
        msg.append(array[i] & 0xff);
      }
      return msg.toString();
    }
    return "null";
  }
}