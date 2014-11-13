/* test1.c
 *	each device descriptor is read (once at time) then written

 * Fri Feb  8 10:39:58 CET 2008 RP
 *	- cvs demo
 *
 * Tue Jan 22 12:27:58 CET 200 RP
 *	- initial revision
 *
 * compile command :
gcc -I /lib/modules/2.4.12/build/include/ test1.c -o test1
*/
#include <stdio.h>

#include <fcntl.h>

#define LEN 10

int main()
{
char buffer[LEN]={~1,~2,~4,~8,~16,~32,~64,~128};
int 	fd,i;

/* /dev/pcitor */
fd=open("/dev/pcitor",O_RDONLY);
if (fd<0) {
	printf("erreur ouverture en lecture\n");
	return 1;
	}
i=read(fd,buffer,LEN);
if(i<0) {
	printf("erreur read\n");
	return 1;
	}
printf("lu sur pcitor %d byte : %x\n",i,*buffer & 0xFF);
close(fd);
fd=open("/dev/pcitor",O_WRONLY);
if (fd<0) {
	printf("erreur ouverture en écriture\n");
	return 1;
	}
for(i=0; i<5;i++) {
	write(fd,buffer + (i % 8),1);
	sleep(1);
	}
close(fd);
/*-------------------------------------------------------*/

/*/dev/pcitor1*/
fd=open("/dev/pcitor1",O_RDONLY);
if (fd<0) {
	printf("erreur ouverture en lecture\n");
	return 1;
	}
i=read(fd,buffer,LEN);
if(i<0) {
	printf("erreur read\n");
	return 1;
	}
printf("lu sur pcitor1 %d byte : %x\n",i,*buffer & 0xFF);
close(fd);
fd=open("/dev/pcitor1",O_WRONLY);
if (fd<0) {
	printf("erreur ouverture en écriture\n");
	return 1;
	}
for(i=0; i<5;i++) {
	write(fd,buffer + (i % 8),1);
	sleep(1);
	}
close(fd);
/*-------------------------------------------------------*/

/*/dev/pcitor2*/
fd=open("/dev/pcitor2",O_RDONLY);
if (fd<0) {
	printf("erreur ouverture en lecture\n");
	return 1;
	}
i=read(fd,buffer,LEN);
if(i<0) {
	printf("erreur read\n");
	return 1;
	}
printf("lu sur pcitor2 %d byte : %x\n",i,*buffer & 0xFF);
fd=open("/dev/pcitor2",O_WRONLY);
if (fd<0) {
	printf("erreur ouverture en écriture\n");
	return 1;
	}
for(i=0; i<5;i++) {
	write(fd,buffer + (i % 8),1);
	sleep(1);
	}
close(fd);
/*-------------------------------------------------------*/
return 0;
}
