/* 
 * Wed Jan 21 18:47:10 CET 2004 RP
 *	- initial rev
 *
APPLI=i2cEval
JDK_HOME=/home/raph/j2sdk1.5.0/
TINI_HOME=/home/raph/TP_TINI/tini1.13/
${JDK_HOME}/bin/javac -classpath \
${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency \
-add I2C400 -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
-d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini
 *
 */
import com.dalsemi.system.*;

/**
 * Simple test of the I2C :  blink leds on Philipps OM5027 evaluation Board
 */
public class i2cEval
{
    static void main(String[] args)
    {
    byte[] b = new byte[3];
    int len,i=0;
    boolean v=true;
    
    System.out.println("i2ctest V1.0 RP/LGM\n2 leds are blinking on Philips eval.board");
    try {
    I2CPort port = new I2CPort();
    port.setClockDelay((byte)2);
    port.setAddress((byte)(0x7E >> 1)); 	//PCF 8574A 8xI/O 	OK

    while (true) {
    	if (v) b[0] = (byte)0x00; //inst leds ON
    	else   b[0] = (byte)0xc0; //inst leds OFF
    	v= !v;
    	len=port.write(b,0,1);
    	try {Thread.sleep(500);}catch(Exception e){}
    	}
    }
    catch (IllegalAddressException e){System.out.println("Illegal Address on Memory mapped I2C"+e);}
    }
}
