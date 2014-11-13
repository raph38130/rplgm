/* Timer2.java native class for T2 Timer
 *
 * Wed May 10 16:11:44 CEST 2006 RP
 *	- pwm
 * ven jan 21 11:48:43 UTC 2005 RP
 *	- read A via T2
 
connectics 
J27.2	P1.0	  T2
J27.3	P1.1  	  T2EX

registers
Timer/Counter Mode Selection and control T2MOD, T2CON
count Registers TH2, TL2 
16-Bit reload capture RCAP2H, RCAP2L 
Timer Input Clock-Select Bit CKCON.5 
Divide-by-13 Clock-Option Bit T2MOD.3 



APPLI=Timer2
JDK_HOME=/home/raph/jdk1.5.0_05/
TINI_HOME=/cdrom/COURS/TP/TP_TINI/tini1.17/

${JDK_HOME}/bin/javac -source 1.2 -target 1.1 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${TINI_HOME}/native/bin/Linux/macro -I${TINI_HOME}/native/lib/ timerT2.a51 ;\
${TINI_HOME}/native/bin/Linux/a390 -f 1.17 -p 400 -l timerT2.mpp ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar TINIConvertor \
-f ${APPLI}.class -n timerT2.tlib -o ${APPLI}.tini -d ${TINI_HOME}/bin/tini.db

*/


import com.dalsemi.system.*;

class Timer2 {

//natives methods
public static native void startpwmT2(int n);
//2*8 bits poid faibles/fort pour regler rapport cyclique
//somme égale à FF -> 100Hz
public static native void stopT2();



void test2()
{
System.out.print("start");
startpwmT2(0x40C0);
try {Thread.sleep(10000);} catch (Exception e) {}

startpwmT2(0xC040);
try {Thread.sleep(10000);} catch (Exception e) {}

startpwmT2(0x8080);
try {Thread.sleep(10000);} catch (Exception e) {}

System.out.print("stop");
stopT2();
}

public static void main(String[] args)
{
      System.out.println("T2 test V1.1");
      System.loadLibrary("timerT2.tlib");
      new Timer2().test2();
}

}
