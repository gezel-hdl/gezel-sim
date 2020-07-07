/* --------------------------------------------------------------
 Copyright (C) 2003 Patrick Schaumont (schaum@ee.ucla.edu)                   
 University of California at Los Angeles
                                                                             
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License
 as published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.
                                                                             
 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
                                                                             
 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 USA
 
  $Id: fdl.ll 104 2009-05-19 14:35:17Z schaum $
  -------------------------------------------------------------- */

/* 
  scanner for vc2005 & gnuwin32 flex

   "/c/Program Files/GnuWin32/bin/flex.exe" -Pfdl -ofdl.cc fdl.ll

  and change 
    class istream;
  into
    #include <iostream>
    using namespace std;
  before compiling fdl.cc
*/

%option c++
%option debug
%option noyywrap

/* lex spec for fsmd parser */
%{
  using namespace std;

  #include "fdlyacc.h" /* token codes */
  #include <fstream>
  #include "fdlparse.h"

  FlexLexer *fdlLexer = 0;
  istream   *fdlinstream = 0;

  int cpp_fdl_openstream(char *f) {
    if (strcmp(f, "stdin") == 0)
      fdlinstream = & std::cin;
    else
      fdlinstream = (istream *) new std::ifstream(f);
    fdlLexer    = new fdlFlexLexer(fdlinstream, &cerr);
    return fdlinstream->fail();
  }

  int fdl_openstream(char *f) {
    return cpp_fdl_openstream(f);
  }

  int fdl_instream_valid() {
    return (fdlinstream != 0);
  }

  int fdllex() {
    if (fdlLexer)
      return fdlLexer->yylex();
    return 0;
  }
  
  void destroy_fdl_lexer() {
    if (fdlLexer)
      delete fdlLexer;
  }
 
  string current_program_line;

  void append_to_line(const char *f) {
    current_program_line += string(f);
  }

  extern void record_line(long, string);
  extern void issue_pending_warning();

  void complete_line() {
    record_line(fdl_lineno, current_program_line);
//    lex_fdl_warning();
    issue_pending_warning();
    current_program_line = "";
  }


//#ifndef YY_SKIP_YYWRAP
//#ifdef __cplusplus
//extern "C" int yywrap( void );
//#else
//extern int yywrap( void );
//#endif
//#endif

%}

ws          [ \t]+
D           [0-9]
B           [0-1]
H           [0-9a-fA-F]
id          [a-zA-Z_][a-zA-Z_0-9]*
nl          [\r][\n]|[\n\r]
F           [0-9a-fA-F\._]+

%x SYNTAXERROR BLOCKCOMMENT

%%
{ws}           { append_to_line((char *) YYText()); continue; }
\#\!.*$        { append_to_line((char *) YYText()); continue; }
\/\/.*$        { append_to_line((char *) YYText()); continue; } 
\/\*	       { append_to_line((char *) YYText());
                 BEGIN(BLOCKCOMMENT);
                 continue;
               }
<BLOCKCOMMENT>{nl} { append_to_line((char *) YYText()); continue; }
<BLOCKCOMMENT>. { append_to_line((char *) YYText()); continue; }
<BLOCKCOMMENT>\*\/ { append_to_line((char *) YYText());
                 BEGIN(0);
                 continue;
               }
"{"            { append_to_line((char *) YYText()); return (fdllval.cmd = BRACE_OPEN); } 
"}"            { append_to_line((char *) YYText()); return (fdllval.cmd = BRACE_CLOSE);   }
"("            { append_to_line((char *) YYText()); return (fdllval.cmd = RBRACKET_OPEN); }
")"            { append_to_line((char *) YYText()); return (fdllval.cmd = RBRACKET_CLOSE);}
"["            { append_to_line((char *) YYText()); return (fdllval.cmd = BRACKET_OPEN); }
"]"            { append_to_line((char *) YYText()); return (fdllval.cmd = BRACKET_CLOSE);}
";"            { append_to_line((char *) YYText()); return (fdllval.cmd = SEMICOLON);     }
":"            { append_to_line((char *) YYText()); return (fdllval.cmd = COLON);         }
"\."           { append_to_line((char *) YYText()); return (fdllval.cmd = DOT);           } 
","            { append_to_line((char *) YYText()); return (fdllval.cmd = COMMA);         }
"->"           { append_to_line((char *) YYText()); return (fdllval.cmd = TARGET);        }
"ipblock"      { append_to_line((char *) YYText()); return (fdllval.cmd = IPBLOCK);       } 
"iptype"       { append_to_line((char *) YYText()); return (fdllval.cmd = IPTYPE);        }
"ipparm"       { append_to_line((char *) YYText()); return (fdllval.cmd = IPPARM);        }
"fsm"          { append_to_line((char *) YYText()); return (fdllval.cmd = FSM);           }
"hardwired"    { append_to_line((char *) YYText()); return (fdllval.cmd = HARDWIRED);     } 
"sequencer"    { append_to_line((char *) YYText()); return (fdllval.cmd = SEQUENCER);     }
"system"       { append_to_line((char *) YYText()); return (fdllval.cmd = SYSTEM);        } 
"stimulus"     { append_to_line((char *) YYText()); return (fdllval.cmd = STIMULUS);      }
"state"        { append_to_line((char *) YYText()); return (fdllval.cmd = STATE);         }
"initial"      { append_to_line((char *) YYText()); return (fdllval.cmd = INITIALSTATE);  }
"lookup"       { append_to_line((char *) YYText()); return (fdllval.cmd = LOOKUP);        }
"use"          { append_to_line((char *) YYText()); return (fdllval.cmd = USE);           }
"if"           { append_to_line((char *) YYText()); return (fdllval.cmd = IF);            }
"then"         { append_to_line((char *) YYText()); return (fdllval.cmd = THEN);          }
"else"         { append_to_line((char *) YYText()); return (fdllval.cmd = ELSE);          }
"dp"           { append_to_line((char *) YYText()); return (fdllval.cmd = DP);            }
"tc"           { append_to_line((char *) YYText()); return (fdllval.cmd = TC);            }
"ns"           { append_to_line((char *) YYText()); return (fdllval.cmd = NS);            } 
"in"           { append_to_line((char *) YYText()); return (fdllval.cmd = IN);            }
"out"          { append_to_line((char *) YYText()); return (fdllval.cmd = OUT);           }
"sig"          { append_to_line((char *) YYText()); return (fdllval.cmd = SIG);           }
"reg"          { append_to_line((char *) YYText()); return (fdllval.cmd = REG);           }
"sfg"          { append_to_line((char *) YYText()); return (fdllval.cmd = SFG);           }
"always"       { append_to_line((char *) YYText()); return (fdllval.cmd = ALWAYS);        }
"="            { append_to_line((char *) YYText()); return (fdllval.cmd = ASSIGN);        }
"\?"           { append_to_line((char *) YYText()); return (fdllval.cmd = QMARK);         }
"@"            { append_to_line((char *) YYText()); return (fdllval.cmd = AT);            }
"\|"           { append_to_line((char *) YYText()); return (fdllval.cmd = IOR);           }
"\^"           { append_to_line((char *) YYText()); return (fdllval.cmd = XOR);           }
"\&"           { append_to_line((char *) YYText()); return (fdllval.cmd = AND);           }
">>"           { append_to_line((char *) YYText()); return (fdllval.cmd = SHR);           }
"<<"           { append_to_line((char *) YYText()); return (fdllval.cmd = SHL);           }
"\+"           { append_to_line((char *) YYText()); return (fdllval.cmd = ADD);           }
"\-"           { append_to_line((char *) YYText()); return (fdllval.cmd = SUB);           }
"\#"           { append_to_line((char *) YYText()); return (fdllval.cmd = HASH);          }
"\*"           { append_to_line((char *) YYText()); return (fdllval.cmd = MUL);           }
"\%"           { append_to_line((char *) YYText()); return (fdllval.cmd = MOD);           }
"\~"           { append_to_line((char *) YYText()); return (fdllval.cmd = NOT);           }
"=="           { append_to_line((char *) YYText()); return (fdllval.cmd = EQ);            }
">"            { append_to_line((char *) YYText()); return (fdllval.cmd = GRT);           }
">="           { append_to_line((char *) YYText()); return (fdllval.cmd = GRTEQ);         }
"<"            { append_to_line((char *) YYText()); return (fdllval.cmd = SMT);           }
"<="           { append_to_line((char *) YYText()); return (fdllval.cmd = SMTEQ);         }
"!="           { append_to_line((char *) YYText()); return (fdllval.cmd = NE);            }
"\$display"    { append_to_line((char *) YYText()); return (fdllval.cmd = DISPLAY);       }
"\$finish"     { append_to_line((char *) YYText()); return (fdllval.cmd = FINISH);        }
"\$hex"        { append_to_line((char *) YYText()); return (fdllval.cmd = HEXDISPLAY);    }
"\$dec"        { append_to_line((char *) YYText()); return (fdllval.cmd = DECDISPLAY);    }
"\$bin"        { append_to_line((char *) YYText()); return (fdllval.cmd = BINDISPLAY);    }
"\$dp"         { append_to_line((char *) YYText()); return (fdllval.cmd = DPNAME);        }
"\$sfg"        { append_to_line((char *) YYText()); return (fdllval.cmd = SFGNAME);       }
"\$cycle"      { append_to_line((char *) YYText()); return (fdllval.cmd = CYCLENUM);      }
"\$toggle"     { append_to_line((char *) YYText()); return (fdllval.cmd = TOGGLENUM);     }
"\$ones"       { append_to_line((char *) YYText()); return (fdllval.cmd = ONESNUM);     }
"\$zeroes"     { append_to_line((char *) YYText()); return (fdllval.cmd = ZEROESNUM);     }
"\$trace"      { append_to_line((char *) YYText()); return (fdllval.cmd = TRACE);         }
"\$option"     { append_to_line((char *) YYText()); return (fdllval.cmd = OPTION);        }
{D}+           { append_to_line((char *) YYText()); 
                 fdllval.posint = (char *) YYText();
                 return POSINT; }
0[xX]{H}+      { append_to_line((char *) YYText()); 
                 fdllval.posint = (char *) YYText();
                 return POSINT; }
0[bB]{B}+      { append_to_line((char *) YYText()); 
                 fdllval.posint = (char *) YYText();
                 return POSINT; }
\"[^\"\n]*\"   { append_to_line((char *) YYText()); 
                 fdllval.str = (char *) YYText();
                 return QSTRING;
               }
{id}           { append_to_line((char *) YYText()); 
                 fdllval.str = (char *) YYText();
//	         if (strlen((char *) YYText()) > MAXSYMLEN) {
//                    std::string k;
//                    k = "Warning: Long Identifier ";
//                    k = k +  YYText() + " will be truncated";
//                    strcpy(fdl_error_message,k.c_str());
//	            lex_fdl_warning();
//	       }
                 return ID; }
{nl}           { complete_line();
                 fdl_lineno++; }
.              { 
 	         cerr << "Unmatched: [" << ((char *) YYText()) << "]\n";
                 append_to_line("-->");
		 append_to_line((char *) YYText());
                 append_to_line("<--");
	         // cerr << "Found syntax error on line " << fdl_lineno << " \n";
                 BEGIN(SYNTAXERROR);
                 continue;
               }
<SYNTAXERROR>. { append_to_line((char *) YYText());
                 // cerr << "Continue syntax error\n";
                 continue;
               }
<SYNTAXERROR>{nl} { cerr << "*** Error line " << fdl_lineno << " : Lexical Error\n";
                    complete_line();
		    exit(0);
                  }
%%

int fdlwrap() {

  if (fdlinstream) {
    // fdlinstream->close();
    if (fdlinstream != & std::cin) {
      delete fdlinstream;
      fdlinstream = 0;
   }
  }

  return 1;
}



