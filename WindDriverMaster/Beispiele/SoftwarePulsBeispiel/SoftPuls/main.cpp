//---------------------------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// Beispielprogramm fuer das erzeugen
// einens rahmen puls auf buchse1 & 2 
// ohne event
//
// nicht vergessen :
// PCIMilTreiber.lib im Projekt
// einbinden !
//
// Autor : Zweig,Marcus
// Datum : 11.12.2003
//
// VC++
//------------------

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
	

	// rahmen puls auf buchse 1 erwuenscht
	PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls1_Rahmen_On ,ErrorStatus);

	// rahmen puls auf buchse 2 erwuenscht
	PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls2_Rahmen_On ,ErrorStatus);


	// der puls soll erst auf buchse 1 1000us anstehen, dann ausgehen
	// und  400us spaeter auf buchse 2 nochmals 1000us anstehen

	// puls auf buchse 1 starten
	PCI_SoftPuls1Start(Cardauswahl, ErrorStatus);
	// 1000us warten
	PCI_TimerWait(Cardauswahl, 1000, 1, ErrorStatus);
	//  puls auf buchse 1 abschalten
	PCI_SoftPuls1Stop(Cardauswahl, ErrorStatus);

	// 400us warten
	PCI_TimerWait(Cardauswahl, 400, 1, ErrorStatus);
	// puls auf buchse 2 starten
	PCI_SoftPuls2Start(Cardauswahl, ErrorStatus);
	// 1000us warten
	PCI_TimerWait(Cardauswahl, 1000, 1, ErrorStatus);
	// puls auf buchse 2 abschalten
	PCI_SoftPuls2Stop(Cardauswahl, ErrorStatus);

	//--- das wars ----

	// PCI- Mil- Karte freigeben
	PCI_DriverClose(Cardauswahl);
}