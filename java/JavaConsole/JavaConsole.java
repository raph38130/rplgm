/* RP Sun Jan 30 21:39:58 CET 2000
 *
 * Application Java Emulateur de Console 
 * 	version initiale pour élèves
 */

import java.awt.*;
import java.awt.event.*;

public class JavaConsole extends Frame implements ActionListener
{
Label l;
TextArea t;
Button b;
java.io.FileReader f;
char[] buffer;
int c;
//constructeur
JavaConsole()
{
//disposition des composants graphiques
b=new Button("LIRE");
t=new TextArea(10,80);
l=new Label("Interface RS232");
b.addActionListener(this);
setLayout(new GridBagLayout());
setSize(600,200);
setTitle("Console V1.0");
add(b); //raccourci de this.add(b)
add(t);
add(l);
show();
//pour accéder au port COM
try	{
	f= new java.io.FileReader("/dev/ttyS2");
	}
catch(java.io.IOException e)
	{System.out.println("erreur ouverture f" + e);}
buffer=new char[80];
}

//methode invoquée lors du démarage de l'application
public static void main(String [] arg)
{
System.out.println("start");
JavaConsole c = new JavaConsole();
}

public void actionPerformed(ActionEvent e)
{
StringBuffer s = new StringBuffer("hello");

if ("LIRE".equals(e.getActionCommand())) {
	System.out.println("en attente de lecture");
        b.setEnabled(false);
	try	{
		while((c=f.read(buffer))>1)
			t.append(new String(buffer,0,c));
		}
	catch(java.io.IOException e2)
		{System.out.println("erreur lecture f" + e2);}
	b.setEnabled(true);
	}
}

}
