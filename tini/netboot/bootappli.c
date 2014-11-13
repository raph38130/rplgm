/* bootappli.c
 * .
 * Wed Mar 31 22:01:47 CEST 2004 RP
 *	- new release
 * Wed Feb  4 13:09:42 CET 2004 RP
 *	- initial revision
 *	- ready for netboot
 *
JDK_HOME=/home/raph/j2sdk_nb/j2sdk1.4.2
rm *asm *ihx *hex *rel *lst *rst *lnk *sym *map *mem *bin
../tools/sdcc/bin/sdcc -V -mds400 --model-flat24 --stack-10bit bootappli.c -c

../tools/sdcc/bin/sdcc -V -mds400 --model-flat24 --stack-10bit \
-Wl-r --xram-loc 0x140000 --code-loc 0x490000 bootappli.rel 

../tools/sdcc/bin/packihx bootappli.ihx > bootappli.hex
${JDK_HOME}/bin/java -cp ../tini1.13/bin/tini.jar:../tools/hex2bin Hex2tbin2 bootappli.hex bootappli.bin
cp bootappli.bin /tftpboot/TINI400

/etc/dhcp3/dhcpd.conf
-=-=-=-=-=-=-=-=-=-=
option domain-name "lgm.fr";
option domain-name-servers neptune.lgm.fr;


host tini {
  hardware ethernet 00:60:35:00:de:9b;
  fixed-address 172.16.0.3;
}

 *
 */
#include <stdio.h> 
#include <tinibios.h>
#include <ds400rom.h>


void main()
{ 
char s=0;
int i=0;
float x=0;


printf("80C400 netboot test LGM V1.1 !!!\n"); 
romInit('n',SPEED_2X);
printf("after init_rom !!!\n      press return\n");
getchar();

printf("%ld millisecondes depuis netboot\n",task_gettimemillis_long());
getchar();
while(1) {
	x += 1.0 / (((float) i)*((float) i));
	printf("%d %f\012",i++,x);
	}
}

 
