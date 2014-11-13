/* stepper lmd18200
 * DC motor
 *
 * lun mar  7 08:58:54 UTC 2005 RP
 *	- PWM = P1.7 (J5.1)
 *	
APPLI=Lmd18200
JDK_HOME=/usr/local/jdk1.5.0_01/
TINI_HOME=/home/raph/TP_TINI/tini1.15/

${JDK_HOME}/bin/javac -target 1.1 -source 1.2 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
*/


import com.dalsemi.system.*;

class Lmd18200 implements Runnable {

BitPort bp;
int val1=50,val2=50;


Lmd18200()
{
bp = new BitPort(BitPort.Port1Bit7);
new Thread(this).start();

byte[]buffer=new byte[10];
int n;
com.dalsemi.shell.server.SystemInputStream is =
		(com.dalsemi.shell.server.SystemInputStream) System.in;
	
while(true)
try {
	
	System.out.println("enter PWM high : ");
	n=System.in.read(buffer);	
	val1=Integer.parseInt(new String(buffer,0,n-1));
	System.out.println("enter PWM low : ");
	n=System.in.read(buffer);	
	val2=Integer.parseInt(new String(buffer,0,n-1));
	
}
catch(Exception e){System.out.println(e);}
}


public void run()
{
try{
while(true) {
	bp.clear();
	Thread.sleep(val1);
	bp.set();
	Thread.sleep(val2);
      }
     }
catch(Exception e){}
}


public static void main(String[] args) {
System.out.println("DC motor controller LMD18200 test V0.0.0");
new Lmd18200();     
}

}
