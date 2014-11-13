/* Demo1.java
 *
 * Mon Dec  5 10:15:23 CET 2005 RP
 *	- version initiale
 */
 
class Demo1
{
	//constructeur de la classe
	Demo1(String s)
	{
	javax.swing.JFrame f = new javax.swing.JFrame("Demo "+ s);
	f.setSize(400,300);
	f.setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
	
	javax.swing.JTextArea ta = new javax.swing.JTextArea();
	ta.setBounds(80,40,300,150);
	
	javax.swing.JButton b1 = new javax.swing.JButton("OK");
	b1.setBounds(80,200,80,50);
	
	javax.swing.JButton b2 = new javax.swing.JButton("cancel");
	b2.setBounds(180,200,80,50);
	
	f.setLayout(null);
	f.add(ta);
	f.add(b1);
	f.add(b2);
	f.setVisible(true);
	}

	//point d'entrée du programme
	static public void main(String[] s)
	{
	System.out.println("hello iris1 v2.0...");
	new Demo1("v0");
	System.out.println("bye !");
	}

}
 

 
 
