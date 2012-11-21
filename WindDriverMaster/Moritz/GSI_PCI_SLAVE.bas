Attribute VB_Name = "MILBUS"
Option Explicit

Global Const StatusOK = 0
Global Const CountCards_Error = 4
Global Const Open_Failure = 8
Global status As Long
Global rstatus As Long
Dim a As Integer
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

Declare Function PCI_PCIcardCount Lib "PCIMilTreiberVBS.dll" _
Alias "_PCI_PCIcardCount@0" () As Long

Declare Function PCI_DriverOpen Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_DriverOpen@4" _
    (ByVal CardNr As Byte) _
As Long

Declare Function PCI_DriverClose Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_DriverClose@4" _
    (ByVal CardNr As Byte) _
As Long

Declare Function PCI_PCIcardReset Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_PCIcardReset@8" _
    (ByVal CardNr As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_IntDisable Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_IntDisable@8 " _
    (ByVal CardNr As Byte, _
     ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_MilBusRead Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_MilBusRead@16" _
    (ByVal CardNr As Byte, _
    ByRef data As Integer, _
    ByRef Fifo_not_empty As Long, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_MilBusWrite Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_MilBusWrite@12" _
    (ByVal CardNr As Byte, _
     ByVal data As Integer, _
     ByRef ErrorStatus As Long) _
As Long



Declare Function PCI_PulsOutSet Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_PulsOutSet@28" _
    (ByVal CardNr As Byte, _
     ByVal PulsBuchse As Integer, _
     ByVal EventOn As Integer, _
     ByVal EventOff As Integer, _
     ByVal PulsRahmen As Integer, _
     ByVal OnOff As Long, _
     ByRef ErrorStatus As Long) _
As Long


Declare Function PCI_SoftPuls1Start Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_SoftPuls1Start@8" _
    (ByVal CardNr As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls1Stop Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_SoftPuls1Stop@8" _
    (ByVal CardNr As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls2Start Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_SoftPuls2Start@8" _
    (ByVal CardNr As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_SoftPuls2Stop Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_SoftPuls2Stop@8" _
    (ByVal CardNr As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_CntrlRegSet Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_CntrlRegSet@12" _
    (ByVal CardNr As Byte, _
    ByVal SetValue As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_CntrlRegRead Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_CntrlRegRead@12" _
    (ByVal CardNr As Byte, _
    ByRef CntrlRegValue As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFilterClear Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_EvFilterClear@8" _
    (ByVal CardNr As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFilterSet Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_EvFilterSet@16" _
    (ByVal CardNr As Byte, _
    ByVal EventCodeNr As Integer, _
    ByVal SetEv As Byte, _
    ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFiFioClear Lib "PCIMilTreiberS.dll" _
    Alias "_PCI_EvFiFioClear@8" _
    (ByVal CardNr As Byte, _
     ByRef ErrorStatus As Long) _
As Long

Declare Function PCI_EvFiFoRead Lib "D:\MILBUS\PCIMilTreiberS.dll" _
    Alias "_PCI_EvFiFoRead@16" _
    (ByVal CardNr As Byte, _
     ByRef EventCodeNr As Integer, _
     ByRef EvFifo_Not_Empty As Long, _
     ByRef ErrorStatus As Long) _
As Long

