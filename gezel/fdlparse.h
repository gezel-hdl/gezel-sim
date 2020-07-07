//--------------------------------------------------------------
// Copyright (C) 2003-2005 P. Schaumont, D. Ching 
//                                                                             
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//                                                                             
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                             
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// $Id: fdlparse.h 35 2006-01-17 13:01:34Z schaum $
//--------------------------------------------------------------

#ifndef FDLPARSE_H
#define FDLPARSE_H

// #define MAXSYMLEN 128

#include <string>

int  fdl_openstream(char *f);
int  fdl_instream_valid();
int  fdllex();
void fdlerror(char *error);
void destroy_fdl_lexer();

extern int fdldebug;              // set by user, read by parser
int call_parser(char *filename); // called by user


extern long fdl_warningcount;
extern long fdl_lineno;
extern char fdl_error_message[255];
void   lex_fdl_warning();

extern enum fdl_error {
  UNKNOWN,
  BAD_FILE_NAME,
  BAD_IO_TYPE,
  BAD_SIGN_TYPE,
  BAD_STORAGE_TYPE,
  BAD_INDEX_EXPR,
  BAD_DP_NAME,
  DP_MULTIPLE_USE,
  UNKNOWN_IDENTIFIER,
  BAD_LEFT_HAND_SIDE,
  STRING_TO_LONG,
  MULTIPLE_SYSTEMS
} fdl_error_id;

#endif
