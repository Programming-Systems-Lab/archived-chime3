package psl.chime.sienautils;
import siena.*;

public class SienaObject {

    public String Sdata;
    public String Susername;
    public String Spassword;
    public String Sneed_auth;
    public String Sfrom_component;
    public String Saccess;
    public String Saddress;
    public String Sprotocol;
    public String Smethod;
    HierarchicalDispatcher Ssiena;

    /**
     * create a basic siena object
     */
    public SienaObject(String component, String data, boolean needAuth) {
	setData(data);
	setFromComponent(component);
	setUsername("anonymous");
	setPassword("anonymous");
	setNeedAuth(needAuth);
    }


    /**
     * create the full siena object
     */

    public SienaObject(String protocol, String address, String this_component, String data, String username, String password, boolean need_auth) {
	setProtocol(protocol);
	setAddress(address);
	setData(data);
	setFromComponent(this_component);
	setPassword(password);
	setUsername(username);
	setNeedAuth(need_auth);
	setMethod("");
	setAccess("");
    }

    /**
     * create the siena object from a Notification
     */

    public SienaObject(Notification not) {
	setData(not.getAttribute("data").stringValue());
	setUsername(not.getAttribute("username").stringValue());
	setPassword(not.getAttribute("password").stringValue());
	setFromComponent(not.getAttribute("from_component").stringValue());
	setNeedAuth(not.getAttribute("auth").stringValue());
	setAccess(not.getAttribute("access").stringValue());
	setProtocol(not.getAttribute("prot").stringValue());
	setAddress(not.getAttribute("address").stringValue());

	if (not.getAttribute("method") != null)
		setMethod(not.getAttribute("method").stringValue());
    }


    /**
     * get the Notification object associated with this siena object
     */

    public Notification getNotification() {
	Notification e = new Notification();
	e.putAttribute("from_component", getFromComponent());
	e.putAttribute("data", getData());
	e.putAttribute("auth", getNeedAuth());
	e.putAttribute("username", getUsername());
	e.putAttribute("password", getPassword());
	e.putAttribute("prot", getProtocol());
	e.putAttribute("address", getAddress());
	e.putAttribute("access", getAccess());
	e.putAttribute("method", getMethod());
	return e;
    }


    /**
     * get the hierarchical dispatcher associated with this object
     */

    public HierarchicalDispatcher getDispatcher() {
	return Ssiena;
    }

    /**
     * set the hierarchical dispatcher
     */

    public void setDispatcher(HierarchicalDispatcher siena) {
	Ssiena = siena;
    }

    /**
     * publish this Siena Object
     */

    public void publish() throws Exception {
	new PublishObject(getDispatcher(), getNotification());
    }

	/**
	 * get the method associated with this object
	 */
	public String getMethod() {
		return Smethod;
	}

	/**
	 * set the method associated with this siena object
	 */

	public void setMethod(String method) {
		Smethod = method;
	}
    /**
     * get the protocol associated with the thing we are trying to retrieve
     */
    public String getProtocol() {
	return Sprotocol;
    }

    /**
     * set the protocol of this siena object
     */

    public void setProtocol(String protocol) {
	Sprotocol = protocol;
    }

    /**
     * set the address of the thing we are trying to retrieve
     */

    public void setAddress(String address) {
	Saddress = address;
    }

    /**
     * get the address encoded in this object
     */

    public String getAddress() {
	return Saddress;
    }


    /**
     * get the data part of this object
     */

    public String getData() {
	return Sdata;
    }


    /**
     * get the username associated with this object
     */

    public String getUsername() {
	return Susername;
    }

    /**
     * get the password associated with this object
     */

    public String getPassword() {
	return Spassword;
    }

    /**
     * get the component where this object was created
     */

    public String getFromComponent() {
	return Sfrom_component;
    }

    /**
     * does this Object need to be authenticated
     */

    public boolean needAuth() {
	if (Sneed_auth.equals("true"))
	    return true;
	else return false;
    }


    /**
     * return the authorization info
     */
    public String getNeedAuth() {
	return Sneed_auth;
    }

    /**
     * return the authorization info
     */
    public String getAccess() {
	return Saccess;
    }





    /**
     * set the username of this SienaObject()
     */

    public void setUsername(String username) {
	this.Susername = username;
    }


    /**
     * set the access of this user
     */

    public void setAccess(String access) {
	this.Saccess = access;
    }


    /**
     * set the password of this SienaObject
     */

    public void setPassword(String password) {
	this.Spassword = password;
    }

    /**
     * set the authorization for this SienaObject
     */

    public void setNeedAuth(boolean need_auth) {
	if (need_auth)
	    this.Sneed_auth = "true";
	else
	    this.Sneed_auth = "false";
    }

    /**
     * set the authorization for this SienaObject
     */

    public void setNeedAuth(String need_auth) {
	this.Sneed_auth = need_auth;
    }


    /**
     * set the component where this data object is created
     */

    public void setFromComponent(String component) {
	this.Sfrom_component = component;
    }

    /**
     * set the data of this Siena Object
     */

    public void setData(String data) {
	this.Sdata = data;
    }

    /**
     * create new SienaObject if you have an old one and you don't
     * want to change most of the parameters
     */

    public void newSienaObject(SienaObject old, String data, String component) {
	setUsername(old.getUsername());
	setPassword(old.getPassword());
	setNeedAuth(old.needAuth());
	setData(data);
	setFromComponent(component);
    }
}
