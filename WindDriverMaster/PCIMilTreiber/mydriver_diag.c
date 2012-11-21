////////////////////////////////////////////////////////////////
// File - mydriver_diag.c
//
// This is a diagnostics application for accessing the MYDRIVER card.
// Code was generated by Driver Wizard.
// It accesses the hardware via WinDriver functions.
// 
////////////////////////////////////////////////////////////////

#include "mydriver_lib.h"
#include "pci_diag_lib.h"
#include "mildriverdef.h"
#include <stdio.h>

// input of command from user
static char line[256];

char *MYDRIVER_GetAddrSpaceName(MYDRIVER_ADDR addrSpace)
{
    return 
        addrSpace==MYDRIVER_AD_BAR0 ? "Addr Space BAR0" :
        addrSpace==MYDRIVER_AD_BAR1 ? "Addr Space BAR1" :
        addrSpace==MYDRIVER_AD_BAR2 ? "Addr Space BAR2" :
        addrSpace==MYDRIVER_AD_BAR3 ? "Addr Space BAR3" :
        addrSpace==MYDRIVER_AD_BAR4 ? "Addr Space BAR4" :
        addrSpace==MYDRIVER_AD_BAR5 ? "Addr Space BAR5" :
        addrSpace==MYDRIVER_AD_EPROM ? "EEPROM Addr Space" :
        "Invalid";
}


MYDRIVER_HANDLE MYDRIVER_LocateAndOpenBoard (DWORD dwVendorID, DWORD dwDeviceID, BOOL fUseInt, int *status, int cards)
{
    MYDRIVER_HANDLE hMYDRIVER = NULL;
	DWORD UserStatus = 0;

    if (dwVendorID == 0)
    {
		*status = VendorID_Error;
		return(0);
	}
	else if (dwDeviceID == 0)
	{
		*status = DeviceID_Error;
		return(0);
	}


    if (cards==0)
    {
		*status = CountCards_Error;
		return(0);
    }

    if (MYDRIVER_Open (&hMYDRIVER, dwVendorID, dwDeviceID, cards - 1, fUseInt ? MYDRIVER_OPEN_USE_INT : 0, &UserStatus))
        *status = StatusOK;
    else
		*status = UserStatus;
   
	return hMYDRIVER;
}

/*
// Diese Interrupt Handler Routine wird aufgerufen,
// wenn ein Interrupt erkannt worden ist. 
void MYDRIVER_IntHandlerRoutine(DWORD MainAdress, DWORD InterruptID, CALLBACK_INT_HANDLER funcCallHandler)
{
	if(funcCallHandler == NULL)
	{
		PostThreadMessage(MainAdress, InterruptID,0,0);
	}
	else
	{
		funcCallHandler();
	}
}*/