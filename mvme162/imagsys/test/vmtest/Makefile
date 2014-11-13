# RP
# projet Systeme 1998
#
# étude de la mise en place de la VM
#
#
#   où placer le code du noyau
OBJ=/tftpboot/vmeboot
#
#   où trouver gcc-m68k et les utilitaires de cross-compilation
EXEC=/disk/C68K/gcc-2.7.2.l.3
#   son nom
C68=gcc-cross
AS68=as
LD68=ld
#
#   CPU card type
TARGET=TARGET/MVME162
#
#   processor
ARCH=ARCH/68040
#
#   IMPORTANT : interrupt vector base register (la valeur dépend du switch sur la carte MVME162)
#VBR=0xFFE00000 jumper J11 1-2
VBR=0
#
#niveau d'optimisation
OPT=2
#
#
CC=$(EXEC)/$(C68) -B$(EXEC)/ 
CFLAGS=-DVBR=$(VBR) -m68040 -O$(OPT)  -I../$(TARGET) -I../$(ARCH) 
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68)
#
#
all: vmeboot

e2s: Crt0.o conversion.o \
     kernel.o schedule.o \
     primitives.o \
     create.o resume.o \
     initprocess.o \
     context-switch.o trap_handler.o mem.o 
	$(LD) -T script       			\
             -Map map				\
             *.o  	          	  	\
     	     -L ../LINK -l68k                   \
     	     -o e2s


user1: userprocess1.o 
	$(LD) 	-T scriptuser       		\
		-Map mapuser1			\
		userprocess1.o	conversion.o	\
     	     	-L ../LINK -l68k                \
     	     	-o user1 ;	
				
user2: userprocess2.o 
	$(LD) 	-T scriptuser       		\
		-Map mapuser2			\
		userprocess2.o	conversion.o	\
     	     	-L ../LINK -l68k                \
     	     	-o user2 ;			


vmeboot: e2s user1 user2
	cat e2s user1 user2 > $(OBJ) ;		\
	ls -l $(OBJ)	
clean:
	rm *.o e2s user1 user2  map mapuser1 mapuser2 $(OBJ)
