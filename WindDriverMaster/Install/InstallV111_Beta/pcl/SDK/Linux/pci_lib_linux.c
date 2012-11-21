/*******************************************\
*                                           *
*  Linux Kernel Driver for PCL PCI Devices  *
*                                           *
*        Part of the shared library         *
*            (WinDriver-Clone)              *
*                                           *
* (c) 2005-2007 Alpermann + Velte EE GmbH   *
*                                           *
*                   V 3.36                  *
\********************************************
                        *      Source       *
                        *  pci_lib_linux.c  *
                        ********************/
#include <sys/ioctl.h>
#include <unistd.h> // usleep

#include "pci_lib_linux.h"
#include "avpcl.h"
#include "avpcldrv.h"

char PCI_ErrorString[1024];
char pclErrorString[1024];

// fehlernummer
int PCI_err;

void Sleep(unsigned long timeout)
{
	usleep(timeout * 1000);
}

/*
BOOL PCI_Open (PCI_HANDLE *phPCI, DWORD dwVendorID, DWORD dwDeviceID, DWORD nCardNum, DWORD dwOptions)
{
	return false;
}

void PCI_Close(PCI_HANDLE hPCI)
{

}
*/

BYTE PCI_ReadDATA0 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA0, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x01  r  Data 1
BYTE PCI_ReadDATA1 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA1, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x02  r  Data 2
BYTE PCI_ReadDATA2 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA2, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x03  r  Data 3
BYTE PCI_ReadDATA3 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA3, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x04  r  Data 4
BYTE PCI_ReadDATA4 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA4, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x05  r  Data 5
BYTE PCI_ReadDATA5 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA5, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x06  r  Data 6
BYTE PCI_ReadDATA6 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA6, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x07  r  Data 7
BYTE PCI_ReadDATA7 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA7, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x08  r  Data 8
BYTE PCI_ReadDATA8 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA8, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x09  r  Data 9
BYTE PCI_ReadDATA9 (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATA9, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x0A  r  Data A (MSB)
BYTE PCI_ReadDATAA (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READDATAA, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
// 0x0C  r  Command Response
BYTE PCI_ReadCMDR (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READCMDR, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}

// 0x0D  r  Acknowledge
BYTE PCI_ReadACK (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READACK, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}

// 0x0E w Command
void PCI_WriteCMD (PCI_HANDLE hPCI, BYTE data)
{
	ioctl(hPCI, AVPCL_IOC_WRITECMD, &data);
}

// 0x0F w Command Data 0
void PCI_WriteCMDD0 (PCI_HANDLE hPCI, BYTE data)
{
	ioctl(hPCI, AVPCL_IOC_WRITECMDD0, &data);
}
// 0x10 w Command Data 1
void PCI_WriteCMDD1 (PCI_HANDLE hPCI, BYTE data)
{
	ioctl(hPCI, AVPCL_IOC_WRITECMDD1, &data);
}
// 0x11 w Config
void PCI_WriteCFG (PCI_HANDLE hPCI, BYTE data)
{
	ioctl(hPCI, AVPCL_IOC_WRITECFG, &data);
}

void PCI_WriteINT_ACK (PCI_HANDLE hPCI, BYTE data)
{
	ioctl(hPCI, AVPCL_IOC_WRITEINTACK, &data);
}

void PCI_WriteINTCSR (PCI_HANDLE hPCI, DWORD data)
{
	ioctl(hPCI, AVPCL_IOC_WRITEINTCSR, &data);
}

// 0x1F rw Version Number
BYTE PCI_ReadVERSION (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READVERSION, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}

BYTE PCI_ReadINTFLGL (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READINTFLGL, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}

BYTE PCI_ReadINTFLGH (PCI_HANDLE hPCI)
{
	BYTE result;
	if (ioctl(hPCI, AVPCL_IOC_READINTFLGH, &result) == 0){
		return result;
	} // end if
	else return PCL_ERR_IOCTL;
}
