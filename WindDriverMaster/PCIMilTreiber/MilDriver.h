//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// MilDriver
//
// Autor           : Zweig,Marcus
// Version         : 1.1
// letzte Änderung : 27.10.06
//------------------------------

#include "MilDriverdef.h"
#include "mydriver_lib.h"
#include "windrvr.h"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>


using namespace std;

int CountPCCards (int &findCards);
string PrintErrorMessage(unsigned int status);
bool SetPriority(int PriorityLevel);

class MilDriver
{
private:
	
// In diese Structur werden die ermittelten daten der Karte 
// abgelegt, auf die dann die dienstleistungtsfunctionen
// zugreifen (muessen)
	struct CardData
	{		
		int VendorID;
		int DeviceID;
		int CardNumber;
		MYDRIVER_HANDLE myHandle;
	};

	CardData CardInfo;

	int  OpenBoard		(bool UseInterrupt, int CardNumber);
	void CloseBoard		(void);

	void SendCommand	(WORD data);
	void WriteMilBus	(WORD data);
	void ReadMilBus		(WORD &data);
	void ReadEventFiFo  (WORD &data);

	void SetTimer1		(DWORD time);
	void SetTimer2_10us	(DWORD time);
	void SetTimer2_1ms	(DWORD time);
	void ReadEVTimer	(DWORD &time);

	void MilBusStatus	(DWORD &MilStatus);
	void ResetCard		(void);

	void SetControlRegister1(WORD data);
	void ReadControlRegister1(WORD &data);

	void ReadEventFilter (DWORD &FilterValue, WORD OffsetValue);
	void SetEventFilter  (WORD data, WORD OffsetValue);

	void ReadIntMask(DWORD &data);
	void WriteIntMask(DWORD data);
	
	int IntEnable (MYDRIVER_INT_HANDLER funcIntHandler, void *pUserDat, WORD SET_MY_PRIORITY);
	void IntDisable (void);

	int TestCard		(void);
	bool TestStatus		(DWORD StatusBit);

	DWORD Read_INTCSR (void);
	void Write_INTCSR (DWORD data);

	bool WaitMilBusWrite(void);
	bool WaitMilBusRead (void);



public:

	MilDriver(void);
	~MilDriver(void);

	void wait(int time);

	int ConnectMilBus(DWORD NumberOfCard);

	bool IFKAlive(BYTE IFKAdresse, BYTE &ReturnIFKNR, DWORD &UserStatus);
	void ResetPCICard(DWORD &UserStatus);

	void WaitOfTimer2 (WORD time, _TimerBase Timerbase, DWORD &UserStatus);
	void SetTimer2	  (WORD time, _TimerBase Timerbase, DWORD &UserStatus);	

	void ReadStatusReg	 (DWORD &myStatusReg, DWORD &UserStatus);
	void SetCntrlReg1    (BYTE SetValue , DWORD &UserStatus);
	void ReadCntrlReg1   (BYTE &CntrlReg, DWORD &UserStatus);
	bool TestCardStatus  (DWORD StatusBit, DWORD &UserStatus);	

	void WriteDataToIFK	 (BYTE FunktionCode, BYTE IFKAdresse, WORD data, DWORD &UserStatus);
	void ReadDataFromIFK (BYTE FunktionCode, BYTE IFKAdresse, WORD &data, DWORD &UserStatus);
	void SendFknCodeToIFK(BYTE FunktionCode, BYTE IFKAdresse, DWORD &UserStatus);
	
	void ReadDataFromFiFo(WORD &data, DWORD &UserStatus, bool &Fifo_not_empty);
	void ReadData        (WORD &data, DWORD &UserStatus);
	void WriteData		 (WORD data,  DWORD &UserStatus);
	void WriteCMD		 (WORD cmd,   DWORD &UserStatus);

	void ReadEVFromFiFo  (WORD &data, DWORD &UserStatus ,bool &EVFifoNotEmpty);
	void SetEVFilter	 (WORD EventCodeNr, BYTE SetEV, DWORD &UserStatus);

	void ReadFromEVFilter(WORD EventCodeNr, BYTE &EVValue,DWORD &UserStatus);
	void ReadEventTimer  (DWORD &Time, DWORD &UserStatus);
	
	void ClearEvFilter(DWORD &UserStatus);
	void ClearEvFiFio (DWORD &UserStatus);

	void ReadInterruptMask(DWORD &data, DWORD &UserStatus);
	void WriteInterruptMask(DWORD data, DWORD &UserStatus);

	void InterruptEnable(DWORD &UserStatus, MYDRIVER_INT_HANDLER funcIntHandler, PVOID pUserDat, WORD SET_MY_PRIORITY);
	void InterruptDisable(DWORD &UserStatus);
	void InterPLXEnable(DWORD &UserStatus);
	void InterruptWait(DWORD &IntAktiv, DWORD &UserStatus);
	void StopInterruptWait(DWORD &UserStatus);
	void Int_Aktiv(DWORD &IntAktiv, DWORD &UserStatus);

	void Puls1_Start_Soft (DWORD &UserStatus);
	void Puls1_Stop_Soft  (DWORD &UserStatus);
	void Puls2_Start_Soft (DWORD &UserStatus);
	void Puls2_Stop_Soft  (DWORD &UserStatus);
	void Puls3_Start_Soft (DWORD &UserStatus);
	void Puls3_Stop_Soft  (DWORD &UserStatus);
	void Puls4_Start_Soft (DWORD &UserStatus);
	void Puls4_Stop_Soft  (DWORD &UserStatus);
	void Puls5_Start_Soft (DWORD &UserStatus);
	void Puls5_Stop_Soft  (DWORD &UserStatus); 
};