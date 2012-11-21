//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// MilDriver
//
// Autor           : Zweig,Marcus
// Version         : 1.09
// letzte Änderung : 27.10.06
//------------------------------


#include "main.h"

MilDriver *MilDriverArray[maxPCIMilKarten];

static bool DriverArrayEmpty;

#ifndef PCIMilTreiber_Slave

EXPORT_TYPE void CALL_TYPE MILDRIVER_Version_0p110_271006(void)
{
}

EXPORT_TYPE void CALL_TYPE MILDRIVER_VersionNr(BYTE* version)
{
	string StringVersion = "Version 1.10";

	for(int ii=0; ii<StringVersion.size();ii++)
	{
		version[ii] = StringVersion[ii];
	}
	
	version[StringVersion.size()] = '\0';
}
#endif


#ifdef PCIMilTreiber_Slave

EXPORT_TYPE void CALL_TYPE MILDRIVER_Version_0p05S_271006(void)
{
}

EXPORT_TYPE void CALL_TYPE MILDRIVER_VersionNr(BYTE* version)
{
	string StringVersion = "Version 0.05S";

	for(int ii=0; ii<StringVersion.size();ii++)
	{
		version[ii] = StringVersion[ii];
	}
	
	version[StringVersion.size()] = '\0';
}
#endif

// Einsprungspunkt der dll. Beim erzeugen der dll wird das array mit
// NULL init. Beim unload werden alle noch aktiven objekte im array
// gesucht und beendet. Die priority des processes wird auf hoch
// gesetzt
//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	int index = 0;
	
	// Die dll wurde aufgerufen -> MilDriverArray auf NULL setzen
	//
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		while(index != maxPCIMilKarten)
		{
			MilDriverArray[index] = NULL;
			index++;
		}
		DriverArrayEmpty = true;
	}
	// die dll wurde beendet -> alle noch vorhandene Objecte in
	// MilDriverArray löschen
	else if (ul_reason_for_call ==  DLL_PROCESS_DETACH)
	{
		index = 0;
		if(DriverArrayEmpty != true)
		{
			while(index != maxPCIMilKarten)
			{
				if (MilDriverArray[index] != NULL)
				{
					delete MilDriverArray[index];
					MilDriverArray[index] = NULL;
				}
				index++;
			}// while(index != maxPCIMilKarten)
			DriverArrayEmpty = true;
		}//if(DriverArrayEmpty != true)
	}
    return TRUE;
}


// Zaehlt die Karten im PC 
//
EXPORT_TYPE DWORD CALL_TYPE PCI_PCIcardCount(void)
{
	int findCards = 0;
	CountPCCards(findCards);
	return(findCards);
}

//Erhöht/erniedrigt die priorität des prozesses
EXPORT_TYPE bool CALL_TYPE PCI_PrioritySet(WORD PriorityLevel)
{
	bool status;
	status=SetPriority(int(PriorityLevel));
	return status;
}


// PCI Karte allokieren
//
EXPORT_TYPE DWORD CALL_TYPE PCI_DriverOpen(BYTE CardNr)
{
	int status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten))
	{
		return(CardNrError);	
	}

	// Object erzeugen und ueber die CardNr im Array
	// speichern
	if (MilDriverArray[CardNr] == NULL)
	{
		MilDriverArray[CardNr] = new MilDriver;
		DriverArrayEmpty = false;
	}
	else
	{
		delete MilDriverArray[CardNr];
		MilDriverArray[CardNr] = new MilDriver;
		DriverArrayEmpty = false;
	}

	// Object mit PCI Karte verknuepfen
	status=MilDriverArray[CardNr]->ConnectMilBus(CardNr);

	// Im Fehlerfall Object wieder loeschen
	if (status != StatusOK) 
	{
		delete MilDriverArray[CardNr];
		MilDriverArray[CardNr] = NULL;
	}

	return(status);
}

// Prüft ob eine bestimmte IFK Online is oder nicht
//
#ifndef PCIMilTreiber_Slave

EXPORT_TYPE DWORD CALL_TYPE PCI_IfkOnline(BYTE CardNr, BYTE IFCNr, BYTE &ReturnIFKNR ,DWORD &ErrorStatus)
{
	DWORD Status = StatusOK;
	bool found;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
			return(NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}
	
	found=MilDriverArray[CardNr]->IFKAlive(IFCNr, ReturnIFKNR, Status);

	if (found == true)
	{
		return(StatusOK);
	}
	else
	{
		ErrorStatus = (ErrorStatus | Status);
		return(Status);
	}
}
#endif

// Liest daten von der IFK
//
#ifndef PCIMilTreiber_Slave

EXPORT_TYPE DWORD CALL_TYPE PCI_IfkRead(BYTE CardNr, BYTE IFKAdress ,BYTE IFKFunktionCode, WORD &data, DWORD &ErrorStatus)
{
	DWORD status    = StatusOK;
	DWORD MyStatus  = StatusOK;
	DWORD ErrorStat = StatusOK;
	WORD DummyData;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
			return(NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}

	//Ist das FiFo ausgeschaltet, liegt noch ein datum an -> lesen und verwerfen
	if(!(PCI_StatusTest(CardNr, Mil_Fifo_On, ErrorStat)))
	{
		// Liegt ein resive ready an -> datum lesen
		if(PCI_StatusTest(CardNr, Mil_Rcv_Rdy, ErrorStat))
		{
			// Datum lesen und vergessen
			MilDriverArray[CardNr]->ReadData(DummyData, status); 
			ErrorStatus = (ErrorStatus | UnrequestedReceiv);
			MyStatus    = UnrequestedReceiv;
		}
	}

	// Lesen
	status = StatusOK;
	MilDriverArray[CardNr]->ReadDataFromIFK(IFKFunktionCode,IFKAdress, data, status);

	// Fehler beim lesen oder war ein ungueltiges datum im speicher
	if((status != StatusOK) || (MyStatus != StatusOK))
	{
		if(status != StatusOK)
		{
			ErrorStatus = (ErrorStatus | status );
			return(status);
		}
		else
		{
			return(MyStatus);
		}
	}
	return(StatusOK);
}
#endif

// Sendet daten zu einer IFK
//
#ifndef PCIMilTreiber_Slave

EXPORT_TYPE DWORD CALL_TYPE PCI_IfkWrite(BYTE CardNr, BYTE IFKAdress, BYTE IFKFunktionCode, WORD data, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
			return(NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]->WriteDataToIFK (IFKFunktionCode, IFKAdress, data, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status );
		return(status);
	}
	return(StatusOK);
}
#endif

// Sendet einen Fkn-Code zu einer IFK
//
#ifndef PCIMilTreiber_Slave

EXPORT_TYPE DWORD CALL_TYPE PCI_FctCodeSnd(BYTE CardNr, BYTE IFKAdress, BYTE FunktionCode, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
			return(NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]->SendFknCodeToIFK(FunktionCode, IFKAdress, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return (status);
	}
	
	return(StatusOK);
}
#endif

// Resetet die PCI-Karte
//
EXPORT_TYPE DWORD CALL_TYPE PCI_PCIcardReset(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
			return(NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]->ResetPCICard(status);
	
	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	return(StatusOK);
}


// Gibt den FehlerCode als Klartext in einem string
// zurück
EXPORT_TYPE DWORD CALL_TYPE PCI_ErrorString (DWORD status, BYTE* error_string)
{
	string ErrorMessage;
	ErrorMessage = PrintErrorMessage(status);

	for(int ii=0; ii<ErrorMessage.size();ii++)
	{
		error_string[ii] = ErrorMessage[ii];
	}
	
	error_string[ErrorMessage.size()] = '\0';

	return (0);
}

// Setzt und wartet auf den Timer in der PC-Mil-Karte
//        
EXPORT_TYPE DWORD CALL_TYPE PCI_TimerWait (BYTE CardNr, WORD Time, BYTE TimeBase, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  


	if(TimeBase == 0)
	{
		MilDriverArray[CardNr]->WaitOfTimer2 (Time, Timerbase_10us, status);
	}
	else if(TimeBase == 1)
	{
		MilDriverArray[CardNr]->WaitOfTimer2 (Time, Timerbase_1ms, status);
	}

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	return(StatusOK);
}

// Setzt den Timer in der PC-Mil-Karte 
// auf gewünschten Wert und wartet NICHT !
EXPORT_TYPE DWORD CALL_TYPE PCI_TimerSet (BYTE CardNr, WORD Time, BYTE TimeBase, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

	if(TimeBase == 0)
	{
		MilDriverArray[CardNr]->SetTimer2 (Time, Timerbase_10us, status);
	}
	else if(TimeBase == 1)
	{
		MilDriverArray[CardNr]->SetTimer2 (Time, Timerbase_1ms, status);
	}

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	return(StatusOK);
	
}

// Liest den Status der PC-Mil-Karte aus und gibt diesen
// zurück
EXPORT_TYPE DWORD CALL_TYPE PCI_StatusRegRead (BYTE CardNr, DWORD &StatusReg, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]->ReadStatusReg(StatusReg, status);
	
	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	return(StatusOK);
}

// Schreibt Daten auf den MilBus 
//
EXPORT_TYPE DWORD CALL_TYPE PCI_MilBusWrite (BYTE CardNr, WORD data, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]->WriteData(data, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	return(StatusOK);
}

// Schreibt Komandowort auf den Bus
//
EXPORT_TYPE DWORD CALL_TYPE PCI_MilBusCMDWrite (BYTE CardNr, WORD cmd, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]->WriteCMD (cmd, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	return(StatusOK);
}

// Einlesen der Interrupt Maske
//
EXPORT_TYPE DWORD CALL_TYPE PCI_IntMaskRead (BYTE CardNr, DWORD &data, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]->ReadInterruptMask(data, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

// Schreiben der Interrupt Maske
//
EXPORT_TYPE DWORD CALL_TYPE PCI_IntMaskWrite(BYTE CardNr, DWORD data , DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]->WriteInterruptMask(data, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}


// Interrupts Freischalten
//
EXPORT_TYPE DWORD CALL_TYPE PCI_IntEnable(BYTE CardNr, MYDRIVER_INT_HANDLER funcIntHandler, PVOID pUserDat, WORD Priority, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	WORD SET_MY_PRIORITY;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  
	
	// Thread priority festlegen
	if(Priority == IntSetPriorityHigh)
	{
		SET_MY_PRIORITY = THREAD_PRIORITY_HIGHEST;
	}
	else if(Priority == IntSetPriorityCritical)
	{
		SET_MY_PRIORITY = THREAD_PRIORITY_TIME_CRITICAL;
	}
	else if(Priority == IntSetPriorityNormal)
	{
		SET_MY_PRIORITY = THREAD_PRIORITY_NORMAL;
	}
	else
	{
		return(FailedSetPriority);
	}

	MilDriverArray[CardNr]->InterruptEnable(status, funcIntHandler, pUserDat, SET_MY_PRIORITY);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	MilDriverArray[CardNr]->InterPLXEnable(status);
	
	return(StatusOK);
}

// Interrupt Abschalten
//
EXPORT_TYPE DWORD CALL_TYPE PCI_IntDisable(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  

//	MilDriverArray[CardNr]->InterPLXEnable(status);

	MilDriverArray[CardNr]->InterruptDisable(status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

// Wartet auf Interrupt

EXPORT_TYPE DWORD CALL_TYPE PCI_IntWait(BYTE CardNr, DWORD &IntAktiv, DWORD &ErrorStatus)
{
	DWORD status	  = StatusOK;
	DWORD data		  = 0;
	BYTE EVValue	  = 0;
	BYTE CntrlReg	  = 0;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  	

	MilDriverArray[CardNr]->InterruptWait(IntAktiv, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

// Welcher interrupt war es
//
EXPORT_TYPE DWORD CALL_TYPE PCI_IntAktiv(BYTE CardNr, DWORD &IntAktiv, DWORD &ErrorStatus)
{
	DWORD status	  = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  	

	MilDriverArray[CardNr]->Int_Aktiv(IntAktiv, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}


EXPORT_TYPE DWORD CALL_TYPE PCI_IntWaitStop(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status	  = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  	

	MilDriverArray[CardNr]->StopInterruptWait(status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}


// Setzt das Control Register 
//
EXPORT_TYPE DWORD CALL_TYPE PCI_CntrlRegSet(BYTE CardNr, BYTE SetValue, DWORD &ErrorStatus)
{
	DWORD status	  = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(CardNrError);
		}
	}  	

	MilDriverArray[CardNr]-> SetCntrlReg1 (SetValue, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}


// Liest das Control- Register aus
//
EXPORT_TYPE DWORD CALL_TYPE PCI_CntrlRegRead(BYTE CardNr, BYTE &CntrlRegValue, DWORD &ErrorStatus)
{
	DWORD status	 = StatusOK;
	WORD EventCodeNr = 1;
	BYTE EVValue	 = 0;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]-> ReadCntrlReg1(CntrlRegValue, status);
	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}


// Setzen den Event- Filters
//
EXPORT_TYPE DWORD CALL_TYPE PCI_EvFilterSet(BYTE CardNr, WORD EventCodeNr, BYTE SetEV, DWORD &ErrorStatus) 
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]-> SetEVFilter (EventCodeNr, SetEV, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

// Liest events die es durch den filter geschafft haben aus dem fifo
//
EXPORT_TYPE DWORD CALL_TYPE PCI_EvFiFoRead (BYTE CardNr, WORD &EventCodeNr, bool &EVFifoNotEmpty, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]->ReadEVFromFiFo (EventCodeNr, status, EVFifoNotEmpty); 

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}


// gibt den zustand des filters fuer ein ausgesuchtes event
// zurueck
EXPORT_TYPE DWORD CALL_TYPE PCI_EvFilterRead(BYTE CardNr, WORD EventCodeNr, BYTE &FilterValue, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]-> ReadFromEVFilter(EventCodeNr, FilterValue, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}


// Testet das StatusRegister der Karte mit dem vom
// Anwender angegebenen StatusBit. Gibt enstsprechend
// true oder false mit
EXPORT_TYPE bool CALL_TYPE PCI_StatusTest(BYTE CardNr, DWORD StatusBit, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	bool Result;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			ErrorStatus = (ErrorStatus | NoPCICartOpen);
			return(false);
		}
		else
		{
			ErrorStatus = (ErrorStatus | CardNrError);
			return(false);
		}
	}

	Result = MilDriverArray[CardNr]-> TestCardStatus (StatusBit, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(false);
	}
	
	return(Result);	
}

// Liest Daten vom FiFo/Bus
//
EXPORT_TYPE DWORD CALL_TYPE PCI_MilBusRead (BYTE CardNr, WORD &data, bool &Fifo_not_empty, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	DWORD ErrorStat = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	// Vom Daten-FiFo oder vom Bus
	if(PCI_StatusTest(CardNr, Mil_Fifo_On, ErrorStat))
	{
		MilDriverArray[CardNr]->ReadDataFromFiFo (data, status, Fifo_not_empty); 
	}
	else
	{
		MilDriverArray[CardNr]->ReadData(data, status); 
	}


	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_EvTimerRead(BYTE CardNr, DWORD &Time ,DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	MilDriverArray[CardNr]-> ReadEventTimer (Time, status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}


EXPORT_TYPE DWORD CALL_TYPE PCI_PulsOutSet(BYTE CardNr, WORD PulsBuchse, WORD EventOn, WORD EventOff, WORD PulsRahmen, bool OnOff, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	// Event Werte ueberpruefen
	// EventCode prüfen
	//
	if ((EventOn > 4095) || (EventOn < 0))
	{
		ErrorStatus = (ErrorStatus | EventNrError);
		return(EventNrError);	
	}

	// Buchsen auswahl pruefen
	//
	if((PulsBuchse != PulsBuchse_1) && (PulsBuchse != PulsBuchse_2)) 
	{
		ErrorStatus = (ErrorStatus | GeneralError);
		return(GeneralError);	
	}

// -------------------- hier gehts los -------------------------

	// Puls erwuenscht oder nicht ?
	if(OnOff == true)
	{
		// Welche Buchse soll gepulst werden ?
		if(PulsBuchse == PulsBuchse_1)
		{
			//Ein- Event mit Buchse 1 verknuepfen
			MilDriverArray[CardNr]-> SetEVFilter (EventOn, Puls1_On, status);
			if(status != StatusOK)
			{
				ErrorStatus = (ErrorStatus | status);
				return(status);
			}

			// Rahmen fuer Buchse 1 auschalten
			MilDriverArray[CardNr]-> SetCntrlReg1 (Cntrl_Puls1_Rahmen_Off, status);
			if(status != StatusOK)
			{
				ErrorStatus = (ErrorStatus | status);
				return(status);
			}
		}
		else
		{
			//Ein- Event mit Buchse 2 verknuepfen
			MilDriverArray[CardNr]-> SetEVFilter (EventOn, Puls2_On, status);
			if(status != StatusOK)
			{
				ErrorStatus = (ErrorStatus | status);
				return(status);
			}

			// Rahmen fuer Buchse 2 auschalten
			MilDriverArray[CardNr]-> SetCntrlReg1 (Cntrl_Puls2_Rahmen_Off, status);
			if(status != StatusOK)
			{
				ErrorStatus = (ErrorStatus | status);
				return(status);
			}
		}// Ende if(PulsBuchse == PulsBuchse_1)

		// Puls oder Rahmenpuls
		if(PulsRahmen == RahmenPuls)
		{
			if(PulsBuchse == PulsBuchse_1)
			{
			    //Aus- Event mit Buchse 1 verknuepfen
				MilDriverArray[CardNr]-> SetEVFilter (EventOff, Puls1_Off, status);
				if(status != StatusOK)
				{
					ErrorStatus = (ErrorStatus | status);
					return(status);
				}

				// Rahmen fuer Buchse 1 einschalten
				MilDriverArray[CardNr]-> SetCntrlReg1 (Cntrl_Puls1_Rahmen_On, status);
				if(status != StatusOK)
				{
					ErrorStatus = (ErrorStatus | status);
					return(status);
				}
			}
			else
			{
				//Aus- Event mit Buchse 2 verknuepfen
				MilDriverArray[CardNr]-> SetEVFilter (EventOff, Puls2_Off, status);
				if(status != StatusOK)
				{
					ErrorStatus = (ErrorStatus | status);
					return(status);
				}

				// Rahmen fuer Buchse 2 einschalten
				MilDriverArray[CardNr]-> SetCntrlReg1 (Cntrl_Puls2_Rahmen_On, status);
				if(status != StatusOK)
				{
					ErrorStatus = (ErrorStatus | status);
					return(status);
				}
			}//if(PulsBuchse == PulsBuchse_1)
		}//if(PulsRahmen == RahmenPuls)
	}
	else
	{	
		if(PulsBuchse == PulsBuchse_1)
		{
			//Ein- Event mit Buchse 1 löschen
			MilDriverArray[CardNr]-> SetEVFilter (EventOn, Puls1_Off, status);
			if(status != StatusOK)
			{
				ErrorStatus = (ErrorStatus | status);
				return(status);
			}
		}
		else
		{
			//Ein- Event mit Buchse 2 löschen
			MilDriverArray[CardNr]-> SetEVFilter (EventOn, Puls2_Off, status);
			if(status != StatusOK)
			{
				ErrorStatus = (ErrorStatus | status);
				return(status);
			}
		}//if(PulsBuchse == PulsBuchse_1)
	}//if(OnOff == true)

	return(StatusOK);
}

// Löscht alle settings im eventfilter
//
EXPORT_TYPE DWORD CALL_TYPE PCI_EvFilterClear(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}
	
	MilDriverArray[CardNr]-> ClearEvFilter(status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}

//Löscht alle events im fifo
//
EXPORT_TYPE DWORD CALL_TYPE PCI_EvFiFioClear(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}
	
	MilDriverArray[CardNr]-> ClearEvFiFio(status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);

}

// Generiert per Software einen Puls
//
EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls1Start(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls1_Start_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}
	
	return(StatusOK);
}

// Generiert per Software einen Puls
//
EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls1Stop (BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls1_Stop_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

// Generiert per Software einen Puls
//
EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls2Start(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls2_Start_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

// Generiert per Software einen Puls
//
EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls2Stop (BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls2_Stop_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls3Start(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls3_Start_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls3Stop (BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls3_Stop_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls4Start(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls4_Start_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls4Stop (BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls4_Stop_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls5Start(BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls5_Start_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}

EXPORT_TYPE DWORD CALL_TYPE PCI_SoftPuls5Stop (BYTE CardNr, DWORD &ErrorStatus)
{
	DWORD status = StatusOK;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]-> Puls5_Stop_Soft (status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}


// Naechsten Interrupt bearbeiten
//
EXPORT_TYPE DWORD CALL_TYPE PCI_IntNextEnable(BYTE CardNr, DWORD &ErrorStatus)
{

	DWORD status = StatusOK;

	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}

	MilDriverArray[CardNr]->InterPLXEnable(status);

	if(status != StatusOK)
	{
		ErrorStatus = (ErrorStatus | status);
		return(status);
	}

	return(StatusOK);
}


// Gibt die PCI-Karte wieder frei und loescht das object
//
EXPORT_TYPE DWORD CALL_TYPE PCI_DriverClose(BYTE CardNr)
{
	int index = 0;
	int emptyIndex = 0;
	
	// Parameter vom User pruefen
	if ((CardNr <= 0) || (CardNr > maxPCIMilKarten) || (MilDriverArray[CardNr] == NULL))
	{
		if(DriverArrayEmpty == true)
		{
			return(NoPCICartOpen);
		}
		else
		{
			return(CardNrError);
		}
	}  

	// Object löschen und array platz freigeben
	delete MilDriverArray[CardNr];
	MilDriverArray[CardNr] = NULL;

	// Prüfen ob noch Objecte vorhanden sind
	while(index != maxPCIMilKarten)
	{
		if(MilDriverArray[index] == NULL)
		{
			emptyIndex++;
		}
		index++;
	}

	if (emptyIndex >= maxPCIMilKarten)
	{
		DriverArrayEmpty = true;
	}

	return(StatusOK);
}