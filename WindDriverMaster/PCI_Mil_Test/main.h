#ifndef _main_
#define _main_

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#include "PCIMilTreiber.h"

using namespace std;

#define BACKGROUND_WHITE  (WORD) 0x00f0 
#define BACKGROUND_CYAN   (WORD) 0x0030 
#define BACKGROUND_YELLOW (WORD) 0x0060
#define BACKGROUND_BLACK  (WORD) 0x0000
 
#define FOREGROUND_YELLOW (WORD) 0x0006 
#define FOREGROUND_CYAN   (WORD) 0x0003 
#define FOREGROUND_WHITE  (WORD) 0x0007  
#define FOREGROUND_BLACK  (WORD) 0x0000
 
WORD IFK_Auswahl(WORD Cardauswahl);

void ClearScreen( void );
void ReadKey (char &answer, string StatusZeile ,COORD StatusCoord);
bool echo_test(WORD Cardauswahl, WORD IFKNumber,string &ErrorMessage, bool ScreenOut, int loop);
bool fifo_test(WORD Cardauswahl, WORD IFKNumber, string &ErrorMessage, bool ScreenOut, int loop);
bool Read_Event_Test(WORD Cardauswahl, string &ErrorMessage, bool ScreenOut, int loop);
bool Event_Filter_Test(WORD Cardauswahl, string &ErrorMessage, bool ScreenOut, int loop);
bool pulse_tester(WORD Cardauswahl, string &ErrorMessage, bool ScreenOut, int loop);
bool rahmen_puls_test(WORD Cardauswahl,  string &ErrorMessage, bool ScreenOut, int loop);
void ResetKarte(WORD Cardauswahl); 
bool interrupt_test(WORD CardNr, string &ErrorMessage, bool ScreenOut, int loop);
void soft_puls_test(WORD Cardauswahl);
void soft_rahmen_puls_test(WORD Cardauswahl);
#endif