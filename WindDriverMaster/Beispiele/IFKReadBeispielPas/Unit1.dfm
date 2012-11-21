object Form1: TForm1
  Left = 333
  Top = 162
  Width = 200
  Height = 276
  Caption = 'IFKRead'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -18
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 24
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 102
    Height = 24
    Caption = 'Send Data : '
  end
  object Label2: TLabel
    Left = 8
    Top = 80
    Width = 97
    Height = 24
    Caption = 'Read Data :'
  end
  object Panel1: TPanel
    Left = 112
    Top = 72
    Width = 73
    Height = 41
    BevelInner = bvLowered
    TabOrder = 0
  end
  object Edit1: TEdit
    Left = 112
    Top = 16
    Width = 73
    Height = 32
    TabOrder = 1
    Text = '1'
    OnKeyPress = Edit1KeyPress
    OnKeyUp = Edit1KeyUp
  end
  object Button1: TButton
    Left = 8
    Top = 128
    Width = 177
    Height = 49
    Caption = 'Send/Read IFK'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 72
    Top = 192
    Width = 113
    Height = 49
    Caption = 'EXIT'
    TabOrder = 3
    OnClick = Button2Click
  end
end
