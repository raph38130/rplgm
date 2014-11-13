/* encoder module HEDS 5540
 *
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



APPLI=CountT2
JDK_HOME=/usr/local/jdk1.5.0_02
TINI_HOME=/home/raph/TP_TINI/tini1.15/

${JDK_HOME}/bin/javac -source 1.2 -target 1.1 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\

${TINI_HOME}/native/bin/Linux/macro -I${TINI_HOME}/native/lib/ timerT2.a51
${TINI_HOME}/native/bin/Linux/a390 -f 1.15 -p 400 -l timerT2.mpp


${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar TINIConvertor \
-f ${APPLI}.class -n timerT2.tlib -o ${APPLI}.tini -d ${TINI_HOME}/bin/tini.db

*/


import com.dalsemi.system.*;

class CountT2 {
int[]TAB;
CountT2()
{
      TAB=new int[500];
      startCountT2();
      //
      //while(true)       	System.out.println("speed= "+speed()+" rpm "+ (readT2()-32768));
      //System.out.println("count "+ (readT2()-32768));
      System.out.println("go !!!");
      for (int i=0; i != 500; i++) {
         TAB[i]=readT2()-32768;
	 try {Thread.sleep(20);} catch (Exception e) {}
	}
      System.out.println("done...");
      for (int i=0; i != 500; i++) {
      	System.out.print(TAB[i]+",");
	//if((i % 20)==0) System.out.println();
	}
      //try {System.in.read();} catch (Exception e) {}
      //try {Thread.sleep(1000);} catch (Exception e) {}
      

}

public static void main(String[] args)
{
      System.out.println("HEDS test with T2 counter V1.1");
      System.loadLibrary("timerT2.tlib");
      new CountT2();
}

//natives methods
public static native void startCountT2();
public static native int readT2();

//rpm
int speed()
{
int n=readT2();	
try {Thread.sleep(100);} catch (Exception e) {}
return ((readT2() - n)*600)/512; 
}



}
