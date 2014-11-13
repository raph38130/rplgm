/* $Id: SXCParser.java,v 1.3 2002/07/10 15:52:31 raph Exp $
 *
 * parse an .sxc file (StarOffice Calc spreadsheet)
 * 	can't  find office.dtd even when not validating
 *	"Relative URI "office.dtd"; can not be resolved without a base URI."
 *	-> add 2nd arg to parse()
 *	pb between dtd 1.5 so52 et 1.6 so6...
 *
 * Mon Mar 18 19:00:45 CET 2002 RP
 *	- initial revision
 *
 */
import java.io.*;
import java.util.zip.*;

import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.*;

class SXCParser
{
static int N=0;
Float note_f=new Float(0),note_m=new Float(0);

SXCParser()
{
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setValidating(true);
        dbf.setIgnoringComments(true);
        dbf.setIgnoringElementContentWhitespace(true);
        //dbf.setCoalescing(false);
        //dbf.setExpandEntityReferences(false);
        DocumentBuilder db = null;
        try {
            	db = dbf.newDocumentBuilder();
        	Document doc = null;
 	    	InputStream istr = new ZipFile("essai1.sxc").getInputStream(new ZipEntry("content.xml"));
	    	doc = db.parse(istr,"file:///home/raph/DOM/SXC/nulldtd/office.dtd");//,
			//"file:///opt2/OpenOffice.org641/share/dtd/officedocument/1_0/");
			//so52 DTD are ok ...
			//"file:///home/raph/DOM/SXC/5.2/office.dtd");
			//"file:///home/raph/OpenOffice.org1.0/share/dtd/officedocument/1_0/office.dtd");

		//sauvegarde du document produit
		FileWriter f1=new FileWriter("save.xml");
		new org.apache.soap.util.xml.DOM2Writer().serializeAsXML(doc,f1);
		f1.close();
	
		//sauvegarde au format .sxc (zipped)
		ZipOutputStream zostr = new ZipOutputStream(new FileOutputStream("essai2.sxc"));
		zostr.putNextEntry(new ZipEntry("content.xml"));
		OutputStreamWriter osw=new OutputStreamWriter(zostr);
		new org.apache.soap.util.xml.DOM2Writer().serializeAsXML(doc,osw);
		osw.close();
		
  		//recurse doc
		DisplayNotes(doc);
        } catch (ParserConfigurationException pce) {
            System.err.println(pce);
            System.exit(1);
        } catch (org.xml.sax.SAXException se) {
            System.err.println(se.getMessage());
            System.exit(1);
        } catch (IOException ioe) {
            System.err.println(ioe);
            System.exit(1);
        }
}
private void DisplayNotes(Node n)
{
if (n==null) return;
//System.out.println("NodeName="+n.getNodeName());
if (n.getNodeName().equals("table:table-cell"))
if (n.hasAttributes() && (n.getAttributes().getNamedItem("table:value")!=null)) {
	n.appendChild(n.cloneNode(true));
	N = (N+1) % 3;
	if (N==1) {
		note_f=new Float(n.getAttributes().getNamedItem("table:value").getNodeValue());
		}
	if (N==2) note_m=new Float(n.getAttributes().getNamedItem("table:value").getNodeValue());
	if (N==0) {
		System.out.println("français "+note_f+" math "
					+note_m+" moyenne "+ (note_f.floatValue()+note_m.floatValue())/2);
		//add a new node there with line average
		
		}		
	}
//recurse on each child node
NodeList nl = n.getChildNodes();
for(int i=0;i<nl.getLength();i++)
	DisplayNotes(nl.item(i));
}


static public void main(String[] s)
  {
  System.out.println("$Id: SXCParser.java,v 1.3 2002/07/10 15:52:31 raph Exp $");
  new SXCParser();
  System.out.println("end...");
  }
}

