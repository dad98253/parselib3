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
#include <string.h>
#include <stdlib.h>
#ifndef BSD
#include <malloc.h>
#else	// BSD
#include <stdlib.h>
#endif	// BSD
#include <stdio.h>


int storestring(char **p, char * string, int length)
{
	int trueLength = 0;
	trueLength = strlen(string);
	if ( length < trueLength ) trueLength = length;
	*p = (char*)malloc(trueLength+1);
	
	if (*p != NULL) {
		strcpy (*p,string);
		return (0);
	}
	return (1);
}

int AllocatePointerVector(void **p, size_t size, int length)
{
//	int i;

//	*p = (void*)malloc((length+1)*size);
	*p = (void*)calloc((length+1), size);
	
	if (*p != NULL) {
//		for (i=0;i<length;i++){
//			*(p+i) = NULL;
//		}
		return (0);
	}
	return (1);
}

int ReAllocatePointerVector(void **p, size_t size, int length)
{
//	int i;
//	void *xx;
//	int izize;

	if ( *p != NULL ) {
		int isize;
		isize = (int)_msize( (void*)*p );
//		printf( "Size of block: %u\n", isize );
		if ( length == 0 ) return (isize);
   /* Reallocate and show new size: */
		if( (*p = realloc( *p, isize + ((length+1) * size) )) ==  NULL ) return (0);
//		izize = isize / size;
		isize = _msize( *p );
/*		for (i=0;i<=length;i++){
			*(p+izize+i) = NULL;
			xx = p+izize+i;
			fprintf(stdout, "i = %i, p = %X, xx = %x\n",i,p,xx);
			fflush(stdout);
		} */
//		printf( "Size of block after realloc: %u\n", isize );
		return (isize);
	}

//	*p = (void*)malloc((length+1)*size);
	*p = (void*)calloc((length+1), size);
	
	if (*p != NULL) {
//		for (i=0;i<length;i++){
//			*(p+i) = NULL;
//		}
		return ((length+1)*size);
	}
	return (0);

}

int AllocateCharVector(unsigned char **p, int length)
{
	int i;
	*p = (unsigned char*)malloc(length+1);
	
	if (*p != NULL) {
		for (i=0;i<length;i++){
			*(*p+i) = '\000';
		}
		return (0);
	}
	return (1);
}

int ReAllocateCharVector(unsigned char **p, int length)
{
	int i;
	int isize;

	if ( *p != NULL ) {
		isize = (int)_msize( (void *)*p );
#ifdef DEBUGREALLOCATECHAR
printf( "Size of block (%llx) (len=%i) before realloc: %u\n", *p, length, isize );
#endif
		if ( length == 0 ) return (isize);

   /* Reallocate and show new size: */
		i = isize + length + 1;
		if( (*p = (unsigned char*)realloc( *p, i) ) ==  NULL ) return (0);
		isize = _msize( *p );
#ifdef DEBUGREALLOCATECHAR
printf( "Size of block (%llx) after realloc: %u (requested %i)\n", *p, isize ,i);
#endif
		return (isize);
	}


	*p = (unsigned char*)malloc(length+1);
#ifdef DEBUGREALLOCATECHAR
printf( "Address of block (%llx) after malloc: size >= %i\n", *p, length+1 );
#endif
	if (*p != NULL) {
		for (i=0;i<length;i++){
			*(*p+i) = '\000';
		}
		return (length+1);
	}
	return (0);
}
