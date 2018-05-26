/*
 * ParseLineRC.cpp
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
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define PARSELIBMAIN
#include "parselib01.h"

#define BOOL bool

#ifdef WINDOZE
#define LONGLONG	__int64
#else	// WINDOZE
#define LONGLONG long long
#endif	// WINDOZE

extern int AddToParseList(char * ctemp, int lenlin, int datatype);
extern int PutToItem(char ctemp, bool bpush, int datatype);
extern int ParseLineRC(char* ctemp);
extern BOOL IsDelim (char C);

int ParseLineRC(char* ctemp){
	  enum STATE {
	      INITIAL,
		  ITEM,
	      STRING,
		  DELIM0,
		  DELIM1,
		  CCOMMENT00,
	      CCOMMENT01,
	      CCOMMENT02,
	      CCOMMENT10,
	      CCOMMENT11,
	      CCOMMENT12,
		  PRAGMA
	  } state = INITIAL;
	  int i;
	  char *ct;
	  int lenlin;
#ifdef DEBUGPARSELINE
	  WinFprintf(fp9,"ParseLineRC called with \"%s\" (len=%lu)\n",ctemp,strlen(ctemp));
#endif
	  if (ParseLineRCNumItems != 0){
			for(i=0;i<ParseLineRCNumItems;i++){
				if(ParseLineRCItems[i] != NULL){
#ifdef DEBUGPARSELINE
	WinFprintf(fp9,"free being called in ParseLineRC for ParseLineRCItems[%i](=%llx)\n",i,(unsigned LONGLONG)ParseLineRCItems[i]);
#endif
					free(ParseLineRCItems[i]);
					ParseLineRCItems[i] = NULL;
				}
			}
			if (ParseLineRCItems != NULL) {
#ifdef DEBUGPARSELINE
	WinFprintf(fp9,"free being called in ParseLineRC for ParseLineRCItems(=%llx)\n",(unsigned LONGLONG)ParseLineRCItems);
#endif
				free(ParseLineRCItems);
			}
			if (ParseLineRCfmtItem != NULL){
#ifdef DEBUGPARSELINE
	WinFprintf(fp9,"free being called in ParseLineRC for ParseLineRCfmtItem(=%llx)\n",(unsigned LONGLONG)ParseLineRCfmtItem);
#endif
				free(ParseLineRCfmtItem);
			}
			ParseLineRCItems = NULL;
			ParseLineRCfmtItem = NULL;
			ParseLineRCNumItems = 0;
	  }
	  ct=ctemp;
	  if ( (lenlin = strlen(ct)) == 0 ) return (0);
	  if (SetParseFmtfmt != 1) return (-1);
	  if (ParseLineRCItems==NULL){
#ifdef DEBUGPARSELINE
			  WinFprintf(fp9,"calling calloc in ParseLineRC for ParseLineRCItems with nmemb=%i, size=%i\n",lenlin+2,(int)sizeof(char*));
#endif
//	  if ((ParseLineRCItems = (char**)malloc(sizeof(char*)*(lenlin+2))) == NULL ){
	  if ((ParseLineRCItems = (char**)calloc(lenlin+2,sizeof(char*))) == NULL ){

#ifdef DEBUGPARSELINE
			  WinFprintf(fp9,"calloc failed in ParseLineRC for ParseLineRCItems\n");
#endif
			  return(-1);
		  }
#ifdef DEBUGPARSELINE
		  WinFprintf(fp9,"calloc called in ParseLineRC for ParseLineRCItems(=%llx) set to %lu (%i) bytes\n",(unsigned LONGLONG)ParseLineRCItems,_msize(ParseLineRCItems),(lenlin+2)*8);
#endif
//		  int tempi =_msize(ParseLineRCItems);
//		  for(i=0;i<tempi;i++)*(char*)(ParseLineRCItems+i)=0;
	  }
	  if (ParseLineRCfmtItem==NULL){
		  if ((ParseLineRCfmtItem = (int*)malloc(sizeof(int)*(lenlin+2))) == NULL ){
#ifdef DEBUGPARSELINE
			  WinFprintf(fp9,"malloc failed in ParseLineRC for ParseLineRCfmtItem\n");
#endif
			  return(-1);
		  }
#ifdef DEBUGPARSELINE
		  WinFprintf(fp9,"malloc called in ParseLineRC for ParseLineRCfmtItem(=%llx) set to %lu (%i) bytes\n",(unsigned LONGLONG)ParseLineRCfmtItem,_msize(ParseLineRCfmtItem),(lenlin+2)*4);
#endif
	  }
	  if ( SetParseFmtNumDelims == 0 || lenlin == 1 ) {
		  if ( AddToParseList(ctemp, lenlin, 0) < 1 ) {
#ifdef DEBUGPARSELINE
			  WinFprintf(fp9,"AddToParseList special return of =-1\n");
#endif
			  return (-1);
		  }
#ifdef DEBUGPARSELINE
			  WinFprintf(fp9,"AddToParseList special return of =1\n");
#endif
		  return (1);
/*		  if ( ( ParseLineRCItems[0] = (char*)malloc(sizeof(char)*(lenlin+2) ) ) == NULL ) {
#ifdef DEBUG
			  WinFprintf(fp9,"malloc failed in ParseLineRC for ParseLineRCItems[0]\n");
#endif
			  return(-1);
		  }
		  strcpy(ParseLineRCItems[0], ctemp);
		  ParseLineRCfmtItem[0] = 2;
		  int NumNums = 0;
		  for(i=0;i<lenlin;i++) {
			  if(!isprint(ctemp[i]))continue;
			  if(!isdigit(ctemp[i])){
				  ParseLineRCfmtItem[0] = 1;
				  break;
			  }
			  NumNums++;
		  }
		  if( NumNums == 0 && ParseLineRCfmtItem[0] == 2 )ParseLineRCfmtItem[0] = 1;
		  ParseLineRCNumItems=1;
		  return (1); */
	  }
#ifdef DEBUGPARSELINE
			  WinFprintf(fp9,"parsing line\n");
#endif
	  int c[2];
	  int linum;
	  int colnum;
	  int colnumin;

	  bool nextLF;
	  bool isLF;

	  	linum = 1;
	  	colnumin = 0;
	  	colnum = 0;

	  	nextLF = false;
	  	isLF = false;

	  	int in=0;

	    if ((c[0] = ct[in] ) == '\000')  return(0);
	  	colnumin++;
	  	in++;
//	  	if (c[0] == '\n') isLF = true;
	  	if (c[0] == '\n') return(0);

	      while ( c[0] != '\000')
	      {
#ifdef DEBUGPARSE
	    	  WinFprintf(fp9,"c[1] = %c = 0x%x\n",c[1],c[1]);
#endif
	    	c[1] = ct[in];
	    	in++;
	  		colnumin++;
	  		if (isLF) {
	  			colnumin = 0;
	  			isLF = false;
	  		}
	  		if (nextLF) {
	  			isLF = true;
	  			nextLF = false;
	  		}
	  		if (c[1] == '\n') {
#ifdef DEBUGPARSE
	  		WinFprintf(fp9,"c[1] == new line\n");
#endif
	  			linum++;
	  			colnum = 0;
	  			nextLF = true;
	  		}
	        switch (state)
	        {
	        case INITIAL:
/*	        	if ( _msize(ParseLineRCfmtItem) < (unsigned int)ParseLineRCNumItems*4 ) {
	      		  if ((ParseLineRCItems = (char**)realloc(ParseLineRCItems,sizeof(char*)*(ParseLineRCNumItems+12))) == NULL ){
#ifdef DEBUG
	      			  WinFprintf(fp9,"realloc failed in ParseLineRC for ParseLineRCItems\n");
#endif
	      			  return(-1);
	      		  }
#ifdef DEBUG
	      		  WinFprintf(fp9,"realloc for ParseLineRCItems set to %lu (%i) bytes\n",_msize(ParseLineRCItems),(ParseLineRCNumItems+12)*8);
#endif
	      		  if ((ParseLineRCfmtItem = (int*)realloc(ParseLineRCfmtItem,sizeof(int)*(ParseLineRCNumItems+12))) == NULL ){
#ifdef DEBUG
	      			  WinFprintf(fp9,"realloc failed in ParseLineRC for ParseLineRCfmtItem\n");
#endif
	      			  return(-1);
	      		  }
#ifdef DEBUG
	      		  WinFprintf(fp9,"realloc for ParseLineRCfmtItem set to %lu (%i) bytes\n",_msize(ParseLineRCfmtItem),(ParseLineRCNumItems+12)*4);
#endif
	      	  }
	        	if ( ( ParseLineRCItems[ParseLineRCNumItems] = (char*)malloc(sizeof(char)*(lenlin-in+40)) ) == NULL ) {
#ifdef DEBUG
	        		WinFprintf(fp9,"malloc failed in ParseLineRC for ParseLineRCItems[%i]\n",ParseLineRCNumItems);
#endif
	        		return(-1);
	        	}
	        	ParseLineRCfmtItem[ParseLineRCNumItems] = 1;
	        	sprintf(ParseLineRCItems[ParseLineRCNumItems],"test %i size = %lu",ParseLineRCNumItems,_msize(ParseLineRCItems[ParseLineRCNumItems]));
	        	ParseLineRCNumItems++;   */
//	  		fprintf(f4,"%c", c[0]);
	        	colnum++;
#ifdef DEBUGPARSE
	        	WinFprintf(fp9,"INITIAL sate, column = %i, c[0] = %c\n", colnum, c[0]);
#endif
	          if (c[0] == '#' ) {
	        	  state = PRAGMA;
	        	  PutToItem(c[0], true, 3);
	        	  break;
	          }
	          else if (c[0] == '/' && c[1] == '/') {
	        	  PutToItem('\000', true, 4);
	        	  return(0);
	  		  }
	          else if ( c[0] == '/' && c[1] == '*' )
	  		  {
	        	  state = CCOMMENT00;
	        	  break;
	  		  }
	          else if (c[0] == '"' )
	          {
	        	  state = STRING;
	        	  break;
	          }
	          else if ( SetParseFmtfmt == 1 && isspace(c[0]) )
	          {
	        	  state = DELIM1;
#ifdef DEBUGPARSE
	        	  WinFprintf(fp9,"INITIAL state found DELIM1, in = %i\n",in);
#endif
	        	  break;
	          }
	          else if ( SetParseFmtfmt > 0 && IsDelim(c[0]) )
	          {
	        	  state = DELIM0;
	        	  PutToItem('\000', true, 0);
#ifdef DEBUGPARSE
	        	  WinFprintf(fp9,"INITIAL state found DELIM0, in = %i\n",in);
#endif
	        	  break;
	          }
	          else {
	        	  state = ITEM;
	        	  PutToItem(c[0], false, 0);
#ifdef DEBUGPARSE
	        	  WinFprintf(fp9,"INITIAL state found ITEM, in = %i\n",in);
#endif
	          }

	          break;


	        case ITEM:
	        	if ( SetParseFmtfmt == 1 && isspace(c[0]) )
	        	{
	        		state = DELIM1;
	        		PutToItem('\000', true, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"ITEM state found DELIM1, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if ( SetParseFmtfmt > 0 && IsDelim(c[0]) )
	        	{
	        		state = DELIM0;
	        		PutToItem('\000', true, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"ITEM state found DELIM0, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else {
	        		state = ITEM;
	        		PutToItem(c[0], false, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"ITEM state found ITEM, in = %i\n",in);
#endif
	        	}
	        	break;


	        case STRING:
	        	if ( c[0] == '"' )
	        	{
	        		state = DELIM1;
	        		PutToItem('\000', true, 3);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"STRING state found DELIM1, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else {
	        		state = STRING;
	        		PutToItem(c[0], false, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"STRING state found STRING, in = %i\n",in);
#endif
	        	}
	        	break;


	        case DELIM0:
	        	if ( SetParseFmtfmt == 1 && isspace(c[0]) )
	        	{
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM0 state found DELIM0, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if ( SetParseFmtfmt > 0 && IsDelim(c[0]) )
	        	{
	        		state = DELIM0;
	        		PutToItem('\000', true, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM0 state found DELIM1, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if (c[0] == '/' && c[1] == '/') {
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM0 state found //, in = %i\n",in);
#endif
	        		return(0);
	        	}
	        	else if ( c[0] == '/' && c[1] == '*' )
	        	{
	        		state = CCOMMENT00;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM0 state found CCOMMENT00, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if (c[0] == '"' )
	        	{
	        		state = STRING;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM0 state found STRING, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else {
	        		state = ITEM;
	        		PutToItem(c[0], false, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM0 state found ITEM, in = %i\n",in);
#endif
	        		break;
	        	}


	        case DELIM1:
	        	if ( SetParseFmtfmt == 1 && isspace(c[0]) )
	        	{
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM1 state found DELIM0, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if ( SetParseFmtfmt > 0 && IsDelim(c[0]) )
	        	{
	        		state = DELIM0;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM1 state found DELIM1, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if (c[0] == '/' && c[1] == '/') {
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM1 state found //, in = %i\n",in);
#endif
	        		return(0);
	        	}
	        	else if ( c[0] == '/' && c[1] == '*' )
	        	{
	        		state = CCOMMENT00;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM1 state found CCOMMENT00, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if (c[0] == '"' )
	        	{
	        		state = STRING;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM1 state found STRING, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else {
	        		state = ITEM;
	        		PutToItem(c[0], false, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"DELIM1 state found ITEM, in = %i\n",in);
#endif
	        		break;
	        	}


	        case CCOMMENT00:
	        	if (c[0] == '*')
	        	{
	        		state = CCOMMENT01;
	        		break;
	        	} else {
	        		printf("error 1 parsing comment in resource file\n");
	        		return(-1);
	        	}


	        case CCOMMENT01:
	        	if (c[0] == '*' && c[1] == '/')
	        	{
	        		state = CCOMMENT02;
	        		break;
	        	} else {
	        		state = CCOMMENT01;
	        		break;
	        	}


	        case CCOMMENT02:
	        	if (c[0] == '/')
	        	{
	        		state = DELIM0;
	        		break;
	        	} else {
	        		printf("error 2 parsing comment in resource file\n");
	        		return(-1);
	        	}


	        case CCOMMENT10:
	        	if (c[0] == '*')
	        	{
	        		state = CCOMMENT11;
	        		break;
	        	} else {
	        		printf("error 3 parsing comment in resource file\n");
	        		return(-1);
	        	}


	        case CCOMMENT11:
	        	if (c[0] == '*' && c[1] == '/')
	        	{
	        		state = CCOMMENT12;
	        		break;
	        	} else {
	        		state = CCOMMENT11;
	        		break;
	        	}


	        case CCOMMENT12:
	        	if (c[0] == '/')
	        	{
	        		state = DELIM1;
	        		break;
	        	} else {
	        		printf("error 4 parsing comment in resource file\n");
	        		return(-1);
	        	}


	        case PRAGMA:
	        	if ( SetParseFmtfmt == 1 && isspace(c[0]) )
	        	{
	        		state = PRAGMA;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"PRAGMA state found DELIM1, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else if ( SetParseFmtfmt > 0 && IsDelim(c[0]) )
	        	{
	        		state = PRAGMA;
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"PRAGMA state found DELIM0, in = %i\n",in);
#endif
	        		break;
	        	}
	        	else {
	        		state = ITEM;
	        		PutToItem(c[0], false, 0);
#ifdef DEBUGPARSE
	        		WinFprintf(fp9,"PRAGMA state found ITEM, in = %i\n",in);
#endif
	        	}
	        	break;

	        }   // end of case
	        c[0]=c[1];

	   }  // end of while  ...

#ifdef DEBUGPARSE
	WinFprintf(fp9,"at EOL state , DELIM1 = %ui, %ui\n", state, DELIM1);
#endif

	if ( state != DELIM1 ) PutToItem('\000', true, 0);
/*
	if ( ParseLineRCNumItems == 0  ) {
			  if ( AddToParseList(ctemp, lenlin, 0) < 1 ) {
#ifdef DEBUGPARSELIST
				  WinFprintf(fp9,"AddToParseList second special return of =-1\n");
#endif
				  return (-1);
			  }
#ifdef DEBUGPARSELIST
				  WinFprintf(fp9,"AddToParseList second special return of =1\n");
#endif
	}
*/
	return(ParseLineRCNumItems);
}
