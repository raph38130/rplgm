# RP
# module IP Actis OUT16
#
# Tue Jul 11 21:05:38 CEST 2000
#	version initiale

CC=m68k-linux-gcc
AS=m68k-ELF-linux-as
LD=m68k-ELF-linux-ld
#
all: out16.s vmeboot

out16.s: out16.c
	$(CC) -S out16.c -o out16.s

out16.o: out16.s
	$(AS) out16.s -o out16.o
	
vmeboot: crt0.o out16.o 
	$(LD) -oformat srec -Ttext=0x10000 \
	crt0.o out16.o \
	-L/opt/COURS/MVME162/samples/lib -l162 \
	-L/opt/c68k/GLIBC/usr/lib/ -lc\
	-o vmeboot ;\
	cp vmeboot /tftpboot
#


