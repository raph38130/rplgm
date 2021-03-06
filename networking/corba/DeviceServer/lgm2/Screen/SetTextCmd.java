//+======================================================================
// $Source: $
//
// Project:      Tango Device Server
//
// Description:  Java source code for the command TemplateClass of the
//               Screen class.
//
// $Author: $
//
// $Revision$
//
// $Log:	$
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//-======================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================



/**
 * @author	$Author: $
 * @version	$Revision: 1.0 $
 */
package Screen;



import org.omg.CORBA.*;
import fr.esrf.Tango.*;
import fr.esrf.TangoDs.*;

/**
 *	Class Description:
 *	draw text into a Swing Frame
*/


public class SetTextCmd extends Command implements TangoConst
{
	//===============================================================
	/**
	 *	Constructor for Command class SetTextCmd
	 *
	 *	@param	name	command name
	 *	@param	in	argin type
	 *	@param	out	argout type
	 */
	//===============================================================
	public SetTextCmd(String name,int in,int out)
	{
		super(name, in, out);
	}

	//===============================================================
	/**
	 *	Constructor for Command class SetTextCmd
	 *
	 *	@param	name            command name
	 *	@param	in              argin type
	 *	@param	in_comments     argin description
	 *	@param	out             argout type
	 *	@param	out_comments    argout description
	 */
	//===============================================================
	public SetTextCmd(String name,int in,int out, String in_comments, String out_comments)
	{
		super(name, in, out, in_comments, out_comments);
	}
	//===============================================================
	/**
	 *	return the result of the device's command.
	 */
	//===============================================================
	public Any execute(DeviceImpl device,Any in_any) throws DevFailed
	{
		Util.out2.println("SetTextCmd.execute(): arrived");
		String argin = extract_DevString(in_any);
		((Screen)(device)).set_text(argin);
		return insert();
	}

	//===============================================================
	/**
	 *	Check if it is allowed to execute the command.
	 */
	//===============================================================
	public boolean is_allowed(DeviceImpl device, Any data_in)
	{
			//	End of Generated Code

			//	Re-Start of Generated Code
		return true;
	}
}
//-----------------------------------------------------------------------------
/* end of $Source: $ */
