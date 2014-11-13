/*
 * Tango2client.cpp
 *
 *
 * Sun Apr 21 01:07:09 CEST 2002 RP
 *	- using C++ API
 * Fri Apr 19 12:38:45 CEST 2002 RP
 *	- initial revision (from db_client.cpp)
 * 
 *
 * 
export LD_LIBRARY_PATH=/home/raph/ooc/lib/:/disk/removable/projet2/TANGO/lib/suse72/
export TANGO_HOST=localhost:20000
 *
 */
 
#include <tango.h>
#include <iostream.h>

using namespace Tango;

int main(int argc,char *argv[])
{
	
try {	string s;

	DeviceProxy *dev= new DeviceProxy("lgm/horloge/serv");
	DeviceData res=dev->command_inout("DevGetDate");
	res >> s;
	cout << "date " << s << endl;
	
	DevCommandInfoList cmd_info_l;
	cmd_info_l=dev->command_list_query();
	for(int i=0;i<cmd_info_l.size();i++) 
		cout << i << "\t" << cmd_info_l[i].cmd_name << endl;

	long int n;
	cout << "enter n for testcmd : ";
	cin >> n;
	DeviceData argin;
	argin << n;
	res=dev->command_inout("DevTestCmd",argin);
	res >> s;
	cout << "testcmd reply=" << s << endl;
	}
	
catch (CORBA::Exception &e) {
	cout << "erreur !!!" << endl;
	//Util::print_exception(e);
	}
				
}

