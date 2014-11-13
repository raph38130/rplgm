/*
 * 
 *
 */

package tini;

import com.dalsemi.system.BitPort;
import com.dalsemi.tininet.http.HTTPServer;
import com.dalsemi.tininet.http.HTTPServerException;

public class WebMotor {

	public static void main(String[] args) {
	        System.loadLibrary("timerT2.tlib");
		HTTPServer httpd = new HTTPServer(80);
		httpd.setHTTPRoot("/");
		httpd.setIndexPage("index.html");
		httpd.setLogging(false);
		System.out.println("TINI CC motor... V1.1");
		while (true) {
			try {
				httpd.serviceRequests();
			} catch (HTTPServerException e) {
				System.out.println("Error occured while servicing requests: "
						+ e.getMessage());
			}
		}
	}

}




