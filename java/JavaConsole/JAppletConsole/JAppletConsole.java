/* JAppletConsole.java
 *
 * JAppletConsole ouvre un terminal VT100 dans le navigateur
 * qui permet d'acceder au port COM d'une machine distante
 * (utiliser avec ProxyServer)
 *
 *
 * 
 * Wed Mar 20 23:18:26 CET 2002 RP
 *	- swing edition
 *
 * 02/12/00 11:40:39 RP
 *	- version initiale
 */

import java.applet.*;
import javax.swing.*;
import java.awt.event.*;
import java.net.*;

public class JAppletConsole
	extends JApplet
	implements KeyListener, Runnable
{
Thread thread;
private JTextArea jta = null;
java.net.Socket s1,s2;
	
public JAppletConsole()
{
	getContentPane().add(new JScrollPane(jta=new JTextArea(10,10)));
	jta.addKeyListener(this);
}
/**
 * Initializes the applet.
 */
public void init() {
	new JAppletConsole();		
}
/**
 * Called when a key has been pressed.
 * @param e the received event
 */
public void keyPressed(KeyEvent e) {
}
/**
 * Called when a key has been released.
 * @param e the received event
 */
public void keyReleased(KeyEvent e) {	
}
/**
 * Called when a key has been typed.
 * @param e the received event
 */
public void keyTyped(KeyEvent e) {
	jta.setEditable(false);
	try {
		s2.getOutputStream().write(e.getKeyChar());
		s2.getOutputStream().flush();
	} catch (java.io.IOException e3) {
		System.out.println("erreur ecriture COM" + e3);
	}
}
/**
 * Contains the thread execution loop.
 */
public void run() {
	byte[] buf = new byte[80];
	int c;
	try {
		s1 = new java.net.Socket(getCodeBase().getHost(), 3333);
		System.out.println("Connexion avec  : " + s1.toString());
	} catch (java.io.IOException e) {
		System.out.println("erreur ouverture socket en lecture" + e);
	}
	try {
		s2 = new java.net.Socket(getCodeBase().getHost(), 4444);
	} catch (java.io.IOException e) {
		System.out.println("erreur ouverture socket en ecriture" + e);
	}
	try {
		while (true) {
			c = s1.getInputStream().read(buf);
			for (int i = 0; i < c; i++)
				if (buf[i] == 0xd)
					buf[i] = (byte) ' ';
			jta.setEditable(true);
			jta.append(new String(buf, 0, c));
			jta.setCaretPosition(jta.getText().length());
		}
	} catch (java.io.IOException e2) {
		System.out.println("erreur lecture COM" + e2);
	}
}
public void start() {
	if (thread == null){
		thread = new Thread(this);
		thread.start();
	}
}

}
