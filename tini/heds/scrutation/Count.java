/* encoder module HEDS 5540
 *
 * mer jan  5 18:05:31 UTC 2005 RP
 *	- read A via P1.0
APPLI=Count
JDK_HOME=/home/raph/jdk1.5.0/
TINI_HOME=/home/raph/TP_TINI/tini1.14/
${JDK_HOME}/bin/javac -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
*/


import com.dalsemi.system.BitPort;
import com.dalsemi.system.TINIOS;

class Count {
   public static void main(String[] args) {
      BitPort bpa,bpled;

      System.out.println("HEDS test V0.0.0");
      bpa = new BitPort(BitPort.Port1Bit0);
      bpled = new BitPort(BitPort.Port5Bit2);
      for (;;) {
         if(bpa.read()==0) bpled.clear();
	 else bpled.set();
      }
}
}
