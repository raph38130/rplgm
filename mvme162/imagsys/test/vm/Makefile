# RP

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

#
#   IMPORTANT : interrupt vector base register (la valeur dépend du switch sur la carte MVME162)
#VBR=0xFFE00000
VBR=0
#
#niveau d'optimisation
OPT=2
#
#
CC=$(EXEC)/$(C68) -B$(EXEC)/ 
CFLAGS=-DVBR=$(VBR) -m68040 -O$(OPT) 
AS=$(EXEC)/$(AS68) -m68040
LD=$(EXEC)/$(LD68)
#
#edition de lien séparée pour le noyau "e2S" et les applications "user"
all: crt0.o vmtest1.o net.o
	$(LD) -T script      	 			\
             -Map map				  	\
             crt0.o vmtest1.o net.o     	\
     	     -L ../../LINK -l68k                          	\
     	     -o $(OBJ)
clean:
	rm *.o

