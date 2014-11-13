/* stepper mc3479
 *
 * mar jan 11 19:04:07 UTC 2005 RP
 *	- CLK = P1.7 (J5.1)
 *	- CCW = P5.3 (J3.1)
APPLI=Stepper
JDK_HOME=/home/raph/jdk1.5.0/
TINI_HOME=/home/raph/TP_TINI/tini1.15/
${JDK_HOME}/bin/javac -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
*/


import com.dalsemi.system.*;

class Stepper {
   public static void main(String[] args) {
      BitPort bpa,bpled;
      boolean b=true;

      System.out.println("stepper test V0.0.0");
      bpa = new BitPort(BitPort.Port1Bit7);
      bpled = new BitPort(BitPort.Port5Bit2);
      
      for (;;) {
         if(b) {
	 	//bpled.clear();
	 	bpa.clear();
		}
	 else 
	 	{
		//bpled.set();
		bpa.set();
		}
	 b = !b;
      }
}
}
