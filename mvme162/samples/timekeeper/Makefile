#Makefile g�n�rique pour cross-compilation 680x0
#RP
#
# Mon Jul  3 13:01:16 CEST 2000
#
# version initiale : 10/98
#
#ex�cutable � produire
OBJ=./bootfile
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
#gcc
AS=m68k-ELF-linux-as
LD=m68k-ELF-linux-ld
source=time

all: crt0.o $(source).o
	$(LD)	-Ttext=$(ORG)	\
		-oformat srec	\
		-Map map 	\
		crt0.o 		\
		$(source).o 	\
		-o $(OBJ)
#
#
