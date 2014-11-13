#include <OB/CORBA.h>
#include <pcitor48_impl.h>

//
// IDL:pcitor48:1.0
//
pcitor48_impl::pcitor48_impl(PortableServer::POA_ptr poa)
    : poa_(PortableServer::POA::_duplicate(poa))
{
}

pcitor48_impl::~pcitor48_impl()
{
}

PortableServer::POA_ptr
pcitor48_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

//
// IDL:pcitor48/SetLed:1.0
//
void
pcitor48_impl::SetLed(CORBA::Short number)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:pcitor48/Read:1.0
//
CORBA::Boolean
pcitor48_impl::Read(CORBA::Short number)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::Boolean _r = false;
    return _r;
}

//
// IDL:pcitor48/Status:1.0
//
void
pcitor48_impl::Status(CORBA::Short number,
                      CORBA::String_out msg,
                      char*& test)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    msg = CORBA::string_dup("");
    CORBA::string_free(test);
    test = CORBA::string_dup("");
}
