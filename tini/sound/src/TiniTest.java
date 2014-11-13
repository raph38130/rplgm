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

import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.PrintStream;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.net.Socket;
import com.oroinc.net.ftp.FTP;
import com.oroinc.net.ftp.FTPClient;
import com.oroinc.net.ftp.FTPReply;
import com.oroinc.net.ftp.FTPConnectionClosedException;

/**
 * Description of the Class
 */
public class TiniTest {

  /**
   * The main program for the TiniTest class
   *
   * @param args The command line arguments
   */
  public static void main(String[] args) {
    StringBuffer program = new StringBuffer();
    for (int i = 1; i < args.length; i++) {
      program.append(args[i]);
      program.append(' ');
    }
    new TiniTest(args[0], program.toString().trim());
  }

  public TiniTest(final String filename, final String arguments) {
    final String SERVER = "tini";
    final String USERNAME = "root";
    final String PASSWORD = "tini";

    try {
      boolean okay = false;
      PrintStream out = null;
      BufferedReader in = null;
      String result= null;
      do {
        try {
          ftpFileToServer(filename, SERVER, USERNAME, PASSWORD);
          Socket socket = new Socket(SERVER, 23);
          in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
          out = new PrintStream(socket.getOutputStream());
          System.out.println(in.readLine());
          findPrompt(in, ':');
          out.println(USERNAME);
          findPrompt(in, ':');
          out.println(PASSWORD);
          findPrompt(in, '>');
          out.print("java ");
          out.print(filename);
          out.print(' ');
          out.println(arguments);
          result = findPrompt(in, '>');
          okay = result.indexOf("Could not execute file:") < 0;
          if (!okay) {
            System.out.println();
            System.out.println("Rebooting TINI to restore heap");
            System.out.println();
            out.println("reboot -h -f");
            findPrompt(in, '.');
            Thread.sleep(30000);
            System.out.println();
            System.out.println("Trying again...");
            System.out.println();
          }
        }
        catch (java.net.ConnectException e) {
          Thread.sleep(30000);
        }
      } while (!okay);
      new FileOutputStream(filename + ".txt").write(result.getBytes());
      out.print("del ");
      out.println(filename);
      findPrompt(in, '>');
      out.println("exit");
      findPrompt(in, '.');
      if (result.indexOf("0 tests failed.") < 0) {
        System.exit(1);
      }
    }
    catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }
  }

  /**
   * Description of the Method
   *
   * @param in Description of Parameter
   */
  private static String findPrompt(BufferedReader in, char prompt) throws IOException {
    char thisChar;
    StringBuffer result = new StringBuffer();
    do {
      thisChar = (char) in.read();
      result.append(thisChar);
      System.out.print(thisChar);
    } while (thisChar != prompt);
    return result.toString();
  }

  private void ftpFileToServer(String filename, String server, String username, String password) throws IOException {
    FTPClient ftp = new FTPClient();
    try {
      int reply;
      ftp.connect(server);
      System.out.println("FTP Connected to " + server + ".");

      // After connection attempt, you should check the reply code to verify
      // success.
      reply = ftp.getReplyCode();

      if(!FTPReply.isPositiveCompletion(reply)) {
        ftp.disconnect();
        throw new IOException("FTP server refused connection.");
      }

      if(!ftp.login(username, password)) {
        ftp.logout();
        throw new IOException("Login to server refused");
      }

      ftp.setFileType(FTP.BINARY_FILE_TYPE);
      ftp.storeFile(filename, new FileInputStream(filename));
      ftp.logout();
      System.out.println("FTP Complete");
    } catch(FTPConnectionClosedException e) {
      throw new IOException("Server closed connection.");
    } finally {
      if(ftp.isConnected()) {
        try {
          ftp.disconnect();
        } catch(IOException f) {
          // do nothing
        }
      }
    }
  }
}
