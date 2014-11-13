/*
 * register.h
 * $Id$
 *
 */



#define SetUserMode() 	asm volatile("move.w #0x0000,%sr")
//masquer les it
#define cli() 		asm volatile("or.w   #0x0700,%sr")
//d�masquer les it
#define sti() 		asm volatile("and.w  #0xF8FF,%sr")
//pour lib�rer le bus 
#define stop()   	asm volatile("stop #0x2000")
 
//diverses macro pour recuperer les parametres plac�s sur la pile avant un appel systeme

//pour recup�rer n (num�ro de fonction) et le premier argument
#define fetch_n_arg1(n,var1) \
asm volatile("move.l (%%a6),%%a0;move.l -4(%%a0),%0;move.l 8(%%a0),%1" : : "g" (n) , "g" (var1) : "%a0" )

#define store_arg1(var1) \
asm volatile("move.l (%%a6),%%a0;move.l %0,8(%%a0)" \
	      : : \
	      "g" (var1) \
	      : "%a0" )

//pour recup�rer le second argument
#define fetch_arg2(var2) \
asm volatile("move.l (%%a6),%%a0;move.l 12(%%a0),%0" : : "g" (var2) : "%a0" )

//pour recup�rer le second et le troisi�me argument
#define fetch_arg2_3(var2,var3) \
asm volatile("move.l (%%a6),%%a0;move.l 12(%%a0),%0;move.l 16(%%a0),%1" \
	      : : \
	      "g" (var2) , "g" (var3) \
	      : "%a0" )

//pour recup�rer le second, le troisi�me et le quatri�me argument
#define fetch_arg2_3_4(var2,var3,var4) \
asm volatile("move.l (%%a6),%%a0;move.l 12(%%a0),%0;move.l 16(%%a0),%1;move.l 20(%%a0),%2" \
	      : : \
	      "g" (var2) , "g" (var3) , "g" (var4) \
	      : "%a0" )


//pour recup�rer les arguments de sys_create
#define fetch_arg_create(var2,var3,var4,var5,var6) \
asm volatile("move.l (%%a6),%%a0;move.l 12(%%a0),%0;move.l 16(%%a0),%1;move.l 20(%%a0),%2;move.l 24(%%a0),%3;move.l 28(%%a0),%4" \
	      : : \
	      "g" (var2) , "g" (var3) , "g" (var4) , "g" (var5) , "g" (var6) \
	      : "%a0" )

//g�nere un appel syst�me n� n par trap 4
#define SYSCALL(n) \
        asm volatile("move.l %0,-(%/sp);trap #4;addq.l #4,%/sp;move.l 8(%%a6),%%d0" \
		      : : \
		      "" (n) \
		      : "memory" )

//macro pour l'acc�s m�moire (inutiles)
#define StoreByte(adresse, n)		\
	asm volatile("move.b %0,(%1)" : : "g" (n) , "a" (adresse) : "memory" )
#define StoreWord(adresse, n)		\
	asm volatile("move.w %0,(%1)" : : "g" (n) , "a" (adresse) : "memory" )
#define StoreLong(adresse, n)		\
	asm volatile("move.l %0,(%1)" : : "g" (n) , "a" (adresse) : "memory" )

#define ReadByte(adresse, n)		\
	asm volatile("move.b (%1),%0" : "=g" (n) : "a" (adresse) : "memory" )
	
