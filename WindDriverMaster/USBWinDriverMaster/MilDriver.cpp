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

#include "MilDriver.h"

// ----------------------------------------------------
//
// Globale Object unabhaenige Variablen
//
// ----------------------------------------------------

struct ProcessInfo
{
	DWORD InterruptID;
	DWORD Process_ID;
};

ProcessInfo myProcessInfo;

// ----------------------------------------------------
//
// Globale Object unabhaenige Funktionen
//
// ----------------------------------------------------


// Zählt alle FTDI am USB-Bus
// 
int CountUSBDevices (int &findUSBDev)
{
	int status = 0;
	FT_ListDevices(&findUSBDev,NULL,FT_LIST_NUMBER_ONLY);
	return(StatusOK);
}


// Gibt die FehlerCodes in Klartext aus
//
string PrintErrorMessage (unsigned int status)
{
	string ErrorMessage;

	switch (status)
	{
	case StatusOK:
		ErrorMessage = "Status OK";
		break;
	case VendorID_Error:
		ErrorMessage = "VendorID Error";
        break;
	case DeviceID_Error:
		ErrorMessage = "DeviceID Error";
		break;
	case CountCards_Error:
		ErrorMessage = "Count Cards Error";
		break;
	case Open_Failure:
		ErrorMessage = "Open Card Failure";
		break;
	case IncorrectWinDriver:
		ErrorMessage = "Incorrecte WinDriver Version";
		break;
	case InvalidHandleValue:
		ErrorMessage = "Invalid Handlevalue";
		break;
	case Failed_Open_WinDrv_Device:
		ErrorMessage = "Cant open WinDriver device";
		break;
	case MilBus_Busy:
		ErrorMessage = "MilBus busy";
		break;
	case TestTimerError:
		ErrorMessage = "Timer 1/2 not running";
		break;
	case Fifo_Full:
		ErrorMessage = "Fifo is Full";
		break;
	case CardNrError:
		ErrorMessage = "Invalid Card Number";
		break;
	case NoPCICartOpen:
		ErrorMessage = "No Cards Open";
		break;
	case NoPCMil:
		ErrorMessage = "No Cards Open";
		break;
	case ReadMilBusTimeOut:
		ErrorMessage = "Read MilBus Timeout";
		break;
	case WriteMilBusTimeOut:
		ErrorMessage = "Write MilBus Timeout";
		break;
	case FifoNotEmpty:
		ErrorMessage = "FiFo not empty";
		break;
	case FifoNotCleared:
		ErrorMessage = "FiFo not cleared";
		break;
	case NoDataToRead:
		ErrorMessage = "No data to read";
		break;
	case EventNrError:
		ErrorMessage = "Event Nr. error";
		break;
	case IFKNrError:
		ErrorMessage = "IFFK Nr. not equal";
		break;
	case FailedEnableIntr:
		ErrorMessage = "Failed interrupt enable";
		break;
	case IntMaskError:
		ErrorMessage = "IntMask value incorrect";
		break;
	case GeneralError:
		ErrorMessage = "Gerneral Error maybe Value error";
		break;
	case MasterSlaveError:
		ErrorMessage = "Master / Slave Error";
		break;
	case UnrequestedReceiv:
		ErrorMessage = "Unrequested receiv";
	case FailedSetPriority:
		ErrorMessage = "Failed set interrupt prio"; 
		break;
	default : ErrorMessage = "Undefined Error";
	}

	return (ErrorMessage);
}


// ----------------------------------------------------
//
// Spezielle Funktionen vom Object MilDriver
//
// ----------------------------------------------------


MilDriver::MilDriver(void)
{
	USBData.MYUSB_HANDLE	  = NULL;
	myProcessInfo.Process_ID  = NULL;
	myProcessInfo.InterruptID = NULL;

	time_t Zeit;
	time(&Zeit);
	srand((unsigned int)Zeit);
	myProcessInfo.InterruptID = (rand() % 10000);
}


MilDriver::~MilDriver(void)
{
	if(USBData.MYUSB_HANDLE != NULL)
	{
		CloseUSBDev();
	}
}

//Liest daten an eine Funktion auf der USB-Device
//
void MilDriver::ReadUSB_Device(DWORD &data, WORD USB_Funtion )
{
	DWORD SendTxByte		= 0;
	DWORD ReadRxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;

	WORD i = 1;

	USB_TxBuffer[0] = USB_PROT_START;
	USB_TxBuffer[1] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[2] = USB_Funtion;
	USB_TxBuffer[3] = USB_PROT_RD;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[6] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[7] = USB_PROT_NULL_BYTE;

	// Rx und Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if((RestBytesRxBuffer != 0) || (RestBytesTxBuffer != 0))
	{
		FT_Purge(USBData.MYUSB_HANDLE, FT_PURGE_RX | FT_PURGE_TX);
	}
	
	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);

	FT_Read(USBData.MYUSB_HANDLE, USB_RxBuffer, USB_RxBufferMAX, &ReadRxByte);
	
	data = (USB_RxBuffer[2] << 8) & 0xFF00;
	data = data | USB_RxBuffer[3];
}

//Schreibt daten an eine Funktion auf der USB-Device
//
void MilDriver::WriteUSB_Device(DWORD data, WORD USB_Funktion)
{
	DWORD SendTxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;
	WORD  TEMP              = 0;

	USB_TxBuffer[0] = USB_PROT_START;
	USB_TxBuffer[1] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[2] = USB_Funktion;
	USB_TxBuffer[3] = USB_PROT_WR;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;

	TEMP = (data & 0xFF00)>>8;
	USB_TxBuffer[6] = BYTE(TEMP);

	TEMP = data & 0x00FF;
	USB_TxBuffer[7] = BYTE(TEMP);

	//Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if(RestBytesTxBuffer != 0)
	{
		FT_Purge(USBData.MYUSB_HANDLE,FT_PURGE_TX);
	}

	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);
}

// Ermöglicht den Zugriff auf die gewünschte USB-Device im Bus
//
int MilDriver::OpenUSBDev (int USBNumber)
{
	DWORD status = 0;
	USBNumber = (USBNumber-1);
	USBData.USBNumber = USBNumber;

	status = FT_Open(USBNumber,&USBData.MYUSB_HANDLE);

	if(status != FT_OK)
	{
		return(Open_Failure);
	}
	else
	{
		Reset_USBDev();
		return(StatusOK);
	}
	
}

// Gibt die USB-Device wieder frei
//
void MilDriver::CloseUSBDev (void)
{
	FT_ResetDevice(USBData.MYUSB_HANDLE);
	FT_Close(USBData.MYUSB_HANDLE);
}

// Legt auf dem MilBus fest, ob Datenwort ein Kommando 
// oder ein Datum ist
void MilDriver::SendCommand (WORD data)
{
	WriteUSB_Device(data, USB_PROT_MIL_CMD);
}

// Schreibt Daten auf den MilBus
//
void MilDriver::WriteMilBus (WORD data)
{
	WriteUSB_Device(data,USB_PROT_MIL_RDWR);
}

// Liest Daten vom MilBus
//
void MilDriver::ReadMilBus (WORD &data)
{
	DWORD ReadData = 0;
	ReadUSB_Device(ReadData, USB_PROT_MIL_RDWR);
	data = WORD(ReadData);
}

// Liest Daten vom Event FiFo
//
void MilDriver::ReadEventFiFo(WORD &data)
{
	DWORD ReadData = 0;
	ReadUSB_Device(ReadData, USB_PROT_EVENT_RD);
	data = WORD(ReadData);
}

// Setzt Timer 1 auf der USB-Device auf gewünschten
// Wert
void MilDriver::SetTimer1 (DWORD time)
{
	WriteUSB_Device(time, USB_PROT_LD_TIMER1);
}

// Setzt Timer 2 auf der USB-Device auf gewünschten
// Wert in us
void MilDriver::SetTimer2_10us (DWORD time)
{
	WriteUSB_Device(time, USB_PROT_LD_TIMER2_10us);
}

// Setzt Timer 2 auf der USB-Device auf gewünschten
// Wert in ms
void MilDriver::SetTimer2_1ms (DWORD time)
{
	WriteUSB_Device(time, USB_PROT_LD_TIMER2_1ms);
}

// Liest die vergangene Zeit zwischen zwei events ein
// (wenn diese gesetzt)
void MilDriver::ReadEVTimer	(DWORD &time)
{
	ReadUSB_Device(time, USB_PROT_EV_TIME_RD);
}

// Liest den MilBus Status ein
//
void MilDriver::MilBusStatus (DWORD &MilStatus)
{
	DWORD SendTxByte		= 0;
	DWORD ReadRxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;

	WORD i = 1;

	USB_TxBuffer[0] = USB_PROT_START;
	USB_TxBuffer[1] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[2] = USB_PROT_STAT1_RD;
	USB_TxBuffer[3] = USB_PROT_RD;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[6] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[7] = USB_PROT_NULL_BYTE;

	// Rx und Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if((RestBytesRxBuffer != 0) || (RestBytesTxBuffer != 0))
	{
		FT_Purge(USBData.MYUSB_HANDLE, FT_PURGE_RX | FT_PURGE_TX);
	}
	
	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);

	FT_Read(USBData.MYUSB_HANDLE, USB_RxBuffer, USB_RxBufferMAX, &ReadRxByte);
	
	MilStatus = USB_RxBuffer[0];

	// Milstatus ist > 16 Bit
	while(i<4)
	{
		MilStatus = MilStatus << 8;
		MilStatus = MilStatus | USB_RxBuffer[i];
		i++;
	}
}

// Zurücksetzen der USB-Device
//
void MilDriver::Reset_USBDev (void)
{
	DWORD SendTxByte		= 0;

	USB_TxBuffer[0] = USB_PROT_START;
	USB_TxBuffer[1] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[2] = USB_PROT_GLOBAL_RES;
	USB_TxBuffer[3] = USB_PROT_WR;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[6] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[7] = USB_PROT_NULL_BYTE;
	
	// Reset Altera
	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);

	// Reset FTDI
	FT_ResetDevice(USBData.MYUSB_HANDLE);

	// Timeouts neu setzen
	FT_SetTimeouts(USBData.MYUSB_HANDLE, USB_TxTimeout, USB_RxTimeout);
	FT_SetLatencyTimer(USBData.MYUSB_HANDLE,1);

	// Rx und Tx Buffer loeschen
	FT_Purge(USBData.MYUSB_HANDLE, FT_PURGE_RX | FT_PURGE_TX);
}

// Setzen des Control Register 1
//
void MilDriver::SetControlRegister1(WORD data)
{
	WriteUSB_Device(data, USB_PROT_CNTRL_REG_RDWR);
}

// Lesen aus dem Control Register
void MilDriver::ReadControlRegister1(WORD &data)
{	
	DWORD ReadData = 0;
	ReadUSB_Device(ReadData, USB_PROT_CNTRL_REG_RDWR);
	data = WORD(ReadData);
}

// Lesen des Event Filters
//
void MilDriver::ReadEventFilter(DWORD &FilterValue, WORD OffsetValue)
{
	DWORD SendTxByte		= 0;
	DWORD ReadRxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;
	WORD  TEMP              = 0;

	WORD i = 1;

	USB_TxBuffer[0] = USB_PROT_START;

	TEMP = (OffsetValue & 0xFF00)>>8;
	TEMP = TEMP | 0x40;
	USB_TxBuffer[1] = BYTE(TEMP);

	TEMP = OffsetValue & 0x00FF;
	USB_TxBuffer[2] = BYTE(TEMP);

	USB_TxBuffer[3] = USB_PROT_RD;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[6] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[7] = USB_PROT_NULL_BYTE;

	// Rx und Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if((RestBytesRxBuffer != 0) || (RestBytesTxBuffer != 0))
	{
		FT_Purge(USBData.MYUSB_HANDLE, FT_PURGE_RX | FT_PURGE_TX);
	}
	
	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);

	FT_Read(USBData.MYUSB_HANDLE, USB_RxBuffer, USB_RxBufferMAX, &ReadRxByte);
	
	FilterValue = (USB_RxBuffer[0] << 8) & 0xFF000000;
	FilterValue = (USB_RxBuffer[1] << 8) & 0xFF0000;
	FilterValue = (USB_RxBuffer[2] << 8) & 0xFF00;
	FilterValue = FilterValue | USB_RxBuffer[3];
}

// Setzen des Event Filters
//
void MilDriver::SetEventFilter(WORD data, WORD OffsetValue)
{
	DWORD SendTxByte		= 0;
	DWORD ReadRxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;
	WORD  TEMP              = 0;

	WORD i = 1;

	USB_TxBuffer[0] = USB_PROT_START;

	TEMP = (OffsetValue & 0xFF00)>>8;
	TEMP = TEMP | 0x40;
	USB_TxBuffer[1] = BYTE(TEMP);

	TEMP = OffsetValue & 0x00FF;
	USB_TxBuffer[2] = BYTE(TEMP);

	USB_TxBuffer[3] = USB_PROT_WR;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;

	TEMP = (data & 0xFF00)>>8;
	USB_TxBuffer[6] = BYTE(TEMP);

	TEMP = data & 0x00FF;
	USB_TxBuffer[7] = BYTE(TEMP);

	//Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if(RestBytesTxBuffer != 0)
	{
		FT_Purge(USBData.MYUSB_HANDLE,FT_PURGE_TX);
	}

	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);
}

// Enable die Interrupts im WinDriver
//
int MilDriver::IntEnable (PVOID pUserDat, WORD SET_MY_PRIORITY)
{
//	return(MYDRIVER_IntEnable (CardInfo.myHandle, funcIntHandler, pUserDat, SET_MY_PRIORITY));
	return(GeneralError);
}

// Disabled die Interrupts im WinDriver
//
void MilDriver::IntDisable (void)
{
//	myProcessInfo.Process_ID = NULL;
//	MYDRIVER_IntDisable (CardInfo.myHandle);
}

// Lesen der Interrupt- Maske
//
void MilDriver::ReadIntMask(DWORD &data)
{
	DWORD SendTxByte		= 0;
	DWORD ReadRxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;

	WORD i = 1;

	USB_TxBuffer[0] = USB_PROT_START;
	USB_TxBuffer[1] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[2] = USB_PROT_INTR_MASK_RDWR;
	USB_TxBuffer[3] = USB_PROT_RD;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[6] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[7] = USB_PROT_NULL_BYTE;

	// Rx und Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if((RestBytesRxBuffer != 0) || (RestBytesTxBuffer != 0))
	{
		FT_Purge(USBData.MYUSB_HANDLE, FT_PURGE_RX | FT_PURGE_TX);
	}
	
	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);

	FT_Read(USBData.MYUSB_HANDLE, USB_RxBuffer, USB_RxBufferMAX, &ReadRxByte);

	data = USB_RxBuffer[0];

	//Interrupt- Maske > 16 Bit
	while(i<4)
	{
		data = data << 8;
		data = data | USB_RxBuffer[i];
		i++;
	}
}

// Schreiben der Interrupt- Maske
//
void MilDriver::WriteIntMask(DWORD data)
{
	DWORD SendTxByte		= 0;
	DWORD ReadRxByte		= 0;
	DWORD RestBytesRxBuffer = 0;
	DWORD RestBytesTxBuffer = 0;
	DWORD EventStatus		= 0;

	USB_TxBuffer[0] = USB_PROT_START;
	USB_TxBuffer[1] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[2] = USB_PROT_INTR_MASK_RDWR;
	USB_TxBuffer[3] = USB_PROT_WR;

	USB_TxBuffer[4] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[5] = USB_PROT_NULL_BYTE;
	USB_TxBuffer[6] = (data & 0xFF00)>>8;
	USB_TxBuffer[7] = (data & 0xFF);

	//Tx Buffer loeschen
	FT_GetStatus(USBData.MYUSB_HANDLE, &RestBytesRxBuffer, &RestBytesTxBuffer, &EventStatus);
	if(RestBytesTxBuffer != 0)
	{
		FT_Purge(USBData.MYUSB_HANDLE,FT_PURGE_TX);
	}

	FT_Write(USBData.MYUSB_HANDLE, USB_TxBuffer, USB_TxBufferMAX, &SendTxByte);
}


// Die von der PCI Karte im PC zur verfügunggestellten
// Resoucen werden getestet
int MilDriver::TestUSB (void)
{
	WORD data	 =0;
	DWORD UserStatus = 0;
	bool EVFifoNotEmpty = true;

	// Event Filter einschalten
	SetCntrlReg1 (Cntrl_EvFilter_On, UserStatus);

	// Event FiFo leeren
	while(EVFifoNotEmpty)
	{		
		ReadEVFromFiFo (data,UserStatus,EVFifoNotEmpty);
	}

#ifndef PCIMilTreiber_Slave
	// Prüfen ob Karte MASTER ist
	if (TestStatus(Master_Slave))
	{
		return(MasterSlaveError);
	}
#endif

#ifdef PCIMilTreiber_Slave
	// Prüfen ob Karte SLAVE ist
	if (!(TestStatus(Master_Slave)))
	{
		return(MasterSlaveError);
	}
#endif

	return (StatusOK);
}

// Auswerten des MilBus Status
// 
bool MilDriver::TestStatus(DWORD StatusBit)
{
	DWORD MilStatus = 0;

	MilBusStatus(MilStatus);

	if((MilStatus & StatusBit)== StatusBit)
		return(true);
	else
		return(false);
}

// wartet die vorgegebene Zeit in sec. ab
//
void MilDriver::wait(int UserTime)
{
	time_t ltime;
	time_t readyTime;

	time(&ltime);

	readyTime = ltime+UserTime+1;

	while (ltime < readyTime)
	{
		time(&ltime);
	}
}


// Hier werden die nötigen Vorbereitungen getroffen
// fuer die Aktivierung der PC- Karte
int MilDriver::ConnectMilBus(DWORD NumberOfCard)
{
	int status=0;

	status = OpenUSBDev(NumberOfCard);
	if(status != StatusOK)
		return(status);

	status = TestUSB();
	if(status != StatusOK)
		return(status);

	return(status);
}


// Prüft, ob eine SE Online ist.
//
bool MilDriver::IFKAlive(BYTE IFKAdresse,BYTE &ReturnIFKNR, DWORD &UserStatus)
{

	bool MilBusReady = false;
	
	// StatusByte 1 lesen
	//
	WORD ReadAdress		  = 0;
	WORD SendFunktionCode = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return (false);
	}

	// Prüfen ob MilBus bereit ist
	//
	MilBusReady = WaitMilBusWrite();

	if(MilBusReady == false)
	{
		UserStatus = (UserStatus | WriteMilBusTimeOut);
		return(false);
	}

	// FunktionCode zusammenbasteln
	// 
	SendFunktionCode = Fct_Rd_Stat0;
	SendFunktionCode = SendFunktionCode << 8;
	SendFunktionCode = SendFunktionCode + IFKAdresse;

	// Zusammengebastelte Funktionscode verschicken
	//
	SendCommand(SendFunktionCode);

	// Prüfen ob MilBus bereit ist
	//
	MilBusReady = WaitMilBusRead();

	if(MilBusReady == false)
	{
		UserStatus = (UserStatus | ReadMilBusTimeOut);
		return(false);
	}

	// Daten vom MilBus lesen
	//
	ReadMilBus(ReadAdress);

	// Bei C0-Status Highbyte wegmaskieren
	//
	ReadAdress = ReadAdress & Fct_Rd_Stat0_AdrMask; 

	ReturnIFKNR = ReadAdress;
	
	// Daten muessen gleich sein
	//
	if(ReadAdress != IFKAdresse)
	{
		UserStatus = (UserStatus | IFKNrError);
		return(false);
	}
	return(true);
}


void MilDriver::ResetUSBDev(DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}	

	Reset_USBDev();
}


// Timer2 setzen, Funktion kommt erst nach Ablauf des Timers
// zurück
void MilDriver::WaitOfTimer2 (WORD time, _TimerBase Timerbase, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}	

	if(Timerbase == Timerbase_10us)
	{
		SetTimer2_10us((int)time);
	}
	else
	{
		SetTimer2_1ms((int)time);
	}

	while (TestStatus(Timeout_2) != true)
	{
	}
}


// Setzt den Timer und kehrt dann wieder zurück
// Überwachung ob timer abgelaufen, ist Sache des Anwenders
void MilDriver::SetTimer2(WORD time, _TimerBase Timerbase, DWORD &UserStatus)	
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	if(Timerbase == Timerbase_10us)
	{
		SetTimer2_10us((int)time);
	}
	else
	{
		SetTimer2_1ms((int)time);
	}
}


// Setzen des Control Registers 1, die Fktn ist noch unvollständig
// und muss überarbeitet werden
void MilDriver::SetCntrlReg1 (BYTE SetValue, DWORD &UserStatus)
{
	WORD CntrlReg = 0;
	BYTE EVValue  = 0;
	DWORD Status  = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	switch(SetValue)
	{
		case Cntrl_EvFilter_On:
			ReadControlRegister1(CntrlReg); // Register lesen
			CntrlReg = CntrlReg | Ctrl_EvFilter_On; // Filter einschalten
			SetControlRegister1(CntrlReg); // Register schreiben
			break;

		case Cntrl_EvFilter_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_EvFilter_On; // Filter ausschalten
			SetControlRegister1(CntrlReg); 
			break;
	
		case Cntrl_Debounce_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Debounce_On;
			SetControlRegister1(CntrlReg);
			break;

		case Cntrl_Debounce_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Debounce_On;
			SetControlRegister1(CntrlReg);
			break;

		case Cntrl_Mil_FiFo_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Mil_Fifo_On; // FiFo einschalten
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_Mil_FiFo_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Mil_Fifo_On; // FiFo ausschalten
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_En_Intr1:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_En_Intr1; // Intr.1 einschalten
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_Dis_Intr1:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_En_Intr1; // Intr.1 ausschalten 
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_En_Intr2:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_En_Intr2; // Intr.2 einschalten
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_Dis_Intr2:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_En_Intr2; // Intr.1 ausschalten 
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_EvFilter_12_Bit:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_EvFilter_8_12; // Filter auf 12 Bit umschalten
			SetControlRegister1(CntrlReg); 
			break;

		case Cntrl_EvFilter_8_Bit:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_EvFilter_8_12; // Filter auf 8 Bit umschalten
			SetControlRegister1(CntrlReg); 			
			break;


		case Cntrl_Puls1_Rahmen_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Puls1_Rahmen_On; // Puls 1 einschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls1_Rahmen_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Puls1_Rahmen_On; // Puls 1 ausschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls2_Rahmen_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Puls2_Rahmen_On; // Puls 2 einschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls2_Rahmen_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Puls2_Rahmen_On; // Puls 2 ausschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls3_Rahmen_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Puls3_Rahmen_On; // Puls 3 einschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls3_Rahmen_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Puls3_Rahmen_On; // Puls 3 ausschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls4_Rahmen_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Puls4_Rahmen_On; // Puls 4 einschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls4_Rahmen_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Puls4_Rahmen_On; // Puls 4 ausschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls5_Rahmen_On:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg | Ctrl_Puls5_Rahmen_On; // Puls 5 einschalten
			SetControlRegister1(CntrlReg); 	
			break;

		case Cntrl_Puls5_Rahmen_Off:
			ReadControlRegister1(CntrlReg);
			CntrlReg = CntrlReg & ~Ctrl_Puls5_Rahmen_On; // Puls 5 ausschalten
			SetControlRegister1(CntrlReg); 	
			break;

		default : UserStatus = (UserStatus | GeneralError);
			break;
	}
}


// Lesen vom Control- Register
//
void MilDriver::ReadCntrlReg1 (BYTE &CntrlReg, DWORD &UserStatus)
{
	WORD Cntrl = 0;
	int Offset = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	ReadControlRegister1(Cntrl); // Register lesen
	CntrlReg   = (BYTE)(Cntrl);
}


// Testet das StatusRegister der Karte mit dem vom
// Anwender angegebenen StatusBit. Gibt enstsprechend
// true oder false mit
bool MilDriver::TestUSBDevStatus (DWORD StatusBit, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return(false);
	}

	if(TestStatus(StatusBit))
		return(true);
	else
		return(false);
}

// MilBus prüfen ob sendebereit, wenn nicht wird ein timer
// aufgezogen. True = MilBus frei, False = TimeOut
bool MilDriver::WaitMilBusWrite(void)
{
	DWORD status	= 0;
	
	// Prüfen ob MilBus sende bereit -> setzen des timers
	// sonst unnötig
	if(TestStatus(Trm_Rdy))
	{
		return(true);
	}

	SetTimer1 (WaitMilBusWriteTimeOut);

	// solange pollen, bis entweder timeout oder MilBus ready
	//
	while(status == 0)
	{
		MilBusStatus(status); 
		status = (status & (Timeout_1 | Trm_Rdy));
	}
	
	// Prüfen was nun passiert ist, timeout oder MilBus ready
	//
	if (status &  Trm_Rdy)
	{
		return(true);// Alles klar -> MilBus ready
	}

	return (false);// scheisse -> timeout
}

// Liest das Register vom PLX aus
//
DWORD MilDriver::Read_INTCSR (void)
{
	return(GeneralError);
}

// Schreibt in das Register vom PLX
//
void MilDriver::Write_INTCSR (DWORD data)
{

}

// Prüft ob MilBus wieder sende bereit ist, dabei
// wird ein Timer aufgezogen
bool MilDriver::WaitMilBusRead(void)
{
	DWORD status	= 0;

	// Prüfen ob MilBus sende bereit -> setzen des timers
	// sonst unnötig
	if(TestStatus(Mil_Rcv_Rdy))
	{
		return(true);
	}

	SetTimer1 (WaitMilBusReadTimeOut);

	// solange pollen, bis entweder timeout oder MilBus ready
	//
	while(status == 0)
	{
		MilBusStatus(status); 
		status = (status & (Timeout_1 | Mil_Rcv_Rdy));
	}
	
	// Prüfen was nun passiert ist, timeout oder MilBus ready
	//
	if (status &  Mil_Rcv_Rdy)
	{
		return(true);// Alles klar -> MilBus ready
	}

	return (false);// scheisse -> timeout
}

// Liest den Status
//
void MilDriver::ReadStatusReg (DWORD &myStatusReg, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}	
	
	MilBusStatus(myStatusReg);
}

// Schreibt daten (data) an eine bestimmte IFK
//
void MilDriver::WriteDataToIFK (BYTE FunktionCode, BYTE IFKAdresse, WORD data, DWORD &UserStatus)
{
	bool MilBusReady = false;
	int SendFunktionCode = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Daten auf MilBus schreiben
	//
	WriteMilBus (data);

	// FunktionCode zusammenbasteln
	// 
	SendFunktionCode = FunktionCode;
	SendFunktionCode = SendFunktionCode << 8;
	SendFunktionCode = SendFunktionCode + IFKAdresse;

	// Zusammengebastelte Funktionscode verschicken
	//
	SendCommand(SendFunktionCode);
}


// Liest daten (data) von einer bestimmten IFK
//
void MilDriver::ReadDataFromIFK (BYTE FunktionCode, BYTE IFKAdresse, WORD &data, DWORD &UserStatus)
{
	bool MilBusReady = false;
	int SendFunktionCode = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// FunktionCode zusammenbasteln
	// 
	SendFunktionCode = FunktionCode;
	SendFunktionCode = SendFunktionCode << 8;
	SendFunktionCode = SendFunktionCode + IFKAdresse;

	// Zusammengebastelte Funktionscode verschicken
	//
	SendCommand(SendFunktionCode);

	// Daten vom MilBus lesen
	//
	ReadMilBus(data);
}

// Versendet einen Funktions Code an eine IFK
//
void MilDriver::SendFknCodeToIFK(BYTE FunktionCode, BYTE IFKAdresse, DWORD &UserStatus)
{
	bool MilBusReady = false;
	int SendFunktionCode = 0;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// FunktionCode zusammenbasteln
	// 
	SendFunktionCode = FunktionCode;
	SendFunktionCode = SendFunktionCode << 8;
	SendFunktionCode = SendFunktionCode + IFKAdresse;

	// Zusammengebastelte Funktionscode verschicken
	//
	SendCommand(SendFunktionCode);
}

// Liest Daten vom Fifo
//
void MilDriver::ReadDataFromFiFo (WORD &data, DWORD &UserStatus, bool &Fifo_not_empty)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Liegen noch Daten an
	//
	if(TestStatus(Mil_Rcv_Rdy))
	{
		ReadMilBus(data);
		Fifo_not_empty = TestStatus(Mil_Rcv_Rdy);
	}
	else
	{
		Fifo_not_empty = false;
	}
}

// Liest einfach Daten vom Bus
//
void MilDriver::ReadData (WORD &data, DWORD &UserStatus)
{
	bool MilBusReady = false;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Daten vom MilBus lesen
	//
	ReadMilBus(data);
}
// Schreibt einfach Daten auf den Bus
//
void MilDriver::WriteData (WORD data, DWORD &UserStatus)
{
	bool MilBusReady = false;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Daten schreiben
	//
	WriteMilBus (data);
}

// Schreibt ein Kommandowort auf den Bus
//
void MilDriver::WriteCMD (WORD cmd, DWORD &UserStatus)
{
	bool MilBusReady = false;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Komandowort schreiben
	//
	SendCommand(cmd);
}

// Liest Events vom Fifo, und prüft ob daten noch
// vorhanden sind
void MilDriver::ReadEVFromFiFo (WORD &data, DWORD &UserStatus, bool &EVFifoNotEmpty)
{
	bool MilBusReady = false;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Liegen noch Daten an
	//
	if(TestStatus(Evt_Fifo_Not_Empty))
	{
		ReadEventFiFo(data);
		EVFifoNotEmpty = TestStatus(Evt_Fifo_Not_Empty);
	}
	else
	{
		EVFifoNotEmpty = false;
	}
}

// Setzt die Bits abhängig von der EventNr.
// Event in FiFo on/off , Timer Clear/run
void MilDriver::SetEVFilter (WORD EventCodeNr, BYTE SetEV, DWORD &UserStatus)
{
	int Offset		= 0;
	WORD SendValue	= 0;
	DWORD FilterValue= 0;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// EventCode prüfen
	//
	if ((EventCodeNr > 4095) || (EventCodeNr < 0))
	{
		UserStatus = (UserStatus | EventNrError);
		return;	
	}

	Offset = EventCodeNr << 2;

	switch(SetEV)
	{
		case EventOn: 
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue | (WORD)EvFilter_EventOn;
			SetEventFilter (SendValue,Offset);
			break;

		case EventOff:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue & (WORD)~EvFilter_EventOn;
			SetEventFilter (SendValue, Offset);
			break;
		
		case  EventTimerClear:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue & (WORD)~EvFilter_EventTimerRun;
			SetEventFilter (SendValue, Offset);			
			break;

		case EventTimerRun:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue | (WORD) EvFilter_EventTimerRun;
			SetEventFilter (SendValue, Offset);
			break;

		case Puls1_On:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue | (WORD)EvFilter_Start_Puls1;//Puls1_On  
			SendValue = SendValue & (WORD)~EvFilter_Stop_Puls1;
			SetEventFilter (SendValue,Offset);
			break;

		case Puls1_Off:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue | (WORD)EvFilter_Stop_Puls1;//Puls1_Off
			SendValue = SendValue & (WORD)~EvFilter_Start_Puls1;
			SetEventFilter (SendValue,Offset);
			break;

		case Puls2_On:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue | (WORD)EvFilter_Start_Puls2;//Puls2_On
			SendValue = SendValue & (WORD)~EvFilter_Stop_Puls2;
			SetEventFilter (SendValue,Offset);
			break;

		case Puls2_Off:
			ReadEventFilter (FilterValue, Offset);
			SendValue = FilterValue | (WORD)EvFilter_Stop_Puls2;//Puls2_Off
			SendValue = SendValue & (WORD)~EvFilter_Start_Puls2;
			SetEventFilter (SendValue,Offset);
			break;
		case EventClear:
			SetEventFilter(0, Offset);
			break;
	}
}


void MilDriver::ReadFromEVFilter(WORD EventCodeNr, BYTE &EVValue, DWORD &UserStatus)
{
	DWORD FilterValue= 0;
	int Offset		  = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// EventCode prüfen
	//
	if ((EventCodeNr > 4095) || (EventCodeNr < 0))
	{
		UserStatus = (UserStatus | EventNrError);
		return;	
	}

	Offset = EventCodeNr << 2;

	ReadEventFilter (FilterValue, Offset);

	EVValue    = (BYTE) FilterValue; // Die EVValue sitzt in den unteren 8 bit 
//	CntrlReg   = (BYTE)(FilterValue >> 8); // Das Cntrl Reg. sitz ab dem 8ten Bit

	//  321098 76543210
	//			     ^^ EVValues Bits (01) Event aktiv (10) Timer 
	//  ^^^^^^ Cntrl Register Bits
}					


// Liest die Zeit zwischen zwei events ein
//
void MilDriver::ReadEventTimer  (DWORD &Time, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	ReadEVTimer(Time);
}

// Setzt alle settings im Ev-Filter 
// wieder auf null
void MilDriver::ClearEvFilter(DWORD &UserStatus)
{
	WORD counter = 0;
	WORD Offset	 = 0;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// EvFilter loeschen
	while(counter != 4095)
	{
		Offset = counter << 2;
		SetEventFilter(0, Offset);
		counter++;
	}
}

// Liest alle evens im fifo aus und
// cleard es so damit
void MilDriver::ClearEvFiFio (DWORD &UserStatus)
{
	WORD data	 =0;
	DWORD ErrorStatus = 0;
	bool EVFifoNotEmpty = true;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}
	// Event FiFo leeren
	while(EVFifoNotEmpty)
	{		
		ReadEVFromFiFo (data,ErrorStatus,EVFifoNotEmpty);
	}
}

/* Lesen der Interrupt-Maske
//
void MilDriver::ReadInterruptMask(DWORD &data, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}
	
	ReadIntMask(data);
}


// Interrupt- Maske setzen
//
void MilDriver::WriteInterruptMask(DWORD data, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	if((data < 0) || (data > 255))
	{
		UserStatus = (UserStatus | IntMaskError);
		return;		
	}

	WriteIntMask(data);
}


// Interrupts einschalten
//
void MilDriver::InterruptEnable(DWORD &UserStatus, MYDRIVER_INT_HANDLER funcIntHandler, PVOID pUserDat, WORD SET_MY_PRIORITY)
{
	BOOL status;
	DWORD Status;

	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Interrupts im WinDriver Freischalten
	if(funcIntHandler != NULL)
	{
		status =  IntEnable (funcIntHandler, pUserDat, SET_MY_PRIORITY);
	}
	else
	{
		myProcessInfo.Process_ID = GetCurrentThreadId();
		status =  IntEnable (MYDRIVER_IntHandlerRoutine, &myProcessInfo, SET_MY_PRIORITY);
	}

	if(status != TRUE)
	{
		UserStatus = (UserStatus | FailedEnableIntr);
		return;
	}

	// Interrupts auf der PCI- Mil- Karte Freischalten
	//
	SetCntrlReg1 (Cntrl_En_Intr1, Status);
}


// Interrupts ausschalten
//
void MilDriver::InterruptDisable(DWORD &UserStatus)
{
	DWORD Status;
	
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(CardInfo.myHandle == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	// Interrupts auf der PCI- Mil- Karte abschalten
	//
	SetCntrlReg1 (Cntrl_Dis_Intr1, Status);

    IntDisable ();
}

// Interruptsauf dem PLX PCI 9050-1 zulassen
//
void MilDriver::InterPLXEnable(DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(CardInfo.myHandle == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	Write_INTCSR (Inter_PLX_Active);
}


void MilDriver::InterruptWait(DWORD &IntAktiv, DWORD &UserStatus)
{
	DWORD status = StatusOK;
	DWORD data = 0;

	MSG *myMSG;
	myMSG = new MSG;

	// Interrupts freigeben
	//
	InterPLXEnable(status);
	if(status != StatusOK)
	{
		UserStatus = (UserStatus | status);
		return;
	}	

	//Wartet auf message (interrupt) und prüft 
	while (myMSG->message != myProcessInfo.InterruptID)
	{
		GetMessage(myMSG, 0, myProcessInfo.InterruptID, myProcessInfo.InterruptID); 
	}

	// Interrupt auswerten welcher wars 
	//
	ReadInterruptMask(data, status);

	if(status != StatusOK)
	{
		UserStatus = (UserStatus | status);
		return;
	}
	
	IntAktiv = data >> 16;
	
	delete myMSG;
}

// Interrupt auswerten
void MilDriver::Int_Aktiv(DWORD &IntAktiv, DWORD &UserStatus)
{
	DWORD status = StatusOK;
	DWORD data = 0;

	// Interrupt auswerten welcher wars 
	//
	ReadInterruptMask(data, status);

	if(status != StatusOK)
	{
		UserStatus = (UserStatus | status);
		return;
	}
	
	IntAktiv = data >> 16;
}

// Simuliert eine Interrupt, damit die InterruptWait Routine
// abgebrochen werden kann
void MilDriver::StopInterruptWait(DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(CardInfo.myHandle == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}
	
	// Prüft, ob Interrupt enabled wurde
	//
	if(myProcessInfo.Process_ID == NULL)
	{
		UserStatus = (UserStatus | FailedEnableIntr);
		return;		
	}

	PostThreadMessage(myProcessInfo.Process_ID, myProcessInfo.InterruptID,0,0);
}*/


void MilDriver::Puls1_Start_Soft (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS1_START);
}


void MilDriver::Puls1_Stop_Soft  (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS1_STOP);
}


void MilDriver::Puls2_Start_Soft (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS2_START);
}


void MilDriver::Puls2_Stop_Soft  (DWORD &UserStatus) 
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS2_STOP);
}


void MilDriver::Puls3_Start_Soft (DWORD &UserStatus)
{
		// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS3_START);
}

void MilDriver::Puls3_Stop_Soft  (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS3_STOP);
}

void MilDriver::Puls4_Start_Soft (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS4_START);
}

void MilDriver::Puls4_Stop_Soft  (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS4_STOP);
}

void MilDriver::Puls5_Start_Soft (DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS5_START);
}

void MilDriver::Puls5_Stop_Soft  (DWORD &UserStatus) 
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(USBData.MYUSB_HANDLE == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	WriteUSB_Device(0, USB_PROT_PULS5_STOP);
}


// Schreibt 

/*void MilDriver::INTCSR_Write (DWORD data, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(CardInfo.myHandle == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	MYDRIVER_WriteINTCSR (CardInfo.myHandle, data);
}

void MilDriver::INTCSR_Read (DWORD &data, DWORD &UserStatus)
{
	// Prüfen, ob die PCI Karte geöffnet worden ist
	//
	if(CardInfo.myHandle == NULL)
	{
		UserStatus = (UserStatus | NoPCMil);
		return;
	}

	MYDRIVER_ReadINTCSR (CardInfo.myHandle);
}*/
