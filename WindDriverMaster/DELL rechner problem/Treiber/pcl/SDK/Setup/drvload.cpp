//***************************************************************************
//*																			*
//*	Windows DLL AVPCL.DLL													*
//*	Setup Program															*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*	Portions (c) 1999-2002 Jungo, see http://www.jungo.com					*
//*																			*
//***************************************************************************

// <DrvLoad.cpp> Alpermann+Velte DLL Setup Program for AV-PCL
// @(#)DrvLoad.cpp 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte

//---------------------------------------------------------------------------

#define WIN32_LAN_AND_MEAN
#include <windows.h>
#if defined USE_SETUPAPI
#include <setupapi.h>
#endif
#include <stdio.h>
#include "drvload.h"

//---------------------------------------------------------------------------

// VXDLDR error codes
#define VXDLDR_ERR_OUT_OF_MEMORY		1
#define VXDLDR_ERR_IN_DOS				2
#define VXDLDR_ERR_FILE_OPEN_ERROR		3
#define VXDLDR_ERR_FILE_READ			4
#define VXDLDR_ERR_DUPLICATE_DEVICE		5
#define VXDLDR_ERR_BAD_DEVICE_FILE		6
#define VXDLDR_ERR_DEVICE_REFUSED		7
#define VXDLDR_ERR_NO_SUCH_DEVICE		8
#define VXDLDR_ERR_DEVICE_UNLOADABLE	9
#define VXDLDR_ERR_ALLOC_V86_AREA		10
#define VXDLDR_ERR_BAD_API_FUNCTION		11

#define VXDLDR_ERR_MAX					11

// VXDLDR driver functions
#define VXDLDR_APIFUNC_GETVERSION		0
#define VXDLDR_APIFUNC_LOADDEVICE		1
#define VXDLDR_APIFUNC_UNLOADDEVICE		2

//---------------------------------------------------------------------------

LoadDriver::LoadDriver()
{
	m_h = NULL;
	ClearErr();
}

//---------------------------------------------------------------------------

LoadDriver::~LoadDriver()
{
}

//---------------------------------------------------------------------------

BOOL LoadDriver::Init(char *sDriverName, char *sDriverFile, DWORD startLevel, char *sDriverPath, BOOL fUseReg)
{
	if (!m_h)
		return FALSE;
	m_fUseReg = fUseReg;
	m_iStartLevel = startLevel;
	SetPaths (sDriverName, sDriverFile, sDriverPath);
	return TRUE;
}

//---------------------------------------------------------------------------

char *LoadDriver::GetLastMsg()
{ 
	return m_sLastMsg;
}

//---------------------------------------------------------------------------

void LoadDriver::ClearErr()
{
	m_sLastMsg[0] = '\0';
	m_fHasErr = FALSE;
	m_fHasWrn = FALSE;
}

//---------------------------------------------------------------------------

void LoadDriver::PrintErr(const char *msg, ...)
{
	va_list marker;
	char s[sizeof(m_sLastMsg)];

	va_start(marker, msg);
	vsprintf(s, msg, marker);
	va_end(marker);

	strcat(m_sLastMsg, s);
	m_fHasErr = TRUE;
}

//---------------------------------------------------------------------------

void LoadDriver::PrintLastError()
{
	DWORD dwError;
	LPTSTR lpMsgBuf;

	dwError = GetLastError();

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);
	if (lpMsgBuf) {
		// Display the string.
		PrintErr(lpMsgBuf);
		// Free the buffer.
		LocalFree(lpMsgBuf);
	}
#if defined USE_SETUPAPI
	else switch (dwError) {
	case ERROR_EXPECTED_SECTION_NAME:	PrintErr("A section name was expected, and not found.\n"); return;
	case ERROR_BAD_SECTION_NAME_LINE:	PrintErr("The section name was not of the correct format.\n"); return;
	case ERROR_SECTION_NAME_TOO_LONG:	PrintErr("The section name exceeded the maximum length of MAX_SECT_NAME_LEN.\n"); return;
	case ERROR_GENERAL_SYNTAX:			PrintErr("The general syntax is incorrect.\n"); return;
	case ERROR_WRONG_INF_STYLE:			PrintErr("The INF is not of the type specified in the function call.\n"); return;
	case ERROR_SECTION_NOT_FOUND:		PrintErr("The section was not found in the INF file.\n"); return;
	case ERROR_LINE_NOT_FOUND:			PrintErr("The line was not found in the INF section.\n"); return;
	}
#endif
}

//---------------------------------------------------------------------------

void LoadDriver::PrintWrn(const char *msg)
{
	strcat(m_sLastMsg, msg);
	m_fHasWrn = TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver::HasErr()
{
	return m_fHasErr;
}

//---------------------------------------------------------------------------

BOOL LoadDriver::HasWrn()
{
	return m_fHasWrn;
}

//---------------------------------------------------------------------------

BOOL LoadDriver::HasMsg()
{
	return m_fHasWrn || m_fHasErr;
}

//---------------------------------------------------------------------------

void LoadDriver::SetPaths(char *sDriverName, char *sDriverFile, char *sDriverPath)
{
	strcpy(m_sDriverName, sDriverName);
	strcpy(m_sDriverFile, sDriverFile);
	if (sDriverPath)
		strcpy(m_sDriverPath, sDriverPath);
	else strcpy(m_sDriverPath, "");
}

//---------------------------------------------------------------------------

LoadDriver95::LoadDriver95()
{
	if (!OpenSCMHandle())
		PrintErr("Cannot open service control manager\n");
}

//---------------------------------------------------------------------------

LoadDriver95::~LoadDriver95()
{
	if (m_h)
		CloseSCMHandle();
}

//---------------------------------------------------------------------------

BOOL LoadDriver95::OpenSCMHandle()
{
	if (m_h)
		return FALSE;
	m_h = CreateFile(
		"\\\\.\\VXDLDR",			// device driver name
		0,							// access
		0,							// share mode
		0,							// security attributes
		CREATE_NEW, 				// create options
		FILE_FLAG_DELETE_ON_CLOSE,	// flags
		0		 					// template
	);
	
	if (m_h==INVALID_HANDLE_VALUE)
		m_h = NULL;
	return m_h!=NULL;
}

//---------------------------------------------------------------------------

void LoadDriver95::CloseSCMHandle()
{
	CloseHandle(m_h);
	m_h = NULL;
}

//---------------------------------------------------------------------------

void LoadDriver95::SetPaths(char *sDriverName, char *sDriverFile, char *sDriverPath)
{
	LoadDriver::SetPaths(sDriverName, sDriverFile, sDriverPath);
	strcpy(m_sDriverExt, ".VXD");
	if(!sDriverPath) {
		char sSystemPath[1024];

		GetSystemDirectory(sSystemPath,sizeof( sSystemPath));
		if (sSystemPath[strlen(sSystemPath)-1] != '\\') 
			strcat(sSystemPath, "\\");
		sprintf(m_sDriverPath, "%sVmm32\\%s%s", sSystemPath, m_sDriverFile, m_sDriverExt);
	}
}

//---------------------------------------------------------------------------

BOOL LoadDriver95::Create()
{
	ClearErr();

	char sKey[1024];
	sprintf(sKey, "System\\CurrentControlSet\\Services\\VxD\\%s", m_sDriverName);

	// updating registry
	HKEY hKey;
	ULONG rc;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, "", 0, KEY_ALL_ACCESS, NULL, &hKey, &rc);
	if (rc == REG_OPENED_EXISTING_KEY) {
		PrintWrn(m_sDriverName);
		PrintWrn(" already exists in Win-95 registry\n");
	} else if (rc != REG_CREATED_NEW_KEY && rc != ERROR_SUCCESS) {
		PrintErr("Cannot open registry key:\n");
		PrintLastError();
		return FALSE;
	}
	
	RegSetValueEx(hKey, "Start", 0, REG_BINARY, (PBYTE) "\0", 1);

	char sVal[1024];
	sprintf (sVal, "*%s", m_sDriverFile);
	RegSetValueEx(hKey, "StaticVxD", 0, REG_SZ, (PBYTE) sVal, 9);
	
	RegCloseKey(hKey);
	
	return TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver95::Delete()
{
	ClearErr();
	LONG rc;

	char sKey[1024];
	sprintf(sKey, "System\\CurrentControlSet\\Services\\VxD\\%s", m_sDriverName);
	rc = RegDeleteKey(HKEY_LOCAL_MACHINE, sKey);
	if (rc != ERROR_SUCCESS) {
		PrintErr(m_sDriverName);
		PrintErr(" does not exist in Win-95 registry:\n");
		PrintLastError();
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver95::Start()
{
	ULONG status, nRet;
	
	if (!m_h)
		return FALSE;
	
	status = DeviceIoControl(
		m_h,						// handle
		VXDLDR_APIFUNC_LOADDEVICE,	// control code (see vxdldr.h in DDK)
		(void *) m_sDriverPath, 	// path of vxd file (No \\.\ prefix)
		strlen(m_sDriverPath) + 1,	// input size
		NULL,						// output buffer
		0,							// output buffer size
		&nRet,						// receives count returned
		NULL
	);
	return status == 1; 
}

//---------------------------------------------------------------------------

BOOL LoadDriver95::Stop()
{
	typedef struct {
		LONG	vxdid;		 // device ID of VxD to unload, or -1 to use name
		CHAR	vxdname[256];// null terminated, case sensitive name of VxD 
		//   to unload if VxD ID is -1
	} UNLOAD_INFO;
	int i;
	
	UNLOAD_INFO unloadInfo;
	ULONG status, nRet;
	
	if (!m_h)
		return FALSE;
	
	unloadInfo.vxdid = -1;
	strcpy(unloadInfo.vxdname, m_sDriverFile);
	for (i = 0; unloadInfo.vxdname[i]; i++)
		unloadInfo.vxdname[i] = toupper(unloadInfo.vxdname[i]);
	
	for (i = 0; i < 1000; i++) {
		status = DeviceIoControl(
			m_h,							// handle
			VXDLDR_APIFUNC_UNLOADDEVICE,	// control code (see vxdldr.h in DDK)
			(void *) &unloadInfo,			// path of vxd file (No \\.\ prefix)
			sizeof (unloadInfo),			// input size
			NULL,							// output buffer
			0,								// output buffer size
			&nRet,							// receives count returned
			NULL
		);
		if (status != 1)
			break;
	}
	if (i == 0) {
		PrintWrn("Could not unload driver, or driver was previously loaded.\n");
	}
	return i > 0 && i < 1000; 
}

//---------------------------------------------------------------------------

BOOL LoadDriver95::Copy(const char *sDllSrc)
{
	char sSystemPath[1024];
	char sDriverSrc[1024];
	char sDriverDest[1024];
	char sDllDest[1024];

	ClearErr();

	GetSystemDirectory(sSystemPath, sizeof(sSystemPath));
	if (sSystemPath[strlen(sSystemPath) - 1] != '\\') 
		strcat(sSystemPath, "\\");

	sprintf(sDriverSrc, "%s%s", m_sDriverFile, m_sDriverExt);
	sprintf(sDriverDest, "%s%s%s%s", sSystemPath, "Vmm32\\", m_sDriverFile, m_sDriverExt);
	if (!CopyFile(sDriverSrc, sDriverDest, FALSE)) {
		PrintErr("Couldn't copy file %s to %s:\n", sDriverSrc, sDriverDest);
		PrintLastError();
		return(FALSE);
	}

	if (sDllSrc != NULL && sDllSrc[0] != '\0') {
		sprintf(sDllDest, "%s%s", sSystemPath, sDllSrc);
		if (!CopyFile(sDllSrc, sDllDest, FALSE)) {
			PrintErr("Couldn't copy file %s to %s:\n", sDllSrc, sDllDest);
			PrintLastError();
			return(FALSE);
		}
	}

	return(TRUE);
}

//***************************************************************************

LoadDriver98::LoadDriver98()
{
	if (!OpenSCMHandle())
		PrintErr("Cannot open service control manager\n");
}

//---------------------------------------------------------------------------

LoadDriver98::~LoadDriver98()
{
//	if (m_h)
//		CloseSCMHandle();
}

//---------------------------------------------------------------------------

BOOL LoadDriver98::OpenSCMHandle()
{
	// currently starting and stoping not supported OpenSCMHandle is empty
	m_h = (HANDLE) 1; // a 'dummy' handle
	return TRUE;
}

//---------------------------------------------------------------------------

void LoadDriver98::CloseSCMHandle()
{
}

//---------------------------------------------------------------------------

void LoadDriver98::SetPaths(char *sDriverName, char *sDriverFile, char *sDriverPath)
{
	LoadDriver::SetPaths(sDriverName, sDriverFile, sDriverPath);
	strcpy(m_sDriverExt, ".SYS");
	if (!sDriverPath)
		sprintf(m_sDriverPath, "\\SystemRoot\\System32\\drivers\\%s%s", sDriverFile, m_sDriverExt);
}

//---------------------------------------------------------------------------

BOOL LoadDriver98::Create()
{
	ClearErr();
	
	char sKey[1024];
	sprintf(sKey, "System\\CurrentControlSet\\Services\\%s", m_sDriverName);

	// updating registry
	HKEY hKey;
	ULONG rc;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, "", 0, KEY_ALL_ACCESS, NULL, &hKey, &rc);
	if (rc == REG_OPENED_EXISTING_KEY) {
		PrintWrn(m_sDriverName);
		PrintWrn(" already exists in Win-98 registry\n");
	} else if (rc != REG_CREATED_NEW_KEY && rc != ERROR_SUCCESS) {
		PrintErr("Cannot open registry key:\n");
		PrintLastError();
		return FALSE;
	}

	DWORD val = 2;
	RegSetValueEx(hKey, "Start", 0, REG_BINARY, (PBYTE) &m_iStartLevel, 4);
	val = 1;
	RegSetValueEx(hKey, "Type", 0, REG_BINARY, (PBYTE) &val, 4);
	val = 1;
	RegSetValueEx(hKey, "ErrorControl", 0, REG_BINARY, (PBYTE) &val, 4);
//	RegSetValueEx(hKey, "Group", 0, REG_SZ, (PBYTE) "Extended Base", 14);
	RegSetValueEx(hKey, "ImagePath", 0, REG_SZ, (PBYTE) m_sDriverPath, 41);
	RegSetValueEx(hKey, "DisplayName", 0, REG_SZ, (PBYTE) m_sDriverName, 10);
	
	RegCloseKey(hKey);
	
	return TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver98::Delete()
{
	ClearErr();
	LONG rc;
	char sKey[1024];
	sprintf(sKey, "System\\CurrentControlSet\\Services\\%s", m_sDriverName);
	rc = RegDeleteKey(HKEY_LOCAL_MACHINE, sKey);
	if (rc!=ERROR_SUCCESS) {
		PrintErr(m_sDriverName);
		PrintErr(" does not exist in Win-98 registry\n");
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver98::Start()
{
	PrintWrn("Loading driver currently not supported for WIN 98.\n");
	return FALSE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver98::Stop()
{
	PrintWrn("UnLoading driver currently not supported for WIN 98.\n");
	return FALSE;
}

//---------------------------------------------------------------------------

BOOL LoadDriver98::Copy(const char *sDllSrc)
{
	char sSystemPath[1024];
	char sDriverSrc[1024];
	char sDriverDest[1024];
	char sDllDest[1024];

	ClearErr();

	GetSystemDirectory(sSystemPath, sizeof(sSystemPath));
	if (sSystemPath[strlen(sSystemPath) - 1] != '\\') 
		strcat(sSystemPath, "\\");

	sprintf(sDriverSrc, "%s%s", m_sDriverFile, m_sDriverExt);
	sprintf(sDriverDest, "%s%s%s%s", sSystemPath, "drivers\\", m_sDriverFile, m_sDriverExt);
	if (!CopyFile(sDriverSrc, sDriverDest, FALSE)) {
		PrintErr("Couldn't copy file %s to %s:\n", sDriverSrc, sDriverDest);
		PrintLastError();
		return(FALSE);
	}

	if (sDllSrc != NULL && sDllSrc[0] != '\0') {
		sprintf(sDllDest, "%s%s", sSystemPath, sDllSrc);
		if (!CopyFile(sDllSrc, sDllDest, FALSE)) {
			PrintErr("Couldn't copy file %s to %s:\n", sDllSrc, sDllDest);
			PrintLastError();
			return(FALSE);
		}
	}

	return(TRUE);
}

//***************************************************************************

LoadDriverNT::LoadDriverNT()
{
	ClearErr();

	if (!OpenSCMHandle()) {
		PrintErr("Cannot open service control manager:\n");
		PrintLastError();
		PrintErr("You need Administrator rights to do this task.\n");
	}
}

//---------------------------------------------------------------------------

LoadDriverNT::~LoadDriverNT()
{
	if (m_h)
		CloseSCMHandle();
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::OpenSCMHandle()
{
	m_h = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	return m_h!=NULL;
}

//---------------------------------------------------------------------------

void LoadDriverNT::CloseSCMHandle()
{
	CloseServiceHandle(m_h);
	m_h = NULL;
}

//---------------------------------------------------------------------------

void LoadDriverNT::SetPaths(char *sDriverName, char *sDriverFile, char *sDriverPath)
{
	LoadDriver::SetPaths(sDriverName, sDriverFile, sDriverPath);
	strcpy(m_sDriverExt, ".SYS");
	if (!sDriverPath)
		sprintf(m_sDriverPath, "\\SystemRoot\\System32\\drivers\\%s%s", sDriverFile, m_sDriverExt);
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::CreateReg()
{
	ClearErr();
	
	char sKey[1024];
	sprintf (sKey, "System\\CurrentControlSet\\Services\\%s", m_sDriverName);
	// updating registry
	HKEY hKey;
	ULONG rc;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, "", 0, KEY_ALL_ACCESS, NULL, &hKey, &rc);
	if (rc == REG_OPENED_EXISTING_KEY) {
		PrintWrn(m_sDriverName);
		PrintWrn(" already exists in Win-NT registry\n");
	} else if (rc != REG_CREATED_NEW_KEY && rc != ERROR_SUCCESS) {
		PrintErr("Cannot open registry key:\n");
		PrintLastError();
		return FALSE;
	}
	
	DWORD val = 2;
	RegSetValueEx(hKey, "Start", 0, REG_DWORD, (PBYTE) &m_iStartLevel, 4);
	val = 1;
	RegSetValueEx(hKey, "Type", 0, REG_DWORD, (PBYTE) &val, 4);
	val = 1;
	RegSetValueEx(hKey, "ErrorControl", 0, REG_DWORD, (PBYTE) &val, 4);
	RegSetValueEx(hKey, "Group", 0, REG_SZ, (PBYTE) "Extended Base", 14);
	RegSetValueEx(hKey, "ImagePath", 0, REG_SZ, (PBYTE) m_sDriverPath, 41);
	RegSetValueEx(hKey, "DisplayName", 0, REG_SZ, (PBYTE) m_sDriverName, 10);
	
	RegCloseKey(hKey);
	
	return TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::DeleteReg()
{
	ClearErr();
	LONG rc;
	char sKey[1024];
	sprintf(sKey, "System\\CurrentControlSet\\Services\\%s", m_sDriverName);
	rc = RegDeleteKey (HKEY_LOCAL_MACHINE, sKey);
	if (rc != ERROR_SUCCESS) {
		PrintErr(m_sDriverName);
		PrintErr(" does not exist in Win-NT registry\n");
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Created()
{
	SC_HANDLE  sr;
	BOOL fCreated = FALSE;

	ClearErr();

	if (!m_h)
		return FALSE;
	
	sr = OpenService(m_h, m_sDriverName, SERVICE_ALL_ACCESS );

	if (sr == NULL) {
		PrintLastError();
	} else {
		fCreated = TRUE;
	}

	if (sr) CloseServiceHandle(sr);
	
	return fCreated;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Create()
{
	SC_HANDLE sr;
	BOOL fOk = TRUE;

	ClearErr();
	
	if (m_fUseReg)
		return CreateReg();
	
	if (!m_h)
		return FALSE;
	
	sr = CreateService(
		m_h,					// handle to service control manager database  
		m_sDriverName,			// pointer to name of service to start 
		m_sDriverName,			// pointer to display name 
		SERVICE_ALL_ACCESS,		// type of access to service 
		SERVICE_KERNEL_DRIVER,	// type of service 
		m_iStartLevel,			// when to start service 
		SERVICE_ERROR_NORMAL,	// severity if service fails to start 
		m_sDriverPath,			// pointer to name of binary file 
		NULL,					// pointer to name of load ordering group 
		NULL,					// pointer to variable to get tag identifier 
		NULL,					// pointer to array of dependency names 
		NULL,					// pointer to account name of service 
		NULL					// pointer to password for service account 
	);	
	if (sr == NULL) {
		PrintLastError();
		fOk = FALSE;
	}
	
	if (sr) CloseServiceHandle(sr);
	
	return fOk;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Running()
{
	SC_HANDLE  sr;
	BOOL fRunning = FALSE;
	SERVICE_STATUS ServiceStatus;

	ClearErr();

	if (!m_h)
		return FALSE;
	
	sr = OpenService(m_h, m_sDriverName, SERVICE_ALL_ACCESS );

	if (sr == NULL) {
		PrintLastError();
	} else {
		BOOL rc = QueryServiceStatus(
			sr,	// handle of service 
			&ServiceStatus	// address of service status structure 
		);
		if (rc) {
			if (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
				fRunning = TRUE;
			}
		} else {
			PrintLastError();
		}
	}

	if (sr) CloseServiceHandle(sr);
	
	return fRunning;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Start()
{
	SC_HANDLE  sr;
	BOOL fOk = TRUE;

	ClearErr();

	if (!m_h)
		return FALSE;
	
	sr = OpenService(m_h, m_sDriverName, SERVICE_ALL_ACCESS );

	if (sr == NULL) {
		PrintLastError();
		fOk = FALSE;
	} else {
		BOOL rc = StartService(
			sr,		// service identifier
			0,		// number of arguments
			NULL	// pointer to arguments
		);
		if (!rc) {
			PrintLastError();
			fOk = FALSE;
		}
	}
	
	if (sr) CloseServiceHandle(sr);
	
	return fOk;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Stop()
{
	SC_HANDLE  sr;
	BOOL fOk = TRUE;
	SERVICE_STATUS	serviceStatus;

	ClearErr();

	sr = OpenService(m_h ,m_sDriverName ,SERVICE_ALL_ACCESS );
	
	if (sr == NULL) {
		PrintLastError();
		fOk = FALSE;
	}

	if (sr) {
		BOOL rc = ControlService(sr, SERVICE_CONTROL_STOP, &serviceStatus);
		if (!rc) {
			switch (GetLastError()) {
			case ERROR_SERVICE_NOT_ACTIVE:
				rc = TRUE;
				break;
			case ERROR_INVALID_SERVICE_CONTROL:
				rc = TRUE;
				fOk = FALSE;
				PrintErr(
					"The requested control is not valid for this service.\n"
					"Presumably the service was already stopped.\n"
				);
				break;
			}
		}
		if (SERVICE_STOPPED != serviceStatus.dwCurrentState)
			rc = FALSE;
		if (!rc) {
			PrintLastError();
			fOk = FALSE;
		}
	}
	
	if (sr) CloseServiceHandle(sr);
	
	return fOk;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Delete()
{
	SC_HANDLE  sr;
	BOOL fOk = TRUE;

	ClearErr();

	if (m_fUseReg)
		return DeleteReg();

	sr = OpenService(m_h, m_sDriverName, SERVICE_ALL_ACCESS );

	if (sr == NULL) {
		PrintLastError();
		fOk = FALSE;
	}

	if (sr) {
		DWORD rc = DeleteService(sr);

		if (!rc) {
			switch(GetLastError()) {
			case ERROR_ACCESS_DENIED:	// The specified handle was not opened with DELETE access.
				break ;
			case ERROR_SERVICE_MARKED_FOR_DELETE:	// The specified service has already been marked for deletion
				rc = TRUE;
				break;
			}
		}
		
		if (!rc) fOk = FALSE;
	}
	
	if (sr) CloseServiceHandle(sr);
	
	return fOk;
}

//---------------------------------------------------------------------------

BOOL LoadDriverNT::Copy(const char *sDllSrc)
{
#if defined USE_SETUPAPI
	HINF hInf;
	char szInfFileName[MAX_PATH];
	char szInfSection[MAX_PATH];
	BOOL bResult = TRUE;
	HSPFILEQ FileQueue;
	PVOID context;

	ClearErr();

	strcpy (szInfFileName, ".\\pcl-pci.inf");

	hInf = SetupOpenInfFile (szInfFileName, NULL, INF_STYLE_WIN4, NULL);

	if (!hInf || (hInf == INVALID_HANDLE_VALUE)) {
		PrintErr("Couldn't open file %s.\n", szInfFileName);
		return FALSE;
	}

	FileQueue = SetupOpenFileQueue();
	if (!FileQueue || (FileQueue == INVALID_HANDLE_VALUE)) { 
		PrintErr("Couldn't open file queue.\n");
		SetupCloseInfFile(hInf); 
		return FALSE; 
	}

	context = SetupInitDefaultQueueCallback(
		NULL	// HWND OwnerWindow
				// parent window of any dialog boxes created by the callback routine 
	); 

	if (!context) {
		PrintErr("Couldn't open queue callback context.\n");
		return FALSE;
	}

	strcpy(szInfSection, TEXT("pcl_pci.NT"));

	bResult = SetupInstallFilesFromInfSection(
		hInf,			// HINF InfHandle, // handle to the INF file 
		NULL,			// HINF LayoutInfHandle, // optional, layout INF handle 
		FileQueue,		// HSPFILEQ FileQueue, // handle to the file queue 
		szInfSection,	// PCTSTR SectionName, // section to install files from 
		".",			// PCTSTR SourceRootPath, // optional, root path to source files 
		NULL			// UINT CopyFlags // optional, specifies copy behavior 
	);
	if (!bResult) {
		PrintErr("Couldn't find section %s in file.\n", szInfSection, szInfFileName);
	} else {
		bResult = SetupCommitFileQueue( 
			NULL,		// Owner 
			FileQueue,	// Queue with the file list 
			SetupDefaultQueueCallback,	// This is our handler, it calls the default for us
			context		// Pointer to resources allocated with SetupInitDefaultQueueCallback/Ex
		); 
		if (!bResult) {
			PrintErr("Couldn't copy files.\n");
		}
	}

	SetupCloseFileQueue(FileQueue);    
	SetupCloseInfFile(hInf);

	return(bResult);
#else
	return(FALSE);
#endif
}

//---------------------------------------------------------------------------

OS_VER GetOsVer(void)
{
	OSVERSIONINFO lVerInfo;
	lVerInfo.dwOSVersionInfoSize = sizeof(lVerInfo);

	if (!GetVersionEx(&lVerInfo)) {
		printf("Cannot get OS version\n");
		return VER_UNKNOWN;
	}

	switch (lVerInfo.dwPlatformId) {
	case VER_PLATFORM_WIN32_NT:
		if (lVerInfo.dwMajorVersion < 4) {
			return(VER_NT3);
		} else if (lVerInfo.dwMajorVersion > 4) {
			return(VER_W2K);
		}
		return(VER_NT4);
	case VER_PLATFORM_WIN32_WINDOWS:
		if (
			(lVerInfo.dwMajorVersion > 4)
		||
			((lVerInfo.dwMajorVersion == 4) && (lVerInfo.dwMinorVersion > 0))
		) {
			return(VER_W98);
		}
		return(VER_W95);
	case VER_PLATFORM_WIN32s:
		return(VER_W31);
	}	 
	return(VER_UNKNOWN);
}

//---------------------------------------------------------------------------

LoadDriver *NewLoadDriver(BOOL fVxd)
{
	switch (GetOsVer()) {
	case VER_W2K:
	case VER_NT4:
		if (fVxd) {
//			printf("-vxd is not supported on WinNT");
//			break;
		}
		return new LoadDriverNT();
		break;
	case VER_W95:
		return new LoadDriver95();
		break;
	case VER_W98:
		if(fVxd) {
			return new LoadDriver95();
		} else {
			return new LoadDriver98();
		}
		break;
	default:
//		printf("This program can run only on Win95/Win98/WinNT/2000");
		break;
	}
	return NULL;
}

//---------------------------------------------------------------------------
