/* stepper UCN5804
 * DC motor
 * mar mar 29 16:21:41 UTC 2005 RP
 *	- PWM P1.7 (J5.1)
 *	- CCW P5.3 (J3.1)
 * lun mar  7 08:58:54 UTC 2005 RP
 *	- PWM = P1.7 (J5.1)
 *	
 APPLI=Ucn5804
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

class Ucn5804 {

//natives methods from timer.tlib
//for HEDS quadarture encoder
public static native void startCountT2();
public static native int readT2();
//stepper arg : b15-b8 steps number b7-b0 delay (must be > 10)
public static native void stepper(int n);

	Ucn5804() {
	BitPort b;
		int val=0;
		b = new BitPort(BitPort.Port5Bit3);
		int c;
		while (true)
			try { 
				System.out.println("enter Ss +- Cc g : steps "+val/256 +" delay "+val%256);
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
				
			} catch (Exception e) {
				System.out.println(e);
			}
	}
	

	public static void main(String[] args) {
		System.out.println("DC motor controller UCN5804 test V1.6");
     		System.loadLibrary("timer.tlib");
		new Ucn5804();
	}

}
