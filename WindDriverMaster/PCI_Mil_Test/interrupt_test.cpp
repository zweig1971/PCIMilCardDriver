#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

bool interrupt_test(_WORD CardNr, string &ErrorMessage, bool ScreenOut, int loop)
{
	_DWORD Status	  = 0;
	_DWORD ErrorStatus = 0;
	_DWORD IntAktiv	  = 0;
	_WORD ErrorCounter = 0;

	_DWORD InMask; 
	_WORD myDta_Rdy = 0x2;
	_WORD myDta_Req = 0x4;

	_BYTE ErrorChar [24];

	string ausgabe;

	bool EVFifoNotEmpty = true;
	bool error = false;
	
	COORD coordScreen;

// -------------- Windows Version (NT oder 2000) ------------------

	OSVERSIONINFO lVerInfo;
	lVerInfo.dwOSVersionInfoSize = sizeof (lVerInfo);
	GetVersionEx (&lVerInfo);
	

// ----------------------------------------------------------------

	if(ScreenOut)
	{
		ClearScreen();
		InMask = 0x6; // Auf Dta_Rdy oder Dta_Req interrupten
	}
	else
	{
		InMask = 0x2; // Nur auf Dta_Rdy interrupten
	}

	PCI_PCIcardReset(CardNr, ErrorStatus);

	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<<"Kleiner Interrupt Test auf Karten Nr.: "<<CardNr<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);   
		cout<<"Bitte einen Mil-Bus-Monitor mit der PCI-Mil-Karte verbinden.        "<<endl;
		cout<<"Mit den Tastern DRDY oder DREQ koennen interrupts ausgeloest werden."<<endl<<endl;

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);


		cout<<"Press Enter to go on...."<<endl;
		getchar();
		getchar();

		ClearScreen();
	}

	
	// Text Ausgabe
	// -------------
	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_RED);  
		cout<<"\nHinweis : Sollte der Rechner haengen bleiben, so koennte keine      "<<endl;

		coordScreen.X = 9;
		coordScreen.Y = 2;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		
		cout<<" Verbindung zum Mil-Monitor da sein, ansonsten viel glueck."<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		cout <<"Interrupts enabeln         :"<<endl;
		cout <<"Interrupt Maske definieren :"<<endl;
		cout <<"Wait interrupt             :"<<endl;
		cout <<"Interrupts disabeln        :"<<endl;
		cout <<""<<endl;

		coordScreen.X = 28;
		coordScreen.Y = 3;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
    }

	// Interrupts enabeln
	Status = PCI_IntEnable (CardNr, NULL, NULL, IntSetPriorityNormal,ErrorStatus);
	
	if(ScreenOut)
	{
		coordScreen.X = 28;
		coordScreen.Y = 4;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(Status != StatusOK)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			PCI_ErrorString (Status, ErrorChar);
			cout<<ErrorChar<<endl<<endl;
				SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
			cout<<endl<<endl<<"... Press Enter to abort...";
			PCI_IntDisable(CardNr, ErrorStatus);
			getchar();
			PCI_PCIcardReset(CardNr, ErrorStatus);
			return false;
		}
		else 
		{
			ErrorMessage = " FAILURE ! set Interrupt enable ! ";
			PCI_IntDisable(CardNr, ErrorStatus);
			PCI_PCIcardReset(CardNr, ErrorStatus);
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

	// Interrupt Maske definieren
	Status = PCI_IntMaskWrite(CardNr, InMask, ErrorStatus);

	if(ScreenOut)
	{
		coordScreen.X = 28;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(Status != StatusOK)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			PCI_ErrorString (Status, ErrorChar);
			cout<<ErrorChar<<endl<<endl;
			PCI_IntDisable (CardNr, ErrorStatus);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
			cout<<endl<<endl<<"... Press Enter to abort...";
			PCI_IntDisable(CardNr, ErrorStatus);
			getchar();
			PCI_PCIcardReset(CardNr, ErrorStatus);
			return false;		
		}
		else 
		{
			ErrorMessage = " FAILURE ! set Interrupt Mask ! ";
			PCI_IntDisable(CardNr, ErrorStatus);
			PCI_PCIcardReset(CardNr, ErrorStatus);
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

	if(lVerInfo.dwMajorVersion == 5 )
	{
		Status = PCI_IntWait(CardNr, IntAktiv, ErrorStatus);

		if((IntAktiv != myDta_Rdy) && (IntAktiv != myDta_Req))
		{
			Status = PCI_IntWait(CardNr, IntAktiv, ErrorStatus);
		}
	}
	else
	{
		Status = PCI_IntWait(CardNr, IntAktiv, ErrorStatus);
	}

	if(ScreenOut)
	{
		coordScreen.X = 28;
		coordScreen.Y = 6;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}
	
	if(Status != StatusOK)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			PCI_ErrorString (Status, ErrorChar);
			cout<<ErrorChar<<endl<<endl;
			PCI_IntDisable (CardNr, ErrorStatus);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
			cout<<endl<<endl<<"... Press Enter to abort...";
			PCI_IntDisable(CardNr, ErrorStatus);
			getchar();
			PCI_PCIcardReset(CardNr, ErrorStatus);
			return false;		
		}
		else
		{
			ErrorMessage = " FAILURE ! wait Interrupt ! ";
			PCI_IntDisable(CardNr, ErrorStatus);
			PCI_PCIcardReset(CardNr, ErrorStatus);
			return false;
		}
	}
	else if(IntAktiv == myDta_Rdy)
	{
		ausgabe = "Data Ready";
		error = false;
	}
	else if (IntAktiv == myDta_Req)
	{
		ausgabe = "Data Request";
		error = false;
	}
	else 
	{
		error = true;
	}


	if(error == false)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<" OK : "<<ausgabe<<endl;
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<" FALSE : "<<hex<<(int)IntAktiv<<endl;
		}
		else
		{
			ErrorMessage = " ERROR ! Wrong Interrupt ! ";
			PCI_IntDisable(CardNr, ErrorStatus);
			PCI_PCIcardReset(CardNr, ErrorStatus);
			return false;
		}
	}

	// Interrupts disabeln
	Status = PCI_IntDisable(CardNr, ErrorStatus);
	if(ScreenOut)
	{
		coordScreen.X = 28;
		coordScreen.Y = 7;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
	}

	if(Status != StatusOK)
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			PCI_ErrorString (Status, ErrorChar);
			cout<<ErrorChar<<endl<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
			cout<<endl<<endl<<"... Press Enter to abort...";
			PCI_IntDisable(CardNr, ErrorStatus);
			getchar();
			PCI_PCIcardReset(CardNr, ErrorStatus);
			return false;		
		}
		else
		{
			ErrorMessage = " FAILURE ! Interrupt disable ! ";
			PCI_IntDisable(CardNr, ErrorStatus);
			PCI_PCIcardReset(CardNr, ErrorStatus);
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

	if(ScreenOut)
	{
		coordScreen.X = 0;
		coordScreen.Y = 10;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" Test beendet"<<endl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		cout<<".... press Enter to go back ....";
		PCI_IntDisable(CardNr, ErrorStatus);
		getchar();
		PCI_PCIcardReset(CardNr, ErrorStatus);
		
	}

	return true;
}
