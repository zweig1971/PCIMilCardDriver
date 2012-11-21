//***************************************************************************
//*																			*
//*	Windows DLL AVPCL.DLL													*
//*	Setup Program															*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*	Portions (c) 1999-2002 Jungo, see http://www.jungo.com					*
//*																			*
//***************************************************************************

// <DrvLoad.h> Alpermann+Velte DLL Setup Program for AV-PCL
// @(#)DrvLoad.h 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte

//---------------------------------------------------------------------------

#ifndef _LOADDRV_H_
#define _LOADDRV_H_ 

class LoadDriver
{
public:
	LoadDriver();
	virtual ~LoadDriver();
	virtual BOOL Init(
		char *sDriverName,
		char *sDriverFile,
		DWORD startLevel = SERVICE_AUTO_START,
		char *sDriverPath = NULL,
		BOOL fUseReg = FALSE
	);
	virtual BOOL Create() = 0;
	virtual BOOL Delete() = 0;
	virtual BOOL Start() = 0;
	virtual BOOL Stop() = 0;
	virtual BOOL Copy(const char *sDllDrc = NULL) = 0;
	char *GetLastMsg();
	virtual BOOL HasErr();
	virtual BOOL HasWrn();
	virtual BOOL HasMsg();
	
protected:
	virtual void PrintErr(const char *msg, ...);
	virtual void PrintLastError();
	virtual void PrintWrn(const char *msg);
	virtual void ClearErr();
	virtual void SetPaths(char *sDriverName, char *sDriverFile, char *sDriverPath);
	virtual BOOL OpenSCMHandle()=0;
	virtual void CloseSCMHandle()=0;
	
	char m_sLastMsg[2000];
	BOOL m_fUseReg;
	HANDLE m_h;
	BOOL m_fHasErr;
	BOOL m_fHasWrn;
	DWORD m_iStartLevel;
	char m_sDriverName[1000];
	char m_sDriverFile[1000];
	char m_sDriverPath[1000];
	char m_sDriverExt[100];
};

class LoadDriver95 : public LoadDriver
{
public:
	LoadDriver95();
	virtual ~LoadDriver95();
	virtual BOOL Create();
	virtual BOOL Delete();
	virtual BOOL Start();
	virtual BOOL Stop();
	virtual BOOL Copy(const char *sDllDrc = NULL);
protected:
	virtual BOOL OpenSCMHandle();
	virtual void CloseSCMHandle();
	virtual void SetPaths(
		char *sDriverName,
		char *sDriverFile,
		char *sDriverPath
	);
};

class LoadDriverNT : public LoadDriver
{
public:
	LoadDriverNT();
	virtual ~LoadDriverNT();
	virtual BOOL Create();
	virtual BOOL Delete();
	virtual BOOL Start();
	virtual BOOL Stop();
	virtual BOOL CreateReg();
	virtual BOOL DeleteReg();
	virtual BOOL Copy(const char *sDllDrc = NULL);
	BOOL Running();
	BOOL Created();
protected:
	virtual BOOL OpenSCMHandle();
	virtual void CloseSCMHandle();
	virtual void SetPaths(
		char *sDriverName,
		char *sDriverFile,
		char *sDriverPath
	);
};

class LoadDriver98 : public LoadDriver
{
public:
	LoadDriver98();
	virtual ~LoadDriver98();
	virtual BOOL Create();
	virtual BOOL Delete();
	virtual BOOL Start();
	virtual BOOL Stop();
	virtual BOOL Copy(const char *sDllDrc = NULL);
protected:
	virtual BOOL OpenSCMHandle();
	virtual void CloseSCMHandle();
	virtual void SetPaths(
		char *sDriverName,
		char *sDriverFile,
		char *sDriverPath
	);
};

LoadDriver *NewLoadDriver(BOOL fVxd);

typedef enum {
	VER_UNKNOWN,
	VER_W31,
	VER_W95,
	VER_W98,
	VER_NT3,
	VER_NT4,
	VER_W2K
} OS_VER;

OS_VER GetOsVer(void);

#endif _LOADDRV_H_
