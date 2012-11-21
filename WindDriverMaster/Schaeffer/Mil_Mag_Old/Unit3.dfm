object Form3: TForm3
  Left = 308
  Top = 178
  Width = 786
  Height = 592
  Caption = 'Sende Fct.-Code to IFK V.0.1'
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
    Left = 16
    Top = 488
    Width = 137
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
    OnClick = SpeedButton1Click
  end
  object Panel1: TPanel
    Left = 16
    Top = 8
    Width = 745
    Height = 465
    BevelInner = bvLowered
    Color = clInactiveBorder
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 137
      Height = 23
      Caption = 'Loop Counter : '
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Panel2: TPanel
      Left = 152
      Top = 16
      Width = 225
      Height = 41
      BevelInner = bvLowered
      Caption = '1'
      Color = clInactiveBorder
      TabOrder = 0
      OnClick = Panel2Click
    end
    object Panel3: TPanel
      Left = 0
      Top = 64
      Width = 745
      Height = 401
      BevelInner = bvLowered
      TabOrder = 1
      object Label2: TLabel
        Left = 16
        Top = 16
        Width = 108
        Height = 23
        Caption = 'Active IFK : '
      end
      object ComboBox1: TComboBox
        Left = 128
        Top = 16
        Width = 65
        Height = 31
        ItemHeight = 23
        TabOrder = 0
        Text = '1'
      end
      object Panel4: TPanel
        Left = 0
        Top = 56
        Width = 745
        Height = 345
        BevelInner = bvLowered
        TabOrder = 1
        object Label5: TLabel
          Left = 136
          Top = 296
          Width = 193
          Height = 23
          Caption = 'Fct + Adresse (hex) : '
        end
        object TPanel
          Left = 0
          Top = 0
          Width = 385
          Height = 281
          BevelInner = bvLowered
          TabOrder = 0
          object Label3: TLabel
            Left = 16
            Top = 40
            Width = 207
            Height = 23
            Caption = 'Send Fct.- Code (hex) :'
          end
          object Edit1: TEdit
            Left = 232
            Top = 40
            Width = 89
            Height = 31
            TabOrder = 0
            Text = '1'
            OnChange = Edit1Change
            OnKeyPress = Edit1KeyPress
          end
        end
        object TPanel
          Left = 384
          Top = 0
          Width = 361
          Height = 281
          BevelInner = bvLowered
          TabOrder = 1
          object Label4: TLabel
            Left = 16
            Top = 16
            Width = 157
            Height = 23
            Caption = 'Send Fct.- Code :'
          end
          object Button3: TButton
            Left = 72
            Top = 64
            Width = 193
            Height = 49
            Caption = 'Reset [01H]'
            TabOrder = 0
            OnClick = Button3Click
          end
          object Button4: TButton
            Left = 72
            Top = 136
            Width = 193
            Height = 49
            Caption = 'Ein    [02H]'
            TabOrder = 1
            OnClick = Button4Click
          end
          object Button5: TButton
            Left = 72
            Top = 208
            Width = 193
            Height = 49
            Caption = 'Aus   [03H]'
            TabOrder = 2
            OnClick = Button5Click
          end
        end
        object Panel5: TPanel
          Left = 336
          Top = 296
          Width = 105
          Height = 33
          BevelInner = bvLowered
          Caption = '1'
          TabOrder = 2
          OnClick = Panel5Click
        end
      end
      object Button7: TButton
        Left = 216
        Top = 16
        Width = 145
        Height = 33
        Caption = 'UPDATE'
        TabOrder = 2
        OnClick = Button7Click
      end
    end
  end
  object Button1: TButton
    Left = 216
    Top = 488
    Width = 137
    Height = 57
    Caption = 'STEP'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 624
    Top = 488
    Width = 137
    Height = 57
    Caption = 'CLOSE'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 536
    Top = 488
  end
end
