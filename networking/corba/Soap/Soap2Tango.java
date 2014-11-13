/* $Id: Soap2Tango.java,v 1.2 2002/05/15 14:43:21 raph Exp $
 * ESRF/LGM project 
 *
 * SOAP-TANGO interface
 *
 * $Header: /opt2/cvs//TANGO/Soap/Soap2Tango.java,v 1.2 2002/05/15 14:43:21 raph Exp $
 *
 * TODO : session managment...
 *
 * RP Thu Nov  1 19:24:36 CET 2001
 *	- complete rewrite of app. to get maximal info from DS itself
 *	- no more use of Soap2TangoHelper.java
 *
 * RP Wed Mar 14 12:59:07 CET 2001
 *	- session managment is still TO DO (see SOAP latest revision 2.2)
 *
 * RP Sun Feb 18 20:35:08 CET 2001
 *	- command_inout, command_list_query
 *	- everything relative to Tango is in Soap2TangoHelper.java
 *
 * USAGE :
 *	to deploy service
 *	java  org.apache.soap.server.ServiceManagerClient http://mercure:8080/soap/servlet/rpcrouter deploy DeploymentDescriptor.xml 
 */

import fr.esrf.Tango.*;
import fr.esrf.TangoDs.*;
import fr.esrf.TangoApi.*;


public class Soap2Tango
{
DeviceProxy device;

public String[] command_list_query(String devname)
{
String[] s=null;
try
  {
	device = new DeviceProxy(devname,"localhost","20000");
	DevCmdInfo[] dev_commands = device.command_list_query();
	s=new String[5*dev_commands.length];
	for(int i=0;i<dev_commands.length;i++) {
		s[5*i+0]=new String(dev_commands[i].cmd_name);
		s[5*i+1]=new String(dev_commands[i].in_type_desc);
		s[5*i+2]=new String(dev_commands[i].out_type_desc);
		s[5*i+3]=new String(TangoConst.Tango_CmdArgTypeName[dev_commands[i].in_type]);
		s[5*i+4]=new String(TangoConst.Tango_CmdArgTypeName[dev_commands[i].out_type]);
		}
  }
catch(DevFailed e) {System.out.println("command_list_query failed");}	
return s;
}

public String command_inout(String devname, String cmd, String args)
{
DeviceData res=null;
try
  {
	device = new DeviceProxy(devname,"localhost","20000");
	DeviceData argin=new DeviceData();
	argin.insert(new Integer(args).intValue());
	res=device.command_inout(cmd,argin);
  }
catch(DevFailed e) {System.out.println("command_list_query failed");}
return res.extractString();
}

}




