# Microsoft Developer Studio Generated NMAKE File, Based on PCIMilTreiber.dsp
!IF "$(CFG)" == ""
CFG=PCIMilTreiber - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PCIMilTreiber - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PCIMilTreiber - Win32 Release" && "$(CFG)" != "PCIMilTreiber - Win32 Debug" && "$(CFG)" != "PCIMilTreiber - Win32 Slave" && "$(CFG)" != "PCIMilTreiber - Win32 VisualBasic" && "$(CFG)" != "PCIMilTreiber - Win32 VisualBasic_Slave"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PCIMilTreiber.mak" CFG="PCIMilTreiber - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PCIMilTreiber - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PCIMilTreiber - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PCIMilTreiber - Win32 Slave" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PCIMilTreiber - Win32 VisualBasic" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PCIMilTreiber - Win32 VisualBasic_Slave" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "PCIMilTreiber - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PCIMilTreiber.dll"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\MilDriver.obj"
	-@erase "$(INTDIR)\mydriver_diag.obj"
	-@erase "$(INTDIR)\mydriver_lib.obj"
	-@erase "$(INTDIR)\print_struct.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PCIMilTreiber.dll"
	-@erase "$(OUTDIR)\PCIMilTreiber.exp"
	-@erase "$(OUTDIR)\PCIMilTreiber.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCIMILTREIBER_EXPORTS" /Fp"$(INTDIR)\PCIMilTreiber.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PCIMilTreiber.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\PCIMilTreiber.pdb" /machine:I386 /out:"$(OUTDIR)\PCIMilTreiber.dll" /implib:"$(OUTDIR)\PCIMilTreiber.lib" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MilDriver.obj" \
	"$(INTDIR)\mydriver_diag.obj" \
	"$(INTDIR)\mydriver_lib.obj" \
	"$(INTDIR)\print_struct.obj"

"$(OUTDIR)\PCIMilTreiber.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PCIMilTreiber.dll" "$(OUTDIR)\PCIMilTreiber.bsc"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MilDriver.obj"
	-@erase "$(INTDIR)\MilDriver.sbr"
	-@erase "$(INTDIR)\mydriver_diag.obj"
	-@erase "$(INTDIR)\mydriver_diag.sbr"
	-@erase "$(INTDIR)\mydriver_lib.obj"
	-@erase "$(INTDIR)\mydriver_lib.sbr"
	-@erase "$(INTDIR)\print_struct.obj"
	-@erase "$(INTDIR)\print_struct.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PCIMilTreiber.bsc"
	-@erase "$(OUTDIR)\PCIMilTreiber.dll"
	-@erase "$(OUTDIR)\PCIMilTreiber.exp"
	-@erase "$(OUTDIR)\PCIMilTreiber.ilk"
	-@erase "$(OUTDIR)\PCIMilTreiber.lib"
	-@erase "$(OUTDIR)\PCIMilTreiber.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCIMILTREIBER_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PCIMilTreiber.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PCIMilTreiber.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MilDriver.sbr" \
	"$(INTDIR)\mydriver_diag.sbr" \
	"$(INTDIR)\mydriver_lib.sbr" \
	"$(INTDIR)\print_struct.sbr"

"$(OUTDIR)\PCIMilTreiber.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\PCIMilTreiber.pdb" /debug /machine:I386 /out:"$(OUTDIR)\PCIMilTreiber.dll" /implib:"$(OUTDIR)\PCIMilTreiber.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MilDriver.obj" \
	"$(INTDIR)\mydriver_diag.obj" \
	"$(INTDIR)\mydriver_lib.obj" \
	"$(INTDIR)\print_struct.obj"

"$(OUTDIR)\PCIMilTreiber.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Slave"

OUTDIR=.\PCIMilTreiber_Slave
INTDIR=.\PCIMilTreiber_Slave
# Begin Custom Macros
OutDir=.\PCIMilTreiber_Slave
# End Custom Macros

ALL : "$(OUTDIR)\PCIMilTreiberS.dll" "$(OUTDIR)\PCIMilTreiber.bsc"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MilDriver.obj"
	-@erase "$(INTDIR)\MilDriver.sbr"
	-@erase "$(INTDIR)\mydriver_diag.obj"
	-@erase "$(INTDIR)\mydriver_diag.sbr"
	-@erase "$(INTDIR)\mydriver_lib.obj"
	-@erase "$(INTDIR)\mydriver_lib.sbr"
	-@erase "$(INTDIR)\print_struct.obj"
	-@erase "$(INTDIR)\print_struct.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PCIMilTreiber.bsc"
	-@erase "$(OUTDIR)\PCIMilTreiberS.dll"
	-@erase "$(OUTDIR)\PCIMilTreiberS.exp"
	-@erase "$(OUTDIR)\PCIMilTreiberS.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCIMILTREIBER_EXPORTS" /D "PCIMilTreiber_Slave" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PCIMilTreiber.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PCIMilTreiber.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MilDriver.sbr" \
	"$(INTDIR)\mydriver_diag.sbr" \
	"$(INTDIR)\mydriver_lib.sbr" \
	"$(INTDIR)\print_struct.sbr"

"$(OUTDIR)\PCIMilTreiber.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\PCIMilTreiberS.pdb" /machine:I386 /out:"$(OUTDIR)\PCIMilTreiberS.dll" /implib:"$(OUTDIR)\PCIMilTreiberS.lib" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MilDriver.obj" \
	"$(INTDIR)\mydriver_diag.obj" \
	"$(INTDIR)\mydriver_lib.obj" \
	"$(INTDIR)\print_struct.obj"

"$(OUTDIR)\PCIMilTreiberS.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic"

OUTDIR=.\PCIMilTreiber_VisualBasic
INTDIR=.\PCIMilTreiber_VisualBasic
# Begin Custom Macros
OutDir=.\PCIMilTreiber_VisualBasic
# End Custom Macros

ALL : "$(OUTDIR)\PCIMilTreiberVB.dll" "$(OUTDIR)\PCIMilTreiber.bsc"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MilDriver.obj"
	-@erase "$(INTDIR)\MilDriver.sbr"
	-@erase "$(INTDIR)\mydriver_diag.obj"
	-@erase "$(INTDIR)\mydriver_diag.sbr"
	-@erase "$(INTDIR)\mydriver_lib.obj"
	-@erase "$(INTDIR)\mydriver_lib.sbr"
	-@erase "$(INTDIR)\print_struct.obj"
	-@erase "$(INTDIR)\print_struct.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PCIMilTreiber.bsc"
	-@erase "$(OUTDIR)\PCIMilTreiberVB.dll"
	-@erase "$(OUTDIR)\PCIMilTreiberVB.exp"
	-@erase "$(OUTDIR)\PCIMilTreiberVB.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCIMILTREIBER_EXPORTS" /D "PCIMilTreiber_VisualBasic" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PCIMilTreiber.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PCIMilTreiber.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MilDriver.sbr" \
	"$(INTDIR)\mydriver_diag.sbr" \
	"$(INTDIR)\mydriver_lib.sbr" \
	"$(INTDIR)\print_struct.sbr"

"$(OUTDIR)\PCIMilTreiber.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\PCIMilTreiberVB.pdb" /machine:I386 /out:"$(OUTDIR)\PCIMilTreiberVB.dll" /implib:"$(OUTDIR)\PCIMilTreiberVB.lib" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MilDriver.obj" \
	"$(INTDIR)\mydriver_diag.obj" \
	"$(INTDIR)\mydriver_lib.obj" \
	"$(INTDIR)\print_struct.obj"

"$(OUTDIR)\PCIMilTreiberVB.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"

OUTDIR=.\PCIMilTreiber_VisualBasic_Slave
INTDIR=.\PCIMilTreiber_VisualBasic_Slave
# Begin Custom Macros
OutDir=.\PCIMilTreiber_VisualBasic_Slave
# End Custom Macros

ALL : "$(OUTDIR)\PCIMilTreiberVBS.dll" "$(OUTDIR)\PCIMilTreiber.bsc"


CLEAN :
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MilDriver.obj"
	-@erase "$(INTDIR)\MilDriver.sbr"
	-@erase "$(INTDIR)\mydriver_diag.obj"
	-@erase "$(INTDIR)\mydriver_diag.sbr"
	-@erase "$(INTDIR)\mydriver_lib.obj"
	-@erase "$(INTDIR)\mydriver_lib.sbr"
	-@erase "$(INTDIR)\print_struct.obj"
	-@erase "$(INTDIR)\print_struct.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PCIMilTreiber.bsc"
	-@erase "$(OUTDIR)\PCIMilTreiberVBS.dll"
	-@erase ".\PCIMilTreiber_VisualBasic\PCIMilTreiberVB.exp"
	-@erase ".\PCIMilTreiber_VisualBasic\PCIMilTreiberVB.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PCIMILTREIBER_EXPORTS" /D "PCIMilTreiber_VisualBasic" /D "PCIMilTreiber_Slave" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PCIMilTreiber.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PCIMilTreiber.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\MilDriver.sbr" \
	"$(INTDIR)\mydriver_diag.sbr" \
	"$(INTDIR)\mydriver_lib.sbr" \
	"$(INTDIR)\print_struct.sbr"

"$(OUTDIR)\PCIMilTreiber.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\PCIMilTreiberVBS.pdb" /machine:I386 /out:"$(OUTDIR)\PCIMilTreiberVBS.dll" /implib:"PCIMilTreiber_VisualBasic/PCIMilTreiberVB.lib" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\MilDriver.obj" \
	"$(INTDIR)\mydriver_diag.obj" \
	"$(INTDIR)\mydriver_lib.obj" \
	"$(INTDIR)\print_struct.obj"

"$(OUTDIR)\PCIMilTreiberVBS.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PCIMilTreiber.dep")
!INCLUDE "PCIMilTreiber.dep"
!ELSE 
!MESSAGE Warning: cannot find "PCIMilTreiber.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PCIMilTreiber - Win32 Release" || "$(CFG)" == "PCIMilTreiber - Win32 Debug" || "$(CFG)" == "PCIMilTreiber - Win32 Slave" || "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic" || "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"
SOURCE=.\main.cpp

!IF  "$(CFG)" == "PCIMilTreiber - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Slave"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MilDriver.cpp

!IF  "$(CFG)" == "PCIMilTreiber - Win32 Release"


"$(INTDIR)\MilDriver.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Debug"


"$(INTDIR)\MilDriver.obj"	"$(INTDIR)\MilDriver.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Slave"


"$(INTDIR)\MilDriver.obj"	"$(INTDIR)\MilDriver.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic"


"$(INTDIR)\MilDriver.obj"	"$(INTDIR)\MilDriver.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"


"$(INTDIR)\MilDriver.obj"	"$(INTDIR)\MilDriver.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mydriver_diag.c

!IF  "$(CFG)" == "PCIMilTreiber - Win32 Release"


"$(INTDIR)\mydriver_diag.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Debug"


"$(INTDIR)\mydriver_diag.obj"	"$(INTDIR)\mydriver_diag.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Slave"


"$(INTDIR)\mydriver_diag.obj"	"$(INTDIR)\mydriver_diag.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic"


"$(INTDIR)\mydriver_diag.obj"	"$(INTDIR)\mydriver_diag.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"


"$(INTDIR)\mydriver_diag.obj"	"$(INTDIR)\mydriver_diag.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mydriver_lib.c

!IF  "$(CFG)" == "PCIMilTreiber - Win32 Release"


"$(INTDIR)\mydriver_lib.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Debug"


"$(INTDIR)\mydriver_lib.obj"	"$(INTDIR)\mydriver_lib.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Slave"


"$(INTDIR)\mydriver_lib.obj"	"$(INTDIR)\mydriver_lib.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic"


"$(INTDIR)\mydriver_lib.obj"	"$(INTDIR)\mydriver_lib.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"


"$(INTDIR)\mydriver_lib.obj"	"$(INTDIR)\mydriver_lib.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\print_struct.c

!IF  "$(CFG)" == "PCIMilTreiber - Win32 Release"


"$(INTDIR)\print_struct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Debug"


"$(INTDIR)\print_struct.obj"	"$(INTDIR)\print_struct.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 Slave"


"$(INTDIR)\print_struct.obj"	"$(INTDIR)\print_struct.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic"


"$(INTDIR)\print_struct.obj"	"$(INTDIR)\print_struct.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PCIMilTreiber - Win32 VisualBasic_Slave"


"$(INTDIR)\print_struct.obj"	"$(INTDIR)\print_struct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

