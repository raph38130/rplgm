/* HTTP spy between port n°8000 & 80
 *
 * Thu Dec 19 22:53:19 CET 2002 RP
 *	- initial revision
 *	threading is mandatory : we can not know when a request is at its end
 */
import javax.swing.*;
import java.awt.*;

class HTTPGui extends Thread
{
java.net.Socket s1,s2;
JTextArea t1,t2;
Thread thread;

HTTPGui(int inport,String host,int outport)
{
//build GUI
JFrame f=new JFrame("HTTPGui V0.0.0");
f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
f.setSize(900,500);

t1 = new JTextArea("in",80,40);
t2 = new JTextArea("out",80,40);

f.getContentPane().add(new JScrollPane(t1),BorderLayout.WEST);
f.getContentPane().add(new JScrollPane(t2),BorderLayout.EAST);
f.setVisible(true);

//open incoming & outcoming sockets
try {
s1 = new java.net.ServerSocket(inport).accept();
s2 = new java.net.Socket(host,outport);
}
catch(java.io.IOException e1) {System.out.println("erreur 1 " +e1);}

//start Thread
start();

//transfer data s1 -> s2
byte[] buffer = new byte[25];
boolean stop=false;
int n=0;

while(!stop) {
	try {
		n=s1.getInputStream().read(buffer);
		if (n>0) t1.append(new String(buffer,0,n));
	}
	catch(java.io.IOException e2) {System.out.println("erreur 2 "+e2);stop=true;}	
	try {
		if (n>0) s2.getOutputStream().write(buffer,0,n);
		s2.getOutputStream().flush();
	}
	catch(java.io.IOException e3) {System.out.println("erreur 3 " +e3);stop=true;}	
	}
System.out.println("you need to restart app...");
}

/*
 * thread code
 * transfer data s2 -> s1
 */

public void run()
{
System.out.println("running");
byte[] buffer = new byte[25];
boolean stop=false;
int n=0;

while(!stop) {
	try {
		n=s2.getInputStream().read(buffer);
		if (n>0) t2.append(new String(buffer,0,n));
	}
	catch(java.io.IOException e1) {System.out.println("erreur 11 "+e1);stop=true;}
	try {
	if (n>0) s1.getOutputStream().write(buffer,0,n);
	s1.getOutputStream().flush();
	}
	catch(java.io.IOException e2) {System.out.println("erreur 12 " +e2);stop=true;}	
	}
System.out.println("you need to restart app...");
}

public static void main(String[] s)
{
String host=null;
int in=0,out=0;

//parse command line
if (s.length == 0) {
	host="localhost";
	in=8000;out=80;
	}
else if (s.length == 3){
	host=s[1];
	in=new Integer(s[0]).intValue();out=new Integer(s[2]).intValue();
	}
else {
	System.out.println("usage: HTTPGui in host port (8000 locahost 80 default)");
	System.exit(0);
	}
System.out.println("starting HTTPGui... listenning on port "+in + " to "+host+":"+out);

new HTTPGui(in,host,out);
}
}
