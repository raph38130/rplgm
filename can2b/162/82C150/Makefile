#Makefile g�n�rique pour cross-compilation 680x0
#RP
#10/98
#
#ex�cutable � produire
OBJ=can
#adresse d'impl�mentation
ORG=0x10000
#degr� d'optimisation
OPT=2
#adresse du VBR de la carte
VBR=0
#si le jumper 7-8sur J11 a �t� chang� sur la carte
#VBR=FFE00000
#
CFLAGS=-DVBR=$(VBR) -m68040 -msoft-float -O$(OPT)
#
#ne pas changer ce qui suit
EXEC=/disk/c68k/m68k-coff/bin
C68=gcc
AS68=as
LD68=ld
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68) 
CC=$(EXEC)/$(C68)

all: crt0.o can_init_it.o can_162_150_it.o trap_handler.o
	$(LD)	-Ttext=$(ORG)	\
		-oformat binary\
		-Map map 	\
		crt0.o /disk/c68k/lib/ensimag/mvme162lx.o \
		can_init_it.o can_162_150_it.o trap_handler.o \
		-l68k		\
		-L /disk/c68k/lib/ -lnewlibc -lm\
		-L /disk/c68k/lib/gcc-lib/m68k-coff/2.7.2.l.3/msoft-float -lgcc\
		-o /tftpboot/vmeboot

#			\

