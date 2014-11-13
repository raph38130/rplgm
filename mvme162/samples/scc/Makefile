OBJ=/tftpboot/vmeboot
EXEC=/disk/c68k/m68k-coff/bin
C68=gcc
AS68=as
LD68=ld
OPT=2
VBR=0
CC=$(EXEC)/$(C68)
CFLAGS=-DVBR=$(VBR) -m68040 -msoft-float -O$(OPT) 
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68) 

all: crt0.o essai1.o
	$(LD)	-oformat=srec -Ttext=0x10000\
		-Map map \
		crt0.o essai1.o \
		-L /disk/c68k/lib/ -l68k \
		-L /disk/c68k/lib/gcc-lib/m68k-coff/2.7.2.l.3/msoft-float -lgcc\
		-o $(OBJ)

#
