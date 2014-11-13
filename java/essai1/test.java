/* RP 10 / 12 / 98
 *
 * applet 1.1
 * avec bouttons, un label
 * une frame de 2 bouttons et un label
 *
 */
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;

public class test extends Applet implements ActionListener
{
Label l,L;
Button b1,b2,b3,b4;
Frame f;

//methode invoquée par le navigateur a chaque démarage de l'application
public void start()
{
//dans le navigateur
l=new Label("                                                  ");
b1=new Button("START");
b1.addActionListener(this);
b2=new Button("CLEAR");
b2.addActionListener(this);
add(b1);
add(b2);
add(l);
//frame indépendante
f=new Frame("HELLO");
f.setSize(200,200);
b3=new Button("OK");
b3.addActionListener(this);
b4=new Button("GO!");
L=new Label("BONJOUR !");
f.setLayout(new GridLayout(2,2));
f.add(b3);
f.add(b4);
f.add(L);
}

public void actionPerformed(ActionEvent e)
{
StringBuffer s = new StringBuffer("hello");

if ("START".equals(e.getActionCommand())) {
        b1.setEnabled(false);
        b2.setEnabled(true);
	f.setVisible(true);
      for (int i=0; i<10; i++) 
            s.append(" "+i);
        l.setText(s.toString());
	}
if ("CLEAR".equals(e.getActionCommand())) {
        b2.setEnabled(false);
        b1.setEnabled(true);
        l.setText("                                          ");
	}
if ("OK".equals(e.getActionCommand())) {
        //message sur la console
	System.out.println("OK");
	f.setVisible(false);
	}

}

}
