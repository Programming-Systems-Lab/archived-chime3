
#if !defined(AFX_MSOFRAXPLUG_H__64DD2878_5F87_4208_9612_F422745E9B36__INCLUDED_)
#define AFX_MSOFRAXPLUG_H__64DD2878_5F87_4208_9612_F422745E9B36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

static const char* APPNAME							= "msoFRAXPlug";
static const char* OFFICE_DOC_PROP_XML_START_TAG	= "<OfficeDocProperties>";
static const char* OFFICE_DOC_PROP_XML_END_TAG		= "</OfficeDocProperties>";
static const char* CONSTRAINT_NAME					= "OfficeDoc";
static const char* CONSTRAINT_VALUE					= "FRAXOfficeDoc";
static const char* TARGET_DOC						= "TargetDoc";
static const char* OFFICE_DOC_PROP_RESULTS_NAME		= "OfficeDocPropResults";
static const char* OFFICE_DOC_PROP_RESULTS_VALUE	= "msoFRAXPlugResults";
static const char* OFFICE_DOC_PROPERTIES			= "DocProperties";
const int MIN_PATH_LENGTH							= 3; // "X:\"

// FRAX specific XML tags
static const char* PROTOCOL_XML_START_TAG			= "<Protocol>";
static const char* PROTOCOL_XML_END_TAG				= "</Protocol>";
static const char* URL_XML_START_TAG				= "<URL>";
static const char* URL_XML_END_TAG					= "</URL>";
static const char* TYPE_XML_START_TAG				= "<Type>";
static const char* TYPE_XML_END_TAG					= "</Type>";
static const char* FRAX_SOURCE_XML_START_TAG		= "<FraxSource>";
static const char* FRAX_SOURCE_XML_END_TAG			= "</FraxSource>";

#endif // !defined(AFX_MSOFRAXPLUG_H__64DD2878_5F87_4208_9612_F422745E9B36__INCLUDED_)
