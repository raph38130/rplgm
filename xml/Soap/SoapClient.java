/* SoapClient.java
 *export CLASSPATH=/usr/local/soap-2_2/lib/soap.jar:/usr/local/soap-2_2/lib/mail.jar:/usr/local/soap-2_2/lib/activation.jar:.
 */


import java.io.*;
import java.net.*;
import java.util.*;
import org.apache.soap.*;
import org.apache.soap.rpc.*;

/**
 * See README for info.
 *
 * @author RP/LGM
 */
public class SoapClient {
  public static void main (String[] args) throws Exception {
    
    int n=123,
    	base=10;
    
    // Build the call.
    Call call = new Call ();
    call.setTargetObjectURI ("service1");
    call.setMethodName ("Remainder");
    call.setEncodingStyleURI(Constants.NS_URI_SOAP_ENC);
    Vector params = new Vector ();
    params.addElement (new Parameter("n", Integer.class, new Integer(n), Constants.NS_URI_SOAP_ENC));
    params.addElement (new Parameter("base", Integer.class, new Integer(base), Constants.NS_URI_SOAP_ENC));
    call.setParams (params);

    Response resp = call.invoke (/* router URL */ new URL("http://localhost:8080/soap/servlet/rpcrouter"), /* actionURI */ "" );

    // Check the response.
    if (resp.generatedFault ()) {
      Fault fault = resp.getFault ();

      System.err.println("Generated fault: " + fault);
    } else {
      Parameter result = resp.getReturnValue ();
      System.out.println (result.getValue ());
    }
  }
}
