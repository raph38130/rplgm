/**************************************************************************** * *  Copyright (C) 2004 Dallas Semiconductor Corporation. All rights Reserved. * *  Permission is hereby granted, free of charge, to any person obtaining a *  copy of this software and associated documentation files (the "Software"), *  to deal in the Software without restriction, including without limitation *  the rights to use, copy, modify, merge, publish, distribute, sublicense, *  and/or sell copies of the Software, and to permit persons to whom the *  Software is furnished to do so, subject to the following conditions: * *  The above copyright notice and this permission notice shall be included *  in all copies or substantial portions of the Software. * *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF *  MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *  IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES *  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *  OTHER DEALINGS IN THE SOFTWARE. * *  Except as contained in this notice, the name of Dallas Semiconductor *  shall not be used except as stated in the Dallas Semiconductor *  Branding Policy. * *  Author: KLA *  Original Version: March 11, 2004 * *  Revision History: *     03-11-04 KLA 1.01 Initial version of software.  This tool allows you to convert any  *                       binary image into a TBIN file suitable for loading by TINI's *                       bootloader programs. * */import java.io.*;public class TBINOutputStream extends OutputStream
{
	ByteArrayOutputStream baos;
	int starting_address;
	OutputStream root;
	public  TBINOutputStream(OutputStream r, int address) throws Exception	{
		root = r;
		starting_address = address;
		baos = new ByteArrayOutputStream();	}

	public void write(byte[] buff, int start, int length)
	{
		baos.write(buff, start, length);
	}

	public void write(byte[] buff)
	{
		baos.write(buff, 0, buff.length);
	}

	public void write(int x)
	{
		baos.write(x);
	}	public void flush()
	{
		try
		{
			if (baos != null)
			{
				byte[] output = baos.toByteArray();				int offset = 0;				int bytestogo = output.length;
				int address = starting_address;
				int lastaddress = address;				int bytesnow;						while (bytestogo > 0)				{					// Write the starting address for this block.					root.write(offset+address);					root.write((offset+address) >>> 8);					root.write((offset+address) >>> 16);							if (bytestogo > (65536-(lastaddress & 0xFFFF)))						bytesnow = (65536-(lastaddress & 0xFFFF));					else						bytesnow = bytestogo;					// Write length (length - 1 is what we write)					root.write(bytesnow - 1);					root.write((bytesnow - 1) >>> 8);					// Write data					int crc = computeCRC(output,offset,bytesnow,0);					root.write(output,offset,bytesnow);					root.write(crc);					root.write(crc >>> 8);System.out.println("Segment start address: "+Integer.toHexString(offset+address));System.out.println("   length: "+bytesnow);System.out.println("      CRC: "+Integer.toHexString(crc & 0xFFFF));					bytestogo -= bytesnow;					offset += bytesnow;					lastaddress += bytesnow;				}
				baos = null;
			}
		}
		catch(Exception e)
		{
			System.out.println("Exception writing TBIN output: "+e);
		}
	}

	public void close()
	{
		flush();
	}	private static final int[] ODD_PARITY = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };	static int computeCRC (int dataToCrc, int seed)	{		int dat = ((dataToCrc ^ (seed & 0xFF)) & 0xFF);			seed = seed >>> 8;		int indx1 = (dat & 0x0F);		int indx2 = (dat >>> 4);			if ((ODD_PARITY [indx1] ^ ODD_PARITY [indx2]) == 1)			seed = seed ^ 0xC001;			dat  = (dat << 6);		seed = seed ^ dat;		dat  = (dat << 1);		seed = seed ^ dat;		return seed;	}	static int computeCRC (byte dataToCrc [], int off, int len)	{		return computeCRC(dataToCrc, off, len, 0);	}	static int computeCRC (byte dataToCrc [], int off, int len, int seed)	{		// loop to do the crc on each data element		for (int i = 0; i < len; i++)			seed = computeCRC(dataToCrc [i + off], seed);		return seed;	}}

