#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 


void soft_rahmen_puls_test(_WORD Cardauswahl)
{
	COORD coordScreen;
	COORD StatusCoord;
	
	StatusCoord.X = 4;
	StatusCoord.Y = 24;

	_DWORD myStatus	= 0;
	int Status		= 0;
	int counterInt	= 0;

	char answer;
	_BYTE ErrorChar [24];

	string StatusZeile ="  Press <SPACE> for singel step, <RETURN> to go on and <X> to exit  ";

	// Kopf
	ClearScreen();

	coordScreen.X = 1;
	coordScreen.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_WHITE | FOREGROUND_RED);	
	cout<<"Software Rahmen Puls Test";

	coordScreen.X = 28;
	coordScreen.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_YELLOW | FOREGROUND_BLUE);
	cout<<" Karten Nr.: "<<Cardauswahl<<endl;

	// -----------------------------------------------------------------

	// Reset Karte
	PCI_PCIcardReset(Cardauswahl, myStatus);

	// Rahmen Puls erwuenscht
	Status = PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls1_Rahmen_On ,myStatus);

	if(Status != StatusOK)
	{
		PCI_ErrorString(Status, ErrorChar);
		coordScreen.X = 1;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_BLACK);
		
		cout<<"Set Control Reg. Cntrl_Puls1_Rahmen_On Failure : "<<ErrorChar<<endl;
	
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);	
		cout<<"\n\nPress Enter to Exit....";
		getchar();
		getchar();
		return;
	}

	Status = PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls2_Rahmen_On ,myStatus);

	if(Status != StatusOK)
	{
		PCI_ErrorString(Status, ErrorChar);
		coordScreen.X = 1;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_BLACK);
		
		cout<<"Set Control Reg. Cntrl_Puls2_Rahmen_On Failure : "<<ErrorChar<<endl;
	
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);	
		cout<<"\n\nPress Enter to Exit....";
		getchar();
		getchar();
		return;
	}

	Status = PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls3_Rahmen_On ,myStatus);

	if(Status != StatusOK)
	{
		PCI_ErrorString(Status, ErrorChar);
		coordScreen.X = 1;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_BLACK);
		
		cout<<"Set Control Reg. Cntrl_Puls3_Rahmen_On Failure : "<<ErrorChar<<endl;
	
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);	
		cout<<"\n\nPress Enter to Exit....";
		getchar();
		getchar();
		return;
	}

	Status = PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls4_Rahmen_On ,myStatus);

	if(Status != StatusOK)
	{
		PCI_ErrorString(Status, ErrorChar);
		coordScreen.X = 1;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_BLACK);
		
		cout<<"Set Control Reg. Cntrl_Puls4_Rahmen_On Failure : "<<ErrorChar<<endl;
	
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);	
		cout<<"\n\nPress Enter to Exit....";
		getchar();
		getchar();
		return;
	}

	Status = PCI_CntrlRegSet(Cardauswahl, Cntrl_Puls5_Rahmen_On ,myStatus);

	if(Status != StatusOK)
	{
		PCI_ErrorString(Status, ErrorChar);
		coordScreen.X = 1;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_BLACK);
		
		cout<<"Set Control Reg. Cntrl_Puls5_Rahmen_On Failure : "<<ErrorChar<<endl;
	
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);	
		cout<<"\n\nPress Enter to Exit....";
		getchar();
		getchar();
		return;
	}

	coordScreen.X = 0;
	coordScreen.Y = 5;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE); 
		
	cout<<"Puls Counter : "<<endl;

	coordScreen.X = 0;
	coordScreen.Y = 7;
	SetConsoleCursorPosition(hConsoleOut, coordScreen);
	SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

	cout<<"Puls-Buchse Nr.: ";

	
	answer = ' ';
	do
	{
		do
		{
			if((counterInt % 2) == 0 || (answer == ' '))
			{
				// Counter
				coordScreen.X = 16;
				coordScreen.Y = 5;
				SetConsoleCursorPosition(hConsoleOut, coordScreen);
				SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);	
				cout<<counterInt<<endl;
			}

			if(answer == ' ')
			{
				ReadKey (answer, StatusZeile ,StatusCoord);
			}
			coordScreen.X = 18;
			coordScreen.Y = 7;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK| FOREGROUND_WHITE);
			cout<<"1"<<endl;
			PCI_TimerWait(Cardauswahl, 400, 1, myStatus);
			PCI_SoftPuls1Start(Cardauswahl, myStatus);
			PCI_TimerWait(Cardauswahl, 1000, 1, myStatus);
			PCI_SoftPuls1Stop(Cardauswahl, myStatus);

			if(answer == ' ')
			{
				ReadKey (answer, StatusZeile ,StatusCoord);
			}
			coordScreen.X = 18;
			coordScreen.Y = 7;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK| FOREGROUND_WHITE);
			cout<<"2"<<endl;
			PCI_TimerWait(Cardauswahl, 400, 1, myStatus);
			PCI_SoftPuls2Start(Cardauswahl, myStatus);
			PCI_TimerWait(Cardauswahl, 1000, 1, myStatus);
			PCI_SoftPuls2Stop(Cardauswahl, myStatus);

			if(answer == ' ')
			{
				ReadKey (answer, StatusZeile ,StatusCoord);
			}
			coordScreen.X = 18;
			coordScreen.Y = 7;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK| FOREGROUND_WHITE);
			cout<<"3"<<endl;
			PCI_TimerWait(Cardauswahl, 400, 1, myStatus);
			PCI_SoftPuls3Start(Cardauswahl, myStatus);
			PCI_TimerWait(Cardauswahl, 1000, 1, myStatus);
			PCI_SoftPuls3Stop(Cardauswahl, myStatus);

			if(answer == ' ')
			{
				ReadKey (answer, StatusZeile ,StatusCoord);
			}
			coordScreen.X = 18;
			coordScreen.Y = 7;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK| FOREGROUND_WHITE);
			cout<<"4"<<endl;
			PCI_TimerWait(Cardauswahl, 400, 1, myStatus);
			PCI_SoftPuls4Start(Cardauswahl, myStatus);
			PCI_TimerWait(Cardauswahl, 1000, 1, myStatus);
			PCI_SoftPuls4Stop(Cardauswahl, myStatus);

			if(answer == ' ')
			{
				ReadKey (answer, StatusZeile ,StatusCoord);
			}
			coordScreen.X = 18;
			coordScreen.Y = 7;
			SetConsoleCursorPosition(hConsoleOut, coordScreen);
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK| FOREGROUND_WHITE);
			cout<<"5"<<endl;
			PCI_TimerWait(Cardauswahl, 400, 1, myStatus);
			PCI_SoftPuls5Start(Cardauswahl, myStatus);
			PCI_TimerWait(Cardauswahl, 1000, 1, myStatus);
			PCI_SoftPuls5Stop(Cardauswahl, myStatus);

			counterInt ++;

		}while (!((_kbhit()) || (answer == ' ') || (answer == 'X')));
			
		if (answer != 'X')
			ReadKey (answer, StatusZeile ,StatusCoord);

	}while(answer != 'X');

	return;
}