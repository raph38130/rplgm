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
package nz.co.invensysenergy.test.comm;

import java.io.InputStream;
import java.io.IOException;
import nz.co.invensysenergy.test.Asserts;

/**
 * This class is a convenience for accumulating input from an InputStream into a
 * buffer.
 */
public class DataBuffer extends Asserts {

  private byte[] data;
  private int dataCount;
  private boolean overflow;

  public DataBuffer(int size) {
    check(size > 0, this+" constructing size zero.");
    data = new byte[size];
  }

  public void add(byte[] newData, int length) {
    int i;
    for (i = 0; (i < length) && (dataCount < data.length); i++) {
      data[dataCount++] = newData[i];
    }
    overflow = overflow || ((dataCount == data.length) && (i < length));
  }

  public void add(InputStream in) throws IOException {
    int numberToRead = in.available();
    final int possibleToRead = data.length - dataCount;
    if (numberToRead > possibleToRead) {
      overflow = true;
      numberToRead = possibleToRead;
    }
    dataCount += in.read(data, dataCount, numberToRead);
  }

  public void addBlocking(InputStream in) throws IOException {
    dataCount += in.read(data, dataCount, data.length - dataCount);
    overflow |= (in.available() > 0);
  }

  public boolean getOverflow() {
    return overflow;
  }

  /**
   * WARNING: We are not overriding the inherited method Object.equals(Object)
   * here. This is a lazy implementation just to allow comparison with byte arrays.
   * To do this properly, we need to provide an implementation that accepts Object
   * (and thus also DataBuffer) and we need to override hashCode() also.
   */
  public boolean equals(byte[] otherData) {
    boolean result = true;
    result &= !overflow;
    result &= (dataCount == otherData.length);
    for (int i = 0; ((i < dataCount) && result); i++) {
      result &= data[i] == otherData[i];
    }
    return result;
  }

  public void clear() {
    dataCount = 0;
  }

  public String reportComparison(byte[] otherData) {
    StringBuffer msg = new StringBuffer();
    if (overflow) {
      msg.append("Buffer has overflowed\n\r");
    }
    if (otherData.length != dataCount) {
      msg.append("Buffer holds " + dataCount + " bytes, expected data is " + otherData.length + " bytes");
    } else if (this.equals(otherData)) {
      msg.append("Buffer matches expected data");
    } else {
      msg.append("Buffer does not match expected data");
    }
    msg.append("\n\rExpected data: ");
    msg.append(convertByteArrayToString(otherData));
    msg.append("\n\rBuffer: ");
    msg.append(convertByteArrayToString(data, dataCount));
    return msg.toString();
  }

  public String toString() {
    return convertByteArrayToString(data, dataCount);
  }
}