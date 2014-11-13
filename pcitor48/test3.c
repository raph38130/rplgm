/* test3.c
 *	
 * Tue Feb  5 10:31:15 CET 2002 RP
 *	- initial revision
 *
 * compile command :
gcc -I /lib/modules/2.4.12/build/include/ test3.c -o test3
*/
#include <stdio.h>
#include <fcntl.h>

#include <signal.h>
#include "pcitor48.h"

#define LEN 10
char buffer[LEN];

int main(int argc, char **argv)
{
int 	fd,
	s;
 
fd=open("/dev/pcitor2",O_RDONLY);
if (fd<0) {
	printf("erreur ouverture en écriture\n");
	return 1;
	}
s=ioctl(fd,PCITOR_ITPC30ON);
if (s<0) {
	printf("erreur ioctl\n");
	return 1;
	}
printf("switch PC3\n");
s=read(fd,buffer,LEN);
if (s<0) {
	printf("erreur read\n");
	return 1;
	}
s=ioctl(fd,PCITOR_ITPC30OFF);
if (s<0) {
	printf("erreur ioctl\n");
	return 1;
	}
printf("switch PC3 again\n");
sleep(5);
s=close(fd);
if (s<0) {
	printf("erreur close\n");
	return 1;
	}
return 0;
}
