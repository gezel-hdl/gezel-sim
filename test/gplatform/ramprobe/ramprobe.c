#include <stdio.h>

void probe(char *cmd) {
  char cmdbuffer[255];
  volatile unsigned int  *probep;
  probep = (volatile unsigned int  *) 0x80000000;

  strcpy(cmdbuffer, cmd);
  *probep = (unsigned) cmdbuffer;
}
  
int main() {

  int i;

  probe("start=3");
  probe("stop=5");
  probe("cmd=print");

  for (i=0; i< 40000; i++) ;

  probe("start=0");
  probe("stop=10");
  probe("cmd=print");

  return 0;
}
