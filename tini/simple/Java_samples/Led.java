/* Mon Jan 12 11:12:47 CET 2004 RP
 *
 *
 *
/home/raph/j2sdk_nb/j2sdk1.4.2/bin/javac -target 1.1 -bootclasspath ../tini1.12/bin/tiniclasses.jar Led.java
/home/raph/j2sdk_nb/j2sdk1.4.2/bin/java -classpath ../tini1.12/bin/tini.jar TINIConvertor -d ../tini1.12/bin/tini.db -f Led.class -o Led.tini 
 *
 */


import com.dalsemi.system.BitPort;
import com.dalsemi.system.TINIOS;

class Led {
   public static void main(String[] args) {
      BitPort bp;

      System.out.println("Led V0.0.0");
      bp = new BitPort(BitPort.Port5Bit2);
      for (;;) {
         // Turn on LED
         bp.clear();
         // Leave it on for 1/4 second
         try {
            Thread.sleep(50);
         } catch (InterruptedException ie) {}
         // Turn off LED
         bp.set();
         // Leave it off for 1/4 second
         try {
            Thread.sleep(50);
         } catch (InterruptedException ie) {}
      }
   }
}
