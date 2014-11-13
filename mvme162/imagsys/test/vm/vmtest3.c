/*
 * RP 04 / 1998
 *
 * PB : comment passer une adresse "virtuelle" au noyau
 *
 * ne marche pas en optimisation 0 mais ok en 2
 */
#define print(s) outstr(s,(char*)((int)s+strlen(s)))

void msg_S()
{
print("mode S OK");
}
//ce qui suit s'execute en mode S
void msg_U()
{
char *s="rien";

//on recupère sur USP l'adresse virtuelle (passée plus tot en mode U)
asm volatile("move.l %%usp,%%a0;move.l (%%a0),%0" : "=a" (s) : : "a0");
print(s);
}
void test_S()
{
int i;
for(i=0;i<5;i++) asm("trap #4");
}

//ici on passe par la pile USP l'adresse de la chaine à afficher
//le handler trap 4 récupère une adresse "virtuelle"
char *msg[] = {"HELLO", "folks", "ENFIN cette", "fois-la", "C'EST BIEN !"};
//ceci s'execute en mode U
void test_U()
{
int i;

for(i=0;i<5;i++) 
   asm volatile("move.l %0,-(%%sp);trap #5;addq.l #4,%%sp"
		: :
		"a" (msg[i]) : "memory" );
}

/*
 * version simpiste
 * facile car installe avant passage en vm
 * ni creation ni liberation des pages utilisées par les descripteurs
 */
void InstallDescr(int virtual_addr,/*alignée sur les blocs de 4k*/
		  int nbpages,
		  int physical_addr)
{
int *plw;
int ri, pi, pgi;
int n; //n°de page
char s[10];
#define TAILLE_PAGE 4096

n = virtual_addr / TAILLE_PAGE;
if (virtual_addr % TAILLE_PAGE) nbpages++;

//n m p
//p = (n°page % 8192) % 64 (si page de 4k)	0...63
//m = (n°page % 8192) / 64			0...127
//n = n°page / 8192				0...127
ri = n / 8192 ;			//n/4096
ntos(ri,s);
print(s);
pi = (n % 8192) / 64;		//(n%4096)/32
ntos(pi,s);
print(s);
pgi = (n % 8192) % 64;		//(n%4096)%32
ntos(pgi,s);
print(s);

//U W UDT
//UR G U1 U0 S CM M U W PDT
#define Resident		0x03
#define CachableCopyback	0x20
//ROOT TABLE descriptor (RI)	occupe 4*64  = 256o
plw = (int*)(0x200000+4*ri);
*plw =       0x205000 | Resident;
//POINTER TABLE descriptor (PI)	occupe 4*128 = 512o
plw = (int*)(0x205000+4*pi);
*plw =       0x209000 | Resident;
//PAGE descriptor (PGI)		occupe 4*128 = 512o
plw = (int*)(0x209000+4*pgi);
*plw =       physical_addr  | Resident | CachableCopyback;
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
    
    move.l #0x00000000,%d0
    movec.l %d0,%dtt0
    movec.l %d0,%itt0
    ");
    
    /*
     * construction de la table des descripteurs en 0x200000
     * la meme pour mode S et P
     * 		- pour une page de 4 k à partir de 0x10000 : code
     * 		(c'est la page n° 0,0,16)
     *		- pour la pile U : une page avant 0x300000
     *		(c'est la page n° 0,11,63)
     *		- pour la pile S : une page avant 0x400000
     *		(c'est la page 1023 n° 0,15,63)
     */
//InstallDescr(addr virtuelle,nb page 4k,addr physique);
     
//map text
InstallDescr(0x10000,1,0x10000);  
//map usp
InstallDescr(0x250000-4096,1,0x370000-4096);  
//map SSP
InstallDescr(0x400000-4096,1,0x400000-4096); 
//autre map ssp
InstallDescr(0x300000-4096,1,0x350000-4096); 


    asm("
    |instr. cache on, data cache on
    move.l #0x80008000,%a0
    movec.l %a0,%cacr
    move.l #0x200000,%a0
    movec.l %a0,%urp
    movec.l %a0,%srp   
    |vm Enable, page de 4k
    move.w #0x8000,%d0
    movec.l %d0,%tcr
    pflusha"
    );
//verifications...
//test_S();
//passer en mode U
asm("
   move.l #0x300000,%sp
   move.l #0x250000,%a0
   move.l %a0,%usp
   move.w #0,%sr
   ");
test_S();

test_U();
//il faudrait restorer le mode S pour terminer correctement
}

