 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.plugs.html;

import java.io.*;
import java.lang.*;
import java.net.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.*;
import psl.chime.frax.plugs.FRAXPlug;

public class HTMLPlug extends FRAXPlug {

    private String metaData;
    private Reader reader;

    /** A number of constructructors which shows the
     * robustness of such a system. All create the same
     * internal representation of all data given
     * Currently only 2nd constructor used
     */
    public HTMLPlug(String added, DataInputStream is) {
	System.out.println("In inputStream constructor");
	metaData = added;
	reader = (Reader) new InputStreamReader(is);
    }

    public HTMLPlug(String added, DataInputStream is, Long length) {
	System.out.println("In inputStream constructor");
	metaData = added;
	reader = (Reader) new InputStreamReader(is);
    }

    public HTMLPlug(String added, Object contents) {
        metaData = added;
	createGlobs(contents);
    }

    public HTMLPlug(String added, byte[] data) {
	metaData = added;
	reader = (Reader) new StringReader(new String(data));
    }

    public HTMLPlug(Object contents) {
	createGlobs(contents);
    }

    /**
     * Create an internal representation of the object which
     * is being provided
     */
    private void createGlobs(Object contents) {
	if (contents instanceof sun.net.www.MeteredStream) {
	    reader = (Reader) new InputStreamReader((InputStream) contents);

	} else if (contents instanceof java.lang.String) {
	    reader = (Reader) new StringReader((String) contents);

	} else {
	    System.out.println("Unknown object");
	}
    }



    /**
     * Main method wihch is used by the classloader and which
     * produces the XML which corresponds to the object
     * which we are trying to get metadata about
     */
    public String toXML() {

	//create an inner call back class for the delegator
	HTMLEditorKit.ParserCallback callback =
	    new HTMLEditorKit.ParserCallback () {

		    Buffer buffer = new Buffer();

		    public void handleStartTag(HTML.Tag tag,
					       MutableAttributeSet attrSet, int pos) {

			//if it is an anchor then add it to the anchor buffer
			if (tag.equals(HTML.Tag.A)) {
			    buffer.links = buffer.links + Buffer.createXMLLink(attrSet);
			    //System.out.println("The current Line is: " + getCurrentLine());
			}
		    }

		    public void handleText(char[] text, int position) {
				//System.out.println("The text is: " + new String(text));
		    }

		    public void handleSimpleTag(HTML.Tag tag,
						MutableAttributeSet attrSet, int pos) {

			//if it is an image tag then add it to the image buffer
			if (tag.equals(HTML.Tag.IMG)) {
			    buffer.images = buffer.images + Buffer.createXMLImg(attrSet);
			}

		    }

		};

	try {
            new ParserDelegator().parse(reader, callback, true);
	} catch (Exception e) {
	    System.err.println(e.getMessage());
	}

	String final_xml =  "<?xml version=\"1.0\"?>\n<MetaInfo createDate=\'" + System.currentTimeMillis() + "\' type = \'html\'>\n" + metaData + Buffer.images + Buffer.links + "</MetaInfo>\n\n";
	System.out.println(final_xml);
	return final_xml;

    }


    /**
     * This class is used by the callback ParseDelegator and it creates the
     * appropriate XML for each of the components of the HTML file
     * so far the only components this knows about are links and images
     */
    public static class Buffer {
	public static String links = new String();
	public static String images = new String();


	/**
	 * Create XML from an image tag
	 */
	public static String createXMLImg(MutableAttributeSet attrSet) {
	    String temp = "<Image>\n";
	    temp = temp + "\"";
	    if  (attrSet.getAttribute(HTML.Attribute.SRC) != null)
		temp = temp + "\t<Source>" +  attrSet.getAttribute(HTML.Attribute.SRC) + "</Source>\n";

	    if  (attrSet.getAttribute(HTML.Attribute.ALT) != null)
		temp = temp + "\t<Alt>" + attrSet.getAttribute(HTML.Attribute.ALT) + "</Alt>\n";

		temp = temp + "\"";
	    return temp + "</Image>\n";
	}


	/**
	 * Create XML from a link tag
	 */
	public static String createXMLLink(MutableAttributeSet attrSet) {
	    String temp = "";
	    if  (attrSet.getAttribute(HTML.Attribute.HREF) != null) {
		temp = "<Link>" +  attrSet.getAttribute(HTML.Attribute.HREF) + "</Link>\n";
	   }
	    return temp;


	}

    }
}

