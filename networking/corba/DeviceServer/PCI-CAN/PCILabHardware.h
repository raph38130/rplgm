//+++++++++++++++++++++++++++++++++++++++++++++++
// Nom du fichier :
//		PCILabHardware.h
//
// Contenu :
//		Définitions niveau hardware pour le driver de
//		la PCILAB
//
// Dernière modification :
//		19/09/2001
//-----------------------------------------------

#ifndef __PCILAB_HARDWARE__
#define __PCILAB_HARDWARE__

//
// Diverses Constantes
//
#define	PCILAB_MAXIMUM_DEVICES		10

#define PCILAB_VENDORID		0x10E8
#define PCILAB_DEVICEID		0x5920
#define PCILAB_SUBVENDORID	0x0066
#define PCILAB_SUBSYSTEMID	0x0520

#define	PCILAB_NT_DEVICE_NAME		L"\\Device\\PCILab"
#define	PCILAB_WIN32_DEVICE_NAME	L"\\DosDevices\\PCILab"
#define	PCILAB_WIN32_DEVICE_PREFIX	L"PCILab"
#define	PCILAB_DRIVER_NAME			L"PCILabDriver"
#define PCILAB_DRIVER_CLASS_NAME	L"Digimétrie PCI Lab"

#define	PCILAB_MAX_NAME_LENGTH			80	


//
// PCILab card registers
//
#define PCILAB_AD_DATA_REGISTER			0x10
#define PCILAB_DA_CH1_OUTPUT_REGISTER	0x10
#define PCILAB_DA_CH2_OUTPUT_REGISTER	0x14
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


//
// AMCC Operation Register Offsets
//
#define AMCC_OP_REG_OMB1        0x00
#define AMCC_OP_REG_OMB2        0x04
#define AMCC_OP_REG_OMB3        0x08 
#define AMCC_OP_REG_OMB4        0x0c
#define AMCC_OP_REG_IMB1        0x10
#define AMCC_OP_REG_IMB2        0x14
#define AMCC_OP_REG_IMB3        0x18 
#define AMCC_OP_REG_IMB4        0x1c
#define AMCC_OP_REG_FIFO        0x20
#define AMCC_OP_REG_MWAR        0x24
#define AMCC_OP_REG_MWTC        0x28
#define AMCC_OP_REG_MRAR        0x2c
#define AMCC_OP_REG_MRTC        0x30
#define AMCC_OP_REG_MBEF        0x34
#define AMCC_OP_REG_INTCSR      0x38
#define AMCC_OP_REG_MCSR        0x3c
#define AMCC_OP_REG_APTCR		0x60
#define AMCC_OP_REG_MCSR_NVDATA  (AMCC_OP_REG_MCSR + 2) /* Data in byte 2 */
#define AMCC_OP_REG_MCSR_NVCMD   (AMCC_OP_REG_MCSR + 3) /* Command in byte 3 */

//
// Propriétés du bus PCI
//
#define PCILAB_MAXIMUM_PCI_BUS			256
#define PCILAB_MAXIMUM_PCI_DEVICES		32
#define PCILAB_MAXIMUM_PCI_FUNCTIONS	1


//
// Propriétés physiques de la carte
//
#define PCILAB_NB_MAX_BASE_ADDRESS		2
#define PCILAB_BASEADDRESS0_LENGTH		128
#define PCILAB_BASEADDRESS1_LENGTH		64
#define PCILAB_NB_VOIES_AD		16
#define PCILAB_NB_VOIES_DA		2
#define PCILAB_NB_PORTS			1
#define PCILAB_NB_TIMERS		1
#define PCILAB_NB_COMPTEURS		3
#define PCILAB_NB_COMPOSANTS	1
#define PCILAB_FREQUENCE_MIN	1					// Hz
#define PCILAB_FREQUENCE_MAX_NUMERIQUE		200000	// Hz
#define PCILAB_FREQUENCE_MAX_ANALOGIQUE		100000	// Hz
#define PCILAB_FREQUENCE_MAX_ANALOGIQUE_8	100000	// Hz
#define PCILAB_FREQUENCE_OSCILLATEUR		4000000	// Hz
#define PCILAB_TEMPS_CONVERSION_ANALOGIQUE	10000	// ns
#define PCILAB_FREQUENCE_TRANSITION_500N_10M	35	// Hz
#define PCILAB_PLEINE_ECHELLE_AD_PAR_DEFAUT		10	// V
#define PCILAB_PLEINE_ECHELLE_DA_PAR_DEFAUT		10	// V

//
// Constantes sur les bits des registres d'interruption 
//
#define PCILAB_IT_ALL		0x0F
#define PCILAB_IT_TIMER		0x08
#define PCILAB_IT_ABOUT		0x04
#define PCILAB_IT_HFULL		0x02
#define PCILAB_IT_DTRIG		0x01 


//
// Définition des ports logiques
//
#define PCILAB_PORTA     0
#define PCILAB_PORTB     1
#define PCILAB_PORTC     2


//
// Modes pour les timers
//
#define PCILAB_TIMER_MODE0			0x00
#define PCILAB_TIMER_MODE1			0x01
#define PCILAB_TIMER_MODE2			0x02
#define PCILAB_TIMER_MODE3			0x03
#define PCILAB_TIMER_MODE4			0x04
#define PCILAB_TIMER_MODE5			0x05
#define PCILAB_VAL_MAX_TIMER_MODE	0x06


//
// Affectation des gains
//
#define PCILAB_GAIN0			0x00
#define PCILAB_GAIN1			0x01
#define PCILAB_GAIN2			0x02
#define PCILAB_GAIN3			0x03
#define PCILAB_VAL_MAX_GAIN		0x04

#endif
