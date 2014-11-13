#Makefile générique pour cross-compilation 680x0
#RP
#	Mon Jul  3 13:17:38 CEST 20001
#		ELF version
#	10/98 : version initiale
#
#exécutable à produire
OBJ=sort
#adresse d'implémentation
ORG=0x10000
#degré d'optimisation
OPT=2
#adresse du VBR de la carte
VBR=0
#si le jumper 7-8 sur J11 a été changé sur la carte
#VBR=0xFFE00000
#
CFLAGS=-DVBR=$(VBR) -m68040 -msoft-float -O$(OPT)
#
#ne pas changer ce qui suit

AS=m68k-ELF-linux-as
LD=m68k-ELF-linux-ld


all: crt0.o $(OBJ).o
	$(LD)	-Ttext=$(ORG)	\
		-oformat binary	\
		-Map map 	\
		crt0.o $(OBJ).o	\
		-o /tftpboot/vmeboot
#
