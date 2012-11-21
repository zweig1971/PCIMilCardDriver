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

// beinhaltet lebenswichtige karteninformationen
// --------------------------------------------
typedef struct MYDRIVER_STRUCT *MYDRIVER_HANDLE;

// Für den KartenTest
//
#define TestTime				0xEE
#define TimeWaitOfTimer			1

#define WaitMilBusReadTimeOut	10	
#define WaitMilBusWriteTimeOut	50
#define Fct_Rd_Stat0			0xC0
#define Fct_Rd_Stat0_AdrMask	0x00FF
#define Inter_PLX_Active		0x5b

enum {DataSend,CommandSend};
enum _TimerBase{Timerbase_10us, Timerbase_1ms};


// Bits fuer die kommunikation zwischen USB <-> ALTERA
// ---------------------------------------------------
#define USB_PROT_START				0xA5	//Startseq.
#define USB_PROT_MIL_RDWR			0x04	
#define USB_PROT_MIL_CMD			0x08
#define USB_PROT_GLOBAL_RES			0x0C	
#define USB_PROT_LD_TIMER1			0x10	
#define USB_PROT_LD_TIMER2_10us		0x14	
#define USB_PROT_LD_TIMER2_1ms		0x18	
#define USB_PROT_CNTRL_REG_RDWR		0x1C
#define USB_PROT_SLAVE_MIL			0x20
#define USB_PROT_EVENT_RD			0x24
#define USB_PROT_EV_TIME_RD			0x28
#define USB_PROT_INTR_MASK_RDWR		0x2C
#define USB_PROT_AUX_IO_RDWR		0x30
#define USB_PROT_PULS1_START		0x34
#define USB_PROT_PULS1_STOP			0x38
#define USB_PROT_PULS2_START		0x3C
#define USB_PROT_PULS2_STOP			0x40
#define USB_PROT_PULS3_START		0x44
#define USB_PROT_PULS3_STOP			0x48
#define USB_PROT_PULS4_START		0x4C
#define USB_PROT_PULS4_STOP			0x50
#define USB_PROT_PULS5_START		0x54
#define USB_PROT_PULS5_STOP			0x58

#define USB_PROT_STAT1_RD			0x00

#define USB_PROT_NULL_BYTE			0x00

#define USB_PROT_WR					0x01
#define USB_PROT_RD					0x00


//Sende (Tx) und Lesebuffer (Rx) definieren
//------------------------------------------
#define USB_TxBufferMAX  8
#define USB_RxBufferMAX  4

//Timeouts fuer das lesen und schreiben zur USB-Device
//-----------------------------------------------------
#define USB_TxTimeout  50
#define USB_RxTimeout  50

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


// Bitmasken fuer den Software Puls	
//------------------------------------
#define SW_Start_Puls			0x1
#define SW_Stop_Puls			0x1


// DatenTypen
//-----------------
/*typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int D_WORD;*/

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
#define FailedSetPriority	         0x2000000


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

