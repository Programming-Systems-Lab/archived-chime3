 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class SimpleDialog extends JFrame {

    /**
     * This creates a simple dialog which we used for
     * demo purposes
     **/
    public SimpleDialog(String title, String message) {
	super(title);
	getContentPane().setLayout(new BorderLayout());
	JScrollPane pane = new JScrollPane();

	//pane.createVerticalScrollBar();
	//pane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
	JViewport jv = pane.getViewport();
	JTextArea text = new JTextArea(message);


	text.setEditable(false);
	jv.setView(text);
	getContentPane().add(pane, BorderLayout.CENTER);
	setSize(500,600);
	this.toFront();
	this.setLocation(300,50);
	setVisible(true);

	this.addWindowListener(new WindowAdapter() {
		public void windowClosing (WindowEvent e) {
			System.exit(0);
		}
	});



    }

    public static  void main(String args[]) {
	new SimpleDialog("FRAXSmartevent", "Pardonnez moi\nkjghskjdghdsgdsg\ndskjgkdslghkldsfj");
    }
}
