/* JCorbaClient.java
 * ESRF/LGM project
 *
 * a Corba client for a TANGO (ESRF Control System) Device Server
 *
 * Mon Apr  1 22:42:00 CEST 2002 Rp
 *	- back to work with TANGO2 and JOB4.1
 * Fri May 18 23:21:34 CEST 2001 RP 
 *	- initial work 
 *	
 * USAGE :
 * replace into device (name,class,server,version,exported,ior) values("dserver/horloge/serv","horloge","horloge/serv","TANGO2",0,"eee");
 * export CLASSPATH=.:/disk/removable/projet2/TANGO/java/Tango.jar:/home/raph/JOB-4.1.1/lib/OB.jar
 */
import java.io.*;
import java.util.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;

import fr.esrf.Tango.*;
import fr.esrf.TangoDs.*;
import fr.esrf.TangoApi.*;

import org.omg.CORBA.*;
import java.util.Properties;

public class JCorbaClient
	extends Frame 
	implements ActionListener
{
/*gui*/
Button b1,b2,b3,b4,b5;
TextArea t1;
TextField tf1,tf2,tf3,tf4;

org.omg.CORBA.Object  	obj;
ORB 			orb;
Device			dbase,
			device;
DevCmdInfo[] 		dev_commands;
String 			dbase_path = "corbaloc::localhost:20000/database";
Any			send = null,
 			received = null;
DevVarLongStringArray 	db_received;

JCorbaClient()
{
super("GUI for TANGO Device Server v1.1");

b1=new Button("command_list_query");
b2=new Button("attr");
b3=new Button("clear");
b4=new Button("cmd_inout");

b1.addActionListener(this);
b2.addActionListener(this);
b3.addActionListener(this);
b4.addActionListener(this);

t1=new TextArea(15,30);	

//tf1=new TextField("id4/fam/9");
tf1=new TextField("lgm/horloge/serv");
tf2=new TextField("Second");
tf3=new TextField("args");
tf4=new TextField("DevGetDate");

setSize(700,400);setLayout(null);
tf1.setBounds(10,50,150,30);
b1.setBounds(10,100,170,20);
b2.setBounds(10,150,120,20);	tf2.setBounds(150,150,100,30);
				tf3.setBounds(150,180,100,30);
b3.setBounds(10,200,120,20);
tf4.setBounds(10,250,150,30);
b4.setBounds(10,280,150,30);
t1.setBounds(280,50,350,200);
add(b1);add(b2);add(b3);add(b4);add(tf1);add(tf2);add(tf3);add(tf4);add(t1);
show();


//change properties to use ORBacus instead of sun orb
Properties props = System.getProperties();
props.put("org.omg.CORBA.ORBClass","com.ooc.CORBA.ORB");
props.put("org.omg.CORBA.ORBSingletonClass","com.ooc.CORBA.ORBSingleton");
System.setProperties(props);
String[] args = new String[1];
// Connect to database
args[0]="";
orb = ORB.init(args, props);
obj = ((com.ooc.CORBA.ORB)orb).string_to_object(dbase_path);
//?
dbase = DeviceHelper.narrow(obj);
}



public void actionPerformed(ActionEvent ev)
{
if (ev.getActionCommand()=="clear"){
	t1.setForeground(Color.black);
	t1.setText("");
	return;
	}
if (ev.getActionCommand()=="cmd_inout"){
	try
 	 {
 	  send = orb.create_any();
 	  DevStringHelper.insert(send, tf1.getText());
 	  received = device.command_inout(tf4.getText(),send);
 	 }
 	catch(DevFailed e) {System.out.println("DbImportDevice failed");}
	
	t1.setForeground(Color.black);
	t1.setText(DevVarLongStringArrayHelper.extract(received).toString());
	return;
	}
	
// Get from TANGO database the informations needed to connect to the device
try
  {
   send = orb.create_any();
   DevStringHelper.insert(send, tf1.getText());
   received = dbase.command_inout(new String("DbImportDevice"),send);
  }
  catch(DevFailed e) {System.out.println("DbImportDevice failed");}
  
// Extract the IOR from informations
db_received = DevVarLongStringArrayHelper.extract(received);

// Check if the device is defined in the database but not exported
if (db_received.lvalue[0] == 0) System.out.println("device defined but not exported");

// Import the TANGO device
obj = ((com.ooc.CORBA.ORB)orb).string_to_object(db_received.svalue[1]);
device = DeviceHelper.narrow(obj);

//button command_list_query
if (ev.getActionCommand()=="command_list_query")
try {
     	dev_commands = device.command_list_query();
	StringBuffer s=new StringBuffer();
	for(int i=0;i<dev_commands.length;i++)
		s = s.append(i+"\t"+dev_commands[i].cmd_name+"\n");
	t1.setForeground(Color.black);
	t1.setText(new String(s));
	
     } catch(DevFailed e){System.out.println("failed cmd_inout");}

//button attr
if (ev.getActionCommand()=="attr")
try {
	//Attribute attr;AttributeConfig[] ac; AttributeValue av[];
	//Command com;
	//String[]s=new String[1];
	//s[0]=tf2.getText();
     	
	//ac=device.get_attribute_config(s);
	//t1.append("\nattr_config : name="+ac[0].name+"data_type="+ac[0].data_type);
	
	//s[0]=TangoConst.Tango_AllAttr;
	//s[0]=tf2.getText();
	received=device.command_inout(new String("DevState"),send);
	t1.append("received="+received.toString());
	//t1.append("attr_value : value="+av[0].value);
     } catch(DevFailed e) {
           	t1.setForeground(Color.red);
		t1.append("failed attr"+e);
           	//t1.setForeground(Color.black);
		}

}

public static void main(String[] args) throws Exception 
{
try{
	//ApiUtil.get_db_obj();
	//DeviceProxy dev= new DeviceProxy("sys/database/1");
	DeviceProxy dev= new DeviceProxy("dserver/horloge/serv");
	DevCmdInfo[] res1=dev.command_list_query();
	for(int i=0;i<res1.length;i++)
		System.out.println(res1[i].cmd_name);
	dev= new DeviceProxy("lgm/horloge/serv");
	DeviceData res2=dev.command_inout("DevGetDate");
	System.out.println(res2.extractString());
	}catch(DevFailed e){System.out.println("failed cmd_inout");}


new JCorbaClient();
}
}
