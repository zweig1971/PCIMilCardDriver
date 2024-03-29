//////////////////////////////////////////////////////////////////////
// File - mydriver_lib.c
//
// Library for accessing the MYDRIVER card.
// Code was generated by Driver Wizard.
// It accesses the hardware via WinDriver functions.
// 
//////////////////////////////////////////////////////////////////////

#include "mydriver_lib.h"
#include "windrvr_int_thread.h"
#include "mildriverdef.h"
#ifndef __KERNEL__
	#include <stdio.h>
#endif

// this string is set to an error message, if one occurs
CHAR MYDRIVER_ErrorString[1024];

// internal data structures
typedef struct
{
    WD_INTERRUPT Int;
    HANDLE hThread;
    WD_TRANSFER Trans[1];
    MYDRIVER_INT_HANDLER funcIntHandler;
} MYDRIVER_INT_INTERRUPT;

typedef struct
{
    DWORD index;
    DWORD dwMask;
    BOOL  fIsMemory;
    BOOL  fActive;
} MYDRIVER_ADDR_DESC;

typedef struct MYDRIVER_STRUCT
{
    HANDLE hWD;
    BOOL   fUseInt;
    MYDRIVER_INT_INTERRUPT Int;
    WD_PCI_SLOT pciSlot;
    MYDRIVER_ADDR_DESC addrDesc[MYDRIVER_ITEMS];
    WD_CARD_REGISTER cardReg;
} MYDRIVER_STRUCT; 


// internal function used by MYDRIVER_Open()
BOOL MYDRIVER_DetectCardElements(MYDRIVER_HANDLE hMYDRIVER);


DWORD MYDRIVER_CountCards (DWORD dwVendorID, DWORD dwDeviceID,int *status)
{
    WD_VERSION ver;
    WD_PCI_SCAN_CARDS pciScan;
    HANDLE hWD;

    MYDRIVER_RegisterWinDriver();

    MYDRIVER_ErrorString[0] = '\0';
    hWD = WD_Open();

    // check if handle valid & version OK
    if (hWD==INVALID_HANDLE_VALUE) 
    {
        return (InvalidHandleValue);
    }

    BZERO(ver);
    WD_Version(hWD,&ver);
    if (ver.dwVer<WD_VER) 
    {
        WD_Close (hWD);
        return (IncorrectWinDriver);
    }

    BZERO(pciScan);
    pciScan.searchId.dwVendorId = dwVendorID;
    pciScan.searchId.dwDeviceId = dwDeviceID;
    WD_PciScanCards (hWD, &pciScan);
    WD_Close (hWD);

    return pciScan.dwCards;
}

BOOL MYDRIVER_Open (MYDRIVER_HANDLE *phMYDRIVER, DWORD dwVendorID, DWORD dwDeviceID, DWORD nCardNum, DWORD dwOptions, DWORD *UserStatus)
{
    MYDRIVER_HANDLE hMYDRIVER = (MYDRIVER_HANDLE) malloc (sizeof (MYDRIVER_STRUCT));

    WD_VERSION ver;
    WD_PCI_SCAN_CARDS pciScan;
    WD_PCI_CARD_INFO pciCardInfo;

    *phMYDRIVER = NULL;
    MYDRIVER_ErrorString[0] = '\0';
    BZERO(*hMYDRIVER);

    MYDRIVER_RegisterWinDriver();

    hMYDRIVER->cardReg.hCard = 0;
    hMYDRIVER->hWD = WD_Open();

    // check if handle valid & version OK
    if (hMYDRIVER->hWD==INVALID_HANDLE_VALUE)
    {
		*UserStatus = Failed_Open_WinDrv_Device;
        goto Exit;
    }

    BZERO(ver);
    WD_Version(hMYDRIVER->hWD,&ver);
    if (ver.dwVer<WD_VER)
    {
		*UserStatus = IncorrectWinDriver;
        goto Exit;
    }

    BZERO(pciScan);
    pciScan.searchId.dwVendorId = dwVendorID;
    pciScan.searchId.dwDeviceId = dwDeviceID;
    WD_PciScanCards (hMYDRIVER->hWD, &pciScan);
    if (pciScan.dwCards==0) // Found at least one card
    {
        *UserStatus = CountCards_Error;
        goto Exit;
    }
    if (pciScan.dwCards<=nCardNum)
    {
        *UserStatus = CountCards_Error;
        goto Exit;
    }

    BZERO(pciCardInfo);
    pciCardInfo.pciSlot = pciScan.cardSlot[nCardNum];
    WD_PciGetCardInfo (hMYDRIVER->hWD, &pciCardInfo);
    hMYDRIVER->pciSlot = pciCardInfo.pciSlot;
    hMYDRIVER->cardReg.Card = pciCardInfo.Card;

    hMYDRIVER->fUseInt = (dwOptions & MYDRIVER_OPEN_USE_INT) ? TRUE : FALSE;
    if (!hMYDRIVER->fUseInt)
    {
        DWORD i;
        // Remove interrupt item if not needed
        for (i=0; i<hMYDRIVER->cardReg.Card.dwItems; i++)
        {
            WD_ITEMS *pItem = &hMYDRIVER->cardReg.Card.Item[i];
            if (pItem->item==ITEM_INTERRUPT)
                pItem->item = ITEM_NONE;
        }
    }
    else
    {
        DWORD i;
        // make interrupt resource sharable
        for (i=0; i<hMYDRIVER->cardReg.Card.dwItems; i++)
        {
            WD_ITEMS *pItem = &hMYDRIVER->cardReg.Card.Item[i];
            if (pItem->item==ITEM_INTERRUPT)
                pItem->fNotSharable = FALSE;
        }
    }

    hMYDRIVER->cardReg.fCheckLockOnly = FALSE;
    WD_CardRegister (hMYDRIVER->hWD, &hMYDRIVER->cardReg);
    if (hMYDRIVER->cardReg.hCard==0)
    {
		*UserStatus = Open_Failure;
        goto Exit;
    }

    if (!MYDRIVER_DetectCardElements(hMYDRIVER))
    {
		*UserStatus = Open_Failure;
        goto Exit;
    }

    // Open finished OK
    *phMYDRIVER = hMYDRIVER;
    return TRUE;

Exit:
    // Error during Open
    if (hMYDRIVER->cardReg.hCard) 
        WD_CardUnregister(hMYDRIVER->hWD, &hMYDRIVER->cardReg);
    if (hMYDRIVER->hWD!=INVALID_HANDLE_VALUE)
        WD_Close(hMYDRIVER->hWD);
    free (hMYDRIVER);
    return FALSE;
}

void MYDRIVER_Close(MYDRIVER_HANDLE hMYDRIVER)
{
    // disable interrupts
    if (MYDRIVER_IntIsEnabled(hMYDRIVER))
        MYDRIVER_IntDisable(hMYDRIVER);

    // unregister card
    if (hMYDRIVER->cardReg.hCard) 
        WD_CardUnregister(hMYDRIVER->hWD, &hMYDRIVER->cardReg);

    // close WinDriver
    WD_Close(hMYDRIVER->hWD);

    free (hMYDRIVER);
}

void MYDRIVER_RegisterWinDriver()
{
    HANDLE hWD;
    WD_LICENSE lic;

    hWD = WD_Open();
    if (hWD!=INVALID_HANDLE_VALUE)
    {
         strcpy(lic.cLicense, "6a2b4ee4645cbafddf2c727ef9c4748e6a93.GSI");
         WD_License(hWD, &lic);
         WD_Close(hWD);
    }
}

void MYDRIVER_WritePCIReg(MYDRIVER_HANDLE hMYDRIVER, DWORD dwReg, DWORD dwData)
{
    WD_PCI_CONFIG_DUMP pciCnf;

    BZERO(pciCnf);
    pciCnf.pciSlot = hMYDRIVER->pciSlot;
    pciCnf.pBuffer = &dwData;
    pciCnf.dwOffset = dwReg;
    pciCnf.dwBytes = 4;
    pciCnf.fIsRead = FALSE;
    WD_PciConfigDump(hMYDRIVER->hWD,&pciCnf);
}

DWORD MYDRIVER_ReadPCIReg(MYDRIVER_HANDLE hMYDRIVER, DWORD dwReg)
{
    WD_PCI_CONFIG_DUMP pciCnf;
    DWORD dwVal;

    BZERO(pciCnf);
    pciCnf.pciSlot = hMYDRIVER->pciSlot;
    pciCnf.pBuffer = &dwVal;
    pciCnf.dwOffset = dwReg;
    pciCnf.dwBytes = 4;
    pciCnf.fIsRead = TRUE;
    WD_PciConfigDump(hMYDRIVER->hWD,&pciCnf);
    return dwVal;
}

BOOL MYDRIVER_DetectCardElements(MYDRIVER_HANDLE hMYDRIVER)
{
    DWORD i;
    DWORD ad_sp;

    BZERO(hMYDRIVER->Int);
    BZERO(hMYDRIVER->addrDesc);

    for (i=0; i<hMYDRIVER->cardReg.Card.dwItems; i++)
    {
        WD_ITEMS *pItem = &hMYDRIVER->cardReg.Card.Item[i];

        switch (pItem->item)
        {
        case ITEM_MEMORY:
        case ITEM_IO:
            {
                DWORD dwBytes;
                DWORD dwPhysAddr;
                BOOL fIsMemory;
                if (pItem->item==ITEM_MEMORY)
                {
                    dwBytes = pItem->I.Mem.dwBytes;
                    dwPhysAddr = pItem->I.Mem.dwPhysicalAddr;
                    fIsMemory = TRUE;
                }
                else 
                {
                    dwBytes = pItem->I.IO.dwBytes;
                    dwPhysAddr = pItem->I.IO.dwAddr & 0xffff;
                    fIsMemory = FALSE;
                }

                for (ad_sp=0; ad_sp<MYDRIVER_ITEMS; ad_sp++)
                {
                    DWORD dwPCIAddr;
                    DWORD dwPCIReg;

                    if (MYDRIVER_IsAddrSpaceActive(hMYDRIVER, (MYDRIVER_ADDR) ad_sp)) continue;
                    if (ad_sp<MYDRIVER_AD_EPROM) dwPCIReg = PCI_BAR0 + 4*ad_sp;
                    else dwPCIReg = PCI_ERBAR;
                    dwPCIAddr = MYDRIVER_ReadPCIReg(hMYDRIVER, dwPCIReg);
                    if (dwPCIAddr & 1)
                    {
                        if (fIsMemory) continue;
                        dwPCIAddr &= ~0x3;
                    }
                    else
                    {
                        if (!fIsMemory) continue;
                        dwPCIAddr &= ~0xf;
                    }
                    if (dwPCIAddr==dwPhysAddr)
                        break;
                }
                if (ad_sp<MYDRIVER_ITEMS)
                {
                    DWORD j;
                    hMYDRIVER->addrDesc[ad_sp].fActive = TRUE;
                    hMYDRIVER->addrDesc[ad_sp].index = i;
                    hMYDRIVER->addrDesc[ad_sp].fIsMemory = fIsMemory;
                    hMYDRIVER->addrDesc[ad_sp].dwMask = 0;
                    for (j=1; j<dwBytes && j!=0x80000000; j *= 2)
                    {
                        hMYDRIVER->addrDesc[ad_sp].dwMask = 
                            (hMYDRIVER->addrDesc[ad_sp].dwMask << 1) | 1;
                    }
                }
            }
            break;
        case ITEM_INTERRUPT:
            if (hMYDRIVER->Int.Int.hInterrupt) return FALSE;
            hMYDRIVER->Int.Int.hInterrupt = pItem->I.Int.hInterrupt;
            break;
        }
    }

    // check that all the items needed were found
    // check if interrupt found
    if (hMYDRIVER->fUseInt && !hMYDRIVER->Int.Int.hInterrupt) 
    {
        return FALSE;
    }

    // check that at least one memory space was found
    for (i = 0; i<MYDRIVER_ITEMS; i++)
        if (MYDRIVER_IsAddrSpaceActive(hMYDRIVER, (MYDRIVER_ADDR) i)) break;
    if (i==MYDRIVER_ITEMS) return FALSE;

    return TRUE;
}

BOOL MYDRIVER_IsAddrSpaceActive(MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace)
{
    return hMYDRIVER->addrDesc[addrSpace].fActive;
}

void MYDRIVER_GetPciSlot(MYDRIVER_HANDLE hMYDRIVER, WD_PCI_SLOT *pPciSlot)
{
    memcpy((PVOID)pPciSlot, &(hMYDRIVER->pciSlot), sizeof(WD_PCI_SLOT));
}

// General read/write function
void MYDRIVER_ReadWriteBlock(MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset, BOOL fRead, PVOID buf, DWORD dwBytes, MYDRIVER_MODE mode)
{
    WD_TRANSFER trans;
    BOOL fMem = hMYDRIVER->addrDesc[addrSpace].fIsMemory;
    // safty check: is the address range active
    if (!MYDRIVER_IsAddrSpaceActive(hMYDRIVER, addrSpace)) return;
    BZERO(trans);
    if (fRead)
    {
        if (mode==MYDRIVER_MODE_BYTE) trans.cmdTrans = fMem ? RM_SBYTE : RP_SBYTE;
        else if (mode==MYDRIVER_MODE_WORD) trans.cmdTrans = fMem ? RM_SWORD : RP_SWORD;
        else if (mode==MYDRIVER_MODE_DWORD) trans.cmdTrans = fMem ? RM_SDWORD : RP_SDWORD;
    }
    else
    {
        if (mode==MYDRIVER_MODE_BYTE) trans.cmdTrans = fMem ? WM_SBYTE : WP_SBYTE;
        else if (mode==MYDRIVER_MODE_WORD) trans.cmdTrans = fMem ? WM_SWORD : WP_SWORD;
        else if (mode==MYDRIVER_MODE_DWORD) trans.cmdTrans = fMem ? WM_SDWORD : WP_SDWORD;
    }
    if (fMem)
        trans.dwPort = hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwTransAddr;
    else trans.dwPort = hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.IO.dwAddr;
    trans.dwPort += dwOffset;

    trans.fAutoinc = TRUE;
    trans.dwBytes = dwBytes;
    trans.dwOptions = 0;
    trans.Data.pBuffer = buf;
    WD_Transfer (hMYDRIVER->hWD, &trans);
}

BYTE MYDRIVER_ReadByte (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset)
{
    BYTE data;
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        PBYTE pData = (PBYTE) (hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else MYDRIVER_ReadWriteBlock( hMYDRIVER, addrSpace, dwOffset, TRUE, &data, sizeof (BYTE), MYDRIVER_MODE_BYTE);
    return data;
}

WORD MYDRIVER_ReadWord (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset)
{
    WORD data;
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        PWORD pData = (PWORD) (hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else MYDRIVER_ReadWriteBlock( hMYDRIVER, addrSpace, dwOffset, TRUE, &data, sizeof (WORD), MYDRIVER_MODE_WORD);
    return data;
}

DWORD MYDRIVER_ReadDword (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset)
{
    DWORD data;
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        PDWORD pData = (PDWORD) (hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else MYDRIVER_ReadWriteBlock( hMYDRIVER, addrSpace, dwOffset, TRUE, &data, sizeof (DWORD), MYDRIVER_MODE_DWORD);
    return data;
}

void MYDRIVER_WriteByte (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset, BYTE data)
{
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        PBYTE pData = (PBYTE) (hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else MYDRIVER_ReadWriteBlock( hMYDRIVER, addrSpace, dwOffset, FALSE, &data, sizeof (BYTE), MYDRIVER_MODE_BYTE);
}

void MYDRIVER_WriteWord (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset, WORD data)
{
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        PWORD pData = (PWORD) (hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else MYDRIVER_ReadWriteBlock( hMYDRIVER, addrSpace, dwOffset, FALSE, &data, sizeof (WORD), MYDRIVER_MODE_WORD);
}

void MYDRIVER_WriteDword (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_ADDR addrSpace, DWORD dwOffset, DWORD data)
{
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        PDWORD pData = (PDWORD) (hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else MYDRIVER_ReadWriteBlock( hMYDRIVER, addrSpace, dwOffset, FALSE, &data, sizeof (DWORD), MYDRIVER_MODE_DWORD);
}

void MYDRIVER_WriteMil_cmd (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Mil_cmd_SPACE, MYDRIVER_Mil_cmd_OFFSET, data);
}

void MYDRIVER_WriteMil_write (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Mil_write_SPACE, MYDRIVER_Mil_write_OFFSET, data);
}

void MYDRIVER_WriteReset_Card (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Reset_Card_SPACE, MYDRIVER_Reset_Card_OFFSET, data);
}

DWORD MYDRIVER_ReadMil_read (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Mil_read_SPACE, MYDRIVER_Mil_read_OFFSET);
}

void MYDRIVER_WriteSet_Timer2_10us (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Set_Timer2_10us_SPACE, MYDRIVER_Set_Timer2_10us_OFFSET, data);
}

void MYDRIVER_WriteSet_Timer1 (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Set_Timer1_SPACE, MYDRIVER_Set_Timer1_OFFSET, data);
}

void MYDRIVER_WriteSet_Timer2_1ms (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Set_Timer2_1ms_SPACE, MYDRIVER_Set_Timer2_1ms_OFFSET, data);
}

DWORD MYDRIVER_Read_C_Filter_ADR (MYDRIVER_HANDLE hMYDRIVER, DWORD OffsetValue)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_C_Filter_ADR_SPACE,(MYDRIVER_C_Filter_ADR_OFFSET+OffsetValue));
}

void MYDRIVER_Write_C_Filter_ADR (MYDRIVER_HANDLE hMYDRIVER, DWORD OffsetValue, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_C_Filter_ADR_SPACE,(MYDRIVER_C_Filter_ADR_OFFSET+OffsetValue), data);
}

DWORD MYDRIVER_ReadMil_Status (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Mil_Status_SPACE, MYDRIVER_Mil_Status_OFFSET);
}

void MYDRIVER_WriteControlRegister1 (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_ControlRegister1_SPACE, MYDRIVER_ControlRegister1_OFFSET, data);
}

DWORD MYDRIVER_ReadControlRegister1 (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_ControlRegister1_SPACE, MYDRIVER_ControlRegister1_OFFSET);
}

void MYDRIVER_WriteSlave_Mil_interlog (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Slave_Mil_interlog_SPACE, MYDRIVER_Slave_Mil_interlog_OFFSET, data);
}

DWORD MYDRIVER_ReadEV_Time_Rd (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_EV_Time_Rd_SPACE, MYDRIVER_EV_Time_Rd_OFFSET);
}

DWORD MYDRIVER_ReadEvent_Read (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Event_Read_SPACE, MYDRIVER_Event_Read_OFFSET);
}

DWORD MYDRIVER_ReadIntrrrupt_Mask (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Intrrrupt_Mask_SPACE, MYDRIVER_Intrrrupt_Mask_OFFSET);
}

void MYDRIVER_WriteIntrrrupt_Mask (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Intrrrupt_Mask_SPACE, MYDRIVER_Intrrrupt_Mask_OFFSET, data);
}

DWORD MYDRIVER_ReadAux_IO (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Aux_IO_SPACE, MYDRIVER_Aux_IO_OFFSET);
}

void MYDRIVER_WriteAux_IO (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Aux_IO_SPACE, MYDRIVER_Aux_IO_OFFSET, data);
}

BOOL MYDRIVER_IntIsEnabled (MYDRIVER_HANDLE hMYDRIVER)
{
    if (!hMYDRIVER->fUseInt) return FALSE;
    if (!hMYDRIVER->Int.hThread) return FALSE;
    return TRUE;
}


void MYDRIVER_IntHandler (PVOID pData, PVOID pUserDat)
{
    MYDRIVER_HANDLE hMYDRIVER = (MYDRIVER_HANDLE) pData;
    MYDRIVER_INT_RESULT intResult;
    intResult.dwCounter = hMYDRIVER->Int.Int.dwCounter;
    intResult.dwLost = hMYDRIVER->Int.Int.dwLost;
    intResult.fStopped = hMYDRIVER->Int.Int.fStopped;
    hMYDRIVER->Int.funcIntHandler(pUserDat);
}

BOOL MYDRIVER_IntEnable (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_INT_HANDLER funcIntHandler, PVOID pUserDat, WORD SET_MY_PRIORITY)
{
	HANDLE my_hThread;
	INT_THREAD_DATA *my_pThread;
    MYDRIVER_ADDR addrSpace;

	WORD MY_PRIORITY = 0;

//	DWORD FilterValue;
//	BYTE CntrlReg;

    if (!hMYDRIVER->fUseInt) return FALSE;
    // check if interrupt is already enabled
    if (hMYDRIVER->Int.hThread) return FALSE;

    BZERO(hMYDRIVER->Int.Trans);
    // This is a sample of handling interrupts:
    // One transfer command is issued to CANCEL the source of the interrupt,
    // otherwise, the PC will hang when an interrupt occurs!
    // You will need to modify this code to fit your specific hardware.
    addrSpace = MYDRIVER_INTCSR_SPACE; // put the address space of the register here
    if (hMYDRIVER->addrDesc[addrSpace].fIsMemory)
    {
        hMYDRIVER->Int.Trans[0].dwPort = hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.Mem.dwTransAddr;
        hMYDRIVER->Int.Trans[0].cmdTrans = WM_DWORD;
    }
    else
    {
        hMYDRIVER->Int.Trans[0].dwPort = hMYDRIVER->cardReg.Card.Item[hMYDRIVER->addrDesc[addrSpace].index].I.IO.dwAddr;
        hMYDRIVER->Int.Trans[0].cmdTrans = WP_DWORD;
    }
	

	// Kommt ein Interrupt der Karte an, so wird die Funktion
	// MYDRIVER_INTCSR aufgerufen. Diese Funktion schreibt in ein
	// Register des PLX PCI9050-1 eine 0x0 rein. d.h. alle Interrupts
	// abschalten
    hMYDRIVER->Int.Trans[0].dwPort += MYDRIVER_INTCSR_OFFSET; // put the offset of the register from the beginning of the address space here
    hMYDRIVER->Int.Trans[0].Data.Dword = 0x0;
    hMYDRIVER->Int.Int.dwCmds = 1;
    hMYDRIVER->Int.Int.Cmd = hMYDRIVER->Int.Trans;
    hMYDRIVER->Int.Int.dwOptions |= INTERRUPT_CMD_COPY;



    // this calls WD_IntEnable() and creates an interrupt handler thread
    hMYDRIVER->Int.funcIntHandler = funcIntHandler;
    if (!InterruptThreadEnable(&hMYDRIVER->Int.hThread, hMYDRIVER->hWD, &hMYDRIVER->Int.Int, MYDRIVER_IntHandler, (PVOID) hMYDRIVER, pUserDat, SET_MY_PRIORITY))
	{
		return FALSE;
	}

	// pruefen ob setzen der priority geklappt hat
	my_hThread = hMYDRIVER->Int.hThread;
	my_pThread = (INT_THREAD_DATA *) my_hThread;
	MY_PRIORITY = GetThreadPriority(my_pThread->hThread);
	if(MY_PRIORITY != SET_MY_PRIORITY)
	{
		return FALSE;
	}
	return TRUE;
}

void MYDRIVER_IntDisable (MYDRIVER_HANDLE hMYDRIVER)
{
    if (!hMYDRIVER->fUseInt) return;
    if (!hMYDRIVER->Int.hThread) return;

    // this calls WD_IntDisable()
    InterruptThreadDisable(hMYDRIVER->Int.hThread);

    hMYDRIVER->Int.hThread = NULL;
}

DWORD MYDRIVER_ReadINTCSR (MYDRIVER_HANDLE hMYDRIVER)
{
    return MYDRIVER_ReadDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_INTCSR_SPACE, MYDRIVER_INTCSR_OFFSET);
}

void MYDRIVER_WriteINTCSR (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_INTCSR_SPACE, MYDRIVER_INTCSR_OFFSET, data);
}


void MYDRIVER_WritePuls1_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls1_Start_Soft_SPACE, MYDRIVER_Puls1_Start_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls1_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls1_Stop_Soft_SPACE, MYDRIVER_Puls1_Stop_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls2_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls2_Start_Soft_SPACE, MYDRIVER_Puls2_Start_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls2_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls2_Stop_Soft_SPACE, MYDRIVER_Puls2_Stop_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls3_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls3_Start_Soft_SPACE, MYDRIVER_Puls3_Start_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls3_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls3_Stop_Soft_SPACE, MYDRIVER_Puls3_Stop_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls4_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls4_Start_Soft_SPACE, MYDRIVER_Puls4_Start_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls4_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls4_Stop_Soft_SPACE, MYDRIVER_Puls4_Stop_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls5_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls5_Start_Soft_SPACE, MYDRIVER_Puls5_Start_Soft_OFFSET, data);
}

void MYDRIVER_WritePuls5_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data)
{
    MYDRIVER_WriteDword(hMYDRIVER, (MYDRIVER_ADDR) MYDRIVER_Puls5_Stop_Soft_SPACE, MYDRIVER_Puls5_Stop_Soft_OFFSET, data);
}