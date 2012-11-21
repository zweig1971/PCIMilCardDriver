// windows_test.cpp : Definiert den Einsprungpunkt für die Anwendung.
//

#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include <string>

#define MAX_LOADSTRING 100

using namespace std;

// Globale Variablen:
HWND hWnd;
HWND DialogHwd;
HINSTANCE hInst;					// aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];								// Text der Titelzeile
TCHAR szWindowClass[MAX_LOADSTRING];								// Text der Titelzeile

// Vorausdeklarationen von Funktionen, die in diesem Code-Modul enthalten sind:
ATOM				MyRegisterClass( HINSTANCE hInstance );
BOOL				InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	Install( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	Installing( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	Uninstall( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	Successful( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	Failure( HWND, UINT, WPARAM, LPARAM );


bool InstallationsRoutine(bool install, string &ErrorMessage);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// ZU ERLEDIGEN: Fügen Sie hier den Code ein.
	MSG msg;
	HACCEL hAccelTable;

	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINDOWS_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Initialisierung der Anwendung durchführen:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WINDOWS_TEST);

	// Hauptnachrichtenschleife:
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
		if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  AUFGABE: Registriert die Fensterklasse.
//
//  KOMMENTARE:
//
//    Diese Funktion und ihre Verwendung sind nur notwendig, wenn dieser Code
//    mit Win32-Systemen vor der 'RegisterClassEx'-Funktion kompatibel sein soll,
//    die zu Windows 95 hinzugefügt wurde. Es ist wichtig diese Funktion aufzurufen,
//    damit der Anwendung kleine Symbole mit den richtigen Proportionen zugewiesen
//    werden.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WINDOWS_TEST);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_WINDOWS_TEST;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNKTION: InitInstance(HANDLE, int)
//
//   AUFGABE: Speichert die Instanzzugriffsnummer und erstellt das Hauptfenster
//
//   KOMMENTARE:
//
//        In dieser Funktion wird die Instanzzugriffsnummer in einer globalen Variable
//        gespeichert und das Hauptprogrammfenster erstellt und angezeigt.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{

  hInst = hInstance; // Instanzzugriffsnummer in unserer globalen Variable speichern

  hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU | WS_THICKFRAME | WS_ICONIC,
      0, 0, 0, 0, NULL, NULL, hInstance, NULL);

   if( !hWnd ) 
   {
      return FALSE;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );
 
   DialogBox(hInst, (LPCTSTR)IDD_StartMenue, hWnd, (DLGPROC)Install);

   return TRUE;
}

//
//  FUNKTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  AUFGABE:  Verarbeitet Nachrichten für das Hauptfenster.
//
//  WM_COMMAND	- Anwendungsmenü verarbeiten
//  WM_PAINT	- Hauptfenster darstellen
//  WM_DESTROY	- Beendigungsnachricht ausgeben und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch( message ) 
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}

// Nachrichtenbehandlungsroutine für "Info"-Feld.
LRESULT CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

// Nachrichtenbehandlungsroutine für "Verdammt"-Feld.
LRESULT CALLBACK Install( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	int result;

	switch( message )
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_Abbruch) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				PostQuitMessage( 0 );
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_Install) 
			{
				DialogBox(hInst, (LPCTSTR)IDD_Installing, hWnd, (DLGPROC)Installing);
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_Uninstall) 
			{
				DialogBox(hInst, (LPCTSTR)IDD_Uninstall, hWnd, (DLGPROC)Uninstall);
				return TRUE;
			}
			
			break;
	}
    return FALSE;
}

LRESULT CALLBACK Installing( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	bool status;
	string ErrorMessage;
	
	switch( message )
	{
		case WM_INITDIALOG:
			status = InstallationsRoutine(true, ErrorMessage);

			if(status != true)
			{
				DialogBox(hInst, (LPCTSTR)IDD_FailureBox, hWnd, (DLGPROC)Failure);
			}
			else
			{
				DialogBox(hInst, (LPCTSTR)IDD_SuccessfulBox, hWnd, (DLGPROC)Successful);
			}
			EndDialog(hDlg, LOWORD(wParam));		
		
			return TRUE;
	}
    return FALSE;
}

LRESULT CALLBACK Uninstall( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	bool status;
	string ErrorMessage;

	switch( message )
	{
		case WM_INITDIALOG:
			status = InstallationsRoutine(false, ErrorMessage);

			if(status != true)
			{
				DialogBox(hInst, (LPCTSTR)IDD_FailureBox, hWnd, (DLGPROC)Failure);
			}
			else
			{
				DialogBox(hInst, (LPCTSTR)IDD_SuccessfulBox, hWnd, (DLGPROC)Successful);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
	}
    return FALSE;
}


LRESULT CALLBACK Failure( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

LRESULT CALLBACK Successful( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}