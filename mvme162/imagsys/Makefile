# RP
# Projet Systeme 1998
# E2S pour mvme162
# $Id: Makefile,v 1.1 1998/04/08 22:13:32 raph Exp $
# 12 / 97
# 01 / 98 amélioration de la portabilité
# 05 / 98 mmu
#
# organisation de l'arborescence :
#          	kernel		l'essentiel du noyau
#          	mm		memory managment (buddy)
#		mmu		la mémoire virtuelle
#		drivers		I.O.
#
#		TARGET		caractéristiques de MVME162
#		ARCH		ctxt switch, manipulation des registres
#
#		samples		exemples de procs utilisateur pour tests
#		lib		wrapper des fonctions systèmes (lib68k.a)
#	   	utilty		diverses fonctions bien utiles (lib68k.a)
#
#		LINK		crt0.s, scripts de ld		
#		BOOT		code 680x0
#
#		test		pour mettre au point e2s
#		test/UNIX	version virtuelle du noyau (setjmp/longjmp)
#
#
#   où placer le code du noyau
OBJ=/tftpboot
#
#   où trouver gcc-m68k et les utilitaires de cross-compilation
#EXEC=/disk/C68K/gcc-2.7.2.l.3
EXEC=/disk/c68k/m68k-coff/bin
#   son nom
#C68=gcc-cross
C68=gcc
AS68=as
LD68=ld $(LDFLAGS)
#
#   CPU card type
TARGET=TARGET/MVME162
#
#   processor type
ARCH=ARCH/68040
#
#   IMPORTANT : interrupt vector base register
#(la valeur dépend du switch sur la carte MVME162)
#remis à 0
#VBR=0xFFE00000
VBR=0
#
#niveau d'optimisation
OPT=2
#
#
#CC=$(EXEC)/$(C68) -B$(EXEC)/
CC=$(EXEC)/$(C68)
CFLAGS=-DVBR=$(VBR) -m68040 -msoft-float -O$(OPT) -Ikernel -Imm -Idrivers -I$(TARGET) -I$(ARCH) -Iinclude
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68)
#edition de lien séparée pour le noyau "e2S" et les applications "user"
#
#nom du processus à compiler
#USER=process2

all: vmeboot
	
e2s: LINK/crt0.o \
     kernel/kernel.o kernel/schedule.o \
     kernel/primitives.o \
     kernel/kill.o kernel/create.o kernel/suspend.o kernel/resume.o \
     kernel/initprocess.o kernel/status.o \
     kernel/semaphore.o kernel/sleep.o \
     lib/func.o \
     mm/mem.o mm/buddy.o mm/load.o \
     mmu/mmu.o \
     drivers/drivers.o drivers/serial.o drivers/fs.o \
     $(ARCH)/context-switch.o $(ARCH)/trap_handler.o $(ARCH)/sysfunc.o
	$(LD)  -T LINK/script       			\
             -Map map				  	\
             LINK/crt0.o                            	\
             kernel/*.o  	          	  	\
             $(ARCH)/*.o            			\
             mm/*.o                 	  		\
	     mmu/*.o					\
             drivers/*.o				\
     	     -L LINK -l68k                         	\
     	     -o $(OBJ)/vmeboot

init: LINK/crt0user.o \
        samples/Menu.o 
	$(LD) 	-T LINK/scriptuserload       		\
		-Map mapinit				\
		LINK/crt0user.o 			\
	     	samples/Menu.o            	  	\
     	     	-L LINK -l68k                          	\
     	     	-o $(OBJ)/init
		
user: LINK/crt0user.o \
        samples/$(USER).o 
	$(LD) 	-T LINK/scriptuserload       		\
		-Map mapuser				\
		LINK/crt0user.o 			\
	     	samples/$(USER).o          	  	\
     	     	-L LINK -l68k                          	\
     	     	-o $(OBJ)/$(USER) ;			\
	ls -l $(OBJ)/$(USER)	


vmeboot: e2s init
	ls -l $(OBJ)
	
tgz :
	tar cvf e2s.tar * 	;\
	gzip e2s.tar		;\
	cp e2s.tar.gz ~/BACKUP	;\
	ls -l e2s.tar.gz
listing:
	cat kernel/*c mm/*c mmu/*c lib/*c drivers/*c utility/*c samples/*c ARCH/68040/*c \
	include/*h kernel/*h mm/*h drivers/*h samples/*h ARCH/68040/*h TARGET/MVME162/*h \
	LINK/*s ARCH/68040/*s \
	| a2ps --header="" -o e2s.ps
	
clean:
	rm LINK/*.o 		;\
	rm kernel/*.o 		;\
	rm mm/*.o mmu/*.o	;\
        rm lib/*.o		;\
        rm drivers/*.o		;\
        rm utility/*.o		;\
        rm samples/*.o		;\
        rm $(ARCH)/*.o		;\
        rm map mapinit mapuser	;\
        rm BOOT/*		;\
	rm e2s.tar.gz		;\
        rm $(OBJ)/*
