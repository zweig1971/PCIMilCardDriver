VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5415
   ClientLeft      =   45
   ClientTop       =   345
   ClientWidth     =   7245
   LinkTopic       =   "Form1"
   ScaleHeight     =   5415
   ScaleWidth      =   7245
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Open MILBUS"
      Height          =   612
      Left            =   240
      TabIndex        =   0
      Top             =   480
      Width           =   1692
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   495
      Left            =   2640
      TabIndex        =   1
      Top             =   600
      Width           =   2655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim status As Long
Dim rete As Long


Private Sub Command1_Click()
rete = PCI_PCIcardCount()
Label1.Caption = rete
End Sub
