#include "main.h"

HANDLE  hConsoleOut;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

void SingelStep (_WORD Cardauswahl)
{
	_WORD IFKNumber	 = 0;
	_WORD auswahl	 = 0;
	_DWORD status	 = 0;

	string ErrorMessage;

	bool ScreenOut = true;
	bool Status = true;

	while(1)
	{

		ClearScreen();
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  

		cout<<"---------- Version 0.02 -- Karten Nr. "<<Cardauswahl<<" -------------"<<endl<<endl;

		cout<< "Kleiner Echo Test..........[1]"<<endl;
		cout<< "Kleiner Fifo Test..........[2]"<<endl;
		cout<< "Kleiner Event Read Test....[3]"<<endl;
		cout<< "Kleiner Event Filter Test..[4]"<<endl;
		cout<< "Kleiner Interrupt Test.....[5]"<<endl;
		cout<< "Pulse Test.................[6]"<<endl;
		cout<< "Rahmen Puls Test...........[7]"<<endl;
		cout<< "Software Puls Test.........[8]"<<endl;
		cout<< "Software Rahmen Puls Test..[9]"<<endl<<endl;
		cout<< "Reset PCI-Mil-Karte.......[10]"<<endl;

		cout<< "\n\nLieber zurueck............[99]"<<endl<<endl;

		cout<<"Choose your destiny :";
		cin>>auswahl;

		switch (auswahl)
		{
		case 1: IFKNumber = IFK_Auswahl(Cardauswahl);
				if (IFKNumber > 0)
				{
					echo_test(Cardauswahl, IFKNumber, ErrorMessage, ScreenOut, 0);
				}
			break;
		case 2: IFKNumber = IFK_Auswahl(Cardauswahl);
				if (IFKNumber > 0)
				{
					fifo_test(Cardauswahl, IFKNumber, ErrorMessage, ScreenOut, 0);
				}
			break;
		case 3: Read_Event_Test(Cardauswahl, ErrorMessage, ScreenOut, 0);
			break;
		case 4: Event_Filter_Test(Cardauswahl, ErrorMessage, ScreenOut, 0);
			break;
		case 5: interrupt_test(Cardauswahl, ErrorMessage, ScreenOut, 0);
			break;
		case 6: pulse_tester(Cardauswahl, ErrorMessage, ScreenOut, 0);
			break;
		case 7: rahmen_puls_test(Cardauswahl, ErrorMessage, ScreenOut, 0);
			break;
		case 8: soft_puls_test(Cardauswahl);
			break;
		case 9:soft_rahmen_puls_test(Cardauswahl);
			break;
		case 10:ResetKarte(Cardauswahl); 
			break;
		default : return;
			break;
		}	
	}
}


void Automatik (_WORD Cardauswahl)
{
	_WORD IFKNumber	 = 0;

	string ErrorMessage;

	bool ScreenOut = false;
	bool Status = true;
	
	COORD coordScreen;

	IFKNumber = IFK_Auswahl(Cardauswahl);
	if (IFKNumber <= 0)
	{
		return;
	}

	ClearScreen();
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
	cout<<"Automatik test mit Karten-Nr.:"<<Cardauswahl<<"    IFK-Nr.:"<<(hex)<<IFKNumber<<(dec)<<endl<<endl<<endl;
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);	

	// Text Ausgabe
	// -------------
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
	cout<< "Echo Test         :"<<endl;
	cout<< "Fifo Test         :"<<endl;
	cout<< "Event Read Test   :"<<endl;
	cout<< "Event Filter Test :"<<endl;
	cout<< "Interrupt Test    :"<<endl;
	cout<< "Pulse Test        :"<<endl;
	cout<< "Rahmen Puls Test  :"<<endl<<endl;

	// --------------------------------------------

	coordScreen.X = 20;
	coordScreen.Y = 3;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	Status = echo_test(Cardauswahl, IFKNumber, ErrorMessage, ScreenOut, 20000);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;

		coordScreen.X = 0;
		coordScreen.Y = 13;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<endl<<endl<<"... Press Enter to abort...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
		getchar();
		return;
	}

	// --------------------------------------------

	coordScreen.X = 20;
	coordScreen.Y = 4;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	Status = fifo_test( Cardauswahl, IFKNumber, ErrorMessage, ScreenOut, 0);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;

		coordScreen.X = 0;
		coordScreen.Y = 13;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<endl<<endl<<"... Press Enter to abort...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
		getchar();
		return;
	}

	// --------------------------------------------

	coordScreen.X = 20;
	coordScreen.Y = 5;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	Status = Read_Event_Test(Cardauswahl, ErrorMessage, ScreenOut, 0);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;

		coordScreen.X = 0;
		coordScreen.Y = 13;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<endl<<endl<<"... Press Enter to abort...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
		getchar();
		return;
	}

	// --------------------------------------------

	coordScreen.X = 20;
	coordScreen.Y = 6;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	Status = Event_Filter_Test(Cardauswahl,ErrorMessage,ScreenOut, 100);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;

		coordScreen.X = 0;
		coordScreen.Y = 13;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<endl<<endl<<"... Press Enter to abort...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
		getchar();
		return;
	}

	// --------------------------------------------

	coordScreen.X = 0;
	coordScreen.Y = 13;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
	cout<<"Achtung !"; 
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
	cout<<" Fuer den Interruptest mit dem Taster DRDY am Mil-Monitor abschalten !"<<endl;
	cout<<"          Mit dem Taster DRDY kann nach Bestaetigung dieser Meldung ein "<<endl;
	cout<<"          Interrupt erzeugt werden."<<endl<<endl;
	cout<<"          Press Enter to go on...";
	getchar();
	getchar();

	coordScreen.X = 0;
	coordScreen.Y = 13;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_RED);
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;

	coordScreen.X = 20;
	coordScreen.Y = 7;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	Status = interrupt_test(Cardauswahl, ErrorMessage, ScreenOut, 0);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;
	}

	// --------------------------------------------

	Status = pulse_tester(Cardauswahl, ErrorMessage, ScreenOut, 0);

	coordScreen.X = 0;
	coordScreen.Y = 13;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_RED);
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;

	coordScreen.X = 20;
	coordScreen.Y = 8;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;

		coordScreen.X = 0;
		coordScreen.Y = 13;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<endl<<endl<<"... Press Enter to abort...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
		getchar();
		return;
	}

	// --------------------------------------------

	Status = rahmen_puls_test(Cardauswahl, ErrorMessage, ScreenOut, 0);

	coordScreen.X = 0;
	coordScreen.Y = 13;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_RED);
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;
	cout<<"                                                                               "<<endl;

	coordScreen.X = 20;
	coordScreen.Y = 9;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);

	if(Status == true)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
		cout<<" OK "<<endl;
	}
	else
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<ErrorMessage<<endl;

		coordScreen.X = 0;
		coordScreen.Y = 13;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);
		cout<<endl<<endl<<"... Press Enter to abort...";
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);
		getchar();
		getchar();
		return;
	}

	coordScreen.X = 0;
	coordScreen.Y = 13;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	cout<<".... Test beendet, press Enter to go back...";
	getchar();
}


void main()
{
	_WORD CardCounter = 0;
	_DWORD status	 = 0;
	_WORD Cardauswahl = 0;
	_DWORD counter	 = 0;
	_WORD myStatus	 = 0;
	_WORD auswahl	 = 0;

	_DWORD ErrorStatus= 0;

	BYTE EventValue = 0;
	BYTE CntrlReg	= 0;

	string ErrorMessage;

	_BYTE ErrorChar [24];
	char FensterTitel[30];
	_BYTE Version[20];

	bool Status = true;
	bool ScreenOut = false;

	int WindowsVersion = 0;
	
	// Zufallszahl ini

	srand((unsigned)time(NULL));

// -------------- Windows Version (NT oder 2000) ------------------

	OSVERSIONINFO lVerInfo;
	lVerInfo.dwOSVersionInfoSize = sizeof (lVerInfo);
	GetVersionEx (&lVerInfo);
//	WindowsVersion = lVerInfo.dwMajorVersion;
	WindowsVersion = lVerInfo.dwPlatformId;

// ----------- Fenstertitel zusammenfriemeln -----------------

	MILDRIVER_VersionNr(Version);
 
    sprintf( FensterTitel, "Dll %s",Version);
    
	SetConsoleTitle(FensterTitel);

// ----------- Ínformationen über den Bildschrim -------------- 

	hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo( hConsoleOut, &csbiInfo );

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
		exit(0);
	}
	else if (CardCounter > 1)
	{
		do
		{
			cout << "Select Card (1-" << CardCounter <<") :";
			cin >> Cardauswahl;
		}while((Cardauswahl > CardCounter)||(Cardauswahl <= 0));
		
		status =PCI_DriverOpen(Cardauswahl);
	}
	else
	{
		status = PCI_DriverOpen(1);
		Cardauswahl = 1;
	}

	PCI_ErrorString (status, ErrorChar);

	cout<<"ConnectMilBus :"<<ErrorChar<<endl;
	if(status != StatusOK)
	{
		cout<< "\nPress Enter to Exit....";
		getchar();
		getchar();
		exit(0);
	}
	

	while(1)
	{

		ClearScreen();
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  

		cout<<"---------- Version 0.2 -- Karten Nr. "<<Cardauswahl<<" -------------"<<endl<<endl;


		cout << "Current System is ";

/*		if(WindowsVersion == 4)
		{
			cout<<"Windows NT"<<endl<<endl;
		}
		else if(WindowsVersion == 5)
		{
			cout<<"Windows 2000"<<endl<<endl;
		}
		else
		{
			cout<<"95/98/ME"<<endl<<endl;
		}*/
	
		// OS- sytem feststellen
		//
		if(lVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if(lVerInfo.dwMajorVersion == 4)
			{
				cout<<"Windows NT"<<endl<<endl;
			}
			else if (lVerInfo.dwMajorVersion == 5 )
			{
				if (lVerInfo.dwMinorVersion == 0 )
				{
					cout<<"Windows 2000"<<endl<<endl;
				}
				else if (lVerInfo.dwMinorVersion == 1)
				{
					cout<<"Windows XP"<<endl<<endl;
				}
			}
		}
		else if(WindowsVersion == VER_PLATFORM_WIN32_WINDOWS)
		{
			cout<<"95/98/ME"<<endl<<endl;
		}

		cout<< "Automatik Test.............[1]"<<endl;
		cout<< "Singel Step Test...........[2]"<<endl;
		cout<< "\n\nLieber doch nichts.........[99]"<<endl<<endl;

		cout<<"Choose your destiny :";
		cin>>auswahl;

		switch (auswahl)
		{
		case 1: Automatik (Cardauswahl);
			break;
		case 2: SingelStep (Cardauswahl);
			break;

		default : SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);
				  cout<<"\nAdios...."<<endl<<endl;		  
				  PCI_TimerWait(Cardauswahl,1000,1,status);
				  PCI_DriverClose(Cardauswahl);
				  exit(0);
			break;
		}	
	}

}

