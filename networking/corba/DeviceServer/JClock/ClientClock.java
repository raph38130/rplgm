/* ClientClock.java
 *
 * projet ESRF/LGM
 *
 * client TANGO/Clock
 *
 * RP Tue Dec 12 09:58:35 CET 2000
 */
import org.omg.CORBA.*;
import Tango.*;
import TangoDs.*;

public class ClientClock implements TangoConst
{

static public void main(String[]s)
{
DevHelper device;
Tango.DevCmdInfo[] commands;

System.out.println("Client Clock");
try
  { 
  device = new DevHelper("dserver/dsclock/mp", "e108poste1", "10000");
  commands = device.command_list_query();
  
  System.out.println("reponse DS = "+device.exec_command("getTimeDate",""));
  
  for(int i=0; i<commands.length;i++)
  	System.out.println(commands[i].cmd_name);
  }
  catch(Exception e) {System.out.println("error opening device");}
  System.out.println("bye...");
}
}
