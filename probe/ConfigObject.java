/*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.probe;
import java.lang.*;
import java.io.*;
import java.util.*;

public class ConfigObject {

	private boolean look_at_metadata = false;
	private boolean look_at_data = false;
	private boolean look_at_length = false;
	private String this_prot;
	private String prot_load_class;

	void setLookAtMetadata(String allow) {
		if (allow.equals("y")
			look_at_metadata = true;
		else
			look_at_metadata = false;
	}

	void setLookAtData(String allow) {
		if (allow.equals("y")
			look_at_data = true;
		else
			look_at_data = false;
	}

	void setLookAtLength(String allow) {
		if (allow.equals("y")
			look_at_length = true;
		else
			look_at_length = false;
	}

	void setProtName(String name) {
		this_prot = name;
	}

	void setProtClass(String classname) {
		prot_load_class = classname;
	}

	boolean lookAtMetadata() {
			return look_at_metadata;
	}

	boolean LookAtData() {
			return look_at_data;
	}

	boolean LookAtLength() {
			return look_at_length;
	}

	String getProtName() {
		return this_prot;
	}

	String getProtClass() {
		return prot_load_class;
	}
}