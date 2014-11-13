
/*---------------------------------------------------------------------------
 * Wed Jun 16 20:45:52 CEST 2004 RP
 *	- APSII5 evaluation
 *
 * Wed Jan 28 21:04:50 CET 2004 RP/LGM
 *	-initial revision

 *---------------------------------------------------------------------------

JDK=/home/raph/j2sdk1.5.0/
TINI=/home/raph/TP_TINI/tini1.12/
$JDK/bin/javac -classpath ${TINI}/bin/modules.jar:${TINI}/bin/tiniclasses.jar *.java ;\
$JDK/bin/java -classpath ${TINI}/bin/tini.jar BuildDependency -add I2C400 -add HTTPSERVER -x $TINI/bin/owapi_dep.txt -p $TINI/bin/modules.jar -d $TINI/bin/tini.db -f PostHandler.class -f i2cledWebServer.class -o HttpServer.tini

 */

import java.io.*;
import java.util.*;
import com.dalsemi.tininet.http.*;
import com.dalsemi.system.*;


public class PostHandler implements PostScript
{
    /**
     * This method handles all responses to the the HTTP POST
     *
     * @param data - a Vector of PostElements holding all supplied fields and Values in pairs
     * @param out - an Output Stream to the client socket
     * @param work - HTTPWorker contains utility functions used to URL encode our response
     * @throws IOException if an IO error occurs     
     */

    public void handlePost( Vector data, OutputStream out, HTTPWorker work ) throws IOException
    {
    // Inform user that we are handling the POST request
        System.out.println("Handling POST Request");

	StringBuffer strBuff;

        PostElement cmd  = (PostElement)data.lastElement();
    // Write out the HTML headers and basic title text here
	strBuff = new StringBuffer ( "<HTML><title>TINI I2C TEST</title>\r\n"
                  + "<BODY><CENTER><H1>Philips OM5027 I2C evaluation Board</H1>\r\n"
                   );
		   
	//strBuff.append("cmd "+cmd.value);
	if (cmd.value.equals("update leds")) { 
 	try {           
 		byte[] buffer = new byte[5];
 		int len;
 		I2CPort port = new I2CPort();
 		port.setClockDelay((byte)2);
 		port.setAddress((byte)(0x7E >> 1)); 
 		len=port.read(buffer,0,1);
		//strBuff.append("read"+buffer[0]+"<br>");
		if (data.size()==1) {
			//strBuff.append("clear both led");
			buffer[0] |= 0xc0; 
			}
		else
		if (data.size()==3) {
			//strBuff.append("set both led");
			buffer[0] &= 0x3F;
			}
		else
		if (((PostElement)data.firstElement()).field.equals("led1")) {
			//strBuff.append("set Led1 clear Led2");
			buffer[0] |= 0x40; 
			buffer[0] &= 0x7F;
			}
		else {
			//strBuff.append("clear Led1 set Led2");
			buffer[0] |= 0x80; 
			buffer[0] &= 0xbF;
		}
    
 		len=port.write(buffer,0,1);
 		}
 	catch (IllegalAddressException e){strBuff.append("Illegal Address on Memory mapped I2C"+e);}
	} else {
	strBuff.append("command not yet implemented !!!");
	}
    // Create HTML footers
        strBuff.append( "<A HREF=http://172.16.0.3/ > back </a></CENTER></BODY></HTML>" );

    // Use HTTPWorker's encode body function to generate the URL encoded response
 		out.write( work.encodeBody( HTTPServer.HTTP_OK, "OK", HTTPServer.MIME_TEXT_HTML, strBuff.length() ) );

    // Send out the bytes of our response page
   		out.write( strBuff.toString().getBytes() );

   	// Ensure that our data is written imediatly
   		out.flush();

  	// Inform user that we are done handling the POST request
  	    System.out.println("Finished handling POST request");
   	}
}
