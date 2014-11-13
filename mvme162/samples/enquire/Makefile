OBJ=/tftpboot/vmeboot
EXEC=/disk/c68k/m68k-coff/bin
C68=gcc
AS68=as
LD68=ld
OPT=0
VBR=0
CC=$(EXEC)/$(C68)
CFLAGS=-DVBR=$(VBR) -m68040 -msoft-float -O$(OPT)  -DNO_MEM -DNO_SIG
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68) 

all: crt0.o enquire3.o
	$(LD)	-T script\
		-Map map \
		crt0.o enquire3.o /disk/c68k/lib/ensimag/mvme162lx.o\
		-L /disk/c68k/lib/ -lnewlibc -lm\
		-L /disk/c68k/lib/gcc-lib/m68k-coff/2.7.2.l.3/msoft-float -lgcc\
		-o $(OBJ)

#
