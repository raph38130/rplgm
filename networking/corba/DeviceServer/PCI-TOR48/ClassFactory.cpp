static const char *RcsId = "$Header: /opt2/cvs//TANGO/DeviceServer/PCI-TOR48/ClassFactory.cpp,v 1.1.1.1 2002/05/15 10:05:03 raph Exp $";
//+=============================================================================
//
// file :        ClassFactory.cpp
//
// description : C++ source for the class_factory method of the DServer
//               device class. This method is responsible to create
//               all class singletin for a device server. It is called
//               at device server startup
//
// project :     TANGO Device Server
//
// $Author: raph $
//
// $Revision: 1.1.1.1 $
//
// $Log: ClassFactory.cpp,v $
// Revision 1.1.1.1  2002/05/15 10:05:03  raph
// initial cvs import
//
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================


#include <tango.h>
#include <PCITOR48Class.h>

/**
 *	Create PCITOR48Class singleton and store it in DServer object.
 *
 * @author	$Author: raph $
 * @version	$Revision: 1.1.1.1 $ $
 */

void Tango::DServer::class_factory()
{

	add_class(PCITOR48::PCITOR48Class::init("PCITOR48"));

}
