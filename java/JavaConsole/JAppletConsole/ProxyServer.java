/* ProxyServer.java
 *
 * Serveur de Relai (socket/tty) pour l'applet AppletConsole
 *
 *	
 * Tue Mar 26 22:43:01 CET 2002 RP
 *	-flush after each char
 *
 * 02/12/00 11:40:39 RP
 * 	-initial revision
 */
class ProxyServer implements Runnable {
	static String COM="/dev/ttyS0";
/**
 * ConsoleServer constructor comment.
 */
public ProxyServer() {
	//super();
	new Thread(this).start();
}
/**
 * Starts the application.
 * @param args an array of command-line arguments
 */
public static void main(java.lang.String[] args) {
	// Insert code to start the application here.
	java.net.Socket s;
	java.io.FileReader f;
	int c;
	new ProxyServer();
	System.out.println("ReadServer : en attente sur le port 3333");
	try {
		s = new java.net.ServerSocket(3333).accept();
		System.out.println("Connexion etabie avec " + s);
		f = new java.io.FileReader(COM);
		while (true) {
			c = f.read();
			s.getOutputStream().write(c);
			s.getOutputStream().flush();
		}
	} catch (java.io.IOException e) {
		System.out.println("erreur dans ReadServer " + e);
	}
}
/**
 * run method comment.
 */
public void run() {
	java.net.Socket s;
	java.io.FileWriter f;
	int c;
	System.out.println("WriteServer : en attente sur le port 4444");
	try {
		s = new java.net.ServerSocket(4444).accept();
		System.out.println("connexion etablie avec " + s.toString());
		f = new java.io.FileWriter(COM);
		while (true) {
			c = s.getInputStream().read();
			//MVME162,OS9... font un echo des commandes utilisateur
			//on fait une transmission par paquet apres chaque CR pour que ce ne soit pas genant
			if (c == 0xA) {
				f.write(0xD);
			} else
				f.write(c);
				f.flush();
		}
	} catch (java.io.IOException e) {
		System.out.println("erreur dans WriteServer " + e);
	}
}
}
