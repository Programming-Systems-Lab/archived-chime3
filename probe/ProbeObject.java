
public class ProbeObject {
	public static int COMPARE_DATA = 0;
	public static int COMPARE_METADATA = 1;
	public static int COMPARE_LENGTH = 2;


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

	void setData(String data) {
			this.data = md5.toDigest(data);
	}

	void setData(DataInputStream ds) {
		br = new BufferedReader(new InputStreamReader(ds));
		String temp;
		while((temp = br.readLine()) != null)
			data = data + temp + "\n";
	}


	void setLength(Long length) {
			this.length = length;
	}

	void setProtocol(String protocol) {
			this.protocol = protocol;
	}

	void setLocation(String location) {
			this.location = location;
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
	boolean equals(ProbeObject pb, int flag) {
		switch (flag) {

		case COMPARE_METADATA:
			if (getMetadata().equals(pb.getMetaData()))
				return true;
		case COMPARE_DATA:
			if (getData().equals(pb.getData()))
				return true;
		case COMPARE_LENGTH:
			if (getLength().equals(pb.getLength()))
				return true;
		}
	}

}
