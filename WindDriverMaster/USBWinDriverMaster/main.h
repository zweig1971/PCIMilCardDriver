//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// USB-MilDriver
//
// Autor           : Zweig,Marcus
// Version         : 0.01
// letzte Änderung : 09.03.05
//------------------------------

#include "mildriver.h"
#include <windows.h>

#define maxPCIMilKarten 5
#define EXPORT_TYPE __declspec(dllexport)

#ifndef PCIMilTreiber_Slave
extern "C" EXPORT_TYPE void MILDRIVER_Version_0p109_130204(void);
#endif

#ifdef PCIMilTreiber_Slave
extern "C" EXPORT_TYPE void MILDRIVER_Version_0p04S_130204(void);
#endif

extern "C" EXPORT_TYPE void MILDRIVER_VersionNr(BYTE* version);

extern "C" EXPORT_TYPE DWORD PCI_PCIcardCount(void);
extern "C" EXPORT_TYPE DWORD PCI_DriverOpen(BYTE CardNr);

#ifndef PCIMilTreiber_Slave
extern "C" EXPORT_TYPE DWORD PCI_IfkOnline(BYTE CardNr, BYTE IFCNr,BYTE &ReturnIFKNR, DWORD &ErrorStatus);
#endif

#ifndef PCIMilTreiber_Slave
extern "C" EXPORT_TYPE DWORD PCI_IfkRead(BYTE CardNr, BYTE IFKAdress, BYTE IFKFunktionCode, WORD &data, DWORD &ErrorStatus);
#endif

#ifndef PCIMilTreiber_Slave
extern "C" EXPORT_TYPE DWORD PCI_IfkWrite(BYTE CardNr, BYTE IFKAdress, BYTE IFKFunktionCode, WORD data, DWORD &ErrorStatus);
#endif

#ifndef PCIMilTreiber_Slave
extern "C" EXPORT_TYPE DWORD PCI_FctCodeSnd(BYTE CardNr, BYTE IFKAdress, BYTE FunktionCode, DWORD &ErrorStatus);
#endif

extern "C" EXPORT_TYPE DWORD PCI_PCIcardReset(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_ErrorString (DWORD status, BYTE* error_string);
extern "C" EXPORT_TYPE DWORD PCI_TimerWait (BYTE CardNr, WORD Time, BYTE TimeBase, DWORD &ErrorStatus); 
extern "C" EXPORT_TYPE DWORD PCI_TimerSet (BYTE CardNr, WORD Time, BYTE TimeBase, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_StatusRegRead (BYTE CardNr, DWORD &StatusReg, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_MilBusWrite (BYTE CardNr, WORD data, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_MilBusCMDWrite (BYTE CardNr, WORD cmd, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_DriverClose(BYTE CardNr);
extern "C" EXPORT_TYPE DWORD PCI_IntMaskRead (BYTE CardNr, DWORD &data, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntMaskWrite(BYTE CardNr, DWORD data , DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntEnable(BYTE CardNr, DWORD funcIntHandler, PVOID pUserDat, WORD Priority, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntDisable(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntWait(BYTE CardNr, DWORD &IntAktiv, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntAktiv(BYTE CardNr, DWORD &IntAktiv, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntWaitStop(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_CntrlRegSet(BYTE CardNr, BYTE SetValue, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_CntrlRegRead(BYTE CardNr, BYTE &CntrlRegValue, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_EvFilterSet(BYTE CardNr, WORD EventCodeNr, BYTE SetEV, DWORD &ErrorStatus); 
extern "C" EXPORT_TYPE DWORD PCI_EvFiFoRead(BYTE CardNr, WORD &EventCodeNr, bool &EVFifoNotEmpty, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_EvFilterRead(BYTE CardNr, WORD EventCodeNr, BYTE &FilterValue, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_IntNextEnable(BYTE CardNr, DWORD &UserStatus);
extern "C" EXPORT_TYPE bool PCI_StatusTest(BYTE CardNr, DWORD StatusBit, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_MilBusRead(BYTE CardNr, WORD &data, bool &Fifo_not_empty, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_EvTimerRead(BYTE CardNr, DWORD &Time ,DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_PulsOutSet(BYTE CardNr, WORD PulsBuchse, WORD EventOn, WORD EventOff, WORD PulsRahmen, bool OnOff, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls1Start(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls1Stop (BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls2Start(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls2Stop (BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls3Start(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls3Stop (BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls4Start(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls4Stop (BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls5Start(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_SoftPuls5Stop (BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_EvFilterClear(BYTE CardNr, DWORD &ErrorStatus);
extern "C" EXPORT_TYPE DWORD PCI_EvFiFioClear(BYTE CardNr, DWORD &ErrorStatus);
