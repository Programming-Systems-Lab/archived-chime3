/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * Parser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * Interface for XML parsers.
 */

package xescii;

import org.jdom.*;
import org.jdom.input.*;


public interface Parser {
    public Tuple parseDoc();
}
