import javax.naming.*;
import javax.naming.directory.*;
import java.util.*;

/**
 * AddNode LDAP example
 * @author Suhit Gupta
 */
public class AddNode {
  private static final String username = "cn=Manager, o=ai2tv";
  private static final String password = "ai2tv";

  public static void main(String[] args) {
    if(args.length != 3) {
      /* Note that in your code you will most likely have to support
       * more than just these primitives
       */
      System.out.println("usage: java AddService <LDAP server:port> <org_unit> <name_of_node>");
      System.exit(-1);
    }

    /* Now, create the "environment" in which we will perform
     * operations.  We need to authenticate here, since we're
     * modifying the service table.
     */
    Hashtable env = new Hashtable();
    env.put(Context.INITIAL_CONTEXT_FACTORY, 
	    "com.sun.jndi.ldap.LdapCtxFactory");
    env.put(Context.PROVIDER_URL, "ldap://" + args[0] + "/o=ai2tv");
    env.put(Context.SECURITY_PRINCIPAL, username);
    env.put(Context.SECURITY_CREDENTIALS, password);

    DirContext ctx = null;
    NamingEnumeration ne = null;
    try {
      // Perform the connection
      ctx = new InitialDirContext(env);

      // Create the attributes to be stored
      Attributes a = new BasicAttributes();
      a.put("objectClass", "OrganizationalUnit");
      a.put("ou", args[2]);

      /* Now store it.  We need to supply the CN and the part of the
       * that wasn't supplied to the InitialDirContext.
       *
       * Note that if we already bound this one, an error will occur
       * here.
       */
    ctx.bind("ou=" + args[2] + ", ou = " + args[1], null, a);
    } catch(NamingException e) { e.printStackTrace(); }

    System.out.println("Done!");
  }
}
