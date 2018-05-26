#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

#include <stdio.h>
#include <string.h>
// read a line of data from a file while stripping off line feed character at the end
// fp     is a pointer to a file (type FILE)
// p      is either a pointer to the string to receive the line or a pointer to the integer to be
//        read from the file
// length is the maximum number of number of characters to read from the file. If length
//        is negative, read a maximum of 25 characters and check to see if the string can be
//        converted to an integer number. Return -1 as the function value returned by inputl
//        if an integer value is found. return -9999 otherwise. Store the integer value in
//        the word pointed to by p.
//        If length is set to a positive string length, determin how long the actual string is
//        (after stripping lf character) and return that length as the value of inputl. If
//        no string is found, return -9999. Store the string read in the string pointed to by p.
// inputl is either the length of the line or a error flag. How it is set depends on the
//        sign of the length input argument (see above)
int inputl(FILE * fp, void * p, int length)
{
	int num, numt;
	char ctemp[30];
	if (length < 0 ) {

// function called with a negative length value
		// get 25 characters from the input file
		if (fgets ( ctemp, 25, fp) == NULL) return (-9999);
		int numtold = numt = strlen(ctemp);
// check if the last character is a line feed and CR
		if ( numt > 0 && ( ctemp[numt-1] == '\012' || ctemp[numt-1] == '\015' ) )ctemp[numt-1] = '\0';
		numt = strlen(ctemp);
		if ( numt > 0 && ( ctemp[numt-1] == '\012' || ctemp[numt-1] == '\015' ) )ctemp[numt-1] = '\0';
		numt = strlen(ctemp);
// if the line only contained LF or CR, read one more line (e.g., could be a windows file being read on linux)
		if ( numt == 0 && numtold > 0 ) {
		// get 25 characters from the input file
				if (fgets ( ctemp, 25, fp) == NULL) return (-9999);
				numt = strlen(ctemp);
		// check if the last character is a line feed and CR
				if ( numt > 0 && ( ctemp[numt-1] == '\012' || ctemp[numt-1] == '\015' ) )ctemp[numt-1] = '\0';
				numt = strlen(ctemp);
				if ( numt > 0 && ( ctemp[numt-1] == '\012' || ctemp[numt-1] == '\015' ) )ctemp[numt-1] = '\0';
				numt = strlen(ctemp);
		}
		if ( numt == 0 ) return (-9999);
// see if we can convert the string to an integer (maximum of 15 digits). If not, return an
// appropriate error code. Store the integer in the address pointed to by p.		
		if ((num = sscanf(ctemp, "%15i", (int*)p)) == EOF ) return (-9999);   // note: changed (int) cast to (int*) 12-15-2015... why did this work on vc6???
		if (num != 1) return (-9999);  // note: changed test from != 0 to != 1 on  12-15-2015 after adding parentheses around assignment of num in previous line... I think num may have been returning the EOF test rather than number of items converted
// an integer was found. Return -1 as the success flag.
		return (-1);

// function called with a positive length value
// get length characters from the input file
	} else {
// function called with a non-negative length value
// get up to "length" characters from the file. Store the characters in the address pointed to by p.
		if (fgets ( (char *)p, length, fp) == NULL) return (-9999);
//		char * szTempxx;
//		szTempxx = fgets ( (char *)p, length, fp);
//		if ( szTempxx == NULL) return (-9999);
// determine how many characters were read and strip the line feed and CR
		int numold = num = strlen((char *)p);
		if ( num > 0 && ( *((char *)p+num-1) == '\012' || *((char *)p+num-1) == '\015' ) ) *((char *)p+num-1) = '\0';
		num = strlen((char *)p);
		if ( num > 0 && ( *((char *)p+num-1) == '\012' || *((char *)p+num-1) == '\015' ) ) *((char *)p+num-1) = '\0';
// if the line only contained LF or CR, read one more line (e.g., could be a windows file being read on linux)
		if ( num == 0 && numold > 1 ) {
		// get up to "length" characters from the file. Store the characters in the address pointed to by p.
				if (fgets ( (char *)p, length, fp) == NULL) return (-9999);
		// determine how many characters were read and strip the line feed and CR
				num = strlen((char *)p);
				if ( num > 0 && ( *((char *)p+num-1) == '\012' || *((char *)p+num-1) == '\015' ) ) *((char *)p+num-1) = '\0';
				num = strlen((char *)p);
				if ( num > 0 && ( *((char *)p+num-1) == '\012' || *((char *)p+num-1) == '\015' ) ) *((char *)p+num-1) = '\0';
		}
		return (strlen((char *)p));
	}
}
