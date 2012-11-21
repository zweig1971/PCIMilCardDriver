//***************************************************************************
//*																			*
//*	Windows DLL AVPCL.DLL													*
//*	Setup Program															*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*	Portions (c) 1999-2002 Jungo, see http://www.jungo.com					*
//*																			*
//***************************************************************************

// <Setup.cpp> Alpermann+Velte DLL Setup Program for AV-PCL
// @(#)Setup.cpp 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte

//---------------------------------------------------------------------------

#define WIN32_LAN_AND_MEAN
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include "drvload.h"

//---------------------------------------------------------------------------

#define DLL_NAME "AvPcl32.dll"

//---------------------------------------------------------------------------

int main (int argc, char *argv[]);

//---------------------------------------------------------------------------

#ifndef _CONSOLE

char g_sAppName[256];
char g_sBuf[4096];

//---------------------------------------------------------------------------

void OutputMessage(BOOL success)
{
	MessageBox(
		NULL,
		g_sBuf,
		g_sAppName,
		MB_OK | (success ? MB_ICONINFORMATION : MB_ICONSTOP) | MB_SETFOREGROUND
	);
}

//---------------------------------------------------------------------------

int printf( char *szFormat, ... )
{
	char sBuf[256];
	DWORD ret;
	va_list marker;
	va_start (marker, szFormat);
	ret = vsprintf(sBuf, szFormat, marker);
	strcat(g_sBuf, sBuf);
	va_end(marker);
	return ret;
}

//---------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int iCmdShow)
{
	char argv0[256];
	
	char *argv_all;
	char *argv[256];
	int ret_val;
	int argc = 0;
	
	BOOL fInArg = FALSE;
	BOOL fInQuote;
	int i, j;
	
	GetModuleFileName(hInstance, argv0, sizeof (argv0));
	strcpy(g_sAppName, argv0);
	argv[argc++] = argv0;
	
	argv_all = (char *) malloc(strlen(szCmdLine) + 1);
	
	j = 0;
	for (i = 0; (char) szCmdLine[i]; i++) {
		char ch = (char) szCmdLine[i];
		if (fInArg) {
			if (fInQuote && ch == '\"' || !fInQuote && (ch == ' ' || ch == '\t')) {
				fInArg = FALSE;
				argv_all[j++] = '\0';
			} else {
				argv_all[j++] = ch;
			}
		} else {
			if (ch!=' ' && ch!='\t') {
				fInArg = TRUE;
				fInQuote = ch == '\"';
				if (!fInQuote)
					i--;
				argv[argc] = argv_all + j;
				argc++;
			}
		}
	}

	if (fInArg)
		argv_all[j++] = '\0';

	argv[argc] = NULL;

	ret_val = main(argc, argv);

	free (argv_all);

	return ret_val;
}

//---------------------------------------------------------------------------

#else //_CONSOLE

void OutputMessage(BOOL success)
{
}

#endif //_CONSOLE

//---------------------------------------------------------------------------

void PrintUsage(char *exe)
{
	printf("%s [Options] Action\n", exe);
	printf("Options:\n");
#ifndef _CONSOLE
	printf("    -silent: supresses output message box.\n");
#endif //_CONSOLE
	printf("    -startup: Valid only for WinNT.\n");
	printf("              Set startup level: boot, system, automatic, manual, disabled\n");
	printf("              (default: automatic).\n");
	printf("    -name:  Set then name of the driver (default: WinDriver).\n");
	printf("    -file:  Set the file name of the driver.\n");
	printf("    -vxd:   load a .vxd on Win98 (default .sys)\n");
	printf("Action:\n");
	printf("    copy:    copy driver to system directory.\n");
	printf("    create:  create a registry entry for the driver.\n");
	printf("    start:   start the driver.\n");
	printf("    install: copy + create + start.\n");
	printf("    delete:  delete the registry entry for the driver.\n");
	printf("    stop:    stop the driver.\n");
	printf("    remove:  stop + delete.\n");
	printf("Examples:\n");
	printf("    To install default driver (i.e. WinDriver):\n");
	printf("        %s\n", exe);
	printf("    is the same as:\n", exe);
	printf("        %s install\n");
	printf("    is the same as:\n", exe);
	printf("        %s copy create start\n");
	printf("\n");
	printf("    To unload default driver (i.e. WinDriver), and remove it from registry:\n");
	printf("        %s stop delete\n", exe);
	printf("    is the same as:\n");
	printf("        %s remove\n", exe);
	OutputMessage(TRUE);
}

//---------------------------------------------------------------------------

int main (int argc, char *argv[])
{
	char *sDriverName = NULL;
	char *sDriverFile = NULL;
	BOOL fQuiet = FALSE;
	BOOL fErrComLine = FALSE;
	BOOL fShowHelp = FALSE;
	char sErrBuf[1024];
	DWORD startup = 2;
	LoadDriverNT *loadDriver = NULL;
	int i;
	int rc = 0;

	if (GetOsVer() != VER_NT4) {
		printf("ERROR.  This program is only useful with Windows NT 4.0\n");
		printf("\n");
		printf("With Windows 95/98/ME and Windows 2000 please\n");
		printf("use the \"Detect new hardware\" feature of the\n");
		printf("operating system in conjunction with the file\n");
		printf("PCL-PCI.INF shipped with this driver.\n");
		rc = 1;
		goto Exit;
	}

	if (argc == 1) {
		argc++;
		char *arg_v[3] = { argv[0], "install", NULL };
		argv = arg_v;
	}		
	for (i=1; i<argc; i++) {
		if (argv[i][0]=='-') {
			if (stricmp(argv[i], "-silent") == 0) {
				fQuiet = TRUE;
				continue;
			} else if (stricmp(argv[i], "-name") == 0) {
				i++;
				if (i == argc) {
					sprintf (sErrBuf, "option '-name' needs a following argument\n");
					fErrComLine = TRUE;
					break;
				}
				sDriverName = argv[i];
				continue;
			} else if (stricmp(argv[i], "-startup") == 0) {
				i++;
				if (i==argc) {
					sprintf (sErrBuf, "option '-startup' needs a following argument\n");
					fErrComLine = TRUE;
					break;
				}
				if (stricmp(argv[i], "boot") == 0)
					startup = SERVICE_BOOT_START;
				else if (stricmp(argv[i],"system") == 0)
					startup = SERVICE_SYSTEM_START;
				else if (stricmp(argv[i],"automatic") == 0)
					startup = SERVICE_AUTO_START;
				else if (stricmp(argv[i],"manual") == 0)
					startup = SERVICE_DEMAND_START;
				else if (stricmp(argv[i],"disabled") == 0)
					startup = SERVICE_DISABLED;
				else {
					sprintf(sErrBuf,"option '-startup' needs one of the following argument:\n");
					sprintf(sErrBuf, "  boot, system, automatic , manual, disabled\n");
					fErrComLine = TRUE;
					break;
				}
				continue;
			} else if (stricmp(argv[i], "-file") == 0) {
				i++;
				if (i==argc) {
					sprintf(sErrBuf, "option '-file' needs a following argument\n");
					fErrComLine = TRUE;
					break;
				}
				sDriverFile = argv[i];
				continue;
			} else if (stricmp(argv[i], "-help") == 0) {
				fShowHelp = TRUE;
				continue;
			} else if (stricmp(argv[i], "-?") == 0) {
				fShowHelp = TRUE;
				continue;
			} else {
				sprintf (sErrBuf, "unknown option %s\n", argv[i]);
				fErrComLine = TRUE;
				break;
			}
		} else if (argv[i][0]=='/') {
			if (stricmp(argv[i], "/help") == 0) {
				fShowHelp = TRUE;
			} else if (stricmp(argv[i], "/?") == 0) {
				fShowHelp = TRUE;
			}
		} else break;
	}
	if (fShowHelp) {
		PrintUsage(argv[0]);
		return EXIT_SUCCESS;
	}
	if (fErrComLine) {
		printf("%s", sErrBuf);
		if (!fQuiet)
			OutputMessage(FALSE);
		
		return EXIT_FAILURE;
	}
	if (!sDriverName && !sDriverFile) {
		sDriverName = "WinDriver";
		sDriverFile = "WINDRVR";
	} else if (!sDriverName) {
		sDriverName = sDriverFile;
	} else if (!sDriverFile) {
		sDriverFile = sDriverName;
	}

	loadDriver = new LoadDriverNT();
	if (!loadDriver) {
		printf("ERROR. please check the following:\n");
		printf("  Operating system: This program can run only on Win95/Win98/WinNT\n");
//		printf("  -vxd option can not be used in WinNT\n");
		rc = 1;
		goto Exit;
	}
	if (loadDriver->HasMsg())
		printf ("%s", loadDriver->GetLastMsg());
	if (loadDriver->HasErr()) {
		rc = 1;
		goto Exit;
	}
	if (!loadDriver->Init(sDriverName, sDriverFile, startup)) {
		printf("Init failed\n");
		rc = 1;
		goto Exit;
	}
	if (i == argc) {
		printf("nothing to do!\n");
		rc = 0;
		goto Exit;
	}
	for (; i<argc; i++)	{
		if (stricmp(argv[i], "copy") == 0) {
			printf("Copying driver files... ");
			if (!loadDriver->Copy(DLL_NAME)) {
				printf("Failed\n");
				printf("%s", loadDriver->GetLastMsg());
				rc = 1;
			} else printf("OK\n");
		} else if (stricmp(argv[i], "delete") == 0) {
			printf("Creating driver entry... ");
			if (!loadDriver->Create()) {
				printf("Failed\n");
				printf("%s", loadDriver->GetLastMsg());
				rc = 1;
			} else printf("OK\n");
		} else if (stricmp(argv[i], "delete") == 0) {
			printf("Deleting driver entry... ");
			if (!loadDriver->Delete()) {
				printf("Failed\n");
				printf("%s", loadDriver->GetLastMsg());
				rc = 1;
			} else printf("OK\n");
		} else if (stricmp(argv[i], "start") == 0) {
			printf("Starting driver... ");
			if (!loadDriver->Start()) {
				printf("Failed\n");
				printf("%s", loadDriver->GetLastMsg());
				rc = 1;
			} else printf("OK\n");
		} else if (stricmp(argv[i], "stop") == 0) {
			printf("Stopping driver... ");
			if (!loadDriver->Stop()) {
				printf("Failed\n");
				printf("%s", loadDriver->GetLastMsg());
				rc = 1;
			} else printf("OK\n");
		} else if (stricmp(argv[i],"install")==0) {
			if (loadDriver->Running()) {
				printf("Stopping driver... ");
				if (!loadDriver->Stop()) {
					printf("Failed\n");
					printf("%s", loadDriver->GetLastMsg());
					rc = 1;
				} else printf("OK\n");
			}
			printf("Copying driver files... ");
			if (!loadDriver->Copy(DLL_NAME)) {
				printf("Failed\n");
				printf("%s", loadDriver->GetLastMsg());
				rc = 1;
			} else printf("OK\n");
			if (!loadDriver->Created()) {
				printf("Creating driver entry... ");
				if (!loadDriver->Create()) {
					printf("Failed\n");
					printf("%s", loadDriver->GetLastMsg());
					rc = 1;
				} else printf("OK\n");
			}
			if (!loadDriver->Running()) {
				printf ("Starting driver... ");
				if (!loadDriver->Start()) {
					printf("Failed\n");
					printf("%s", loadDriver->GetLastMsg());
					rc = 1;
				} else printf("OK\n");
			}
		} else if (stricmp(argv[i],"remove")==0) {
			if (loadDriver->Running()) {
				printf ("Stopping driver... ");
				if (!loadDriver->Stop()) {
					printf("Failed\n");
					printf("%s", loadDriver->GetLastMsg());
					rc = 1;
				} else printf("OK\n");
			}
			if (loadDriver->Created()) {
				printf ("Deleting driver entry... ");
				if (!loadDriver->Delete()) {
					printf("Failed\n");
					printf("%s", loadDriver->GetLastMsg());
					rc = 1;
				} else printf("OK\n");
			}
			if (rc == 0) {
				printf("Driver successfully removed\n");
			}
		} else {
			printf("unknown option %s\n", argv[i]);
			rc = 1;
			goto Exit;
		}
	}

Exit:
	delete loadDriver;
	if (!fQuiet)
		OutputMessage(rc == EXIT_SUCCESS);
	return rc;
}
