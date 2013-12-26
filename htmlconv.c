/* htmlconv.c,v 1.4 1998-10-10 11:39:52-04 rl Exp */

/************************************************************************* 
 *                                                                       * 
 * htmlconv.c                                                            * 
 * Convert text/html emails to text/plain                                * 
 * 1998-01-10, Rolf Lochbuehler                                          * 
 *                                                                       * 
 *************************************************************************/

/*

Some assumptions:
(1) The original email file is of text/html type.
(2) The original email file has a header line saying "Content-Type: text/html"
(3) The original email file may be PGP signed.
(3) As specified in RFC-822, a null line marks the end of a header.

*/


#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


#define AUTHOR "Rolf Lochbuehler"
#define EMAIL "rolf@together.net"
#define PROGRAM "HtmlConv"
#define PROGRAM_CALL "htmlconv"
#define VERSION "1.4"


#define RET_OK 0
#define RET_HELP 1
#define RET_INVALID_ARGUMENT 2
#define RET_FILENAME_MISSING 3


#define NO 0
#define YES 1
#define OFF 0
#define ON 1
#define EQUAL 0

        
/* text/html */

#define MAX_URL 2000

#define HTML_ANCHOR_START "a href=\""
#define HTML_ANCHOR_END "/a"

#define HTML_GT "gt"
#define HTML_LT "lt"
#define HTML_NBSP "nbsp"


/* multipart/alternative */
#define BOUNDARY_STR "gqistsuod1rarnirtsm2dazagnwrt3mkdplagni4vsmobarag5irasgohar6gncrxsmod7"


/* MIME */
#define CONTENT_TYPE "Content-Type:"
#define SUBJECT "Subject:"


/* Functions */
void  help       ( void );
void  html       ( FILE* file, char* start, char* end );
void  header     ( FILE* file, char* start, char* end, char* boundarystr );
void  multipart  ( FILE* file, char* start, char* end );
void  plain      ( FILE* file, char* start, char* end );


/************************************************************************* 
 *                                                                       * 
 * main()                                                                * 
 * Interpret command line options and open/read/write/close files        * 
 *                                                                       * 
 *************************************************************************/
int main ( int argc, char** argv )
  {

  int c;
  int opt;
  FILE *file;
  char filename[FILENAME_MAX];
  int filesize;
  struct stat filebuf;
  char* start;
  char* end;
  char* p;

  switch( argc )
    {

    case 2:
      if( (EQUAL == stricmp(argv[1],"/h")) || (EQUAL == stricmp(argv[1],"-h")) )
        {
        help();
        return( RET_HELP );
        }
      strcpy( filename, argv[1] );
      break;

    case 3:
      if( (EQUAL != stricmp(argv[1],"/h")) && (EQUAL != stricmp(argv[1],"-h")) )
        puts( "\n*"PROGRAM" Error* Too many arguments" );
      help();
      return( RET_HELP );
      break;

    default:
      help();
      return( RET_HELP );

    };   /* end switch */

  /* Open input file */
  file = fopen( filename, "r" );
  if( NULL == file )
    return RET_INVALID_ARGUMENT;

  /* Get size of file */
  fstat( fileno(file), &filebuf );
  filesize = filebuf.st_size;

  /* Allocate memory */
  start = (char*) _alloca( filesize );

  /* Read input file */
  p = start;
  while( (c = fgetc(file)) != EOF ) 
    {
    *p = (char) c;
    p += 1;
    }
  end = p - 1;

  /* Erase contents of input file */
  fclose( file );
  remove( filename );
  file = fopen( filename, "w" );

  /* Write back converted input file */
  multipart( file, start, end );

  /* Close input file */
  fclose( file );

  /* Don't need to free memory, used _alloca() */

  return RET_OK;

  }


/************************************************************************* 
 *                                                                       * 
 * help()                                                                * 
 * Print help info for user                                              * 
 *                                                                       * 
 *************************************************************************/
void help ( void )
  {
  puts(
    "\n"
    PROGRAM" "VERSION", "AUTHOR" <"EMAIL">\n"
    "Purpose:\n"
    "  Converts a text/html email to a multipart/alternative (RFC 1341)\n"
    "  email that additionally has a text/plain version of the original\n"
    "  text/html part. PMMail will display the text/plain part of the\n"
    "  multipart/alternative email.\n"
    "Usage:\n"
    "  "PROGRAM_CALL" [/h] [File]\n"
    "Argument:\n"
    "  (none)       Display this info, then exit\n"
    "  /h (or -h)   Display this info, then exit\n"
    "  File         The email file to be converted\n"
    "Note:\n"
    "  The input file will be substituted by the converted file, but the\n"
    "  original email will not be altered and will become the last part\n"
    "  of the multipart/alternative email."
    );
  return;
  }


/************************************************************************* 
 *                                                                       * 
 * multipart()                                                           * 
 * Convert email to multipart/alternative                                * 
 *                                                                       * 
 *************************************************************************/
void multipart ( FILE* file, char* start, char* end )
  {

  char* p;

  header( file, start, end, BOUNDARY_STR );

  fputs( "\n--"BOUNDARY_STR"\n", file );

  plain( file, start, end );

  fputs( "\n--"BOUNDARY_STR"\n", file );

  html( file, start, end );

  fputs( "\n--"BOUNDARY_STR"--\n", file );

  return;

  }


/************************************************************************* 
 *                                                                       * 
 * header()                                                              * 
 * Write top level header of multipart/alternative email                 * 
 *                                                                       * 
 *************************************************************************/
void header ( FILE* file, char* start, char* end, char* boundarystr )
  {

  char* p;
  int strobe;

  strobe = ON;
  p = start;

  while( ('\n' != *p) || ('\n' != *(p+1)) ) 
    {

    /* Ignore subject line */
    if( ('s' == tolower(*p)) && (EQUAL == strnicmp(SUBJECT,p,strlen(SUBJECT))) )
      strobe = OFF;
    if( (OFF == strobe) && ('\n' == *p) )
      strobe = ON;

    /* Change content type */
    if( (ON == strobe) && ('c' == tolower(*p)) && (EQUAL == strnicmp(CONTENT_TYPE,p,strlen(CONTENT_TYPE))) )
      {
      fputs( CONTENT_TYPE" multipart/alternative; boundary=", file );
      fputs( boundarystr, file );
      while( (';' != *p) && ('\n' != *p) )
        p += 1;
      continue;
      }
    else
      fputc( *p, file );

    p += 1;

    }

  fputs( "\nX-Filtered: Converted from text/html to multipart/alternative by "PROGRAM" ("VERSION", "AUTHOR" <"EMAIL">)\n", file );

  fputc( '\n', file );

  return;

  }


/************************************************************************* 
 *                                                                       * 
 * html()                                                                * 
 * Write original email file content as text/html part of                * 
 * multipart/alternative email                                           * 
 *                                                                       * 
 *************************************************************************/
void html ( FILE* file, char* start, char* end )
  {

  char* p;

  p = start;
  while( p <= end ) 
    {
    fputc( *p, file );
    p += 1;
    }

  return;

  }


/************************************************************************* 
 *                                                                       * 
 * plain()                                                               * 
 * Write text/plain part of multipart/alternative email                  * 
 *                                                                       * 
 *************************************************************************/
void plain ( FILE* file, char* start, char* end )
  {

  char* p;               /* Memory pointer */
  int strobe = ON;    
  int i;                 /* All purpose index variable */
  char url[MAX_URL+1];   /* URL */

  p = start;

  /* Header */

  while( ('\n' != *p) || ('\n' != *(p+1)) ) 
    {

    /* Ignore subject line */
    if( ('s' == tolower(*p)) && (EQUAL == strnicmp(SUBJECT,p,strlen(SUBJECT))) )
      strobe = OFF;
    if( (OFF == strobe) && ('\n' == *p) )
      strobe = ON;

    /* Change content type */
    if( (ON == strobe) && ('c' == tolower(*p)) && (EQUAL == strnicmp(CONTENT_TYPE,p,strlen(CONTENT_TYPE))) )
      {
      fputs( CONTENT_TYPE" text/plain", file );
      while( (';' != *p) && ('\n' != *p) )
        p += 1;
      continue;
      }
    else
      fputc( *p, file );

    p += 1;

    }

  fputs( "\n\n", file );

  /* Go to the '\n' at the end of the empty line that marks the end of the header */
  p += 1;

  /* Body */

  while( ++p <= end )
    {

    /* Formatting commands */

    if( '&' == *p ) 
      {

      if( EQUAL == strnicmp(p+1,HTML_GT,strlen(HTML_GT)) )
        {
        fputc( '>', file );
        p += strlen(HTML_GT);
        if( ';' == *(p+1) )
          p += 1;
        continue;
        }

      else if( EQUAL == strnicmp(p+1,HTML_LT,strlen(HTML_LT)) )
        {
        fputc( '<', file );
        p += strlen(HTML_LT);
        if( ';' == *(p+1) )
          p += 1;
        continue;
        }

      else if( EQUAL == strnicmp(p+1,HTML_NBSP,strlen(HTML_NBSP)) )
        {
        fputc( ' ', file );
        p += strlen(HTML_NBSP);
        if( ';' == *(p+1) )
          p += 1;
        continue;
        }

      }   /* end if */

    else if( '<' == *p ) 
      {

      if( EQUAL == strnicmp(p+1,HTML_ANCHOR_START,strlen(HTML_ANCHOR_START)) )
        {

        while( '"' != *p )
          p += 1;

        p += 1;

        /* Read URL */
        for( i = 0; (i < MAX_URL-1) && ('"' != *p); i += 1, p += 1 )
          url[i] = *p;
        url[i+1] = '\n';

        while( '>' != *p )
          p += 1;

        continue;

        }

      else if( EQUAL == strnicmp(p+1,HTML_ANCHOR_END,strlen(HTML_ANCHOR_END)) )
        {

        fputs( " <", file );
        fputs( url, file );
        fputc( '>', file );

        p += 1 + strlen(HTML_ANCHOR_END);

        continue;

        }

      else
        {

        while( '>' != *p )
          p += 1;

        continue;

        }

      }   /* end if */

    fputc( *p, file );

    }   /* end while */

  return;

  }

