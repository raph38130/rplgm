# Generic makefile for m68k-ELF
# RP
# Mon Jul  3 13:17:38 CEST 2000
#
# TODO
#		librairie avec appels 162bug utiles
#		ajouter libC
#		m68k-linux-gcc invoque as86
#
ORG=0x10000
VBR=0
#
CC_FLAGS= -D VBR=$(VBR)
#
AS=m68k-ELF-linux-as 
CC=m68k-linux-gcc -S $(CC_FLAGS)
LD=m68k-ELF-linux-ld
#
essai.s: essai.c
	$(CC) essai.c -o essai.s
essai.o: essai.s
	$(AS) essai.s -o essai.o
	
all: essai.o crt0.o
	$(LD) -Ttext=$(ORG) crt0.o essai.o	\
	-Map map        			\
	-oformat srec				\
	-o bootfile
