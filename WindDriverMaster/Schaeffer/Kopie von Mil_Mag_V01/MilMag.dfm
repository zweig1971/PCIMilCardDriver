object Form_MilMag: TForm_MilMag
  Left = 0
  Top = 0
  Width = 847
  Height = 589
  AutoSize = True
  Caption = 'Mil_Mag V.0.2'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormMilMagClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxIFKamBus: TGroupBox
    Left = 744
    Top = 283
    Width = 95
    Height = 216
    Caption = 'IFK am Bus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object ListBoxIFK: TListBox
      Left = 16
      Top = 24
      Width = 57
      Height = 137
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ItemHeight = 16
      ParentFont = False
      TabOrder = 0
    end
    object PanelAnzahlIFK: TPanel
      Left = 16
      Top = 176
      Width = 57
      Height = 21
      BevelInner = bvLowered
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
  end
  object GroupBoxIFKStatus: TGroupBox
    Left = 399
    Top = 38
    Width = 438
    Height = 242
    Caption = 'IFK Status lesen'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Panel3: TPanel
      Left = 8
      Top = 18
      Width = 417
      Height = 217
      BevelInner = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label6: TLabel
        Left = 7
        Top = 33
        Width = 95
        Height = 16
        Caption = 'Status C0 (bin) :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 7
        Top = 78
        Width = 95
        Height = 16
        Caption = 'Status C1 (bin) :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 7
        Top = 120
        Width = 95
        Height = 16
        Caption = 'Status C2 (bin) :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 104
        Top = 10
        Width = 38
        Height = 16
        Caption = 'Status'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label10: TLabel
        Left = 210
        Top = 10
        Width = 48
        Height = 16
        Caption = 'Adresse'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 105
        Top = 144
        Width = 29
        Height = 16
        Caption = 'MSB'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label12: TLabel
        Left = 210
        Top = 144
        Width = 25
        Height = 16
        Caption = 'LSB'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 308
        Top = 10
        Width = 27
        Height = 16
        Caption = 'Data'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object Label14: TLabel
        Left = 309
        Top = 144
        Width = 23
        Height = 16
        Caption = 'Hex'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object PanelStatC0MSB: TPanel
        Left = 103
        Top = 28
        Width = 85
        Height = 27
        BevelInner = bvLowered
        Caption = '0000 0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object PanelStatC1MSB: TPanel
        Left = 103
        Top = 73
        Width = 86
        Height = 27
        BevelInner = bvLowered
        Caption = '0000 0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object PanelStatC2MSB: TPanel
        Left = 103
        Top = 115
        Width = 86
        Height = 27
        BevelInner = bvLowered
        Caption = '0000 0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object PanelAdrC0LSB: TPanel
        Left = 207
        Top = 28
        Width = 85
        Height = 27
        BevelInner = bvLowered
        Caption = '0000 0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object PanelAdrC1LSB: TPanel
        Left = 207
        Top = 73
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '0000 0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object PanelAdrC2LSB: TPanel
        Left = 208
        Top = 115
        Width = 86
        Height = 28
        BevelInner = bvLowered
        Caption = '0000 0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object PanelDataC0: TPanel
        Left = 306
        Top = 28
        Width = 61
        Height = 27
        BevelInner = bvLowered
        Caption = '0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object PanelDataC1: TPanel
        Left = 307
        Top = 73
        Width = 60
        Height = 27
        BevelInner = bvLowered
        Caption = '0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object PanelDataC2: TPanel
        Left = 307
        Top = 115
        Width = 61
        Height = 27
        BevelInner = bvLowered
        Caption = '0000'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
    end
  end
  object GroupBoxSendFCTCode: TGroupBox
    Left = 3
    Top = 38
    Width = 394
    Height = 242
    Caption = 'Sende Fkt.-Code zur IFK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object PanelSendFCTC: TPanel
      Left = 224
      Top = 20
      Width = 161
      Height = 176
      BevelInner = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label4: TLabel
        Left = 28
        Top = 13
        Width = 103
        Height = 16
        Caption = 'Send Fkt. -Code :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object ButtonReset: TButton
        Left = 24
        Top = 40
        Width = 105
        Height = 33
        Caption = 'Reset [01h]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = ButtonResetClick
      end
      object ButtonEin: TButton
        Left = 25
        Top = 88
        Width = 104
        Height = 33
        Caption = 'Ein [02h]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = ButtonEinClick
      end
      object ButtonAus: TButton
        Left = 24
        Top = 134
        Width = 105
        Height = 35
        Caption = 'Aus [03h]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = ButtonAusClick
      end
    end
    object PanelSendFCTHex: TPanel
      Left = 8
      Top = 20
      Width = 217
      Height = 176
      BevelInner = bvLowered
      TabOrder = 1
      object Label3: TLabel
        Left = 8
        Top = 33
        Width = 132
        Height = 16
        Caption = 'Send Fkt.-Code (hex) :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object InputSendFCTCode: TEdit
        Left = 142
        Top = 31
        Width = 51
        Height = 24
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = '0'
      end
      object ButtonSendFctCode: TButton
        Left = 11
        Top = 140
        Width = 156
        Height = 25
        Caption = 'SEND'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = ButtonSendFctCodeClick
      end
    end
    object Panel2: TPanel
      Left = 8
      Top = 195
      Width = 377
      Height = 41
      BevelInner = bvLowered
      TabOrder = 2
      object Label5: TLabel
        Left = 24
        Top = 13
        Width = 128
        Height = 16
        Caption = 'Fkt. + Adresse (hex) :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object PanelFktAdress: TPanel
        Left = 167
        Top = 10
        Width = 106
        Height = 25
        BevelInner = bvLowered
        Caption = '0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
  end
  object GroupBoxSollDataWrite: TGroupBox
    Left = 0
    Top = 282
    Width = 741
    Height = 218
    Caption = 'Sollwert-Daten auf Bus Schreiben'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object Panel14: TPanel
      Left = 332
      Top = 24
      Width = 402
      Height = 184
      BevelInner = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object GroupBox3: TGroupBox
        Left = 8
        Top = 3
        Width = 192
        Height = 174
        Caption = 'IW1 [81h]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label17: TLabel
          Left = 8
          Top = 40
          Width = 64
          Height = 16
          Caption = 'Volt (hex) :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label18: TLabel
          Left = 8
          Top = 96
          Width = 64
          Height = 16
          Caption = 'Volt (dec) :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object PanelVoltHexIW1: TPanel
          Left = 73
          Top = 39
          Width = 104
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object PanelVoltDecIW1: TPanel
          Left = 74
          Top = 95
          Width = 104
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object GroupBox4: TGroupBox
        Left = 201
        Top = 3
        Width = 195
        Height = 173
        Caption = 'IW2 [82h]'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object Label19: TLabel
          Left = 8
          Top = 42
          Width = 64
          Height = 16
          Caption = 'Volt (hex) :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label20: TLabel
          Left = 8
          Top = 97
          Width = 64
          Height = 16
          Caption = 'Volt (dec) :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object PanelVoltHexIw2: TPanel
          Left = 74
          Top = 42
          Width = 104
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object PanelVoltDecIW2: TPanel
          Left = 74
          Top = 96
          Width = 104
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
    end
    object Panel11: TPanel
      Left = 9
      Top = 24
      Width = 320
      Height = 185
      BevelInner = bvLowered
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Panel15: TPanel
        Left = 5
        Top = 9
        Width = 308
        Height = 168
        BevelInner = bvLowered
        TabOrder = 0
        object Label15: TLabel
          Left = 11
          Top = 32
          Width = 64
          Height = 16
          Caption = 'Volt (hex) :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 10
          Top = 96
          Width = 64
          Height = 16
          Caption = 'Volt (dec) :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object EditVolthex: TEdit
          Left = 76
          Top = 30
          Width = 77
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '0'
          OnKeyPress = EditVoltHexKP
        end
        object EditVoltdec: TEdit
          Left = 76
          Top = 94
          Width = 77
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnKeyPress = EditVoltDecKP
        end
        object GroupBox1: TGroupBox
          Left = 160
          Top = 8
          Width = 143
          Height = 54
          Caption = 'Schrittweite'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object ButtonVoltHexPlus: TButton
            Left = 8
            Top = 24
            Width = 33
            Height = 20
            Caption = ' + '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            OnClick = ButtonVoltHexPlusClick
          end
          object EditVoltHexStep: TEdit
            Left = 51
            Top = 24
            Width = 46
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '1'
          end
          object ButtonVoltHexMinus: TButton
            Left = 103
            Top = 24
            Width = 33
            Height = 21
            Caption = ' - '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            OnClick = ButtonVoltHexMinusClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 160
          Top = 73
          Width = 143
          Height = 54
          Caption = 'Schrittweite'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object ButtonVoltDecPlus: TButton
            Left = 8
            Top = 24
            Width = 33
            Height = 20
            Caption = ' + '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            OnClick = ButtonVoltDecPlusClick
          end
          object EditVoltDecStep: TEdit
            Left = 51
            Top = 24
            Width = 45
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '1'
            OnKeyPress = EditVoltDecSKP
          end
          object ButtonVoltDecMinus: TButton
            Left = 104
            Top = 24
            Width = 33
            Height = 19
            Caption = ' - '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            OnClick = ButtonVoltDecMinusClick
          end
        end
        object ButtonSendSollW: TButton
          Left = 5
          Top = 139
          Width = 156
          Height = 22
          Caption = 'SEND'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          OnClick = ButtonSendSollWClick
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 4
    Top = 0
    Width = 833
    Height = 36
    BevelInner = bvLowered
    TabOrder = 4
    object Label1: TLabel
      Left = 282
      Top = 9
      Width = 117
      Height = 18
      Caption = 'Aktive IFK (hex) :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object ComboBoxActIFK: TComboBox
      Left = 407
      Top = 6
      Width = 63
      Height = 26
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ItemHeight = 18
      ParentFont = False
      TabOrder = 0
      Text = '0'
    end
  end
  object ButtonExit: TButton
    Left = 685
    Top = 502
    Width = 153
    Height = 33
    Caption = 'EXIT'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = ButtonExitClick
  end
  object TimerIFKamBus: TTimer
    Interval = 2000
    OnTimer = TimerIFKamBusTimer
    Left = 568
    Top = 504
  end
  object TimerStatIW1_IW2: TTimer
    Interval = 500
    OnTimer = TimerStatIW1_IW2Timer
    Left = 536
    Top = 504
  end
  object MainMenu1: TMainMenu
    Left = 8
    object Info1: TMenuItem
      Caption = 'About'
      object Info2: TMenuItem
        Caption = 'Info'
        OnClick = Info2Click
      end
    end
  end
end
