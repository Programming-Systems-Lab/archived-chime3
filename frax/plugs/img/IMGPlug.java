 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.plugs.img;

import java.net.*;
import javax.swing.ImageIcon;
import psl.chime.frax.plugs.FRAXPlug;
import java.io.*;
import java.util.*;

public class IMGPlug extends FRAXPlug {

    ImageIcon image;
    String final_xml;

    /**
     * just a constructor to create an internal
     * representation of the object
     */
    public IMGPlug(String addon, DataInputStream is, Long length) {
	int len = length.intValue();
	byte data[] = new byte[len];

	try {
	    is.readFully(data);
	} catch (IOException e) {
	    System.err.println("An IOException has occurred");
	}

	image = new ImageIcon(data);
	final_xml = addon;
    }


    /**
     * not really used at the moment but maybe needed
     * in the future if we decide to get more from images
     * for now I am going to leave this in
     */
    private byte[] convertBytebyte(Vector temp) {
	int length = temp.size();
	byte[] data = new byte [length];


	Byte[] tempByte = (Byte[]) temp.toArray();

	for (int i = 0; i < length; i++) {
	    data[i] = tempByte[i].byteValue();
	}

	return data;
    }


    /**
     * a number of constructors which again show the
     * versatility of using classloaders
     */
    public IMGPlug(byte[] data) {
	image = new ImageIcon(data);
    }

    public IMGPlug(String addon, byte[] data) {
	final_xml = addon;
	image = new ImageIcon(data);
    }

    public IMGPlug() {
	image = new ImageIcon();
    }

    public IMGPlug(URL location) {
	image = new ImageIcon(location);
    }

    public IMGPlug(String addon, URL location) {
	final_xml = addon;
	image = new ImageIcon(location);
    }


    /**
     * add the information about the image which we are
     * trying to get. Return the XML
     */
    public String toXML() {
	final_xml = "<IMAGE>\n" + final_xml;
	final_xml = final_xml + "<WIDTH>" + image.getIconWidth() + "<WIDTH>\n";
	final_xml = final_xml + "<HEIGHT>" + image.getIconHeight() + "<HEIGHT>\n";
	final_xml = final_xml + "<IMAGE>\n";
	final_xml =  "<?xml version=\"1.0\"?>\n<MetaInfo createDate=\'" + System.currentTimeMillis() + "\' type = \'image\'>\n" + final_xml + "</MetaInfo>\n\n";
;
	return final_xml;
    }

    /**
     * Test of the image plug
     */
    public static void main(String args[]) {
	try {
	    IMGPlug ip = new IMGPlug(new URL("http://www.cs.brandeis.edu/img/dept-title-bar.gif"));
	    ip.toXML();
	} catch (Exception e) {
	}

    }
}
