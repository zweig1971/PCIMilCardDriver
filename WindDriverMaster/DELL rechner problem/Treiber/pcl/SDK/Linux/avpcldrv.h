/*******************************************\
*                                           *
*  Linux Kernel Driver for PCL PCI Devices  *
*                                           *
* (c) 2005-2007 Alpermann + Velte EE GmbH   *
*                                           *
*                   V 3.36                  *
\********************************************
                        *      Header       *
                        *    avpcldrv.h     *
                        ********************/
#ifndef _AVPCL_H
#define _AVPCL_H
#include <linux/ioctl.h>

#define AVPCL_IOC_MAGIC	0x17

/*
	define ioctl-commands
*/
#define AVPCL_IOC_READDATA0  _IOR(AVPCL_IOC_MAGIC, 1,  int)
#define AVPCL_IOC_READDATA1  _IOR(AVPCL_IOC_MAGIC, 2,  int)
#define AVPCL_IOC_READDATA2  _IOR(AVPCL_IOC_MAGIC, 3,  int)
#define AVPCL_IOC_READDATA3  _IOR(AVPCL_IOC_MAGIC, 4,  int)
#define AVPCL_IOC_READDATA4  _IOR(AVPCL_IOC_MAGIC, 5,  int)
#define AVPCL_IOC_READDATA5  _IOR(AVPCL_IOC_MAGIC, 6,  int)
#define AVPCL_IOC_READDATA6  _IOR(AVPCL_IOC_MAGIC, 7,  int)
#define AVPCL_IOC_READDATA7  _IOR(AVPCL_IOC_MAGIC, 8,  int)
#define AVPCL_IOC_READDATA8  _IOR(AVPCL_IOC_MAGIC, 9,  int)
#define AVPCL_IOC_READDATA9  _IOR(AVPCL_IOC_MAGIC, 10, int)
#define AVPCL_IOC_READDATAA  _IOR(AVPCL_IOC_MAGIC, 11, int)

#define AVPCL_IOC_READCMDR   _IOR(AVPCL_IOC_MAGIC, 12, int)
#define AVPCL_IOC_READACK    _IOR(AVPCL_IOC_MAGIC, 13, int)

#define AVPCL_IOC_WRITECMD    _IOR(AVPCL_IOC_MAGIC, 14, int)
#define AVPCL_IOC_WRITECMDD0  _IOR(AVPCL_IOC_MAGIC, 15, int)
#define AVPCL_IOC_WRITECMDD1  _IOR(AVPCL_IOC_MAGIC, 16, int)
#define AVPCL_IOC_WRITECFG    _IOR(AVPCL_IOC_MAGIC, 17, int)
#define AVPCL_IOC_WRITEINTACK _IOR(AVPCL_IOC_MAGIC, 18, int)

#define AVPCL_IOC_READVERSION _IOR(AVPCL_IOC_MAGIC, 19, int)
#define AVPCL_IOC_READINTFLGL _IOR(AVPCL_IOC_MAGIC, 20, int)
#define AVPCL_IOC_READINTFLGH _IOR(AVPCL_IOC_MAGIC, 21, int)
#define AVPCL_IOC_WRITEINTCSR _IOR(AVPCL_IOC_MAGIC, 22, int)

//#define AVPCL_IOC_MAX	21


#endif // _AVPCL_H
