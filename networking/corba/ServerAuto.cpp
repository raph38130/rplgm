// **********************************************************************
//
// this server is started by IMR service
//
// **********************************************************************

#include <OB/CORBA.h>
#include <OB/BootManager.h>
#include <OB/Properties.h>

#include <pcitor48_impl.h>

#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
    //
    // Resolve Root POA
    //
    cout << "c" << endl;
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    cout << "d" << endl;
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
    cout << "e" << endl;
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();

    cout << "1" <<endl;

    CORBA::PolicyList pl(2);
    pl.length(2);
    pl[0]=rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
    pl[1]=rootPOA->create_id_assignment_policy(PortableServer::USER_ID);

    cout << "2" <<endl;

    PortableServer::POA_var pPOA=rootPOA->create_POA("pcitor48",manager,pl);
    pcitor48_impl* pImpl = new pcitor48_impl(pPOA);
    PortableServer::ServantBase_var servant = pImpl;
    PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId("pcitor48");
    pPOA -> activate_object_with_id(oid, servant);
    pcitor48_var p = pImpl -> _this();

    cout << "3" <<endl;
    
    //
    // Save reference
    //
    CORBA::String_var s = orb -> object_to_string(p);
    
    const char* refFile = "pcitor48auto.ior";
    ofstream out(refFile);
    if(out.fail())
    {
	cerr << argv[0] << ": can't open `" << refFile << "': "
	     << strerror(errno) << endl;
	return EXIT_FAILURE;
    }

    out << s << endl;
    out.close();
    
    //
    // Run implementation
    //
    manager -> activate();
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

    try
    {
	OB::Properties_var properties = OB::Properties::getDefaultProperties();
	OB::ParseArgs(argc, argv, properties);
	properties -> getProperty("ooc.orb.server_name");
	properties -> setProperty("ooc.orb.server_name", "pcitor48");
	cout << "a" << endl;
	orb = OBCORBA::ORB_init(argc, argv, OB::Properties::_nil(),"");
	cout << "b" << endl;
	
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
