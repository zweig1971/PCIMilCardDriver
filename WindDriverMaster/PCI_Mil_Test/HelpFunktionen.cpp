#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

// Prüft ob ein key gedrückt wurde, und gibt diesen zurück
// ausserdem gibt es eine Status zeile an belibiger stelle
// im Fenster zurück
void ReadKey (char &answer, string StatusZeile ,COORD StatusCoord)
{
	int ch;

	if(answer == ' ')
	{
		SetConsoleCursorPosition(hConsoleOut, StatusCoord); 
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_YELLOW | FOREGROUND_WHITE);	
		cout<<StatusZeile;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
		while(!_kbhit());
	}
		ch	 = _getch();
		ch	 = toupper(ch);
		answer = ch;	
}


// Löscht den inhalt das kleinen Ausgabefensters
void ClearScreen( void )
{
	COORD coordScreen;
    DWORD    dummy;
    COORD    Home = { 0, 0 };
	FillConsoleOutputAttribute( hConsoleOut, BACKGROUND_BLACK, csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	FillConsoleOutputCharacter( hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy );
	coordScreen.X = 0;
	coordScreen.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, coordScreen); 
}


// IFKs suchen, anzeigen und auswählen
//
_WORD IFK_Auswahl(_WORD Cardauswahl)
{
	_WORD counter		  = 0;
	_WORD CounterFindCards = 0;
	_DWORD myStatus		  = 0;
	_WORD IFKNumber		  = 0;
	int Found			  = 0;
	BYTE ReturnIFKNr	  = 0;

	cout<< "\nFollowing IFK found :"<<endl;
	cout<< "============================"<<endl<<endl;

	counter =1;
	while(counter != 255)
	{
		Found = PCI_IfkOnline(Cardauswahl, counter, ReturnIFKNr, myStatus);
		if(Found == StatusOK)
		{
			cout<<"FFK Adress (hex) : "<<hex<<counter<<dec<<endl;
			CounterFindCards++;
		}	
		counter++;
	}

	myStatus = 0;
	counter  = 0;
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

	if(CounterFindCards == 0)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN  | FOREGROUND_RED);   
		cout<<"\n\n Sorry, No IFK Cards Found !!"<<endl;
		cout<<" ----------------------------"<<endl;
		SetConsoleTextAttribute(hConsoleOut, FOREGROUND_WHITE);   
		getchar();
		getchar();
		return(0);

	}		
	else if (CounterFindCards == 1)
	{
		PCI_TimerWait(Cardauswahl,1000,1,myStatus);
		return(ReturnIFKNr);
	}

	cout<<"\n\nWelches Schweindel haetten Sie den gerne :";
	cin>>(hex)>>IFKNumber;

	return (IFKNumber);
}

void ResetKarte(_WORD Cardauswahl)
{
	_DWORD ErrorStatus = 0;
	_WORD  status	  = 0;

	_BYTE ErrorChar [26];

	ClearScreen();

	cout<< " R E S E T !"<<endl;
	cout<< " ==========="<<endl<<endl;

	status = PCI_PCIcardReset(Cardauswahl, ErrorStatus);

	PCI_ErrorString (status, ErrorChar);

	if (status != StatusOK)
	{
		cout<<ErrorChar<<endl;
		getchar();
		getchar();
	}

	cout<<ErrorChar<<endl;
}