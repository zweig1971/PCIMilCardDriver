object Form4: TForm4
  Left = 318
  Top = 157
  Width = 893
  Height = 490
  Caption = 'Lese IFK Status V.0.1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -18
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 24
  object SpeedButton1: TSpeedButton
    Left = 8
    Top = 400
    Width = 121
    Height = 49
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'LOOP'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 865
    Height = 385
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Panel2: TPanel
      Left = 8
      Top = 16
      Width = 513
      Height = 353
      BevelInner = bvLowered
      TabOrder = 0
      object Label1: TLabel
        Left = 16
        Top = 32
        Width = 132
        Height = 24
        Caption = 'Read Counter : '
      end
      object Panel3: TPanel
        Left = 168
        Top = 32
        Width = 185
        Height = 25
        BevelInner = bvLowered
        Caption = '1'
        TabOrder = 0
      end
      object Panel5: TPanel
        Left = 0
        Top = 80
        Width = 513
        Height = 273
        BevelInner = bvLowered
        TabOrder = 1
        object Label3: TLabel
          Left = 16
          Top = 16
          Width = 98
          Height = 24
          Caption = 'Aktive IFK : '
        end
        object ComboBox1: TComboBox
          Left = 120
          Top = 16
          Width = 65
          Height = 32
          ItemHeight = 24
          TabOrder = 0
          Text = '1'
        end
        object Panel6: TPanel
          Left = 0
          Top = 64
          Width = 513
          Height = 209
          BevelInner = bvLowered
          TabOrder = 1
          object Panel7: TPanel
            Left = 0
            Top = 0
            Width = 513
            Height = 209
            BevelInner = bvLowered
            TabOrder = 0
            object Label4: TLabel
              Left = 16
              Top = 32
              Width = 136
              Height = 24
              Caption = 'Status C0  (bin) :'
            end
            object Label5: TLabel
              Left = 16
              Top = 80
              Width = 136
              Height = 24
              Caption = 'Status C1  (bin) :'
            end
            object Label6: TLabel
              Left = 16
              Top = 128
              Width = 136
              Height = 24
              Caption = 'Status C2  (bin) :'
            end
            object Label7: TLabel
              Left = 160
              Top = 8
              Width = 47
              Height = 20
              Caption = 'Status'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -17
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label8: TLabel
              Left = 264
              Top = 8
              Width = 59
              Height = 20
              Caption = 'Adresse'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -17
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label9: TLabel
              Left = 416
              Top = 8
              Width = 35
              Height = 20
              Caption = 'Data'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -17
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label10: TLabel
              Left = 160
              Top = 152
              Width = 35
              Height = 20
              Caption = 'MSB'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -17
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label11: TLabel
              Left = 328
              Top = 152
              Width = 31
              Height = 20
              Caption = 'LSB'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -17
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label12: TLabel
              Left = 424
              Top = 152
              Width = 34
              Height = 20
              Caption = 'HEX'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -17
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Panel8: TPanel
              Left = 160
              Top = 32
              Width = 97
              Height = 33
              BevelInner = bvLowered
              Caption = '0000 0000'
              TabOrder = 0
            end
            object Panel9: TPanel
              Left = 264
              Top = 32
              Width = 97
              Height = 33
              BevelInner = bvLowered
              Caption = '0000 0000'
              TabOrder = 1
            end
            object Panel10: TPanel
              Left = 264
              Top = 72
              Width = 97
              Height = 33
              BevelInner = bvLowered
              Caption = '0000 0000'
              TabOrder = 2
            end
            object Panel11: TPanel
              Left = 160
              Top = 72
              Width = 97
              Height = 33
              BevelInner = bvLowered
              Caption = '0000 0000'
              TabOrder = 3
            end
            object Panel12: TPanel
              Left = 160
              Top = 120
              Width = 97
              Height = 33
              BevelInner = bvLowered
              Caption = '0000 0000'
              TabOrder = 4
            end
            object Panel13: TPanel
              Left = 264
              Top = 120
              Width = 97
              Height = 33
              BevelInner = bvLowered
              Caption = '0000 0000'
              TabOrder = 5
            end
            object Panel14: TPanel
              Left = 416
              Top = 32
              Width = 57
              Height = 33
              BevelInner = bvLowered
              Caption = '0000'
              TabOrder = 6
            end
            object Panel15: TPanel
              Left = 416
              Top = 72
              Width = 57
              Height = 33
              BevelInner = bvLowered
              Caption = '0000'
              TabOrder = 7
            end
            object Panel16: TPanel
              Left = 416
              Top = 120
              Width = 57
              Height = 33
              BevelInner = bvLowered
              Caption = '0000'
              TabOrder = 8
            end
          end
        end
        object Button5: TButton
          Left = 208
          Top = 16
          Width = 129
          Height = 33
          Caption = 'UPDATE'
          TabOrder = 2
          OnClick = Button5Click
        end
      end
    end
    object Panel17: TPanel
      Left = 528
      Top = 16
      Width = 321
      Height = 353
      BevelInner = bvLowered
      TabOrder = 1
      object Label13: TLabel
        Left = 14
        Top = 96
        Width = 64
        Height = 20
        Caption = 'Error List'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 321
        Height = 81
        BevelInner = bvLowered
        TabOrder = 0
        object Label2: TLabel
          Left = 16
          Top = 32
          Width = 124
          Height = 24
          Caption = 'Error Counter :'
        end
        object Panel18: TPanel
          Left = 144
          Top = 32
          Width = 153
          Height = 25
          BevelInner = bvLowered
          Caption = '1'
          TabOrder = 0
        end
      end
      object ListBox1: TListBox
        Left = 16
        Top = 120
        Width = 177
        Height = 209
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 16
        ParentFont = False
        TabOrder = 1
      end
      object Button3: TButton
        Left = 208
        Top = 288
        Width = 105
        Height = 41
        Caption = 'Clear'
        TabOrder = 2
        OnClick = Button3Click
      end
    end
  end
  object Button1: TButton
    Left = 208
    Top = 400
    Width = 121
    Height = 49
    Caption = 'STEP'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 752
    Top = 400
    Width = 121
    Height = 49
    Caption = 'CLOSE'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 680
    Top = 400
  end
end
