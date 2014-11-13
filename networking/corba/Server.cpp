// **********************************************************************
//
// $Id: Server.cpp,v 1.2 2002/07/10 16:31:02 raph Exp $
//
// **********************************************************************

#include <OB/CORBA.h>
#include <OB/BootManager.h>

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
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
    
    //
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();
    
    //
    // Create implementation object
    //
    //pcitor48_impl* pImpl ;//= new pcitor48_impl(orb);
    pcitor48_impl* pImpl = new pcitor48_impl(rootPOA);
    PortableServer::ServantBase_var servant = pImpl;
    pcitor48_var p = pImpl -> _this();

    //
    // Resolve the BootManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

    PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId("pcitor48");
    bootManager -> add_binding(oid, p);
    
    //
    // Save reference
    //
    CORBA::String_var s = orb -> object_to_string(p);
    
    const char* refFile = "pcitor48.ior";
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
