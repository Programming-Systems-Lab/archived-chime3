#ifndef EXPORTS_H
#define EXPORTS_H

#ifdef EXPORT_COMMON
	#define CommonExport __declspec( dllexport ) 
#else 
	#define CommonExport __declspec( dllimport ) 
#endif


#endif