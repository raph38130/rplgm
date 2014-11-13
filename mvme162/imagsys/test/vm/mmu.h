/******************************************************************************/
/**                                                                          **/
/**   PTP signifie ``Page de Tables des Pages'' c'est a dire page contenant  **/
/**   des tables de pages.                                                   **/
/**   Les tables de page de niveaux 1 et 2 font 512 octets, il y en a donc   **/
/**   8 par page de 4k.                                                      **/
/**   Les tables de pages de niveau 3 font 256 octets, il y en a donc 16     **/
/**   par page.                                                              **/
/**   De maniere a ne pas avoir de fragmentation dans les PTP, on gerera des **/
/**   pages contenant des tables de niveaux 1 et 2 exclusivment et des       **/
/**   pages contenant des tables de niveau 3 exclusivement.                  **/
/**     								     **/
/**   Modifications mineures par RP					     **/
/******************************************************************************/
#define PHYS_MEM 0x400000
#define TAILLE_PAGE     4096
#define NB_BITS_PAR_MOTS 32

#define NB_PAGES_PHYS_MEM PHYS_MEM / TAILLE_PAGE
#define TAILLE_BITMAP_PAGES_ALLOUEES NB_PAGES_PHYS_MEM / NB_BITS_PAR_MOTS

typedef unsigned long int  paddr_t;        /* <physical address> type */


/*   descripteur de page de tables de pages                                   */
/*   les bits "alloue" permettent de savoir si une table est allouee ou non   */
/*   dans la page. Pour les niveaux 1 et 2 seuls 8 bits sur les 16 sont       */
/*   utilises. Le champ "type" vaut 0 pour les niveaux 1 et 2 et 1 pour       */
/*   une page contenant des tables de niveau 3                                */
#define NIV1_2 0
#define NIV3   1
struct desc_ptp
   {
   unsigned int adr;         /*   adresse physique de la page                 */
   unsigned int valid : 1;   /*   descripteur valide ou non                   */
   unsigned int type : 1;    /*   page pour tables de niveaux (1 et 2) ou 3   */
   unsigned int : 14;        /*   inutilise                                   */
   unsigned int alloue : 16; /*   indicateur de table allouee                 */
   };
/*   tableau des descripteurs de page de tables de pages   */
#define NB_DESC_PTP 100

/*   descripteur dans les tables de pages   */
typedef int DESCR;

extern DESCR * AllocTablePages(int);
extern MapEspaceVirtuel(DESCR *,char *,char *,int);
