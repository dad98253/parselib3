/*
 * SetParseFmt.cpp
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

#include <stdarg.h>
#include <stdio.h>
#ifndef BSD
#include <malloc.h>
#else	// BSD
#include <stdlib.h>
#endif	// BSD
#include <string.h>
#include "parselib01.h"
//#include "Resources.h"

void SetParseFmt(int fmt, int numdelim, ...){
//	fmt			format id ?
//	numdelim	number of delimiters used for this format
//	...			(char*) the delimiters
	va_list args;
	if (fmt==0){
		if(SetParseFmtfmt==0)return;
		SetParseFmtfmt=fmt;
		if(SetParseFmtNumDelims==0)return;
	}
	int i;
	for(i=0;i<SetParseFmtNumDelims;i++){
		if(SetParseFmtDelims[i] != NULL){
			free(SetParseFmtDelims[i]);
			SetParseFmtDelims[i] = NULL;
		}
	}
	if (SetParseFmtDelims != NULL ) free(SetParseFmtDelims);
	SetParseFmtDelims = NULL;
	SetParseFmtNumDelims=0;		// needed if we want to re-use SetParseFmt
	if (numdelim==0)return;
	if(fmt!=1){
		printf("bad SetParseFmt call, fmt=%i\n",fmt);
		SetParseFmtfmt=0;
		SetParseFmtNumDelims=0;
		return;
	}
	if ( SetParseFmtNumDelims != 0 ) {
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"SetParseFmt problem at line %i. Data not properly initilaized prior to call...\n");
#endif
		return;
	}
	SetParseFmtNumDelims = numdelim;
	SetParseFmtfmt = fmt;
	SetParseFmtDelims = (char**)realloc(SetParseFmtDelims,sizeof(char*)*numdelim);
	if (SetParseFmtDelims == NULL) {
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"SetParseFmt unable to allocate space for delims\n");
#endif
		return;
	}
	char * delim;
	va_start(args, numdelim);
	for(i=0;i<SetParseFmtNumDelims;i++){
		delim = va_arg(args, char*);
		SetParseFmtDelims[i] = (char*)calloc(1,sizeof(char)*strlen(delim));
		if(SetParseFmtDelims[i] == NULL){
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"SetParseFmt unable to allocate space for SetParseFmtDelims[%i]\n",i);
#endif
		return;
		}
		strcpy(SetParseFmtDelims[i],delim);
	}
	va_end(args);
#ifdef DEBUGSETPARSEFMT
	dfprintf(__LINE__,__FILE__,DEBUGSETPARSEFMT,"SetParseFmt delims = ");
	for(i=0;i<SetParseFmtNumDelims;i++) dfprintf(__LINE__,__FILE__,DEBUGSETPARSEFMT,"\"%s\"   ",SetParseFmtDelims[i]);
	dfprintf(__LINE__,__FILE__,DEBUGSETPARSEFMT,"\n");
#endif
	return;
}
