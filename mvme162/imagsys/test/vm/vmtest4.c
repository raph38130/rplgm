/*
 * on veut mapper 4 Mo ce qui fait 1024 pages (de 4k)
 * il faut donc 1024 / 64 = 16 pointers tables
 *
 * il nous faut donc :
 *	1 	ROOT TABLE (niv 1)	1  entrée  utilisée  dans cette table
 *	1	POINTER TABLE (niv2)	16 entrées utilisées dans cette table
 *	16*64	PAGE DESCRIPTOR (niv3)	64 entrées utilisées dans chaque table
 *
 * occupant (1 + 16 + 16*64)*4 = 4164 octets
 *
 *
 *                
 */
 
/*
 * 0000 0000	4000 0000 	map en direct pour le mode S
 *				virt_adr = phys_adr
 *
 * 8000 0000	F000 0000	espace d'un process en mode U
 *				deux process différents occupent le meme espace
 *				vituel car :
 *					virt_adr != adr_phy
 *					si virt_adrP1 == virt_adrP2
 *					alors adr_phyP2 != adr_phyP2
 *
 * FF00 0000	FFFF FFFF 	transparent mode S et U
 *				pas de VM sur cet espace
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
void InstallDescr(int virtual_addr,	//alignée sur les blocs de 4k
		  int physical_addr,
		  int nbpages,
		  int base)		//où mettre cette table
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
pi = (n % 8192) / 64;		//(n%4096)/32
pgi = (n % 8192) % 64;		//(n%4096)%32

//U W UDT
//UR G U1 U0 S CM M U W PDT
#define Resident		0x03
#define CachableCopyback	0x20
ntos(ri,s);
print(s);
ntos(pi,s);
print(s);
ntos(pgi,s);
print(s);


#define BASE 0x200000
//ROOT TABLE descriptor (RI)	occupe 4*64  = 256o
ri=0;
plw = base;
*plw =       (base+4) | Resident;
//POINTER TABLE descriptor (PI)	occupe 4*128 = 512o
plw = base+4*pi;
*plw =       base+...+pi*512 | Resident;

//PAGE descriptor (PGI)		occupe 4*128 = 512o
for(i=0;i<;i++) {
   plw = (int*)();
   *plw =       physical_addr  | Resident | CachableCopyback;
   }
}



main()
{
char *pb;

SetHandler(VBR+36*4,msg_S);//trap 4
SetHandler(VBR+37*4,msg_U);//trap 5

asm ("
    /* FFxxxxxx transparent
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
    
//map text
InstallDescr(0x10000,1,0x10000);  
//map usp
InstallDescr(0x300000-4096,1,0x380000-4096);  
//map SSP
InstallDescr(0x400000-4096,1,0x400000-4096); 


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
    pflusha
    |
    | maintenant on est en VM
    |
    move.l #0x250000,%a0
    move.l %a0,%usp
    move.w #0,%sr
    ");
test_U();
//a-t-on accès
//pb = (char*)0x150000;
//*pb = 7;
test_S();
//il faudrait restorer le mode S pour terminer correctement
}

