#ifndef FRAXFILTER_H
#define FRAXFILTER_H

#include <AString.h>
#include <EvtLogger.h>
#include <comdef.h>
//import MSXML3 classes
#import <msxml3.dll> named_guids raw_dispinterfaces

class FRAXFilter
{
public:
	
	FRAXFilter(); 
	~FRAXFilter(); 

	AString FilterDocSummaryInfoSection( AString szXML );
	AString FilterHyperlinksInfoSection( AString szXML );
	AString FilterSummaryInfoSection( AString szXML );
	//AString FilterUserDefinedPropertiesSection( AString szXML );
	

private:
	BOOL GetXMLData( AString szXMLTag, VARIANT& vData, MSXML2::IXMLDOMElementPtr spRoot );
	EvtLogger m_EvtLogger;
};

#endif