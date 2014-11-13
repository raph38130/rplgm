/* $Id: Client.java,v 1.1.1.1 2002/07/10 13:11:53 raph Exp $
 *
 * a CORBA client skeleton (java ooc edition)
 *
 * Fri Feb 22 12:00:25 CET 2002 RP
 *	- initial revision
 */


public class Client
{

public static void main(String args[])
{
java.util.Properties props=System.getProperties();
props.put("org.omg.CORBA.ORBClass","com.ooc.CORBA.ORB");
props.put("org.omg.CORBA.ORBSingletonClass","com.ooc.CORBA.ORBSingleton");

try	{
  	//init ORB
  	org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
	//get IOR from file
	//alternate solution is Naming
	/*org.omg.CORBA.Object obj = orb.string_to_object(
				       new java.io.BufferedReader(
					new java.io.FileReader(args[0])).readLine());
	*/				
	org.omg.CORBA.Object obj = orb.string_to_object("corbaloc::localhost:10000/pcitor48");
	
	//narrow = dynamic cast + type verif.
	pcitor48 card = pcitor48Helper.narrow(obj);
	//perform action on remote object
	card.SetLed((short)3);
	System.in.read();
	card.SetLed((short)4);
	System.in.read();
	card.SetLed((short)5);
	System.in.read();
	System.out.println("done...");
	System.exit(0);
	}
catch(Exception e){System.out.println("erreur"+e);}   
}
}
