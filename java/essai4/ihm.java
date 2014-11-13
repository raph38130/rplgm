/*
 * RP Wed Mar 10 20:58:49 MET 1999
 *
 * utilisation de flush pour forcer le rechargement de l'image
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
	Button b1,b2,b3,b4;
	public static Image vue;
	
	public ihm() {
	add(b1=new Button("image"));
	add(b2=new Button("close"));
	add(b3=new Button("flush"));
	add(b4=new Button("getimage"));
	
	v=new Visu();

	b1.addActionListener(this);
	b2.addActionListener(this);
	b3.addActionListener(this);
	b4.addActionListener(this);
	}
	
	
	
	public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand()=="image") {
		v.setVisible(true);
		}
        if (e.getActionCommand()=="close") {
		v.setVisible(false);
		vue.flush();
		}
        if (e.getActionCommand()=="flush") {
		vue.flush();
		}
	if (e.getActionCommand()=="getimage") {
		try {
   		vue = getImage(new java.net.URL("http://localhost/image.jpg"));
		}
		catch (IOException ev) {System.out.println("bad url");}
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
	
	public void paint(Graphics g) {
	if (ihm.vue != null) g.drawImage(ihm.vue,0,0,this);
	}
}
