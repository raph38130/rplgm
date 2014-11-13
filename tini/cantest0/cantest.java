/*
 * cantest.java
 * 
 *
 * Wed Mar 31 17:15:31 CEST 2004 RP
 *	- synchro en thread
 *
 * Fri Feb  6 10:13:15 CET 2004 RP
 *
 * IDcan 82C150 fixé à 011 1000 0100 (switch carte : ON 234 OFF 1)
 *			0x384
 * alimentation 9V nécessaire (5V carte Tini insuffisant...)
 *
JDK_HOME=/home/raph/jdk1.5.0_08/
TINI_HOME=/mnt/COURS/TP/TP_TINI/tini1.17/
${JDK_HOME}/bin/javac  -source 1.2 -target 1.1 -classpath ${TINI_HOME}/bin/modules.jar:${TINI_HOME}/bin/tiniclasses.jar cantest.java ;\
${JDK_HOME}/bin/java -classpath ${TINI_HOME}/bin/tini.jar: BuildDependency -add CAN400 -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar -d ${TINI_HOME}/bin/tini.db -f cantest.class -o cantest.tini

 *
 *
 */
import com.dalsemi.comm.*;
import com.dalsemi.system.*;
import com.dalsemi.system.*;

public class cantest implements Runnable
{
    	/* 125Kbit/s with crystal of 18.432MHz */
  	static final int CAN_DIVISOR = 41;
    	static final int CAN_TSEG1 = 3;
    	static final int CAN_TSEG2 = 5;
    	static final int CAN_SJW = 1;
    	static final byte CANBUSNUM = CanBus.CANBUS0;
    
	CanBus a;
        byte[] syn,oe,ledOn,ledOff;

cantest()
{
        syn =    new byte[2];
        oe =     new byte[3];
        ledOn =  new byte[3];
        ledOff = new byte[3];
        syn[0] =   (byte)0xAA;	//sync data for 82C150
        syn[1] =   (byte)0x04;
        oe[0] =    (byte)0x04;	//port direction
        oe[1] =    (byte)0xFF;
        oe[2] =    (byte)0xFF;
	ledOn[0]=  (byte)0x03;
	ledOn[1]=  (byte)0x00;
	ledOn[2]=  (byte)0x00;
	ledOff[0]= (byte)0x03;
	ledOff[1]= (byte)0x20;	//turn on led13
	ledOff[2]= (byte)0x00;
try
{	
        a = new CanBus(CANBUSNUM);
        a.setBaudRatePrescaler(CAN_DIVISOR);
        a.setTSEG1(CAN_TSEG1);
        a.setTSEG2(CAN_TSEG2);
        a.setSynchronizationJumpWidth(CAN_SJW);
        a.enableController();
        a.setMessageCenterTXMode(1);
}
catch (Throwable e) {System.out.println("CAN init Error "+e);}

	new Thread(this).start();		
}

public void run()
{
while(true) 
	try
	{
	a.sendDataFrame(0xaa, false, syn);
	while(true) {
		a.sendDataFrame(0xaa, false, syn);
		try {Thread.sleep(190);}catch(Exception e){}
		}
	}
	catch (Throwable e) {System.out.println("sync fail "+e);}
}

public void sendData()
{
        try
        {
	boolean b=true;
	while (true) {
		try {Thread.sleep(1000);}catch(Exception e){}
		a.sendDataFrame(0x384, false, oe);
		if (b) { 
			a.sendDataFrame(0x384, false, ledOn);
			System.out.println("OFF");
			}
	        else {
			a.sendDataFrame(0x384, false, ledOff);
			System.out.println("ON");
			}
		b = !b;
		}
        }
        catch (Throwable e) {System.out.println("CAN send "+e);}
}


static void main(String args[])
{
        System.out.println("cantest 82C150 V0.2.6");
	new cantest().sendData();        
}
}
