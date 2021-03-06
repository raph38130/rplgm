//=============================================================================
//
// file :        Clock.h
//
// description : Include for the Clock class.
//
// project :	lgm
//
// $Author: $
//
// $Revision: $
//
// $Log: Clock.h,v $
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================
#ifndef _CLOCK_H
#define _CLOCK_H

#include <tango.h>
//using namespace Tango;

/**
 * @author	$Author: $
 * @version	$Revision: $ $
 */

 //	Add your own constants definitions here.
 //-----------------------------------------------


namespace Clock
{

/**
 * Class Description:
 * a simple device server for TANGO2 at LGM
 */

/*
 *	Device States Description:
 */


class Clock: public Tango::Device_2Impl
{
public :
	//	Add your own data members here
	//-----------------------------------------


	//	Here is the Start of the automatic code generation part
	//-------------------------------------------------------------	
/**
 *	@name attributes
 *	Attributs member data.
 */
//@{
//@}

/**
 *	@name Device properties
 *	Device properties member data.
 */
//@{
//@}

/**@name Constructors
 * Miscellaneous constructors */
//@{
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	Clock(Tango::DeviceClass *,string &);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	Clock(Tango::DeviceClass *,const char *);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device name
 *	@param d	Device description.
 */
	Clock(Tango::DeviceClass *,const char *,const char *);
//@}

/**@name Destructor
 * Only one desctructor is defined for this class */
//@{
/**
 * The object desctructor.
 */	
	~Clock() {};
//@}

	
/**@name Miscellaneous methods */
//@{
/**
 *	Initialize the device
 */
	virtual void init_device();
/**
 *	Always executed method befor execution command method.
 */
	virtual void always_executed_hook();

//@}

/**
 * @name Clock methods prototypes
 */

//@{
/**
 * return system info
 *	for test only
 *	@return	date and time in a string
 *	@exception DevFailed
 */
	Tango::DevString	get_date();

/**
 *	Read the device properties from database
 */
	 void get_device_property();
//@}

	//	Here is the end of the automatic code generation part
	//-------------------------------------------------------------	



protected :	
	//	Add your own data members here
	//-----------------------------------------
};

}	// namespace

#endif	// _CLOCK_H
