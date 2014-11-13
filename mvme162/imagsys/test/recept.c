#include <stdio.h>
#include <fcntl.h>

int main()
{
FILE* f;
char buff[250],c;

f=fopen("/dev/cua2","r");

while(1) {
	c=fgetc(f);
	//strcat(p,buff);
	printf("%c\n",c);
	}
return 0;
}
