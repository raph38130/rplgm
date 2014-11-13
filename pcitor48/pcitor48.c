/* pcitor48_2.c
 *
 * driver linux PCI-TOR48
 *
 *
 * Revision history
 * Tue Dec 16 21:48:18 CET 2003 RP
 * 	- kernel 2.6 without interrupt
 *
 * Wed Feb 13 18:20:43 CET 2002 v0.3 RP
 *	- added blocking read on PortC (PC30)
 *
 * Sat Jan 19 21:58:55 CET 2002 v0.2 RP
 *	- Digimetrie give us s5920 initialisation procedure
 *	- portA = /dev/pcitor portB = /dev/pcitor1 portC = /dev/pcitor2
 *
 * Fri Dec 28 20:27:38 CET 2001 v0.1 RP  
 *	- Initial release
 *
 * compile command :
gcc -D__KERNEL__ -DMODULE -DMODVERSIONS \
-I /lib/modules/2.6.0/build/include/ \
-Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fomit-frame-pointer \
-fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 \
-march=i686 -falign-functions=4  \
-include /lib/modules/2.6.0/build/include/linux/modversions.h \
-c -o pcitor48_2.o pcitor48_2.c
ld -m elf_i386 -r pcitor48_2.o -o pcitor48_2.ko
 *
 * install command :
insmod pcitor48_2.o debug=1
 *
 * descriptor : one desc. for each 8 bits ports (I use CHRDEV=133)
mknod /dev/pcitor  c 133 0
mknod /dev/pcitor1 c 133 1
mknod /dev/pcitor2 c 133 2
 *
 * TODO :
 *	- multi cards support : search card_state in cards list
 */
      
#include <linux/version.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/tqueue.h>
#include <linux/interrupt.h>

#include "pcitor48.h"

#define CHRDEV		133

MODULE_AUTHOR("RP/LGM");
MODULE_DESCRIPTION("PCI-TOR48 (Digimetrie) driver : 48 digital I/O");
MODULE_LICENSE("GPL");
//MODULE_CLASSES("{sound}");

//MODULE_CLASSES("io");

//MODULE_DEVICES("{{digimetrie,pcitor48}}");

static int debug=0;
MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "debug mode");

static struct card_state {
	int iobase1;
	int iobase2;
	struct card_state* next;
	} *cards; /* a linked list of installed pcitor cards */
	
static int pcitor_open(struct inode *inode, struct file *file)
{
	if (debug) printk(KERN_INFO "open/mode %d",file->f_mode);
	switch (MINOR(inode->i_rdev)) {
		case 0 : 
		case 1 : 
		case 2 : break; /* nothing to do */
		default : return -ENODEV;
		}
	return 0;
}

static int pcitor_release(struct inode *inode, struct file *file)
{
	if (debug) printk(KERN_INFO "release");
	if (MINOR(inode->i_rdev)>2) return -ENODEV;
	//maybe safer to set device to input mode ?
	
	return 0;
}

static ssize_t pcitor_write(struct file *file, const char *buffer, size_t count, loff_t *ppos)
{
	struct card_state *s=(struct card_state *)file->private_data;
	unsigned int 	minor=MINOR(file->f_dentry->d_inode->i_rdev),
			DRA=s->iobase2 + 0x00,
			DRB=s->iobase2 + 0x04,
			DRC=s->iobase2 + 0x08;	

	if (debug) printk(KERN_INFO "write/file_mode %d minor %d",file->f_mode,minor);
	/* TO DO !!!
	copy_from_user(data,buffer,count);
	*/
	switch(minor) {
		case 0 : outb(*buffer,DRA);	
			 break;
		case 1 : outb(*buffer,DRB);
			 break;
		case 2 : outb(*buffer,DRC);
			 break; 
		default : return -ENODEV;
		}
	return 0;
}

static ssize_t pcitor_read(struct file *file, char *buffer, size_t count, loff_t *ppos)
{
	struct card_state *s=(struct card_state *)file->private_data;
	unsigned int 	minor=MINOR(file->f_dentry->d_inode->i_rdev),
			DRA=s->iobase2 + 0x00,
			DRB=s->iobase2 + 0x04,
			DRC=s->iobase2 + 0x08;	

	if (debug) printk(KERN_INFO "write/file_mode %d minor %d",file->f_mode,minor);
	switch(minor) {
		case 0 : *buffer = (char) (inb(DRA));
			 break;
		case 1 : *buffer = (char) (inb(DRB));
			 break;
		case 2 : *buffer = (char) (inb(DRC));
			 break; 
		default : return -ENODEV;
		}
	return 1;
}

static int pcitor_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{	
	if (debug) printk(KERN_INFO "ioctl cmd=%d",cmd);
return 0;
}


static /*const*/ struct file_operations ops = {
	owner:		THIS_MODULE,
	read: 		pcitor_read,
	write:		pcitor_write,
	ioctl:		pcitor_ioctl,
	open:		pcitor_open,
	release:	pcitor_release,
	};

static struct pci_device_id digimetrie_ids[] = {
	{PCI_VENDOR_ID_AMCC, 0x5920, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{0,},
/*	{PCI_VENDOR_ID_CMEDIA, PCI_DEVICE_ID_CMEDIA_CM8338A, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{PCI_VENDOR_ID_CMEDIA, PCI_DEVICE_ID_CMEDIA_CM8338B, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{PCI_VENDOR_ID_CMEDIA, PCI_DEVICE_ID_CMEDIA_CM8738, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{PCI_VENDOR_ID_CMEDIA, PCI_DEVICE_ID_CMEDIA_CM8738B, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{PCI_VENDOR_ID_AL, PCI_DEVICE_ID_CMEDIA_CM8738, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{0,},
*/
};
MODULE_DEVICE_TABLE(pci, digimetrie_ids);


static int  __init probe_pcitor(struct pci_dev *pcidev, const struct pci_device_id *pci_id)
{
	struct card_state *s;
	int iobase1,iobase2;
	
	printk(KERN_NOTICE "pcitor48 driver: loading v0.4 / compile date " __TIME__ " " __DATE__ "\n");
	pci_enable_device(pcidev);
	iobase1 = pci_resource_start(pcidev, 0);
	iobase2 = pci_resource_start(pcidev, 1);
	s = kmalloc(sizeof(*s), GFP_KERNEL);
	if (!s) {
		printk(KERN_WARNING "out of memory\n");
		return -ENOMEM;
		}
	s->iobase1=iobase1;
	s->iobase2=iobase2;
	
	cards=s;
	if (debug) {
		printk(KERN_INFO "iobase1 = 0x%x\n",iobase1);
		printk(KERN_INFO "iobase2 = 0x%x\n",iobase2);
		}
	pci_set_master(pcidev);	/* enable bus mastering */
	if (!request_region(iobase2, 64, "pcitor")) {
		printk(KERN_INFO "pcitor: io ports %#x-%#x in use\n", iobase2,iobase2+64-1);
		return -ENODEV;
		}
	register_chrdev(CHRDEV,"pcitor48",&ops);
	
	/* amcc 5920 configuration (pci bridge)*/
	/* APTCR  Add-on Pass Thru Configuration Register */
	outl(0xA4A4A4A4,s->iobase1+0x60);
	/* reset pseudo 8255   */
	outb(0x00,s->iobase2+0x38);
	return 0;
}

static void __devexit pcitor_remove(struct pci_dev *pci)
{	struct card_state *s=cards;
	/* amcc5920 INTCSR : do not forward irq */
	outl(0x00000c0c,s->iobase1+0x38);
	release_region(s->iobase2, 64);
	unregister_chrdev(CHRDEV,"pcitor48");
	kfree(s);
	printk(KERN_INFO "pcitor48: unloading\n");
}

static struct pci_driver driver = {
	.name = "PCI-TOR48",
	.id_table = digimetrie_ids,
	.probe = probe_pcitor,
	.remove = __devexit_p(pcitor_remove),
};

static void  __exit exit_drv(void)
{
	pci_unregister_driver(&driver);
	
}
static int __init init_drv(void)
{
	int err;

	if ((err = pci_module_init(&driver)) < 0) {
		printk(KERN_ERR "card not found or device busy\n");
		return err;
	}
return 0;
}
module_init(init_drv);
module_exit(exit_drv);
