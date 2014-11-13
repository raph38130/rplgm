/* i2ctest.c
 * .
 *
 * Wed Feb  4 13:09:42 CET 2004 RP
 *	- initial revision
 *	- avec tini_i2c library
 *	- ready for netboot
 *
rm *asm *ihx *hex *rel *lst *rst *lnk *sym *map *mem *bin
../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit i2ctest.c -c
../tools/sdcc/bin/sdcc -mds400 --model-flat24 --stack-10bit \
-L./tini_i2c_lib -li2clib \
-Wl-r --xram-loc 0x140000 --code-loc 0x490000 i2ctest.rel 
../tools/sdcc/bin/packihx i2ctest.ihx > i2ctest.hex
../../j2sdk_nb/j2sdk1.4.2/bin/java -cp ../tini1.12/bin/tini.jar:../tools/hex2bin Hex2tbin2 i2ctest.hex i2ctest.bin
cp i2ctest.bin /tftpboot/TINI400

 *
 */
#include <stdio.h> 
#include <tinibios.h>
#include "tini_i2c_lib/tini_i2c.h"

//external I2C (saa1064) pins are P1.1 P1.0
//libi2c is for internal i2c (ds1672) P31 P32

void main()
{ 
int i=0;
char s=0,buffer[25];

printf("i2ctest LGM V0.1.1 !!!\r\n"); 
romInit('n',SPEED_2X);
printf("after init_rom!!!\r\n");
getchar();
printf("read i2c bus!!!\r\n");
buffer[0]=0x00;
buffer[1]=0x06;
buffer[2]=0x0F;
buffer[3]=0xF0;
buffer[4]=0xA0;
buffer[5]=0x0E;
while(1) {
	//buffer[0]=0x00;
	buffer[0]=0xaa;
	buffer[1]=0xaa;
	//s=i2c_writeblock(0x7E,buffer,1);
	i2c_select(0x72);
	i2c_stop();
	s=i2c_readbyte(1);
	printf("read %x\n\r",s & 0xFF);
	//getchar();
	//buffer[0]=0xc0;
	//s=i2c_writeblock(0x7E,buffer,1);
	//getchar();
	}
}

 
