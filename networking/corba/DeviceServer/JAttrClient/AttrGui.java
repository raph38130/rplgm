/* AttrGui.java
 * ESRF/LGM project
 *
 * a Corba client for a TANGO (ESRF Control System) Device Server
 *
 * RP Fri May 18 23:21:34 CEST 2001
 *	- initial work 
 *	
 * USAGE :
 *
 * export CLASSPATH=.::/disk/projet/tango/java/Tango.jar:/disk/projet/tango/ORB/java/OB.jar
 */
import java.io.*;
import java.util.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;

import Tango.*;
import TangoDs.*;

import org.omg.CORBA.*;
import java.util.Properties;


public class AttrGui 
	extends Frame 
	implements ActionListener
{
/*gui*/
Button b1,b2,b3,b4,b5;
TextArea t1;
TextField tf1,tf2,tf3;


org.omg.CORBA.Object  	obj;
ORB 			orb;
Device			dbase,
			device;
DevCmdInfo[] 		dev_commands;
String 			dbase_path = "iiop://mercure:10000/database";
//String 			dbase_path = "iiop://e108poste1:10000/database";
Any			send = null,
 			received = null;
DevVarLongStringArray 	db_received;



AttrGui()
{
super("GUI for TANGO Device Server v1.1");

b1=new Button("command_list_query");
b2=new Button("attr");
b1.addActionListener(this);
b2.addActionListener(this);

t1=new TextArea(15,30);	

//tf1=new TextField("id4/fam/9");
tf1=new TextField("id3/clok/1");
tf2=new TextField("Second");
tf3=new TextField("args");

setSize(700,280);setLayout(null);
tf1.setBounds(10,50,100,30);
b1.setBounds(10,100,120,20);
b2.setBounds(10,150,120,20);	tf2.setBounds(150,150,100,30);
				tf3.setBounds(150,180,100,30);
t1.setBounds(280,50,350,200);
add(b1);add(b2);add(tf1);add(tf2);add(tf3);add(t1);
show();


// Connect to database
Properties props = System.getProperties();
props.put("org.omg.CORBA.ORBClass","com.ooc.CORBA.ORB");
props.put("org.omg.CORBA.ORBSingletonClass","com.ooc.CORBA.ORBSingleton");
System.setProperties(props);
String[] args = new String[1];
args[0]="";
orb = ORB.init(args, props);
obj = ((com.ooc.CORBA.ORB)orb).string_to_object(dbase_path);
//?
dbase = DeviceHelper.narrow(obj);
}



public void actionPerformed(ActionEvent ev)
{
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
		s = s.append(dev_commands[i].cmd_name+" ");
	t1.setText(new String(s));
     } catch(DevFailed e){System.out.println("failed cmd_inout");}

//button attr
if (ev.getActionCommand()=="attr")
try {
	Attribute attr;AttributeConfig[] ac; AttributeValue av[];
	String[]s=new String[1];
	s[0]=tf2.getText();
     	
	ac=device.get_attribute_config(s);
	t1.append("\nattr_config : name="+ac[0].name+"data_type="+ac[0].data_type);
	
	//s[0]=TangoConst.Tango_AllAttr;
	s[0]=tf2.getText();
	device.read_attributes(tf2.getText());
	//t1.append("\nattr_value : name="+av[0].name);
	//t1.append("attr_value : value="+av[0].value);
     } catch(DevFailed e) {
           	t1.setForeground(Color.red);
		t1.append("failed attr"+e);
           	//t1.setForeground(Color.black);
		}

}

public static void main(String[] args) throws Exception 
{
new AttrGui();
}
}
