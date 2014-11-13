/* $Id: JClient.java,v 1.5 2002/05/15 14:42:53 raph Exp $
 * ESRF/LGM project
 *
 * a Corba client for a TANGO (ESRF Control System) Device Server
 *
 * Wed May 15 12:16:47 CEST 2002
 *	- swing, add TANGO_HOST in DeviceProxy constructor call
 * Sat Apr 20 21:53:22 CEST 2002 RP
 *	- use TangoApi
 * Mon Apr  1 22:42:00 CEST 2002 Rp
 *	- back to work with TANGO2 and JOB4.1
 * Fri May 18 23:21:34 CEST 2001 RP 
 *	- initial work 
 *	
 * USAGE :
 * populate DataBase
 * 	replace into device (name,class,server,version,exported,ior) values("dserver/horloge/serv","horloge","horloge/serv","TANGO2",0,"eee");
 * 	replace into device (name,class,server,version,exported,ior) values("lgm/horloge/serv","horloge","horloge/serv","TANGO2",0,"eee");
 * start DS
 *	./Horloge serv -v4
 * run JClient
 * export CLASSPATH=.:../../TANGO/java/Tango.jar:../../TANGO/java/OB.jar
 * java JClient
 */

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

import fr.esrf.Tango.*;
import fr.esrf.TangoDs.*;
import fr.esrf.TangoApi.*;

public class JClient
	extends JFrame 
	implements ActionListener
{
/*gui*/
JButton b1,b3,b4,b5;
JTextArea t1;
JTextField tf1,tf2;
JComboBox cb1;
JLabel l1,l2,l3,l4;

/*tango*/
DeviceProxy dev;
//DevCmdInfo[] cil;
CommandInfo[] cil;

JClient()
{
super("a GUI for TANGO Device Server $Id: JClient.java,v 1.5 2002/05/15 14:42:53 raph Exp $");

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
//tf1=new JTextField("lgm/horloge/serv");
tf1=new JTextField("sys/database/1");
tf2=new JTextField("10");

b3.addActionListener(this);
b4.addActionListener(this);
cb1.addActionListener(this);
b1.addActionListener(this);

setSize(700,600);getContentPane().setLayout(null);
tf1.setBounds(10,50,150,30);
b1.setBounds(10,100,180,20);	cb1.setBounds(250,100,200,30);
		l1.setBounds(470,100,200,30); l2.setBounds(600,100,200,30);
		l3.setBounds(470,130,200,30); l4.setBounds(600,130,200,30);
		b4.setBounds(250,160,150,30);	tf2.setBounds(470,160,200,30);
		t1.setBounds(200,250,350,200);
			b3.setBounds(300,460,120,20);
getContentPane().add(b1);getContentPane().add(b3);getContentPane().add(b4);
getContentPane().add(tf1);getContentPane().add(tf2);
getContentPane().add(t1);getContentPane().add(cb1);
getContentPane().add(l1);getContentPane().add(l2);getContentPane().add(l3);getContentPane().add(l4);

setDefaultCloseOperation(EXIT_ON_CLOSE);
show();
}



public void actionPerformed(ActionEvent ev)
{
try {
if (ev.getActionCommand()=="clear"){
	t1.setForeground(Color.black);
	t1.setText("");
	return;
	}
if (ev.getActionCommand()=="Execute"){
	DeviceData argin=new DeviceData();
	if (l3.getText().equals("DevInteger")) argin.insert(new Integer(tf2.getText()).intValue()); 
	if (l3.getText().equals("DevString")) argin.insert(tf2.getText()); 
		
	DeviceData res2=dev.command_inout(cb1.getSelectedItem().toString(),argin);
	t1.append(res2.extractString());
	}
	
if (ev.getActionCommand()=="command_list_query") {
	cb1.removeAllItems();
	//dev= new DeviceProxy(tf1.getText(),"172.16.104.8","32907");
	dev= new DeviceProxy(tf1.getText(),"localhost","30000");
	cil=dev.command_list_query();
	for(int i=0;i<cil.length;i++)
		cb1.addItem(cil[i].cmd_name);	
	}
if (ev.getActionCommand()=="comboBoxChanged"){
	l1.setText(cil[((JComboBox)ev.getSource()).getSelectedIndex()].in_type_desc);
	l2.setText(cil[((JComboBox)ev.getSource()).getSelectedIndex()].out_type_desc);
	l3.setText(TangoConst.Tango_CmdArgTypeName[cil[((JComboBox)ev.getSource()).getSelectedIndex()].in_type]);
	l4.setText(TangoConst.Tango_CmdArgTypeName[cil[((JComboBox)ev.getSource()).getSelectedIndex()].out_type]);
	}
} catch(DevFailed e) {
        t1.setForeground(Color.red);
        t1.append("failed request\n"+e);
        }
}

public static void main(String[] args) throws Exception 
{
	new JClient();
}

}
