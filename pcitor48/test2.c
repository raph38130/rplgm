/* test2.c
 *	leds are switched ON sequentialy
 *	coud be run simultaneously  ./test2 /dev/pcitor1 100000 &  ./test2 /dev/pcitor2 10000
 *
 * Tue Jan 22 12:27:58 CET 200 RP
 *	- initial revision
 *
 * compile command :
gcc -I /lib/modules/2.4.12/build/include/ test2.c -o test2
*/
#include <stdio.h>
#include <unistd.h>

//#include <asm/io.h>
#include <fcntl.h>

#define LEN 10

int main(int argc, char **argv)
{
char buffer[LEN]={~1,~2,~4,~8,~16,~32,~64,~128};
int 	fd,i,delai;
 
if (argc!=3) {
	printf("usage: test2 /dev/pcitor? delay\n");
	return 0;
	}
fd=open(argv[1],O_WRONLY);
sscanf(argv[2],"%d",&delai);
if (fd<0) {
	printf("erreur ouverture en écriture\n");
	return 1;
	}
for(i=0; i<200;i++) {
	write(fd,buffer + (i % 8),1);
	usleep(delai);
	}
close(fd);

return 0;
}
