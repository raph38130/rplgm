static const char *RcsId = "$Header: $";
//+=============================================================================
//
// file :        ClockClass.cpp
//
// description : C++ source for the ClockClass. A singleton
//               class derived from DeviceClass. It implements the
//               command list and all properties and methods required
//               by the Clock once per process.
//
// project :     TANGO Device Server
//
// $Author: $
//
// $Revision: $
//
// $Log: ClockClass.cpp,v $
//
// copyleft :   European Synchrotron Radiation Facility
//              BP 220, Grenoble 38043
//              FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================


#include <tango.h>

#include <Clock.h>
#include <ClockClass.h>


namespace Clock
{


//+----------------------------------------------------------------------------
//
// method : 		GetDateCmd::GetDateCmd()
// 
// description : 	constructor for the command of the Clock.
//
// In : - name : The command name
//		- in : The input parameter type
//		- out : The output parameter type
//		- in_desc : The input parameter description
//		- out_desc : The output parameter description
//
//-----------------------------------------------------------------------------
GetDateCmd::GetDateCmd(	const char		*name,
								Tango::CmdArgType in,
				       			Tango::CmdArgType out,
								const char		*in_desc,
				       			const char		*out_desc,
								Tango::DispLevel level)
:Command(name,in,out,in_desc,out_desc, level)
{
}
//
//	Constructor without in/out parameters description
//
GetDateCmd::GetDateCmd(	const char		*name,
								Tango::CmdArgType in,
				       			Tango::CmdArgType out)
:Command(name,in,out)
{
}




//+----------------------------------------------------------------------------
//
// method : 		GetDateCmd::is_allowed()
// 
// description : 	method to test whether command is allowed or not in this
//			state. In this case, the command is allowed only if
//			the device is in ON state
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns :	boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------
bool GetDateCmd::is_allowed(Tango::DeviceImpl *device, const CORBA::Any &in_any)
{
		//	End of Generated Code

		//	Re-Start of Generated Code
		return true;
}




//+----------------------------------------------------------------------------
//
// method : 		GetDateCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *GetDateCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "GetDateCmd::execute(): arrived" << endl;

	return insert((static_cast<Clock *>(device))->get_date());
}


//
//----------------------------------------------------------------
//	Initialize pointer for singleton pattern
//----------------------------------------------------------------
//
ClockClass *ClockClass::_instance = NULL;

//+----------------------------------------------------------------------------
//
// method : 		ClockClass::ClockClass(string &s)
// 
// description : 	constructor for the ClockClass
//
// in : - s : The class name
//
//-----------------------------------------------------------------------------
ClockClass::ClockClass(string &s):DeviceClass(s)
{

	cout2 << "Entering ClockClass constructor" << endl;
	write_class_property();
	
	cout2 << "Leaving ClockClass constructor" << endl;

}
//+----------------------------------------------------------------------------
//
// method : 		ClockClass::~ClockClass()
// 
// description : 	destructor for the ClockClass
//
//-----------------------------------------------------------------------------
ClockClass::~ClockClass()
{
	_instance = NULL;
}

//+----------------------------------------------------------------------------
//
// method : 		ClockClass::instance
// 
// description : 	Create the object if not already done. Otherwise, just
//			return a pointer to the object
//
// in : - name : The class name
//
//-----------------------------------------------------------------------------
ClockClass *ClockClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new ClockClass(s);
		}
		catch (bad_alloc)
		{
			throw;
		}		
	}		
	return _instance;
}

ClockClass *ClockClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}

//+----------------------------------------------------------------------------
//
// method : 		ClockClass::command_factory
// 
// description : 	Create the command object(s) and store them in the 
//			command list
//
//-----------------------------------------------------------------------------
void ClockClass::command_factory()
{
	command_list.push_back(new GetDateCmd("GetDate",
		Tango::DEV_VOID, Tango::DEV_STRING,
		"",
		"date and time in a string",
		Tango::OPERATOR));

	//	add polling if any
	for (int i=0 ; i<command_list.size(); i++)
	{
	}
}

//+----------------------------------------------------------------------------
//
// method : 		ClockClass::device_factory
// 
// description : 	Create the device object(s) and store them in the 
//			device list
//
// in :		Tango::DevVarStringArray *devlist_ptr : The device name list
//
//-----------------------------------------------------------------------------
void ClockClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	
	for (long i=0 ; i < devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i] << endl;
						
		// Create device and add it into the device list
		//----------------------------------------------------
		device_list.push_back(new Clock(this, (*devlist_ptr)[i]));							 

		// Export device to the outside world
		// Check before id database used.
		//---------------------------------------------
		if (Tango::Util::_UseDb == true)
			export_device(device_list.back());
		else
			export_device(device_list.back(), (*devlist_ptr)[i]);
	}
}
//+----------------------------------------------------------------------------
//
// method : 		ClockClass::write_class_property
// 
// description : 	Set class description as property in database
//
//-----------------------------------------------------------------------------
void ClockClass::write_class_property()
{
	//	First time, check if database used
	//--------------------------------------------
	if (Tango::Util::_UseDb == false)
		return;

	//	Prepeare DbDatum
	//--------------------------------------------
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("lgm");
	title << str_title;

	Tango::DbDatum	description("Description");
	string	str_desc("a simple device server for TANGO2 at LGM");
	description << str_desc;
	
	Tango::DbData	data;
	data.push_back(title);
	data.push_back(description);

	//	Call database and and values
	//--------------------------------------------
	get_db_class()->put_property(data);
}

}	// namespace
