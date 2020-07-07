#ifndef _ARM_EXT_H
#define _ARM_EXT_H

/* You should not modify this file. */

/* Macros for ARM instruction extensions.

   The condition suffix "nv" means "never". Instructions with
   nv suffixes are reserved by the ARM ISA for extensions (see
   ARM Architecture Reference Manual page A3-5).
   Therefore, we legally use suitable opcodes with "nv" suffixes
   to represent our new "surrogate" instructions.
   The good news is that since these opcodes and "nv" are
   all recogonized by gcc and gas (the GNU assembler), we donot
   have to modify gcc and gas to accomodate the surrogates.


   NOTE: THE ARGUMENT FOR THE MACROS MUST BE VARIABLE NAMES. YOU
   **SHOULD NOT** USE EXPRESSIONS AS ARGUMENT.

*/

/* Below we have two macros for instructions with 2 inputs and 2 outputs.
   For both macros, argument S1, S2 are input variables, and
   D1 and D2 are output variables.

   Macro OP2x2_1 is mapped to function unit sfu2x2_1 in sfu.hpp.
   value of S1 is sent to port in1, S2 sent to port in2,
   result D1 will be read back from out1, and D2 from out2.

   Macro OP2x2_2 is mapped to function unit sfu2x2_2 in sfu.hpp.
   value of S1 is sent to port in1, S2 sent to port in2,
   result D1 will be read back from out1, and D2 from out2.

   Below shows an example of using the macros. The example
   assumes that sfu2x2_1 implements "swap" function in update(), i.e.
   out1 = in2, out2 = in1;
   
#include <stdio.h>
#include "arm_ext.h"

int main()
{
   int x = 10, y=20;

   OP2x2_1(x,y,x,y);

   fprintf(stderr, "%d %d\n", x, y);

   return 0;
}

   The above code should print "20 10" to the terminal.

*/

#define OP2x2_1(D1,D2,S1,S2) \
	asm volatile ("smullnv %0, %1, %2, %3": \
		"=&r"(D1),"=&r"(D2): \
		"r"(S1),"r"(S2));

#define OP2x2_2(D1,D2,S1,S2) \
	asm volatile ("smullnvs %0, %1, %2, %3": \
		"=&r"(D1),"=&r"(D2): \
		"r"(S1),"r"(S2));

/* Below we have two macros for instructions with 3 inputs and 1 outputs. 

   Macro OP3x1_1 is mapped to function unit sfu3x1_1 in sfu.hpp.
   value of S1 is sent to port in1, S2 sent to port in2, S3 to port in3,
   result D1 will be read back from out1.

   Macro OP3x1_2 is mapped to function unit sfu3x1_2 in sfu.hpp.
   value of S1 is sent to port in1, S2 sent to port in2, S3 to port in3,
   result D1 will be read back from out1.
 */
#define OP3x1_1(D1, S1, S2, S3) \
	asm volatile ("mlanv %0, %1, %2, %3": \
		"=&r"(D1): "r"(S1), "r"(S2), "r"(S3)); \

#define OP3x1_2(D1, S1, S2, S3) \
	asm volatile ("mlanvs %0, %1, %2, %3": \
		"=&r"(D1): "r"(S1), "r"(S2), "r"(S3)); \

#endif
