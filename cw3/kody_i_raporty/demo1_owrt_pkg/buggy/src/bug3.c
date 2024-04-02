#include <stdio.h>
#include <stdlib.h>
char s1[10]="012345678\x00";
char s2[10]="abcdfefgh\x00";
/*
Ten program pokazuje, że błędy dostępu do pamięci nie zawsze są wykrywane przez
mechanizmy ochrony i mogą doprowadzić do błędnego działania aplikacji. 
*/
int main()
{
  int i;
  for(i=0;i<24;i++) {
    s1[i]=i+64;
  }
  printf("s1=%s\ns2=%s\n",s1,s2);
}
