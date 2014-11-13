/*
 * 
 * 
APPLI=Clignotant
JDK_HOME=/home/apsii/jdk1.5.0_05/
TINI_HOME=/home/apsii/TP_TINI/tini1.17/

${JDK_HOME}/bin/java -cp ${TINI_HOME}/bin/tini.jar TINIConvertor \
-f ${APPLI}.class -o ${APPLI}.tini \
-d ${TINI_HOME}/bin/tini.db


 * 
 */


package tini;

import com.dalsemi.system.BitPort;

public class Clignotant {

	public static void main(String[] args) throws InterruptedException {
		System.out.println("clignotant v0.0");
		BitPort p=new BitPort(BitPort.Port5Bit2);
		while(true) {
			p.set();
			Thread.sleep(250);
			p.clear();		
			Thread.sleep(250);
		}
	}

}
