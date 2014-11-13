/* WDemo.java
 *
 * mer oct  5 11:09:10 CEST 2005 RP
 *	- version graphique
 */


import javax.swing.*;

class WDemo
{
private JTextArea ta;

WDemo()
{
JFrame f = new JFrame("WDemo");
JButton b1 = new JButton("clic");
JButton b2 = new JButton("reset");
ta = new JTextArea();

b1.setBounds(50,300,75,50);

b1.addActionListener(new java.awt.event.ActionListener ()
			{
			public void actionPerformed(java.awt.event.ActionEvent e)
			{
			ta.append("klok");
			}			
			}
		     );

b2.setBounds(150,300,75,50);
b2.addActionListener(new java.awt.event.ActionListener ()
			{
			public void actionPerformed(java.awt.event.ActionEvent e)
			{
			ta.setText("");
			}			
			}
			);

ta.setBounds(50,50,200,250);

f.setLayout(null);
f.add(b1);f.add(b2);f.add(ta);
f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
f.setSize(300,400);
f.setVisible(true);
}



public static void main(String[] s)
{
System.out.println("hello apsii WDemo v1.1");
new WDemo();
System.out.println("bye...");
}

}
