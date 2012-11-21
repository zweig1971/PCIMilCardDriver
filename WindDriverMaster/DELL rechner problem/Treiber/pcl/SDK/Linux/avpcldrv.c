/*******************************************\
*                                           *
*  Linux Kernel Driver for PCL PCI Devices  *
*                                           *
* (c) 2005-2007 Alpermann + Velte EE GmbH   *
*                                           *
*                   V 3.36                  *
\********************************************
                        *      Source       *
                        *    avpcldrv.c     *
                        ********************/

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/types.h>

#include "avpcldrv.h"
#include "params.h"
#include "avpcl.h"
#include "funcs.h"

MODULE_AUTHOR(VER_LEGAL_COPYRIGHT);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AVPCL Driver 4Linux Ver " AVPCL_VER_S "." AVPCL_SUB_S);

#define AVPCL_VENDOR_ID    0x10b5
#define AVPCL_DEVICE_5V    0x9050
#define AVPCL_DEVICE_3V    0x9030
#define AVPCL_SUBVENDOR_ID AVPCL_VENDOR_ID

static unsigned long 	memstart[PCL_HANDLES],
			memlen[PCL_HANDLES],
			membase[PCL_HANDLES];
static int device_count;

static int avpcl_major_number;

static u8 device_ready = 0;

static int avpcl_ioctl( struct inode *inode, struct file *instanz,
		       unsigned int cmd, unsigned long arg )
{
	if ( !device_ready ) return -EFAULT;

	int this_minor = MINOR(inode -> i_rdev);

	if ( this_minor >= device_count ) return -EFAULT;

	if (_IOC_TYPE(cmd) != AVPCL_IOC_MAGIC) return -ENOTTY;

	switch (cmd){

	case (AVPCL_IOC_READDATA0):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x00) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA1):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x04) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA2):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x08) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA3):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x0C) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA4):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x10) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA5):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x14) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA6):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x18) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA7):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x1C) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA8):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x20) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATA9):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x24) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READDATAA):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x28) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READINTFLGL):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x2C) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READINTFLGH):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x48) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READCMDR):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x30) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case (AVPCL_IOC_READACK):{
			u8 data = readb( (u8*)(membase[this_minor] + 0x34) );
			copy_to_user( (void *)arg, &data, sizeof(data) );
			return 0;
		} // end case
	case AVPCL_IOC_READVERSION:{
			u8 version = readb((u8*) membase[this_minor] + 0x7C);
			copy_to_user((void *)arg, &version, sizeof(version));
			return 0;
		} // end case

	case AVPCL_IOC_WRITECMD:{
			u8 comm;
			copy_from_user(&comm, (void*) arg, sizeof(comm));
			writeb(comm, (u8*) membase[this_minor] + 0x38);
			return 0;
		} // end case
	case AVPCL_IOC_WRITECMDD0:{
			u8 comm;
			copy_from_user(&comm, (void*) arg, sizeof(comm));
			writeb(comm, (u8*) membase[this_minor] + 0x3C);
			return 0;
		} // end case
	case AVPCL_IOC_WRITECMDD1:{
			u8 comm;
			copy_from_user(&comm, (void*) arg, sizeof(comm));
			writeb(comm, (u8*) membase[this_minor] + 0x40);
			return 0;
		} // end case
	case AVPCL_IOC_WRITECFG:{
			u8 comm;
			copy_from_user(&comm, (void*) arg, sizeof(comm));
			writeb(comm, (u8*) membase[this_minor] + 0x44);
			return 0;
		} // end case
	case AVPCL_IOC_WRITEINTACK:{
			u8 comm;
			copy_from_user(&comm, (void*) arg, sizeof(comm));
			writeb(comm, (u8*) membase[this_minor] + 0x4C);
			return 0;
		} // end case

		default: return -EINVAL;

	} // end switch
	return -EIO;
}


static irqreturn_t pci_isr( int irq, void *dev_id, struct pt_regs *regs )
{
    return IRQ_HANDLED;
}

static int device_probe(struct pci_dev *dev, const struct pci_device_id *id)
{

	if (device_count >= PCL_HANDLES){
		printk( KERN_ERR "AV PCL-PCI: Maximum Number of Devices reached.\n" );
		return -EIO;
	} // end if

	// reserve IRQ
	if(request_irq(dev->irq,pci_isr,SA_INTERRUPT|SA_SHIRQ,"pci_drv",dev)) {
		printk( KERN_ERR "AV PCL-PCI: IRQ %d not free.\n", dev->irq );
		return -EIO;
	} // end if

	//   reserve memory
	memstart[device_count] = pci_resource_start( dev, 2 );
	memlen[device_count]   = pci_resource_len( dev, 2 );
	if( request_mem_region( memstart[device_count], memlen[device_count], dev->dev.kobj.name ) == NULL ) {
		printk(KERN_ERR "AV PCL-PCI: Memory address conflict for device \"%s\"\n",
			dev -> dev.kobj.name);
		free_irq( dev -> irq, dev );
		return -EIO;
	} // end if

	// map memory
	membase[device_count] = (unsigned long) ioremap(memstart[device_count], memlen[device_count]);

	device_count++;

#if 0
	printk( KERN_DEBUG "--------------> probe ready "
			"(device_count %i, "
			"membase 0x%lx, "
			"devicename: %s )\n",
	device_count,
	membase[device_count],
	dev->dev.kobj.name );
#endif

	pci_enable_device( dev );
	device_ready = 1;
	return 0;
}

static void device_deinit( struct pci_dev *pdev )
{
	free_irq( pdev->irq, pdev );
	if ( memstart[device_count - 1] )
		release_mem_region( memstart[device_count - 1], memlen[device_count - 1] );
	if ( membase[device_count - 1] )
		iounmap( (void *) membase[device_count - 1] );
	device_count--;
	device_ready = 0;
}

static struct file_operations pci_fops = {
	.owner = THIS_MODULE,
	.ioctl = avpcl_ioctl,
};

static struct pci_device_id pci_drv_tbl[] __devinitdata = {
	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_5V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_LV_5V,  0, 0, 0 },
	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_5V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_D_5V,   0, 0, 0 },
	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_5V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_TS_5V,  0, 0, 0 },

	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_3V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_LV_3V, 0, 0, 0 },
	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_3V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_D_3V,  0, 0, 0 },
	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_3V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_HD_3V, 0, 0, 0 },
	{ AVPCL_VENDOR_ID, AVPCL_DEVICE_3V, AVPCL_SUBVENDOR_ID, PCL_PCI_SUBSYSTEM_DEVICE_ID_L_3V,  0, 0, 0 },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, pci_drv_tbl);

static struct pci_driver pci_drv = {
    .name     = "AV PCL-PCI",
    .id_table = pci_drv_tbl,
    .probe    = device_probe,
    .remove   = device_deinit,
};

static int __init avpcl_init( void )
{
// 	release_mem_region( 0xd4000000 , 0x100 );
// 	release_mem_region( 0xd5000000 , 0x100 );

	device_count = 0;
	memset( membase,  0, sizeof(membase)  );
	memset( memstart, 0, sizeof(memstart) );
	memset( memlen,   0, sizeof(memlen)   );

//	MODULE_DEVICE_TABLE(pci, pci_drv_tbl);

	// register device
	avpcl_major_number = register_chrdev( 0 /*automatic Majornumber*/, "AV PCL-PCI", &pci_fops );
	if( avpcl_major_number != 0 ) {
		if( pci_module_init( &pci_drv ) != 0 ){
			printk ( KERN_ERR "PCI init failed\n" );
			unregister_chrdev( avpcl_major_number, "AV PCL-PCI" );
			return -EIO;
		} // end if
#ifdef DEBUG
		printk( KERN_DEBUG "Anmeldung erfolgreich (Major Number %i)\n", avpcl_major_number );
#endif
		return 0;
	} // end if
	printk ( KERN_ERR "Init failed\n" );
	return -EIO;
}

static void __exit avpcl_exit( void )
{
	pci_unregister_driver( &pci_drv );
	if ( unregister_chrdev( avpcl_major_number, "AV PCL-PCI" ) == 0 ){
#ifdef DEBUG
	printk( KERN_DEBUG "Abmeldung erfolgreich\n" );
#endif
	    return;
	} // end if
	printk( KERN_ERR "Exit failed\n" );
}

module_init(avpcl_init);
module_exit(avpcl_exit);
