#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

_WORD dauerCounter;
_WORD VerifyFailtInt;

bool dauertest (_WORD Cardauswahl,string &ErrorMessage, bool ScreenOut)
{

	_WORD time		   = 4;
	_WORD temp		   = 0;
	_WORD counter	   = 0;
	_WORD ReadDaten	   = 0; 
	_WORD StatusReg	   = 0;
	_WORD SendeDaten	   = 0;
	_DWORD ErrorStatus  = 0;
	_WORD ErrorCounter  = 0;

	_WORD buffercounter = 0;

	COORD coordScreen;
	COORD StatusCoord;

	StatusCoord.X = 4;
	StatusCoord.Y = 24;
	
	bool BildschirmAusgabe = false;
	bool EVFifoNotEmpty = true;
	bool testOK = true;

	char answer;

	_WORD Buffer[600];
	_WORD TempBuff[600];

// --------------------------------------------------
// ------------ DauerTest EVFifo --------------------
// --------------------------------------------------

	// Events settings löschen
	//--------------------------
	ErrorCounter = 0;
	ErrorStatus  = 0;
	buffercounter= 0;

	PCI_EvFilterClear(Cardauswahl, ErrorStatus);

	// Buffer löschen
	//---------------
	counter = 0;
	do
	{
		Buffer[counter] = 0;
		TempBuff[counter] = 0;
		counter++;
	}while(counter < 599);
			
			
	// Gerade oder ungerade EV setzen
	//--------------------------------
	temp = 0;
	counter = 0;
	ErrorStatus = 0;
			
	if((dauerCounter % 2) == 0)
	{
		// Schreiben der geraden events und scharf machen
		//
		while((ErrorStatus & EventNrError) != EventNrError) 
		{
			temp = counter % 2;
			if(temp == 0)
				PCI_EvFilterSet(Cardauswahl, counter, EventOn, ErrorStatus);
			counter++;
		}
	}  
	else
	{
		// Schreiben der ungeraden events und scharf machen
		//
		while((ErrorStatus & EventNrError) != EventNrError) 
		{
			temp = counter % 2;
			if(temp != 0)
				PCI_EvFilterSet(Cardauswahl, counter, EventOn, ErrorStatus);
			counter++;
		}
	}

	// EV versenden
	//--------------------
	counter    = 0;
	SendeDaten = 0;
	StatusReg  = 0;
	ErrorStatus= 0;

	do
	{		
		SendeDaten = counter;				

		if((counter > 199) && (counter < 256))
		{
			Buffer[buffercounter] = counter;
			buffercounter++;
		}
		else if((dauerCounter % 2) == 0)
		{
			temp = counter % 2;
			if(temp == 0)
			{
				Buffer[buffercounter] = counter;
				buffercounter++;						
			}	
		}
		else
		{
			temp = counter % 2;
			if(temp != 0)
			{
				Buffer[buffercounter] = counter;
				buffercounter++;
			}	
		}

		PCI_MilBusWrite(Cardauswahl,SendeDaten,ErrorStatus);
				
		if(counter >= 254)
			PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);	
				
		counter++;	

	}while(!(PCI_StatusTest(Cardauswahl, Evt_Fifo_Full, ErrorStatus))&&(counter <= 600));


	if(counter >= 600)
	{
		if(ScreenOut)
		{
			coordScreen.X = 2;
			coordScreen.Y = 6;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);   
			cout<<" WARNUNG ! Es wurden keine Events empfangen ! "<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		}
		else
		{
			ErrorMessage = " ERROR ! Es wurde kein Event empfangen ! ";
			return false;
		}
	}
	else
	{
		if(ScreenOut)
		{
			coordScreen.X = 2;
			coordScreen.Y = 6;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			cout<<"                                              "<<endl;
		}
	}

	counter		   = 0;
	ErrorStatus	   = 0;
	EVFifoNotEmpty = true;

	while(EVFifoNotEmpty)
	{		
		PCI_EvFiFoRead(Cardauswahl, ReadDaten, EVFifoNotEmpty, ErrorStatus);
		TempBuff[counter] = ReadDaten;
		if(ReadDaten != Buffer[counter])
		{
			VerifyFailtInt++;
			BildschirmAusgabe = true;
		}
		counter++;
	}


	// FiFo löschen
	//---------------------------

	PCI_EvFiFioClear(Cardauswahl, ErrorStatus);
			

	// Bilschirmausgabe
	//--------------------
	if(ScreenOut)
	{
		if(((dauerCounter % 10) == 0) || (BildschirmAusgabe) || (answer == ' '))
		{
			coordScreen.X = 25;
			coordScreen.Y = 2;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
			cout<<dauerCounter<<endl;
		}

		if(BildschirmAusgabe)
		{
			coordScreen.X = 25;
			coordScreen.Y = 4;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
			cout<<VerifyFailtInt<<endl;

			BildschirmAusgabe = false;
		}
	}

	return true;
}


bool Event_Filter_Test(_WORD Cardauswahl, string &ErrorMessage, bool ScreenOut, int loop)
{
	_WORD time		   =4;
	_WORD temp		   =0;
	_WORD counter	   =0;
	_WORD einEvent	   =0;
	_WORD ReadDaten	   =0; 
	_WORD StatusReg	   =0;
	_WORD SendeDaten	   =0;
	_DWORD ErrorStatus  =0;
	_DWORD EVTimeEins   =0;
	_DWORD EVTimeZwei   =0;
	_DWORD EVTimeDrei   =0;
	_WORD ErrorCounter  =0;

	_WORD SollDaten = 0;
	_WORD IstDaten  = 0;

	COORD coordScreen;
	COORD StatusCoord;

	StatusCoord.X = 4;
	StatusCoord.Y = 24;
	
	_WORD Fehlerhafte_Daten = 0;

	BYTE EventValue = 0;
	BYTE CntrlReg	= 0;

	bool BildschirmAusgabe = false;
	bool EVFifoNotEmpty = true;
	bool testOK = true;
	bool status = true;

	char answer;

	string StatusZeile ="  Press <SPACE> for singel step, <RETURN> to go on and <X> to exit  ";

	if(ScreenOut)
	{
		ClearScreen();
	}

	PCI_PCIcardReset(Cardauswahl, ErrorStatus);

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<<"Kleiner Event Filter Test auf Karten Nr.: "<<Cardauswahl<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);   
		cout<<"Bitte das SWPZ-Adapterkabel SE->TIF zwischen den Mil-Bus und"<<endl;
		cout<<"der zwei poligen Lemo-Buchse verbinden."<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);


		cout<<"Press Enter to go on...."<<endl;
		getchar();
		getchar();
	}

	if(ScreenOut)
	{
		ClearScreen();
	}

	if(ScreenOut)
	{
		// Text Ausgabe
		// -------------
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<"...begin test..."<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		cout <<"Clear event settings and test it     :"<<endl;
		cout <<"Clear event FiFo and test it         :"<<endl;
		cout <<"Write event Nr. to filter and set it :"<<endl;
		cout <<"Verifying events settings            :"<<endl;
		cout <<""<<endl;
	}

	// Events settings löschen
	//--------------------------
	
	PCI_EvFilterClear(Cardauswahl, ErrorStatus);

	// Testen ob settings gelöscht
	//-----------------------------	
	counter		= 0;
	EventValue  = 0;
	ErrorStatus = 0;
	
	while((ErrorStatus & EventNrError) != EventNrError) 
	{
		PCI_EvFilterRead(Cardauswahl, counter, EventValue, ErrorStatus);

		if(EventValue != 0)
			ErrorCounter++;
			
		counter++;
	}

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 2;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((counter-1) < 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! insufficient events cleared ! "<<endl;
			testOK = false;	
		}
		else 
		{
			ErrorMessage = " ERROR ! insufficient events cleared ! ";
			return false;
		}
	}
	else if((counter-1) > 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! events cleared overflow! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! events cleared overflow! ";
			return false;
		}
	}
	else if(ErrorCounter == 0)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Some settings not cleared :"<<ErrorCounter<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Some settings not cleared !";
			return false;
		}
	}

	// FiFo löschen
	//---------------------------

	PCI_EvFiFioClear(Cardauswahl, ErrorStatus);

	// FiFo prüfen
	//--------------------
	EVFifoNotEmpty = true;
	ReadDaten   = 0;
	ErrorStatus = 0;

	PCI_EvFiFoRead (Cardauswahl, ReadDaten, EVFifoNotEmpty, ErrorStatus);

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 3;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(EVFifoNotEmpty == true)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! EV-FiFo not empty !"<<endl;
			testOK = false;
		}
		else
		{
			ErrorMessage = " ERROR ! EV-FiFo not empty !";
			return false;
		}
	}
	else 
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}


	// Event schreiben und scharf machen
	ErrorCounter = 0;
	counter		 = 0;
	ErrorStatus  = 0;
	
	do
	{
		PCI_EvFilterSet(Cardauswahl, counter, EventOn, ErrorStatus);
		counter++;
	}while(((ErrorStatus & EventNrError) != EventNrError) && ((counter % 4097) != 0)); 

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 4;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((counter-1) == 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{	
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Daten to write :"<<(counter-1)<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<endl<<endl<<"... Press Enter to abort...";
			getchar();
			return false;
		}
		else
		{
			ErrorMessage = " FAILURE ! Daten to write !";
			return false;
		}
	}

	// Events überprüfen ob alle scharf sind
	counter     = 0;
	ErrorStatus = 0;
	EventValue	= 0;
	ErrorCounter= 0;

	while((ErrorStatus & EventNrError) != EventNrError) 
	{
		PCI_EvFilterRead(Cardauswahl, counter, EventValue, ErrorStatus);

		if(EventValue != 1)
			ErrorCounter++;
			
		counter++;
	}

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if ((counter-1) < 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! insufficient entrys ! "<<endl;
			testOK = false;
		}
		else 
		{
			ErrorMessage = " ERROR ! insufficient entrys ! ";
			return false;
		}
	}
	else if((counter-1) > 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! entrys overflow! "<<endl;
			testOK = false;	
		}
		else 
		{
			ErrorMessage = " ERROR ! entrys overflow! ";
			return false;
		}
	}
	else if(ErrorCounter == 0)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Verifying Errors :"<<ErrorCounter<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Verifying Errors !";
			return false;
		}
	}

//--------------------------------------------------------------

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		cout <<"Clear event settings and test it     :"<<endl;
		cout <<"Set odd events                       :"<<endl;
		cout <<"Activate filter                      :"<<endl;
		cout <<"Send events                          :"<<endl;
		cout <<"Verifying received events in fifo    :"<<endl;
	}
	// Events settings löschen
	//--------------------------
	
	PCI_EvFilterClear(Cardauswahl, ErrorStatus);
	
	// Testen
	//------------------------	
	counter		= 0;
	EventValue  = 0;
	ErrorStatus = 0;
	
	while((ErrorStatus & EventNrError) != EventNrError) 
	{
		PCI_EvFilterRead(Cardauswahl, counter, EventValue, ErrorStatus);

		if(EventValue != 0)
			ErrorCounter++;
			
		counter++;
	}

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 6;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((counter-1) < 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! insufficient events cleared ! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! insufficient events cleared ! ";
			return false;
		}
	}
	else if((counter-1) > 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! events cleared overflow! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! events cleared overflow! ";
			return false;
		}
	}
	else if(ErrorCounter == 0)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Some settings not cleared :"<<ErrorCounter<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Some settings not cleared !";
			return false;
		}
	}


	// Schreiben der ungeraden events und scharf machen
	//
	temp = 0;
	counter = 0;
	ErrorStatus = 0;

	while((ErrorStatus & EventNrError) != EventNrError) 
	{
		temp = counter % 2;
		if(temp == 0)
			PCI_EvFilterSet(Cardauswahl, counter, EventOn, ErrorStatus);
		counter++;
	}

	// Prüfen ob das auch angekommen ist
	//

	temp		 = 0;
	counter		 = 0;
	EventValue	 = 0;
	ErrorCounter = 0;
	ErrorStatus  = 0;

	while((ErrorStatus & EventNrError) != EventNrError) 
	{
		temp = counter % 2;
		if(temp == 0)
			PCI_EvFilterRead (Cardauswahl, counter, EventValue, ErrorStatus);
		
		if(EventValue != 1)
			ErrorCounter++;

		counter++;
	}

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 7;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((counter-1) < 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! insufficient events tested ! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! insufficient events tested ! ";
			return false;
		}
	}
	else if((counter-1) > 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! events tested overflow! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! events tested overflow! ";
			return false;
		}
	}
	else if(ErrorCounter == 0)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Some settings not set :"<<ErrorCounter<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Some settings not set !";
			return false;
		}
	}	

	// Filter einschalten
	PCI_CntrlRegSet(Cardauswahl, Cntrl_EvFilter_On, ErrorStatus);

	// Prüfen ob eingeschalten
	if(ScreenOut)
	{
		StatusReg = 0;
		coordScreen.X = 38;
		coordScreen.Y = 8;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(PCI_StatusTest(Cardauswahl, EvFilter_On, ErrorStatus))
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE !"<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Filter not set !";
			return false;
		}
	}
		
	// events versenden

	counter    = 0;
	SendeDaten = 0;
	StatusReg  = 0;
	ErrorStatus= 0;

	do
	{		
		SendeDaten = counter;
		
		PCI_MilBusWrite(Cardauswahl, SendeDaten,ErrorStatus);
		
		if(counter >= 254)
			PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);	
		
		counter++;	

	}while(!(PCI_StatusTest(Cardauswahl, Evt_Fifo_Full, ErrorStatus))&&(counter <= 600));

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 9;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(PCI_StatusTest(Cardauswahl, Evt_Fifo_Full, ErrorStatus))
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! FiFo overflow !"<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! FiFo overflow !";
			return false;
		}
	}


	//Die empfangenen events im ev_fifo müssen alle
	//gerade sein, wenn nicht->fehler.
	
	counter = 0;
	ErrorStatus = 0;
	VerifyFailtInt = 0;
	EVFifoNotEmpty = true;

	while((EVFifoNotEmpty) && (counter <= 100)) 
	{		
		PCI_EvFiFoRead (Cardauswahl, ReadDaten, EVFifoNotEmpty, ErrorStatus);

		if((ReadDaten % 2)!= 0)
		{
			VerifyFailtInt++;
		}
		counter++;
	}

	while((EVFifoNotEmpty) && (counter <= 156)) 
	{		
		PCI_EvFiFoRead (Cardauswahl, ReadDaten, EVFifoNotEmpty, ErrorStatus);

		if(ReadDaten!= (counter+100))
		{
			VerifyFailtInt++;
		}
		counter++;
	}
	
	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 10;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((counter-1) < 156)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" ERROR ! insufficient events ! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! insufficient events ! ";
			return false;
		}
	}
	else if((counter-1) > 156)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" ERROR ! FiFo overflow ! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! FiFo overflow ! ";
			return false;
		}
	}
	else if(VerifyFailtInt == 0)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Bad events :"<<VerifyFailtInt<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Bad events !";
			return false;
		}
	}

	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
	if(!testOK)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<endl<<endl<<"  TEST FAILURE  "<<endl<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
			cout<< "..press ENTER to exit test..."<<endl;
			getchar();
			return false;
		}
	}

//-------------------------------TIMER TEST-----------------------------

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);
		cout<<"\nTimer test"<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		cout <<"Clear event settings and test it     :"<<endl;
		cout <<"Set event timer                      :"<<endl;
		cout <<"Send event                           :"<<endl;
		cout <<"Wait a while...                      :"<<endl;
		cout <<"Verifying time                       :"<<endl;
	}

	// Events settings löschen
	//--------------------------

	PCI_EvFilterClear(Cardauswahl, ErrorStatus);
	
	// Testen
	//------------------------	
	counter		= 0;
	EventValue  = 0;
	ErrorStatus = 0;
	
	while((ErrorStatus & EventNrError) != EventNrError) 
	{
		PCI_EvFilterRead(Cardauswahl, counter, EventValue, ErrorStatus);

		if(EventValue != 0)
			ErrorCounter++;
			
		counter++;
	}

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 14;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((counter-1) < 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! insufficient events cleared ! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! insufficient events cleared ! ";
			return false;
		}
	}
	else if((counter-1) > 4096)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" ERROR ! events cleared overflow! "<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! events cleared overflow! ";
			return false;
		}
	}
	else if(ErrorCounter == 0)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Some settings not cleared :"<<ErrorCounter<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Some settings not cleared !";
			return false;
		}
	}

	ErrorStatus = 0;
	EventValue	= 0;
	
	// per zufall irgentein event aussuchen und timer setzen
	einEvent = rand()%4095;
	PCI_EvFilterSet(Cardauswahl, einEvent, EventTimerRun, ErrorStatus);
	PCI_EvFilterRead(Cardauswahl, einEvent, EventValue, ErrorStatus);

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 15;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(EventValue != 2)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" FAILURE ! Cannot Timer set !"<<endl;;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " FAILURE ! Cannot Timer set !";
			return false;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK "<<endl;
		}
	}

	//Timer einlesen
	PCI_EvTimerRead(Cardauswahl, EVTimeEins ,ErrorStatus);

	//event versenden 
	PCI_MilBusWrite(Cardauswahl, einEvent,ErrorStatus);
	
	// Auf Event warten
	PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);	
	
	//Timer einlesen
	PCI_EvTimerRead(Cardauswahl, EVTimeZwei ,ErrorStatus);

	// Warten	
	PCI_TimerWait (Cardauswahl, 1000, Timerbase_1ms, ErrorStatus);	
	
	//Timer einlesen
	PCI_EvTimerRead(Cardauswahl, EVTimeDrei ,ErrorStatus);

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 16;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;

		coordScreen.X = 38;
		coordScreen.Y = 17;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;	

		coordScreen.X = 38;
		coordScreen.Y = 18;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(EVTimeZwei > 30)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" ERROR ! First time is too high ("<<EVTimeZwei<<"/"<<EVTimeDrei<<")"<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! First time is too high ";
			return false;
		}

	}
	else if(EVTimeDrei < 999000)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<" ERROR ! Second  time is too small ("<<EVTimeZwei<<"/"<<EVTimeDrei<<")"<<endl;
			testOK = false;	
		}
		else
		{
			ErrorMessage = " ERROR ! Second  time is too small ";
			return false;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK ("<<EVTimeZwei<<"/"<<EVTimeDrei<<")"<<endl;
		}
	}

	if(!testOK)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
			cout<<endl<<endl<<"  TEST FAILURE  "<<endl<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
			cout<<endl<< "..press ENTER to go back..."<<endl;
			getchar();
			return false;
		}
	}

	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<endl<<endl<<" TESTING ENDS SUCCESSFUL  "<<endl;
		}
	}

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		cout<<endl<< "..press ENTER to go ..."<<endl;
		getchar();
	}

// --------------------------------------------------
// ------------ DauerTest EVFifo --------------------
// --------------------------------------------------

   	answer = ' ';
	dauerCounter   = 0;
	VerifyFailtInt = 0;
	
	if(ScreenOut)
	{
		ClearScreen();
	}


	// Filter einschalten
	PCI_CntrlRegSet(Cardauswahl, Cntrl_EvFilter_On, ErrorStatus);
	
	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<<"Dauer Event Filter Test mit Karten Nr. "<<Cardauswahl<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		cout<<"Test durchlaeufe       : "<<dauerCounter<<endl<<endl;
		cout<<"Fehlerhafte Events     : "<<Fehlerhafte_Daten<<endl;
	}

	// FiFo löschen
	//---------------------------
	PCI_EvFiFioClear(Cardauswahl, ErrorStatus);

	dauerCounter = 0;
	VerifyFailtInt = 0;
	answer = ' ';
	
	if(ScreenOut)
	{
		do
		{
			do
			{
				status = dauertest (Cardauswahl,ErrorMessage,ScreenOut);
				dauerCounter++;
			}while (!((_kbhit()) || (answer == ' ') || (answer == 'X')));
			
			if (answer != 'X')
				ReadKey (answer, StatusZeile ,StatusCoord);
		}while(answer != 'X');	
	}
	else
	{
		do
		{
			status = dauertest (Cardauswahl,ErrorMessage,ScreenOut);
			dauerCounter++;
		}while((dauerCounter <= loop) && (status != false));
	}
	
	return status;
}
