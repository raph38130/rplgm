// **********************************************************************
//
// Generated by the ORBacus IDL to Java Translator
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

// Version: 4.1.1

//
// IDL:pcitor48:1.0
//
final public class pcitor48Holder implements org.omg.CORBA.portable.Streamable
{
    public pcitor48 value;

    public
    pcitor48Holder()
    {
    }

    public
    pcitor48Holder(pcitor48 initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = pcitor48Helper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        pcitor48Helper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return pcitor48Helper.type();
    }
}
