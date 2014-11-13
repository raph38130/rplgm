/* $Id: SoapTangoClient.java,v 1.2 2002/05/15 14:43:21 raph Exp $
 * ESRF/LGM project
 *
 * a SOAP gui client for the "ESRF Control System TANGO"
 *
 * $History$
 *
 *
 * Thu Nov  1 19:14:30 CET 200
 *	- no more hang with j2sdk1.4.0
 *	- soap-2_2 implements session : OK scope="Session" in DeploymentDescriptor.xml
 *
 * RP Sat Sep 15 20:05:06 CEST 2001 (back to work)
 *	- j2sdk1.4.0 VM hang soap when runing in server mode (client OK)
 *	- no more need of xerces (jaxp is a builtin of j2sdk1.4)
 *
 * RP Tue May 22 10:40:12 CEST 2001
 *	- simple type, array serialization
 *	- bean serialization
 *	- current xml-soap release does not handle SOAPSMTPConnection
 *
 * RP Wed Mar 14 12:34:09 CET 2001
 *	- SSL java.net.SocketException: SSL implementation not available
 *
 * RP Wed Feb 14 11:51:20 CET 2001
 *	- added command_list_query
 *
 * RP Thu Feb  8 19:19:04 CET 2001
 *	- better gui,
 *	- soap2.1 : bug with SMTP transport but the same code is ok with 2.0
 *		error is : 	java.lang.NullPointerException
 *      			at org.apache.soap.rpc.Call.getEnvelopeString(Call.java:163)
 *
 * RP Sun Jan 28 19:26:46 CET 2001
 *	- version SMTP
 *	- pas arrivé à ajuster polling time : POP est arreté par inetd
 *
 * RP Tue Jan 16 10:16:18 CET 2001
 *	- GUI
 *
 * RP Tue Dec 12 09:58:35 CET 2000
 *	- initial work (serialization bug correct by Constants.NS_URI_SOAP_ENC)
 *	
 * USAGE :
 * export CLASSPATH=.:/opt2/soap-2_2/lib/soap.jar:/opt2/soap-2_2/lib/mail.jar:/opt2/soap-2_2/lib/activation.jar:../TANGO/java/Tango.jar
 */
 
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

import java.util.*;
import java.net.*;
import org.apache.soap.*;

import org.apache.soap.encoding.soapenc.*;
import org.apache.soap.rpc.*;

public class SoapTangoClient
	extends JFrame 
	implements ActionListener
{
/*gui*/
JButton b1,b3,b4,b5;//b2
JTextArea t1;
JTextField tf1,tf2,tf3;//tf4;
JComboBox cb1;
JLabel l1,l2,l3,l4;

/*tango*/
String[] cil; //CmdInfoList

SoapTangoClient()
{
super("Soap client for TANGO Device Server $Id: SoapTangoClient.java,v 1.2 2002/05/15 14:43:21 raph Exp $");

b1=new JButton("command_list_query");
b3=new JButton("clear");
b4=new JButton("Execute");
l1=new JLabel("in arg");
l2=new JLabel("out arg");
l3=new JLabel("type");
l4=new JLabel("type");
String[]s={"cmds"};
cb1=new JComboBox(s);
t1=new JTextArea("ouput area",15,30);	
tf1=new JTextField("lgm/horloge/serv");
tf2=new JTextField("10");
tf3=new JTextField("http://localhost:8080/soap/servlet/rpcrouter");

b3.addActionListener(this);
b4.addActionListener(this);
cb1.addActionListener(this);
b1.addActionListener(this);

setSize(700,600);getContentPane().setLayout(null);
tf1.setBounds(10,50,150,30);
tf3.setBounds(200,50,300,30);
b1.setBounds(10,100,180,20);	cb1.setBounds(250,100,200,30);
		l1.setBounds(470,100,200,30); l2.setBounds(600,100,200,30);
		l3.setBounds(470,130,200,30); l4.setBounds(600,130,200,30);
		b4.setBounds(250,160,150,30);	tf2.setBounds(470,160,200,30);
		t1.setBounds(200,250,350,200);
			b3.setBounds(300,460,120,20);
getContentPane().add(b1);getContentPane().add(b3);getContentPane().add(b4);
getContentPane().add(tf1);getContentPane().add(tf2);getContentPane().add(tf3);
getContentPane().add(t1);getContentPane().add(cb1);
getContentPane().add(l1);getContentPane().add(l2);getContentPane().add(l3);getContentPane().add(l4);

setDefaultCloseOperation(EXIT_ON_CLOSE);
show();
}

public void actionPerformed(ActionEvent ev)
{
if (ev.getActionCommand()=="clear"){
	t1.setForeground(Color.black);
	t1.setText("");
	return;
	}
	
Call call = new Call();
call.setTargetObjectURI("urn:Soap2TangoProxy");
call.setEncodingStyleURI(Constants.NS_URI_SOAP_ENC);
Vector params = new Vector();
params.addElement(new Parameter("devname", String.class, tf1.getText(), Constants.NS_URI_SOAP_ENC));
Response resp=null;

try {
if (ev.getActionCommand()=="Execute"){
	call.setMethodName("command_inout");
	params.addElement(new Parameter("cmd", String.class, cb1.getSelectedItem(), Constants.NS_URI_SOAP_ENC));
	params.addElement(new Parameter("args", String.class, tf2.getText(), Constants.NS_URI_SOAP_ENC));
	call.setParams(params);
      	resp = call.invoke(new URL(tf3.getText()), "");
	if (!resp.generatedFault()) {
		//display return values
     	 	Parameter ret = resp.getReturnValue();
     	 	String s = (String)ret.getValue();
	 	t1.setText(s);	
      		return;
		}
	}
	
if (ev.getActionCommand()=="command_list_query") {
	cb1.removeAllItems();
	
	call.setMethodName("command_list_query");
	call.setParams(params);
      	resp = call.invoke(new URL(tf3.getText()), "");
	if (!resp.generatedFault()) {
		//build choice bar
     	 	Parameter ret = resp.getReturnValue();
     	 	cil = (String[])ret.getValue();
	 	for (int i=0;i<cil.length;i+=5)
			cb1.addItem(cil[i]);
		return;
		}
	}
} catch (SOAPException e3)
    	    {
      	    t1.setForeground(Color.red);
	    t1.setText("request error"+e3.getFaultCode()+"):"+e3.getMessage());
      	    return;
    	    }
  catch (java.io.IOException e4)
    	    {
      	    t1.setForeground(Color.red);
	    t1.setText("request error"+e4);
      	    return;
    	    }

if (ev.getActionCommand()=="comboBoxChanged"){
	int index=((JComboBox)ev.getSource()).getSelectedIndex();
	l1.setText(cil[5*index+1]);
	l2.setText(cil[5*index+2]);
	l3.setText(cil[5*index+3]);
	l4.setText(cil[5*index+4]);
	return;
	}	
Fault fault = resp.getFault();
t1.setForeground(Color.red);
t1.setText("server fault: \nFault Code   = " + fault.getFaultCode()+"\n  Fault String = " + fault.getFaultString());
       
}

public static void main(String[] args) throws Exception 
{
	new SoapTangoClient();
}

}

