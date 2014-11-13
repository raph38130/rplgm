/*
 * allocation mémoire simpliste par bloc de 4k en DRAM
 *	0000 00000 	0040 0000	4Mo
 */
#include "mem.h"
#define print(s) outstr(s,s+strlen(s))

DESCR *ArbreS;
struct desc_ptp tdesc_ptp[NB_DESC_PTP];

void* DRAM_Alloc()
{
char s[30];

static int DRAMBASE=0x200000;
DRAMBASE += 4096;
print("DRAM alloc");
n_htos(DRAMBASE,s);
print(s);
return (void*)DRAMBASE;
}

/*
 * allocation d'une page de tables de descripteurs en SRAM
 *
 *	 ffe0 0000	ffe1 ffff	128Ko
 *
 * QUESTION : est-ce nécessaire d'allouer une page (4k) de tables
 *		oui, pour la programmer
 */


void* SRAM_Alloc()
{
char s[30];

static int SRAMBASE=0xffe00000;
SRAMBASE += 4096;
print("SRAM alloc");
n_htos(SRAMBASE,s);
print(s);
return (void*)SRAMBASE;
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
   tdesc_ptp[i].adr = (unsigned int) SRAM_Alloc();
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
void MapEspaceVirtuel(DESCR *arbre,char *adr_deb,char *adr_fin,int trans,char *adr_phy)
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
	 if (trans & TRANSPARANT)
	    *p = (int) adr_cour | 0x63 | ((trans & MODE_S) ? (1<<7) : 0);  /*   0x63 = non cachable, resident   */
	 else {
	    print("data");
	    *p = (int) adr_phy | 0x63;
	    }
         adr_cour = adr_cour + 0x1000;
	 }
      }
   }
}

void init_mm()
{
int i;

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
//construire Arbre initial
ArbreS = AllocTablePages(NIV1_2);
print("A");
//code e2s 2 pages + 162 Bug
MapEspaceVirtuel(ArbreS,0x0000,0x10000+2*4096-1,TRANSPARANT,0);//modeS
//vecteurs 68040 + data 162Bug + e2s + processU + pile S
//MapEspaceVirtuel(ArbreS,0,0x3FFFFF,TRANSPARANT,0);  //modeS
print("B");
//ssp e2s 1 page
MapEspaceVirtuel(ArbreS,0x400000-4096,0x3FFFFF,TRANSPARANT,0);  //modeS
print("C");

asm("move.l %0,%%a0;movec.l %%a0,%%srp" : : "a" (ArbreS) : "a0"  );
asm("  
    |vm Enable, page de 4k
    move.w #0x8000,%d0
    movec.l %d0,%tcr
    pflusha
    |
    | maintenant on est en VM
    |
    " );
print("on est en VM"); 
 
}






