package psl.chime.probe;
import java.io.*;
import java.lang.*;

public class ProbeObject {
	public static final int COMPARE_DATA = 0;
	public static final int COMPARE_METADATA = 1;
	public static final int COMPARE_LENGTH = 2;


	private String metadata;
	private Long length;
	private String data;
	private String protocol;
	private String location;
	private MD5 md5 = null;


	ProbeObject() {
		if (md5 == null)
			md5 = new MD5();
	}

	void setMetadata(String metadata) {
		this.metadata = md5.toDigest(metadata);
	}

	void setMetadata(ProbeObject po) {
			this.metadata = po.getMetadata();
	}

	void setData(String data) {
			this.data = md5.toDigest(data.trim());
	}

	void setData(DataInputStream ds) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(ds));
		String temp;
		while((temp = br.readLine()) != null)
			data = data + temp + "\n";

		setData(data.trim());
	}


	void setData(ProbeObject po) {
		this.data = po.getData();
	}


	void setLength(Long length) {
			this.length = length;
	}

	void setLength(ProbeObject po) {
		this.length = po.getLength();
	}

	void setProtocol(String protocol) {
			this.protocol = protocol.trim();
	}

	void setLocation(String location) {
			this.location = location.trim();
	}

	String getMetadata() {
		return metadata;
	}

	String getData() {
		return data;
	}

	Long getLength() {
		return length;
	}

	String getProtocol() {
		return protocol;
	}

	String getLocation() {
		return location;
	}

	//compare this object to the other ProbeObject
	public boolean equals(ProbeObject pb, int flag) {
		System.err.println("Old Object is: " + pb + "\nNew Object is: " + this);

		switch (flag) {

		case COMPARE_METADATA:
			if (getMetadata().equals(pb.getMetadata()))
				return true;
			else
				return false;

		case COMPARE_DATA:
			if (getData().equals(pb.getData()))
				return true;
			else
				return false;

		case COMPARE_LENGTH:
			if (getLength().equals(pb.getLength()))
				return true;
			else
				return false;
			}
		return false;
	}

	public String toString() {
		String temp = new String();
		temp = "--------------------------------------\n";
		temp = temp + "<<<Protocol : " + getProtocol() + "\n";
		temp = temp + "<<<Location : " + getLocation() + "\n";
		temp = temp + "<<<Metadata : " + getMetadata() + "\n";
		temp = temp + "<<<Data : " + getData() + "\n";
		temp = temp + "<<<Length : " + getLength() + "\n";
		temp = temp + "--------------------------------------\n";
		return temp;
	}

}


