// AString.h: interface for the AString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTRING_H__90312EB5_FBDC_4531_915B_27A1991E5F5B__INCLUDED_)
#define AFX_ASTRING_H__90312EB5_FBDC_4531_915B_27A1991E5F5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afx.h>
#include <exports.h>
#include <KMPMatcher.h>

#pragma warning( disable : 4290 ) 
const int MAX_BUFFER_LEN = 50;
const int DEFAULT_DECIMAL_PLACES = 2;

class CommonExport AString : public CString
{
public:
	
	/* Constructors */
	AString();
	AString( const AString& stringSrc );
	AString( const TCHAR ch, int nRepeat = 1 );
	AString( LPCTSTR lpch, int nLength );
	AString( const unsigned char* psz );
	AString( LPCWSTR lpsz );
	AString( LPCSTR lpsz );
	AString( const CString& szString );

	/*	Additional Constructors to convert long, int, double, 
		unsigned int, unsigned long to string */
	AString( const int nValue );
	AString( const unsigned int uValue );
	AString( const double dblValue );
	AString( const double dblValue, int nDecimalPlaces );
	AString( const long lngValue );
	AString( const unsigned long ulValue );

	virtual ~AString();

	//methods

	/* Procedure removes leading and trailing whitespace from AString */
	AString Strip(); 
	operator LPCTSTR () const;
	/* Returns the underlying string as integer */
	int AsInt();
	/* Returns the underlying string as double */
	double AsDouble();
	/* Returns the underlying string as long */
	long AsLong();
	/* Returns the underlying string as unsigned long */
	unsigned long AsUnsignedLong();
	/* Returns the underlying string as unsigned integer */
	unsigned int AsUnsignedInt();
	/* Returns the underlying string as DWORD */
	DWORD AsDWORD();

	//assignment operators
	const AString& operator=( const unsigned char* pszRhs );
	const AString& operator=( LPCSTR pszRhs );
	const AString& operator=( LPCWSTR pwszRhs );
	const AString& operator=( const CString& szRhs );
	const AString& operator=( const AString& szRhs );

	//pattern matching

	/*	Function searches the AString for a specific pattern 
		and if found replaces it with the replacment text.

		NOTE:	If the pattern overlaps then for each pattern occurence,
				the replacement text is inserted e.g
				for 
			
				text			"abbabbabbabbabba" 
				pattern			"abba"
				replacment text "beans"

		the resulting string is "beansbeansbeansbeansbeans" because the 
		pattern "abba" occurs 5 times in the text (one instance of the
		pattern overlaps/is part of another instance) so we get 5 instances
		of the replacement text.  */
	AString FindReplace( AString szPattern, AString szReplacement );
		
	/*	Function searches the AString for a specific pattern ignoring case
		and if found replaces it with the replacment text.

		NOTE:	If the pattern overlaps then for each pattern occurence,
				the replacement text is inserted e.g
				for 
				
					text			"abbabbabbabbabba" 
					pattern			"ABBA"
					replacment text "beans"

	  the resulting string is "beansbeansbeansbeansbeans" because the 
	  pattern "abba" occurs 5 times in the text (one instance of the
	  pattern overlaps/is part of another instance) so we get 5 instances
	  of the replacement text. */
	AString FindReplaceNoCase( AString szPattern, AString szReplacement );
	/*	Function returns the zero based index of a specific pattern after
		offset nLastIndex.

		Input parameters:	szPattern  - pattern to search for
							nLastIndex - look for pattern after this position
							in string. 
		Return values:		On Success - returns 0-based index of pattern
							On Error   - returns -1 (pattern not found)

		NOTE:	If nLastIndex is out of range (greater than length of string)
		function returns -1.	*/
	int IndexOf( AString szPattern, int nLastIndex = -1 );
	/*	Function returns the zero based index of a specific pattern after
		offset nLastIndex, ignores case.

		Input parameters:	szPattern  - pattern to search for
							nLastIndex - look for pattern after this position
										 in string. 
		Return values:		On Success - returns 0-based index of pattern
							On Error   - returns -1 (pattern not found)

	NOTE:	If nLastIndex is out of range (greater than length of string)
			function returns -1.	*/
	int IndexOfNoCase( AString szPattern, int nLastIndex = -1 );
		
	/*	Function returns the number of times a specific pattern occurs in
		an AString.

	Input parameters:	szPattern - pattern to search for

	Return values:		an integer >= 0 of the number of times pattern
						occurs. */
	int KMPMatch( AString szPattern );
	/*	Function returns the number of times a specific pattern occurs in
		an AString ignoring case.

	Input parameters:	szPattern - pattern to search for

	Return values:		an integer >= 0 of the number of times pattern
						occurs. */
	int KMPMatchNoCase( AString szPattern );
		
	/*	Function returns the zero based index of the last occurrence of
		a specific pattern. 

	Input parameters:	szPattern  - pattern to search for
						
	Return values:		On Success - returns 0-based index of pattern
						On Error   - returns -1 (pattern not found) */
	int LastIndexOf( AString szPattern );
	/*	Function returns the zero based index of the last occurrence of
		a specific pattern ignoring case. 

		Input parameters:	szPattern  - pattern to search for
						
		Return values:		On Success - returns 0-based index of pattern
							On Error   - returns -1 (pattern not found) */
	int LastIndexOfNoCase( AString szPattern );
	/*	Function returns the zero based index of the Nth occurrence of
		a specific pattern. 

	Input parameters:	szPattern  - pattern to search for
						int nNthIndex -	 the Nth pattern occurrence to 
										 find. e.g used to find the
										 3rd or 4th occurrence of the
										 pattern.
						
	Return values:		On Success - returns 0-based index of pattern
						On Error   - returns -1 (pattern not found) */
	int NthIndexOf( AString szPattern, int nNthIndex );
	/*	Function returns the zero based index of the Nth occurrence of
		a specific pattern ignoring case. 

	Input parameters:	szPattern  - pattern to search for
						int nNthIndex -	 the Nth pattern occurrence to 
										 find. e.g used to find the
										 3rd or 4th occurrence of the
										 pattern.
						
	Return values:		On Success - returns 0-based index of pattern
						On Error   - returns -1 (pattern not found) */
	int NthIndexOfNoCase( AString szPattern, int nNthIndex );
};

#endif // !defined(AFX_ASTRING_H__90312EB5_FBDC_4531_915B_27A1991E5F5B__INCLUDED_)
