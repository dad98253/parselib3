/*
 * AddToParseList.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: dad
 */

#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

//#include "StdAfx.h"

#ifndef BSD
#include <malloc.h>
#else	// BSD
#include <stdlib.h>
#endif	// BSD
#include <ctype.h>
#include <string.h>

//#include "Resources.h"
#include "parselib01.h"
#include "debug.h"

int AddToParseList(char * ctemp, int lenlin, int datatype){
	int i;
#ifdef DEBUGPARSELIST
	if (ctemp[lenlin] != '\000' ) {
			dfprintf2(__LINE__,__FILE__,DEBUGPARSELIST, "AddToParseList called with non terminated ctemp, lenlin = %i\n", lenlin );
	} else {
		  dfprintf2(__LINE__,__FILE__,DEBUGPARSELIST, "AddToParseList called with ctemp = \"%s\", lenlin = %i\n", ctemp , lenlin );
	}
	dfprintf2(__LINE__,__FILE__,DEBUGPARSELIST,"ParseLineRCNumItems = %i\n",ParseLineRCNumItems);
#endif
	if ( ( ParseLineRCItems[ParseLineRCNumItems] = (char*)malloc(sizeof(char)*(lenlin+2) ) ) == NULL ) {
#ifdef DEBUG
		  dfprintf2(__LINE__,__FILE__,TRACE,"malloc failed in AddToParseList for ParseLineRCItems[0]\n");
#endif
		  return(-1);
	}
	  if ( lenlin > 0 ) strncpy(ParseLineRCItems[ParseLineRCNumItems], ctemp, lenlin);
	  *(ParseLineRCItems[ParseLineRCNumItems]+lenlin) = '\000';
	  if ( datatype == 0 ){
		  ParseLineRCfmtItem[ParseLineRCNumItems] = 2;
		  int NumNums = 0;
		  for(i=0;i<lenlin;i++) {
			  if(!isprint(ctemp[i]))continue;
			  if(!isdigit(ctemp[i])){
				  ParseLineRCfmtItem[ParseLineRCNumItems] = 1;
				  break;
			  }
			  NumNums++;
		  }
		  if( NumNums == 0 && ParseLineRCfmtItem[ParseLineRCNumItems] == 2 )ParseLineRCfmtItem[ParseLineRCNumItems] = 1;
	  } else {
		  ParseLineRCfmtItem[ParseLineRCNumItems] = datatype;
	  }
	  if(lenlin == 0)ParseLineRCfmtItem[ParseLineRCNumItems] = 0;
	  ParseLineRCNumItems++;
	  return (ParseLineRCNumItems);

}
