/*
 * mer mar 22 14:24:09 CET 2006 RP
 * 	- activation par le serveur web du saa1064
 * 
 * Sun Jun 27 19:23:42 CEST 2004 RP
 *	- OOP rewriting
 *
 * Thu Jun 24 00:31:21 CEST 2004 RP
 * 	- add port.setClockDelay((byte) 2) OK
 *	- remove pullup to 5V !!!
 *
 * Wed Jan 21 18:47:10 CET 2004 RP 
 * 	- initial rev : 7 seg. led display NOT OK
 *
APPLI=WebSaa
JDK_HOME=/home/apsii/jdk1.5.0_05
TINI_HOME=/home/apsii/TP_TINI/tini1.17/

${JDK_HOME}/bin/javac -source 1.2 -target 1.1 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar *.java ;

${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-add I2C400 -add HTTPSERVER -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f Active7Seg.class -f Web1064.class -o ${APPLI}.tini
 *  
 */
import com.dalsemi.system.*;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Vector;

import com.dalsemi.tininet.http.HTTPWorker;
import com.dalsemi.tininet.http.PostElement;
import com.dalsemi.tininet.http.PostScript;


public class Active7Seg implements PostScript 
{

public void handlePost(Vector arg0, OutputStream arg1, HTTPWorker arg2) throws IOException
{
	byte[] b = new byte[9];
	I2CPort port;
	PostElement pe;
	
	try {
	port = new I2CPort();
	port.setAddress((byte) (0x72 >> 1)); //jumper OFF OFF OFF
	port.setClockDelay((byte) 2); 
	b[0] = (byte) 0x00; 				//instruction register
	b[1] = (byte) 0x27;					//dynamic all banks 12mA
	System.out.println("recu "+arg0.size()+" params");
	for (int i=0; i< arg0.size(); i++) {
		pe = (PostElement)arg0.elementAt(i);
		System.out.println(pe.value);
		b[2+i]=(byte)Integer.parseInt(pe.value,2);
	}
	
	port.write(b, 0, 6);
		}
	catch (IllegalAddressException e) {
		System.out.println("Illegal Address on Memory mapped I2C" + e);}
}
}







