

import java.lang.*;
import java.util.*;
import java.io.*;
import siena.*;

// Nodes of the default file settings table
class VemNode
{
    String file;
    String type;
    String sub;
    String object;

    public VemNode (String file, String type, String sub, String object)
    {
		file   = this.file;
		type   = this.type;
		sub    = this.sub;
		object = this.object;
    }

    public VemNode ()
    {
		this ("", "", "", "");
    }
}