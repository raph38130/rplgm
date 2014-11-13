// **********************************************************************
//
//$Id: Client.cpp,v 1.2 2002/07/10 16:30:52 raph Exp $
//
// **********************************************************************

#include <OB/CORBA.h>

#include <pcitor48.h>

#include <stdlib.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
    //
    // Get "pcitor48" object
    //
    CORBA::Object_var obj;
    CORBA::Any any;
    any <<= OB::RETRY_ALWAYS;
    CORBA::PolicyList pl;
    pl.length(1);
    pl[0] = orb -> create_policy(OB::RETRY_POLICY_ID, any);
    CORBA::Object_var pmObj =
        orb -> resolve_initial_references("ORBPolicyManager");
    CORBA::PolicyManager_var pm = CORBA::PolicyManager::_narrow(pmObj);
    pm -> add_policy_overrides(pl);

    try
    {
        //obj  = orb -> string_to_object("relfile:/pcitor48auto.ior");
        obj  = orb -> string_to_object("corbaloc::localhost:10000/pcitor48");
    }
    catch(const CORBA::BAD_PARAM&)
    {
    }

    if(CORBA::is_nil(obj))
    {
	cerr << argv[0] << ": cannot read IOR from ior file" << endl;
	return EXIT_FAILURE;
    }
    
    pcitor48_var p = pcitor48::_narrow(obj);
    assert(!CORBA::is_nil(p));
    
    //
    // Main loop
    //
    cout << "Enter 's' for SetLed, 'r' for read 't' for status or 'x' for exit:\n";
    char c;
    int n;bool b;
    do
    {
	cout << "> ";
	cin >> c;
	if(c == 's') {
	    cin >> n;
	    p -> SetLed(n);
	    }
	else if(c == 'r') {
	    cin >> n;
	    b = p -> Read(n);
	    cout << "read : " << b <<endl;
	    }
	else if(c == 't') {
	    cin >> n;
	    CORBA::String_var s1="testing in param ",s2="testing inout param";
	    p -> Status(n,s1,s2);
	    cout << "status : s1 " << s1 << " s2 " << s2 << endl;
	    }
    }
    while(cin.good() && c != 'x');

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

    try
    {
	orb = CORBA::ORB_init(argc, argv);
	status = run(orb, argc, argv);
    }
    catch(const CORBA::Exception& ex)
    {
	cerr << ex << endl;
	status = EXIT_FAILURE;
    }

    if(!CORBA::is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const CORBA::Exception& ex)
	{
	    cerr << ex << endl;
	    status = EXIT_FAILURE;
	}
    }
    
    return status;
}
