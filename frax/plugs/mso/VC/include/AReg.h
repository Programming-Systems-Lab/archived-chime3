// AReg.h: interface for the AReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREG_H__A089907F_45E5_44B9_AB32_41D6ADDD8668__INCLUDED_)
#define AFX_AREG_H__A089907F_45E5_44B9_AB32_41D6ADDD8668__INCLUDED_
#include <AString.h>
#include <windows.h>
#include "exports.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CommonExport AReg  
{
public:
	AReg();
	virtual ~AReg();
	
/*	Funciton attempts to open registry key and subkey then query 
	subkey value byte data retrieved from registry returned in 
	AString szRetData.
	
	Input paramters: hKeyIN - registry key to query
					 szSubKey - sub key to query
					 szRetData - Refernce to AString to store byte data
								 returned (if any)

	Return values:	Function returns the type of data stored in reg key 
				e.g REG_SZ, REG_DWORD etc. so that any necessary 
				conversions can be done on szRetData. 
				
				On error returns REG_NONE as type */
	static DWORD GetRegSubKeyValue( HKEY hKey,
									AString szSubKey,
									AString szValueName,
									AString& szRetData ); 


	

};

#endif // !defined(AFX_AREG_H__A089907F_45E5_44B9_AB32_41D6ADDD8668__INCLUDED_)
