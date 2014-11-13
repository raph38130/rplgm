/* stepper mc3479
 *
 * mar jan 11 19:04:07 UTC 2005 RP
 *	- CLK = P1.7 (J5.1)
 *	- CCW = P5.3 (J3.1)
APPLI=StepperCW
JDK_HOME=/home/raph/jdk1.5.0/
TINI_HOME=/home/raph/TP_TINI/tini1.15/
${JDK_HOME}/bin/javac -target 1.1 -source 1.2 -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
*/


import com.dalsemi.system.*;

class StepperCW implements Runnable {

BitPort bpa,bpb,bpled;
int delay;

StepperCW()
{
bpa = new BitPort(BitPort.Port1Bit7);
bpb = new BitPort(BitPort.Port5Bit3);
bpled = new BitPort(BitPort.Port5Bit2);
delay=0;
new Thread(this).start();
byte[]buffer=new byte[10];
int n;
while(true)
try {
	
	System.out.println("speed (-100,+100) :");
	n=System.in.read(buffer);
	
	System.out.println("lu "+n+ " car " 
		+(int)buffer[0]+" "
		+(int)buffer[1]+" "
		+(int)buffer[2]+" "
		+(int)buffer[3]+" "
		+(int)buffer[4]+" "
		);
	int val=Integer.parseInt(new String(buffer,0,n-1));
	if (val>0) 
		{
		bpb.set();
		delay=val;
		}
	else    {
		bpb.clear();
		delay = -val;
		}
	
}
catch(Exception e){System.out.println(e);}
}


public void run()
{
boolean b=true;
try{
while(true) {
         if(b) {
	 	//bpled.clear();
	 	bpa.clear();
		Thread.sleep(delay);
		}
	 else 
	 	{
		//bpled.set();
		bpa.set();
		Thread.sleep(delay);
		}
	 b = !b;
      }
     }
catch(Exception e){}
}


public static void main(String[] args) {

System.out.println("stepper test V0.0.0");
new StepperCW();     
}

}
