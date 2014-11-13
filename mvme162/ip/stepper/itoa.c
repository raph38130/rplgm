/* itoa.c
 * Wed Nov 27 09:23:42 CET 2002 RP
 *	- initial revision
 *
 */

itoa(int N, char *buffer)
{
int 	i,
	r,
	q;

     if (N<10) i=1;
else if (N<100) i=2;
else if (N<1000) i=3;
else if (N<10000) i=4;
else if (N<100000) i=5;
else if (N<1000000) i=6;


buffer[i--]=0;
while(N>0) {
	r=N % 10;
	q=N / 10;
	buffer[i--]=r +'0';
	N=q;
	}	
}

/*
main()
{
int n1=0x3F54;
char buf[10];

itoa(n1,buf);
printf("n1=0x%x n1=%s\n",n1, buf);
}
*/
