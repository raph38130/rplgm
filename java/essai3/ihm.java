/*
 * RP Wed Mar 10 20:58:49 MET 1999
 *
 *
 *
 */
import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import java.io.*;
import java.awt.image.*;


public class ihm 
	extends Applet
	implements ActionListener
	{
	Visu v;
	Button b1,b2;
	
	public ihm() {
	
	add(b1=new Button("image"));
	add(b2=new Button("close"));
	
	v=new Visu();

	b1.addActionListener(this);
	b2.addActionListener(this);
	} 
	
	public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand() == "image") {
		v.setVisible(true);
		}
        if (e.getActionCommand() == "close") {
		v.setVisible(false);
		try {
		System.out.println("closing");
		getAppletContext().showDocument(new java.net.URL("http://localhost"));
		System.out.println("closed");
		    }
		catch(IOException E1) {System.out.println(E1);}
		}		
	}
	
	public void windowClosed(WindowEvent e) {
	System.out.println("exit");
	}
}

class Visu extends Frame {
	Visu() {
	setSize(200,200);
	}
}
