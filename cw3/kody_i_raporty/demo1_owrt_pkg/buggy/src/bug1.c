#include <stdio.h>
#include <stdlib.h>
int *table=NULL;
int main()
{
  int i;
  //"Zapomniana" inicjalizacja wskaźnika "table"
  //table=(int*) malloc(1000*sizeof(int));
  for(i=0;i<1000;i++) {
    table[i]=i;
  }
}
