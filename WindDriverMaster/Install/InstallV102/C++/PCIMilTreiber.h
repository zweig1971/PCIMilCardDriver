//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// PCIMilTreiber.h
//
// Autor           : Zweig,Marcus
// Version         : 0.98
// letzte Änderung : 22.08.02
//------------------------------

enum {DataSend,CommandSend};
enum _TimerBase{Timerbase_10us, Timerbase_1ms};

// Bitmasken für status_1
//------------------------
#define Interlock			0x1		//Bit 0
#define Dta_Rdy				0x2
#define Dta_Req				0x4
#define Trm_Rdy				0x8
#define Mil_Rcv_Rdy			0x10
#define Mil_Fifo_Full		0x20
#define Mil_Fifo_On			0x40
#define Cmd_Rcv				0x80
#define Timeout_1			0x100
#define Timeout_2			0x200
#define Debounce_On			0x400
#define Frei_D11			0x800
#define Evt_Fifo_Not_Empty	0x1000
#define Evt_Fifo_Full		0x2000
#define EvFilter_On			0x4000
#define Master_Slave		0x8000	//Bit 15


// Bitmasken fuer das Control Register 1
// (Ctrl_Reg1, nur setzen)
// --------------------------------------
#define Ctrl_EvFilter_On	 0x1
#define Ctrl_Debounce_On	 0x2
#define Ctrl_Mil_Fifo_On	 0x4
#define Ctrl_En_Intr1		 0x8 
#define Ctrl_En_Intr2		 0x10 
#define Ctrl_EvFilter_8_12   0x20
#define Ctrl_Puls1_Rahmen_On 0x40
#define Ctrl_Puls2_Rahmen_On 0x80


// Bitmasken fuer den das EventFilter
//------------------------------------
#define EvFilter_EventOn        0x1
#define EvFilter_EventTimerRun  0x2 
#define EvFilter_Start_Puls1	0x4
#define EvFilter_Stop_Puls1		0x8
#define EvFilter_Start_Puls2	0x10
#define EvFilter_Stop_Puls2		0x20	


// DatenTypen
//-----------------
typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned long DWORD;

// Fehlermeldungen 
//----------------

#define StatusOK					 0x0
#define VendorID_Error				 0x1
#define DeviceID_Error				 0x2
#define CountCards_Error			 0x4
#define Open_Failure				 0x8
#define IncorrectWinDriver			 0x10
#define InvalidHandleValue			 0x20
#define Failed_Open_WinDrv_Device	 0x40
#define MilBus_Busy					 0x80
#define TestTimerError				 0x100
#define Fifo_Full					 0x200
#define CardNrError					 0x400
#define NoPCICartOpen				 0x800
#define NoPCMil						 0x1000
#define ReadMilBusTimeOut			 0x2000
#define WriteMilBusTimeOut			 0x4000
#define FifoNotEmpty				 0x8000
#define FifoNotCleared				 0x10000
#define NoDataToRead				 0x20000
#define EventNrError				 0x40000
#define IFKNrError					 0x80000
#define FailedEnableIntr			 0x100000
#define IntMaskError			     0x200000
#define GeneralError                 0x400000


enum {EventOn, EventOff, EventTimerClear, EventTimerRun, EventClear, Puls1_On, 
      Puls1_Off, Puls2_On, Puls2_Off, Cntrl_EvFilter_On,
	  Cntrl_EvFilter_Off, Cntrl_Debounce_On, Cntrl_Debounce_Off,
	  Cntrl_Mil_FiFo_On, Cntrl_Mil_FiFo_Off, Cntrl_En_Intr1, Cntrl_Dis_Intr1,
	  Cntrl_En_Intr2, Cntrl_Dis_Intr2, Cntrl_EvFilter_12_Bit, Cntrl_EvFilter_8_Bit,
	  Cntrl_Puls1_Rahmen_On, Cntrl_Puls1_Rahmen_Off, Cntrl_Puls2_Rahmen_On, 
	  Cntrl_Puls2_Rahmen_Off, PulsBuchse_1, PulsBuchse_2, Puls, RahmenPuls};


extern "C" void MILDRIVER_Version_0p094_160702(void);
extern "C" void MILDRIVER_VersionNr(char* version);
extern "C" int PCI_PCIcardCount(void);
extern "C" int PCI_DriverOpen(int CardNr);
extern "C" int PCI_IfkOnline(int CardNr, BYTE IFCNr,BYTE &ReturnIFKNR, DWORD &ErrorStatus);
extern "C" int PCI_IfkRead(int CardNr, BYTE IFKAdress, BYTE IFKFunktionCode, WORD &data, DWORD &ErrorStatus);
extern "C" int PCI_IfkWrite(int CardNr, BYTE IFKAdress, BYTE IFKFunktionCode, WORD data, DWORD &ErrorStatus);
extern "C" int PCI_PCIcardReset(int CardNr, DWORD &ErrorStatus);
extern "C" int PCI_FctCodeSnd(int CardNr, BYTE IFKAdress, BYTE FunktionCode, DWORD &ErrorStatus);
extern "C" int PCI_ErrorString (int status, char* error_string);
extern "C" int PCI_TimerWait (int CardNr, WORD Time, BYTE TimeBase, DWORD &ErrorStatus); 
extern "C" int PCI_TimerSet (int CardNr, WORD Time, BYTE TimeBase, DWORD &ErrorStatus);
extern "C" int PCI_StatusRegRead (int CardNr, WORD &StatusReg, DWORD &ErrorStatus);
extern "C" int PCI_MilBusWrite (int CardNr, WORD data, DWORD &ErrorStatus);
extern "C" int PCI_DriverClose(int CardNr);
extern "C" int PCI_IntMaskRead (int CardNr, DWORD &data, DWORD &ErrorStatus);
extern "C" int PCI_IntMaskWrite(int CardNr, DWORD data , DWORD &ErrorStatus);
extern "C" int PCI_IntEnable(int CardNr, DWORD &ErrorStatus);
extern "C" int PCI_IntDisable(int CardNr, DWORD &ErrorStatus);
extern "C" int PCI_IntWait(int CardNr, DWORD &IntAktiv, DWORD &ErrorStatus);
extern "C" int PCI_IntWaitStop(int CardNr, DWORD &ErrorStatus);
extern "C" int PCI_CntrlRegSet(int CardNr, BYTE SetValue, DWORD &ErrorStatus);
extern "C" int PCI_CntrlRegRead(int CardNr, BYTE &CntrlRegValue, DWORD &ErrorStatus);
extern "C" int PCI_EvFilterSet(int CardNr, WORD EventCodeNr, BYTE SetEV, DWORD &ErrorStatus); 
extern "C" int PCI_EvFiFoRead(int CardNr, WORD &EventCodeNr, bool &EVFifoNotEmpty, DWORD &ErrorStatus);
extern "C" int PCI_EvFilterRead(int CardNr, WORD EventCodeNr, BYTE &FilterValue, DWORD &ErrorStatus);
extern "C" bool PCI_StatusTest(int CardNr, WORD StatusBit, DWORD &ErrorStatus);
extern "C" int PCI_MilBusRead(int CardNr, WORD &data, bool &Fifo_not_empty, DWORD &ErrorStatus);
extern "C" int PCI_EvTimerRead(int CardNr, DWORD &Time ,DWORD &ErrorStatus);
extern "C" int PCI_PulsOutSet(int CardNr, WORD PulsBuchse, WORD EventOn, WORD EventOff, WORD PulsRahmen, bool OnOff, DWORD &ErrorStatus);
extern "C" int PCI_EvFilterClear(int CardNr, DWORD &ErrorStatus);
extern "C" int PCI_EvFiFioClear(int CardNr, DWORD &ErrorStatus);