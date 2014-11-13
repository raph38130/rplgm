import java.applet.*;
import java.awt.*;
import java.awt.event.*;
/**
 * AppletConsole ouvre un terminal VT100 dans le navigateur
 * qui permet d'acceder au port COM d'une machine distante
 * (utiliser avec ConsoleServer)
 *
 *
 * code généré avec VisualAge Java (IBM)
 * Creation date: (02/11/00 17:45:25)
 * @author: RP
 */
public class AppletConsole extends Applet implements KeyListener, Runnable {
	Thread thread;
	private TextArea ivjTextArea1 = null;
	java.net.Socket s1,s2;
	private Label ivjLabel1 = null;
	private Label ivjLabel2 = null;
/**
 * Returns information about this applet.
 * @return a string of information about this applet
 */
public String getAppletInfo() {
	return "AppletConsole V2.3\n" + 
		"\n" + 
		"Insert the type's description here.\n" + 
		"Creation date: (02/11/00 17:45:21)\n" + 
		"@author: \n" + 
		"";
}
/**
 * Return the Label1 property value.
 * @return java.awt.Label
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private java.awt.Label getLabel1() {
	if (ivjLabel1 == null) {
		try {
			ivjLabel1 = new java.awt.Label();
			ivjLabel1.setName("Label1");
			ivjLabel1.setFont(new java.awt.Font("dialog", 2, 12));
			ivjLabel1.setText("Attention : port COM choisi par le serveur sur TacoBox");
			ivjLabel1.setBounds(91, 291, 340, 21);
			// user code begin {1}
			// user code end
		} catch (java.lang.Throwable ivjExc) {
			// user code begin {2}
			// user code end
			handleException(ivjExc);
		}
	}
	return ivjLabel1;
}
/**
 * Return the Label2 property value.
 * @return java.awt.Label
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private java.awt.Label getLabel2() {
	if (ivjLabel2 == null) {
		try {
			ivjLabel2 = new java.awt.Label();
			ivjLabel2.setName("Label2");
			ivjLabel2.setText("");
			ivjLabel2.setBounds(4, 342, 593, 21);
			ivjLabel2.setForeground(java.awt.Color.red);
			// user code begin {1}
			// user code end
		} catch (java.lang.Throwable ivjExc) {
			// user code begin {2}
			// user code end
			handleException(ivjExc);
		}
	}
	return ivjLabel2;
}
/**
 * Return the TextArea1 property value.
 * @return java.awt.TextArea
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private java.awt.TextArea getTextArea1() {
	if (ivjTextArea1 == null) {
		try {
			ivjTextArea1 = new java.awt.TextArea();
			ivjTextArea1.setName("TextArea1");
			ivjTextArea1.setBounds(13, 16, 555, 268);
			// user code begin {1}
			ivjTextArea1.addKeyListener(this);
			// user code end
		} catch (java.lang.Throwable ivjExc) {
			// user code begin {2}
			// user code end
			handleException(ivjExc);
		}
	}
	return ivjTextArea1;
}
/**
 * Called whenever the part throws an exception.
 * @param exception java.lang.Throwable
 */
private void handleException(java.lang.Throwable exception) {

	/* Uncomment the following lines to print uncaught exceptions to stdout */
	// System.out.println("--------- UNCAUGHT EXCEPTION ---------");
	// exception.printStackTrace(System.out);
}
/**
 * Initializes the applet.
 */
public void init() {
	try {
		super.init();
		setName("AppletConsole");
		setLayout(null);
		setSize(608, 381);
		add(getTextArea1(), getTextArea1().getName());
		add(getLabel1(), getLabel1().getName());
		add(getLabel2(), getLabel2().getName());
		// user code begin {1}
		// user code end
	} catch (java.lang.Throwable ivjExc) {
		// user code begin {2}
		// user code end
		handleException(ivjExc);
	}
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
	try {
		s2.getOutputStream().write(e.getKeyChar());
		s2.getOutputStream().flush();
	} catch (java.io.IOException e3) {
		System.out.println("erreur ecriture COM" + e3);
	}
}
/**
 * main entrypoint - starts the part when it is run as an application
 * @param args java.lang.String[]
 */
public static void main(java.lang.String[] args) {
	try {
		Frame frame = new java.awt.Frame();
		AppletConsole aAppletConsole;
		Class iiCls = Class.forName("AppletConsole");
		ClassLoader iiClsLoader = iiCls.getClassLoader();
		aAppletConsole = (AppletConsole)java.beans.Beans.instantiate(iiClsLoader,"AppletConsole");
		frame.add("Center", aAppletConsole);
		frame.setSize(aAppletConsole.getSize());
		frame.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent e) {
				System.exit(0);
			};
		});
		frame.setVisible(true);
	} catch (Throwable exception) {
		System.err.println("Exception occurred in main() of java.applet.Applet");
		exception.printStackTrace(System.out);
	}
}
/**
 * Paints the applet.
 * If the applet does not need to be painted (e.g. if it is only a container for other
 * awt components) then this method can be safely removed.
 * 
 * @param g  the specified Graphics window
 * @see #update
 */
public void paint(Graphics g) {
	super.paint(g);

	// insert code to paint the applet here
}
/**
 * Contains the thread execution loop.
 */
public void run() {
	byte[] buf = new byte[80];
	int c;
	try {
		s1 = new java.net.Socket(getCodeBase().getHost(), 3333);
		getLabel2().setText("Connexion avec TacoBox : " + s1.toString());
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
			getTextArea1().append(new String(buf, 0, c));
		}
	} catch (java.io.IOException e2) {
		System.out.println("erreur lecture COM" + e2);
	}
}
/**
 * Starts up the thread.
 */
public void start() {
	if (thread == null){
		thread = new Thread(this);
		thread.start();
	}
}
/**
 * Terminates the thread and leaves it for garbage collection.
 */
public void stop() {
	if (thread != null) {
		thread.stop();
		thread = null;
	}
	try {
		if (s1 != null) {
			s1.close();
			s1 = null;
		}
		if (s2 != null) {
			s2.close();
			s2 = null;
		}
	} catch (java.io.IOException e) {
		System.out.println(e);
	}
}
}
