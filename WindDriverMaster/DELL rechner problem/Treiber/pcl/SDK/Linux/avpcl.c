/*******************************************\
*                                           *
*  Linux Kernel Driver for PCL PCI Devices  *
*                                           *
*        Part of the shared library         *
*                                           *
* (c) 2005-2007 Alpermann + Velte EE GmbH   *
*                                           *
*                   V 3.36                  *
\********************************************
                        *      Source       *
                        *      avpcl.c      *
                        ********************/
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

#include "avpcl.h"
#include "params.h"
#include "pci_lib_linux.h"


int pclOpen( int *hdl, unsigned base )
{
	int fileDescriptor;

	switch (base){
		case PCL_BASE_PCI_1:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl0", O_RDWR);
			break;
		case PCL_BASE_PCI_2:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl1", O_RDWR);
			break;
		case PCL_BASE_PCI_3:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl2", O_RDWR);
			break;
		case PCL_BASE_PCI_4:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl3", O_RDWR);
			break;
		case PCL_BASE_PCI_5:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl4", O_RDWR);
			break;
		case PCL_BASE_PCI_6:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl5", O_RDWR);
			break;
		case PCL_BASE_PCI_7:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl6", O_RDWR);
			break;
		case PCL_BASE_PCI_8:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl7", O_RDWR);
			break;
		case PCL_BASE_PCI_9:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl8", O_RDWR);
			break;
		case PCL_BASE_PCI_10:
			fileDescriptor = open(PATHTODEVICEFILES"avpcl9", O_RDWR);
			break;
		default:
			return PCL_ERR_NOT_FOUND;
	} // end switch

	if (fileDescriptor != -1) *hdl = fileDescriptor;
	else return PCL_ERR_NOTOPEN;

	return PCL_OK;
}

int pclClose( int hdl )
{
	return close(hdl);
}

PCL_BOOL pclPciExec( PCI_HANDLE hPCI, BYTE cmd )
{
	BYTE ack, cmdr;
	struct timeval tstart, tstop, tdiff;
	struct timezone tz;
	unsigned int msec;

	ack = PCI_ReadACK(hPCI);
	PCI_WriteCMD(hPCI, cmd);

	gettimeofday(&tstart, &tz);

	while (ack == PCI_ReadACK(hPCI)) {
		gettimeofday(&tstop, &tz);
		timersub(&tstop, &tstart, &tdiff);
		msec = tdiff.tv_sec * 1000 + tdiff.tv_usec / 1000;
		if (msec > PCL_TIMEOUT) {
			if (ack != PCI_ReadACK(hPCI)) break; // last chance
			sprintf(PCI_ErrorString, "pclPciExec(): Sent CMD 0x%X, No ACK received\n", cmd);
			return(PCL_ERR_TIMEOUT);
		}
	}
	if ((cmdr = PCI_ReadCMDR(hPCI)) != cmd) {
		sprintf(PCI_ErrorString, "pclPciExec(): Sent CMD 0x%X, received CMDR 0x%x\n", cmd, cmdr);
		return(PCL_ERR_UK_CMD);
	}

	return PCL_OK;
}
