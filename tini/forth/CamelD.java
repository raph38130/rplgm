/*
 * tini/camelforth dowloader 
 * choose :
 * 	filename
 *  	bank number
 *  	exit timeout
 */

package fr.lgm;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import java.io.FileReader;

public class CamelD implements SerialPortEventListener {

FileReader f;
SerialPort port=null;
boolean xon;
int sleeptime = 5;

@Override
public void serialEvent(SerialPortEvent arg0) {
char c;
if (arg0.getEventType() != SerialPortEvent.DATA_AVAILABLE)
	{System.out.print("bad data");return;}
try {
while (port.getInputStream().available() > 0) {
	c = (char) port.getInputStream().read();
	if (c == 17 || c == 0xd)
		xon = true;
	if (c != 17)
	System.out.print(c);
	}
} catch (Exception e) {
	e.printStackTrace();
}
}
void download() {
char c;
int sleeptime = 5;
try {
while ((c = (char) f.read()) != (char) -1) {
	while (!xon) {
		Thread.sleep(sleeptime);
	}
	xon = false;
	if (c == 0xa || c == 0xd) {
		port.getOutputStream().write(0xd);
	} else {
		port.getOutputStream().write(c);
	}
	}
Thread.sleep(20000);
System.out.println("eof");
port.close();
} catch (Exception e) {
	e.printStackTrace();
}
}

CamelD(){
try {
xon=true;
f= new FileReader("/home/raph/TP_TINI/forth/demo2.fth");

port=(SerialPort)CommPortIdentifier.getPortIdentifier("/dev/ttyUSB0").open("CamelD",1000);  
port.setSerialPortParams(115200,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
port.setOutputBufferSize(1);
port.setInputBufferSize(1);
System.out.println("serial port "+port+" "+port.getBaudRate());
port.notifyOnOverrunError(true);
port.setDTR(true);
Thread.sleep(100);
port.setDTR(false);
Thread.sleep(250);			
port.addEventListener(this);
port.notifyOnDataAvailable(true);	
port.getOutputStream().write(0xd);
Thread.sleep(50);
port.getOutputStream().write('B');
Thread.sleep(10);
port.getOutputStream().write('1');
Thread.sleep(10);
port.getOutputStream().write('2');
Thread.sleep(10);
port.getOutputStream().write(0xd);
Thread.sleep(10);
port.getOutputStream().write('G');
Thread.sleep(10);
port.getOutputStream().write(0xd);
Thread.sleep(100);

download();
} catch (Exception e) {
	e.printStackTrace();
} 
}

public static void main(String[] args) {
System.out.print("CamelD v1.1");
new CamelD();
}
}

