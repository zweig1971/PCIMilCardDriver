//---------------------------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// Beispielprogramm fuer eine
// interrupt behandlung auf
// event Nr. 1
//
// nicht vergessen :
// PCIMilTreiber.lib im Projekt
// einbinden !
//
// Autor : Zweig,Marcus
// Datum : 11.12.2003
//
// VC++
//--------------------------

#include "PCIMilTreiber.h"
#include <iostream.h>
#include <stdio.h>


void main()
{
	DWORD ErrorStatus = 0;
	DWORD InMask	  = 0;
	DWORD IntAktiv	  = 0;
	WORD CardCounter  = 0;
	DWORD Cardauswahl = 0;
	WORD status		  = 0;
	WORD MaxLoop	  = 100;
	WORD counter	  = 0;
	WORD EventCodeNr  = 0;

	BYTE  ErrorChar [12];

	bool EVFifoNotEmpty;

// ------------------ PCI MilBus Karten zählen ----------------
	CardCounter = PCI_PCIcardCount(); 
	cout << "PCIMilBusCard on board :" << CardCounter << endl;

// ------------------- Karte aussuchen und anwählen ------------
	if(CardCounter == 0)
	{
		cout<< "\nNo Card(s) on Board ! " <<endl;
		cout<< "----------------------" <<endl;
		cout<< "Press Enter to Exit....";
		getchar();
		return;
	}
	else if (CardCounter > 1)// Gibt es mehr wie eine -> auswaehlen
	{
		do
		{
			cout << "Select Card (1-" << CardCounter <<") :";
			cin >> Cardauswahl;
		}while((Cardauswahl > CardCounter)||(Cardauswahl <= 0));
		
		status =PCI_DriverOpen(Cardauswahl);
	}
	else // gibt es nur eine -> keine auswahl anbieten
	{
		status = PCI_DriverOpen(1);
		Cardauswahl = 1;
	}

	// hat alles geklappt
	PCI_ErrorString (status, ErrorChar);
	cout<<"ConnectMilBus :"<<ErrorChar<<endl;

	if(status != StatusOK) // nein ? -> tschuess
	{
		cout<< "\nPress Enter to Exit....";
		getchar();
		getchar();
		return;
	}
	
	// erst mal PCI-Mil-Karte reseten
	PCI_PCIcardReset(Cardauswahl, ErrorStatus);

	// Filter einschalten
	PCI_CntrlRegSet(Cardauswahl, Cntrl_EvFilter_On, ErrorStatus);

	// Aufraeumen : alle Event settings loeschen
	PCI_EvFilterClear(Cardauswahl, ErrorStatus);

	// Aufraeumen : Event- FiFo loeschen
	PCI_EvFiFioClear(Cardauswahl, ErrorStatus);

	// Uns interessiert nur Event 1 
	// kommen dieses an, so soll es  
	// im Event- FiFo reingeschrieben werden
	PCI_EvFilterSet(Cardauswahl, 1, EventOn, ErrorStatus);

	// Interrupt Maske definieren, und zwar wollen wir wissen
	// ob event 1 angekommen ist, das koennen wir aber nicht 
	// direkt pruefen, deshalb haben wir den filter so 
	// eingestellt, das nur event 1 durchkommen kann 
	// (abgesehen von den events >= C8hex )
	// daher verknuepfen wir den interrupt mit dem
	// bit Evt_Fifo_Not_Empty. Das ist das 6te bit in
	// der interrupt-maske (siehe UsersGuide) also :
	// 1000000 -> 0x40 (hex)
	
	InMask = 0x40;

	PCI_IntMaskWrite(Cardauswahl, InMask, ErrorStatus);
	
	// Interrupts enabeln ohne eine callback-funktion
	status = PCI_IntEnable (Cardauswahl, NULL, NULL , ErrorStatus);

	if(status != StatusOK)
	{
		PCI_ErrorString (status, ErrorChar);
		cout<<ErrorChar<<endl<<endl;

		// im fehlerfall aufraeumen
		PCI_EvFilterSet(Cardauswahl, 1, EventClear, ErrorStatus);

		// ende
		PCI_DriverClose(Cardauswahl);

		getchar();
		getchar();
		return;
	}

	cout<<"\nWarte auf interrupt....."<<endl;

	do
	{
		EventCodeNr = 0;
		PCI_IntWait(Cardauswahl, IntAktiv, ErrorStatus);
		PCI_EvFiFoRead(Cardauswahl, EventCodeNr, EVFifoNotEmpty, ErrorStatus);

	}while ((IntAktiv != Evt_Fifo_Not_Empty) && (EventCodeNr != 1));

	// Aufraeumen : Event- FiFo loeschen
	PCI_EvFiFioClear(Cardauswahl, ErrorStatus);

	PCI_EvFilterSet(Cardauswahl, 1, EventClear, ErrorStatus);

	// WICHTIG !!!
	PCI_IntDisable(Cardauswahl, ErrorStatus);

	// ende
	PCI_DriverClose(Cardauswahl);

	cout<<"\nEvent gekommen :"<<EventCodeNr<<endl;
	cout<<"\n\nPress Enter to exit...."<<endl;
	getchar();
}