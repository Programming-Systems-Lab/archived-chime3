 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax;
import java.lang.Class;
import java.lang.reflect.*;
import java.lang.*;
import java.io.*;
import java.util.*;

public class FRAXClassLoader extends ClassLoader{

    private Hashtable classes = new Hashtable();

    /**
     * this is the required class loader which takes a
     * classname and tries to see if it was loaded already
     * then the Class object is returned otherwise the class
     * byte code is loaded and a class instance generated
     * @params: className - the class name to load
     * @params: resolveIt - if you need to resolve a class
     */
    public synchronized Class loadClass(String className, boolean resolveIt)
        throws ClassNotFoundException {
        Class result;
        byte classData[] = null;

        System.out.println("Loading class : "+className);

        /* Check our local cache of classes */
        result = (Class)classes.get(className);
        if (result != null) {
            System.out.println("returning already loaded class");
            return result;
        }

        /* Check if this is a class in our classpath */
        try {
            result = super.findSystemClass(className);
            System.out.println("returning system class (in CLASSPATH).");
            return result;
        } catch (ClassNotFoundException e) {
            System.out.println("Not a system class.");
        }


        /* Define the class */
        result = defineClass(className, classData, 0, classData.length);
        if (result == null) {
            throw new ClassFormatError();
        }

        if (resolveIt) {
            resolveClass(result);
        }

        classes.put(className, result);
        System.out.println("Returning newly loaded class.");
        return result;
     }


    //want to make sure that all classes are resolved first
    public synchronized Class loadClass(String className)
	throws ClassNotFoundException {
	return loadClass(className, true);
     }


    /**
     * Primarily for debugging purposes
     * this just creates an array of classes from the
     * objects array
     */
    private Class[] createClassArray(Object[] data) {
	Class[] types = new Class[data.length];
	for (int i = 0; i < data.length; i++) {
	    if (data[i] == null)
		System.err.println("The " + i + "th parameter is null - ODD");
	    else {
		types[i] = data[i].getClass();
		//System.out.println(types[i]);
	    }
	}

	return types;
    }

    /**
     * Primarily for Debugging purposes
     * This returns an array of constructors that are
     * available for a particular class
     */
    private void printConstructors(Class classname) {
	System.out.println("Class is: " + classname.getName());
	System.out.println("Available Constructor Params follow: ");
	Constructor[] constr = classname.getConstructors();
	for (int i = 0; i < constr.length; i++) {
	    Class[] temp = constr[i].getParameterTypes();
	    for (int j = 0; j < temp.length; j++) {
		System.out.print(temp[j].getName() + ", ");
	    }
	    System.out.println();
	}
    }


    /**
     * Create an instance of a class
     * @params: classname - A class object representing the classname
     * to be loaded.
     * parameters: an array of objects that should be passed as arguments
     * to the constructor
     * Throws a bunch of exceptions which identify why a particualr class
     * can't be loaded
     **/
    public synchronized Object instantiateClass(Class classname, Object[] parameters) {

	//Object[] test = {"hello"};
	//Class[] myparams = (Class[]) params;

	Class[] types = createClassArray(parameters);
	System.out.println("In instantiate class");

	printConstructors(classname);

	System.out.println("The first constr param is: " + parameters[0].getClass().getName());

	try {
	    Constructor constr = classname.getConstructor(types);
	    System.out.println("The constructor is: " + constr);
	    return constr.newInstance(parameters);

	} catch (NoSuchMethodException e) {
	    System.err.println("No matching Constructor exists:" + e.getMessage());
	    return null;
	} catch (InstantiationException e) {
	    System.err.println("Can't instantiate an abstract class:" + e.getMessage());
	    return null;
	} catch (IllegalAccessException e) {
	    System.err.println("This constructor is not accessible:" + e.getMessage());
	    return null;
	} catch (InvocationTargetException e) {
	    System.err.println("The constructor has thrown an exception:" + e.getMessage());
	    return null;
	}
    }


}
