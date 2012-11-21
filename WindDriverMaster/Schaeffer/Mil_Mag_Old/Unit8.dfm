object Form8: TForm8
  Left = 289
  Top = 255
  Width = 367
  Height = 425
  Caption = 'PCI- Mil- Karten Auswahl'
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
  object Button1: TButton
    Left = 216
    Top = 336
    Width = 137
    Height = 49
    Caption = 'CLOSE '
    TabOrder = 0
    OnClick = Button1Click
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 345
    Height = 313
    TabOrder = 1
    object Panel2: TPanel
      Left = 8
      Top = 8
      Width = 329
      Height = 297
      BevelInner = bvLowered
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 132
        Height = 23
        Caption = 'Error Counter :'
      end
      object Panel3: TPanel
        Left = 0
        Top = 48
        Width = 329
        Height = 249
        BevelInner = bvLowered
        TabOrder = 0
        object Label13: TLabel
          Left = 6
          Top = 8
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
        object Button2: TButton
          Left = 200
          Top = 200
          Width = 121
          Height = 41
          Caption = 'Clear'
          TabOrder = 0
          OnClick = Button2Click
        end
      end
      object Panel4: TPanel
        Left = 144
        Top = 8
        Width = 169
        Height = 33
        BevelInner = bvLowered
        Caption = '0'
        TabOrder = 1
      end
    end
  end
  object ListBox1: TListBox
    Left = 24
    Top = 96
    Width = 177
    Height = 209
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ItemHeight = 16
    ParentFont = False
    TabOrder = 2
  end
end
