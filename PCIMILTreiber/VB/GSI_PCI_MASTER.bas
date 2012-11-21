Attribute VB_Name = "MILBUS"
Rem-----------------------------
Rem GSI Gesellschaft für
Rem Schwerionenforschung mbH,
Rem Darmstadt, Germany
Rem
Rem PCIMilTreiberVB.bas
Rem
Rem Autor           : P.Moritz / M.Zweig
Rem Version         : 0.01
Rem letzte Änderung : 01.04.05
Rem------------------------------

Option Explicit

Rem Bitmasken für status_1
Rem------------------------
Global Const Interlock = &H1     'Bit 0
Global Const Dta_Rdy = &H2
Global Const Dta_Req = &H4
Global Const Trm_Rdy = &H8
Global Const Mil_Rcv_Rdy = &H10
Global Const Mil_Fifo_Full = &H20
Global Const Mil_Fifo_On = &H40
Global Const Cmd_Rcv = &H80
Global Const Timeout_1 = &H100
Global Const Timeout_2 = &H200
Global Const Debounce_On = &H400
Global Const Frei_D11 = &H800
Global Const Evt_Fifo_Not_Empty = &H1000
Global Const Evt_Fifo_Full = &H2000
Global Const EvFilter_On = &H4000
Global Const Master_Slave = &H8000
Global Const Puls_In1 = &H10000
Global Const Puls_In2 = &H20000
Global Const Puls_In3 = &H40000   'Bit18

Rem Bitmasken fuer das Control Register 1
Rem (Ctrl_Reg1, nur setzen)
Rem --------------------------------------
Global Const Ctrl_EvFilter_On = &H1
Global Const Ctrl_Debounce_On = &H2
Global Const Ctrl_Mil_Fifo_On = &H4
Global Const Ctrl_En_Intr1 = &H8
Global Const Ctrl_En_Intr2 = &H10
Global Const Ctrl_EvFilter_8_12 = &H20
Global Const Ctrl_Puls1_Rahmen_On = &H40
Global Const Ctrl_Puls2_Rahmen_On = &H80
Global Const Ctrl_Puls3_Rahmen_On = &H100
Global Const Ctrl_Puls4_Rahmen_On = &H200
Global Const Ctrl_Puls5_Rahmen_On = &H400

Rem Bitmasken fuer den das EventFilter
Rem ------------------------------------
Global Const EvFilter_EventOn = &H1
Global Const EvFilter_EventTimerRun = &H2
Global Const EvFilter_Start_Puls1 = &H4
Global Const EvFilter_Stop_Puls1 = &H8
Global Const EvFilter_Start_Puls2 = &H10
Global Const EvFilter_Stop_Puls2 = &H20

Rem Fehlermeldungen
Rem ----------------
Global Const StatusOK = &H0
Global Const VendorID_Error = &H1
Global Const DeviceID_Error = &H2
Global Const CountCards_Error = &H4
Global Const Open_Failure = &H8
Global Const IncorrectWinDriver = &H10
Global Const InvalidHandleValue = &H20
Global Const Failed_Open_WinDrv_Device = &H40
Global Const MilBus_Busy = &H80
Global Const TestTimerError = &H100
Global Const Fifo_Full = &H200
Global Const CardNrError = &H400
Global Const NoPCICartOpen = &H800
Global Const NoPCMil = &H1000
Global Const ReadMilBusTimeOut = &H2000
Global Const WriteMilBusTimeOut = &H4000
Global Const FifoNotEmpty = &H8000
Global Const FifoNotCleared = &H10000
Global Const NoDataToRead = &H20000
Global Const EventNrError = &H40000
Global Const IFKNrError = &H80000
Global Const FailedEnableIntr = &H100000
Global Const IntMaskError = &H200000
Global Const GeneralError = &H400000
Global Const MasterSlaveError = &H800000
Global Const UnrequestedReceiv = &H1000000

Rem Aufzaehlungen
Rem ----------------
Global Const EventOn As Integer = 0
Global Const EventOff As Integer = 1
Global Const EventTimerClear As Integer = 2
Global Const EventTimerRun As Integer = 3
Global Const EventClear As Integer = 4
Global Const Puls1_On As Integer = 5
Global Const Puls1_Off As Integer = 6
Global Const Puls2_On As Integer = 7
Global Const Puls2_Off As Integer = 8
Global Const Cntrl_EvFilter_On As Integer = 9
Global Const Cntrl_EvFilter_Off As Integer = 10
Global Const Cntrl_Debounce_On As Integer = 11
Global Const Cntrl_Debounce_Off As Integer = 12
Global Const Cntrl_Mil_FiFo_On As Integer = 13
Global Const Cntrl_Mil_FiFo_Off As Integer = 14
Global Const Cntrl_En_Intr1 As Integer = 15
Global Const Cntrl_Dis_Intr1 As Integer = 16
Global Const Cntrl_En_Intr2 As Integer = 17
Global Const Cntrl_Dis_Intr2 As Integer = 18
Global Const Cntrl_EvFilter_12_Bit As Integer = 19
Global Const Cntrl_EvFilter_8_Bit As Integer = 20
Global Const Cntrl_Puls1_Rahmen_On As Integer = 21
Global Const Cntrl_Puls1_Rahmen_Off As Integer = 22
Global Const Cntrl_Puls2_Rahmen_On As Integer = 23
Global Const Cntrl_Puls2_Rahmen_Off As Integer = 24
Global Const Cntrl_Puls3_Rahmen_On As Integer = 25
Global Const Cntrl_Puls3_Rahmen_Off As Integer = 26
Global Const Cntrl_Puls4_Rahmen_On As Integer = 27
Global Const Cntrl_Puls4_Rahmen_Off As Integer = 28
Global Const Cntrl_Puls5_Rahmen_On As Integer = 29
Global Const Cntrl_Puls5_Rahmen_Off As Integer = 30
Global Const PulsBuchse_1 As Integer = 31
Global Const PulsBuchse_2 As Integer = 32
Global Const Puls As Integer = 33
Global Const RahmenPuls As Integer = 34
Global Const IntSetPriorityNormal As Integer = 35
Global Const IntSetPriorityHigh As Integer = 36
Global Const IntSetPriorityCritical As Integer = 37

Rem Funktion Deklarationen
Rem ---------------------------------------------------------------
Declare Function MILDRIVER_VersionNr Lib "PCIMilTreiberVB.dll" _
Alias "_MILDRIVER_VersionNr@4" _
(ByRef version As Byte) _
As Long

Declare Function PCI_PCIcardCount Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_PCIcardCount@0" () As Long

Declare Function PCI_DriverOpen Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_DriverOpen@4" _
(ByVal CardNr As Byte) _
As Long

Declare Function PCI_IfkOnline Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IfkOnline@16" _
 (ByVal CardNr As Byte, _
  ByVal IFCNr As Byte, _
  ByRef ReturnIFKNR As Byte, _
  ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IfkRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IfkRead@20" _
(ByVal CardNr As Byte, _
 ByVal IFKAdress As Byte, _
 ByVal IFKFunktionCode As Byte, _
 ByRef data As Integer, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IfkWrite Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IfkWrite@20" _
 (ByVal CardNr As Byte, _
  ByVal IFKAdress As Byte, _
  ByVal IFKFunktionCode As Byte, _
  ByVal data As Integer, _
  ByVal ErrorStatus As Long) _
As Long

Declare Function PCI_PCIcardReset Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_PCIcardReset@8" _
(ByVal CardNr As Byte, _
ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_FctCodeSnd Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_FctCodeSnd@16" _
(ByVal CardNr As Byte, _
 ByVal IFKAdress As Byte, _
 ByVal FunktionCode As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_ErrorString Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_ErrorString@8" _
(ByVal status As Long, _
 ByRef error_string As Byte) _
As Long

Declare Function PCI_TimerWait Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_TimerWait@16" _
(ByVal CardNr As Byte, _
 ByVal Time As Integer, _
 ByVal TimeBase As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_TimerSet Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_TimerSet@16" _
(ByVal CardNr As Byte, _
 ByVal Time As Integer, _
 ByVal TimeBase As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_StatusRegRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_StatusRegRead@12" _
(ByVal CardNr As Byte, _
 ByRef StatusReg As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_MilBusWrite Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_MilBusWrite@12" _
(ByVal CardNr As Byte, _
 ByVal data As Integer, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_DriverClose Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_DriverClose@4" _
(ByVal CardNr As Byte) _
As Long

Declare Function PCI_IntMaskRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IntMaskRead@12" _
(ByVal CardNr As Byte, _
 ByRef data As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IntMaskWrite Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IntMaskWrite@12" _
(ByVal CardNr As Byte, _
 ByVal data As Long, _
 ByRef ErrorStatus As Long) _
As Long

Rem Declare Function PCI_IntEnable   Lib "PCIMilTreiberVB.dll" _
Rem Alias "_PCI_IntEnable@20" (Byte CardNr, MYDRIVER_INT_HANDLER funcIntHandler, void *pUserDat, WORD Priority, DWORD &ErrorStatus)As Long

Declare Function PCI_IntDisable Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IntDisable@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IntWait Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IntWait@12" _
(ByVal CardNr As Byte, _
 ByRef IntAktiv As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IntAktiv Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IntAktiv@12" _
(ByVal CardNr As Byte, _
 ByRef IntAktiv As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IntNextEnable Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_IntNextEnable@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_CntrlRegSet Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_CntrlRegSet@12" _
(ByVal CardNr As Byte, _
 ByVal SetValue As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_CntrlRegRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_CntrlRegRead@12" _
(ByVal CardNr As Byte, _
 ByRef CntrlRegValue As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFilterSet Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_EvFilterSet@16" _
    (ByVal CardNr As Byte, _
    ByVal EventCodeNr As Integer, _
    ByVal SetEv As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFiFoRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_EvFiFoRead@16" _
    (ByVal CardNr As Byte, _
     ByRef EventCodeNr As Integer, _
     ByRef EvFifo_Not_Empty As Long, _
     ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFilterRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_EvFilterRead@16" _
(ByVal CardNr As Byte, _
 ByVal EventCodeNr As Integer, _
 ByVal FilterValue As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_StatusTest Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_StatusTest@12" _
(ByVal CardNr As Byte, _
 ByVal StatusBit As Long, _
 ByRef ErrorStatus As Long) _
As Boolean

Declare Function PCI_MilBusRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_MilBusRead@16" _
(ByVal CardNr As Byte, _
 ByRef data As Integer, _
 ByRef Fifo_not_empty As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvTimerRead Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_EvTimerRead@12" _
(ByVal CardNr As Byte, _
 ByRef data As Integer, _
 ByRef Fifo_not_empty As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_PulsOutSet Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_PulsOutSet@28" _
(ByVal CardNr As Byte, _
 ByVal PulsBuchse As Integer, _
 ByVal EventOn As Integer, _
 ByVal EventOff As Integer, _
 ByVal PulsRahmen As Integer, _
 ByVal OnOff As Long, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls1Start Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls1Start@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls1Stop Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls1Stop@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls2Start Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls2Start@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls2Stop Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls2Stop@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls3Start Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls3Start@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls3Stop Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls3Stop@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls4Start Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls4Start@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls4Stop Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls4Stop@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls5Start Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls5Start@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls5Stop Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_SoftPuls5Stop@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFilterClear Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_EvFilterClear@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFiFioClear Lib "PCIMilTreiberVB.dll" _
Alias "_PCI_EvFiFioClear@8" _
(ByVal CardNr As Byte, _
 ByRef ErrorStatus As Long) _
As Long



