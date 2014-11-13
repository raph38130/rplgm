#include <stdio.h>

main()
{
long int i,j=1000000;

while(1) {
   for (i=0;i<10000000;i++);
   printf("                                %d\n",j++);
   }
}
