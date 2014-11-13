#Makefile générique pour cross-compilation 680x0
#RP
#10/98
#
#exécutable à produire
OBJ=essai2
#adresse d'implémentation
ORG=0x10000
#degré d'optimisation
OPT=0
#adresse du VBR de la carte
VBR=0
#si le jumper 7-8sur J11 a été changé sur la carte
VBR=0xFFE00000
#
CFLAGS=-DVBR=$(VBR) -m68040 -msoft-float -O$(OPT)
#
#ne pas changer ce qui suit
EXEC=/home/raph/crosstool/m68k-unknown-linux-gnu/gcc-3.4.3-glibc-2.3.2/bin/
C68=m68k-unknown-linux-gnu-gcc
AS68=m68k-unknown-linux-gnu-as
LD68=m68k-unknown-linux-gnu-ld
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68) 
CC=$(EXEC)/$(C68)

all: crt0.o $(OBJ).o ls7166.o trap_handler.o mvme162.o
	$(LD)	-Ttext=$(ORG)	\
		--oformat binary\
		-Map map 	\
		crt0.o $(OBJ).o ls7166.o trap_handler.o mvme162.o\
		-L /home/raph/crosstool/m68k-unknown-linux-gnu/gcc-3.4.3-glibc-2.3.2/m68k-unknown-linux-gnu/lib/ \
		-lc -o /tftpboot/vmeboot

#		-l68k	/disk/c68k/lib/ensimag/mvme162lx.o 	\
#		-L /disk/c68k/lib/ -lnewlibc -lm\
#		-L /disk/c68k/lib/gcc-lib/m68k-coff/2.7.2.l.3/msoft-float -lgcc\
#
