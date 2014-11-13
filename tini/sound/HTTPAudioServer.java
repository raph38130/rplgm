
/*---------------------------------------------------------------------------
 * Copyright (C) 2004 Dallas Semiconductor Corporation, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Dallas Semiconductor
 * shall not be used except as stated in the Dallas Semiconductor
 * Branding Policy.
 *---------------------------------------------------------------------------
 */


/*
connectique
TINI	MAX542		Audio.gnd 1V (resistor) Audio max542.2
J21.5	11
J21.7	07

J27.1	+5V
J27.2	08
J27.3	10
J27.4	GND


APPLI=HTTPAudioServer
JDK_HOME=/usr/local/jdk1.5.0_01/
TINI_HOME=/home/raph/TP_TINI/tini1.15/

${JDK_HOME}/bin/javac -source 1.2 -target 1.1 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar *.java 

${TINI_HOME}/native/bin/Linux/macro -I${TINI_HOME}/native/lib/ audioplayer.a51
${TINI_HOME}/native/bin/Linux/a390 -f 1.15 -p 400 -l audioplayer.mpp

${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency -add HTTPSERVER \
-x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar -d ${TINI_HOME}/bin/tini.db \
-f HTTPAudioServer.class -f PlayPostedAudio.class \
-n audioplayer.tlib -d ${TINI_HOME}/bin/tini.db -o ${APPLI}.tini 

*/


// Imports
import com.dalsemi.tininet.http.HTTPServer;
import com.dalsemi.tininet.http.HTTPServerException;
import java.io.*;

public class HTTPAudioServer
{
    static Object lock;

    /**
     * Main method of our web server
     */
    public static void main(String[] args)
    {
        // Create a new HTTPServer on port 80
        HTTPServer httpd = new HTTPServer(80);
        // Set /html as the default directory for HTML files
        httpd.setHTTPRoot( "/html" );
        // Set index.html as the index page, sent after a GET request
        httpd.setIndexPage( "index.html" );
        // Disable loggin for our HTTP Server
        httpd.setLogging( false );

        try
        {
            System.loadLibrary("audioplayer.tlib");
        }
        catch(Exception e)
        {
            System.out.println("Error: Could not load the library: "+e);
            return;
        }

        // Tell user that we are ready to go
        System.out.println("TINI server now servicing Requests...");

        // Handle HTTP Connections forever
        while (true)
        {
            try
            {
                httpd.serviceRequests(); 
            }
            catch ( HTTPServerException e )
            { 
                System.out.println( "Error occured while servicing requests: " + e.getMessage() );
            }
        }
    }

}
