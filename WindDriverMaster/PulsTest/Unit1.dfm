object Form1: TForm1
  Left = 630
  Top = 182
  Width = 395
  Height = 294
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 25
  object SpeedButton1: TSpeedButton
    Left = 0
    Top = 192
    Width = 145
    Height = 65
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'LOOP'
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 385
    Height = 185
    BevelInner = bvLowered
    TabOrder = 0
    object Shape1: TShape
      Left = 24
      Top = 32
      Width = 25
      Height = 25
    end
    object Shape2: TShape
      Left = 24
      Top = 72
      Width = 25
      Height = 25
    end
    object Shape3: TShape
      Left = 24
      Top = 112
      Width = 25
      Height = 25
    end
    object Label1: TLabel
      Left = 56
      Top = 32
      Width = 101
      Height = 25
      Caption = 'Puls input 1'
    end
    object Label2: TLabel
      Left = 56
      Top = 72
      Width = 102
      Height = 25
      Caption = 'Puls Input 2'
    end
    object Label3: TLabel
      Left = 56
      Top = 112
      Width = 101
      Height = 25
      Caption = 'Puls input 3'
    end
  end
  object Button1: TButton
    Left = 240
    Top = 192
    Width = 145
    Height = 65
    Caption = 'EXIT'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 160
    Top = 200
  end
end
