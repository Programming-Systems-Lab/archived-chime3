package psl.chime.vem;

import java.lang.*;

public class VemObject {

	public String shape2d = "";
	public String classtype = "";
	public String subtype = "";
	public String shape = "";
	public String protocol = "";
	public String url = "";

	public String objUrl = "";
	public String roomUrl = "";

	public String getClasstype() {
		return classtype;
	}

	public void setClasstype(String _classtype) {
		classtype = _classtype;
	}

	public String getSubclass() {
			return classtype;
	}

	public void setSubclass(String _subtype) {
			subtype = _subtype;
	}

	public String getShape() {
			return shape;
	}

	public void setShape(String _shape) {
			shape = _shape;
	}

	public String getProtocol() {
			return protocol;
	}

	public void setProtocol(String _protocol) {
			protocol = _protocol;
	}

	public String getUrl() {
			return url;
	}

	public void setUrl(String _url) {
		url = _url;
	}

	public String getObjUrl() {
		return objUrl;
	}

	public void setObjUrl(String _objUrl) {
		objUrl = _objUrl;
	}

	public String getRoomUrl() {
		return roomUrl;
	}

	public void setRoomUrl(String _roomUrl) {
		roomUrl = _roomUrl;
	}

	public void setShape2D(String _shape2d) {
		shape2d = _shape2d;
	}

	public String getShape2D() {
		return shape2d;
	}
}