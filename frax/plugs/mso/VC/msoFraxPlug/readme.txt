msoFRAXPlug.exe 

Startup parameters

The start up parameters are the same as any Siena C++ subscriber
an example is given below:

msoFRAXPlug -port 2345 -host 160.39.200.34 senp://160.39.200.34:4321

Overview

Executable runs in an infinite loop  subscribing to Siena events
containing requests for document properties. 

A request for document properties is specified by the Notification:

Notification n;
n[CONSTRAINT_NAME] = CONSTRAINT_VALUE;
n[TARGET_DOC] = "http://localhost/testweb/images/testDoc.doc";

where 

CONSTRAINT_NAME = "OfficeDoc"
      TARGET_DOC = "TargetDoc"
	 
msoFRAXPlug will attempt to down load the TARGET_DOC to a local folder
extract what document properties it can and publish the results
as a Siena event with an XML string representation of the
document properties as a data member in the notification:

Notification n;
n[OFFICE_DOC_PROP_RESULTS_NAME] = OFFICE_DOC_PROP_RESULTS_VALUE
n[OFFICE_DOC_PROPERTIES] = <XML datastring>

where 

OFFICE_DOC_PROP_RESULTS_NAME = "OfficeDocPropResults"
OFFICE_DOC_PROP_RESULTS_VALUE= "msoFRAXPlugResults";
OFFICE_DOC_PROPERTIES	= "DocProperties";

Setup

The COM dlls 

COMEvtLogger.dll
MSDocProp.dll
MsoProp.dll

must all be registered on the server using the regsvr32 <dllname>

It is recommended that all the COM dlls ba placed on a single location
in addition to the messaging dll (genmsg.dll) and common.dll.
If the MFC dlls are not in the system path already they can be copied to
the same directory as the COM dlls and common.dll.
Dependencies

MFC dlls (debug or release) this version uses the debug version
MFC42(D).dll
MSVCRT(D).dll
MSVCIRT(D).dll

VB runtime module
MSVBVM60.dll

common.dll 	- (debug or release) dll of misc utility classes
genmsg.dll 	- windows messaging dll, facilitates writing to NT eventlog
COMEvtLogger.dll- COM object, used for writing to the NT Event log
		  mainly used from VB
MSDocProp.dll	- COM object (C++) used to extract simple properties
		  from MS Office documents without the need for
                  MS Office to be installed on the server
MsoProp.dll	- COM object (VB) used to extract extended properties
		  e.g embedded Hyperlinks. For this dll to extract these
		  properties the relevant MS Office Applications must be
  		  installed and registered on the server. Currently 
                  supports MS Word and MS Excel Docuements only.	 

