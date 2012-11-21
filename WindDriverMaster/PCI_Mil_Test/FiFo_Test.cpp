#include "main.h"

extern HANDLE  hConsoleOut;
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

// Testet den Mil Fifo in den unteren 8 Bit
//
bool fifo_test(_WORD Cardauswahl, _WORD IFKNumber, string &ErrorMessage, bool ScreenOut, int loop)
{
	_DWORD ErrorStatus= 0;
	_DWORD StatusReg  = 0;
	_WORD counter    = 0;

	int HelpCounter = 1;
	int VerifyFailtInt= 0;
	int dummy = 0;
	int myFiFo[512] = {0};

	int FiFo_Ueberlaufe	 = 0;
	int FiFo_Unterlauf	 = 0;
	int Fehlerhafte_Daten= 0;	

	_WORD bufferCounter_eins = 0;
	_WORD bufferCounter_zwei = 0;

	_WORD time		= 7;
	_WORD SendeDaten = 0;
	_WORD ReadDaten	= 0; 
	_WORD OrginalDaten = 0;
	_WORD SollDaten	= 0;
	_WORD IstDaten	= 0;

	bool Fifo_not_empty    = true;
	bool BildschirmAusgabe = false;

	BYTE FunktionCodeEins = 0x13;
	BYTE FunktionCodeZwei = 0x89;
	_WORD data = 0;

	char answer; 
	
	string ausgabe_eins;
	string ausgabe_zwei;
	string StatusZeile ="  Press <SPACE> for singel step, <RETURN> to go on and <X> to exit  ";

	COORD coordScreen;
	COORD StatusCoord;
	
	StatusCoord.X = 4;
	StatusCoord.Y = 24;

	_DWORD dauertestCounter = 0;

	if(ScreenOut)
	{
		ClearScreen();
	}

	PCI_PCIcardReset(Cardauswahl, ErrorStatus);
	
	// FiFo einschalten
	//
	PCI_CntrlRegSet (Cardauswahl, Cntrl_Mil_FiFo_On, ErrorStatus);

	// Status Lesen
	//
	PCI_StatusRegRead (Cardauswahl, StatusReg, ErrorStatus);


	if(ScreenOut)
	{
		// Ueberschrift ausgeben
		//
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);   
		cout<<"Kleiner FIFO Test :"<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		coordScreen.X = 30;
		coordScreen.Y = 0;
		SetConsoleCursorPosition(hConsoleOut, coordScreen);
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_YELLOW | FOREGROUND_BLUE);
		cout<<" Karten Nr.: "<<Cardauswahl<<endl;
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);

		cout<<"\nFIFO laesst sich ";
	}

	// Prüfen, ob Fifo eingeschaltet ist
	//
	if(StatusReg & Mil_Fifo_On)
	{	
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_GREEN | FOREGROUND_WHITE);  
			cout<<"setzen ! "<<endl<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
		}
	}
	else
	{
		if(ScreenOut)
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<"NICHT setzen !!!"<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
			cout<<"Press Enter to go back......"<<endl;
			getchar();
			getchar();
			return false;
		}
		else
		{
			ErrorMessage = "ERROR ! FiFo laesst sich NICHT setzen !";
			return false;
		}
	}


	// Daten zur IFK senden und mit FunktiosCode wieder zurücklesen
	// bis entweder Fifo voll oder Loop Maximal ist (schlecht)
	//
	counter = 1;
	while (!(PCI_StatusTest(Cardauswahl, Mil_Fifo_Full,ErrorStatus))&&((counter % 0xFFFF) != 0))
	{
		SendeDaten = ~counter; // Negieren
		
		// Daten an IFK versenden
		PCI_IfkWrite (Cardauswahl,IFKNumber,FunktionCodeEins,SendeDaten,ErrorStatus);
		// Daten von IFK lesen
		PCI_FctCodeSnd(Cardauswahl, IFKNumber, FunktionCodeZwei, ErrorStatus);

		// Warten, ca 70us wichtig !!
		PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);

		SendeDaten = counter;
		
		// Daten an IFK versenden
		PCI_IfkWrite (Cardauswahl, IFKNumber, FunktionCodeEins,SendeDaten,ErrorStatus);
		// Daten von IFK lesen
		PCI_FctCodeSnd(Cardauswahl, IFKNumber, FunktionCodeZwei, ErrorStatus);

		// Warten, ca 70us wichtig !!
		PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);		

		counter++;
	}

	// Fifo SOLLTE voll sein
	if(PCI_StatusTest(Cardauswahl, Mil_Fifo_Full, ErrorStatus))
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
			ErrorMessage = " ERROR ! FiFo zeigt nicht voll an ! ";
			return false;
		}
	}

	if(ScreenOut)
	{
		cout << "Es Wurden " << 2*(counter-1) << " Daten von der IFK angefordert ! "<<endl;
		cout <<"Das Fifo ist " << ausgabe_eins << " ! "<<endl<<endl;
	}

	// Anzahl der Reingeschriebenen Daten merken
	// diese muessen gleich den gelesenen Daten aus dem Fifo sein
	dummy = 2*(counter-1);

	counter = 1;
	VerifyFailtInt = 0;

	// Solange lesen, bis Fifo leer ist oder Max. ueberschritten
	//
	while((Fifo_not_empty) && ((counter % 0xFFFF) != 0)) 
	{		
		// Daten aus Fifo lesen
		//
		PCI_MilBusRead(Cardauswahl, ReadDaten,Fifo_not_empty,ErrorStatus);
		
		// Daten pruefen
		//
		if(ReadDaten != ((_WORD)~counter))
		{
			VerifyFailtInt++;
		}

		// Daten aus Fifo lesen
		//
		PCI_MilBusRead(Cardauswahl, ReadDaten,Fifo_not_empty,ErrorStatus);
		
		// Daten pruefen
		//
		if(ReadDaten != counter)
		{
			VerifyFailtInt++;
		}
		counter++;	
	}

	// Ausgabe
	//
	if(ScreenOut)
	{
		cout<<"Es wurden "<<2*(counter-1)<<" Daten aus dem Fifo gelesen"<<endl;
		cout<<"Davon sind "<<VerifyFailtInt<< " Fehlerhaft."<<endl<<endl;
	}

	if(ScreenOut)
	{
		if(!((dummy == (2*(counter-1))) && (VerifyFailtInt == 0)))
		{
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_RED | FOREGROUND_WHITE);  
			cout<<"Der Test war Fehlerhaft !!" <<endl<<endl;
			SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
			cout<< "..press ENTER to EXIT..."<<endl;
			getchar();
			getchar();
			return false;
		}
	}
	else
	{
		if (dummy != (2*(counter-1)))
		{
			ErrorMessage = " ERROR ! Anzahl Lesen/Schreiben ungleich ! ";
			return false;
		}
		else if(VerifyFailtInt != 0)
		{
			ErrorMessage = " ERROR ! Verify Read/Write ! ";
			return false;
		}
		else if (2*(counter-1) != 512)
		{
			ErrorMessage = " ERROR ! FiFo Eintraeger ungleich 512 ! ";
			return false;
		}
	}

//---------------------------------------------
//----------- Dauertest -----------------------
//---------------------------------------------
	if(ScreenOut)
	{
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLUE | FOREGROUND_WHITE);  
		cout<<" Dauertest "<<endl<<endl;

		answer = ' ';
		FiFo_Ueberlaufe	 = 0;
		FiFo_Unterlauf	 = 0;
		Fehlerhafte_Daten= 0;	
		dauertestCounter = 0;
		
		SetConsoleTextAttribute(hConsoleOut, BACKGROUND_BLACK | FOREGROUND_WHITE);  
		cout<<"Test durchlaeufe       : "<<dauertestCounter<<endl<<endl;
		cout<<"FiFo Ueberlaufe        : "<<FiFo_Ueberlaufe<<endl;
		cout<<"FiFo Unterlauf         : "<<FiFo_Unterlauf<<endl;
		cout<<"Fehlerhafte Daten      : "<<Fehlerhafte_Daten<<endl;
		cout<<"Soll / Ist Daten       : "<<"- / -"<<endl;	

		do
		{
			do
			{

	//---------------- FIFO vollschreiben --------------------

				bufferCounter_eins = bufferCounter_eins + counter;
				
				if(bufferCounter_eins >= 65280)
					bufferCounter_eins = 0;
				
				bufferCounter_zwei = bufferCounter_eins;
				
				counter		= 1;
				SendeDaten	= 0;
				ErrorStatus = 0;

				dauertestCounter++;

				while (!(PCI_StatusTest(Cardauswahl, Mil_Fifo_Full,ErrorStatus))&&((counter % 0x102) != 0))
				{
					SendeDaten = ~(bufferCounter_eins+counter); // Negieren
					
					// Daten an IFK versenden
					PCI_IfkWrite (Cardauswahl,IFKNumber,FunktionCodeEins,SendeDaten,ErrorStatus);
					// Daten von IFK lesen
					PCI_FctCodeSnd(Cardauswahl, IFKNumber, FunktionCodeZwei, ErrorStatus);

					// Warten, ca 70us wichtig !!
					PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);

					SendeDaten = (bufferCounter_eins+counter);
					
					// Daten an IFK versenden
					PCI_IfkWrite (Cardauswahl,IFKNumber,FunktionCodeEins,SendeDaten,ErrorStatus);
					// Daten von IFK lesen
					PCI_FctCodeSnd(Cardauswahl, IFKNumber, FunktionCodeZwei, ErrorStatus);

					// Warten, ca 70us wichtig !!
					PCI_TimerWait (Cardauswahl, time, Timerbase_10us, ErrorStatus);

					counter++;
				}

				// Fifo SOLLTE voll sein
				if(!(PCI_StatusTest(Cardauswahl, Mil_Fifo_Full, ErrorStatus)))
				{
					FiFo_Ueberlaufe++;
					BildschirmAusgabe = true;
				}			
			
				if((2*(counter-1)) < 512)
				{
					FiFo_Unterlauf++;
					BildschirmAusgabe = true;
				}

	//---------------- FIFO auslesen --------------------

				counter = 1;
				Fifo_not_empty = true;

				// Solange lesen, bis Fifo leer ist oder Max. ueberschritten
				//
				while((Fifo_not_empty) && ((counter % 0xFFFF) != 0)) 
				{		
					// Daten aus Fifo lesen
					//
					PCI_MilBusRead(Cardauswahl, ReadDaten,Fifo_not_empty,ErrorStatus);
					
					// Daten pruefen
					//
					if(ReadDaten != ((_WORD)~(bufferCounter_zwei+counter)))
					{
						Fehlerhafte_Daten++;
						SollDaten =((_WORD)~(bufferCounter_zwei+counter));
						IstDaten  = ReadDaten;
						BildschirmAusgabe = true;
					}

					// Daten aus Fifo lesen
					//
					PCI_MilBusRead(Cardauswahl, ReadDaten,Fifo_not_empty,ErrorStatus);
					
					// Daten pruefen
					//
					if(ReadDaten != (bufferCounter_zwei+counter))
					{
						Fehlerhafte_Daten++;
						SollDaten = (bufferCounter_zwei+counter);
						IstDaten  = ReadDaten;
						BildschirmAusgabe = true;
					}
					counter++;	
				}				
				
				//Ausgabe des daurchlaufes

				if((BildschirmAusgabe) || (!(dauertestCounter % 10)) || (answer == ' '))
				{
					//dauertestCounter
					coordScreen.X = 25;
					coordScreen.Y = 12;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<dauertestCounter<<endl;
				}

				if(BildschirmAusgabe)
				{
					//FiFo_Ueberlaufe
					coordScreen.X = 25;
					coordScreen.Y = 14;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<FiFo_Ueberlaufe<<endl;

					//FiFo_Unterlauf
					coordScreen.X = 25;
					coordScreen.Y = 15;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<FiFo_Unterlauf<<endl;

					//Fehlerhafte_Daten
					coordScreen.X = 25;
					coordScreen.Y = 16;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<Fehlerhafte_Daten<<endl;

					//Soll/Ist Daten
					coordScreen.X = 25;
					coordScreen.Y = 17;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<"                       "<<endl;

					coordScreen.Y = 17;
					SetConsoleCursorPosition(hConsoleOut, coordScreen);
					cout<<hex<<SollDaten<<"/"<<IstDaten<<dec<<endl;
					
					BildschirmAusgabe = false;
				}

			}while (!((_kbhit()) || (answer == ' ') || (answer == 'X')));		
			if (answer != 'X')
				ReadKey (answer, StatusZeile ,StatusCoord);
		}while(answer != 'X');
	}
	return true;
  }