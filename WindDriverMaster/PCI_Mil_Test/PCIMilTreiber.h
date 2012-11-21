//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// PCIMilTreiber.h
//
// Autor           : Zweig,Marcus
// Version         : 1.09
// letzte Änderung : 13.02.04
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
#define Master_Slave		0x8000	
#define Puls_In1			0x10000
#define Puls_In2			0x20000 
#define Puls_In3			0x40000 //Bit18


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
#define Ctrl_Puls3_Rahmen_On 0x100
#define Ctrl_Puls4_Rahmen_On 0x200
#define Ctrl_Puls5_Rahmen_On 0x400


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
typedef unsigned char _BYTE;
typedef unsigned short int _WORD;
typedef unsigned int _DWORD;

// Interrupt aufruf- funktionen
typedef void (*MYDRIVER_INT_HANDLER)(void *p);

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
#define MasterSlaveError			 0x800000
#define UnrequestedReceiv			 0x1000000   

enum {EventOn, EventOff, EventTimerClear, EventTimerRun, EventClear, Puls1_On, 
      Puls1_Off, Puls2_On, Puls2_Off, Cntrl_EvFilter_On,
	  Cntrl_EvFilter_Off, Cntrl_Debounce_On, Cntrl_Debounce_Off,
	  Cntrl_Mil_FiFo_On, Cntrl_Mil_FiFo_Off, Cntrl_En_Intr1, Cntrl_Dis_Intr1,
	  Cntrl_En_Intr2, Cntrl_Dis_Intr2, Cntrl_EvFilter_12_Bit, Cntrl_EvFilter_8_Bit,
	  Cntrl_Puls1_Rahmen_On, Cntrl_Puls1_Rahmen_Off, Cntrl_Puls2_Rahmen_On, 
	  Cntrl_Puls2_Rahmen_Off, Cntrl_Puls3_Rahmen_On, Cntrl_Puls3_Rahmen_Off,
	  Cntrl_Puls4_Rahmen_On, Cntrl_Puls4_Rahmen_Off, Cntrl_Puls5_Rahmen_On,
	  Cntrl_Puls5_Rahmen_Off, PulsBuchse_1, PulsBuchse_2, Puls, RahmenPuls,
	  IntSetPriorityNormal, IntSetPriorityHigh, IntSetPriorityCritical};


extern "C" void MILDRIVER_VersionNr(_BYTE* version);
extern "C" _DWORD PCI_PCIcardCount(void);
extern "C" _DWORD PCI_DriverOpen  (_BYTE CardNr);
extern "C" _DWORD PCI_IfkOnline   (_BYTE CardNr, _BYTE IFCNr,_BYTE &ReturnIFKNR, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IfkRead     (_BYTE CardNr, _BYTE IFKAdress, _BYTE IFKFunktionCode, _WORD &data, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IfkWrite    (_BYTE CardNr, _BYTE IFKAdress, _BYTE IFKFunktionCode, _WORD data, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_PCIcardReset(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_FctCodeSnd  (_BYTE CardNr, _BYTE IFKAdress, _BYTE FunktionCode, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_ErrorString (_DWORD status, _BYTE* error_string);
extern "C" _DWORD PCI_TimerWait   (_BYTE CardNr, _WORD Time, _BYTE TimeBase, _DWORD &ErrorStatus); 
extern "C" _DWORD PCI_TimerSet    (_BYTE CardNr, _WORD Time, _BYTE TimeBase, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_StatusRegRead(_BYTE CardNr, _DWORD &StatusReg, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_MilBusWrite (_BYTE CardNr, _WORD data, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_DriverClose (_BYTE CardNr);
extern "C" _DWORD PCI_IntMaskRead (_BYTE CardNr, _DWORD &data, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntMaskWrite(_BYTE CardNr, _DWORD data , _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntEnable   (_BYTE CardNr, MYDRIVER_INT_HANDLER funcIntHandler, void *pUserDat, _WORD Priority, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntDisable  (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntWait     (_BYTE CardNr, _DWORD &IntAktiv, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntAktiv(_BYTE CardNr, _DWORD &IntAktiv, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntNextEnable(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_IntDisable   (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_CntrlRegSet (_BYTE CardNr, _BYTE SetValue, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_CntrlRegRead(_BYTE CardNr, _BYTE &CntrlRegValue, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvFilterSet (_BYTE CardNr, _WORD EventCodeNr, _BYTE SetEV, _DWORD &ErrorStatus); 
extern "C" _DWORD PCI_EvFiFoRead  (_BYTE CardNr, _WORD &EventCodeNr, bool &EVFifoNotEmpty, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvFilterRead(_BYTE CardNr, _WORD EventCodeNr, _BYTE &FilterValue, _DWORD &ErrorStatus);
extern "C" bool PCI_StatusTest (_BYTE CardNr, _DWORD StatusBit, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_DataFiFoRead(_BYTE CardNr, _WORD &data, bool &Fifo_not_empty ,_DWORD &ErrorStatus);
extern "C" _DWORD PCI_MilBusRead  (_BYTE CardNr, _WORD &data, bool &Fifo_not_empty, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvTimerRead (_BYTE CardNr, _DWORD &Time ,_DWORD &ErrorStatus);
extern "C" _DWORD PCI_PulsOutSet  (_BYTE CardNr, _WORD PulsBuchse, _WORD EventOn, _WORD EventOff, _WORD PulsRahmen, bool OnOff, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls1Start(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls1Stop (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls2Start(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls2Stop (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls3Start(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls3Stop (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls4Start(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls4Stop (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls5Start(_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_SoftPuls5Stop (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvFilterClear (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvFiFioClear  (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvFilterClear (_BYTE CardNr, _DWORD &ErrorStatus);
extern "C" _DWORD PCI_EvFiFioClear  (_BYTE CardNr, _DWORD &ErrorStatus);