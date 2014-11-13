#include <stdio.h>

main()
{
FILE *f,*f2;
int n,N;
unsigned char*p;
f=fopen("/tftpboot/data","r");
f2=fopen("data.csv","w");
if (f==NULL) {
        printf("erreur ouverture\n");
   exit(0);
   }
N=0;
while(!feof(f)) {
        p=(unsigned char*)&n;
   
   //motorola
   p=p+3;
   *p--=(unsigned char)fgetc(f);
   *p--=(unsigned char)fgetc(f);
   *p--=(unsigned char)fgetc(f);
   *p--=(unsigned char)fgetc(f);
   /* 
   //intel
   p=p;
   *p++=(unsigned char)fgetc(f);
   *p++=(unsigned char)fgetc(f);
   *p++=(unsigned char)fgetc(f);
   *p++=(unsigned char)fgetc(f);
   */
        printf("%d ",n);
        fprintf(f2,"%d\n",n);

   N++;
   //if(!(N % 10)) fprintf(f2,"\n");
   }
   printf("%d data\n",N);
}
