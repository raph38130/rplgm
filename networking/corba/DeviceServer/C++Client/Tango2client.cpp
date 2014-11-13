/*
 * Tango2client.cpp
 *
 *
 *
 * Fri Apr 19 12:38:45 CEST 2002 RP
 *	- initial revision (from db_client.cpp)
 * 
 *
 * export LD_LIBRARY_PATH=/home/raph/ooc/lib/:/disk/removable/projet2/TANGO/lib/suse72/
 */
 
#include <tango.h>

#include <assert.h>
#include <iostream.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
Tango::Device_var dev;
CORBA::ORB_var orb;
CORBA::Object_var obj;	

CORBA::Any send;
CORBA::Any_var received ;	
try {
	CORBA::ORB_var orb = CORBA::ORB_init(argc,argv);
	obj = orb->string_to_object("corbaloc::172.16.0.1:20000/database");
//	obj = orb->string_to_object("corbaloc::172.16.104.63:20000/database");
	dev = Tango::Device::_narrow(obj);

	cout << "---Connected to database device" << endl;		
	CORBA::String_var s = dev->name();
	cout << "Received name : " << s << endl; 
	Tango::DevState d = dev->state();
	cout << "Received state : " << d << endl;
	s = dev->description();
	cout << "Received description : " << s << endl;
	s = dev->status();
	cout << "Received status : " << s << endl;
	
	cout << "---DbInfo" << endl;	
	received = dev->command_inout("DbInfo",send);
	const Tango::DevVarStringArray *info_list;
	assert(received >>= info_list);					
	for (int i=0; i<info_list->length(); i++) {
		cout << (*info_list)[i] << endl;
		}
					
	//import device				
	cout << "---import device" << endl;		
	char *device_name = CORBA::string_dup("lgm/horloge/serv");
	send <<= device_name;				
	received = dev->command_inout("DbImportDevice",send);
	const Tango::DevVarLongStringArray *import_info;
	assert(received >>= import_info);					
	cout << "Device import info " << " version " << (import_info->svalue)[2] << " IOR " << (import_info->svalue)[1] << endl;
	cout << "Device exported " << (import_info->lvalue)[0] << endl;		
	obj = orb->string_to_object((import_info->svalue)[1]);
	dev = Tango::Device::_narrow(obj);
	s = dev->description();
	cout << "Received description : " << s << endl;
	const Tango::DevCmdInfoList *list;
	list = dev->command_list_query();
	for (int i=0; i<list->length(); i++) {
		cout << i  /*<<list[i]->cmd_name*/ << endl;
		}				
	received = dev->command_inout("DevGetDate",send);
	const Tango::DevString *result;
	//assert(received >>= result);
	received >>= result;
	cout << "GetTime : " << result << endl;
	}
	catch (CORBA::Exception &e) {
		cout << "erreur !!!" << endl;
		//Tango::Util::print_exception(e);
		}
				
}


/*			case 1 :
				try {
					CORBA::String_var n = dev->name();
					cout << "Received name : " << n << endl;
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
				
//
// Get the description attribute
//
				
			case 2 :
				try {
					CORBA::String_var d = dev->description();
					cout << "Received description : " << d << endl;
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;

//
// Get the state attribute
//
				
			case 3 :
				try {
					Tango::DevState d = dev->state();
					cout << "Received state : " << d << endl;
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;

//
// Get the status attribute
//
				
			case 4 :
				try {
					CORBA::String_var d = dev->status();
					cout << "Received status : " << d << endl;
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
				
//
// Execute the DbGetDeviceList command
//
				
			case 5 :
				try {
					int j;
					for (j=0; j<10000; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
						Tango::DevVarStringArray *server_class = new Tango::DevVarStringArray;
						//server_class->(CORBA::string_dup("StepperMotor/dd"));
						//server_class->append(CORBA::string_dup("StepperMotor"));
						(*server_class)[0]=CORBA::string_dup("StepperMotor");
						send <<= server_class;
								
						CORBA::Any_var received = dev->command_inout("DbGetDeviceList",send);
						Tango::DevVarStringArray *device_list;
						assert(received >>= device_list);
						
						int i;	
						for (i=0; i<device_list->length(); i++) {
							cout << "Device " << i << " name " << (*device_list)[i] << endl;
						}
					}
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
//
// Execute the DbGetClassList command
//
				
			case 6 :
				try {
					int j;
					for (j=0; j<10; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
 						char *server_name = CORBA::string_dup("steppermotor/dd");;
                                                send <<= server_name; 
								
						CORBA::Any_var received = dev->command_inout("DbGetClassList",send);
						Tango::DevVarStringArray *class_list;
						assert(received >>= class_list);
						
						int i;	
						for (i=0; i<class_list->length(); i++) {
							cout << "Class " << i << " name " << (*class_list)[i] << endl;
						}
					}
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
//
// Execute the DbExportDevice command
//
				
			case 7 :
				try {
					int j;
					for (j=0; j<10; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
						Tango::DevVarStringArray *export_info = new Tango::DevVarStringArray;
						export_info->append(CORBA::string_dup("id00/motor/01"));
						export_info->append(CORBA::string_dup("ior"));
						export_info->append(CORBA::string_dup("localhost"));
						export_info->append(CORBA::string_dup("1"));
						export_info->append(CORBA::string_dup("0"));
						send <<= export_info;
								
						CORBA::Any_var received = dev->command_inout("DbExportDevice",send);
					}
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
//
// Execute the DbUnExportDevice command
//
				
			case 8 :
				try {
					int j;
					for (j=0; j<10; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
						char *device_name = CORBA::string_dup("id00/motor/01");;
						send <<= device_name;
								
						CORBA::Any_var received = dev->command_inout("DbUnExportDevice",send);
					}
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
//
// Execute the DbUnExportServer command
//
				
			case 9 :
				try {
					int j;
					for (j=0; j<10; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
						char *server_name = CORBA::string_dup("StepperMotor/dd");;
						send <<= server_name;
								
						CORBA::Any_var received = dev->command_inout("DbUnExportServer",send);
					}
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
//
// Execute the DbImportDevice command
//
				
			case 10 :
				try {
					int j;
					for (j=0; j<10; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
						char *device_name = CORBA::string_dup("id00/motor/01");;
						send <<= device_name;
								
						CORBA::Any_var received = dev->command_inout("DbImportDevice",send);
						Tango::DevVarLongStringArray *import_info;
						assert(received >>= import_info);
						
						cout << "Device import info " << " version " << (import_info->svalue)[2] << " IOR " << (import_info->svalue)[1] << endl;
						cout << "Device exported " << (import_info->lvalue)[0] << endl;
					}
				}
				catch (CORBA::Exception &e) {
					//Tango::Util::print_exception(e);
				}
				break;
//
// Execute the DbGetDeviceProperty command
//
			case 11 :
				try {
					int j;
					char *d_name="id00/motor/01", *r_name="position";
					for (j=0; j<10; j++) {
						cout << endl << "loop " << j << endl;
						CORBA::Any send;
						Tango::DevVarStringArray *property_names = new Tango::DevVarStringArray;
						property_names->append(CORBA::string_dup(d_name));
						property_names->append(CORBA::string_dup(r_name));
						property_names->append(CORBA::string_dup("dummy"));
						send <<= property_names;
								
						CORBA::Any_var received = dev->command_inout("DbGetDeviceProperty",send);
						Tango::DevVarStringArray *property_list;
						assert(received >>= property_list);
						
						int i;	
						cout << endl << "device name " << (*property_list)[0] << " number " << (*property_list)[1] << endl << endl;
						for (i=2; i<property_list->length(); i++) {
							cout << (*property_list)[i] << endl;
						 }
					 }
				 }
				 catch (CORBA::Exception &e) {
					 //Tango::Util::print_exception(e);
				 }
				 break;
				
				 //
// Execute the DbPutDeviceProperty command
//
			case 12 :
				try {
					int j;
					char *d_name="id00/motor/01", *r_name="position", *count[8] = {"1","2","3","4","5","6","7","8"}, value[8][16];
					for (j=0; j<10; j++) {
						cout << "loop " << j << endl;
						CORBA::Any send;
						Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
						property_list->append(CORBA::string_dup(d_name));
						property_list->append(CORBA::string_dup("1"));
						property_list->append(CORBA::string_dup(r_name));
						property_list->append(CORBA::string_dup("8"));
						int k;
						for (k=0; k<8; k++) {
							sprintf(value[k],"%d",k);
							property_list->append(CORBA::string_dup(value[k]));
						}
						send <<= property_list;
								
						CORBA::Any_var received = dev->command_inout("DbPutDeviceProperty",send);
					 }
				 }
				 catch (CORBA::Exception &e) {
					 //Tango::Util::print_exception(e);
				 }
				 break;
				
//
// Execute the DbInfo command
//
*/			
