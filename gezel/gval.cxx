//--------------------------------------------------------------
// Copyright (C) 2003-2010 P. Schaumont 
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
//--------------------------------------------------------------


#include "gval.h"
#include "rtopt.h"
#include <map>
#include <cstdlib>
#include <cstring>
#include "gmp.h"

map<int, int> wlenstats;

gval * make_gval(unsigned _wordlength, unsigned _sign) {
  wlenstats[_wordlength]++;
  return new gval(_wordlength, _sign);
}

gval * make_gval(const char *v) {
  gval *r = new gval(v);
  wlenstats[r->getwordlength()]++;
  return r;
}

gval * make_gval(unsigned _wordlength, unsigned _sign, const char *valuestr) {
  wlenstats[_wordlength]++;
  return new gval(_wordlength, _sign, valuestr);
}

void gval_showstats(ostream &os) {
  map<int, int>::iterator k;
  os.width(20); os << "Wordlength";
  os.width(20); os << "Number of Signals\n";
  for (k = wlenstats.begin(); k != wlenstats.end(); k++) {
    os.width(20); os << k->first;
    os.width(20); os << k->second;
    os << "\n";
  }
}

istream & operator >> (istream &is, gval &v) {
  v.readstream(is);
  return is;
}

ostream & operator << (ostream &os, gval &v) {
  v.writestream(os);
  return os;
}

//---------------------------------------------------------

void gval::init_mask(unsigned w) {
  mpz_set_ui(mask,   0L);
  mpz_set_ui(mask_1, 0L);
  mpz_setbit(mask,   w);
  mpz_sub_ui(mask,   mask, 1L);
  mpz_setbit(mask_1, w);
  mpz_neg   (mask_1, mask_1);
  // next statements force allocation of sufficient space for
  // value at construction time. Otherwise, mpz_scan1 function
  // fails
  mpz_setbit(value,w);
  mpz_clrbit(value,w);
}

void gval::rescale() {
  mpz_and(value, value, mask);
  if (sign &&
      (mpz_scan1(value, wordlength-1) == (wordlength-1)) &&
      (mpz_cmp_ui(value, 0L) > 0)) {
    mpz_add(value, value, mask_1);
  }
}

gval::gval(unsigned _wordlength, unsigned _sign) :
  sign(_sign) ,
  wordlength(_wordlength),
  curbase(16) {
  mpz_init(value);
  mpz_init(mask);
  mpz_init(mask_1);
  init_mask(wordlength);
}

void gval::scaninit(const char *bv) {
  // create unsigned or signed mpzcl with wordlength as require to represen bv
  // which is in one of the following formats:
  // - decimal:     [0-9]+
  // - hexadecimal: 0[xX][0-9a-fA-F]+
  // - binary:      0[bB][01]+
  sign  = 0;

  // empty string
  if (bv[0] == 0) {
    wordlength = 1;
    mpz_init(value);
    mpz_init(mask);
    mpz_init(mask_1);
    init_mask(wordlength);
  }

  if (bv[0] == '-') {
    //  cerr << "NEGATIVE " << bv << "\n";
    sign = 1;
    bv++;
  }
  
  // check base
  const char *buf;
  if (bv[0] == '0') {
    if ((bv[1] == 'x') || (bv[1] == 'X')) {
      curbase = 16;
      buf = &(bv[2]);
      mpz_init_set_str(value, buf, curbase); 
    } else if ((bv[1] == 'b') || (bv[1] == 'B')) {
      curbase = 2;
      buf = &(bv[2]);
      mpz_init_set_str(value, buf, curbase);
    } else {
      // is decimal string
      curbase = 10;
      mpz_init_set_str(value, bv, curbase);
    }
  } else {
    // is decimal string
    curbase = 10;
    mpz_init_set_str(value, bv, curbase);
  }

  // if this was a negative number, allow 1 extra bit for sign
  if (sign)
    wordlength = (unsigned int) mpz_sizeinbase(value, 2) + 1;
  else
    wordlength = (unsigned int) mpz_sizeinbase(value, 2) + 1;

  // cout << "inp " << bv << " wl " << wordlength << "\n";
  mpz_init(mask);
  mpz_init(mask_1);
  init_mask(wordlength);

  //  cerr << "--------- before\n";
  //  dbgprint();

  if (sign) {
    // a '-' was found, so this must be a negative number
    mpz_neg(value, value);
  }

  //  cerr << "--------- after\n";
  //  dbgprint();
 
}

string gval::getbitstring() {
  char buf[wordlength+1];
  mpz_get_str(buf, 2, value);
  return string(buf);
}

gval::gval(const char *bv) {
  scaninit(bv);
}

gval::gval(unsigned _wordlength, unsigned _sign, const char *valuestr) {
  scaninit(valuestr);
  sign       = _sign;
  wordlength = _wordlength;
  init_mask(wordlength);
  rescale();
}
  
gval & gval::operator = (const gval &  v) {
  mpz_set(value, v.value);
  if ((wordlength != v.wordlength) || (sign != v.sign)) {
    rescale();
  }
  return *this;
}

void gval::valuecopy(const gval &  v) { // like 'operator =' without rescale()
  mpz_set(value, v.value);
}
  
unsigned long gval::toulong() const {
  return mpz_get_ui(value);
}

void gval::assignuarray(unsigned *v, unsigned s) {
  // COPY in LITTLE ENDIAN format
  mpz_import(value, s, -1, sizeof(v[0]), 0, 0, v);
  rescale();
}

void gval::touarray(unsigned *v, unsigned *s) {
  // COPY IN LITTLE ENDIAN format
  mpz_export(v, (size_t *) s, -1, sizeof(v[0]), 0, 0, value);
  rescale();
}

signed long gval::toslong() const {
  if (mpz_fits_slong_p(value) == 0)
    cerr << "*** WARNING: Conversion to toslong() loses precision\n";
  return mpz_get_si(value);
}

void gval::assignulong(unsigned long v) {
  mpz_set_ui(value, v);
  rescale();
}

void gval::assignslong(signed long v) {
  mpz_set_si(value, v);
  rescale();
}

bool gval::isz()  const {
  return (mpz_cmp_ui(value, 0L) == 0);
}

bool gval::isnz() const {
  return (mpz_cmp_ui(value, 0L) != 0);
}
  
unsigned gval::getwordlength() const {
  return wordlength;
}

void gval::setwordlength(unsigned _wordlength) {
  wordlength = _wordlength;
  init_mask(wordlength);
  rescale();
}

unsigned gval::hamdist(const gval &h1) {
  return mpz_hamdist(value, h1.value);
}

unsigned gval::popcount() {
  return mpz_popcount(value);
}

unsigned gval::weighted_hamdist(const gval &h1, vector<unsigned> &wts) {
  unsigned acc = 0;
  unsigned i;
  for (i=0; i<wts.size(); i++) {
    if (mpz_tstbit(value,i) != mpz_tstbit(h1.value,i))
      acc += wts[i];
  }
  return acc;
}

unsigned gval::weighted_popcount(vector<unsigned> &wts) {
  unsigned acc = 0;
  unsigned i;
  for (i=0; i<wts.size(); i++) {
    if (mpz_tstbit(value,i) == 1)
      acc += wts[i];
  }
  return acc;
}

unsigned gval::getsign() const {
  return sign;
}

void gval::setbase(unsigned n) {
  curbase = n;
}
  
bool gval::operator == (const gval & z2) {
  return (mpz_cmp(value, z2.value) == 0);
}

bool gval::operator != (const gval & z2) {
  return (mpz_cmp(value, z2.value) != 0);
}

bool gval::operator <= (const gval & z2) {
  return (mpz_cmp(value, z2.value) <= 0);
}

bool gval::operator >= (const gval & z2) {
  return (mpz_cmp(value, z2.value) >= 0);
}

bool gval::operator <  (const gval & z2) {
  return (mpz_cmp(value, z2.value) < 0);
}

bool gval::operator >  (const gval & z2) {
  return (mpz_cmp(value, z2.value) > 0);
}
  
void gval::and_gval   (const gval &o1, const gval &o2) {
  mpz_and(value, o1.value, o2.value);
}

void gval::ior_gval   (const gval &o1, const gval &o2) {
  mpz_ior(value, o1.value, o2.value);
}

void gval::xor_gval   (const gval &o1, const gval &o2) {
  mpz_xor(value, o1.value, o2.value);
}
  
void gval::shr_gval   (const gval &o1, const gval &o2) {
  mpz_fdiv_q_2exp(value, o1.value, mpz_get_ui(o2.value));
  rescale();
}

void gval::shr_ui_gval   (const gval &o1, const unsigned int o2) {
  mpz_fdiv_q_2exp(value, o1.value, o2);
  rescale();
}

void gval::shl_gval   (const gval &o1, const gval &o2) {
  mpz_mul_2exp(value, o1.value, mpz_get_ui(o2.value));
  rescale();
}

void gval::add_gval   (const gval &o1, const gval &o2) {
  mpz_add(value, o1.value, o2.value);
  rescale();
}

void gval::concat_gval(const gval &o1, const gval &o2) {
  mpz_mul_2exp(value, o1.value, o2.wordlength);
  mpz_ior(value, value, o2.value);
}

void gval::sub_gval   (const gval &o1, const gval &o2) {
  mpz_sub(value, o1.value, o2.value);
  rescale();
}

void gval::mod_gval   (const gval &o1, const gval &o2) {
  mpz_mod(value, o1.value, o2.value);
  rescale();
}

void gval::mul_gval   (const gval &o1, const gval &o2) {
  mpz_mul(value, o1.value, o2.value);
  rescale();
}

void gval::not_gval   (const gval &o1) {
  mpz_com(value, o1.value);
  rescale();
}

void gval::neg_gval   (const gval &o1) {
  mpz_neg(value, o1.value);
}

void gval::readstream (istream &is) {
  mpz_t m;
  mpz_init(m);

  char buf[4096] = "";
  is >> buf; // ugly .. can cause buf to overflow
  if (mpz_init_set_str(m, buf, curbase)) {
    if (buf[0] != 0)
      cerr << "*** Mismatched format: number [" << buf << "] in base " << curbase << "\n";
  }

  mpz_set(value,m);
  rescale();  // REMOVE TO ALLEVIATE PRECISION RULE (left-operand)
  mpz_clear(m);
}

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

void gval::writebuf(char *buf) {
  mpz_t m;

  mpz_init(m);
  mpz_set(m, value);
  if (((curbase == 2) || (curbase == 16)) && (mpz_cmp_ui(value, 0L) < 0)) {
    mpz_abs(m,m);
    mpz_add(m, m, mask_1);
    mpz_neg(m, m);
  }
  mpz_get_str(buf,curbase, m);
  mpz_clear(m); 
}

void gval::writestream(ostream &os) {
  char buf[4096];

  writebuf(buf);
  //  unsigned n = (unsigned int) mpz_sizeinbase(m, curbase);
  unsigned n = strlen(buf);
  if (curbase == 2) {
    if (wordlength < n) {
      cerr << "assertion error in gval::writestream. wl ";
      cerr << wordlength << " n " << n << "\n";
      exit(0);
    }
    //       add leading zeroes - for now, only for binary
    for (unsigned i=0; i < wordlength -n; i++)
      os << '0';
  }

  os << buf;
}

void gval::dbgprint() const {
  char buf[4096];
  cerr << "gval obj @ " << this << "\n";
  cerr << "     wl: " << wordlength;
  cerr << " sgn: " << sign;
  cerr << " curbase: " << curbase << "\n";
  mpz_get_str(buf,16, value);
  cerr << "     value: " << buf;
  mpz_get_str(buf,16, mask);
  cerr << " mask: " << buf;
  mpz_get_str(buf,16, mask_1);
  cerr << " mask_1: " << buf << "\n";;
}




