object Form2: TForm2
  Left = 485
  Top = 319
  Width = 480
  Height = 369
  Caption = 'Welche IFK ist am Bus  V.0.1'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 23
  object Label1: TLabel
    Left = 136
    Top = 32
    Width = 119
    Height = 23
    Caption = 'IFK'#39's online : '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton1: TSpeedButton
    Left = 128
    Top = 168
    Width = 121
    Height = 57
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'LOOP'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 16
    Width = 31
    Height = 21
    Caption = 'Hex'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object ListBox1: TListBox
    Left = 24
    Top = 32
    Width = 81
    Height = 297
    ItemHeight = 23
    TabOrder = 0
  end
  object Button3: TButton
    Left = 336
    Top = 272
    Width = 121
    Height = 57
    Caption = 'CLOSE'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button3Click
  end
  object Button1: TButton
    Left = 128
    Top = 272
    Width = 121
    Height = 57
    Caption = 'STEP'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = Button1Click
  end
  object Panel1: TPanel
    Left = 256
    Top = 32
    Width = 73
    Height = 33
    BevelInner = bvLowered
    Caption = '0'
    Color = clScrollBar
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 352
    Top = 32
  end
end
