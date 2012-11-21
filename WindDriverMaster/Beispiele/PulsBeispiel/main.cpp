//---------------------------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// Beispielprogramm fuer
// puls an buchse 
// 1 & 2 auf event 1 
// (Beispiel)
//
// nicht vergessen :
// PCIMilTreiber.lib im Projekt
// einbinden !
//
// Autor : Zweig,Marcus
// Datum : 11.12.2003
//
// VC++
//------------------------,

#include "PCIMilTreiber.h"
#include <iostream.h>
#include <stdio.h>


void main()
{
	DWORD ErrorStatus = 0;
	DWORD CardCounter = 0;
	DWORD Cardauswahl = 0;
	DWORD status	  = 0;
	WORD MaxLoop	  = 100;
	WORD counter	  = 0;
	WORD EventCodeNr  = 0;

	BYTE ErrorChar [12];

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
	// kommen dieses an, so sollen sie im Event- FiFo 
	// reingeschrieben werden
	PCI_EvFilterSet(Cardauswahl, 1, EventOn, ErrorStatus);

	// Kommt Event 1 an, soll ein Puls an Buchse 1 & 2
	// anliegen
	PCI_PulsOutSet(Cardauswahl, PulsBuchse_1, 1, 0, Puls, true, ErrorStatus);
	PCI_PulsOutSet(Cardauswahl, PulsBuchse_2, 1, 0, Puls, true, ErrorStatus);

	// Das wars eigentlich, ab jetzt Pulst die PCI-Mil- Karte
	// fleissig vor sich hin, jedesmal wenn Event 1 vobeischaut
	// Nur das Event 1 wird in das Event- FiFo uebernommen

	do
	{
		// Event FiFo auslesen
		PCI_EvFiFoRead(Cardauswahl, EventCodeNr, EVFifoNotEmpty, ErrorStatus);

		// Ist was fuer mich drin ?
		if(EventCodeNr == 1)
		{
			counter++;
			cout<<"Ein Rahmen- Puls Event : "<<EventCodeNr<<endl;
			EventCodeNr = 0;
		}

	}while (counter != MaxLoop);


	// Fertig, Pulse wieder auschalten
	PCI_PulsOutSet(Cardauswahl, PulsBuchse_1, 1, 0, Puls, false, ErrorStatus);
	PCI_PulsOutSet(Cardauswahl, PulsBuchse_2, 1, 0, Puls, false, ErrorStatus);

	// PCI- Mil- Karte freigeben
	PCI_DriverClose(Cardauswahl);
}