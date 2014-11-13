/* $Id$
 * ESRF/LGM project
 *
 * a JaxRPC client for a TANGO (ESRF Control System) Device Server
 *
  * run 
 * export CLASSPATH=.:
 * java 
 */
package lgm;


import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

import javax.xml.rpc.Stub;


public class JAXRPC_Client
	extends JFrame 
	implements ActionListener
{
/*gui*/
JButton b1,b2,b3,b4,b5;
JTextArea t1;
JTextField tf1,tf2;
JComboBox cb1;
JLabel l1,l2,l3,l4;


JAXRPC_Client()
{
super("a GUI for JAXRPC $Id$");

b1=new JButton("myFunc1");
b2=new JButton("commandinout");
b3=new JButton("clear");
b4=new JButton("getservlist");
l1=new JLabel("");
l2=new JLabel("");
l3=new JLabel("");
l4=new JLabel("");
String[]s={""};
cb1=new JComboBox(s);
t1=new JTextArea("ouput area",15,30);	

tf1=new JTextField("123");

tf2=new JTextField("");

b3.addActionListener(this);
b4.addActionListener(this);
cb1.addActionListener(this);
b1.addActionListener(this);
b2.addActionListener(this);

setSize(700,600);getContentPane().setLayout(null);
tf1.setBounds(10,50,150,30);
b1.setBounds(10,100,180,20);	cb1.setBounds(250,100,200,30);
		l1.setBounds(470,100,200,30); l2.setBounds(600,100,200,30);
		l3.setBounds(470,130,200,30); l4.setBounds(600,130,200,30);
		b4.setBounds(250,160,150,30);	tf2.setBounds(470,160,200,30);
		t1.setBounds(200,250,350,200);
			b3.setBounds(300,460,120,20);
			b2.setBounds(450,460,120,20);
getContentPane().add(b1);getContentPane().add(b2);getContentPane().add(b3);getContentPane().add(b4);
getContentPane().add(tf1);getContentPane().add(tf2);
getContentPane().add(t1);getContentPane().add(cb1);
getContentPane().add(l1);getContentPane().add(l2);getContentPane().add(l3);getContentPane().add(l4);

setDefaultCloseOperation(EXIT_ON_CLOSE);
show();
}

private static Stub createProxy() {
        // Note: *_Impl is implementation-specific.
        return (Stub)(new Tango2_Impl().getTango2IFPort());
    }



public void actionPerformed(ActionEvent ev)
{
if (ev.getActionCommand()=="clear"){
	t1.setForeground(Color.black);
	t1.setText("");
	return;
	}
if (ev.getActionCommand()=="myFunc1"){
	try {
            Stub stub = createProxy();
            Tango2IF t = (Tango2IF)stub;
            t1.append(t.myFunc1(Integer.parseInt(tf1.getText())));
            } catch (Exception ex) {
            ex.printStackTrace();
            }
	}
if (ev.getActionCommand()=="getservlist"){
	try {
            Stub stub = createProxy();
            Tango2IF t = (Tango2IF)stub;
            String[]s=t.getserverlist();
	    for(int i =0; i<s.length;i++)
	    	t1.append(s[i]+"\n");
            } catch (Exception ex) {
            ex.printStackTrace();
            }
	}
if (ev.getActionCommand()=="commandinout"){
	try {
            Stub stub = createProxy();
            Tango2IF t = (Tango2IF)stub;
            String[]s=t.commandinout("lgm/screen/1","SetText","HELLO !!!");
	    for(int i =0; i<s.length;i++)
	    	t1.append(s[i]+"\n");
            } catch (Exception ex) {
            ex.printStackTrace();
            }
	}
	
}

public static void main(String[] args) throws Exception 
{
	new JAXRPC_Client();
}

}
