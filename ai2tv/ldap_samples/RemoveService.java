import javax.naming.*;
import javax.naming.directory.*;
import java.util.*;

/**
 * RemoveService LDAP example.
 * @author Suhit Gupta
 */

public class RemoveService {
  private static final String username = "cn=Manager,o=ai2tv";
  private static final String password = "ai2tv";

  public static void main(String[] args) {
    if(args.length != 3 && args.length != 4) {
      System.out.println("usage: java AddService <LDAP server:port> <node> <branch> <sub_branch>");
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

      /* Perform the unbinding (deletion).  Note that this will 
       * throw an exception as long as ou=services exists, whether or
       * not the CN exists!  We supply the part of the DN not supplied
       * to the InitialDirContext.
       */
      if(args.length == 3)
	ctx.unbind("ou=" + args[1] + ",ou=" + args[2]);
      else if(args.length == 4)
	ctx.unbind("cn=" + args[1] + ",ou=" + args[3] + ",ou=" + args[2]);
    } catch(NamingException e) { e.printStackTrace(); }

    System.out.println("Done!");
  }
}
