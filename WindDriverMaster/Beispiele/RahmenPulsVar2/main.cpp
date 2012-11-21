//---------------------------------------------
// GSI Gesellschaft f�r 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// Beispielprogramm fuer rahmen puls an buchse 
// 1 & 2 auf event 1 & 2 OHNE PCI_PulsOutSet 
// diese varante beinhaltet mehr aufrufe als
// die varante mit PCI_PulsOutSet. Dafuer ist
// diese hier flexibler, z.B. kann man an buchse 1
// einen rahmen- puls fahren waehrend buchse 2 pulst
// etc.
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
	DWORD CardCounter = 0;
	DWORD Cardauswahl = 0;
	DWORD status	  = 0;
	WORD MaxLoop	  = 100;
	WORD counter	  = 0;
	WORD EventCodeNr  = 0;

	BYTE ErrorChar [12];

	bool EVFifoNotEmpty;

// ------------------ PCI MilBus Karten z�hlen ----------------
	CardCounter = PCI_PCIcardCount(); 
	cout << "PCIMilBusCard on board :" << CardCounter << endl;

// ------------------- Karte aussuchen und anw�hlen ------------
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
	
	// Uns interessiert nur Event 1 & 2
	// kommen diese an, so sollen sie im Event- FiFo 
	// reingeschrieben werden
	PCI_EvFilterSet(Cardauswahl, 1, EventOn, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 2, EventOn, ErrorStatus);

	// Bei Event Nr.1 soll der Rahmen- Puls an Buchse 1 & 2
	// angehen
	PCI_EvFilterSet(Cardauswahl, 1, Puls1_On, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 1, Puls2_On, ErrorStatus);

	// Bei Event Nr.2 soll der Rahmen- Puls an Buchse 1 & 2
	// ausgehen
	PCI_EvFilterSet(Cardauswahl, 2, Puls1_Off, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 2, Puls2_Off, ErrorStatus);

	// Rahmen- Puls auf Buchse 1 & 2 ist erwuenscht 
	PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls1_Rahmen_On, ErrorStatus);
	PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls2_Rahmen_On, ErrorStatus);

	// Das wars eigentlich, ab jetzt faehrt die PCI- Mil- Karte den Rahmen 
	// bei Event Nr.1 Buchse 1 & 2 hoch, und bei Event Nr.2 wieder runter
	// so lange wie dieser Rechner- Prozess aktiv ist.
	// Nur Event 1 & 2 landen im Event- FiFo. 

	do
	{
		// Event FiFo auslesen
		PCI_EvFiFoRead(Cardauswahl, EventCodeNr, EVFifoNotEmpty, ErrorStatus);

		// Ist was fuer mich drin ?
		if((EventCodeNr == 1) || (EventCodeNr == 2))
		{
			counter++;
			cout<<"Ein Rahmen- Puls Event : "<<EventCodeNr<<endl;
			EventCodeNr = 0;
		}

	}while (counter != MaxLoop);


	// Fertig, Pulse wieder auschalten
	PCI_EvFilterSet(Cardauswahl, 1, Puls1_Off, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 1, Puls2_Off, ErrorStatus);

	// die Events interessieren nicht mehr
	PCI_EvFilterSet(Cardauswahl, 1, EventOff, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 2, EventOff, ErrorStatus);

	// Aufraeumen : Event- FiFo loeschen
	PCI_EvFiFioClear(Cardauswahl, ErrorStatus);

	// PCI- Mil- Karte freigeben
	PCI_DriverClose(Cardauswahl);
}