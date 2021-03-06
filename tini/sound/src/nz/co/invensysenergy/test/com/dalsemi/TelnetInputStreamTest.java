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
package nz.co.invensysenergy.test.com.dalsemi;

import java.io.*;
import com.dalsemi.shell.server.telnet.TelnetInputStream;
import com.dalsemi.shell.server.SystemInputStream;
import nz.co.invensysenergy.test.TestSuite;

/**
 * This test suite tests the behaviour of the TelnetInputStream.
 * Most characteristics are tested based on SystemInputStreamTest, but it does
 * add some behavioural testing of its own.
 */

public class TelnetInputStreamTest extends SystemInputStreamTest {

  private final static byte[] TELNET_SOURCE = "The quick \377\377and the dead.\r\000".getBytes();
  private final static byte[] TELNET_RESULT = "The quick \377and the dead.\r\n".getBytes();
  private final static byte[] TELNET_ECHO = "The quick and the dead.\r\n".getBytes();

  /**
   * Adds tests in addition to the core tests defined in the parent class.
   */
  public TelnetInputStreamTest() {
    addTest(new TestTelnetCharacters());
  }

  /**
   * Returns a TelnetInputStream to allow the SystemInputStreamTest class to test
   * the TelnetInputStream behaviour which is inherited from SystemInputStream.
   *
   * @param in Basic underlying stream.
   * @param out PrintStream required for echoing.
   * @return A TelnetInputStream to be tested.
   */
  SystemInputStream constructStream(InputStream in, PrintStream out) {
    return new TelnetInputStream(in, out);
  }

  /**
   * This test checks escaped 0xFF characters and the alternate Unix line
   * termination of \r\0.
   */
  private class TestTelnetCharacters extends SystemInputStreamTest.TestBaseRead {
    protected byte[] getTestString() {
      return TELNET_SOURCE;
    }

    protected byte[] getExpectedResult() {
      return TELNET_RESULT;
    }

    protected byte[] getExpectedEcho() {
      return TELNET_ECHO;
    }

  }

}
