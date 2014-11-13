/*
 *
 * utilisation des procédure de B CASSAGNE
 * 
 * dans cet exemple :
 *
 * FF00 0000	FFFF FFFF	transparant mode S et U
 *
 * pile S en 40 0000		mapping virtuel transparant
 * pile U en 30 0000		mappé en virtuel 20 0000
 *
 * code en 10 000		mappé en virtuel transparant pour S et U	
 */
 
#include "mmu.h"

#define print(s) outstr(s,(char*)((int)s+strlen(s)))
//U W UDT
//UR G U1 U0 S CM M U W PDT
#define Resident		0x03
#define CachableCopyback	0x20


DESCR *ArbreS, *ArbreU;
struct desc_ptp tdesc_ptp[NB_DESC_PTP];



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


int AllocPhysique()
{
char s[30];

static int base=0x200000;
base += 4096;
ntos(base,s);
print(s);
return base;
}

/******************************************************************************/
/*                                                                            */
/*                                AllocTablePages                             */
/*                                                                            */
/*   But:                                                                     */
/*      Alloue une table de pages de niveau 1, 2 ou 3 et remplit la table     */
/*      de descripteurs invalides (mis a 0)                                   */
/*                                                                            */
/******************************************************************************/
DESCR * AllocTablePages(int niv)
{
int i, j, trouve, nbtables, tailletable, nbdesc;
DESCR * atable, *p;

switch (niv)
   {
   case NIV1_2 : nbtables = 8; tailletable = 512; nbdesc = 128; break;
   case NIV3 :  nbtables = 16; tailletable = 256; nbdesc = 64; break;
   default : return((DESCR *)-1);
   }

/*   on recherche une table libre dans un descripteur valide du bon type  */
/*   ------------------------------------------------------------------   */
i = 0; trouve = 0;
while ( !(trouve) && i < NB_DESC_PTP)
   {
   if (tdesc_ptp[i].valid && (tdesc_ptp[i].type == niv))
      for (j = 0; j < nbtables; j++)
	 if ((tdesc_ptp[i].alloue & (1 << j)) == 0)
	    {
	    tdesc_ptp[i].alloue = tdesc_ptp[i].alloue | (1 << j);
	    trouve = 1;
	    break;
	    }
   if (trouve) break;
   i++;
   }

if (trouve)
   atable = (DESCR *)(tdesc_ptp[i].adr + j * tailletable);
else  /*   on recherche un descripteur non valide   */
   {  /*   --------------------------------------   */
   for (i = 0; i < NB_DESC_PTP; i++)
      if ( ! tdesc_ptp[i].valid )  { trouve = 1; break;}

   if (! trouve) return ((DESCR *)-1);   /*   tableau tdesc_ptp sature   */
   tdesc_ptp[i].valid = 1;
   print("desc");
   tdesc_ptp[i].adr = (unsigned int) AllocPhysique();
   tdesc_ptp[i].type = niv;
   tdesc_ptp[i].alloue = 1;   /*  premiere table allouee, les autres libres   */
   atable = (DESCR *)(tdesc_ptp[i].adr);
   }

/*   on fait une RAZ de la table   */
/*   ---------------------------   */
p = atable;
for (i = 1; i <= nbdesc; i++)
   *p++ = 0;

/*   DEBUG   */
//kprintf("AllocTablePages : atable = %x\n",atable);
return(atable);
}



/******************************************************************************/
/*                                                                            */
/*                              MapEspaceVirtuel                              */
/*                                                                            */
/*   But:                                                                     */
/*      Alloue sur un arbre de l'espace virtuel allant de adr_deb a adr_fin   */
/*      en transparent c'est a dire virtuel <--> physique si trans == 1       */
/*      et en allouant ds pages physiques si trans == 0                       */
/*      Cet algorithme rajoute de l'espace virtuel a un arbre existant        */
/*      qui doit etre un arbre valide avec eventuellement des descripteurs    */
/*      invalides (il peut donc se reduire a une seule table de niveau 1)     */
/*                                                                            */
/******************************************************************************/
MapEspaceVirtuel(DESCR *arbre,char *adr_deb,char *adr_fin,int trans)
{
DESCR *atab_niv1, *atab_niv2, *atab_niv3;
DESCR *p, *q;
int iniv1, iniv1_deb, iniv1_fin;
int iniv2, iniv2_deb, iniv2_fin;
int iniv3, iniv3_deb, iniv3_fin;
char *adr_cour;

adr_cour = adr_deb;
iniv1_deb = (unsigned int) adr_deb >> 25;
iniv1_fin = (unsigned int) adr_fin >> 25;
atab_niv1 = arbre;
/*   boucle sur les descripteurs de la table de niveau 1   */
/*   ---------------------------------------------------   */
for (iniv1 = iniv1_deb ; iniv1 <= iniv1_fin; iniv1++)
   {
   p = atab_niv1 + iniv1;
   if ((*p & 0x3) == 0)   /*   les descripteur est invalide   */
      {
      atab_niv2 = (DESCR *) AllocTablePages(NIV1_2);
      *p = (int) atab_niv2 | 0x03;   /*   03 = UDT resident   */
      }
   else atab_niv2 = (DESCR *) (*p & 0xffffff00);
   iniv2_deb = ((unsigned int) adr_cour >> 18) & 0x7f;
   if (adr_fin >= (char *) ((iniv1 + 1) * 0x02000000))
      iniv2_fin = 127;
   else iniv2_fin = ((unsigned int) adr_fin >> 18) & 0x7f;

   /*   boucle sur les descripteurs de la table de niveau 2   */
   /*   ---------------------------------------------------   */
   for (iniv2 = iniv2_deb ; iniv2 <= iniv2_fin ; iniv2++)
      {
      p = atab_niv2 + iniv2;
      if ((*p & 0x03) == 0) /*   le descripteur est invalide   */
	 {
	 atab_niv3 = (DESCR *) AllocTablePages(NIV3);
	 *p = (int) atab_niv3 | 0x03;   /*   03 = UDT resident   */
	 }
      else atab_niv3 = (DESCR *) (*p & 0xffffff00);
      iniv3_deb = ((unsigned int) adr_cour >> 12) & 0x3f;
      if (adr_fin >= (char *) (iniv1 * 0x02000000 + (iniv2 + 1) * 0x40000))
	 iniv3_fin = 63;
      else iniv3_fin = ((unsigned int) adr_fin >> 12) & 0x3f;

      /*   boucle sur les descripteurs de la table de niveau 3   */
      /*   ---------------------------------------------------   */
      for (iniv3 = iniv3_deb; iniv3 <= iniv3_fin ; iniv3++)
	 {
	 p = atab_niv3 + iniv3;
	 if (trans)
	    *p = (int) adr_cour | 0x63;  /*   0x63 = non cachable, resident   */
	 else {
	    print("data");
	    *p = (int) AllocPhysique() | 0x63;
	    }
         adr_cour = adr_cour + 0x1000;
	 }
      }
   }
}




void test_acces()
{
int i;
char *pb;

pb = (char*)0x10003;
i= *pb;//yes
test_U();
pb = (char*)(0x10003+4096);
i= *pb;//no
}




main()
{
char *pb;
int i;

SetHandler(VBR+36*4,msg_S);//trap 4
SetHandler(VBR+37*4,msg_U);//trap 5

/*   initialisation du tableau tdesc_ptp   */
/*   -----------------------------------   */
for (i = 0; i < NB_DESC_PTP; i++)
   tdesc_ptp[i].valid = 0;


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
    

/*
 * 1 ere étape 
 * 	1 table de niv 2 suffit
 */
print("creation ArbreS");
ArbreS = AllocTablePages(NIV1_2);
print("creation ArbreU");
ArbreU = AllocTablePages(NIV1_2);
/*
 * Mapping espace superviseur
 */
print("ArbreS text");
MapEspaceVirtuel(ArbreS,(char*) 0x10000,(char*)(0x10000+4000),1);
print("ArbreS pileS");
MapEspaceVirtuel(ArbreS,(char*)(0x400000-4096),(char*)0x3FFFFF,1);
print("ArbreS pileU");
MapEspaceVirtuel(ArbreS,(char*)(0x203000-4096),(char*)0x203FFF,1);
/*
print("ArbreS");
MapEspaceVirtuel(ArbreS,(char*)(0x350000-4096),(char*)0x34FFFF,1);
*/
/*
 * Mapping espace U
 */
print("ArbreU text");
MapEspaceVirtuel(ArbreU,(char*)0x10000,(char*)(0x10000+4095),1);
print("ArbreU pile");
MapEspaceVirtuel(ArbreU,(char*)(0x350000-4096),(char*)0x34FFFF, 0);

 

asm("|instr. cache on, data cache on
    move.l #0x80008000,%a0
    movec.l %a0,%cacr
    ");
asm("move.l %0,%%a0;movec.l %%a0,%%srp" : : "a" (ArbreS) : "a0"  );
asm("move.l %0,%%a0;movec.l %%a0,%%urp" : : "a" (ArbreU) : "a0" );

asm("  
    |vm Enable, page de 4k
    move.w #0x8000,%d0
    movec.l %d0,%tcr
    pflusha
    |
    | maintenant on est en VM
    |
    move.l #0x350000,%a0
    move.l %a0,%usp
    " );
    
test_S();//OK
print("test1");
asm("    move.w #0,%sr
    ");
test_U();
//a-t-on accès
test_acces();



//test_S();
//il faudrait restorer le mode S pour terminer correctement
}

