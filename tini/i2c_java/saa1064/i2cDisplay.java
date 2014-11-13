/*
 * Thu Jun 24 00:31:21 CEST 2004 RP
 * 	- add port.setClockDelay((byte) 2) OK
 *	- remove pullup to 5V !!!
 *
 * Wed Jan 21 18:47:10 CET 2004 RP 
 * 	- initial rev : 7 seg. led display NOT OK
 *
APPLI=i2cDisplay
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

public class i2cDisplay {


static final byte chiffre_0 = (byte) 0x3F;
static final byte chiffre_1 = (byte) 0x06;
static final byte chiffre_2 = (byte) 0x5b;
static final byte chiffre_3 = (byte) 0x4F;
static final byte chiffre_4 = (byte) 0x66;
static final byte chiffre_5 = (byte) 0x6D;
static final byte chiffre_6 = (byte) 0x7D;
static final byte chiffre_7 = (byte) 0x07;
static final byte chiffre_8 = (byte) 0x7F;
static final byte chiffre_9 = (byte) 0x6F;
static final byte chiffre_a = (byte) 0x77; // a
static final byte chiffre_b = (byte) 0x7C;
static final byte chiffre_c = (byte) 0x39;
static final byte chiffre_d = (byte) 0x5E;
static final byte chiffre_e = (byte) 0x79;
static final byte chiffre_f = (byte) 0x71; // f
static final byte chiffre_p = (byte) 0x80; // .

static final byte[] chiffre = 
	{chiffre_0,chiffre_1,chiffre_2,chiffre_3,chiffre_4,chiffre_5,
	chiffre_6,chiffre_7,chiffre_8,chiffre_9,
	chiffre_a,chiffre_b,chiffre_c,chiffre_d,chiffre_e,chiffre_f};
	
static byte[] b = new byte[9];
static I2CPort port;

static void display(int n) throws IllegalAddressException {
		b[5] = chiffre[n % 16]; 		//digit1
		b[4] = chiffre[(n / 16) % 16]; 	//digit2
		b[3] = chiffre[(n / 256) % 16]; 	//digit3
		b[2] = chiffre[(n / 4096) % 16]; 	//digit4
		port.write(b, 0, 6);
}

public static void main(String[] args) {
		System.out.println("i2c 7 segment display V0.9.99 ( SAA1064 )");
		try {
			port = new I2CPort();
			port.setAddress((byte) (0x72 >> 1)); //jumper OFF OFF OFF
			port.setClockDelay((byte) 2); 
			b[0] = (byte) 0x00; 	//instruction register
			b[1] = (byte) 0x27;	//dynamic all banks 12mA
			int i=0; 
			while (true) {
				display(i++);
				//System.out.println(i);
				try {
					Thread.sleep(500);
				} catch (InterruptedException e) {
				}
			}
		} catch (IllegalAddressException e) {
			System.out.println("Illegal Address on Memory mapped I2C" + e);
		}
	}
}