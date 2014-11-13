// test.cpp : Defines the entry point for the console application.
// Dernière modif 30/07/01

#include "stdio.h"
#include "stdlib.h" 
#include "conio.h"

#define MAX_PCI_BUS_NUMBER		256
#define MAX_PCI_DEVICE_NUMBER	32

// Configuration des cartes PCI Digimétrie
#define PCI_VENDORID	0x10E8
#define PCI_DEVICEID	0x5920

// Définition des erreurs PCI
#define	PCI_SUCCESSFUL				0x00
#define	PCI_NOT_SUCCESSFUL			0x01
#define PCI_FUNC_NOT_SUPPORTED		0x81
#define	PCI_BAD_VENDOR_ID			0x83
#define	PCI_DEVICE_NOT_FOUND		0x86
#define	PCI_BAD_REGISTER_NUMBER		0x87
#define PCI_BIOS_INCORRECT_VERSION	0x88

// Définition des fonctions interruption PCI
#define PCI_FUNCTION_ID           0xb1
#define PCI_BIOS_PRESENT          0x01
#define FIND_PCI_DEVICE           0x02
#define FIND_PCI_CLASS_CODE       0x03
#define GENERATE_SPECIAL_CYCLE    0x06
#define READ_CONFIG_BYTE          0x08        

// PCILab card registers
//
#define PCILAB_AD_DATA_REGISTER			0x10
#define PCILAB_DA_CH1___outpwUT_REGISTER	0x10
#define PCILAB_DA_CH2___outpwUT_REGISTER	0x14
#define PCILAB_AD_CONTROL_REGISTER		0x18
#define PCILAB_AD_STATUS_REGISTER		0x18
#define PCILAB_DIGITAL_IO_REGISTER		0x1C
#define PCILAB_TRIGGER_REGISTER			0x20
#define PCILAB_COMPTEUR0				0x00
#define PCILAB_COMPTEUR1				0x04
#define PCILAB_COMPTEUR2				0x08
#define PCILAB_TIMER_CTRL				0x0C
#define PCILAB_AD_GAIN_REGISTER			0x24
#define PCILAB_BURST_NUMBER_REGISTER	0x28
#define PCILAB_AD_AUTOSCAN_REGISTER		0x2C
#define	PCILAB_AD_FUNCTION_REGISTER		0x30
#define PCILAB_AD_FIFO_RESET_REGISTER	0x34
#define PCILAB_IT_FUNCTION_REGISTER		0x38
#define PCILAB_IT_STATUS_REGISTER		0x38


typedef struct _PCI_SPACE {
    unsigned short ven_id;
    unsigned short dev_id;
    unsigned short command;
    unsigned short status;
    unsigned char rev;
    unsigned char class1;
    unsigned char class2;
    unsigned char class3;
    unsigned char cache_line;
    unsigned char latency;
    unsigned char header_type;
    unsigned char bist;
    unsigned long base[6];
    unsigned long res1;
    unsigned short svid;
	unsigned short sid;
    unsigned long exp_rom;
    unsigned long res2;
	unsigned long res3;
    unsigned char int_line;
    unsigned char int_pin;
    unsigned char min_gnt;
    unsigned char max_lat;
} PCI_SPACE;


// Paramètres des cartes PCI
typedef struct _PCI_PARAMETERS 
{
	unsigned short subvendorID;
	unsigned short systemID;
 	unsigned char interruptLine;
	unsigned long baseAddress0;
	unsigned long baseAddress1;
} PCI_PARAMETERS, *PPCI_PARAMETERS;


// Fonctions locales
void
GetPciCardConfiguration( 
	unsigned char busNumber, 
	unsigned char deviceNumber,
	unsigned char *configuration,
	unsigned char configurationLength );

unsigned short
GetDigimetriePciCardParameters(PPCI_PARAMETERS *pciParameters);         

short acquisition_pcilab(
	unsigned short premiere_voie,
	unsigned short nb_voies,
	unsigned long base0,
	unsigned long base1);


////////////////////////////
//   MAIN	
////////////////////////////
int main(int argc, char* argv[])
{                   
	unsigned long base0,base1;
	unsigned short i,premiere_voie,nb_voies;
	short erreur;
	unsigned short pciCardNumber;
	PPCI_PARAMETERS pciParameters = NULL;
	PPCI_PARAMETERS pPciParameters = NULL;

	pciCardNumber = GetDigimetriePciCardParameters( &pciParameters );
	pPciParameters = pciParameters;

	printf("Carte Digimétrie trouvées dans ce PC :\n");

	for(i=0; i<pciCardNumber; i++)
	{
		printf( "Carte n°%x : \n\tCarte : %4X\n\tBaseAddress0=%4lX, BaseAddress1=%4lX, IRQ=%d",
				i+1,
				pPciParameters->systemID,
				pPciParameters->baseAddress0 & 0xFFFC,
				pPciParameters->baseAddress1 & 0xFFFC,
				pPciParameters->interruptLine
			  );
		printf( "\n" );

		pPciParameters++;
	}

	if( pciCardNumber == 0 )
		printf("Aucune carte Digimétrie n'a été trouvée.\n"); 
		
	else
	{	
	base0=pPciParameters->baseAddress0 & 0xFFFC;
	base1=pPciParameters->baseAddress1 & 0xFFFC,  
	
		printf("\nentrer numéro 1ère voie : ");scanf("%d",&premiere_voie); 
		printf("\nentrer nombre de voies : ");scanf("%d",&nb_voies);
		
	
	erreur=acquisition_pcilab(premiere_voie,nb_voies,base0,base1);		
		printf("erreur digitool : %d",erreur); 
    }
	free(pciParameters);

	return 0;
}


unsigned short
GetDigimetriePciCardParameters(PPCI_PARAMETERS *pciParameters)
{

	unsigned char status = PCI_NOT_SUCCESSFUL;
	unsigned short pciCardNumber = 0;
	unsigned char busNumber;
	unsigned char deviceNumber;
	PPCI_PARAMETERS pPciParameters;
	PCI_SPACE pciSpace;

	//
	// Compte le nombre de cartes PCI Digimétrie
	//
	// Initialisation de SI pour chercher le 1er périphérique
	_asm
	{
		mov si,0x00
	}

	// On continue tant que la recherche n'est pas terminee
	// ou que le peripherique n'est pas le bon
	while ((status != PCI_DEVICE_NOT_FOUND)&& (status!=PCI_BAD_VENDOR_ID))
	{
		_asm 
		{	
			// Initialisation de l'interruption
			mov ah,PCI_FUNCTION_ID
			mov al,FIND_PCI_DEVICE
			// Passage des paramètres du périphérique
			mov cx,PCI_DEVICEID
			mov dx,PCI_VENDORID
			int 0x1a
			mov status,ah
			// Incrémente pour chercher le suivant
			inc si
		}
		if( status == 0 )
			pciCardNumber++;
	}

	//
	// Alloue la mémoire nécessaire pour stocker les informations
	//
	*pciParameters = (PPCI_PARAMETERS)malloc(pciCardNumber*sizeof(PCI_PARAMETERS));
	if( pciParameters == NULL )
		return NULL;
	pPciParameters = *pciParameters;

	// Réinitialise le status
	status = PCI_NOT_SUCCESSFUL;
	
	//
	// Récupère les informations de toutes les cartes PCI Digimétrie
	//
	// Initialisation de SI pour chercher le 1er périphérique
	_asm
	{
		mov si,0x00
	}

	// On continue tant que la recherche n'est pas terminee
	// ou que le peripherique n'est pas le bon
	while ((status != PCI_DEVICE_NOT_FOUND)&& (status!=PCI_BAD_VENDOR_ID))
	{
		_asm 
		{	
			// Initialisation de l'interruption
			mov ah,PCI_FUNCTION_ID
			mov al,FIND_PCI_DEVICE
			// Passage des paramètres du périphérique
			mov cx,PCI_DEVICEID
			mov dx,PCI_VENDORID
			int 0x1a
			mov status,ah
			// Incrémente pour chercher le suivant
			inc si
		}
		if( status == 0 ) 
		{
			_asm
			{	// Récupération des valeurs contenues dans les registres
				mov busNumber,bh
				mov deviceNumber,bl
			}
			// Charge l'espace de configuration de la carte PCI
			GetPciCardConfiguration( busNumber, 
									 deviceNumber,
									 (unsigned char*)&pciSpace,
									 sizeof(pciSpace)
								   );
			// Récupère les données intéressantes
			pPciParameters->baseAddress0 = pciSpace.base[0];
			pPciParameters->baseAddress1 = pciSpace.base[1];
			pPciParameters->interruptLine = pciSpace.int_line;
			pPciParameters->subvendorID = pciSpace.svid;
			pPciParameters->systemID = pciSpace.sid;
			// Pointe vers la prochaine structure
			pPciParameters++;
		}
	}

	return pciCardNumber;
}


void
GetPciCardConfiguration( 
	unsigned char busNumber, 
	unsigned char deviceNumber,
	unsigned char *configuration,
	unsigned char configurationLength )
{
	unsigned char i;
	unsigned char byteValue;

  	for (i=0; i<configurationLength; i++) 
  	{
		// Lecture d'un octet
		byteValue=5;
		_asm
		{
			// Initialisation de l'interruption
			mov ah,PCI_FUNCTION_ID
			mov al,READ_CONFIG_BYTE
			mov bh,busNumber
			mov bl,deviceNumber
			mov di,word ptr i
			int 0x1A
			// Recuperation de l'octet lu
			mov byteValue,cl
		}

		// Stocke la valeur dans un paramètre de la fonction
		*(configuration+i) = byteValue;
  	}
}

short acquisition_pcilab(
	unsigned short voie,
	unsigned short nb_voies,
	unsigned long base0,
	unsigned long base1
	)
{       
    unsigned short i;  
    int status,resultat;
	char byte=0;
                      
	//si voie > 15
	if(voie>15) return (5);

	//si nb_voies+voie > 15
	if((nb_voies+voie)>16) return (6);

      // initialisation paramètres du périphérique AMCC pour accés aux composants
	  _outpw((unsigned int)base0+0x60,0xA4A4);
	  _outpw((unsigned int)base0+0x38,0x0C0C);                                                   
	  
	  // met à 0le registre de A/D control
	_outpw((unsigned int)base1+ PCILAB_AD_CONTROL_REGISTER, 0);

	//
	// Gestion de la voie et du gain
	//
	// Autorise la programmation des gains
	_outpw((unsigned int)base1+ PCILAB_AD_AUTOSCAN_REGISTER, 0x0001);

	
	for(i=voie;i<nb_voies;i++)
	// Ecrit le numero de voie et le gain dans le registre A/D gain/channel (ici-gain=0)
	_outpw((unsigned int)base1+ PCILAB_AD_GAIN_REGISTER, i);

	// Interdit la programmation des gains
	_outpw((unsigned int)base1+ PCILAB_AD_AUTOSCAN_REGISTER,0x0000);

	//
	// Reset de la fifo
	//
	// Ecrit une valeur pour reseter la FIFO
	_outpw((unsigned int)base1+ PCILAB_AD_FIFO_RESET_REGISTER,   0x0000);
	
	// Lit une valeur pour être sûr de ne plus avoir de données dans la FIFO
	_inpw((unsigned int)base1+ PCILAB_AD_DATA_REGISTER);
	
	
	
		

	// Positionne le mode simple, déclenchement par soft, interruptions dévalidées
	// dans le registre A/D control
	_outpw((unsigned int)base1+ PCILAB_AD_CONTROL_REGISTER,  0);
	
	
	do
	{      
	
		printf( "voie :" );

		for(i=voie;i<nb_voies;i++)
	 	{
	 	// commute la voie
	 	_outpw((unsigned int)base1+ PCILAB_AD_GAIN_REGISTER, i);

		// Génére une impulsion trigger soft qui va entraîner une conversion A/D
		_outpw((unsigned int)base1+ PCILAB_TRIGGER_REGISTER, 0);

			// Attend que la conversion soit terminée
			do {
			status = _inpw((unsigned int)base1+ PCILAB_AD_STATUS_REGISTER );
		printf("%4X ",status);
			status &= 0x0001;
			} while( status == 0 );

		// Lecture résultat de conversion
		resultat= _inpw((unsigned int)base1+ PCILAB_AD_DATA_REGISTER);
		printf("%d = %d  ",i,resultat);
		
		}
	

		printf( "\n" );
		                  
//	if(getchar()) byte=1; 	                  
			                  
		
	}while(byte==0);
	
return (0);
}			 
		
	
		

	  