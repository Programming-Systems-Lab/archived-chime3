// KMPMatcher.h: interface for the KMPMatcher class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KMPMATCHER_H__CF353CA0_E90C_4B02_834A_008562EBBA41__INCLUDED_)
#define AFX_KMPMATCHER_H__CF353CA0_E90C_4B02_834A_008562EBBA41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <TCHAR.h>
#include <COMDEF.H>
#include <exports.h>

/*	String Matcher Class.
	Pattern Matching engine based on the algorithm of Knuth, Morris and Pratt (KMP) and extended
	for general use by Rean Griffith 2001. 

	All methods declared as static since KMPMatcher has no member data therefore an instance of
	the class need not be created. */


class CommonExport KMPMatcher  
{
public:
	KMPMatcher(); //Constructor
	virtual ~KMPMatcher();//Destructor
		
	/* Return the number of times szPattern occurs in the string szText */
	static int KMPMatch( const TCHAR* szText, const TCHAR* szPattern ); //case sensitive
	/* Return the number of times szPattern occurs in the string szText (ignoring case) */
	static int KMPMatchNoCase( const TCHAR* szText, const TCHAR* szPattern );
	/* Return the index (array location) of szPattern in szText. The optional parameter
	   nLastIndex is used to search for the occurrence (index) of szPattern after offset 
	   nLastIndex of szText. */
	static int IndexOf( const TCHAR* szText, const TCHAR* szPattern, int nLastIndex = -1 );
	/* Return the index (array location) of szPattern in szText. The optional parameter
	   nLastIndex is used to search for the occurrence (index) of szPattern after offset 
	   nLastIndex of szText (ignore case) . */
	static int IndexOfNoCase( const TCHAR* szText, const TCHAR* szPattern, int nLastIndex = -1 );
	/* Search szText for szPattern and replace every occurrence of szPattern with 
	   szReplacement. 
	
	NOTE: (1) szPattern and szReplacement must be the same length since we are dealing with 
			  fixed sized (static or dynamic) arrays. 
		  (2) Function expects character arrays (static or dynamic) TCHAR* pointers initialized
	          using TCHAR* szString = "ababababababa" will cause a crash on an attempt to
	          do a replace on szString.
			  
			  Reason: Strings declared as above are usually allocated from memory that can only
	                  be read from not written to.
			  
			  Instead use: TCHAR myString[<Place array size here!!!>]; 
									OR
						   TCHAR* myString = new TCHAR[<Place array size here!!!>]; 
				
			  To guarantee that myString[] can be written to. */
	static BOOL FindReplace( TCHAR szText[], TCHAR szPattern[], TCHAR szReplacement[] );
	/* Search szText for szPattern and replace every occurrence of szPattern with 
	   szReplacement (ignores case). 
	
	NOTE: (1) szPattern and szReplacement must be the same length since we are dealing with 
			  fixed sized (static or dynamic) arrays. 
		  (2) Function expects character arrays (static or dynamic) TCHAR* pointers initialized
	          using TCHAR* szString = "ababababababa" will cause a crash on an attempt to
	          do a replace on szString.
			  
			  Reason: Strings declared as above are usually allocated from memory that can only
	                  be read from not written to.
			  
			  Instead use: TCHAR myString[<Place array size here!!!>]; 
									OR
						   TCHAR* myString = new TCHAR[<Place array size here!!!>]; 
				
			  To guarantee that myString[] can be written to. */
	
	static BOOL FindReplaceNoCase( TCHAR szText[], TCHAR szPattern[], TCHAR szReplacement[] );
	/* Returns the last offset in (0-based array) szText where szPattern occurs.*/
	static int LastIndexOf( const TCHAR* szText, const TCHAR* szPattern );
	/* Returns the last offset in (0-based array) szText where szPattern occurs (ignores case).*/
	static int LastIndexOfNoCase( const TCHAR* szText, const TCHAR* szPattern );
	/* Function seaches a string for the Nth occurrence of a pattern. */
	static int NthIndexOf( const TCHAR* szText, const TCHAR* szPattern, int nNthIndex );
	/* Function seaches a string for the Nth occurrence of a pattern (ignore case). */
	static int NthIndexOfNoCase( const TCHAR* szText, const TCHAR* szPattern, int nNthIndex );

private:
	/*  Heart of pattern matching engine. The prefix function for a pattern encapsulates 
		knowledge about how the patten matches against shifts of itself. pi[q] stores the 
	    length of the longest prefix of the pattern that is a proper suffix of the pattern.
		This information is then used to rule out searching invalid shifts when pattern
		matching. */
	static void ComputePrefixFunction( const TCHAR* szPattern, int nPatternLen, int pi[] );
};

#endif // !defined(AFX_KMPMATCHER_H__CF353CA0_E90C_4B02_834A_008562EBBA41__INCLUDED_)
