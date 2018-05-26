/*
 * IsDelim.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: dad
 */

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
#include <ctype.h>

//#include "Resources.h"
#include "parselib01.h"

#define BOOL bool
#ifndef WINDOZE
#define TRUE true
#define FALSE false
#endif

BOOL IsDelim (char C){
	int i;
	if ( SetParseFmtNumDelims < 2 ) return (FALSE);
	for (i=1;i<SetParseFmtNumDelims;i++) {
		if ( C == *SetParseFmtDelims[i] ) return (TRUE);
	}
	return (FALSE);
}
