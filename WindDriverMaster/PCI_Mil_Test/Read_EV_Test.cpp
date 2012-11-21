#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

bool Read_Event_Test(_WORD Cardauswahl, string &ErrorMessage, bool ScreenOut, int loop)
{
	_DWORD ErrorStatus= 0;
	_WORD StatusReg   = 0;
	_WORD counter	 = 0;
	_WORD SendeDaten	 = 0;
	_WORD time		 = 3;
	_WORD ReadDaten	 = 0; 
	_WORD OrginalDaten= 0;
	
	int dummy = 0;
	int VerifyFailtInt = 0;
	int LoopCounter = 0;

	bool EVFifoNotEmpty = true;
	bool BildschirmAusgabe = false;

	string ausgabe_eins;

	if(ScreenOut)
	{
		ClearScreen();
	}

	PCI_PCIcardReset(Cardauswahl, ErrorStatus);
	
	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<<"Kleiner Read Event Test :"<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);   
		cout<<"Bitte das SWPZ-Adapterkabel SE->TIF zwischen den Mil-Bus und"<<endl;
		cout<<"der zwei poligen Lemo-Buchse verbinden."<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		cout<<"Press Enter to go on...."<<endl;
		getchar();
		getchar();
	}

	counter = 1;
	LoopCounter = 0;
		
	do
	{		
		SendeDaten = ~counter; 
		PCI_MilBusWrite (Cardauswahl, SendeDaten, ErrorStatus);
		LoopCounter ++;

		if(LoopCounter >= 254)
			PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);	

		if(!(PCI_StatusTest(Cardauswahl, Evt_Fifo_Full, ErrorStatus)))
		{
			SendeDaten = counter;
			PCI_MilBusWrite (Cardauswahl, SendeDaten, ErrorStatus);
			LoopCounter++;
		}

		if(LoopCounter >= 254)
			PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);

		counter++;

	}while(!(PCI_StatusTest(Cardauswahl, Evt_Fifo_Full, ErrorStatus))&&(LoopCounter <= 300));

	dummy = LoopCounter;

	if (PCI_StatusTest(Cardauswahl, Evt_Fifo_Full, ErrorStatus))
	{
		ausgabe_eins = "voll";
	}
	else
	{
		if(ScreenOut)
		{
			ausgabe_eins = "angeblich noch NICHT voll";// Fehlerfall
		}
		else
		{
			ErrorMessage = " ERROR ! EV- FiFo nicht voll! ";
			return false;
		}
	}

	if(ScreenOut)
	{
		cout << "\nEs Wurden " << dummy << " Daten rausgeschrieben ! "<<endl;
		cout <<"Das Event FiFo ist " << ausgabe_eins << " ! "<<endl<<endl;
	}

	counter = 1;
	LoopCounter = 0;
	VerifyFailtInt = 0;

	while((EVFifoNotEmpty) && ((counter % 0xFFFF) != 0)) 
	{		
		
		PCI_EvFiFoRead(Cardauswahl, ReadDaten, EVFifoNotEmpty, ErrorStatus);
		LoopCounter++;
		if(ReadDaten != ((_WORD)~counter))
		{
			VerifyFailtInt++;
		}

		if(EVFifoNotEmpty)
		{
			PCI_EvFiFoRead(Cardauswahl, ReadDaten, EVFifoNotEmpty, ErrorStatus);
			if(ReadDaten != counter)
			{
				VerifyFailtInt++;
			}
			LoopCounter++;
		}
		counter++;
	}

	// Ausgabe
	//
	if(ScreenOut)
	{
		cout<<"Es wurden "<<LoopCounter<<" Daten aus dem Fifo gelesen"<<endl;
		cout<<"Davon sind "<<VerifyFailtInt<< " Fehlerhaft."<<endl<<endl;
	}

	if((dummy == LoopCounter) && (VerifyFailtInt == 0))
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<"Der Test war Erfolgreich ! "<<endl<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<"Der Test war Fehlerhaft !!" <<endl<<endl;
		}
		else
		{
			if(dummy == LoopCounter)
			{
				ErrorMessage = " ERROR ! Anzahl Lesen/Schreiben ungleich ! ";
				return false;
			}
			else if(VerifyFailtInt != 0)
			{
				ErrorMessage = " ERROR ! Verify Read/Write ! ";
				return false;
			}
			else if (LoopCounter != 256)
			{
				ErrorMessage = " ERROR ! FiFo Eintraeger ungleich 256 ! ";
				return false;
			}
		}
	}

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
		cout<< "..press ENTER to go on..."<<endl;
		getchar();
	}
	return true;
}

