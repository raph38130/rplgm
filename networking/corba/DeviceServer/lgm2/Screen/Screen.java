//+============================================================================
// $Source: $
//
// project :     Tango Device Server
//
// Description:	java source code for the Screen class and its commands.
//              This class is derived from DeviceImpl class.
//              It represents the CORBA servant obbject which
//              will be accessed from the network. All commands which
//              can be executed on the Screen are implemented
//              in this file.
//
// $Author: $
//
// $Revision$
//
// $Log:	$
//
// copyleft :   European Synchrotron Radiation Facility
//              BP 220, Grenoble 38043
//              FRANCE
//
//-============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================


package Screen;


import java.util.*;
import org.omg.CORBA.*;
import fr.esrf.Tango.*;
import fr.esrf.TangoDs.*;
import fr.esrf.TangoApi.*;


/**
 *	Class Description:
 *	a simple GUI
 *
 * @author	$Author:  $
 * @version	$Revision: 1.0 $
 */

//--------- Start of States Description ----------
/*
 *	Device States Description:
 */
//--------- End of States Description ----------


public class Screen extends DeviceImpl implements TangoConst
{
	protected	int	state;

	//--------- Start of attributes data members ----------

	//--------- End of attributes data members ----------


	//--------- Start of properties data members ----------

	//--------- End of properties data members ----------


	//	Add your own data members here
	//--------------------------------------
javax.swing.JFrame f;
javax.swing.JLabel l;


//=========================================================
/**
 *	Constructor for simulated Time Device Server.
 *
 *	@param	cl	The DeviceClass object
 *	@param	s	The Device name.
 */
//=========================================================
	Screen(DeviceClass cl, String s) throws DevFailed
	{
		super(cl,s);
		init_device();
	}
//=========================================================
/**
 *	Constructor for simulated Time Device Server.
 *
 *	@param	cl	The DeviceClass object
 *	@param	s	The Device name.
 *	@param	d	Device description.
 */
//=========================================================
	Screen(DeviceClass cl, String s, String d) throws DevFailed
	{
		super(cl,s,d);
		init_device();
	}


//=========================================================
/**
 *	Initialize the device.
 */
//=========================================================
	public void init_device() throws DevFailed
	{
		System.out.println("Screen() create " + device_name);

		//	Initialise variables to default values
		//-------------------------------------------
		set_state(DevState.ON);
		f= new javax.swing.JFrame("Tango2 Screen V0.0.0");
		f.setSize(200,200);
		f.setVisible(true);
		f.getContentPane().add(l=new javax.swing.JLabel());

	}

//=========================================================
/**
 *	Method always executed before command execution.
 */
//=========================================================
	public void always_executed_hook()
	{	
		Util.out2.println("In always_executed_hook method()");
	}


//=========================================================
/**
 *	Execute command "SetText" on device.
 *	draw text into a Swing Frame
 *
 * @param	argin	the char to print on screen
 */
//=========================================================
	public void set_text(String argin) throws DevFailed
	{
		Util.out2.println("Entering set_text()");

		// ---Add your Own code to control device here ---
		l.setText(argin);
		Util.out2.println("Exiting set_text()");
	}



//=========================================================
/**
 *	main part for the device server class
 */
//=========================================================
	public static void main(String[] argv)
	{
		try
		{
			Util tg = Util.init(argv,"Screen");
			tg.server_init();

			System.out.println("Ready to accept request");

			tg.server_run();
		}

		catch (OutOfMemoryError ex)
		{
			System.err.println("Can't allocate memory !!!!");
			System.err.println("Exiting");
		}
		catch (UserException ex)
		{
			Except.print_exception(ex);
			
			System.err.println("Received a CORBA user exception");
			System.err.println("Exiting");
		}
		catch (SystemException ex)
		{
			Except.print_exception(ex);
			
			System.err.println("Received a CORBA system exception");
			System.err.println("Exiting");
		}
		
		System.exit(-1);		
	}
}
	

//--------------------------------------------------------------------------
/* end of $Source: $ */
