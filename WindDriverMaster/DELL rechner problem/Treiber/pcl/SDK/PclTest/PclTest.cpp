//***************************************************************************
//*																			*
//*	Demo and Test Program for Windows DLL AVPCL.DLL							*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*																			*
//*	This program reads several data from the PCL PCI device driver			*
//*																			*
//***************************************************************************

// <pcltest.cpp> Alpermann+Velte DLL for AV-PCL
// @(#)pcltest.cpp 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte
#define VER_S     "3.36"

//---------------------------------------------------------------------------
#ifdef _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

#elif defined(LINUX)
// Linux compile command:
// gcc -o PclTest PclTest.c -DLINUX -L. -lavpcl32 -lncurses
#include <curses.h>
#include <stdlib.h>

#ifndef __USE_BSD
#	define __USE_BSD
#	include <string.h>
#	include <unistd.h>
#	undef __USE_BSD
#else
#	include <string.h>
#	include <unistd.h>
#endif

typedef bool BOOL;

#endif // defined(LINUX)

#include "avpcl.h"

//---------------------------------------------------------------------------

#ifdef LINUX
#define printfw printw
#else
#define printfw printf
#endif

//---------------------------------------------------------------------------

void gotoxy(int x, int y) // set cursor to new position of the screen
{
#ifdef _WIN32
	HANDLE hStdout;
	COORD coordCursor; // new cursor position coordinates

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	coordCursor.X = (short) (x - 1);
	coordCursor.Y = (short) (y - 1);

	SetConsoleCursorPosition(
		hStdout, // handle of console screen buffer
		coordCursor // new cursor position coordinates
	);
#elif defined(LINUX)
	move(y-1, x-1);
#endif
}

//---------------------------------------------------------------------------

void clrscr(void) // clear screen
{
#ifdef _WIN32
	DWORD dummy;
	COORD Home = { 0, 0 };
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;	// Console information

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo( hStdout, &csbiInfo );

	FillConsoleOutputCharacter(
		hStdout, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y,
		Home, &dummy
	);
#elif defined(LINUX)
	initscr();		// start ncurses mode
	refresh();
#endif
}


//---------------------------------------------------------------------------

void clreol(void) // clear from cursor position to end of line
{
#ifdef _WIN32
	DWORD dummy;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;	// Console information

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo( hStdout, &csbiInfo );

	FillConsoleOutputCharacter(
		hStdout, ' ', csbiInfo.dwSize.X - csbiInfo.dwCursorPosition.X,
		csbiInfo.dwCursorPosition, &dummy
	);
#elif defined(LINUX)
	int cx, cy, cols, rows;
	getmaxyx(stdscr, rows, cols);
	getyx(stdscr, cy, cx);
	int n;
	for (n=0; n < cols - cx; n++){
		printw(" ");
	} // end for
	move(cy, cx);
#endif
}

//---------------------------------------------------------------------------

void cursorVisible(BOOL show) // hide or show cursor
{
#ifdef _WIN32
	HANDLE hStdout;
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hStdout, &ConsoleCursorInfo);
	ConsoleCursorInfo.bVisible = show;
	SetConsoleCursorInfo(hStdout, &ConsoleCursorInfo);
#elif defined(LINUX)
	curs_set(show);
#endif
}

//---------------------------------------------------------------------------

#ifdef LINUX

void Sleep(long ms)
{
	usleep(ms * 1000);
}

#endif

//---------------------------------------------------------------------------

BOOL legalBase(int base) // is base a legal address of a PCI card?
{
	if (base > PCL_BASE_PCI_OFS && base <= PCL_BASE_PCI_LAST) return(TRUE);
	if (base < PCL_BASE_0) return(FALSE);
	if (base > PCL_BASE_F) return(FALSE);
	if ((base & 0x1F) != 0) return(FALSE);
	return(TRUE);
}

//---------------------------------------------------------------------------

const char *err2msg(int result) // convert PCL error number to string
{
	return(
		result == PCL_NO_NEWD       ? "No new data" :
		result == PCL_OK            ? "OK" :
		result == PCL_ERR_INV_PORT  ? "Invalid port id" :
		result == PCL_ERR_NOT_FOUND ? "PCL hardware not found" :
		result == PCL_ERR_NO_HANDLE ? "No free handle found" :
		result == PCL_ERR_INV_HDL   ? "Invalid handle" :
		result == PCL_ERR_NOTOPEN   ? "Handle not opened by pclOpen()" :
		result == PCL_ERR_NULL      ? "Null pointer assignment" :
		result == PCL_ERR_TIMEOUT   ? "Timeout" :
		result == PCL_ERR_INV_REG   ? "Register not available" :
		result == PCL_ERR_INV_BANK  ? "Invalid transmit bank number" :
		result == PCL_ERR_RANGE     ? "Parameter out of range" :
		result == PCL_ERR_INIT      ? "Can't initialize library" :
		result == PCL_ERR_IOCTL     ? "Error in device driver while"
										" IOCTL" :
		result == PCL_ERR_VER       ? "Wrong Version of device driver" :
		result == PCL_ERR_NOT_SUPP	? "Function not supported" :
		result == PCL_ERR_UK_CMD	? "Unknown command" :
		result == PCL_ERR_OPEN		? "Can't open device driver" :
									  "(Unknown)"
	);
}

//---------------------------------------------------------------------------

void fatal(				// handle fatal errors
	BOOL fullscreen,	// fullscreen mode
	char *reason,		// string describing failure
	int result			// return code
) {
#ifdef LINUX
	cbreak();
#else
	if (fullscreen) gotoxy(1, 15);
#endif
	printfw(
		"Fatal Error while %s: %s (%d)",
		reason, err2msg(result), result
	);
	printfw(
		"\n%s\n"
		"Press any key to continue...",
		pclGetErrorString()
	);
	getch();
#ifdef LINUX
	endwin();
#endif
	exit(1);
}

//---------------------------------------------------------------------------

void warning(			// show warning
	BOOL fullscreen,	// fullscreen mode
	int result			// return code
) {
	char *s;

	if (fullscreen) gotoxy(1, 20);
	printfw(err2msg(result));
	clreol();
	s = strdup(pclGetErrorString());
	if (s[0] != '\0') s[strlen(s) - 1] = '\0';
	printfw("\n%s", s);
	clreol();
	free(s);
}

//---------------------------------------------------------------------------

int main( // main function
	int argn,
	char **argc
) {
#ifdef LINUX
	initscr();
	raw();			// disable line buffering
	keypad(stdscr, TRUE);	// make F1, F2, ... accessible
	noecho();		// don't echo on getch
	halfdelay(1);		// wait 1/10 th second for key press
#endif // LINUX

	int result;						// return code of pclXxx() functions
	int hdl;						// handle to access pcl
	int base = PCL_BASE_PCI_1;		// address of pcl card
	BOOL running = TRUE;
	int ident, bytea, byteb, bytec, firmware, dll;

	if (argn > 1) {
		if        (sscanf(argc[1], "%d",   &base) == 1 && legalBase(base)) {
		} else if (sscanf(argc[1], "%x",   &base) == 1 && legalBase(base)) {
		} else if (sscanf(argc[1], "0x%x", &base) == 1 && legalBase(base)) {
		} else if (sscanf(argc[1], "0X%x", &base) == 1 && legalBase(base)) {
		} else if (sscanf(argc[1], "$%x",  &base) == 1 && legalBase(base)) {
		} else {
#ifdef LINUX
			cbreak();
#endif
			printfw(
				"Error in parameter base_address: %s\n"
				"Please try:  PCLTEST base_address\n\n"
				"Press any key to continue...",
				argc[1]
			);
			getch();
#ifndef LINUX
			exit(1);
#else
			endwin();
			return 1;	
#endif
		}
	}
	if ((result = pclOpen(&hdl, base)) != PCL_OK) {
									// open pcl card and get handle
		printfw(
			"Unable to find PCL card at address %d (0x%X)\n"
			"Please try:  PCLTEST base_address\n\n",
			base, base
		);
		fatal(FALSE, "pclOpen", result);
	}
//	the following forces a power-on reset of the PCL board, if needed

	if ((result = pclReset(hdl, PCL_IMODE_POR)) < PCL_OK) {
		warning(FALSE, result);
	}

	{ // get some version information from pcl
		if ((result = pclGetVersion(hdl, &ident, &firmware, &dll))
			== PCL_OK
		) {	// get version information
			if ((result = pclGetVerSpecial(hdl, &bytea, &byteb, &bytec))
				!= PCL_OK
			) {	// get special version information
				bytea = byteb = bytec = 0;
				warning(FALSE, result);
			}
		} else {
			ident = firmware = dll = bytea = byteb = bytec = 0;
			warning(FALSE, result);
		}
		if (dll < PCL_VER_DLL) {	// check if DLL version is o.k.
			gotoxy(1, 15);
			printfw(
				"Warning: Version of DLL is V%d.%02d, "
				"but has to be V%d.%02d or higher.\n",
				dll >> 8, dll & 0xff,
				PCL_VER_DLL >> 8, PCL_VER_DLL & 0xff
			);
		}
	}

	// set the vitc reader mode.  we want to read from line 6 to 22
	if ((result = pclVitcSetup(hdl, 6, 22, PCL_VLINE_BLOCK)) < PCL_OK) {
		warning(FALSE, result);
	}

	// set timecode priority for mixed register
	if ((result = pclPriority(hdl, PCL_PRIOR_VLC)) < PCL_OK) {
		warning(FALSE, result);
	}

	//	for demonstration we want to get VITC and CTL information synchronus.
	//	so we only get _new_ data from device driver.  for the first time,
	//	we have to do a dummy read to be sure not to get obsolete data
	if ((result = pclGetRegSync(hdl, PCL_REG_VITC,
		NULL, NULL, NULL, NULL, NULL)) < PCL_OK
	) {
		warning(FALSE, result);
	}
	if ((result = pclGetRegSync(hdl, PCL_REG_LTC,
		NULL, NULL, NULL, NULL, NULL)) < PCL_OK
	) {
		warning(FALSE, result);
	}

	cursorVisible(FALSE);	// hide cursor
	clrscr(); // first clear screen
	gotoxy(1,1); // build screen mask
	printfw(
		"Alpermann+Velte PCL Cards - PclTest Version " VER_S "\n"
		"\n"
		"Mixed      (asyn) : --:--:--:--  --------\n"
		"\n"
		"VITC       (sync) : --:--:--:--  --------\n"
		"LTC        (sync) : --:--:--:--  --------\n"
		"\n"
		"ATC LTC    (sync) : --:--:--:--  --------\n"
		"ATC VITC 1 (sync) : --:--:--:--  --------\n"
		"ATC VITC 2 (sync) : --:--:--:--  --------\n"
		"\n"
		"HANC LTC   (sync) : --:--:--:--  --------\n"
		"HANC VITC  (sync) : --:--:--:--  --------\n"
		"\n"
		"Version           : --\n"
		"\n"
		"eX)it (esc)"
	);

	gotoxy(21, 15);
	printfw(
		"%s, Firmware: V%d, %s (%c%c/%c), DLL: V%d.%02d",
		(firmware & 0xc0) == 0x00 ? "PCL6 or PCL3" :
		(firmware & 0xc0) == 0x80 ? "PCL5 or PCL10" :
		(firmware & 0xc0) == 0xc0 ? "PCL12" :
		(firmware & 0xc0) == 0x40 ? "PCL PCI" : "unknown",
			// pcl identifier is coded in bit 6 and 7 of
			//	firmware version
		firmware & 0x3f,
			// pcl firmware version
		ident ? "special" : "no special",
		byteb, bytea, bytec,	// special oem version of pcl?
		dll >> 8, dll & 0xff	// version of device driver
	);

	while (running) { // main program loop
		int ch;

#ifdef LINUX
		while ((ch = getch()) == ERR) {
#else
		while (!kbhit()) { // there isn't any keystroke
#endif
			PCL_TC time;
			PCL_USER user;
			int	bits, frame, newd, stat;

			{ // read mixed register
				if ((result = pclGetTc(hdl, &time)) == PCL_OK) {
								// read data from the mixed register
					gotoxy(21, 3);
					printfw(
						"%2x:%02x:%02x:%02x",
							// time is returned in bcd format.
							//	by using the hex format string, it's
							//	possible to show it without
							//	converting it to binary
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time))
					);
				} else {
					warning(TRUE, result);
				}

				printfw("  ");
				if ((result = pclGetUser(hdl, &user)) == PCL_OK) {
					printfw("%08lx", user);
				} else {
					warning(TRUE, result);
				}
				printfw("  ");
				if ((result = pclGetRegister(hdl, PCL_REG_MIXED, NULL, NULL, NULL, NULL, &newd)) == PCL_OK) {
					printfw(
						"%s",
						newd & PCL_NEWD_VITC ? "VITC" :
						newd & PCL_NEWD_LTC  ? "LTC " :
						newd & PCL_NEWD_CTL  ? "CTL " :
												""
					);
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read vitc register
				if ((result = pclGetRegSync(
					hdl, PCL_REG_VITC, &time, &bits, &user, &frame, NULL)) == PCL_OK
				) {
					// read data from the vitc register.  for demonstration only,
					//	we do it synchronus
					gotoxy(21, 5);
					printfw(	// show time and user information
						"%2x:%02x:%02x:%02x  %08lx  V%d",
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time)),
						user,
						((bits & 
							(frame == PCL_FRAME_25 ?
								PCL_BITS_VITC_PF : PCL_BITS_VITC_NF)
						) ? 2 : 1)
					);
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read ltc register
				int hr, mi, se, fr;

				if ((result = pclGetRegSync(hdl, PCL_REG_LTC, &time, NULL, &user, NULL, NULL)) == PCL_OK) {
					// read data from the ltc register.
					pclBcd2Tc(time, &hr, &mi, &se, &fr);
					// another way to show timecode data is
					//	first to convert it to binary
					gotoxy(21, 6);
					printfw("%2d:%02d:%02d:%02d  %08lx", hr, mi, se, fr, user);
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read atc ltc register
				if ((result = pclGetRegSync(
					hdl, PCL_REG_ATC_LTC, &time, &bits, &user, &frame, NULL)) == PCL_OK
				) {
					gotoxy(21, 8);
					printfw(	// show time and user information
						"%2x:%02x:%02x:%02x  %08lx",
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time)),
						user
					);
					if ((result = pclGetRegStatus(hdl, PCL_REG_ATC_LTC, &stat)) == PCL_OK) {
						printfw(	// show status information
							"  %c %c",
							stat & PCL_STAT_DBB2_TC_VALIDITY ? 'V' : ' ',
							stat & PCL_STAT_DBB2_PROCESS_BIT ? 'P' : ' '
						);
					} else {
						warning(TRUE, result);
					}
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read atc vitc 1 register
				if ((result = pclGetRegSync(
					hdl, PCL_REG_ATC_VITC1, &time, &bits, &user, &frame, NULL)) == PCL_OK
				) {
					gotoxy(21, 9);
					printfw(	// show time and user information
						"%2x:%02x:%02x:%02x  %08lx  V%d",
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time)),
						user,
						((bits & 
							(frame == PCL_FRAME_25 ?
								PCL_BITS_VITC_PF : PCL_BITS_VITC_NF)
						) ? 2 : 1)
					);
					if ((result = pclGetRegStatus(hdl, PCL_REG_ATC_VITC1, &stat)) == PCL_OK) {
						printfw(	// show status information
							"  %c %c  Line %2d %c",
							stat & PCL_STAT_DBB2_TC_VALIDITY ? 'V' : ' ',
							stat & PCL_STAT_DBB2_PROCESS_BIT ? 'P' : ' ',
							(stat & PCL_STAT_DBB2_VITC_LINE_SELECT) >> 8,
							stat & PCL_STAT_DBB2_VITC_LINE_DUP ? 'D' : ' '
						);
					} else {
						warning(TRUE, result);
					}
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read atc vitc 2 register
				if ((result = pclGetRegSync(
					hdl, PCL_REG_ATC_VITC2, &time, &bits, &user, &frame, NULL)) == PCL_OK
				) {
					gotoxy(21, 10);
					printfw(	// show time and user information
						"%2x:%02x:%02x:%02x  %08lx  V%d",
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time)),
						user,
						((bits & 
							(frame == PCL_FRAME_25 ?
								PCL_BITS_VITC_PF : PCL_BITS_VITC_NF)
						) ? 2 : 1)
					);
					if ((result = pclGetRegStatus(hdl, PCL_REG_ATC_VITC2, &stat)) == PCL_OK) {
					printfw(	// show status information
							"  %c %c  Line %2d %c",
							stat & PCL_STAT_DBB2_TC_VALIDITY ? 'V' : ' ',
							stat & PCL_STAT_DBB2_PROCESS_BIT ? 'P' : ' ',
							(stat & PCL_STAT_DBB2_VITC_LINE_SELECT) >> 8,
							stat & PCL_STAT_DBB2_VITC_LINE_DUP ? 'D' : ' '
						);
					} else {
						warning(TRUE, result);
					}
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read hanc ltc register
				if ((result = pclGetRegSync(
					hdl, PCL_REG_HANC_LTC, &time, &bits, &user, &frame, NULL)) == PCL_OK
				) {
					gotoxy(21, 12);
					printfw(	// show time and user information
						"%2x:%02x:%02x:%02x  %08lx",
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time)),
						user
					);
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			{ // read hanc vitc register
				if ((result = pclGetRegSync(
					hdl, PCL_REG_HANC_VITC, &time, &bits, &user, &frame, NULL)) == PCL_OK
				) {
					gotoxy(21, 13);
					printfw(	// show time and user information
						"%2x:%02x:%02x:%02x  %08lx  V%d",
						HIBYTE(HIWORD(time)),
						LOBYTE(HIWORD(time)),
						HIBYTE(LOWORD(time)),
						LOBYTE(LOWORD(time)),
						user,
						((bits & 
							(frame == PCL_FRAME_25 ?
								PCL_BITS_VITC_PF : PCL_BITS_VITC_NF)
						) ? 2 : 1)
					);
					if ((result = pclGetRegStatus(hdl, PCL_REG_HANC_VITC, &stat)) == PCL_OK) {
						printfw(	// show status information
							"  %c    Line %2d %c",
							stat & PCL_STAT_HANC_VALIDITY ? 'V' : ' ',
							(stat & PCL_STAT_HANC_VITC_LINE_SELECT) / PCL_STAT_HANC_VITC_LINE_SELECT_DIV + 
								(frame == PCL_FRAME_25 ?
									6 : 10),
							stat & PCL_STAT_HANC_VITC_LINE_DUP ? 'D' : ' '
						);
					} else {
						warning(TRUE, result);
					}
				} else {
					if (result < PCL_OK) {
						warning(TRUE, result);
					}
				}
			}
			Sleep(10);
		}

#ifndef LINUX
	 	ch = getch();
#endif
		switch (ch) {
			// there is a keystroke waiting
		case 'x': case 'X': case '\x1b':
			running = FALSE;
			break;
		}
	}
	// it's important to close the device if we don't need it anymore
	pclClose(hdl);
	gotoxy(1, 24);
	cursorVisible(TRUE);	// show cursor
#ifdef LINUX
	endwin();
#endif
	return(0);
}
