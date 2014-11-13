//++
// Fonction:
//		PCILabResetCardRegisters
//
// Description:
//		Cette fonction permet d'initialiser les principaux registres
//		de la carte PCI avec en particulier mise à zéro des interruptions.
//
// Arguments:
//		Pointeur vers le DeviceObject
//
// Valeur de retour:
//		VOID
//--
VOID
PCILabResetCardRegisters(
	IN PDEVICE_EXTENSION pDeviceExtension 
	)
{
	//
	// Mise en place de quatre wait states
	//
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress0 
								 + AMCC_OP_REG_APTCR ),
					  0xA4A4A4A4 );
	//
	// Interdiction de générer des interruptions.
	//
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress0 
								 + AMCC_OP_REG_INTCSR ),
					  0x00000C0C);
	//
	//Initialisation des registres Digimétrie
	//
	// Registre CH1 D/A Data Reg.
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1
								 + PCILAB_DA_CH1_OUTPUT_REGISTER ),
					  2048 );

	// Registre CH2 D/A Data Reg.
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1
								 + PCILAB_DA_CH2_OUTPUT_REGISTER ),
					  2048 );
	
	// Registre A/D Control Reg.
	pDeviceExtension->ControlRegisterStatus = 0;
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1 
								 + PCILAB_AD_CONTROL_REGISTER ),
					  pDeviceExtension->ControlRegisterStatus );
	
	// Registre entrée/sortie numérique
	pDeviceExtension->DigitalOutRegisterStatus = 0;
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1 
								 + PCILAB_DIGITAL_IO_REGISTER ),
					  pDeviceExtension->DigitalOutRegisterStatus );

	// Registre A/D Burst No. Reg.
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1 
								 + PCILAB_BURST_NUMBER_REGISTER ),
					  0 );

	// Registre A/D Auto Scan Mode 
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1 
								 + PCILAB_AD_AUTOSCAN_REGISTER ),
					  0 );

	// Registre A/D Function Reg.
	pDeviceExtension->FunctionRegisterStatus = 0;
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1 
								 + PCILAB_AD_FUNCTION_REGISTER ),
					  pDeviceExtension->FunctionRegisterStatus );
	
	// Registre A/D Data FIFO Reset
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1 
								 + PCILAB_AD_FIFO_RESET_REGISTER ),
					  0 );
	// Registre Interrupt Control Reg.
	pDeviceExtension->InterruptRegisterStatus = 0;
	WRITE_PORT_ULONG( (PULONG) ( (ULONG)pDeviceExtension->BaseAddress1
								 + PCILAB_IT_FUNCTION_REGISTER ),
					  pDeviceExtension->InterruptRegisterStatus );

	// Dévalide les flags pour les fonctions xxxscan
	pDeviceExtension->dinscanFinished = TRUE;
	pDeviceExtension->doutscanFinished = TRUE;
	pDeviceExtension->ainscanFinished = TRUE;
	pDeviceExtension->aoutscanFinished = TRUE;
}



//++++++++++++++++++++++++++++++++++++++++++++++++++
// Nom du fichier :
//		PCILabFonctionsAnalogique.c
//
// Contenu :
//		Routines qui permettent l'acquisition
//		et la génération de signaux analogiques :
//			- Ain()
//			- Ainscan()
//			- Aout()
//			- Aoutscan()
//
// Dernière modification :
//		19/09/2001
//--------------------------------------------------
#include "PCILabDriver.h"

#define ERR_DT_TIMER      		29	// erreur chargement timer
#define ERR_DT_FIFOPLEINE		24	// l'acquisition n'a pas eu le temps de 
									// vider le buffer.


//++
// Fonction:
//		Ain
//
// Description:
//		Cette fonction permet une conversion analogique/numérique.
//		Elle retourne la valeur brute du convertisseur.
//
// Valeur de retour:
//		STATUS_SUCCESS si succès
//--
NTSTATUS
Ain(
	IN PDEVICE_EXTENSION pDeviceExtension,
	IN PIRP pIrp
	)
{
	// Valeur du bit diff
	UCHAR diff = 0;

	// Charge les buffers d'entrée/sortie
	PPCILAB_AIN_IN_BUFF pInBuff = pIrp->AssociatedIrp.SystemBuffer;
	PPCILAB_AIN_OUT_BUFF pOutBuff = pIrp->AssociatedIrp.SystemBuffer;

	//
	// Gestion du registre de controle A/D
	//
	// Calcule le bit diff
	diff = (pInBuff->mode & DT_AD_SIMP) >> 14;
	// Met à diff le bit Diff
	if( diff == 1 )
		pDeviceExtension->ControlRegisterStatus |= 0x40;
	else
		pDeviceExtension->ControlRegisterStatus &= 0xBF;

	// Met à 0 les bits ExtM et TmrTr
	pDeviceExtension->ControlRegisterStatus &= 0xF3;
		
	// Ecrit sur le registre de controle 
	WRITE_PORT_UCHAR( (PUCHAR) ((ULONG)pDeviceExtension->BaseAddress1
								+ PCILAB_AD_CONTROL_REGISTER),
					   pDeviceExtension->ControlRegisterStatus 
					);

	//
	// Gestion de la voie et du gain
	//
	// Autorise la programmation des gains
	WRITE_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
							      + PCILAB_AD_AUTOSCAN_REGISTER),
					   0x0001
					 );

	// Ecrit le numero de voie et le gain dans le registre A/D gain/channel
	WRITE_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
								  + PCILAB_AD_GAIN_REGISTER),
					   (USHORT) (pInBuff->voie + (pInBuff->gain << 8))
					 );

	// Interdit la programmation des gains
	WRITE_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
							      + PCILAB_AD_AUTOSCAN_REGISTER),
					   0x0000
					 );

	//
	// Reset de la fifo
	//
	// Ecrit une valeur pour reseter la FIFO
	WRITE_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
								  + PCILAB_AD_FIFO_RESET_REGISTER),
					   0x0000
					 );
	
	// Lit le registre pour valider le reset
	READ_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
								 + PCILAB_AD_DATA_REGISTER) 
					);	

	//
	// Lecture de la donnée 
	//
	// Première acquisition pour compenser le temps d'établissement
	// de l'amplificateur
	PCILabAquire( pDeviceExtension );
	// Acquisition réelle de la valeur en cours
	pOutBuff->data = PCILabAquire( pDeviceExtension );

	return STATUS_SUCCESS;
}

//++
// Fonction:
//		Aout
//
// Description:
//		Cette fonction permet d'écrire sur un port de sortie
//		analogique.
//		Elle écrit la valeur brute donnée.
//
// Valeur de retour:
//		STATUS_SUCCESS si succès
//--
NTSTATUS
Aout(
	IN PDEVICE_EXTENSION pDeviceExtension,
	IN PIRP pIrp
	)
{
	// Charge les buffers d'entrée/sortie
	PPCILAB_AOUT_IN_BUFF pInBuff = pIrp->AssociatedIrp.SystemBuffer;
	PPCILAB_AOUT_OUT_BUFF pOutBuff = pIrp->AssociatedIrp.SystemBuffer;

	// Si l'écriture se fait sur la voie du DAC égale à 0
	if( pInBuff->voie == 0 )
		// Ecrit dans le registre
		WRITE_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
									  + PCILAB_DA_CH1_OUTPUT_REGISTER),
						   (USHORT) (pInBuff->data >> 4)
						 );
	// Si elle se fait sur la voie du DAC 1
	else
		// Ecrit dans le registre
		WRITE_PORT_USHORT( (PUSHORT) ((ULONG)pDeviceExtension->BaseAddress1
									  + PCILAB_DA_CH2_OUTPUT_REGISTER),
						   (USHORT) (pInBuff->data >> 4)
						 );

	return STATUS_SUCCESS;
}

