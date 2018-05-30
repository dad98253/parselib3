/*
 * PutToItem.cpp
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
#ifndef BSD
#include <malloc.h>
#else	// BSD
#include <stdlib.h>
#endif	// BSD
#include <ctype.h>
#include <string.h>

//#include "Resources.h"
#include "parselib01.h"

int PutToItem(char ctemp, bool bpush, int datatype){
	int i;
	int lenlin;
#ifdef DEBUGPUTTOITEM
	if (ctemp == '\000' ) {
			dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM, "PutToItem called with zero ctemp, push = %u, type = %i\n", bpush, datatype);
	} else {
		  dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM, "PutToItem called with ctemp = \"%c\", push = %u, type = %i\n", ctemp,  bpush, datatype);
	}
	dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"ParseLineRCNumItems = %i, ParseLineRCItems[ParseLineRCNumItems] = 0x%llx\n",ParseLineRCNumItems,(unsigned long long)ParseLineRCItems[ParseLineRCNumItems]);
#endif
	if(ParseLineRCItems[ParseLineRCNumItems] != NULL ) {
		lenlin = strlen(ParseLineRCItems[ParseLineRCNumItems]);
	} else {
		lenlin = 6000000;
	}
#ifdef DEBUGPUTTOITEM
	dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"ParseLineRCItems[%i](=%llx), lenlin=%i, LengthOfInputLine=%i\n",ParseLineRCNumItems,(unsigned long long)ParseLineRCItems[ParseLineRCNumItems],lenlin,LengthOfInputLine);
#endif
	if ( lenlin > LengthOfInputLine ){ ////////////////////////////////////////////////////////
		  if ( ( ParseLineRCItems[ParseLineRCNumItems] = (char*)realloc(ParseLineRCItems[ParseLineRCNumItems],sizeof(char)*(LengthOfInputLine+22) ) ) == NULL ) {
#ifdef DEBUGPUTTOITEM
			  dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"realloc failed in PutToItem for ParseLineRCItems[%i] increase\n",ParseLineRCNumItems);
#endif
			  return(-1);
		  }
		  LengthOfInputLine+=20;
	}
	if ( lenlin == 6000000 ) {
		*ParseLineRCItems[ParseLineRCNumItems] = '\000';
		lenlin = strlen(ParseLineRCItems[ParseLineRCNumItems]);
#ifdef DEBUGPUTTOITEM
		dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"at (lenlin == 6000000) in PutToItem, new lenlin=%i, ParseLineRCItems[%i](=%llx), *ParseLineRCItems[%i] = 0x%x\n",lenlin,ParseLineRCNumItems,(unsigned long long)ParseLineRCItems[ParseLineRCNumItems],ParseLineRCNumItems,*ParseLineRCItems[ParseLineRCNumItems]);
#endif
	}
	  *(ParseLineRCItems[ParseLineRCNumItems]+lenlin) = ctemp;
#ifdef DEBUGPUTTOITEM
	  if (ctemp == '\000') {
		dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"ctemp(=NULL) stuffed into *ParseLineRCItems[%i] at 0x%llx\n",ParseLineRCNumItems,(unsigned long long)(ParseLineRCItems[ParseLineRCNumItems]+lenlin));
	  } else {
		dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"ctemp(=%c) stuffed into *ParseLineRCItems[%i] at 0x%llx\n",*(ParseLineRCItems[ParseLineRCNumItems]+lenlin),ParseLineRCNumItems,(unsigned long long)(ParseLineRCItems[ParseLineRCNumItems]+lenlin));
	  }
#endif
	  lenlin++;
	  *(ParseLineRCItems[ParseLineRCNumItems]+lenlin) = '\000';
	  if ( datatype == 0 ){
		  ParseLineRCfmtItem[ParseLineRCNumItems] = 2;
		  int NumNums = 0;
		  for(i=0;i<lenlin;i++) {
			  if(!isprint(*(ParseLineRCItems[ParseLineRCNumItems]+i)))continue;
			  if(!isdigit(*(ParseLineRCItems[ParseLineRCNumItems]+i))){
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
	  if ( bpush ) {
#ifdef DEBUGPUTTOITEM
	dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"realloc being called in PutToItem for ParseLineRCItems[%i](=%llx), lenlin=%i\n",ParseLineRCNumItems,(unsigned long long)ParseLineRCItems[ParseLineRCNumItems],lenlin+2);
#endif
		  if ( ( ParseLineRCItems[ParseLineRCNumItems] = (char*)realloc(ParseLineRCItems[ParseLineRCNumItems],sizeof(char)*(lenlin+2) ) ) == NULL ) {
#ifdef DEBUGPUTTOITEM
			  dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"realloc failed in PutToItem for ParseLineRCItems[0]\n");
#endif
			  return(-1);
		  }
		  ParseLineRCNumItems++;
		  if ( ( ParseLineRCItems[ParseLineRCNumItems] = (char*)malloc(sizeof(char)*(LengthOfInputLine+2) ) ) == NULL ) {
#ifdef DEBUGPUTTOITEM
			  dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"malloc failed in PutToItem for ParseLineRCItems[0]\n");
#endif
			  return(-1);
		  }
		  *ParseLineRCItems[ParseLineRCNumItems] = '\000';
#ifdef DEBUGPUTTOITEM
		  dfprintf(__LINE__,__FILE__,DEBUGPUTTOITEM,"malloc called in PutToItem for ParseLineRCItems[%i](=%llx), lenlin=%i\n",ParseLineRCNumItems,(unsigned long long)ParseLineRCItems[ParseLineRCNumItems],LengthOfInputLine+2);
#endif
	  }
	  return (ParseLineRCNumItems);
}
