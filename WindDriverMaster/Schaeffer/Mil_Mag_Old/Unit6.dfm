object Form6: TForm6
  Left = 65
  Top = 174
  Width = 1198
  Height = 571
  Caption = 'Sollwert- Daten auf Mil- Bus Schreiben V.0.1'
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
  object SpeedButton1: TSpeedButton
    Left = 8
    Top = 456
    Width = 145
    Height = 73
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
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 1169
    Height = 433
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Panel2: TPanel
      Left = 16
      Top = 24
      Width = 489
      Height = 393
      BevelInner = bvLowered
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label1: TLabel
        Left = 16
        Top = 16
        Width = 169
        Height = 23
        Caption = 'Wr.- Data- Count : '
      end
      object Panel3: TPanel
        Left = 184
        Top = 16
        Width = 113
        Height = 25
        BevelInner = bvLowered
        Caption = '1'
        TabOrder = 0
      end
      object Panel4: TPanel
        Left = 8
        Top = 48
        Width = 473
        Height = 337
        BevelInner = bvLowered
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object Label2: TLabel
          Left = 24
          Top = 24
          Width = 164
          Height = 23
          Caption = 'Aktive IFK  (hex) : '
        end
        object ComboBox1: TComboBox
          Left = 192
          Top = 24
          Width = 73
          Height = 31
          ItemHeight = 23
          TabOrder = 0
          Text = '1'
        end
        object Panel5: TPanel
          Left = 0
          Top = 72
          Width = 473
          Height = 265
          BevelInner = bvLowered
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label3: TLabel
            Left = 16
            Top = 32
            Width = 45
            Height = 23
            Caption = 'Hex :'
          end
          object Label4: TLabel
            Left = 16
            Top = 128
            Width = 98
            Height = 23
            Caption = 'Volt (dec) :'
          end
          object Label5: TLabel
            Left = 16
            Top = 216
            Width = 99
            Height = 23
            Caption = 'Data (bin) :'
          end
          object Edit1: TEdit
            Left = 120
            Top = 32
            Width = 153
            Height = 31
            TabOrder = 0
            Text = '1'
            OnKeyPress = Edit1KeyPress
          end
          object GroupBox1: TGroupBox
            Left = 280
            Top = 16
            Width = 185
            Height = 65
            Caption = 'Schrittweite'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object Button1: TButton
              Left = 8
              Top = 24
              Width = 41
              Height = 25
              Caption = '+'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnClick = Button1Click
            end
            object Button2: TButton
              Left = 128
              Top = 24
              Width = 41
              Height = 25
              Caption = '-'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnClick = Button2Click
            end
            object Edit3: TEdit
              Left = 64
              Top = 24
              Width = 57
              Height = 31
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              Text = '1'
            end
          end
          object Panel7: TPanel
            Left = 120
            Top = 216
            Width = 337
            Height = 25
            BevelInner = bvLowered
            Caption = '0000 0000 0000 0000'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -20
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object GroupBox4: TGroupBox
            Left = 280
            Top = 104
            Width = 185
            Height = 65
            Caption = 'Schrittweite'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -17
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            object Button5: TButton
              Left = 8
              Top = 24
              Width = 41
              Height = 25
              Caption = '+'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnClick = Button5Click
            end
            object Button6: TButton
              Left = 128
              Top = 24
              Width = 41
              Height = 25
              Caption = '-'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              OnClick = Button6Click
            end
            object Edit4: TEdit
              Left = 64
              Top = 24
              Width = 57
              Height = 31
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -20
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              Text = '1'
              OnKeyPress = Edit4KeyPress
            end
          end
          object Edit2: TEdit
            Left = 120
            Top = 128
            Width = 153
            Height = 31
            TabOrder = 4
            Text = '1'
            OnKeyPress = Edit2KeyPress
          end
        end
        object Button8: TButton
          Left = 288
          Top = 24
          Width = 137
          Height = 33
          Caption = 'UPDATE'
          TabOrder = 2
          OnClick = Button8Click
        end
      end
    end
    object Panel8: TPanel
      Left = 512
      Top = 24
      Width = 641
      Height = 393
      BevelInner = bvLowered
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Label6: TLabel
        Left = 8
        Top = 16
        Width = 162
        Height = 23
        Caption = 'Rd- Data- Count : '
      end
      object Panel9: TPanel
        Left = 168
        Top = 16
        Width = 105
        Height = 25
        BevelInner = bvLowered
        Caption = '1'
        TabOrder = 0
      end
      object GroupBox2: TGroupBox
        Left = 8
        Top = 56
        Width = 305
        Height = 329
        Caption = 'IW1 [81H]'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object Label7: TLabel
          Left = 8
          Top = 64
          Width = 102
          Height = 23
          Caption = 'Volt (hex) : '
        end
        object Label8: TLabel
          Left = 8
          Top = 160
          Width = 98
          Height = 23
          Caption = 'Volt (dec) :'
        end
        object Label9: TLabel
          Left = 8
          Top = 280
          Width = 99
          Height = 23
          Caption = 'Data (bin) :'
        end
        object Panel10: TPanel
          Left = 112
          Top = 64
          Width = 169
          Height = 25
          BevelInner = bvLowered
          Caption = '01234567890'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object Panel11: TPanel
          Left = 112
          Top = 160
          Width = 169
          Height = 25
          BevelInner = bvLowered
          Caption = '1'
          TabOrder = 1
        end
        object Panel12: TPanel
          Left = 112
          Top = 280
          Width = 185
          Height = 25
          BevelInner = bvLowered
          Caption = '1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
      end
      object GroupBox3: TGroupBox
        Left = 328
        Top = 56
        Width = 305
        Height = 329
        Caption = 'IW2 [82H]'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -20
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        object Label10: TLabel
          Left = 16
          Top = 64
          Width = 102
          Height = 23
          Caption = 'Volt (hex) : '
        end
        object Label11: TLabel
          Left = 16
          Top = 160
          Width = 98
          Height = 23
          Caption = 'Volt (dec) :'
        end
        object Label12: TLabel
          Left = 8
          Top = 280
          Width = 99
          Height = 23
          Caption = 'Data (bin) :'
        end
        object Panel13: TPanel
          Left = 120
          Top = 64
          Width = 169
          Height = 25
          BevelInner = bvLowered
          Caption = '1'
          TabOrder = 0
        end
        object Panel14: TPanel
          Left = 120
          Top = 160
          Width = 169
          Height = 25
          BevelInner = bvLowered
          Caption = '1'
          TabOrder = 1
        end
        object Panel15: TPanel
          Left = 112
          Top = 280
          Width = 185
          Height = 25
          BevelInner = bvLowered
          Caption = '1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
      end
    end
  end
  object Button3: TButton
    Left = 1032
    Top = 456
    Width = 145
    Height = 73
    Caption = 'CLOSE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 200
    Top = 456
    Width = 145
    Height = 73
    Caption = 'STEP'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = Button4Click
  end
  object Button7: TButton
    Left = 488
    Top = 456
    Width = 153
    Height = 73
    Caption = 'Error List'
    TabOrder = 3
    OnClick = Button7Click
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 696
    Top = 456
  end
end
