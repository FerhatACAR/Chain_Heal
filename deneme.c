#include <stdio.h>
#include <stdlib.h>
#include "fields.h"
#include <math.h>

int main(int argc, char **argv) 
{
  IS is;
  int i, n;
  char *lines[10];
  
  printf("\n\n\n%f\n\n\n",rint(0.87));

  /* Read the lines of standard input, and only keep the last ten. */

  // is = new_inputstruct(NULL);
  // n = 0;
  // while (get_line(is) >= 0) {
  //   //printf(is->text1);
  //   printf("%s %s %s\n",is->fields[0],is->fields[1], is->fields[2]);        /* This is the bad line -- it doesn't copy the string. */
  //   i = atoi(is->fields[0]);
  //   printf("%d",i);
  //   n++;
  // }

  /* Print the last 10 lines, or fewer if there are fewer lines. 
     Remember that is->text1 has a newline at the end. */

  //i = (n >= 10) ? (n-10) : 0;
  //for ( ; i < n; i++) printf("%s", lines[i%10]);

  exit(0);
}