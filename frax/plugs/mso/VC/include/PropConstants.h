#ifndef PropConstants_H
#define PropConstants_H
#include <AString.h>

#define PID_HYPERLINKSCHANGED		0x00000016L // VT_BOOL
#define PID_HYPERLINKS				0x00000002	// VT_BLOB	

// Summary info XML tags
static const char* SUMMARY_INFO_XML_START_TAG		= "<SummaryInfo>";
static const char* SUMMARY_INFO_XML_END_TAG			= "</SummaryInfo>";
static const char* TITLE_XML_START_TAG				= "<Title>";
static const char* TITLE_XML_END_TAG				= "</Title>";
static const char* SUBJECT_XML_START_TAG			= "<Subject>";
static const char* SUBJECT_XML_END_TAG				= "</Subject>";
static const char* AUTHOR_XML_START_TAG				= "<Author>";
static const char* AUTHOR_XML_END_TAG				= "</Author>";
static const char* KEYWORDS_XML_START_TAG			= "<Keywords>";
static const char* KEYWORDS_XML_END_TAG				= "</Keywords>";
static const char* COMMENTS_XML_START_TAG			= "<Comments>";
static const char* COMMENTS_XML_END_TAG				= "</Comments>";
static const char* TEMPLATE_XML_START_TAG			= "<Template>";
static const char* TEMPLATE_XML_END_TAG				= "</Template>";
static const char* LAST_AUTHOR_XML_START_TAG		= "<LastAuthor>";
static const char* LAST_AUTHOR_XML_END_TAG			= "</LastAuthor>";
static const char* REVISION_NUM_XML_START_TAG		= "<RevisionNumber>";
static const char* REVISION_NUM_XML_END_TAG			= "</RevisionNumber>";
static const char* EDIT_TIME_XML_START_TAG			= "<EditTime>";
static const char* EDIT_TIME_XML_END_TAG			= "</EditTime>";
static const char* LAST_PRINTED_XML_START_TAG		= "<LastPrinted>";
static const char* LAST_PRINTED_XML_END_TAG			= "</LastPrinted>";
static const char* CREATE_DATE_TIME_XML_START_TAG	= "<Created>";
static const char* CREATE_DATE_TIME_XML_END_TAG		= "</Created>";
static const char* LAST_SAVED_XML_START_TAG			= "<LastSaved>";
static const char* LAST_SAVED_XML_END_TAG			= "</LastSaved>";
static const char* PAGE_COUNT_XML_START_TAG			= "<PageCount>";
static const char* PAGE_COUNT_XML_END_TAG			= "</PageCount>";
static const char* WORD_COUNT_XML_START_TAG			= "<WordCount>";
static const char* WORD_COUNT_XML_END_TAG			= "</WordCount>";
static const char* CHAR_COUNT_XML_START_TAG			= "<CharCount>";
static const char* CHAR_COUNT_XML_END_TAG			= "</CharCount>";
static const char* THUMBNAIL_XML_START_TAG			= "<Thumbnail>";
static const char* THUMBNAIL_XML_END_TAG			= "</Thumbnail>";
static const char* APPNAME_XML_START_TAG			= "<AppName>";
static const char* APPNAME_XML_END_TAG				= "</AppName>";
static const char* DOC_SECURITY_XML_START_TAG		= "<DocSecurity>";
static const char* DOC_SECURITY_XML_END_TAG			= "</DocSecurity>";

// DocSummary XML tags
static const char* DOC_SUMMARY_INFO_XML_START_TAG	= "<DocSummaryInfo>";
static const char* DOC_SUMMARY_INFO_XML_END_TAG		= "</DocSummaryInfo>";
static const char* CATEGORY_XML_START_TAG			= "<Category>";
static const char* CATEGORY_XML_END_TAG				= "</Category>";
static const char* PRES_FORMAT_XML_START_TAG		= "<PresFormat>";
static const char* PRES_FORMAT_XML_END_TAG			= "</PresFormat>";
static const char* BYTE_COUNT_XML_START_TAG			= "<ByteCount>";
static const char* BYTE_COUNT_XML_END_TAG			= "</ByteCount>";
static const char* LINE_COUNT_XML_START_TAG			= "<LineCount>";
static const char* LINE_COUNT_XML_END_TAG			= "</LineCount>";
static const char* PARAGRAPH_COUNT_XML_START_TAG	= "<ParagraphCount>";
static const char* PARAGRAPH_COUNT_XML_END_TAG		= "</ParagraphCount>";
static const char* SLIDE_COUNT_XML_START_TAG		= "<SlideCount>";
static const char* SLIDE_COUNT_XML_END_TAG			= "</SlideCount>";
static const char* NOTE_COUNT_XML_START_TAG			= "<NoteCount>";
static const char* NOTE_COUNT_XML_END_TAG			= "</NoteCount>";
static const char* HIDDEN_COUNT_XML_START_TAG		= "<HiddenCount>";
static const char* HIDDEN_COUNT_XML_END_TAG			= "</HiddenCount>";
static const char* MM_CLIP_COUNT_XML_START_TAG		= "<MultiMediaClipCount>";
static const char* MM_CLIP_COUNT_XML_END_TAG		= "</MultiMediaClipCount>";
static const char* SCALE_XML_START_TAG				= "<Scale>";
static const char* SCALE_XML_END_TAG				= "</Scale>";
static const char* MANAGER_XML_START_TAG			= "<Manager>";
static const char* MANAGER_XML_END_TAG				= "</Manager>";
static const char* COMPANY_XML_START_TAG			= "<Company>";
static const char* COMPANY_XML_END_TAG				= "</Company>";
static const char* LINKS_DIRTY_XML_START_TAG		= "<LinksDirty>";
static const char* LINKS_DIRTY_XML_END_TAG			= "</LinksDirty>";
static const char* HYPERLINKS_CHANGED_XML_START_TAG	= "<HyperLinksChanged>";
static const char* HYPERLINKS_CHANGED_XML_END_TAG	= "</HyperLinksChanged>";

// MediaFile Summary XML tags
static const char* MF_SUMMARY_INFO_XML_START_TAG	= "<MFSummaryInfo>";
static const char* MF_SUMMARY_INFO_XML_END_TAG		= "</MFSummaryInfo>";
static const char* EDITOR_XML_START_TAG				= "<Editor>";
static const char* EDITOR_XML_END_TAG				= "</Editor>";
static const char* SUPPLIER_XML_START_TAG			= "<Supplier>";
static const char* SUPPLIER_XML_END_TAG				= "</Supplier>";
static const char* SOURCE_XML_START_TAG				= "<Source>";
static const char* SOURCE_XML_END_TAG				= "</Source>";
static const char* SEQ_NUM_XML_START_TAG			= "<SequenceNumber>";
static const char* SEQ_NUM_XML_END_TAG				= "</SequenceNumber>";
static const char* PROJECT_XML_START_TAG			= "<Project>";
static const char* PROJECT_XML_END_TAG				= "</Project>";
static const char* OWNER_XML_START_TAG				= "<Owner>";
static const char* OWNER_XML_END_TAG				= "</Owner>";
static const char* RATING_XML_START_TAG				= "<Rating>";
static const char* RATING_XML_END_TAG				= "</Rating>";
static const char* PRODUCTION_XML_START_TAG			= "<Production>";
static const char* PRODUCTION_XML_END_TAG			= "</Production>";
static const char* COPYRIGHT_XML_START_TAG			= "<Copyright>";
static const char* COPYRIGHT_XML_END_TAG			= "</Copyright>";

// User defines properties XML tags
static const char* USED_DEF_PROPS_XML_START_TAG		= "<UserDefinedProperties>";
static const char* USED_DEF_PROPS_XML_END_TAG		= "</UserDefinedProperties>";

// Hyperlinks XML tags
static const char* HLINKS_INFO_XML_START_TAG		= "<HLinksInfo>";
static const char* HLINKS_INFO_XML_END_TAG			= "</HLinksInfo>";
static const char* HYPERLINKS_XML_START_TAG			= "<HLINK_DATA>";
static const char* HYPERLINKS_XML_END_TAG			= "</HLINK_DATA>";

// Array of PIDSI's of properties you want
struct pidsiStruct 
{
	char *name;
	long pidsi;
	const char* m_szXMLStartTag;
	const char* m_szXMLEndTag;
}; 

struct pidsiStruct SummaryInfo[] = 
{
	{"Title",           PIDSI_TITLE,		TITLE_XML_START_TAG,			TITLE_XML_END_TAG}, // VT_LPSTR
    {"Subject",         PIDSI_SUBJECT,		SUBJECT_XML_START_TAG,			SUBJECT_XML_END_TAG}, // ...
    {"Author",          PIDSI_AUTHOR,		AUTHOR_XML_START_TAG,			AUTHOR_XML_END_TAG},
    {"Keywords",        PIDSI_KEYWORDS,		KEYWORDS_XML_START_TAG,			KEYWORDS_XML_END_TAG},
    {"Comments",        PIDSI_COMMENTS,		COMMENTS_XML_START_TAG,			COMMENTS_XML_END_TAG},
    {"Template",        PIDSI_TEMPLATE,		TEMPLATE_XML_START_TAG,			TEMPLATE_XML_END_TAG},
    {"LastAuthor",      PIDSI_LASTAUTHOR,	LAST_AUTHOR_XML_START_TAG,		LAST_AUTHOR_XML_END_TAG},
    {"RevisionNumber",	PIDSI_REVNUMBER,	REVISION_NUM_XML_START_TAG,		REVISION_NUM_XML_END_TAG},
    {"EditTime",        PIDSI_EDITTIME,		EDIT_TIME_XML_START_TAG,		EDIT_TIME_XML_END_TAG}, // VT_FILENAME (UTC)
    {"LastPrinted",     PIDSI_LASTPRINTED,	LAST_PRINTED_XML_START_TAG,		LAST_PRINTED_XML_END_TAG}, // ...
    {"Created",         PIDSI_CREATE_DTM,	CREATE_DATE_TIME_XML_START_TAG,	CREATE_DATE_TIME_XML_END_TAG},
    {"LastSaved",       PIDSI_LASTSAVE_DTM,	LAST_SAVED_XML_START_TAG,		LAST_SAVED_XML_END_TAG},
    {"PageCount",       PIDSI_PAGECOUNT,	PAGE_COUNT_XML_START_TAG,		PAGE_COUNT_XML_END_TAG}, // VT_I4
    {"WordCount",       PIDSI_WORDCOUNT,	WORD_COUNT_XML_START_TAG,		WORD_COUNT_XML_END_TAG}, // ...
    {"CharCount",       PIDSI_CHARCOUNT,	CHAR_COUNT_XML_START_TAG,		CHAR_COUNT_XML_END_TAG},
    {"Thumbnail",       PIDSI_THUMBNAIL,	THUMBNAIL_XML_START_TAG,		THUMBNAIL_XML_END_TAG}, // VT_CF
    {"AppName",         PIDSI_APPNAME,		APPNAME_XML_START_TAG,			APPNAME_XML_END_TAG}, // VT_LPSTR
    {"DocSecurity",     PIDSI_DOC_SECURITY,	DOC_SECURITY_XML_START_TAG,		DOC_SECURITY_XML_END_TAG} // VT_I4
};

struct pidsiStruct DocSummaryInfo[] = 
{
	{"Category",		PIDDSI_CATEGORY,		CATEGORY_XML_START_TAG,				CATEGORY_XML_END_TAG}, // VT_LPSTR
	{"PresFormat",		PIDDSI_PRESFORMAT,		PRES_FORMAT_XML_START_TAG,			PRES_FORMAT_XML_END_TAG}, //VT_LPSTR
    {"ByteCount",		PIDDSI_BYTECOUNT,		BYTE_COUNT_XML_START_TAG,			BYTE_COUNT_XML_END_TAG }, // VT_I4
	{"LineCount",		PIDDSI_LINECOUNT,		LINE_COUNT_XML_START_TAG,			LINE_COUNT_XML_END_TAG }, // VT_I4
	{"ParagraphCount",	PIDDSI_PARCOUNT,		PARAGRAPH_COUNT_XML_START_TAG,		PARAGRAPH_COUNT_XML_END_TAG }, // VT_I4
	{"SlideCount",		PIDDSI_SLIDECOUNT,		SLIDE_COUNT_XML_START_TAG,			SLIDE_COUNT_XML_END_TAG }, // VT_I4
	{"NoteCount",		PIDDSI_NOTECOUNT,		NOTE_COUNT_XML_START_TAG,			NOTE_COUNT_XML_END_TAG }, // VT_I4
	{"HiddenCount",		PIDDSI_HIDDENCOUNT,		HIDDEN_COUNT_XML_START_TAG,			HIDDEN_COUNT_XML_END_TAG }, // VT_I4
	{"MMClipCount",		PIDDSI_MMCLIPCOUNT,		MM_CLIP_COUNT_XML_START_TAG,		MM_CLIP_COUNT_XML_END_TAG }, // VT_I4
	{"Scale",			PIDDSI_SCALE,			SCALE_XML_START_TAG,				SCALE_XML_END_TAG }, // VT_I4
	{"Manager",			PIDDSI_MANAGER,			MANAGER_XML_START_TAG,				MANAGER_XML_END_TAG }, // VT_I4
	{"Company",			PIDDSI_COMPANY,			COMPANY_XML_START_TAG,				COMPANY_XML_END_TAG }, // VT_I4
	{"LinksDirty",		PIDDSI_LINKSDIRTY,		LINKS_DIRTY_XML_START_TAG,			LINKS_DIRTY_XML_END_TAG }, // VT_I4
	{"LinksChanged",	PID_HYPERLINKSCHANGED,	HYPERLINKS_CHANGED_XML_START_TAG,	HYPERLINKS_CHANGED_XML_END_TAG} // VT_BOOL
};	


struct pidsiStruct MediaFileSummaryInfo[] = 
{
	{"Editor",		PIDMSI_EDITOR,		EDITOR_XML_START_TAG,		EDITOR_XML_END_TAG}, // VT_LPWSTR
	{"Supplier",	PIDMSI_SUPPLIER,	SUPPLIER_XML_START_TAG,		SUPPLIER_XML_END_TAG}, // VT_LPWSTR
	{"Source",		PIDMSI_SOURCE,		SOURCE_XML_START_TAG,		SOURCE_XML_END_TAG}, // VT_LPWSTR
	{"SequenceNum",	PIDMSI_SEQUENCE_NO,	SEQ_NUM_XML_START_TAG,		SEQ_NUM_XML_END_TAG}, // VT_LPWSTR
	{"Project",		PIDMSI_PROJECT,		PROJECT_XML_START_TAG,		PROJECT_XML_END_TAG}, // VT_LPWSTR
	{"Owner",		PIDMSI_OWNER,		OWNER_XML_START_TAG,		OWNER_XML_END_TAG}, // VT_LPWSTR
	{"Rating",		PIDMSI_RATING,		RATING_XML_START_TAG,		RATING_XML_END_TAG}, // VT_LPWSTR
	{"Production",	PIDMSI_PRODUCTION,	PRODUCTION_XML_START_TAG,	PRODUCTION_XML_END_TAG}, // VT_LPWSTR
	{"Copyright",	PIDMSI_COPYRIGHT,	COPYRIGHT_XML_START_TAG,	COPYRIGHT_XML_END_TAG}, // VT_LPWSTR
};	

struct pidsiStruct HyperlinksInfo[] =
{
	{"Hyperlinks",	PID_HYPERLINKS,	HYPERLINKS_XML_START_TAG,	HYPERLINKS_XML_END_TAG}, // VT_BLOB
};

// Number of properties we are interested in
const int SUMMARY_PROP_COUNT		= 18;
const int DOC_SUMMARY_PROP_COUNT	= 14;
const int MF_SUMMARY_PROP_COUNT		= 9;
const int HYPERLINK_PROP_COUNT		= 1;

#endif