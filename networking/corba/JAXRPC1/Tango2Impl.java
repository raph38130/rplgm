/* $Id$
 *
 *
 */
package lgm;
import fr.esrf.Tango.*;
import fr.esrf.TangoDs.*;
import fr.esrf.TangoApi.*;
 
public class Tango2Impl implements Tango2IF {

    /* for test only */
    public String myFunc1(int n)
    {
        return new Integer(n*20+1).toString();
    }    
    
    /* Tango2 Interface */
    public String[] getserverlist()
    {
    String []s=null;
    try {
    	fr.esrf.TangoApi.Database db= new fr.esrf.TangoApi.Database("localhost","30000");
    	s=db.get_server_list();
    	}
    catch(fr.esrf.Tango.DevFailed e) {System.out.println("getserverlist"+e);}
    return s;
    }
    
    public String[] commandlistquery(String device)
    {
    String[] s=null;
    try {
    	fr.esrf.TangoApi.DeviceProxy dev= new fr.esrf.TangoApi.DeviceProxy(device,"localhost","30000");
    	fr.esrf.TangoApi.CommandInfo[] cil=dev.command_list_query();
	s=new String[cil.length];
	for (int i=0;i<cil.length;i++) s[i]=cil[i].cmd_name;
    	}
    catch(fr.esrf.Tango.DevFailed e) {System.out.println("commandlistquery"+e);}
    return s;
    }
    
    public String[] commandinout(String device, String cmd, String args)
    {
    String[] s=null;
    try {
    	fr.esrf.TangoApi.DeviceProxy dev= new fr.esrf.TangoApi.DeviceProxy(device,"localhost","30000");
    	fr.esrf.TangoApi.CommandInfo ci=dev.command_query(cmd);
	fr.esrf.TangoApi.DeviceData argin=new fr.esrf.TangoApi.DeviceData();

	//todo : swich with TangoType
	if (TangoConst.Tango_CmdArgTypeName[ci.in_type].equals("DevString"))
		argin.insert(args);
	if (TangoConst.Tango_CmdArgTypeName[ci.in_type].equals("DevInteger"))
		argin.insert(new Short(args).shortValue());
	fr.esrf.TangoApi.DeviceData res=dev.command_inout(cmd, argin);
	s=new String[1];
	if (TangoConst.Tango_CmdArgTypeName[ci.out_type].equals("DevString"))
		s[0]=res.extractString();
	if (TangoConst.Tango_CmdArgTypeName[ci.out_type].equals("DevInteger"))
		s[0]=new Short(res.extractShort()).toString();
    	}
    catch(fr.esrf.Tango.DevFailed e) {System.out.println("commandinout"+e);}
    return s;
    }
    
}
