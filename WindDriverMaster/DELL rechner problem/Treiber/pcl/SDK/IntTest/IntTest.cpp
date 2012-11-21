//***************************************************************************
//*																			*
//*	Interrupt Test Program for Windows DLL AVPCL.DLL						*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*																			*
//*	This program shows reading timecode from PCL PCI using interrupts.		*
//*																			*
//***************************************************************************

// <inttest.cpp> Alpermann+Velte DLL for AV-PCL
// @(#)inttest.cpp 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte
#define VER_S     "3.36"

//---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

#include "avpcl.h"

//---------------------------------------------------------------------------

#ifdef LINUX
#define printfw printw
#else
#define printfw printf
#endif

//---------------------------------------------------------------------------

#ifdef _WIN32

void gotoxy(int x, int y) // set cursor to new position of the screen
{
	HANDLE hStdout;
	COORD coordCursor; // new cursor position coordinates

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	coordCursor.X = (short) (x - 1);
	coordCursor.Y = (short) (y - 1);

	SetConsoleCursorPosition(
		hStdout, // handle of console screen buffer
		coordCursor // new cursor position coordinates
	);
}

#endif

//---------------------------------------------------------------------------

#ifdef _WIN32

void clrscr(void) // clear screen
{
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
}

#endif

//---------------------------------------------------------------------------

#ifdef _WIN32

void clreol(void) // clear from cursor position to end of line
{
	DWORD dummy;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;	// Console information

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo( hStdout, &csbiInfo );

	FillConsoleOutputCharacter(
		hStdout, ' ', csbiInfo.dwSize.X - csbiInfo.dwCursorPosition.X,
		csbiInfo.dwCursorPosition, &dummy
	);
}

#endif

//---------------------------------------------------------------------------

#ifdef _WIN32

void cursorVisible(BOOL show) // hide or show cursor
{
	HANDLE hStdout;
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hStdout, &ConsoleCursorInfo);
	ConsoleCursorInfo.bVisible = show;
	SetConsoleCursorInfo(hStdout, &ConsoleCursorInfo);
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
	if (fullscreen) gotoxy(1, 20);
	printf(
		"Fatal Error while %s: %s (%d)",
		reason, err2msg(result), result
	);
	printf(
		"\n%s\n"
		"Press any key to continue...",
		pclGetErrorString()
	);
	getch();
	exit(1);
}

//---------------------------------------------------------------------------

void warning(			// show warning
	BOOL fullscreen,	// fullscreen mode
	int result			// return code
) {
	char *s;

	if (fullscreen) gotoxy(1, 20);
	printf(err2msg(result));
	clreol();
	s = strdup(pclGetErrorString());
	if (s[0] != '\0') s[strlen(s) - 1] = '\0';
	printf("\n%s", s);
	clreol();
	free(s);
}

//---------------------------------------------------------------------------

void __cdecl intHandler(			// interrupt handler
	int hdl,						// handle from pclOpen()
	int flag,						// interupt flag to handle
	const PCL_INT_RESULT *intResult	// interrupt result
) {
	int result;						// result from pclXxx() functions
	DWORD time, user;				// time code information
	int newd, bits, frame, stat;	// time code information

	if (intResult->stopped) {	// interrupt was disabled while servicing
		return;					// don't handle interrupt
	}

	switch (flag) {	// what interrupt source to handle

	case PCL_IMASK_MIXED:	// handle mixed register interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_MIXED, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from mixed register
			gotoxy(20, 3);
			printf( // show time and user information
				"%2x:%02x:%02x:%02x  %08lx  %s",
				HIBYTE(HIWORD(time)),
				LOBYTE(HIWORD(time)),
				HIBYTE(LOWORD(time)),
				LOBYTE(LOWORD(time)),
				user,
				newd & PCL_NEWD_VITC ? "VITC" :
				newd & PCL_NEWD_LTC  ? "LTC " :
				newd & PCL_NEWD_CTL  ? "CTL " :
				                       "?   "
			);
		} else {
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_VITC:	// handle VITC interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_VITC, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from VITC register
			gotoxy(20, 4);
			printf( // show time and user information
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
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_LTC:	// handle LTC interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_LTC, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from LTC register
			gotoxy(20, 5);
			printf( // show time and user information
				"%2x:%02x:%02x:%02x  %08lx",
				HIBYTE(HIWORD(time)),
				LOBYTE(HIWORD(time)),
				HIBYTE(LOWORD(time)),
				LOBYTE(LOWORD(time)),
				user
			);
		} else {
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_ATC_LTC:		// handle ATC LTC interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_ATC_LTC, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from ATC LTC register
			gotoxy(20, 8);
			printf( // show time and user information
				"%2x:%02x:%02x:%02x  %08lx",
				HIBYTE(HIWORD(time)),
				LOBYTE(HIWORD(time)),
				HIBYTE(LOWORD(time)),
				LOBYTE(LOWORD(time)),
				user
			);
			if ((result = pclGetRegStatus(hdl, PCL_REG_ATC_LTC, &stat)) == PCL_OK) {
				printfw(
					// show status information
					"  %c %c",
					stat & PCL_STAT_DBB2_TC_VALIDITY ? 'V' : ' ',
					stat & PCL_STAT_DBB2_PROCESS_BIT ? 'P' : ' '
				);
			} else {
				warning(TRUE, result);
			}
		} else {
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_ATC_VITC1:	// handle ATC VITC 1 interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_ATC_VITC1, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from ATC VITC 1 register
			gotoxy(20, 9);
			printf( // show time and user information
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
				printfw(
					// show status information
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
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_ATC_VITC2:	// handle ATC VITC 2 interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_ATC_VITC2, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from ATC VITC 2 register
			gotoxy(20, 10);
			printf( // show time and user information
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
				printfw(
					// show status information
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
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_HANC_LTC:	// handle HANC LTC interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_HANC_LTC, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from HANC LTC register
			gotoxy(20, 12);
			printf( // show time and user information
				"%2x:%02x:%02x:%02x  %08lx",
				HIBYTE(HIWORD(time)),
				LOBYTE(HIWORD(time)),
				HIBYTE(LOWORD(time)),
				LOBYTE(LOWORD(time)),
				user
			);
		} else {
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_HANC_VITC:	// handle HANC VITC interrupt
		if ((result = pclGetRegister(hdl, PCL_REG_HANC_VITC, &time, &bits,
			&user, &frame, &newd)) == PCL_OK
		) {	// get time code from HANC VITC register
			gotoxy(20, 13);
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
			warning(TRUE, result);	// error while pclGetRegister()
		}
		break;

	case PCL_IMASK_V1:	// handle V1 interrupt (odd video field)
		gotoxy(20, 6);
		printf("V1  (odd)");	// show video field
		break;

	case PCL_IMASK_V2:	// handle V2 interrupt (even video field)
		gotoxy(20, 6);
		printf("V2 (even)");	// show video field
		break;

	default:	// unknown interrupt source
		break;	// don't handle interrupt
	}
}

//---------------------------------------------------------------------------

int main( // main function
	int argn,
	char **argc
) {
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
			printf(
				"Error in parameter base_address: %s\n"
				"Please try:  INTTEST base_address\n\n"
				"Press any key to continue...",
				argc[1]
			);
			getch();
			exit(1);
		}
	}
	if ((result = pclOpen(&hdl, base)) != PCL_OK) {
									// open pcl card and get handle
		printf(
			"Unable to find PCL card at address %d (0x%X)\n"
			"Please try:  INTTEST base_address\n\n",
			base, base
		);
		fatal(FALSE, "pclOpen", result);
	}

//	the following forces a power-on reset of the PCL board, if needed
	if ((result = pclReset(hdl, PCL_IMODE_POR)) < PCL_OK) {
		warning(FALSE, result);
	}

	{ // get some version informations from pcl
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
			printf(
				"Warning: Version of DLL is V%d.%02d, "
				"but has to be V%d.%02d or higher.\n",
				dll >> 8, dll & 0xff,
				PCL_VER_DLL >> 8, PCL_VER_DLL & 0xff
			);
		}
	}

	cursorVisible(FALSE);	// hide cursor
	clrscr(); // first clear screen
	gotoxy(1,1); // build screen mask
	printf(
		"Alpermann+Velte PCL Cards - IntTest Version " VER_S "\n"
		"\n"
		"Mixed      (int) : --:--:--:--  --------\n"
		"VITC       (int) : --:--:--:--  --------\n"
		"LTC        (int) : --:--:--:--  --------\n"
		"Video      (int) : --\n"
		"\n"
		"ATC LTC    (int) : --:--:--:--  --------\n"
		"ATC VITC 1 (int) : --:--:--:--  --------\n"
		"ATC VITC 2 (int) : --:--:--:--  --------\n"
		"\n"
		"HANC LTC   (int) : --:--:--:--  --------\n"
		"HANC VITC  (int) : --:--:--:--  --------\n"
		"\n"
		"Version          : --\n"
		"\n"
		"D)isable Interrupts    E)nable Interrupts\n"
		"eX)it (esc)"
	);

	gotoxy(20, 15);
	printf(
		"%s, Firmware: V%d, %s (%c%c/%c), DLL: V%d.%02d",
		(firmware & 0xc0) == 0x00 ? "PCL6 or PCL3" :
		(firmware & 0xc0) == 0x80 ? "PCL5 or PCL10" :
		(firmware & 0xc0) == 0xc0 ? "PCL12" :
		(firmware & 0xc0) == 0x40 ? "PCL PCI" : "unknown",
			// pcl identifier is coded in bit 6 and 7 of
			//	firmware version
		firmware & 0x3F,
			// pcl firmware version
		ident ? "special" : "no special",
		byteb, bytea, bytec,	// special oem version of pcl?
		dll >> 8, dll & 0xff	// version of device driver
	);

	// set the vitc reader mode.  we want to read from line 6 to 22
	if ((result = pclVitcSetup(hdl, 6, 22, PCL_VLINE_BLOCK)) < PCL_OK) {
		warning(TRUE, result);
	}

#	define PCL_IMASK ( \
		PCL_IMASK_V1 | \
		PCL_IMASK_V2 | \
		PCL_IMASK_MIXED | \
		PCL_IMASK_VITC | \
		PCL_IMASK_LTC | \
		PCL_IMASK_ATC_LTC | \
		PCL_IMASK_ATC_VITC1 | \
		PCL_IMASK_ATC_VITC2 | \
		PCL_IMASK_HANC_LTC | \
		PCL_IMASK_HANC_VITC \
	)										// PCL interrupts to enable

	if ((result = pclIntMask(hdl, 0)) < PCL_OK) {
		warning(TRUE, result);
	}
	if ((result = pclSetIntHandler(hdl, intHandler)) < PCL_OK) {
		warning(TRUE, result);
	}
	if ((result = pclIntMask(hdl, PCL_IMASK)) < PCL_OK) {
		warning(TRUE, result);
	}
/*
	// the following might be enabled for time critical interrupt service
	if ((result = pclSetIntPriority(hdl, THREAD_PRIORITY_TIME_CRITICAL)) < PCL_OK) {
		warning(TRUE, result);
	}
*/
	HANDLE hThreadInt;
	DWORD dw;
	DWORD error;

	if ((result = pclGetThreadHandle(hdl, PCL_THREAD_INT, &hThreadInt)) != PCL_OK) {
		fatal(FALSE, "pclGetThreadHandle INT", result);
	}
	if ((dw = SetThreadAffinityMask(hThreadInt, 1)) == 0) {
		error = GetLastError();
	}

	HANDLE hThreadEvent;
	if ((result = pclGetThreadHandle(hdl, PCL_THREAD_EVENT, &hThreadEvent)) != PCL_OK) {
		fatal(FALSE, "pclGetThreadHandle EVENT", result);
	}
	if ((dw = SetThreadAffinityMask(hThreadEvent, 1)) == 0) {
		error = GetLastError();
	}

	while (running) { // main program loop
		int ch;

		switch (ch = getch()) {	// wait until there is
								//	a keystroke waiting
		case 'd': case 'D':	// disable all interrupts
			if ((result = pclIntMask(hdl, 0)) < PCL_OK) {
				warning(TRUE, result);
			}
			break;
		case 'e': case 'E':	// enable all interrupts
			if ((result = pclIntMask(hdl, PCL_IMASK)) < PCL_OK) {
				warning(TRUE, result);
			}
			break;
		case 'x': case 'X': case '\x1b':
			running = FALSE;
			break;
		}
	}
	// it's important to close the device if we don't need it anymore
	pclClose(hdl);
	gotoxy(1, 24);
	cursorVisible(TRUE);	// show cursor
	return(0);
}
