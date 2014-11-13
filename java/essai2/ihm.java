/*
 * RP Wed Mar 10 20:58:49 MET 1999
 *
 * utilisation de flush pour forcer le rechargement de l'image
 */
import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import java.io.*;
import java.awt.image.*;


public class ihm 
	extends Applet
	implements AdjustmentListener, ActionListener
	{
	Scrollbar s1,s2,s3;
	TextArea t1;
	Visu v;
	Button b1,b2,b3;
	public static Image vue;
	//java.awt.Toolkit t;
	
	public ihm() {
	add(s1=new Scrollbar());
	add(s2=new Scrollbar());
	add(s3=new Scrollbar());
	add(t1=new TextArea());
	add(b1=new Button("image"));
	add(b2=new Button("close"));
	add(b3=new Button("reload"));
	
	v=new Visu();

	b1.addActionListener(this);
	b2.addActionListener(this);
	b3.addActionListener(this);
	s1.addAdjustmentListener(this);
	s2.addAdjustmentListener(this);
	s3.addAdjustmentListener(this);
	}
	
	
	public void adjustmentValueChanged(AdjustmentEvent e) {
	Adjustable s = e.getAdjustable();
	t1.append(Integer.toString(s.getValue())+"\n");
	} 
	
	public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand()=="image") {
		v.setVisible(true);
		}
        if (e.getActionCommand()=="close") {
		v.setVisible(false);
		vue.flush();
		}
	if (e.getActionCommand()=="reload") {
		try {
		System.out.println("getimage");
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
