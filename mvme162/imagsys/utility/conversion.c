/*
 * $Id$
 */
#define N	20




void ntos(int M, char *s)
{
int r,q,i=N-1;
char b[N];
unsigned int n;

b[i--]=0;
n = (unsigned int ) M;
do {
   q = n / 10;
   r = n % 10;
   n = q;
   b[i--] = r + '0';
   }
while(q!=0);
strcpy(s,b+i+1);
}
void n_htos(int M, char *s)
{
int r,q,i=N-1;
char b[N];
unsigned int n;

b[i--]=0;
n = (unsigned int ) M;
do {
   q = n / 16;
   r = n % 16;
   n = q;
   b[i--] = (r<10) ? (r + '0') : (r + 'A' - 10);
   }
while(q!=0);
strcpy(s,b+i+1);
}



int ston(char *s)
{
int i=0,n=0;

while(s[i]) n = 10*n + s[i++] - '0';

return n;
}
