#include "commun.h"
#include "control-regs.h"
#include "mmu.h"

extern int _end;

/******************************************************************************/
/*                                                                            */
/*                                AllocPhysPage                               */
/*                                                                            */
/*   But:                                                                     */
/*      Alloue une page de memoire physique                                   */
/*                                                                            */
/******************************************************************************/
char * AllocPhysPage()
{
int i,j, trouve;

trouve = 0;
for (i = 0; i < TAILLE_BITMAP_PAGES_ALLOUEES; i++)
   {
   for ( j = 0; j < 32; j++)
      if ((t_bitmap_pages_allouees[i] & ( 1 << j)) == 0)
	 { trouve = 1; break; }
   if (trouve) break;
   }
if (! trouve) return((char *)0);
t_bitmap_pages_allouees[i] = t_bitmap_pages_allouees[i] | ( 1 << j);
return ((char *) ((i * 32 + j) * 4096));
}

/******************************************************************************/
/*                                                                            */
/*                                FreePhysPage                                */
/*                                                                            */
/*   But:                                                                     */
/*      Libere une page de memoire physique                                   */
/*                                                                            */
/******************************************************************************/
void FreePhysPage(char * adr)
{
unsigned int apage;
int nopage, i, j;
apage = (unsigned int) adr & 0xfffff000;
nopage = apage / 4096;
i = nopage / 32;
j = nopage % 32;
t_bitmap_pages_allouees[i] = t_bitmap_pages_allouees [i] & ~( 1 << j);
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
DESCR * AllocTablePages(niv)
int niv;
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
   tdesc_ptp[i].adr = (unsigned int) AllocPhysPage();
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
kprintf("AllocTablePages : atable = %x\n",atable);
return(atable);
}


/******************************************************************************/
/*                                                                            */
/*                                FreeTablePages                              */
/*                                                                            */
/*   But:                                                                     */
/*      Libere une table de pages                                             */
/*                                                                            */
/******************************************************************************/
int FreeTablePages(DESCR *d)
{
int i, notable, trouve, tailletable; 
unsigned int apage;

/*   determination de l'adresse de la page qui contient cette table  */
apage = (unsigned int) d & 0xfffff000;

/*   On recherche le descripteur qui decrit cette page   */
trouve = 0;
for (i = 0; i < NB_DESC_PTP; i++)
   if (tdesc_ptp[i].valid && (tdesc_ptp[i].adr == apage))
      { trouve = 1; break; }

if (! trouve) return(-1);   /*   adresse fournie incorrecte   */
if (tdesc_ptp[i].type == NIV1_2) tailletable = 512; else tailletable = 256;
notable = ((unsigned int) d - apage) / tailletable;
tdesc_ptp[i].alloue = tdesc_ptp[i].alloue & ~(1  << notable);
return(1);
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
MapEspaceVirtuel(arbre,adr_deb,adr_fin,trans)
DESCR *arbre;
char *adr_deb, *adr_fin;
int trans;
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
	 else
	    *p = (int) AllocPhysPage() | 0x63;
         adr_cour = adr_cour + 0x1000;
	 }
      }
   }
}

/******************************************************************************/
/*                                                                            */
/*                                PrintTableDescriptor                        */
/*                                                                            */
/*   But:                                                                     */
/*                                                                            */
/******************************************************************************/
void PrintTableDescriptor(char *adr, DESCR *pd, int niv)
{
int udt;

udt = *pd & 0x3;
if (udt == 0 || udt == 1)
   return;
else 
   {
   if (niv == 2) kprintf("   ");
   kprintf("[%08x] : %08x", pd, *pd & 0xffffff00);
   if (*pd & 0x8) kprintf(" U");
   if (*pd & 0x4) kprintf(" W");
   kprintf(" UDT = Res.\n");
   }
}

/******************************************************************************/
/*                                                                            */
/*                                PrintPageDescriptor                         */
/*                                                                            */
/*   But:                                                                     */
/*                                                                            */
/******************************************************************************/
void PrintPageDescriptor(char *adr, DESCR *pd)
{
int pdt, cm;

pdt = *pd & 0x3;
if (pdt == 0)
   return;
else
   {
   kprintf("      ");
   kprintf("[%08x] : %08x --> %08x", pd, adr, *pd & 0xfffff000);
   if (*pd & 0x400) kprintf(" G");
   if (*pd & 0x80) kprintf(" S");
   cm = (*pd & 0x60) >> 5;
   switch (cm)
      {
      case 0 : kprintf(" CM = WT"); break;
      case 1 : kprintf(" CM = CB"); break;
      case 2 : kprintf(" CM = Non,S"); break;
      case 3 : kprintf(" CM = Non"); break;
      }
   if (*pd & 0x10) kprintf(" M");
   if (*pd & 0x08) kprintf(" U");
   if (*pd & 0x04) kprintf(" W");
   if (pdt == 1 || pdt == 3)
      kprintf(" PDT = Res.\n");
   else kprintf(" PDT = Indirect\n");
   }
}

/******************************************************************************/
/*                                                                            */
/*                                PrintTablePages                             */
/*                                                                            */
/*   But:                                                                     */
/*                                                                            */
/******************************************************************************/
void PrintTablePages(DESCR* arbre, int deb, int fin)
{
DESCR *atab_niv1, *atab_niv2, *atab_niv3;
DESCR *p, *q;
int iniv1, iniv2, iniv3;
char *adr_cour;
int udt;

adr_cour = (char *)(deb * 0x02000000);
atab_niv1 = arbre;
for (iniv1 = deb ; iniv1 <= fin; iniv1++)   /*  boucle sur les descr de niv 1 */
   {
   p = atab_niv1 + iniv1;
   PrintTableDescriptor(adr_cour, p, 1);
   udt = *p & 0x3;
   if (udt == 2 || udt == 3)
      {
      atab_niv2 = (DESCR *) (*p & 0xffffff00);
      for (iniv2 = 0 ; iniv2 <= 127 ; iniv2++) /*  boucle sur descr de niv 2  */
         {
         p = atab_niv2 + iniv2;
         PrintTableDescriptor(adr_cour, p, 2);
         udt = *p & 0x3;
         if (udt == 2 || udt == 3)
            {
            atab_niv3 = (DESCR *) (*p & 0xffffff00);
            for (iniv3 = 0; iniv3 <= 63 ; iniv3++)  /*   boucle descr niv 3   */
	       {
	       p = atab_niv3 + iniv3;
	       PrintPageDescriptor(adr_cour, p);
               adr_cour = adr_cour + 0x1000;
	       }
	    }
	 else adr_cour = adr_cour + 0x40000;
         }
      }
   else adr_cour = adr_cour + 0x2000000;
   }
}

/******************************************************************************/
/*                                                                            */
/*                                init_mmu                                    */
/*                                                                            */
/*   But:                                                                     */
/*                                                                            */
/******************************************************************************/
void init_mmu()
{
int i, j, nopage;
paddr_t lastpage, apage;


/*   initialisation du tableau t_bitmap_pages_allouees   */
/*   -------------------------------------------------   */
for (i = 0; i < TAILLE_BITMAP_PAGES_ALLOUEES; i++)
   t_bitmap_pages_allouees[i] = 0;
lastpage = ((paddr_t) &_end + TAILLE_PAGE ) & 0xfffff000;
kprintf("Derniere page utilisee par le noyau :%x\n",lastpage);
for (apage = 0; apage <= lastpage; apage = apage + 4096)
   {
   nopage = apage / 4096;
   i = nopage / 32;
   j = nopage % 32;
   t_bitmap_pages_allouees[i] = t_bitmap_pages_allouees[i] | ( 1 << j);
   }

/*   initialisation du tableau tdesc_ptp   */
/*   -----------------------------------   */
for (i = 0; i < NB_DESC_PTP; i++)
   tdesc_ptp[i].valid = 0;

/*   On cree l'arbre de tables des pages mappant l'espace systeme   */
/*   ------------------------------------------------------------   */
ArbreTablePagesSysteme = AllocTablePages(NIV1_2);
kprintf("ArbreTablePagesSysteme = %x\n",ArbreTablePagesSysteme);
MapEspaceVirtuel(ArbreTablePagesSysteme, (char *)0,
					(char *) (PHYS_MEM - TAILLE_PAGE),1);
MapEspaceVirtuel(ArbreTablePagesSysteme,(char*)0xff000000,(char *)0xfffff000,1);
WriteControlReg(SRP,ArbreTablePagesSysteme);

/*   on met en route le mmu et on enleve le transparent mis par 16xBug de     */
/*   maniere a ne fonctionner qu'avec les tables de traduction du MMU         */
WriteControlReg(TCR,0x8000);
WriteControlReg(DTT1,0);

kprintf("MMU mis en oeuvre (system only) !!!\n");
}

