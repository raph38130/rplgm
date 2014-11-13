/*
 * RP 04/1998
 * PB : comment passer une adresse "virtuelle" au noyau
 */
#define print(s) outstr(s,s+strlen(s))

void msg_S()
{
print("mode S OK");
}
void msg_U()
{
char *s;

asm volatile("move.l (%%a6),%%a0;move.l -4(%%a0),%0" : "=a" (s) : : "a0" );

print(s);
}
void test_S()
{
int i;
for(i=0;i<5;i++) asm("trap #4");
}

//ici on passe par la pile l'adresse de la chaine à afficher
//le handler trap 4 récupère une adresse "virtuelle"
char *msg[] = {"hello", "folks", "comment", "ca", "va ?"};
void test_U()
{
int i;

for(i=0;i<5;i++)
asm volatile("move.l %0,-(%%sp);trap #5;addq.l #4,%%sp"
		: :
		"a" (msg[i]) : "memory");
}

void InstallDescr(int ri,int pi,int pgi)
{
int *plw;

//U W UDT
//UR G U1 U0 S CM M U W PDT
#define Resident		0x03
#define CachableCopyback	0x20
//ROOT TABLE descriptor (RI)
plw = (int*)(0x200000+4*ri);
*plw =       0x205000 | Resident;
//POINTER TABLE descriptor (PI)
plw = (int*)(0x205000+4*pi);
*plw =       0x209000 | Resident;
//PAGE descriptor (PGI)
plw = (int*)(0x209000+4*pgi);
*plw =       0x10000  | Resident | CachableCopyback;
}



main()
{
SetHandler(VBR+36*4,msg_S);//trap 4
SetHandler(VBR+37*4,msg_U);//trap 5

asm ("
    /* Transparent Translation Register
     * doc Motorola 3.1.3
     *
     * FFxxxxxx transparent
     * Enable
     * FC2 ignored (ok pour accès S ou U)
     * CM non cachable, serialized
     * R/W
     */
    move.l #0xff00c040,%d0
    movec.l %d0,%itt1
    movec.l %d0,%dtt1
    
    /* 
     * bloc 00xxxxxxxx transparent pour le mode S uniquement
     */
    move.l #0x0000a040,%d0
    move.l #0x00000000,%d0
    movec.l %d0,%dtt0
    movec.l %d0,%itt0
    ");
    
    /*
     * table des descripteurs en 0x200000
     * 		- pour une seule page de 4 k à partir de 0x10000 : code + pile
     * 		(c'est la page n° 0,0,16)
     *		- pour la pile U : une page avant 0x300000
     *		(c'est la page n° 0,11,63)
     *		- pour la pile S : une page avant 0x400000
     *		(c'est la page 1023 n° 0,15,63)
     */
InstallDescr(0,0,16);  
//InstallDescr(0,0,17);  
InstallDescr(0,11,63);  
InstallDescr(0,15,63);  

//n m p
//p = (n°page % 8192) % 64 (si page de 4k)	0...63
//m = (n°page % 8192) / 64			0...127
//n = n°page / 8192				0...127
    asm("
    |instr. cache on, data cache on
    move.l #0x80008000,%a0
    movec.l %a0,%cacr
    move.l #0x200000,%a0
    movec.l %a0,%urp
    movec.l %a0,%srp   
    |vm Enable, page de 4k
    move.w #0x0000,%d0
    move.w #0x8000,%d0
    movec.l %d0,%tcr
    pflusha"
    );

test_S();
//passer en mode U
asm("
   move.l #0x300000,%a0
   |exemple avec une seule page
   |move.l #0x10000+4096,%a0
   move.l %a0,%usp
   move.w #0,%sr
   ");
   
test_U();
//il faudrait restorer le mode S pour terminer correctement
}

