#include "PCIMILTreiber.h"

void main()
{
	DWORD status = 0;
	bool statusb;
	DWORD StatusReg = 0;
	DWORD ErrorStatus = 0;

	status = PCI_DriverOpen(2);

	while(1)
	{
		PCI_StatusRegRead(2, StatusReg, ErrorStatus);
		statusb = PCI_StatusTest (2, Puls_In1, ErrorStatus);
	}

	PCI_DriverClose(2);
}