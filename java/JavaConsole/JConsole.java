/* RP Sat Feb  5 19:18:08 CET 2000
 *
 * Application Java Emulateur de Console 
 * 	version compl�te avec Thread
 *
 * 11/02/00 gestion du CR (pour 162bug)
 * 05/02/00 version initiale
 *
 */


import java.awt.*;
import java.awt.event.*;


public class JConsole extends Frame
		      implements Runnable, KeyListener
{
Thread thread;
TextArea t;
java.io.FileReader f1;
java.io.FileWriter f2;

//constructeur de l'application java JConsole
JConsole()
{
//disposition des composants graphiques dans la "Frame"
setLayout(new GridBagLayout());
setSize(600,300);
setTitle("JConsole V2.1");
t=new TextArea(15,80);
t.addKeyListener(this);
add(t);
show();


//initialisation de descripteur de fichiers
//pour acc�der au port COM en utilisant les "device descriptor" unix
try	{
	f1= new java.io.FileReader("/dev/ttyS2");
	}
catch(java.io.IOException e)
	{System.out.println("erreur ouverture COM en lecture" + e);}
try	{
	f2= new java.io.FileWriter("/dev/ttyS2");
	}
catch(java.io.IOException e)
	{System.out.println("erreur ouverture COM en �criture" + e);}


//cr�ation et d�marrage du thread
Thread thread = new Thread(this);
thread.start();
}


//m�thode invoqu�e lors du d�marage de l'application
public static void main(String [] arg)
{
JConsole c = new JConsole();
}



//m�thode impos�e par l'interface Runnable
//ex�cut�e lors de l'activation du thread
public void run()
{
char[] buf =new char[80];
int c;

System.out.println("running");
try	{
	while(true) {
		c=f1.read(buf);
		for (int i=0;i<c;i++)
			if (buf[i]==0xd) buf[i]=' ';
		t.append(new String(buf,0,c));
		}	
	}
catch(java.io.IOException e2)
	{System.out.println("erreur lecture COM" + e2);}
}


//m�thodes pour intercepter les �v�nements clavier
//l'interface KeyListener impose trois m�thodes qu'il faut d�finir
//m�me si une seule est utilis�e
public void keyTyped(KeyEvent e)
{
try	{
	if (e.getKeyChar()!=0xa ) 
		f2.write(e.getKeyChar());
	else	{
		f2.write(0xd);
		f2.flush();	
		}
	}
catch(java.io.IOException e3)
		{System.out.println("erreur ecriture COM" + e3);}
}

public void keyPressed(KeyEvent e){}
public void keyReleased(KeyEvent e){}

}//fin de la classe JConsole
