VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5412
   ClientLeft      =   132
   ClientTop       =   732
   ClientWidth     =   7248
   LinkTopic       =   "Form1"
   ScaleHeight     =   5412
   ScaleWidth      =   7248
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text4 
      Height          =   288
      Left            =   3840
      TabIndex        =   30
      Text            =   "02"
      Top             =   2880
      Width           =   252
   End
   Begin VB.TextBox Text3 
      Height          =   288
      Left            =   3240
      TabIndex        =   29
      Text            =   "00"
      Top             =   2880
      Width           =   252
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Stop Pulse "
      Height          =   252
      Left            =   1920
      TabIndex        =   28
      Top             =   2880
      Width           =   1212
   End
   Begin VB.TextBox Text2 
      Height          =   288
      Left            =   3240
      TabIndex        =   25
      Text            =   "02"
      Top             =   2520
      Width           =   252
   End
   Begin VB.TextBox Text1 
      Height          =   288
      Left            =   3840
      TabIndex        =   18
      Text            =   "01"
      Top             =   2520
      Width           =   252
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Aktivate Puls1/2"
      Height          =   252
      Left            =   1680
      TabIndex        =   17
      Top             =   2520
      Width           =   1452
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Puls1-Blib"
      Height          =   372
      Left            =   240
      TabIndex        =   14
      Top             =   2520
      Width           =   1212
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Close MILBUS"
      Height          =   372
      Left            =   3120
      TabIndex        =   4
      Top             =   480
      Width           =   1332
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Read MILBUS"
      Height          =   372
      Left            =   1680
      TabIndex        =   3
      Top             =   480
      Width           =   1212
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Open MILBUS"
      Height          =   372
      Left            =   240
      TabIndex        =   0
      Top             =   480
      Width           =   1212
   End
   Begin VB.Label Label22 
      BackColor       =   &H80000011&
      Height          =   252
      Left            =   4320
      TabIndex        =   31
      Top             =   2880
      Width           =   732
   End
   Begin VB.Label Label21 
      Caption         =   "ACC"
      Height          =   252
      Left            =   3240
      TabIndex        =   27
      Top             =   2160
      Width           =   372
   End
   Begin VB.Label Label20 
      Caption         =   "Event"
      Height          =   252
      Left            =   3720
      TabIndex        =   26
      Top             =   2160
      Width           =   492
   End
   Begin VB.Label Label19 
      Caption         =   "Error:"
      Height          =   252
      Left            =   240
      TabIndex        =   24
      Top             =   3480
      Width           =   732
   End
   Begin VB.Label Label18 
      Caption         =   "Status:"
      Height          =   252
      Left            =   240
      TabIndex        =   23
      Top             =   3120
      Width           =   732
   End
   Begin VB.Label Label17 
      Caption         =   "EV_da:"
      Height          =   252
      Left            =   3360
      TabIndex        =   22
      Top             =   4080
      Width           =   492
   End
   Begin VB.Label Label16 
      Caption         =   "Loop Err:"
      Height          =   252
      Left            =   3360
      TabIndex        =   21
      Top             =   3720
      Width           =   732
   End
   Begin VB.Label Label15 
      Caption         =   "Label15"
      Height          =   252
      Left            =   4200
      TabIndex        =   20
      Top             =   3720
      Width           =   972
   End
   Begin VB.Label Label14 
      BackColor       =   &H80000011&
      Height          =   252
      Left            =   4320
      TabIndex        =   19
      Top             =   2520
      Width           =   732
   End
   Begin VB.Label Label13 
      Caption         =   "Label13"
      Height          =   252
      Left            =   960
      TabIndex        =   16
      Top             =   3480
      Width           =   612
   End
   Begin VB.Label Label12 
      Caption         =   "Label12"
      Height          =   252
      Left            =   960
      TabIndex        =   15
      Top             =   3120
      Width           =   612
   End
   Begin VB.Label Label11 
      BackColor       =   &H00FFFFC0&
      Height          =   252
      Left            =   2280
      TabIndex        =   13
      Top             =   1560
      Width           =   612
   End
   Begin VB.Label Label10 
      Caption         =   "FC"
      Height          =   252
      Left            =   1680
      TabIndex        =   12
      Top             =   1320
      Width           =   492
   End
   Begin VB.Label Label9 
      Caption         =   "Count"
      Height          =   252
      Left            =   2280
      TabIndex        =   11
      Top             =   1920
      Width           =   612
   End
   Begin VB.Label Label8 
      Height          =   252
      Left            =   3720
      TabIndex        =   10
      Top             =   960
      Width           =   732
   End
   Begin VB.Label Label7 
      Caption         =   "Status:"
      Height          =   252
      Left            =   3120
      TabIndex        =   9
      Top             =   960
      Width           =   492
   End
   Begin VB.Label Label6 
      Caption         =   "Data:"
      Height          =   252
      Left            =   1680
      TabIndex        =   8
      Top             =   1560
      Width           =   492
   End
   Begin VB.Label Label5 
      Height          =   252
      Left            =   2160
      TabIndex        =   7
      Top             =   960
      Width           =   732
   End
   Begin VB.Label Label4 
      BackColor       =   &H80000009&
      Height          =   252
      Left            =   2280
      TabIndex        =   6
      Top             =   1320
      Width           =   612
   End
   Begin VB.Label Label3 
      BackColor       =   &H8000000A&
      Caption         =   "Status:"
      Height          =   252
      Left            =   1680
      TabIndex        =   5
      Top             =   960
      Width           =   492
   End
   Begin VB.Label Label2 
      Caption         =   "Status"
      Height          =   252
      Left            =   240
      TabIndex        =   2
      Top             =   960
      Width           =   1212
   End
   Begin VB.Label Label1 
      Caption         =   "Cardnumber"
      Height          =   252
      Left            =   240
      TabIndex        =   1
      Top             =   1320
      Width           =   1212
   End
   Begin VB.Menu Menu 
      Caption         =   "Program"
      Begin VB.Menu Cancel 
         Caption         =   "Cancel"
      End
      Begin VB.Menu quit 
         Caption         =   "End"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public nummer As Long
Public err As Long
Public datacount As Long
Public fifo_flag As Long

Public mildata As Integer
Public fc As Integer

Public cv As Byte
Public event_check_flag As Boolean







Private Sub Command1_Click()

nummer = 0
status = 0
err = 0
cv = 0

nummer = PCI_PCIcardCount
Label1.Caption = nummer

cv = nummer
status = PCI_DriverOpen(cv)
Label2.Caption = status

Rem status = PCI_PCIcardReset(cv, err)
Rem Label2.Caption = status

Rem status = PCI_IntDisable(cv, err)
Rem Label2.Caption = status


End Sub

Private Sub Command2_Click()
  fifo_flag = False
  mildata = 0
  datacount = 0
  Label4.Caption = ""
  Label5.Caption = ""
  Label9.Caption = " "
  Do
    status = PCI_MilBusRead(cv, mildata, fifo_flag, err)
    DoEvents
    Select Case status
    Case 0
      Label5.Caption = status
      datacount = datacount + 1
      Label9.Caption = datacount
      If datacount Mod 2 = 0 Then
      Label4.Caption = Hex(mildata)
      Else
      Label11.Caption = Hex(mildata)
      End If
    Case Else
      Label5.Caption = "waiting"
    End Select
  Loop Until datacount > 100000
  Label9.Caption = Str$(datacount) + " ok"
End Sub

Private Sub Command3_Click()
  status = PCI_DriverClose(cv)
  Label8.Caption = status
End Sub

Private Sub Command4_Click()
status = PCI_SoftPuls1Start(cv, err)
Label12.Caption = status
Label13.Caption = err
End Sub

Private Sub Command5_Click()
Dim ev_da As Long
Dim EVFifoNotEmpty As Long
Dim OnOff As Long
Dim Eventnr As Integer
Dim e1 As Integer
Dim e2 As Integer
Dim event_rd As Integer

event_check_flag = False

Label14.BackColor = RGB(100, 100, 100)
Label14.Caption = " "
Label22.BackColor = RGB(100, 100, 100)
Label22.Caption = " "

e1 = Val(Text1.Text) + 256 * Val(Text2.Text)
e2 = Val(Text4.Text) + 256 * Val(Text3.Text)

OnOff = True
Eventnr = Val(Text1.Text)
  status = PCI_PCIcardReset(cv, err)
  status = PCI_CntrlRegSet(cv, Cntrl_EvFilter_On, err)
  status = PCI_EvFilterClear(cv, err)
  status = PCI_EvFiFioClear(cv, err)
  status = PCI_EvFilterSet(cv, e1, Puls1_On, err)
  Rem status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  Rem status = PCI_EvFilterSet(cv, e2, Puls2_On, err)
  Rem status = PCI_PulsOutSet(cv, PulsBuchse_1, e1, e2, Puls, OnOff, err)
  Rem status = PCI_PulsOutSet(cv, PulsBuchse_2, e1, e2, Puls, OnOff, err)
  event_check_flag = True
  Label13.Caption = Hex(err)
  
  status = PCI_EvFilterSet(cv, e1, EventOn, err)
  Rem status = PCI_EvFilterSet(cv, e2, EventOn, err)
  Do
    DoEvents
    
    status = PCI_EvFiFoRead(cv, event_rd, EVFifoNotEmpty, err)
    
    Select Case event_rd
    Case Is = e1
      status = PCI_SoftPuls2Start(cv, err)
      Rem  status = PCI_SoftPuls2Stop(cv, err)
      Rem Label14.BackColor = RGB(255, 0, 0)
      Rem Label14.Caption = "Roger!"
      Rem Label22.BackColor = RGB(100, 100, 100)
      Rem Label22.Caption = " "
    Case Is = e2
      Label22.BackColor = RGB(255, 0, 0)
      Label22.Caption = "Roger!"
      Label14.BackColor = RGB(100, 100, 100)
      Label14.Caption = " "
    Case Else
      Label14.BackColor = RGB(100, 100, 100)
      Label14.Caption = " "
      Label22.BackColor = RGB(100, 100, 100)
      Label22.Caption = " "
    End Select
    event_rd = 0
    Rem status = PCI_EvFiFioClear(cv, err)
    Rem Label15.Caption = event_rd
  Loop Until event_check_flag = False
  
End Sub

Private Sub Command6_Click()
event_check_flag = False
End Sub

Private Sub Form_Load()
datacount = 0
End Sub


Private Sub quit_Click()
  status = PCI_DriverClose(cv)
  Unload Form1
  End
End Sub
