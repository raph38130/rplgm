package lgm;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;

public class ChServer {
	private Socket s1 = null, // sens client vers le serveur
			s2 = null; // sens inverse

	private ServerSocket serv = null;
	private Vector participants; // la liste des utilisateurs connectés
    //private int cpt=0;

	// pour chaque participant
	class Dispatch implements Runnable {
		private Socket s = null, sl2 = null;
		private byte[] buffer = new byte[500];
		private int n, p;

		Dispatch(Socket ns) {
			s = ns;
		}

		public void run() {
			try {
				while (true) {
					// attendre un message
					n = s.getInputStream().read(buffer);
					//System.out.println(cpt++ + "thread "+ this.toString() + " " + participants.size());
					// le répéter à tous les participants
					for (int i = 0; i < participants.size(); i += 2) {
						p = participants.indexOf(s);
						if (p != i) { // on ne réemet pas au même
							sl2 = (Socket) participants.get(i + 1);
							// ajouter l'origine du message
							sl2.getOutputStream().write(s.getInetAddress().toString().getBytes());
							if (n > 0) sl2.getOutputStream().write(buffer, 0, n);
						}
					}
				}
			} catch (IOException e) {
				// e.printStackTrace();
				participants.remove(s);
				participants.remove(sl2);
			}
		}
	}

	// pour les nouveaux participants
	class OpenSession implements Runnable {
		public void run() {
			while (true) {
				try {
					// la nouvelle socket crée pour le nouveau participant
					System.out.println("attente participant");
					s1 = serv.accept();
					// n° de port du client (variable pour tests monoposte
					s2 = new Socket(s1.getInetAddress(), 1234);
					// l'enregistrer dans la liste des participants
					participants.add(s1);
					participants.add(s2);
					// créer un nouveau thread pour ce participant
					new Thread(new Dispatch(s1)).start();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

	ChServer() {
		// créer thread à l'écoute des nouveaux arrivants
		try {
			serv = new ServerSocket(3007);
		} catch (IOException e) {
			e.printStackTrace();
		}
		participants = new Vector();
		new Thread(new OpenSession()).start();
	}

	public static void main(String[] args) {
		System.out.println("ChServer V1.0 IRC relay");
		new ChServer();
	}

}
