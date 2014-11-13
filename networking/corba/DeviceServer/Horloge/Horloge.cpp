static const char *RcsId = "$Header: /opt2/cvs//TANGO/DeviceServer/Horloge/Horloge.cpp,v 1.1.1.1 2002/05/15 10:05:03 raph Exp $";
//+=============================================================================
//
// file :         Horloge.cpp
//
// description :  C++ source for the Horloge and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                Horloge are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author: raph $
//
// $Revision: 1.1.1.1 $
//
// $Log: Horloge.cpp,v $
// Revision 1.1.1.1  2002/05/15 10:05:03  raph
// initial cvs import
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================


//===================================================================
//
//	The folowing table gives the correspondance
//	between commands and method's name.
//
//	Command's name	|	Method's name
//	----------------------------------------
//	DevState	|	dev_state()
//	DevStatus	|	dev_status()
//	DevGetDate	|	dev_get_date()
//	DevTestCmd	|	dev_test_cmd()
//
//===================================================================



#include <tango.h>
#include <Horloge.h>
#include <time.h>

namespace Horloge
{

//+----------------------------------------------------------------------------
//
// method : 		Horloge::Horloge(string &s)
// 
// description : 	constructor for simulated Horloge
//
// in : - cl : Pointer to the DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------
Horloge::Horloge(Tango::DeviceClass *cl,string &s):Tango::DeviceImpl(cl,s.c_str())
{
	init_device();
}

Horloge::Horloge(Tango::DeviceClass *cl,const char *s):Tango::DeviceImpl(cl,s)
{
	init_device();
}

Horloge::Horloge(Tango::DeviceClass *cl,const char *s,const char *d)
:Tango::DeviceImpl(cl,s,d)
{
	init_device();
}

void Horloge::init_device()
{
	cout << "Horloge::Horloge() create device " << device_name << endl;

	// Initialise variables to default values
	//--------------------------------------------
}

//+----------------------------------------------------------------------------
//
// method : 		Horloge::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------
void Horloge::always_executed_hook()
{

	cout2 << "In always_executed_hook method" << endl;
	
}

//+------------------------------------------------------------------
/**
 *	method:	Horloge::dev_get_date
 *
 *	description:	method to execute "DevGetDate"
 *
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevString Horloge::dev_get_date()
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter 5.3)
	//------------------------------------------------------------
	Tango::DevString	argout  = new char[60];
	time_t t=time(NULL);
	strcpy(argout, ctime(&t));
	cout2 << "Horloge::dev_get_date(): entering... !" << endl;
	
	//	Add your own code to control device here

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	Horloge::dev_test_cmd
 *
 *	description:	method to execute "DevTestCmd"
 *	a demo command to show arg passing rules
 *
 * @param	argin	an integer to parse
 * @return	parsed int
 *
 */
//+------------------------------------------------------------------
Tango::DevString Horloge::dev_test_cmd(Tango::DevLong argin)
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter 5.3)
	//------------------------------------------------------------
	Tango::DevString	argout  = new char[6];
	cout2 << "Horloge::dev_test_cmd(): entering... ! argin=" << argin<< endl;
	switch(argin % 8) {
		case 0 : strcpy(argout, "ZERO"); break;
		case 1 : strcpy(argout, "ONE"); break;
		case 2 : strcpy(argout, "TWO"); break;
		case 3 : strcpy(argout, "THREE"); break;
		case 4 : strcpy(argout, "FOUR"); break;
		case 5 : strcpy(argout, "FIVE"); break;
		case 6 : strcpy(argout, "SIX"); break;
		case 7 : strcpy(argout, "SEVEN"); break;
	}
	//	Add your own code to control device here

	return argout;
}

}	//	namespace
