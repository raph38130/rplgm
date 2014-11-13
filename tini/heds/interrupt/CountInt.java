/* encoder module HEDS 5540
 *
 * mer jan  5 18:05:31 UTC 2005 RP
 *	- read A via P1.7 int5
 
APPLI=CountInt
JDK_HOME=/home/raph/jdk1.5.0/
TINI_HOME=/home/raph/TP_TINI/tini1.15/
${JDK_HOME}/bin/javac -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
*/


import com.dalsemi.system.*;

class CountInt implements  ExternalInterruptEventListener {

int n;
CountInt()
{
      n=0;
      try {
      	new ExternalInterrupt(ExternalInterrupt.INT5).addEventListener(this); 
	}
      catch(java.util.TooManyListenersException e1){System.out.println("erreur handler");}
      catch(com.dalsemi.system.ExternalInterruptException e2){System.out.println("erreur handler");}
      while(true)
      {
      System.out.println(n);
      try {Thread.sleep(500);} catch (InterruptedException e) {}
      }
}

public void externalInterruptEvent(ExternalInterruptEvent ev) 
{
	 n++;
}

public static void main(String[] args)
{
      System.out.println("HEDS / Int test V0.0.0");
      new CountInt();
}

}
