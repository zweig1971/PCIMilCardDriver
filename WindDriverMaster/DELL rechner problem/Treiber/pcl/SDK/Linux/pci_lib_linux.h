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
                        *      Header       *
                        *  pci_lib_linux.h  *
                        ********************/

#ifndef _PCI_LIB_LINUX_H_
#define _PCI_LIB_LINUX_H_

#include <asm/types.h>

//---------------------------------------------------------------------------

#ifndef PATHTODEVICEFILES
#define PATHTODEVICEFILES "./"
#endif

#define PCI_HANDLE int
#define PCI_ADDR unsigned long
#define WD_PCI_SLOT int

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char BYTE;							/* size is 1 */
#endif // !_BYTE_DEFINED

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short WORD;						/* size is 2 */
#endif // !_WORD_DEFINED

#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
typedef unsigned long DWORD;						/* size is 4 */
#endif // !_DWORD_DEFINED

#ifndef _BOOL_DEFINED
#define _BOOL_DEFINED
typedef unsigned char BOOL;
#endif

#ifndef INT
#define INT int
#endif
#ifndef PVOID
#define PVOID void *
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
// To use 64bit / 32bit operations, the
// makro do_div() from <asm/div64.h> must be used.
// The file funcs.cpp makes use of those functions.
//#include <asm/div64.h>
#ifndef __int64
#define __int64 long long
#endif
#ifndef NULL
#define NULL (void *)0
#endif


//---------------------------------------------------------------------------

#define INTCSR_ACK 0x543	// wert, der als interrupt acknowledge ins
				// register INTCSR geschrieben werden muss


//---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

// enum { PCI_DEFAULT_VENDOR_ID = 0x10B5 };
// enum { PCI_DEFAULT_DEVICE_ID_5V = 0x9050 };
// enum { PCI_DEFAULT_DEVICE_ID_3V = 0x9030 };

typedef enum
{
    PCI_MODE_BYTE   = 0,
    PCI_MODE_WORD   = 1,
    PCI_MODE_DWORD  = 2
} PCI_MODE;

// typedef enum
// {
//     PCI_AD_BAR0 = AD_PCI_BAR0,
//     PCI_AD_BAR1 = AD_PCI_BAR1,
//     PCI_AD_BAR2 = AD_PCI_BAR2,
//     PCI_AD_BAR3 = AD_PCI_BAR3,
//     PCI_AD_BAR4 = AD_PCI_BAR4,
//     PCI_AD_BAR5 = AD_PCI_BAR5,
//     PCI_AD_EPROM = AD_PCI_BAR_EPROM,
// } PCI_ADDR;

// Number of IO and memory ranges
// enum { PCI_ITEMS = AD_PCI_BARS };

// PCI register definitions
// enum { PCI_DATA0_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA0_OFFSET = 0x0 };
// enum { PCI_DATA1_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA1_OFFSET = 0x4 };
// enum { PCI_DATA2_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA2_OFFSET = 0x8 };
// enum { PCI_DATA3_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA3_OFFSET = 0xc };
// enum { PCI_DATA4_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA4_OFFSET = 0x10 };
// enum { PCI_DATA5_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA5_OFFSET = 0x14 };
// enum { PCI_DATA6_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA6_OFFSET = 0x18 };
// enum { PCI_DATA7_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA7_OFFSET = 0x1c };
// enum { PCI_DATA8_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA8_OFFSET = 0x20 };
// enum { PCI_DATA9_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATA9_OFFSET = 0x24 };
// enum { PCI_DATAA_SPACE = PCI_AD_BAR2 };
// enum { PCI_DATAA_OFFSET = 0x28 };
// enum { PCI_CMDR_SPACE = PCI_AD_BAR2 };
// enum { PCI_CMDR_OFFSET = 0x30 };
// enum { PCI_ACK_SPACE = PCI_AD_BAR2 };
// enum { PCI_ACK_OFFSET = 0x34 };
// enum { PCI_CMD_SPACE = PCI_AD_BAR2 };
// enum { PCI_CMD_OFFSET = 0x38 };
// enum { PCI_CMDD0_SPACE = PCI_AD_BAR2 };
// enum { PCI_CMDD0_OFFSET = 0x3c };
// enum { PCI_CMDD1_SPACE = PCI_AD_BAR2 };
// enum { PCI_CMDD1_OFFSET = 0x40 };
// enum { PCI_CFG_SPACE = PCI_AD_BAR2 };
// enum { PCI_CFG_OFFSET = 0x44 };
// enum { PCI_VERSION_SPACE = PCI_AD_BAR2 };
// enum { PCI_VERSION_OFFSET = 0x7c };
// enum { PCI_INTFLG_SPACE = PCI_AD_BAR2 };
// enum { PCI_INTFLG_OFFSET = 0x2c };
// enum { PCI_INTCSR_SPACE = PCI_AD_BAR0 };
// enum { PCI_INTCSR_OFFSET = 0x4c };
// enum { PCI_INT_ACK_SPACE = PCI_AD_BAR2 };
// enum { PCI_INT_ACK_OFFSET = 0x4c };

// #ifndef NDEBUG
// enum { PCI_EROMBRD_SPACE = PCI_AD_BAR0 };
// enum { PCI_EROMBRD_OFFSET = 0x38 };
// #endif

// typedef struct PCI_STRUCT *PCI_HANDLE;
//


void Sleep(unsigned long timeout);

// typedef struct
// {
//     DWORD dwCounter;   // number of interrupts received
//     DWORD dwLost;      // number of interrupts not yet dealt with
//     BOOL fStopped;     // was interrupt disabled during wait
// } PCI_INT_RESULT;

// typedef void (*PCI_INT_HANDLER)( PCI_HANDLE hPCI, PCI_INT_RESULT *intResult);

// // options for PCI_Open
// enum { PCI_OPEN_USE_INT =   0x1 };


BOOL PCI_Open (PCI_HANDLE *phPCI, DWORD dwVendorID, DWORD dwDeviceID, DWORD nCardNum, DWORD dwOptions);
// void PCI_RegisterWinDriver();
void PCI_Close(PCI_HANDLE hPCI);
// DWORD PCI_CountCards (DWORD dwVendorID, DWORD dwDeviceID);
// BOOL PCI_IsAddrSpaceActive(PCI_HANDLE hPCI, PCI_ADDR addrSpace);

// void PCI_GetPciSlot(PCI_HANDLE hPCI, WD_PCI_SLOT *pPciSlot);

// General read/write function
// void PCI_ReadWriteBlock(PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset, BOOL fRead, PVOID buf, DWORD dwBytes, PCI_MODE mode);
// BYTE PCI_ReadByte (PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset);
// WORD PCI_ReadWord (PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset);
// DWORD PCI_ReadDword (PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset);
// void PCI_WriteByte (PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset, BYTE data);
// void PCI_WriteWord (PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset, WORD data);
// void PCI_WriteDword (PCI_HANDLE hPCI, PCI_ADDR addrSpace, DWORD dwOffset, DWORD data);
// 0x00  r  Data 0 (LSB)
BYTE PCI_ReadDATA0 (PCI_HANDLE hPCI);
// 0x01  r  Data 1
BYTE PCI_ReadDATA1 (PCI_HANDLE hPCI);
// 0x02  r  Data 2
BYTE PCI_ReadDATA2 (PCI_HANDLE hPCI);
// 0x03  r  Data 3
BYTE PCI_ReadDATA3 (PCI_HANDLE hPCI);
// 0x04  r  Data 4
BYTE PCI_ReadDATA4 (PCI_HANDLE hPCI);
// 0x05  r  Data 5
BYTE PCI_ReadDATA5 (PCI_HANDLE hPCI);
// 0x06  r  Data 6
BYTE PCI_ReadDATA6 (PCI_HANDLE hPCI);
// 0x07  r  Data 7
BYTE PCI_ReadDATA7 (PCI_HANDLE hPCI);
// 0x08  r  Data 8
BYTE PCI_ReadDATA8 (PCI_HANDLE hPCI);
// 0x09  r  Data 9
BYTE PCI_ReadDATA9 (PCI_HANDLE hPCI);
// 0x0A  r  Data A (MSB)
BYTE PCI_ReadDATAA (PCI_HANDLE hPCI);
// 0x0C  r  Command Response
BYTE PCI_ReadCMDR (PCI_HANDLE hPCI);
// 0x0D  r  Acknowledge
BYTE PCI_ReadACK (PCI_HANDLE hPCI);
// 0x0E w Command
void PCI_WriteCMD (PCI_HANDLE hPCI, BYTE data);
// 0x0F w Command Data 0
void PCI_WriteCMDD0 (PCI_HANDLE hPCI, BYTE data);
// 0x10 w Command Data 1
void PCI_WriteCMDD1 (PCI_HANDLE hPCI, BYTE data);
// 0x11 w Config
void PCI_WriteCFG (PCI_HANDLE hPCI, BYTE data);
// 0x1F rw Version Number
BYTE PCI_ReadVERSION (PCI_HANDLE hPCI);
// 0x0B  r  Interrupt Flags
BYTE PCI_ReadINTFLGL (PCI_HANDLE hPCI);
BYTE PCI_ReadINTFLGH (PCI_HANDLE hPCI);

// PLX rw Interrupt Control/Status Register
// DWORD PCI_ReadINTCSR (PCI_HANDLE hPCI);
void PCI_WriteINTCSR (PCI_HANDLE hPCI, DWORD data);

// #ifndef NDEBUG
// DWORD PCI_ReadEROMBRD (PCI_HANDLE hPCI);
// void PCI_WriteEROMBRD (PCI_HANDLE hPCI, DWORD data);
// #endif

// 0x13  w  Interrupt Acknowledge
void PCI_WriteINT_ACK (PCI_HANDLE hPCI, BYTE data);

// handle interrupts
/*BOOL PCI_IntIsEnabled (PCI_HANDLE hPCI);
BOOL PCI_IntEnable (PCI_HANDLE hPCI, PCI_INT_HANDLER funcIntHandler);
void PCI_IntDisable (PCI_HANDLE hPCI);*/

// access to PCI configuration registers
// void PCI_WritePCIReg(PCI_HANDLE hPCI, DWORD dwReg, DWORD dwData);
// DWORD PCI_ReadPCIReg(PCI_HANDLE hPCI, DWORD dwReg);

// aus windrvr_int_thread.h
// #define WD_EVENT int
// typedef void (*EVENT_HANDLER)(WD_EVENT *event, void *data);

// Function: PCI_RegisterEvent()
//   Register to receive Plug-and-Play and power notification events according to the given criteria.
// BOOL PCI_RegisterEvent(PCI_HANDLE hPCI, DWORD dwAction, DWORD dwVendorID, DWORD dwDeviceID,
//     WD_PCI_SLOT pciSlot, EVENT_HANDLER funcHandler);

// Function: PCI_UnregisterEvent()
//   Unregister events notification.
// void PCI_UnregisterEvent(PCI_HANDLE hPCI);

// BOOL PCI_IntPriority(PCI_HANDLE hPCI, INT nPriority);

// this string is set to an error message, if one occurs
extern char PCI_ErrorString[];

// fehlernummer
extern int PCI_err;
#define PCI_ERR_NONE	0
#define PCI_ERR_IRQ	1
#define PCI_ERR_ITEMS	2

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------

// strukturen aus pci_lib.c

// typedef struct {
// 	WD_INTERRUPT Int;
// 	HANDLE hThread;
// 	WD_TRANSFER Trans[1];
//     PCI_INT_HANDLER funcIntHandler;
// } PCI_INT_INTERRUPT;
//
// typedef struct {
// 	DWORD index;
// 	DWORD dwMask;
// 	BOOL  fIsMemory;
// 	BOOL  fActive;
// } PCI_ADDR_DESC;
//
// typedef struct PCI_STRUCT {
// 	HANDLE hWD;
//     HANDLE hEvent;
// 	BOOL   fUseInt;
// 	PCI_INT_INTERRUPT Int;
// 	WD_PCI_SLOT pciSlot;
// 	PCI_ADDR_DESC addrDesc[PCI_ITEMS];
// 	WD_CARD_REGISTER cardReg;
// } PCI_STRUCT;

//---------------------------------------------------------------------------

#endif
