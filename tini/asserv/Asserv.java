/* Asserv.java 
 *
 *
 * mer mar 30 20:01:32 UTC 2005 RP
 * stepper UCN5804
 *	- PWM P1.7 (J5.1)
 *	- CCW P5.3 (J3.1)
 * HEDS 5540
 *	- A(5)		T2	(J27.2)
 *	- B(7) 		T2EX (J27.3)
 *	- VCC (2) GND (3)
 *	
 APPLI=Asserv
 JDK_HOME=/usr/local/jdk1.5.0_01/
 TINI_HOME=/home/raph/TP_TINI/tini1.15/

${TINI_HOME}/native/bin/Linux/macro -I${TINI_HOME}/native/lib/ timer.a51 ;\
${TINI_HOME}/native/bin/Linux/a390 -f 1.15 -p 400 -l timer.mpp ;\
 ${JDK_HOME}/bin/javac -target 1.1 -source 1.2 -classpath \
 ${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
 ${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
 -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
 -d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -n timer.tlib -o ${APPLI}.tini
 */

import com.dalsemi.system.*;

class Asserv {

//natives methods from timer.tlib
//for HEDS quadarture encoder
public static native void startCountT2();
public static native int readT2();
//stepper arg : b15-b8 steps number b7-b0 delay (must be > 10)
public static native void stepper(int n);
int     p=0,
	i=0,
	d=0,
	cold=0,cnew=0,delay=0;
BitPort b;
Asserv() {
	b = new BitPort(BitPort.Port5Bit3);
	startCountT2();
	}
//stepper motor follow counter
void follow()
{
while(true) {
	cnew=readT2()-32768;
	if (cnew!=cold) {
		if (cnew>cold) 
		      b.set();
		else  b.clear(); 
		p = Math.abs(cnew);// - cold);
		//i += cnew;
		//d =;
		//System.out.println("p "+p+" i "+i);
		stepper((p << 8) | 8);
		//cold=cnew;
		}
}	
	
/*	
	while (true)
	try { 

		System.out.println("enter Ss +- Cc g : steps "+val/256 +" delay "+val%256+" codeur "+(readT2()-32768));
		c = System.in.read();
		System.in.read();
		if (c=='+')
			val++;
		if (c=='-')
			val--;
		if (c=='S')
			val += 256;
		if (c=='s')
			val -=256;
		if (c=='C')
			b.set();
		if (c=='c')
			b.clear();
		if (c=='g')
			stepper(val);			
	} catch (Exception e) {System.out.println(e);}
*/
}
	

public static void main(String[] args) {
	System.out.println("Asserv test V1.2");
	System.loadLibrary("timer.tlib");
	new Asserv().follow();
}

}
