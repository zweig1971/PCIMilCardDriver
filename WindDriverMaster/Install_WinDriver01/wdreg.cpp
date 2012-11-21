#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <commdlg.h> 
#include <commctrl.h> 

#include "drvload.h"

struct _stat FileStatus;

OPENFILENAME OpenFileName;
OFSTRUCT OfStruct;

using namespace std;

bool FileCopy(string source, string destination)
{
	FILE *fs, *fd;
	int i = 0;
	int c = 0;
	char charSource[64];
	char charDestination[64];

	// source kopieren
	while(source[i] != '\0')
	{
		charSource[i] = source[i];
		i++;
	}
	charSource[i] = '\0';

	// destination kopieren
	i=0;
	while(destination[i] != '\0')
	{
		charDestination[i] = destination[i];
		i++;
	}
	charDestination[i] = '\0';

	// file oeffnen
	fs = fopen(charSource, "rb"); 
	fd = fopen(charDestination, "wb");
	if((fs == NULL) || (fd == NULL))
	{
		return false;
	}

	c = getc(fs);

	while (c != EOF )
	{
		putc(c, fd); 
		c = getc(fs);
	}

	fclose(fs); 
	fclose(fd);

	return true;
}

bool DeleteFile(string file)
{
	int i = 0;
	int hFile;

	char FileName[64];

	while(file[i] != '\0')
	{
		FileName[i] = file[i];
		i++;
	}
	
	FileName[i] = '\0';

	hFile = OpenFile(FileName,&OfStruct,OF_DELETE);

	if (hFile == HFILE_ERROR) 
	{
		return false;
	}

	return true;
}

bool InstallationsRoutine(bool install, string &ErrorMessage)
{
    char *sDriverName = NULL;
    char *sDriverFile = NULL;
	bool status		  = true;
    BOOL fQuiet       = FALSE;
    BOOL fErrComLine  = FALSE;
    BOOL fVxd         = FALSE;
    DWORD startup     = 2;
    LoadDriver *loadDriver = NULL;

	string SystemDriverDir;
	string Dlldestination;
	string WinDrvrdestination;

	char SystemDir [256];

	// Dateien zu kopieren;
	string Dllsource     = "A:\\PCIMilTreiber.dll";
	string WinDrvrsource = "A:\\windrvr.sys";

	// Treibername festlegen
    sDriverName = "WinDriver";
    sDriverFile = "WINDRVR";

	// System Verzeichnis rausfinden
	GetSystemDirectory(SystemDir, 255);

	// destination dir fuer die windrvr.sys
	SystemDriverDir = SystemDir;
	WinDrvrdestination = SystemDriverDir + "\\drivers\\windrvr.sys";

	// destination dir fuer die PCIMilDriver.dll
	Dlldestination = SystemDriverDir + "\\PCIMilTreiber.dll";

	if (install == true)
	{

		// windrvr.sys kopieren
		status = FileCopy(WinDrvrsource, WinDrvrdestination);
		if(status != true)
		{
			ErrorMessage = "copy windrvr.sys failure !";
			return false;
		}

		// PCIMilDriver.dll kopieren
		status = FileCopy(Dllsource, Dlldestination);
		if(status != true)
		{
			ErrorMessage = "copy PCIMilDriver.dll failure !";
			return false;
		}

		loadDriver = NewLoadDriver (fVxd);

		if (!loadDriver->Init(sDriverName, sDriverFile, startup))
		{
			ErrorMessage = "Init failed ! ";
			return false;
		}

		// Vielleicht vorhandenden alten Treiber entfernen
		loadDriver->Stop();
		loadDriver->Delete();
 
		// neuen Treiber installieren
		if (!loadDriver->Create())
		{
			ErrorMessage = "Create driver entry failed !";
			return false;
		}
   
		if (!loadDriver->Start())
		{
			ErrorMessage = "Starting driver entry failed !";
			return false;
		}
	}
	else
	{
		// windrvr.sys und PCIMilTreiber.dll loeschen
		status = DeleteFile(WinDrvrdestination);
		if(status != true)
		{
			ErrorMessage = "delete windrvr.sys failure !";
		}

		status = DeleteFile(Dlldestination);
		if(status != true)
		{
			ErrorMessage = "delete windrvr.sys failure !";
		}

		loadDriver = NewLoadDriver (fVxd);

		if (!loadDriver->Init(sDriverName, sDriverFile, startup))
		{
			ErrorMessage = "Init failed ! ";
			return false;
		}

		loadDriver->Stop();
		loadDriver->Delete();
	
		return status;
	}
	return true;
}

