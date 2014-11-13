/* RelaySMTP.java
 *
 * projet ESRF/LGM
 *
 * relai SOAP/SMTP vers SOAP/HTTP server
 *
 * RP Sun Jan 28 18:25:33 CET 2001
 *	- version initiale 
 * 
 */
public class RelaySMTP 
{
public static void main(String[] args) throws Exception 
{
System.out.println("Relay v0.0");
try {
	new org.apache.soap.server.SMTP2HTTPBridge(
			10000,
			"localhost","soapreq","xxx",
			new java.net.URL("http://mercure:8080/soap/servlet/rpcrouter"),
			"localhost"
			).run();
    } catch (java.io.IOException e) {System.out.println(e);}
}
}
