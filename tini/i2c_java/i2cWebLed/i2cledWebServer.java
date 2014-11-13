import com.dalsemi.tininet.http.HTTPServer;
import com.dalsemi.tininet.http.HTTPServerException;

/**
 * This is a simple example of a HTTP server using a form post
 */

class i2cledWebServer
{
    static Object lock;

      /**
       * Main method of our web server
       */
      public static void main(String[] args)
      {
      	// Create a new HTTPServer on port 80
          HTTPServer httpd = new HTTPServer(80);
        // Set /html as the default directory for HTML files
          httpd.setHTTPRoot( "/html" );
        // Set index.html as the index page, sent after a GET request
          httpd.setIndexPage( "index.html" );
        // Set the Log file name to be log.txt
          httpd.setLogFilename( "log.txt" );
        // Enable loggin for our HTTP Server
          httpd.setLogging( true );

        // Tell user that we are ready to go
          System.out.println("TINI server now servicing Requests...");

        // Handle HTTP Connections forever
          while (true)
          {
              try
              { httpd.serviceRequests(); }
              catch ( HTTPServerException e )
              { 
                  System.out.println( "Error occured while servicing requests: " + e.getMessage() );
              }
          }
      }
}
