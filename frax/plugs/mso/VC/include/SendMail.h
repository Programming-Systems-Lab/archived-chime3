// SendMail.h: interface for the SendMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDMAIL_H__33981140_542A_11D5_80B8_0050DAC00BBC__INCLUDED_)
#define AFX_SENDMAIL_H__33981140_542A_11D5_80B8_0050DAC00BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <exports.h>
#include <evtlogger.h>
#include <list>

#import <cdonts.dll> named_guids raw_dispinterfaces
using namespace CDONTS;

using namespace std;

class CommonExport SendMail : public CObject
{
	DECLARE_DYNAMIC( SendMail )
public:
	SendMail();
	virtual ~SendMail();
		
	/* Function sends a plaintext email message. (Normal Priority) 

	Input parameters:	szFrom		-	From email address (sender)
						szTo		-	To email address (receiver or 
										list of receivers each separated by  
										a semicolon	";" )
						szSubject		- Subject of the email 
						szBodyText		- Body of the email message
						lstAttachements	- A reference to a list of fully
											quallified filenames. By
										  "fully qualified" the name and 
										  path to the file is specified.
						szCC			- Carbon Copy address or list of
										  addresses each separated by a 
										  semicolon ";"
						szBCC			- Blind Carbon Copy address or list
										  of addresses each separated by a
										  semicolon ";"
										  
	Return values:		On Success returns TRUE
						On Error returns FALSE.  */
	BOOL SendPlainTextEmail( AString szFrom, //required
							 AString szTo,  //optional can be empty ""
							 AString szSubject, //required
							 AString szBodyText, //required
							 list<AString>& lstAttachments, //required 
							 AString szCC = "", //optional
							 AString szBCC = "" ); //optional

	/* Function sends an HTML email message. (Normal Priority) 

		Input parameters:	szFrom		-	From email address (sender)
							szTo		-	To email address (receiver or 
											list of receivers each separated by  
											a semicolon	";" )
							szSubject		- Subject of the email 
							szBodyText		- Body of the email message
							lstAttachements	- A reference to a list of fully
											  quallified filenames. By
											  "fully qualified" the name and 
											  path to the file is specified.
							szContentBase	- Used only with HTML mail, 
											  specifies a base for all URLs relating to 
											  the email's message body. 
							szContentLocation - Used only with HTML mail and in
												conjunction with szContentBase
												sets an absolute or relative 
												path for all URLs relating 
												to the email's message body. 
							szCC			- Carbon Copy address or list of
											  addresses each separated by a 
											  semicolon ";"
							szBCC			- Blind Carbon Copy address or list
											  of addresses each separated by a
											  semicolon ";"
									  
	Return values:		On Success returns TRUE
					On Error returns FALSE.  */
	BOOL SendHTMLEmail(	AString szFrom, //required
						AString szTo, //optional can be empty ""
						AString szSubject, //required
						AString szBodyText, //required
						list<AString>& lstAttachments, //required
						AString szContentBase = "", //optional
						AString szContentLocation = "", //optional
						AString szCC = "", //optional
						AString szBCC = "" ); //optional
private:
	/* Function writes an error event message to the event log 

		Input parameters:	szFile	- source file where error occurred
							lngLine - line in source file where error occurred
							szMsg	- message to write 

	Return values:		On Success returns TRUE
						On Error returns FALSE */
	BOOL ReportError( char* szFile, long lngLine, AString szMsg );

	/*	Function sends an e-mail message either as plaintext or as HTML.

		Input parameters:	szFrom		-	From email address (sender)
							szTo		-	To email address (receiver or 
											list of receivers each separated by  
											a semicolon	";" )
							szSubject		- Subject of the email 
							szBodyText		- Body of the email message
							lstAttachements	- A reference to a list of fully
											  quallified filenames. By
										      "fully qualified" the name and 
										      path to the file is specified.
						    lngImportanceLevel - Message importance
							szCC			- Carbon Copy address or list of
											  addresses each separated by a 
											  semicolon ";"
							szBCC			- Blind Carbon Copy address or list
											  of addresses each separated by a
											  semicolon ";"
							bSendHTML		- Send mail as HTML email if TRUE
											  else send as plaintext
							szContentBase	- Used only with HTML mail, 
											  specifies a base for all URLs relating to 
											  the email's message body. 

							szContentLocation - Used only with HTML mail and in
												conjunction with szContentBase
												sets an absolute or relative 
												path for all URLs relating 
												to the email's message body. 

	Return Values:	on Success returns TRUE
				on Error returns FALSE and any error(s) logged in NT 
				event log. */
	BOOL SendEMail( AString szFrom, //required
					AString szTo, //optional can be empty ""
					AString szSubject, //required
					AString szBodyText, //required
					list<AString>& lstAttachments, //required
					long lngImportanceLevel = CdoNormal, //optional
					AString szCC = "", //optional
					AString szBCC = "", //optional
					BOOL bSendHTML = FALSE, //optional
					AString szContentBase = "", //optional
					AString szContentLocation = "" ); //optional
	
	EvtLogger m_EvtLogger;
	BOOL IsEmptyString( AString szString );
	static const AString m_szClassname;
};

#endif // !defined(AFX_SENDMAIL_H__33981140_542A_11D5_80B8_0050DAC00BBC__INCLUDED_)
