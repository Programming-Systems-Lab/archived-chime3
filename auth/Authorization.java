package psl.chime.auth;
import siena.*;
import java.util.*;
import psl.chime.sienautils.*;
import java.util.*;
import java.io.*;

/**
 * Check if the authorization information is correct
 */
public class Authorization {
    HierarchicalDispatcher siena;
    Hashtable hash;

    public Authorization(HierarchicalDispatcher hd, SienaObject s) {
        siena = hd;
	readFile("passwd.txt");
	authorize(s);
    }


    /**
     * authorize the siena object so that it is accepted
     */

    public void authorize(SienaObject obj) {
	String access;
	try {
	    if ((access = checkUserPass(obj)) != null) {
		System.out.println("Verified: " + obj.getUsername());
		obj.setNeedAuth(false);
		obj.setAccess(access);
		//resubmit it to siena
		new PublishObject(siena, obj);
	    } else {
		System.out.println(obj.getUsername() + ": password or username incorrect:" + obj.getPassword());
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
    

    /**
     * Check the username and password
     */
    public String checkUserPass(SienaObject obj) {
	System.out.println("Checking username and pass");
	AuthObject auth = (AuthObject) hash.get(obj.getUsername());
	
	//username not found
	if (auth == null)
	    return null;
	
	if (auth.getPassword().equals(obj.getPassword()))
	    return auth.getUserType();
	else
	    return null;
    }
    
    
    /**
     * Read the password file into memory
     */
    public void readFile(String filename) {
	
	File file = findFile(filename);
	try {
	    hash = new Hashtable();
	    BufferedReader in = new BufferedReader(new FileReader(file));
	    String line;
	    
	    while((line = in.readLine()) != null) {
		StringTokenizer st = new StringTokenizer(line,":");
		if (st.countTokens() == 3) {
		    AuthObject auth = new AuthObject(st.nextToken().trim(), st.nextToken().trim(), st.nextToken().trim());
		    hash.put(auth.getUsername(), auth);
		}
	    }	    
	} catch (Exception e) {
	    System.err.println("Password File not found");
	    System.exit(0);
	}
    }

    /**
     * Try to find the config file in the classpath
     */
    private File findFile(String file) {
	String classpath = System.getProperty("java.class.path");
	StringTokenizer st = new StringTokenizer(classpath, File.pathSeparator);
	
	while (st.hasMoreTokens()) {
	    String path = st.nextToken();
	    System.out.println(path + File.separatorChar + file);
	    File pathf = new File(path + File.separatorChar + file);
	    if (pathf.exists()) {
		return pathf;
	    }
	    
	}
	
	return null;
    }
}
