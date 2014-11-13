/* CorbaClient.java
 *
 * projet ESRF/LGM
 *
 * un client CORBA pour TANGO
 *
 * RP Wed Feb 14 10:11:19 CET 2001
 *	- version initiale
 *
 */
import Tango.*;
import TangoDs.*;

import org.omg.CORBA.*;

import java.util.Properties;
//import java.util.Vector;

class CorbaClient implements TangoConst
{
String[] array_result;

CorbaClient(String device_name, String command_name)
{
org.omg.CORBA.Object  obj;
ORB 	orb;
Device	dbase, device;
DevCmdInfo[] dev_commands;

String 	dbase_path = "iiop://mercure:10000/database";

Any	send = null,
 	received = null;

DevVarLongStringArray db_received;

// Connect to database
Properties props = System.getProperties();
props.put("org.omg.CORBA.ORBClass","com.ooc.CORBA.ORB");
props.put("org.omg.CORBA.ORBSingletonClass","com.ooc.CORBA.ORBSingleton");
System.setProperties(props);
//
String[] args = new String[1];
args[0]="";
orb = ORB.init(args, props);
obj = ((com.ooc.CORBA.ORB)orb).string_to_object(dbase_path);
//?
dbase = DeviceHelper.narrow(obj);

// Get from TANGO database the informations needed to connect to the device
try
  {
   send = orb.create_any();
   DevStringHelper.insert(send, device_name);
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

try {
     if	(command_name=="black_box")
     	array_result = device.black_box(0);
     if (command_name=="command_list_query") {
     	dev_commands = device.command_list_query();
	}
     else {
     	received = device.command_inout(command_name, send);
	array_result=new String[1];
	array_result[0]=received.extract_string();
	}
     } catch(DevFailed e){System.out.println("failed to execute "+command_name);}

System.out.println("execute "+ command_name +" on " + device_name+" :\n");
}


String get()
{
if (array_result != null)
for(int i=0;i<array_result.length;i++)
	System.out.println(array_result[i]);
return "not yet implemented";
}

public static void main(String[] s)
{
System.out.println("CorbaClient for DSClock v0.0");
new CorbaClient("id1/DSClock/1","getTimeDate").get();
new CorbaClient("id1/DSClock/1","black_box").get();
new CorbaClient("id1/DSClock/1","command_list_query").get();
}
}
