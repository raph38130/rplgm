/* Server.java
 *	jooc edition
 * 
 * Fri Feb 22 12:00:25 CET 2002 RP
 *	- initial revision
 *
 * run with 
 java  -Xbootclasspath/p:/usr/local/JOB-4.1.1/lib/OB.jar -Dooc.orb.oa.endpoint="iiop --port 10000
 */



public class Server
{

public static void main(String args[])
{
System.out.println("v0.0");
java.util.Properties props=System.getProperties();
props.put("org.omg.CORBA.ORBClass","com.ooc.CORBA.ORB");
props.put("org.omg.CORBA.ORBSingletonClass","com.ooc.CORBA.ORBSingleton");

org.omg.CORBA.ORB orb = null;
try	{
  	//init ORB
	orb = org.omg.CORBA.ORB.init(args, props);
	/*if (args.length !=1) {
	 System.out.println("usage : Server iorfile");
	 System.exit(1);
	 }*/
	/* Portable Object Adaptater
	 * the POA is designed to allow programmers to build object implementations
	 * that can provide consistent service for objects whose lifetimes (from the
	 * perspective of a client holding a reference for such an object) span multiple
	 * server lifetimes.
	 */ 
	org.omg.PortableServer.POA rootPOA =
		org.omg.PortableServer.POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
	rootPOA.the_POAManager().activate();
	pcitor48 card = new pcitor48_impl(rootPOA)._this(orb);
		
	com.ooc.OB.BootManager manager=
		com.ooc.OB.BootManagerHelper.narrow(
			orb.resolve_initial_references("BootManager"));
	byte[] id="pcitor48".getBytes();
	manager.add_binding(id,card);
	
	
	java.io.PrintWriter out = new java.io.PrintWriter(new java.io.FileOutputStream("ior.ref"));
 	out.println(orb.object_to_string(card));
 	out.close();
	System.out.println("ref file created");
	//start object 
	orb.run();
	}
catch(Exception ex) {ex.printStackTrace();} 
}

}
