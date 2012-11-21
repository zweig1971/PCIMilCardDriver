#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

bool rahmen_puls_test(_WORD Cardauswahl,  string &ErrorMessage, bool ScreenOut, int loop)
{
	_WORD ErrorCounter = 0;
	_WORD counter	  = 0;
	_DWORD ErrorStatus = 0;
	_WORD SendeDaten	  = 0;
	_WORD ReadDaten	   =0; 
	_BYTE EventValue   = 0;
	_BYTE CntrlRegValue= 0; 
	
	bool EVFifoNotEmpty = true;
	bool testOK = true;

	COORD coordScreen;
	COORD StatusCoord;

	StatusCoord.X = 4;
	StatusCoord.Y = 24;

	PCI_PCIcardReset(Cardauswahl, ErrorStatus);

	if(ScreenOut)
	{
		ClearScreen();
		
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

		ClearScreen();

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<<"Puls Test mit Karten Nr. "<<Cardauswahl<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		// Text Ausgabe
		// -------------

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		cout <<"Clear Event- Filter and test it      :"<<endl;
		cout <<"Clear Event- FiFo   and test it      :"<<endl;
		cout <<"Set Event 1 EventOff,Puls1/2 On      :"<<endl;
		cout <<"Set Event 2 EventOff,Puls1/2 Off     :"<<endl;
		cout <<"Switch Event Filter on               :"<<endl;
		cout <<"Set Rahmen Puls 1 On                 :"<<endl;
		cout <<"Set Rahmen Puls 2 On                 :"<<endl;
		cout <<"Event FiFo empty                     :"<<endl;
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

	coordScreen.X = 38;
	coordScreen.Y = 2;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

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
			ErrorMessage = " ERROR ! events cleared overflow ! ";
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
			ErrorMessage = "  FAILURE ! Some settings not cleared ! ";
			return false;
		}
	}

	//---------------------------
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
			ErrorMessage = " ERROR ! EV-FiFo not empty ! ";
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

	// Event eins setzen
	PCI_EvFilterSet(Cardauswahl, 1, Puls1_On, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 1, Puls2_On, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 1, EventOff, ErrorStatus);

	// Value von event eins einlesen
	EventValue = 0;
	PCI_EvFilterRead(Cardauswahl,1, EventValue, ErrorStatus);

	// Value von event eins pruefen
	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 4;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(EventValue != (EvFilter_Start_Puls1 | EvFilter_Start_Puls2))
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" Failure"<<endl;
			testOK = false;
		}
		else
		{
			ErrorMessage = " Failure ! ";
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

	// Event zwei setzen
	PCI_EvFilterSet(Cardauswahl, 2, EventOff, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 2, Puls1_Off, ErrorStatus);
	PCI_EvFilterSet(Cardauswahl, 2, Puls2_Off, ErrorStatus);

	// Value von event zwei einlesen
	EventValue = 0;
	PCI_EvFilterRead(Cardauswahl,2, EventValue, ErrorStatus);

	// Value von event zwei pruefen
	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(EventValue != (EvFilter_Stop_Puls1 | EvFilter_Stop_Puls2))
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" Failure"<<endl;
			testOK = false;
		}
		else
		{
			ErrorMessage = " Failure ! ";
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


	// Filter einschalten
	//--------------------

	PCI_CntrlRegSet(Cardauswahl, Cntrl_EvFilter_On, ErrorStatus);

	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 6;
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
			cout<<" Failure !"<<endl;
			testOK = false;
		}
		else
		{
			ErrorMessage = " Failure ! ";
			return false;
		}
	}

	// Puls oder Rahmen Puls
	PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls1_Rahmen_On, ErrorStatus);

	// Control Register auslesen
	PCI_CntrlRegRead(Cardauswahl, CntrlRegValue, ErrorStatus );
	
	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 7;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((CntrlRegValue & Ctrl_Puls1_Rahmen_On) != Ctrl_Puls1_Rahmen_On)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" Failure !"<<endl;
			testOK = false;
		}
		else
		{
			ErrorMessage = " Failure ! ";
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

	// Puls oder Rahmen Puls
	PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls2_Rahmen_On, ErrorStatus);

	// Control Register auslesen
	PCI_CntrlRegRead(Cardauswahl, CntrlRegValue, ErrorStatus );
	
	if(ScreenOut)
	{
		coordScreen.X = 38;
		coordScreen.Y = 8;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if((CntrlRegValue & Ctrl_Puls2_Rahmen_On) != Ctrl_Puls2_Rahmen_On)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" Failure !"<<endl;
			testOK = false;
		}
		else
		{
			ErrorMessage = " Failure ! ";
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

	coordScreen.X = 0;
	coordScreen.Y = 13;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	
	if(testOK == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<"\n\n...Press Enter to START Rahmen Pulse 1 & 2 ...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
	
		SendeDaten = 1;
		PCI_MilBusWrite(Cardauswahl, SendeDaten,ErrorStatus);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<"\n\n...Press Enter to STOP Rahmen Pulse 1 & 2 ...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();

		SendeDaten = 2;
		PCI_MilBusWrite(Cardauswahl, SendeDaten,ErrorStatus);

		if(ScreenOut)
		{
			coordScreen.X = 38;
			coordScreen.Y = 9;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
		}

		if (PCI_StatusTest(Cardauswahl, Evt_Fifo_Not_Empty, ErrorStatus))
		{
			if(ScreenOut)
			{
				SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
				cout<<" Error ! Event FiFo is not empty !"<<endl;
			}
			else
			{
				ErrorMessage = " Error ! Event FiFo is not empty ! ";
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
	}

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		coordScreen.X = 0;
		coordScreen.Y = 20;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		cout<< "..press ENTER to go back..."<<endl;
		getchar();
	}
	return true;
}