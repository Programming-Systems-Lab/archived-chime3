package psl.chime.auth;

public class AuthObject {
    String Ausername;
    String Apassword;
    String Aaccess;

    /**
     * create the object which will represent the 
     * authentication data
     */
    public AuthObject(String username, String password, String user_type) {
	Ausername = username;
	Apassword = password;
	Aaccess = user_type;
    }

    /**
     * get the username 
     */
    public String getUsername() {
	return Ausername;
    }

    /**
     * get the password
     */
    public String getPassword() {
	return Apassword;
    }

    /**
     * get the classification of the user
     */
    public String getUserType() {
	return Aaccess;
    }

}
