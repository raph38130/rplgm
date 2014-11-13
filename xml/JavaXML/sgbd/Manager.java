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

/*import org.apache.soap.*;
import org.apache.soap.encoding.*;
import org.apache.soap.encoding.soapenc.*;
import org.apache.soap.rpc.*;
import org.apache.soap.util.net.*;
import org.apache.soap.util.xml.*;
*/
import java.util.*;
import java.io.*;

import javax.xml.parsers.*;
import org.w3c.dom.*;



class Manager
{
static LinkedList l;
static TreeSet	tree;
static private void DOMRecurse(Node n)
{
	if (n==null) return;
	if(n.getNodeName().equals("student")) {
		String s1=n.getFirstChild().getFirstChild().getNodeValue(),
		s2=n.getFirstChild().getNextSibling().getFirstChild().getNodeValue(),
		s3=n.getFirstChild().getNextSibling().getNextSibling().getAttributes().item(0).getNodeValue();
		l.add(new Eleve(s1,s2,new Float(s3).floatValue()));
		return ;
		}
	NodeList nl = n.getChildNodes();
	for(int i=0;i<nl.getLength();i++)
		DOMRecurse(nl.item(i));
}

static public void main(String[]param)
{
	System.out.println("sgbd v0.0");
	
	tree=new TreeSet();
	TreeSet tree2=null;
				
	/* Collections : build an ordered treeset */
	tree.add(new Eleve("bbb","dd",(float)10.0));
	tree.add(new Eleve("aaa","dd",(float)10.0));
	tree.add(new Eleve("xyz","dd",(float)05.0));
	tree.add(new Eleve("bbb","ccc",(float)10.0));
	tree.add(new Eleve("bbb","aaa",(float)10.0));
	tree.add(new Eleve("bbb","ccc",(float)08.0));
	tree.add(new Eleve("ttt","ccc",(float)08.0));
	
	/* AbstractCollection.toString() */
	System.out.println("initial tree is : "+tree.toString());
	
	/* iterator */
	Iterator i=tree.iterator();
	while(i.hasNext()) {
		Eleve e=(Eleve)i.next();
		if (e.value<10) System.out.println("<10 : "+e.toString());
		}	
		
	/* serialization */
	try	{
		ObjectOutputStream oos = new ObjectOutputStream(
							new FileOutputStream("serialised.data")
										);
		oos.writeObject(tree);
		ObjectInputStream ois = new ObjectInputStream(
							new FileInputStream("serialised.data")
										);
		tree2=(TreeSet) ois.readObject();
		}
	catch(IOException e1){System.out.println("serialisation error"+e1);}
	catch(ClassNotFoundException e2){System.out.println("deserialisation error"+e2);}
	System.out.println("deserialized tree is : "+tree2.toString());

	/* Collection to XML "by hand" */
	try	{
		PrintWriter f1=new PrintWriter(
						new FileOutputStream("data.xml"));
		f1.println("<?xml version=\"1.0\" encoding=\"us-ascii\"?>");
		Iterator it=tree.iterator();
		f1.println("<school>");
		while(it.hasNext()) {
			Eleve e=(Eleve)it.next();
			f1.println("\t"+e.toXML());
			}
		f1.println("</school>");
		f1.close();
		}
	catch(IOException e3){System.out.println("XML error"+e3);}
	
	/* XML to DOM */
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        DocumentBuilder db = null;
        try {
            db = dbf.newDocumentBuilder();
        } catch (ParserConfigurationException pce) {
            System.err.println(pce);
            System.exit(1);
        }
        Document doc = null;
        try {
            doc = db.parse("data.xml");
        } catch (org.xml.sax.SAXException se) {
            System.err.println(se.getMessage());
            System.exit(1);
        } catch (IOException ioe) {
            System.err.println(ioe);
            System.exit(1);
        }
	System.out.println("DOM representation of data.xml is "+org.apache.soap.util.xml.DOMWriter.nodeToString(doc));
	
	/* search through DOM */
	NodeList nl = doc.getElementsByTagName ("average");
	for(int k=0;k<nl.getLength();k++) 
		System.out.println(((Element)nl.item(k)).getAttribute("math"));

	/*DOM to List */
	l=new LinkedList();
	DOMRecurse(doc);
	System.out.println("deDOMized list is : "+l.toString());
	
	/* java.beans.XMLEncoder */
	try	{
		Eleve z=new Eleve("bbb","dd",(float)10.0);
		java.beans.XMLEncoder e = new java.beans.XMLEncoder(
				new BufferedOutputStream(new FileOutputStream("beans.xml")));
		Eleve el=new Eleve();
		el.set("bbb","dd",(float)10.0);
		e.writeObject(el);
		//e.writeObject(new java.util.Date());
		//LinkedList l=new LinkedList();
		TreeSet l=new TreeSet();
		//Hashtable l=new Hashtable();
		l.add("aaa");
		l.add("bbb");
		if (l.isEmpty())System.out.println("empty treeset"); 
		e.writeObject(l);
		/*Eleve x=new Eleve("bbb","dd",(float)10.0);
		Eleve y=(Eleve)x.getClass().newInstance();
		System.out.println("x "+x.toString()
				 +" y "+y.toString());*/
		e.close();
		}
	catch(IOException e4){System.out.println("XMLEncoder error"+e4);}
	//catch(InstantiationException ie){System.out.println("ie error"+ie);}
	//catch(IllegalAccessException iae){System.out.println("iae error"+iae);}
	
	
	System.out.println("premier "+l.getFirst().toString()
			+" dernier "+l.getLast().toString());
	
	
	
	/* Collection to XML via SOAP */
	/*String s=new String("hello");
	org.apache.soap.util.Bean bean=new org.apache.soap.util.Bean(
						TreeSet.class,tree);
	
	org.apache.soap.rpc.Parameter p1=new org.apache.soap.rpc.Parameter(
				"astring",
				String.class,
				s,
				org.apache.soap.Constants.NS_URI_SOAP_ENC);
	System.out.println("p1 = "+p1);
	org.apache.soap.rpc.Parameter p2=new org.apache.soap.rpc.Parameter(
				"mytreeset",
				TreeSet.class,
				tree,
				org.apache.soap.Constants.NS_URI_SOAP_ENC);
	System.out.println("p2 = "+p2);
	org.apache.soap.rpc.Parameter p3=new org.apache.soap.rpc.Parameter(
				"mydate",
				Date.class,
				new Date(),
				org.apache.soap.Constants.NS_URI_SOAP_ENC);
	
	Call call = new Call();
	call.setTargetObjectURI("urn:Soap2TangoServer");
	call.setEncodingStyleURI(Constants.NS_URI_SOAP_ENC);
	call.setMethodName("method_name");
	SOAPMappingRegistry smr=new SOAPMappingRegistry();
	BeanSerializer bs=new BeanSerializer();
	DateSerializer ds= new DateSerializer();
	CalendarSerializer cs= new CalendarSerializer();
	smr.mapTypes(
		Constants.NS_URI_SOAP_ENC,
                new QName("mydate", "mydate"),
               Date.class,
		cs, cs);
	Vector params = new Vector();
	params.addElement(p1);	
	//params.addElement(p2);	
	params.addElement(p3);	
	call.setParams(params);
	try {
	call.invoke(new java.net.URL("http://localhost:8000"), "");
	}
	catch(IOException e4){System.out.println("SOAP error"+e4);}
	catch(SOAPException e5){System.out.println("SOAP error"+e5);}
	*/

	System.out.println("bye...");
}
}
