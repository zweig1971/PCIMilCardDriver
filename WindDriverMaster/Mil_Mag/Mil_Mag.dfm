object Form1: TForm1
  Left = 323
  Top = 182
  Width = 681
  Height = 444
  Caption = 'Small Mil_Mag main menue  V.0.1'
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
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 673
    Height = 145
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 155
      Height = 23
      Caption = 'Aktive IFK  (Hex):'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 456
      Top = 24
      Width = 129
      Height = 23
      Caption = 'Found IFK'#39's  : '
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object UPDATE: TButton
      Left = 288
      Top = 88
      Width = 113
      Height = 41
      Caption = 'UPDATE'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = UPDATEClick
    end
    object Panel2: TPanel
      Left = 592
      Top = 24
      Width = 65
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
      TabOrder = 1
    end
    object ListBox1: TListBox
      Left = 168
      Top = 24
      Width = 73
      Height = 105
      ItemHeight = 23
      TabOrder = 2
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 152
    Width = 673
    Height = 185
    TabOrder = 1
    object Button2: TButton
      Left = 376
      Top = 32
      Width = 273
      Height = 57
      Caption = 'Welche IFK am Bus '
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 24
      Top = 32
      Width = 273
      Height = 57
      Caption = 'Sende Fct-Code zu IFK'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 24
      Top = 104
      Width = 273
      Height = 57
      Caption = 'Lese IFK Status'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = Button4Click
    end
    object Button5: TButton
      Left = 376
      Top = 104
      Width = 273
      Height = 57
      Caption = 'Sollwert-Daten Schreiben'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = Button5Click
    end
  end
  object Button1: TButton
    Left = 520
    Top = 352
    Width = 153
    Height = 65
    Caption = 'EXIT'
    Font.Charset = ANSI_CHARSET
    Font.Color = clRed
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = Button1Click
  end
end
