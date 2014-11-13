/*
m68k-linux-gcc print.c -o print.s -S -B /usr/lib/gcc-lib/ -I /usr/include/
m68k-linux-as print.s -o print.o
*
*
*/

#include <math.h>

#define print(s) outstr(s,s+strlen(s))
#define N 30
 
main()
{
char s[30]="2.71828";
int i;
float x=2.71828;
double y=M_PI;

y=sin(y);
for(i=0;i<N;i++) {
	sprintf(s,"hello %d %f\n\r",i,sin((double)2*M_PI*i/N));
	print(s);
	}
sscanf(s,"%f",&x);
sprintf(s,"x lu = %f",x);
	print(s);
}
