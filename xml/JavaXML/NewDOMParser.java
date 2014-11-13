/* NewDOMParser.java
 *
 * parse an xml file
 *
 * RP Tue Nov 20 23:05:11 CET 2001
 *	- jaxp implementation (now included in j2sdk)
 *	- initial revision
 *
 */
import javax.xml.parsers.*;
import org.w3c.dom.*;
import java.io.*;

class NewDOMParser
{


NewDOMParser()
{
        // Step 1: create a DocumentBuilderFactory and configure it
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();

        // Optional: set various configuration options
        dbf.setValidating(false);
        dbf.setIgnoringComments(false);
        dbf.setIgnoringElementContentWhitespace(false);
        dbf.setCoalescing(true);
        dbf.setExpandEntityReferences(true);

        // At this point the DocumentBuilderFactory instance can be saved
        // and reused to create any number of DocumentBuilder instances
        // with the same configuration options.

        // Step 2: create a DocumentBuilder that satisfies the constraints
        // specified by the DocumentBuilderFactory
        DocumentBuilder db = null;
        try {
            db = dbf.newDocumentBuilder();
        } catch (ParserConfigurationException pce) {
            System.err.println(pce);
            System.exit(1);
        }

        // Set an ErrorHandler before parsing
        /*try {
        OutputStreamWriter errorWriter =
            new OutputStreamWriter(System.err, "UTF-8");
            db.setErrorHandler(new MyErrorHandler(new PrintWriter(errorWriter, true)));
        } catch (IOException e) {
            System.err.println(e);
            System.exit(1);
        }*/

        // Step 3: parse the input file
        Document doc = null;
        try {
            doc = db.parse("notes.xml");
        } catch (org.xml.sax.SAXException se) {
            System.err.println(se.getMessage());
            System.exit(1);
        } catch (IOException ioe) {
            System.err.println(ioe);
            System.exit(1);
        }
  	// the proof
	DOMDisplay(doc);
}
//recusively dispay each node
private void DOMDisplay(Node n)
{
if (n==null) return;
System.out.println("NodeName="+n.getNodeName()+" NodeValue="+n.getNodeValue());
NodeList nl = n.getChildNodes();
for(int i=0;i<nl.getLength();i++)
	DOMDisplay(nl.item(i));
}

/*private static class MyErrorHandler implements org.xml.sax.ErrorHandler {
        /
        private PrintWriter out;

        MyErrorHandler(PrintWriter out) {
            this.out = out;
        }
	public void warning(org.xml.sax.SAXParseException e){System.out.println("warning");}
	public void error(org.xml.sax.SAXParseException e){System.out.println("error");}
	public void fatalError(org.xml.sax.SAXParseException e){System.out.println("fatalerror");}
}*/

static public void main(String[] s)
  {
  System.out.println("NewDOMParser V0.1.0");
  
  new NewDOMParser();
  
  System.out.println("NewDOMParser end...");
  }
}

