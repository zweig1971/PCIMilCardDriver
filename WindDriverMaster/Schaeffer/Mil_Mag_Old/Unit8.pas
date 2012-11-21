unit Unit8;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls,Unit1, ExtCtrls;

type
  TForm8 = class(TForm)
    Button1: TButton;
    Panel1: TPanel;
    Panel2: TPanel;
    Panel3: TPanel;
    Label1: TLabel;
    Panel4: TPanel;
    ListBox1: TListBox;
    Label13: TLabel;
    Button2: TButton;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
    ErrorList: TStrings;
  end;

var
  Form8: TForm8;

implementation

//uses Unit2;

{$R *.DFM}

procedure TForm8.FormCreate(Sender: TObject);

var x,y :integer;
    CardCounter :integer;

begin
    Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

//     PCICount := TStringList.Create;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form8.height := (form8.clientheight * y div ScreenHeightDev)+
                           form8.height - form8.ClientHeight;

          form8.width  := (form8.clientwidth * x div ScreenWidthDev)+
                           form8.Height - form8.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     // errorliste erzeugen
     ErrorList := TStringList.Create;

     // 
end;

procedure TForm8.Button1Click(Sender: TObject);
begin
     Close
end;

procedure TForm8.Button2Click(Sender: TObject);
begin
     // Clear Button

     //loeschen der box
     ListBox1.Items.Clear;

     // error liste loeschen
     ErrorList.Clear;

     // ausgabe error counter loeschen
     Panel4.Caption := '0';
end;

end.
