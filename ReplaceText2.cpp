#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif


//#include "StdAfx.h"
#ifdef WINDOZE
#include <windows.h>
#else
#include "lindows.h"
#endif
//#include <stdio.h>
//#include <stdlib.h>
//#include <tchar.h>

#ifndef BSD
#include <malloc.h>
#else	// BSD
#include <stdlib.h>
#endif	// BSD
#ifdef WINDOZE
#include <stdarg.h>
//#include <string>
#include <string.h>
#else
#include <stdarg.h>
#include <string.h>
#endif
#include "msgbox.h"
#include "debug.h"
//#include "options.h"
//#include "globals.h"
//#include "passwin.h"

int ReplaceText2( const char * szStringIn , char ** lpsEditedString , int NumReplacements , va_list ap ) {
//	va_list ap;
//	va_start(ap, NumReplacements);	/* Initialize variable arguments. */

	char** StringToReplaceList = NULL;
	char** SubstituteStringList = NULL;
	char * lpsEditedResource = NULL;
	char * lpsResourceData = (char *)szStringIn;
	int LenResource = strlen(lpsResourceData);
	int * iPos = NULL;
	int * nSubs = NULL;

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE, "calling ReplaceText2, NumReplacements = %i\n", NumReplacements);
#endif

	if ( NumReplacements > 0 )
	{
		StringToReplaceList = (char **)malloc( NumReplacements*sizeof(*StringToReplaceList) );
		if ( StringToReplaceList == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "malloc for StringToReplaceList failed at line %i in ReplaceText2\n", __LINE__);
#endif
		MsgBox("out of memory at line %i in ReplaceText2",__LINE__);
		return(-999);
		}
		SubstituteStringList = (char **)malloc( NumReplacements*sizeof(*SubstituteStringList) );
		if ( SubstituteStringList == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "malloc for SubstituteStringList failed at line %i in ReplaceText2\n", __LINE__);
#endif
		MsgBox("out of memory at line %i in ReplaceText2",__LINE__);
		return(-999);
		}
		iPos = (int *)malloc( NumReplacements*sizeof(*iPos) );
		if ( iPos == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "malloc for iPos failed at line %i in ReplaceText2\n", __LINE__);
#endif
		MsgBox("out of memory at line %i in ReplaceText2",__LINE__);
		return(-999);
		}
		nSubs = (int *)malloc( NumReplacements*sizeof(*nSubs) );
		if ( nSubs == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "malloc for nSubs failed at line %i in ReplaceText2\n", __LINE__);
#endif
		MsgBox("out of memory at line %i in ReplaceText2",__LINE__);
		return(-999);
		}
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "sizeof StringToReplaceList(0x%x-0x%x) = %i\n", StringToReplaceList,StringToReplaceList+NumReplacements*sizeof(*StringToReplaceList),NumReplacements*sizeof(*StringToReplaceList));
		dfprintf(__LINE__,__FILE__,TRACE, "sizeof SubstituteStringList(0x%x-0x%x) = %i\n", SubstituteStringList,SubstituteStringList+NumReplacements*sizeof(*SubstituteStringList),NumReplacements*sizeof(*SubstituteStringList));
		dfprintf(__LINE__,__FILE__,TRACE, "sizeof iPos(0x%x-0x%x) = %i\n",iPos,iPos+NumReplacements*sizeof(*iPos),NumReplacements*sizeof(*iPos));
		dfprintf(__LINE__,__FILE__,TRACE, "sizeof nSubs(0x%x-0x%x) = %i\n", nSubs,nSubs+NumReplacements*sizeof(*nSubs),NumReplacements*sizeof(*nSubs));
#endif
		int i;
		int iDeltaSizeToWrite = 0;

		for (i=0;i<NumReplacements;i++)
		{
			*(StringToReplaceList+i) = va_arg(ap, char *);
			*(SubstituteStringList+i) = va_arg(ap, char *);

			int LenSubstitute = strlen(*(SubstituteStringList+i));
			LPSTR lpsStringToReplace = *(StringToReplaceList+i);
#ifdef DEBUG
			LPSTR SubstituteString = *(SubstituteStringList+i);
			dfprintf(__LINE__,__FILE__,TRACE, "for i = %i, lpsStringToReplace = \"%s\", SubstituteString = \"%s\"\n", i, lpsStringToReplace, SubstituteString);
#endif
			int LenOriginal = strlen(lpsStringToReplace);
			char* lpstrloc = strstr(lpsResourceData , lpsStringToReplace);
			*(nSubs+i) = 0;

// count the number of occurances
			while ( lpstrloc != NULL && (lpstrloc+LenOriginal) < lpsResourceData+LenResource+1 )
			{
				(*(nSubs+i))++;
				lpstrloc = strstr(lpstrloc+LenOriginal , lpsStringToReplace);
			}
// set iPos to the first one
			lpstrloc = strstr(lpsResourceData , lpsStringToReplace);
			if ( lpstrloc == NULL )
			{
				*(iPos+i) = -1;
#ifdef DEBUG
			if ( LenResource > 150 ) {
				dfprintf(__LINE__,__FILE__,TRACE, "Could Not Find \"%s\" in the text supplied\n", lpsStringToReplace);
			} else {
				dfprintf(__LINE__,__FILE__,TRACE, "Could Not Find \"%s\" in the following text:\n%s\n", lpsStringToReplace,lpsResourceData);
			}
#endif
				return (-2);
			} else {
				*(iPos+i) = lpstrloc - lpsResourceData;
			}

			if ( (LenSubstitute - LenOriginal) > 0 ) iDeltaSizeToWrite+= (LenSubstitute - LenOriginal) * (*(nSubs+i)) ;
		}

//Determine the size to write the file
//If length is greater, then nothing to worry about
//if smaller, then we must calculate size to shrink
//file in order to cut off excess superflous data
		int iSizeToWrite = LenResource + iDeltaSizeToWrite;

//Using the Replace Function To change
//String Data

		lpsEditedResource = (char *)malloc(iSizeToWrite+1);

		int iNextReplaceIndex = -1024;
		int iNextReplacePos = iSizeToWrite+2;

// now find which iPos is smaller... replace it and loop through all replacement pairs untill all iPos values are -1
//To Replace the whole file
		for (i=0;i<NumReplacements;i++)
		{
			if ( *(iPos+i) < iNextReplacePos )
			{
				iNextReplacePos = *(iPos+i);
				iNextReplaceIndex = i;
			}
		}

		LPSTR lpsNextEdit;
		LPSTR lpsNextOriginal;
		LPSTR lpsStringToReplace;
		LPSTR SubstituteString;
		int LenSubstitute;
		char* lpstrloc;
		lpsNextEdit = lpsEditedResource;
		lpsNextOriginal = lpsResourceData;

		while ( iNextReplaceIndex != -1024 )
		{
			lpsStringToReplace = *(StringToReplaceList+iNextReplaceIndex);
			int LenOriginal = strlen(lpsStringToReplace);
			SubstituteString = *(SubstituteStringList+iNextReplaceIndex);
			LenSubstitute = strlen(SubstituteString);

			lpstrloc = strstr(lpsNextOriginal , lpsStringToReplace);
			if ( lpstrloc == NULL )
			{
				*(iPos+iNextReplaceIndex) = iSizeToWrite+2;
#ifdef DEBUG
				dfprintf(__LINE__,__FILE__,TRACE, "Imposible error at line %i for %s at position %i \n", __LINE__, lpsStringToReplace, *(iPos+iNextReplaceIndex));
#endif
				break;
			}
			else {
				*(iPos+iNextReplaceIndex) = lpstrloc - lpsResourceData;
			}
			while (lpsNextOriginal<lpstrloc)
			{
				*lpsNextEdit = *lpsNextOriginal;
				lpsNextOriginal++;
				lpsNextEdit++;
			}
			*lpsNextEdit = '\000';
			strcat(lpsNextEdit , SubstituteString);
			lpsNextEdit+=LenSubstitute;
			lpsNextOriginal+=LenOriginal;

			*lpsNextEdit = '\000';

			lpstrloc = strstr(lpsNextOriginal , lpsStringToReplace);
			if ( lpstrloc == NULL )
			{
				*(iPos+iNextReplaceIndex) = iSizeToWrite+2;
			}
			else {
				*(iPos+iNextReplaceIndex) = lpstrloc - lpsResourceData;
			}

			iNextReplaceIndex = -1024;
			iNextReplacePos = iSizeToWrite+2;
			for (i=0;i<NumReplacements;i++)
			{
				if ( *(iPos+i) < iNextReplacePos )
				{
					iNextReplacePos = *(iPos+i);
					iNextReplaceIndex = i;
				}
			}
		}

		strcat(lpsNextEdit , lpsNextOriginal);
		free(StringToReplaceList);
		StringToReplaceList = NULL;
		free(SubstituteStringList);
		SubstituteStringList = NULL;
		free(iPos);
		iPos = NULL;
		free(nSubs);
		nSubs = NULL;



	} else {		// end of if,then on NumReplacements > 0
		lpsEditedResource = (char *)malloc(LenResource+1);
		strcpy(lpsEditedResource,lpsResourceData);
	}				// end of if,then, else on NumReplacements > 0


	*lpsEditedString = lpsEditedResource;

	return((int)strlen((const char *)lpsEditedResource));
}


