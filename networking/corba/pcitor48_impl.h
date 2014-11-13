#ifndef ___pcitor48_impl_h__
#define ___pcitor48_impl_h__

#include <pcitor48_skel.h>

//
// IDL:pcitor48:1.0
//
class pcitor48_impl : virtual public POA_pcitor48,
                      virtual public PortableServer::RefCountServantBase
{
    pcitor48_impl(const pcitor48_impl&);
    void operator=(const pcitor48_impl&);

    PortableServer::POA_var poa_;

public:

    pcitor48_impl(PortableServer::POA_ptr);
    ~pcitor48_impl();

    virtual PortableServer::POA_ptr _default_POA();

    //
    // IDL:pcitor48/SetLed:1.0
    //
    virtual void SetLed(CORBA::Short number)
        throw(CORBA::SystemException);

    //
    // IDL:pcitor48/Read:1.0
    //
    virtual CORBA::Boolean Read(CORBA::Short number)
        throw(CORBA::SystemException);

    //
    // IDL:pcitor48/Status:1.0
    //
    virtual void Status(CORBA::Short number,
                        CORBA::String_out msg,
                        char*& test)
        throw(CORBA::SystemException);
};

#endif
