import java.io.IOException;
import java.net.*;
import javax.swing.JFrame;
/*
 * Created on 13 mai 2004
 * 
 * @author raph
 *  
 */
public class JChat extends JFrame implements Runnable {
	private static int chatPort;
	public static void main(String[] args) {
		if (args.length >= 1)
			chatPort = Integer.parseInt(args[0]);
		else
			chatPort = 3333;
		new JChat();
	}
	private javax.swing.JButton jButton = null;
	private javax.swing.JPanel jContentPane = null;
	private javax.swing.JTextField jTextField = null;
	private javax.swing.JTextField jTextField1 = null;
	private javax.swing.JTextArea jTextArea = null;
	private java.net.Socket s1, s2;
	private javax.swing.JTextField jTextField2 = null;
	private javax.swing.JScrollPane jScrollPane = null;
	/**
	 * This is the default constructor
	 */
	public JChat() {
		super();
		initialize();
		new Thread(this).start();
	}
	/**
	 * 
	 * This method initializes jButton
	 * 
	 * @return javax.swing.JButton
	 *  
	 */
	private javax.swing.JButton getJButton() {
		if (jButton == null) {
			jButton = new javax.swing.JButton();
			jButton.setBounds(340, 11, 96, 44);
			jButton.setText("Connect");
			jButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					try {
						s1 = new Socket(jTextField.getText(), Integer
								.parseInt(jTextField2.getText()));
					} catch (UnknownHostException e1) {
						e1.printStackTrace();
					} catch (IOException e2) {
						e2.printStackTrace();
					}
				}
			});
		}
		return jButton;
	}
	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new javax.swing.JPanel();
			jContentPane.setLayout(null);
			jContentPane.add(getJButton(), java.awt.BorderLayout.CENTER);
			jContentPane.add(getJTextField(), null);
			jContentPane.add(getJTextField1(), null);
			jContentPane.add(getJTextField2(), null);
			jContentPane.add(getJScrollPane(), null);
		}
		return jContentPane;
	}
	/**
	 * 
	 * This method initializes jTextField
	 * 
	 * @return javax.swing.JTextField
	 *  
	 */
	private javax.swing.JTextField getJTextField() {
		if (jTextField == null) {
			jTextField = new javax.swing.JTextField();
			jTextField.setBounds(41, 22, 90, 20);
			jTextField.setText("localhost");
		}
		return jTextField;
	}
	/**
	 * 
	 * This method initializes jTextField1
	 * 
	 * @return javax.swing.JTextField
	 *  
	 */
	private javax.swing.JTextField getJTextField1() {
		if (jTextField1 == null) {
			jTextField1 = new javax.swing.JTextField();
			jTextField1.setBounds(42, 296, 241, 20);
			jTextField1.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					jTextArea.append(jTextField1.getText() + "\n");
					try {
						s1.getOutputStream().write(
								jTextField1.getText().getBytes());
						s1.getOutputStream().flush();
					} catch (IOException e1) {
						e1.printStackTrace();
					}
					jTextField1.setText(null);
				}
			});
		}
		return jTextField1;
	}
	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
		this.setContentPane(getJContentPane());
		this.setTitle("JChat V0.1");
		this.setSize(481, 410);
		this.setVisible(true);
	}
	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
		try {
			s2 = new ServerSocket(chatPort).accept();
			byte[] buffer = new byte[10];
			int n;
			while (true) {
				n = s2.getInputStream().read(buffer);
				jTextArea.append(">>>>" + new String(buffer, 0, n) + "\n");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	/**
	 * 
	 * This method initializes jTextArea
	 * 
	 * @return javax.swing.JTextArea
	 *  
	 */
	private javax.swing.JTextArea getJTextArea() {
		if (jTextArea == null) {
			jTextArea = new javax.swing.JTextArea();
			jTextArea.setEditable(false);
			jTextArea.setCaretPosition(0);
		}
		return jTextArea;
	}
	/**
	 * 
	 * This method initializes jTextField2
	 * 
	 * @return javax.swing.JTextField
	 *  
	 */
	private javax.swing.JTextField getJTextField2() {
		if (jTextField2 == null) {
			jTextField2 = new javax.swing.JTextField();
			jTextField2.setBounds(158, 23, 35, 20);
			jTextField2.setText("3333");
		}
		return jTextField2;
	}
	/**
	 * 
	 * This method initializes jScrollPane
	 * 
	 * @return javax.swing.JScrollPane
	 *  
	 */
	private javax.swing.JScrollPane getJScrollPane() {
		if (jScrollPane == null) {
			jScrollPane = new javax.swing.JScrollPane();
			jScrollPane.setBounds(42, 66, 239, 202);
			jScrollPane.setViewportView(getJTextArea());
		}
		return jScrollPane;
	}
} //  @jve:visual-info decl-index=0 visual-constraint="10,10"
