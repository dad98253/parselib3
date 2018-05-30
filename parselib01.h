/*
 * parselib01.h
 *
 *  Created on: May 25, 2018
 *      Author: dad
 */

#ifndef PARSELIB01_H_
#define PARSELIB01_H_


#ifndef PARSELIBMAIN
#define EXTERN		extern
#define INITIZERO
#define INITSZERO
#define INITBOOLFALSE
#define INITBOOLTRUE
#define INITNULL
#define INITSNULL
#define INITBUFFERSIZE
#define INITDIRSIZE
#else  // PARSELIBMAIN
#define EXTERN
#define INITIZERO	=0
#define INITSZERO	={0}
#define INITBOOLFALSE	=false
#define INITBOOLTRUE	=true
#define INITNULL	=NULL
#define INITSNULL	={NULL}
#define INITBUFFERSIZE	=BUFFERSIZE
#define INITDIRSIZE		=DIRSIZE
#endif  // PARSELIBMAIN


#define NUMCFHANDLES	20
#define __iscsymf(c) (isalpha(c) || ((c) == '_'))
#define __iscsym(c)  (isalnum(c) || ((c) == '_'))

//EXTERN int numCFHandles INITIZERO;
//EXTERN char *CFileHandles[NUMCFHANDLES] INITSNULL;

//EXTERN char ** tempIntResource INITNULL;
//EXTERN char ** ResourceName INITNULL;
//EXTERN char ** ResourceValue INITNULL;
//EXTERN char ** ResourceType INITNULL;
//EXTERN int NumTempIntResources INITIZERO;
//EXTERN int NumResources INITIZERO;
EXTERN char ** ParseLineRCItems INITNULL;
EXTERN int ParseLineRCNumItems INITIZERO;
EXTERN int * ParseLineRCfmtItem INITNULL;
EXTERN int LengthOfInputLine INITIZERO;
EXTERN char ** SetParseFmtDelims INITNULL;
EXTERN int SetParseFmtNumDelims INITIZERO;
EXTERN int SetParseFmtfmt INITIZERO;
//EXTERN int ResLineNum INITIZERO;

#ifndef RESDEFS
//EXTERN int ResourcelibInit();
//EXTERN int CompareStringResource(const void *m1, const void *m2);
#endif // RESDEFS

#endif /* RESOURCES_H_ */
