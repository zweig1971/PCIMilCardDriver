VERSION 5.00
Begin VB.Form Event_Timing 
   BackColor       =   &H8000000A&
   BorderStyle     =   0  'None
   Caption         =   "PCI_MIL EventMonitor/Decoder"
   ClientHeight    =   8745
   ClientLeft      =   5040
   ClientTop       =   675
   ClientWidth     =   5745
   FillColor       =   &H00E0E0E0&
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   8745
   ScaleWidth      =   5745
   ShowInTaskbar   =   0   'False
   Visible         =   0   'False
   Begin VB.CommandButton Cmd_pz 
      Caption         =   "Run/Stop"
      Height          =   255
      Left            =   120
      Style           =   1  'Graphical
      TabIndex        =   41
      Top             =   1800
      Width           =   1215
   End
   Begin VB.ListBox List_Acc 
      BackColor       =   &H0080FFFF&
      Columns         =   10
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1031
         SubFormatType   =   1
      EndProperty
      DragMode        =   1  'Automatic
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   240
      TabIndex        =   38
      Top             =   8040
      Width           =   4455
   End
   Begin VB.TextBox Text6 
      Alignment       =   2  'Center
      BackColor       =   &H8000000A&
      BorderStyle     =   0  'None
      Height          =   285
      Left            =   5280
      TabIndex        =   34
      Text            =   "1"
      Top             =   6480
      Width           =   255
   End
   Begin VB.CommandButton Cmd_Beep3 
      Caption         =   "Beep3"
      Height          =   375
      Left            =   3240
      Style           =   1  'Graphical
      TabIndex        =   28
      Top             =   360
      Width           =   615
   End
   Begin VB.CommandButton Cmd_Beep2 
      Caption         =   "Beep2"
      Height          =   375
      Left            =   2400
      Style           =   1  'Graphical
      TabIndex        =   27
      Top             =   360
      Width           =   615
   End
   Begin VB.VScrollBar VScroll1 
      Height          =   255
      Left            =   4680
      Max             =   10
      Min             =   1000
      SmallChange     =   10
      TabIndex        =   25
      Top             =   4920
      Value           =   50
      Width           =   255
   End
   Begin VB.TextBox Text5 
      Alignment       =   1  'Right Justify
      Height          =   285
      Left            =   4920
      TabIndex        =   24
      Text            =   "50"
      Top             =   4920
      Width           =   495
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   4080
      Top             =   4800
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Stop with Acc"
      Height          =   375
      Left            =   120
      TabIndex        =   22
      Top             =   1320
      Width           =   1215
   End
   Begin VB.CommandButton Cmd_Beep1 
      Appearance      =   0  'Flat
      Caption         =   "Beep1"
      Height          =   375
      Left            =   1560
      MaskColor       =   &H80000004&
      Style           =   1  'Graphical
      TabIndex        =   21
      Top             =   360
      Width           =   615
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Dec Pulse2"
      Height          =   252
      Left            =   4320
      Style           =   1  'Graphical
      TabIndex        =   20
      Top             =   2520
      Width           =   1095
   End
   Begin VB.ComboBox Combo_acc3 
      Height          =   315
      IntegralHeight  =   0   'False
      ItemData        =   "Event_Timing.frx":0000
      Left            =   3240
      List            =   "Event_Timing.frx":0002
      TabIndex        =   18
      Text            =   "6"
      Top             =   840
      Width           =   612
   End
   Begin VB.ComboBox Combo_acc2 
      Height          =   315
      Left            =   2400
      TabIndex        =   17
      Text            =   "14"
      Top             =   840
      Width           =   612
   End
   Begin VB.ComboBox combo_acc1 
      BackColor       =   &H00FFFFFF&
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1031
         SubFormatType   =   1
      EndProperty
      Height          =   315
      ItemData        =   "Event_Timing.frx":0004
      Left            =   1560
      List            =   "Event_Timing.frx":0006
      TabIndex        =   16
      Text            =   "10"
      Top             =   840
      Width           =   612
   End
   Begin VB.ListBox List_Mon 
      BackColor       =   &H00FFC0C0&
      Height          =   5130
      Left            =   240
      TabIndex        =   14
      Top             =   2760
      Width           =   975
   End
   Begin VB.ListBox List3 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1031
         SubFormatType   =   1
      EndProperty
      Height          =   6690
      Left            =   3240
      TabIndex        =   13
      Top             =   1200
      Width           =   612
   End
   Begin VB.ListBox List2 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1031
         SubFormatType   =   1
      EndProperty
      Height          =   6690
      Left            =   2400
      TabIndex        =   11
      Top             =   1200
      Width           =   612
   End
   Begin VB.ListBox List1 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1031
         SubFormatType   =   1
      EndProperty
      Height          =   6690
      ItemData        =   "Event_Timing.frx":0008
      Left            =   1560
      List            =   "Event_Timing.frx":000A
      TabIndex        =   1
      Top             =   1200
      Width           =   612
   End
   Begin VB.TextBox Text4 
      Height          =   288
      Left            =   4320
      TabIndex        =   12
      Text            =   "02"
      Top             =   3000
      Width           =   372
   End
   Begin VB.TextBox Text3 
      Alignment       =   1  'Right Justify
      Height          =   288
      Left            =   5040
      TabIndex        =   10
      Text            =   "43"
      Top             =   3000
      Width           =   372
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Stop Decoder"
      Height          =   375
      Left            =   120
      Style           =   1  'Graphical
      TabIndex        =   9
      Top             =   840
      Width           =   1215
   End
   Begin VB.TextBox Text2 
      Alignment       =   1  'Right Justify
      Height          =   288
      Left            =   4320
      TabIndex        =   6
      Text            =   "08"
      Top             =   1680
      Width           =   375
   End
   Begin VB.TextBox Text1 
      Alignment       =   1  'Right Justify
      Height          =   288
      Left            =   5040
      TabIndex        =   5
      Text            =   "40"
      Top             =   1680
      Width           =   372
   End
   Begin VB.CommandButton Timing_Control 
      Appearance      =   0  'Flat
      Caption         =   "Start Decoder"
      Default         =   -1  'True
      Height          =   375
      Left            =   120
      TabIndex        =   4
      Top             =   360
      Width           =   1215
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Dec Pulse1"
      Height          =   252
      Left            =   4320
      Style           =   1  'Graphical
      TabIndex        =   3
      Top             =   1200
      Width           =   1095
   End
   Begin VB.Label Lbl_pz 
      Alignment       =   2  'Center
      Caption         =   "SIS"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   18
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3960
      TabIndex        =   40
      Top             =   360
      Width           =   1695
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Right Justify
      Caption         =   "VrtAcc"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   4920
      TabIndex        =   39
      Top             =   7560
      Width           =   735
   End
   Begin VB.Line Line3 
      X1              =   4680
      X2              =   4800
      Y1              =   8160
      Y2              =   8220
   End
   Begin VB.Line Line2 
      X1              =   4680
      X2              =   4800
      Y1              =   8160
      Y2              =   8100
   End
   Begin VB.Line Line1 
      X1              =   5040
      X2              =   4680
      Y1              =   8160
      Y2              =   8160
   End
   Begin VB.Label Label14 
      Alignment       =   2  'Center
      BackColor       =   &H00E0E0E0&
      Caption         =   "SuperCycleMonitor On/Off"
      Height          =   255
      Left            =   1560
      TabIndex        =   37
      Top             =   8400
      Width           =   2295
   End
   Begin VB.Label Label12 
      Alignment       =   2  'Center
      BackColor       =   &H0080FFFF&
      Caption         =   "-"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   5040
      TabIndex        =   36
      Top             =   7920
      Width           =   615
   End
   Begin VB.Label Label11 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "GSI EventMon2 by P. Moritz /M. Zweig (click to close)"
      ForeColor       =   &H80000008&
      Height          =   735
      Left            =   4200
      TabIndex        =   35
      Top             =   5400
      Visible         =   0   'False
      Width           =   1455
   End
   Begin VB.Label Label10 
      Alignment       =   1  'Right Justify
      Caption         =   "Cards in PC"
      Height          =   255
      Left            =   4320
      TabIndex        =   33
      Top             =   6240
      Width           =   855
   End
   Begin VB.Label Label9 
      Alignment       =   1  'Right Justify
      BackColor       =   &H00FFC0C0&
      Caption         =   "Event"
      Height          =   255
      Left            =   720
      TabIndex        =   32
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label4 
      BackColor       =   &H00FFC0C0&
      Caption         =   "Acc"
      Height          =   255
      Left            =   240
      TabIndex        =   31
      Top             =   2400
      Width           =   375
   End
   Begin VB.Label Label7 
      Alignment       =   1  'Right Justify
      Caption         =   "Event"
      Height          =   255
      Left            =   4920
      TabIndex        =   30
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label6 
      Caption         =   "Acc"
      Height          =   255
      Left            =   4320
      TabIndex        =   29
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label3 
      Caption         =   "Event Display Interval [ms]"
      Height          =   495
      Left            =   4440
      TabIndex        =   26
      Top             =   4440
      Width           =   975
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "Card in use "
      Height          =   255
      Left            =   4320
      TabIndex        =   23
      Top             =   6480
      Width           =   855
   End
   Begin VB.Label Label13 
      Alignment       =   2  'Center
      Caption         =   "------ Virtual Accelerator --------"
      Height          =   255
      Left            =   1560
      TabIndex        =   19
      Top             =   120
      Width           =   2175
   End
   Begin VB.Label Label15 
      Alignment       =   2  'Center
      BackColor       =   &H00FFC0C0&
      Caption         =   "Eventmonitor"
      Height          =   255
      Left            =   240
      TabIndex        =   15
      Top             =   2160
      Width           =   975
   End
   Begin VB.Label Label21 
      Caption         =   "Acc"
      Height          =   255
      Left            =   4320
      TabIndex        =   8
      Top             =   1440
      Width           =   375
   End
   Begin VB.Label Label20 
      Alignment       =   1  'Right Justify
      Caption         =   "Event"
      Height          =   255
      Left            =   4920
      TabIndex        =   7
      Top             =   1440
      Width           =   495
   End
   Begin VB.Label Label8 
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   2160
      Width           =   735
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackColor       =   &H8000000A&
      Height          =   255
      Left            =   5280
      TabIndex        =   0
      Top             =   6240
      Width           =   255
   End
   Begin VB.Menu Menu 
      Caption         =   "Program"
      Begin VB.Menu Cancel 
         Caption         =   "Cancel"
      End
      Begin VB.Menu ACQ 
         Caption         =   "Quit"
      End
   End
   Begin VB.Menu Machine 
      Caption         =   "PZ"
      Begin VB.Menu PZ_UNI 
         Caption         =   "UNILAC"
      End
      Begin VB.Menu PZ_SIS 
         Caption         =   "SIS"
         Checked         =   -1  'True
      End
      Begin VB.Menu PZ_ESR 
         Caption         =   "ESR"
      End
   End
   Begin VB.Menu Help 
      Caption         =   "Help"
      Begin VB.Menu C1 
         Caption         =   "Click on DecPulse1/2 to test pulse output 1/2"
      End
      Begin VB.Menu C2 
         Caption         =   "To delegate DecPulse1 click into leftmost VirtAcc column or use ACC and Event field 1"
      End
      Begin VB.Menu C3 
         Caption         =   "To delegate DecPulse2 click into center Virtacc column or use ACC and Event field 2"
      End
      Begin VB.Menu About 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "Event_Timing"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public FoundCards As Long
Public err As Long
Public datacount As Long
Public fifo_flag As Long
Public EVFifoNotEmpty As Long

Public mildata As Integer
Public rows As Integer
  
Public cv As Byte
Public acc1, acc2, acc3 As Byte
Public start_cycle, stop_cycle As Byte
Public trig_event As Byte

Public event_check_flag As Boolean
Public stop_synch As Boolean
Public mil_status As Boolean
Public beep1_on, beep2_on, beep3_on As Boolean
Public vrtacc_stop As Boolean

Dim OnOff As Long
Dim Eventnr As Integer
Dim e1, e1_old As Integer
Dim e2, e2_old As Integer
Dim event_rd As Integer

Dim event_nr As Byte
Public virtacc As Byte
Dim virtacc_old As Byte
Dim rest As Byte
Dim tif$

Private Sub About_Click()
  Label11.Visible = True
End Sub

Private Sub ACQ_Click()
  End
End Sub

Private Sub Cmd_Beep1_Click()
  beep1_on = Not beep1_on
  If beep1_on = True Then
    Cmd_Beep1.BackColor = vbRed
  Else
    Cmd_Beep1.BackColor = vbWhite
  End If
End Sub

Private Sub Cmd_Beep2_Click()
  beep2_on = Not beep2_on
  If beep2_on = True Then
    Cmd_Beep2.BackColor = vbRed
  Else
    Cmd_Beep2.BackColor = vbWhite
  End If
End Sub

Private Sub Cmd_Beep3_Click()
  beep3_on = Not beep3_on
  If beep3_on = True Then
    Cmd_Beep3.BackColor = vbRed
  Else
    Cmd_Beep3.BackColor = vbWhite
  End If
End Sub

Private Sub Cmd_Scycle_Click()
  List_Acc.Visible = False
End Sub

Private Sub combo_acc1_Click()
  acc1 = Val(combo_acc1.Text)
  List1.Clear
End Sub

Private Sub Combo_acc2_Click()
  acc2 = Val(Combo_acc2.Text)
  List2.Clear
End Sub

Private Sub Combo_acc3_Click()
  acc3 = Val(Combo_acc3.Text)
  List3.Clear
End Sub

Private Sub Command3_Click()
  mil_status = PCI_DriverClose(cv)
  Label8.Caption = mil_status
End Sub

Private Sub Command1_Click()

End Sub

Private Sub Command4_Click()
  mil_status = PCI_SoftPuls1Start(cv, err)
End Sub

Private Sub Timimg_Control_click()
  
End Sub

Private Sub Command6_Click()
  event_check_flag = False
  Timer1.Enabled = False
  Cmd_pz.BackColor = vbRed
End Sub

Private Sub Command7_Click()
  mil_status = PCI_SoftPuls2Start(cv, err)
End Sub

Private Sub Command8_Click()
  stop_synch = True
End Sub

Private Sub Form_Load()
  Dim Msg, Style, Title, Help, Ctxt, Response, MyString
  Dim i As Integer
  Dim FifoNotEmpty, regwert As Long
  
  Dim fifo_flag As Boolean
  vrtacc_stop = False
  FifoNotEmpty = &H8000
  event_check_flag = True
  start_cycle = 32
  stop_cycle = 55
  datacount = 0
  FoundCards = 0
  mil_status = 0
  err = 0
  cv = 0
  stop_synch = False
  List_Acc.Clear
  For i = 0 To 9
    List_Acc.AddItem " "
  Next i
  FoundCards = PCI_PCIcardCount
  Label1.Caption = FoundCards
  Select Case FoundCards
    Case Is = 1
      cv = FoundCards
    Case Is = 2
      Msg = "Use Card #2? = Yes ,  Use Card #1 = No[Default]"                                     ' Define message.
      Style = vbYesNo + vbDefaultButton2                       ' Define buttons.
      Title = "Select Card 1 or 2"                             ' Define title.
      Response = MsgBox(Msg, Style, Title)
      If Response = vbYes Then                                 ' User chose Yes.
        cv = 2
      Else                                                     ' User chose No.
        cv = 1
      End If
    Case Else
      Msg = "No Card found, continue?"
      Style = vbYesNo + vbCritical + vbDefaultButton2
      Title = "No Card found"   ' Define title.
      Response = MsgBox(Msg, Style, Title)
      If Response = vbYes Then
        cv = 2
      Else
        cv = 1
      End If
      cv = 0
  End Select
  mil_status = PCI_DriverOpen(cv)
  acc1 = combo_acc1.Text
  Text2.Text = combo_acc1.Text
  acc2 = Combo_acc2.Text
  Text4.Text = Combo_acc2.Text
  acc3 = Combo_acc3.Text
  For i = 0 To 15
    combo_acc1.AddItem i
    Combo_acc2.AddItem i
    Combo_acc3.AddItem i
  Next i
  Event_Timing.Show
  trig_event = start_cycle
  OnOff = True
  Eventnr = Val(Text1.Text)
  mil_status = PCI_PCIcardReset(cv, err)
  mil_status = PCI_CntrlRegSet(cv, Cntrl_EvFilter_Off, err)
  mil_status = PCI_EvFiFioClear(cv, err)
  mil_status = PCI_CntrlRegSet(cv, Cntrl_EvFilter_12_Bit, err)
  e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
  e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
  mil_status = PCI_EvFilterClear(cv, err)
  mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
  mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  event_check_flag = True
  stop_synch = False

End Sub

Private Sub Form_Unload(Cancel As Integer)
  Event_Timing.Hide
  End
End Sub



Private Sub Label11_Click()
  Label11.Visible = False
End Sub

Private Sub Label12_Click()
  List_Acc.Visible = True
End Sub

Private Sub Label14_Click()
  List_Acc.Visible = Not List_Acc.Visible
  Line1.Visible = List_Acc.Visible
  Line2.Visible = Line1.Visible
  Line3.Visible = Line1.Visible
End Sub

Private Sub List_Acc_Click()
  List_Acc.Visible = False
End Sub

Private Sub List1_Click()
  Text2.Text = combo_acc1.Text
  Text1.Text = List1.Text
  e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
  e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
  mil_status = PCI_EvFilterClear(cv, err)
  mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
  mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
    
End Sub

Private Sub List2_Click()
  Text4.Text = Combo_acc2.Text
  Text3.Text = List2.Text
  e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
  e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
  mil_status = PCI_EvFilterClear(cv, err)
  mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
  mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)

End Sub



Private Sub PZ_ESR_Click()
  start_cycle = 32
  stop_cycle = 55
  PZ_UNI.Checked = False
  PZ_SIS.Checked = False
  PZ_ESR.Checked = True
  Lbl_pz.Caption = "ESR"
End Sub

Private Sub PZ_SIS_Click()
  start_cycle = 32
  stop_cycle = 55
  PZ_UNI.Checked = False
  PZ_SIS.Checked = True
  PZ_ESR.Checked = False
  Lbl_pz.Caption = "SIS"
End Sub

Private Sub PZ_UNI_Click()
  start_cycle = 16
  stop_cycle = 29
  PZ_UNI.Checked = True
  PZ_SIS.Checked = False
  PZ_ESR.Checked = False
  Lbl_pz.Caption = "UNILAC"
End Sub

Private Sub Text1_Change()
  If Val(Text1.Text) >= 0 Then
    e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
    e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
    mil_status = PCI_EvFilterClear(cv, err)
    mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
    mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  End If
End Sub

Private Sub Text2_Change()
  If Val(Text2.Text) >= 0 Then
    e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
    e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
    mil_status = PCI_EvFilterClear(cv, err)
    mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
    mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  End If
End Sub

Private Sub Text3_Change()
  If Val(Text3.Text) >= 0 Then
    e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
    e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
    mil_status = PCI_EvFilterClear(cv, err)
    mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
    mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  End If
End Sub

Private Sub Text4_Change()
  If Val(Text4.Text) >= 0 Then
    e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
    e2 = Val(Text3.Text) + 256 * Val(Text4.Text)
    mil_status = PCI_EvFilterClear(cv, err)
    mil_status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
    mil_status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  End If
End Sub

Private Sub Text5_Change()
  
  If Val(Text5.Text) >= 10 Then
    Timer1.Interval = Text5.Text
    VScroll1.Value = Val(Text5.Text)
  End If
  
End Sub

Private Sub Text6_Change()
  If Val(Text6.Text) >= 1 And Val(Text6.Text) <= FoundCards Then cv = Val(Text6.Text)
End Sub

Private Sub Timer1_Timer()
    Dim acc_rows As Integer
    Do
      mil_status = PCI_EvFiFoRead(cv, event_rd, EVFifoNotEmpty, err)
      If event_rd > 0 Then
        event_nr = event_rd And &HFF
        virtacc = (event_rd And &HF00) / 256
        rest = (event_rd And &HF000) / 4096
        Select Case event_nr
        Case 1 To 254
          tif$ = Format(virtacc, "0# ") + "       " + Format(event_nr, "00#")
          List_Mon.AddItem tif$
          If List_Mon.ListCount > 25 Then List_Mon.RemoveItem 0
        End Select
        Select Case event_nr
        Case 1 To 200
          If event_nr = start_cycle Then
            Label12.Caption = virtacc
          End If
          If event_nr = stop_cycle Then
            List_Acc.AddItem Str$(virtacc) + "-"
            List_Acc.RemoveItem 0
            Label12.Caption = "-"
          End If
          If List_Acc.ListCount > 5 Then
           Rem  List_Acc.RemoveItem 0
           Rem  List_Acc.RemoveItem 0
          End If
          Select Case virtacc
          Case Is = acc1
            If event_nr = start_cycle Then
            List1.Clear
            combo_acc1.BackColor = vbRed
            If beep1_on Then Beep
            End If
            If event_nr = stop_cycle Then
              combo_acc1.BackColor = vbWhite
              vrtacc_stop = True
              If stop_synch = True Then
                event_check_flag = False
                Timer1.Enabled = False
              End If
            End If
            List1.AddItem event_nr
          Case Is = acc2
            If event_nr = start_cycle Then
              List2.Clear
              Combo_acc2.BackColor = vbRed
            End If
            If event_nr = stop_cycle Then
              Combo_acc2.BackColor = vbWhite
              vrtacc_stop = True
              If stop_synch = True Then
                event_check_flag = False
                Timer1.Enabled = False
              End If
            End If
            List2.AddItem event_nr
          Case Is = acc3
            If event_nr = start_cycle Then
              List3.Clear
              Combo_acc3.BackColor = vbRed
            End If
            If event_nr = stop_cycle Then
              Combo_acc3.BackColor = vbWhite
              vrtacc_stop = True
              If stop_synch = True Then
                event_check_flag = False
                Timer1.Enabled = False
              End If
            End If
            List3.AddItem event_nr
          End Select
        End Select
      End If
      event_rd = 0
    Loop Until EVFifoNotEmpty = 0
End Sub

Private Sub Timing_Control_Click()
  Dim i As Integer
  OnOff = True
  stop_synch = False
  event_check_flag = True
  List_Acc.Clear
  For i = 0 To 9
    List_Acc.AddItem " "
  Next i
  Call read_timing
  Cmd_pz.BackColor = vbGreen
  End Sub

Sub read_timing()
    Timer1.Interval = Text5.Text
    Timer1.Enabled = True
End Sub



Private Sub VScroll1_Change()
  Text5.Text = Val(VScroll1.Value)
  Timer1.Interval = Text5.Text
End Sub
