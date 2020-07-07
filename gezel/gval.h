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
// $Id: gval.h 113 2009-06-19 16:11:11Z schaum $
//--------------------------------------------------------------

#ifndef GVAL_H
#define GVAL_H

#include <iostream>
#include <vector>

#include "gmp.h"

using namespace std;

// abstract base class for values
class gval {  

  mpz_t value;
  mpz_t mask;  
  mpz_t mask_1;

  unsigned sign;
  unsigned wordlength;
  unsigned curbase;

  void init_mask(unsigned w);
  void rescale();
  void scaninit(const char *bv);

 public:

  gval(unsigned _wordlength, unsigned _sign);
  gval(const char *);
  gval(unsigned _wordlength, unsigned _sign, const char *valuestr);
  
  gval & operator = (const gval &  v);
  void valuecopy    (const gval &  v); // like 'operator =' without rescale()
  
  unsigned long toulong() const;
    signed long toslong() const;
  void          assignulong (unsigned long);
  void          assignslong (signed long);
  
  unsigned getwordlength() const;
  void     setwordlength(unsigned _wordlength);
  unsigned getsign      () const;
  unsigned hamdist      (const gval &h1);
  unsigned popcount     ();
  unsigned weighted_hamdist      (const gval &h1, vector<unsigned> &wts);
  unsigned weighted_popcount     (vector<unsigned> &wts);
  void     setbase      (unsigned n);
  void     dbgprint     () const;
  bool     isz()  const;
  bool     isnz() const;
  
  bool operator == (const gval & z2);
  bool operator != (const gval & z2);
  bool operator <= (const gval & z2);
  bool operator >= (const gval & z2);
  bool operator <  (const gval & z2);
  bool operator >  (const gval & z2);
  
  void and_gval   (const gval &o1, const gval &o2);
  void ior_gval   (const gval &o1, const gval &o2);
  void xor_gval   (const gval &o1, const gval &o2);  
  void shr_gval   (const gval &o1, const gval &o2);
  void shr_ui_gval(const gval &o1, const unsigned int o2);
  void shl_gval   (const gval &o1, const gval &o2);
  void add_gval   (const gval &o1, const gval &o2);
  void concat_gval(const gval &o1, const gval &o2);
  void sub_gval   (const gval &o1, const gval &o2);
  void mod_gval   (const gval &o1, const gval &o2);
  void mul_gval   (const gval &o1, const gval &o2);
  void not_gval   (const gval &o1);
  void neg_gval   (const gval &o1);
  
  void readstream (istream &is);
  void writestream(ostream &os);
  void writebuf   (char *buf);

  string getbitstring();
};  

gval * make_gval(unsigned _wordlength, unsigned _sign);
gval * make_gval(const char *);
gval * make_gval(unsigned _wordlength, unsigned _sign, const char *valuestr);

istream & operator >> (istream &is, gval &v);
ostream & operator << (ostream &os, gval &v);
void gval_showstats   (ostream &os);

#endif

