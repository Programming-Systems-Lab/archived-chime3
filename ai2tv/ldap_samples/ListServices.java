import javax.naming.*;
import javax.naming.directory.*;
import java.util.*;

/*
 * @author Suhit Gupta
 */

public class ListServices {
  private static String LDAPServer="softe.cs.columbia.edu:389";

  public static void main(String[] args) {

    // Support another LDAP server
    if(args.length > 0 && (args[0].equals("-?") || 
			   args[0].startsWith("--help") ||
			   args[0].equals("-h"))) {
      System.out.println("usage: java ListServices [classes or combo] [class_name] [lecture# or hw#]");
      System.exit(-1);
    }
    
    Hashtable env = new Hashtable();
    env.put(Context.INITIAL_CONTEXT_FACTORY, 
	    "com.sun.jndi.ldap.LdapCtxFactory");
    env.put(Context.PROVIDER_URL, "ldap://liberty.psl.cs.columbia.edu:389/o=ai2tv");

    DirContext ctx = null;
    NamingEnumeration ne = null;
    try {
      // Perform the connection
      ctx = new InitialDirContext(env);
      // Do the list operation
      ne = ctx.list("ou=" + args[1] + ",ou=" + args[0]);
    } catch(NamingException e) { e.printStackTrace(); }

    while(ne.hasMoreElements()) {
      NameClassPair ncp = (NameClassPair)ne.nextElement();
      System.out.println("Found " + ncp.getName() + "; attributes are:");

      try {
	/* For this node under the ctx.list context, get the
	 * associated attributes
	 */
	Attributes a = ctx.getAttributes("cn=" + args[2] + ",ou=" + args[1] + ",ou=" + args[0]);
	String b = null;
	/* Now, get the individual attributes in an enumeration */
	NamingEnumeration ids = a.getIDs();
	while(ids.hasMoreElements()) {
	  String id = (String)ids.next();
	  /* Print out the IDs (keys) and their associated values.  Note the
	   * double-get
	   */
	  System.out.println(id + ": " + a.get(id).get());
	}
	System.out.println("\n-----");
	/* Note that here we could now inquire as to the individual
	 * attributes using its get() method.
	 */
      } catch(NamingException e) { e.printStackTrace(); }
    }

    System.out.println("Done!");
  }
}

