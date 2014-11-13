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
APPLI=WebMotor
JDK_HOME=/home/apsii/jdk1.5.0_05
TINI_HOME=/home/apsii/TP_TINI/tini1.17/

${JDK_HOME}/bin/javac -source 1.2 -target 1.1 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar *.java ;

${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-add HTTPSERVER -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f Active.class -f Timer2.class -f WebMotor.class -n timerT2.tlib -o ${APPLI}.tini
 *  
 */
import com.dalsemi.system.*;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Vector;

import com.dalsemi.tininet.http.HTTPWorker;
import com.dalsemi.tininet.http.PostElement;
import com.dalsemi.tininet.http.PostScript;


public class Active implements PostScript 
{

public void handlePost(Vector arg0, OutputStream arg1, HTTPWorker arg2) throws IOException
{
	
	
	PostElement pe;
						
	for (int i=0; i< arg0.size(); i++) {
		pe = (PostElement)arg0.elementAt(i);
		//System.out.println(pe.field+" "+pe.value);
		
		if (arg0.size()==1)
			new BitPort(BitPort.Port5Bit3).clear();
		else    new BitPort(BitPort.Port5Bit3).set();
		
		if (pe.field.equals("rc")) {
			int val=Integer.parseInt(pe.value);
			if (val==0)
				Timer2.stopT2();
			else {
				int n1=(val * 255) / 100;
				int n2=255-n1;
				Timer2.startpwmT2( (n2<<8) + n1);
				}
			}
		}
		
}
}







