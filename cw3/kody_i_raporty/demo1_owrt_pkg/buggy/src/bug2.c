#include <stdio.h>
int table[1000];
int main()
{
  int i;
  //Próba wyjścia poza obszar tablicy
  for(i=0;i<1000000;i++) {
    table[i]=i;
  }
}
