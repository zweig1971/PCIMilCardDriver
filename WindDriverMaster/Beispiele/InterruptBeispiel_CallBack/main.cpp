//---------------------------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// Beispielprogramm fuer
// puls an buchse 
// 1 & 2 auf event 1 
//
// nicht vergessen :
// PCIMilTreiber.lib im Projekt
// einbinden !
// Autor : Zweig,Marcus
// Datum : 11.12.2003
//
// VC++
//--------------------------

#include "PCIMilTreiber.h"
#include <iostream.h>
#include <stdio.h>

// Die daten-strukture fuer die 
// callback-funktion
struct IntStructure {
	DWORD IntCounter;
	DWORD IntAktiv;
	DWORD Cardauswahl;		
	bool IntResive;
};

// Die callback-fuktion. Hier sollte der
// Code stehen der bei einem interrupt 
// ausgefuehrt werden sollte.
void MY_InterruptHandler(void *p)
{
	DWORD status= 0;
	DWORD ErrorStatus = 0;

	IntStructure *p1 = (IntStructure*)p;

	// Welcher interrupt wars ?
	PCI_IntAktiv(p1->Cardauswahl, p1->IntAktiv, ErrorStatus);

	// Daten in die structur uebertragen
	p1->IntCounter = p1->IntCounter + 1;
	p1->IntResive = true;

	// Aufraeumen : Event- FiFo loeschen
	PCI_EvFiFioClear(p1->Cardauswahl, ErrorStatus);

	// OK interrupt abgearbeitet, naechster bitte
    PCI_IntNextEnable(p1->Cardauswahl, status);
}


void main()
{
	DWORD ErrorStatus = 0;
	DWORD InMask	  = 0;
	DWORD IntAktiv	  = 0;
	DWORD CardCounter = 0;
	
	DWORD status	  = 0;
	WORD MaxLoop	  = 100;
	WORD counter	  = 0;
	WORD EventCodeNr  = 0;
	
	BYTE ErrorChar [26];

	// Structur anlegen und ini.
	IntStructure myIntStruct;

	myIntStruct.Cardauswahl = 0;
	myIntStruct.IntAktiv	= 0;
	myIntStruct.IntCounter	= 0;
	myIntStruct.IntResive   = false;

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
			cin >> myIntStruct.Cardauswahl;
		}while((myIntStruct.Cardauswahl > CardCounter)||(myIntStruct.Cardauswahl <= 0));
		
		status =PCI_DriverOpen(myIntStruct.Cardauswahl);
	}
	else // gibt es nur eine -> keine auswahl anbieten
	{
		status = PCI_DriverOpen(1);
		myIntStruct.Cardauswahl = 1;
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
	PCI_PCIcardReset(myIntStruct.Cardauswahl, ErrorStatus);

	// Filter einschalten
	PCI_CntrlRegSet(myIntStruct.Cardauswahl, Cntrl_EvFilter_On, ErrorStatus);

	// Aufraeumen : alle Event settings loeschen
	PCI_EvFilterClear(myIntStruct.Cardauswahl, ErrorStatus);

	// Aufraeumen : Event- FiFo loeschen
	PCI_EvFiFioClear(myIntStruct.Cardauswahl, ErrorStatus);

	// Uns interessiert nur Event 1 
	// kommen dieses an, so soll es  
	// im Event- FiFo reingeschrieben werden
	PCI_EvFilterSet(myIntStruct.Cardauswahl, 1, EventOn, ErrorStatus);

	// Interrupt Maske definieren, und zwar wollen wir wissen
	// ob event 1 angekommen ist, das koennen wir aber nicht 
	// direkt pruefen, deshalb haben wir den filter so 
	// eingestellt, das nur event 1 durchkommen kann 
	// (abgesehen von den events >= C8hex )
	// daher verknuepfen wir den interrupt mit dem
	// bit Evt_Fifo_Not_Empty. Das ist das 6te bit in
	// der interrupt-maske (siehe UsersGuide) also :
	// 1000000 -> 0x40 (hex)
	
	InMask = 0x4;

	PCI_IntMaskWrite(myIntStruct.Cardauswahl, InMask, ErrorStatus);

	// Interrupts enabeln jetzt mit einer callback-funktion
	// und der structur
	status = PCI_IntEnable (myIntStruct.Cardauswahl, MY_InterruptHandler, &myIntStruct ,IntSetPriorityHigh,  ErrorStatus);

	if(status != StatusOK)
	{
		PCI_ErrorString (status, ErrorChar);
		cout<<ErrorChar<<endl<<endl;

		// im fehlerfall aufraeumen
		PCI_EvFilterSet(myIntStruct.Cardauswahl, 1, EventClear, ErrorStatus);

		// ende
		PCI_DriverClose(myIntStruct.Cardauswahl);

		getchar();
		getchar();
		return;
	}

	// Hier irgentein code der vom main ausgefuehrt werden soll
	while(myIntStruct.IntCounter <= 10000)
	{
		if (myIntStruct.IntResive == true) 
		{
			cout<<"Int. count : "<<myIntStruct.IntCounter<<endl;
			cout<<"Int. active :"<<myIntStruct.IntAktiv<<endl;
			cout<<"---------------------------"<<endl;

			myIntStruct.IntResive = false;
		}
	}

	// WICHTIG !!!
	PCI_IntDisable(myIntStruct.Cardauswahl, ErrorStatus);
	PCI_ErrorString (status, ErrorChar);
	cout<<"IntDisable :"<<ErrorChar<<endl;

	// ende
	PCI_DriverClose(myIntStruct.Cardauswahl);
	PCI_ErrorString (status, ErrorChar);
	cout<<"DriverClose :"<<ErrorChar<<endl;

	// Wartet
	cout<<"\n...ende..\n";
	getchar();
}