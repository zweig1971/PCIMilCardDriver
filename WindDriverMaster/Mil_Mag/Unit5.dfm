object Form5: TForm5
  Left = 860
  Top = 345
  Width = 314
  Height = 508
  Caption = 'Bedeutung der IFK-Status Bits'
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
  object GroupBox3: TGroupBox
    Left = 5
    Top = 8
    Width = 300
    Height = 393
    Caption = 'Bedeutung der IFK- Status Bits'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label11: TLabel
      Left = 8
      Top = 40
      Width = 243
      Height = 24
      Caption = 'Bit 15 : Intrlck Mask 1- Enable'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -18
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label12: TLabel
      Left = 8
      Top = 80
      Width = 279
      Height = 23
      Caption = 'Bit 14 : Dta Rdy Mask 1-Enable'
    end
    object Label13: TLabel
      Left = 8
      Top = 120
      Width = 280
      Height = 23
      Caption = 'Bit 13 : Dta Req Mask 1-Enable'
    end
    object Label14: TLabel
      Left = 8
      Top = 160
      Width = 241
      Height = 23
      Caption = 'Bit 12 : Intelck- Sign 0-aktiv'
    end
    object Label15: TLabel
      Left = 8
      Top = 200
      Width = 249
      Height = 23
      Caption = 'Bit 11 : Dta Rdy Sign 0-aktiv'
    end
    object Label16: TLabel
      Left = 8
      Top = 240
      Width = 250
      Height = 23
      Caption = 'Bit 10 : Dta Req Sign 0-aktiv'
    end
    object Label17: TLabel
      Left = 8
      Top = 280
      Width = 267
      Height = 23
      Caption = 'Bit 09 : F-Gen SW3 To 0-aktiv'
    end
    object Label18: TLabel
      Left = 8
      Top = 320
      Width = 240
      Height = 23
      Caption = 'Bit 08 : Powerup Bit 0-aktiv'
    end
  end
  object Button1: TButton
    Left = 88
    Top = 416
    Width = 145
    Height = 49
    Caption = 'CLOSE'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button1Click
  end
end
