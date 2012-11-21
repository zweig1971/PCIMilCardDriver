unit Unit2;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, DBCGrids, Buttons, ExtCtrls, Unit1;

type
  TForm2 = class(TForm)
    Label1: TLabel;
    ListBox1: TListBox;
    Button3: TButton;
    SpeedButton1: TSpeedButton;
    Button1: TButton;
    Label2: TLabel;
    Panel1: TPanel;
    Timer1: TTimer;
    procedure Button3Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    IFKOnline: TStrings
  end;

var
  Form2: TForm2;

implementation

{$R *.DFM}

procedure TForm2.Button3Click(Sender: TObject);
begin

     // IFK Liste loeschen und ausgeben
     IFKOnline.Clear;
     Form2.ListBox1.Items := IFKOnline;

     // Zaehler auf null setzen
     Form2.Panel1.Caption := '0';

     // Loop- button ruecksetzen
     Form2.SpeedButton1.Down := False;
     Close
end;

procedure TForm2.Button1Click(Sender: TObject);

var CountFound:integer;

begin
     // Loop Button aus
     Form2.SpeedButton1.Down := False;

     // Liste creat, loeschen und anzeigen
     IFKOnline := TStringList.Create;
     IFKOnline.Clear;
     ListBox1.Items := IFKOnline;
     Panel1.Caption := '0';

     // IFKs suchen und anzeigen
     IFKFound(IFKOnline, CountFound);
     ListBox1.Items := IFKOnline;
     Panel1.Caption := IntToStr(CountFound);
     IFKOnline.Clear;
end;

procedure TForm2.FormCreate(Sender: TObject);

var x,y:integer;

begin

     Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form2.height := (form2.clientheight * y div ScreenHeightDev)+
                           form2.height - form2.ClientHeight;

          form2.width  := (form2.clientwidth * x div ScreenWidthDev)+
                           form2.Height - form2.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     // IFK liste erzeugen
     IFKOnline := TStringList.Create;
end;

procedure TForm2.Timer1Timer(Sender: TObject);

var CountFound:integer;

begin
     IFKOnline.Clear;

     if (Form2.SpeedButton1.Down = True) then begin
        IFKFound(IFKOnline, CountFound); // IFKs suchen
        Form2.ListBox1.Items := IFKOnline;
        Form2.Panel1.Caption := IntToStr(CountFound);
        IFKOnline.Clear;
     end;

end;

end.
