 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.plugs;

public abstract class FRAXPlug {

    /**
     * This is a class which has to be extended by all plugs
     * for now it just requires each plug to have a toXML()
     * method. More maybe added later
     */

    public abstract String toXML();
    //abstract boolean toXML(Object info_from_protocol);

}
