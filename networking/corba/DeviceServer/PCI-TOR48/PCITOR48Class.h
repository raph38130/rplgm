//=============================================================================
//
// file :         PCITOR48Class.h
//
// description :  Include for the PCITOR48Class root class.
//                This class is represents the singleton class for
//                the PCITOR48 device class.
//                It contains all properties and methods which the 
//                PCITOR48 requires only once e.g. the commands.
//			
// project :      TANGO Device Server
//
// $Author: raph $
//
// $Revision: 1.1.1.1 $
//
// $Log: PCITOR48Class.h,v $
// Revision 1.1.1.1  2002/05/15 10:05:03  raph
// initial cvs import
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================

#ifndef _PCITOR48CLASS_H
#define _PCITOR48CLASS_H

#include <tango.h>


namespace PCITOR48
{
//
// Define classes for commands
//
class DevReadCmd : public Tango::Command
{
public:
	DevReadCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *);
	DevReadCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevReadCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};


class DevWriteCmd : public Tango::Command
{
public:
	DevWriteCmd(const char *,Tango::CmdArgType, Tango::CmdArgType,const char *,const char *);
	DevWriteCmd(const char *,Tango::CmdArgType, Tango::CmdArgType);
	~DevWriteCmd() {};
	
	virtual bool is_allowed (Tango::DeviceImpl *, const CORBA::Any &);
	virtual CORBA::Any *execute (Tango::DeviceImpl *, const CORBA::Any &);
};



//
// The PCITOR48Class singleton definition
//

class PCITOR48Class : public Tango::DeviceClass
{
public:

//	add your own data members here
//------------------------------------

public:

//	Method prototypes
	static PCITOR48Class *init(const char *);
	static PCITOR48Class *instance();
	~PCITOR48Class();
	
protected:
	PCITOR48Class(string &);
	static PCITOR48Class *_instance;
	void command_factory();

private:
	void device_factory(const Tango::DevVarStringArray *);
};


}	//	namespace PCITOR48

#endif // _PCITOR48CLASS_H
