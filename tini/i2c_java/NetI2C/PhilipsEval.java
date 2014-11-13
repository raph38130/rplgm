/* PhilipsEval
 *
 *
 * Tue Apr  6 01:01:32 CEST 2004 RP
 *	- philips evaluation board demo
 *	- use with GUI "I2CLed" (netbeans)
 *
 * Wed Jan 21 18:47:10 CET 2004 RP
 *	- initial rev : 7 seg. led display
APPLI=PhilipsEval
JDK_HOME=/home/raph/j2sdk_nb/j2sdk1.4.2/
TINI_HOME=/home/raph/TP_TINI/tini1.12/
${JDK_HOME}/bin/javac -target 1.1 -classpath ${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar ${APPLI}.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar BuildDependency -add I2C400 -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar -d ${TINI_HOME}/bin/tini.db -f ${APPLI}.class -o ${APPLI}.tini

 */
import com.dalsemi.system.*;

public class PhilipsEval implements Runnabl
private I2CPort port;

PhilipsEval()
{
port = new I2CPort();
port.setClockDelay((byte) 2);
port.setAddress((byte)(0x7E >> 1)); 	//PCF 8574A 8xI/O 	OK
new Thread(this).start();
}

void blink()
{
byte[] b = new byte[10];
try {
while(true) {
    b[0] = (byte)0x00; 		//leds ON
    int len=port.write(b,0,1);
    //if (len>0) System.out.println("__");
    try {Thread.sleep(10000);}catch(Exception e){}
    b[0] = (byte)0xc0; 		//leds OFF
    len=port.write(b,0,1);
    //if (len>0) System.out.println("++");
    try {Thread.sleep(10000);}catch(Exception e){}
    }
    }
    catch (IllegalAddressException e){System.out.println("Illegal Address on Memory mapped I2C"+e);}
}

public void run()
{
int c,len;
byte[] buffer = new byte[10];
try {
java.net.Socket s= new java.net.ServerSocket(3333).accept();
while(true) {
	c=s.getInputStream().read();
	switch(c) {
	     case 0: 	port.read(buffer,0,1);	//0?
	     		buffer[0] &= 0x7F; 		
    	     		len=port.write(buffer,0,1);
			break;
	     case 1: 	port.read(buffer,0,1);	//1?
	     		buffer[0] |= 0x80; 		
    	     		len=port.write(buffer,0,1);
			break;
	     case 2: 	port.read(buffer,0,1);	//?0
	     		buffer[0] &= 0xbF; 		
    	     		len=port.write(buffer,0,1);
			break;
	     case 3: 	port.read(buffer,0,1);	//?1
	     		buffer[0] |= 0x40; 		
    	     		len=port.write(buffer,0,1);
			break;
		  }
	}
    }
    catch (IllegalAddressException e){System.out.println("Illegal Address on Memory mapped I2C"+e);}
    catch (java.io.IOException e){System.out.println("socket erreur"+e);}
}

static void main(String[] args)
{    
    System.out.println("Philips OM5027 I2C evaluation board tests V0.4.0 RP/LGM\n");
    new PhilipsEval().blink();
}

}
