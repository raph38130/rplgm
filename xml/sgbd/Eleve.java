/* Manager.java
 *
 * SGBD demo 
 *
 * Thu Jan 31 10:47:41 CET 2002 RP
 *	- initial revision with :
 *		Collection
 *		serialization
 *		XML
 *		SOAP (todo)
 */
package sgbd;

import java.util.*;
import java.io.*; 


public class Eleve implements	Comparable,
				Serializable
{
String	lname,
	fname;
float 	value;

public Eleve()
{
lname="xx";fname="yy";value=0;
}
public Eleve(String s1, String s2, float f1)
{
	lname=s1; fname=s2;value=f1;
}
public String toString()
{
	return "name : "+lname+" "+fname+" average : "+value;
}
public String get()
{
	return "name : "+lname+" "+fname+" average : "+value;
}
public void set(String s1, String s2, float f1)
{
	lname=s1; fname=s2;value=f1;
}

public String toXML()
{
	return "<student><name>"+lname+"</name>"+"<fname>"+fname+"</fname>"
		+"<average math=\""+value+"\"/></student>";
}
public int compareTo(Object o)
{
	int r;
	Eleve e=(Eleve) o;
	r=lname.compareTo(e.lname);
	if (r!=0) return r;
	r=fname.compareTo(e.fname);
	if (r!=0) return r;
	r=new Float(value).compareTo(new Float(e.value));
	return r;
}
}


