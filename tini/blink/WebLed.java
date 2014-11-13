/*
 * 
 * 
 APPLI=WebLed
 JDK_HOME=/home/apsii/jdk1.5.0_05/
 TINI_HOME=/home/apsii/TP_TINI/tini1.17/

 ${JDK_HOME}/bin/java -cp ${TINI_HOME}/bin/tini.jar BuildDependency \
 -add HTTPSERVER -x ${TINI_HOME}/bin/owapi_dep.txt -p ${TINI_HOME}/bin/modules.jar \
 -d ${TINI_HOME}/bin/tini.db \
 -f ${APPLI}.class -f ActivateLed.class -o ${APPLI}.tini

 *
 *
 */

package tini;

import com.dalsemi.system.BitPort;
import com.dalsemi.tininet.http.HTTPServer;
import com.dalsemi.tininet.http.HTTPServerException;

public class WebLed {
	public static Thread t = null;
	public static boolean b = true;
	public static int duration = 500;
	private BitPort p = null;

	WebLed() {
		p = new BitPort(BitPort.Port5Bit2);
		t = new Thread(new Runnable() {
			public void run() {
				while (true) {
					try {
						p.set();
						Thread.sleep(duration);
						p.clear();
						Thread.sleep(duration);
					} catch (InterruptedException e) {
					}
				}
			}
		});
		t.start();
	}

	public static void main(String[] args) {
		new WebLed();
		HTTPServer httpd = new HTTPServer(8080);
		httpd.setHTTPRoot("/");
		httpd.setIndexPage("led.html");
		httpd.setLogging(false);
		System.out.println("TINI server now servicing Requests... V1.0");
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
