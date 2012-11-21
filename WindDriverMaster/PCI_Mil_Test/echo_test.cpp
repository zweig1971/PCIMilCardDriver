#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

unsigned long int counterInt = 0;

int ReadMilBusTimeOutInt;
int WriteMilBusTimeOutInt;
int VerifyFailtInt;
_WORD SendData;
_WORD ReadData;

bool BildschirmAusgabe = false;

bool echo_dauertest(int Cardauswahl, _WORD IFKNumber, string &ErrorMessage, bool ScreenOut)
{
	_DWORD myStatus	= 0;
	
	SendData = (counterInt % 0xFFFF);
	ReadData = 0;

	// Zugriff
	PCI_IfkWrite(Cardauswahl, IFKNumber, 0x13, SendData, myStatus);
	PCI_IfkRead (Cardauswahl, IFKNumber, 0x89, ReadData, myStatus);

	// Zähler hochsetzen
	if(myStatus & WriteMilBusTimeOut)
	{
		if(ScreenOut)
		{
			WriteMilBusTimeOutInt++;
			BildschirmAusgabe = true;
		}
		else
		{
			ErrorMessage = " ERROR ! Write MilBus Time Out ! ";
			return false;
		}
	}
			
	if(myStatus & ReadMilBusTimeOut)
	{
		if(ScreenOut)
		{
			ReadMilBusTimeOutInt++;
			BildschirmAusgabe = true;
		}
		else
		{
			ErrorMessage = " ERROR ! Read MilBus Time Out ! ";
			return false;
		}
	}
			
	if (ReadData != SendData)
	{
		if(ScreenOut)
		{
			VerifyFailtInt++;
			BildschirmAusgabe = true;
		}
		else
		{
			ErrorMessage = " ERROR ! Verify Read/Send Data ! ";
			return false;
		}
	}
	myStatus = 0;

	return true;
}


// Ein kleine IFK Test, ein datum wird gesendet und 
// danach wieder lesen
bool echo_test(_WORD Cardauswahl, _WORD IFKNumber,string &ErrorMessage, bool ScreenOut, int loop)
{

	ReadMilBusTimeOutInt = 0;
	WriteMilBusTimeOutInt= 0;
	VerifyFailtInt       = 0;
	SendData			 = 0;
	ReadData			 = 0;
	counterInt			 = 0;

	_DWORD myStatus	= 0;
	char answer;
	
	COORD coordScreen;
	COORD StatusCoord;
	
	StatusCoord.X = 4;
	StatusCoord.Y = 24;

	bool weiter = true;
	bool Status = true;


	string _counter			   = "Counter               : ";
	string _ReadMilBusTimeOut  = "Read  MilBus TimeOuts : ";
	string _WriteMilBusTimeOut = "Write MilBus TimeOuts : "; 
	string _VerifyFailt		   = "Verify  Failt         : ";
	string _SendData_ReadData  = "Send Data / Read Data : ";

	string StatusZeile ="  Press <SPACE> for singel step, <RETURN> to go on and <X> to exit  ";


	PCI_PCIcardReset(Cardauswahl, myStatus);

// ------------------------------ Maske -----------------------------------

	if(ScreenOut)
	{
		ClearScreen();

		coordScreen.X = 0;
		coordScreen.Y = 0;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);

		// Kopf
		coordScreen.X = 1;
		coordScreen.Y = 0;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_WHITE | FOREGROUND_RED);	
		cout<<" IFK Nummer : " <<(hex)<< IFKNumber <<(dec);

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_CYAN | FOREGROUND_RED);	
		cout<<"   Write Funktions Code: 0x13   ";

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_WHITE | FOREGROUND_RED);	
		cout<<" Read  Funktions Code : 0x89 ";

		coordScreen.X = 25;
		coordScreen.Y = 2;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_YELLOW | FOREGROUND_BLUE);
		cout<<" Karten Nr.: "<<Cardauswahl<<endl;
		// Counter:

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);   
		coordScreen.X = 0;
		coordScreen.Y = 5;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		
		cout<<_counter<<counterInt<<endl;

		// Der Rest
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);   
		coordScreen.X = 0;
		coordScreen.Y = 7;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);

		cout<< _ReadMilBusTimeOut  << ReadMilBusTimeOutInt <<endl; 
		cout<< _WriteMilBusTimeOut << WriteMilBusTimeOutInt<<endl; 
		cout<< _VerifyFailt        << VerifyFailtInt       <<endl<<endl; 

		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<< _SendData_ReadData  <<endl; 

	// --------------------------- endlos -----------------------------------

		coordScreen.X = 0;
		coordScreen.Y = 0;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);

		answer = ' ';
	}

	if(ScreenOut)
	{
		do
		{
			do
			{

				echo_dauertest(Cardauswahl, IFKNumber, ErrorMessage, ScreenOut);
			
			
				if(((counterInt % 1000) == 0) || (BildschirmAusgabe == true) || (answer == ' '))
				{
					// Counter
					coordScreen.X = 24;
					coordScreen.Y = 5;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);	
					cout<<counterInt<<endl;
				}

				if(BildschirmAusgabe == true)
				{
					// ReadMilBusTimeOut
					coordScreen.X = 24;
					coordScreen.Y = 7;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);   
					cout<<ReadMilBusTimeOutInt<<endl;

					//WriteMilBusTimeOut
					coordScreen.X = 24;
					coordScreen.Y = 8;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<WriteMilBusTimeOutInt<<endl;

					//VerifyFailt
					coordScreen.X = 24;
					coordScreen.Y = 9;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<VerifyFailtInt<<endl;

					//SendData ReadData
					coordScreen.X = 24;
					coordScreen.Y = 11;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);
					cout <<(hex)<< SendData <<" - " <<(hex)<< ReadData << (dec) << endl;
				}//if(BildschirmAusgabe == true)
					
				BildschirmAusgabe = false;		
				counterInt ++;

			}while (!((_kbhit()) || (answer == ' ') || (answer == 'X')));
			
			if (answer != 'X')
				ReadKey (answer, StatusZeile ,StatusCoord);
		}while(answer != 'X');
	}
	else
	{
		do
		{
			Status = echo_dauertest(Cardauswahl, IFKNumber, ErrorMessage, ScreenOut);
			counterInt ++;
		}while((counterInt <= loop) && (Status != false));
	}

	return Status;
}