#Makefile g�n�rique pour cross-compilation 680x0
#RP
#	Mon Jul  3 13:17:38 CEST 2000
#		ELF
#	10/98 : version initiale
#
#ex�cutable � produire
OBJ=bintodec
#adresse d'impl�mentation
ORG=0x10000
#
AS=m68k-ELF-linux-as
LD=m68k-ELF-linux-ld
#
all: crt0.o $(OBJ).o
	$(LD)	-Ttext=$(ORG)	\
		-oformat srec	\
		-Map map 	\
		crt0.o $(OBJ).o	\
		-o $(OBJ)
#
