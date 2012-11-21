unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, UnitMil;

type
  TForm1 = class(TForm)
    Panel1: TPanel;
    Button1: TButton;
    SpeedButton1: TSpeedButton;
    Shape1: TShape;
    Shape2: TShape;
    Shape3: TShape;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Timer1: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

      {Bidschirmaufloesung}
const ScreenHeightDev = 1024;
      ScreenWidthDev  = 1280;

var
  Form1: TForm1;
  Cardauswahl:integer;


implementation

{$R *.DFM}

procedure TForm1.FormCreate(Sender: TObject);

var x,y,i:integer;
    status:_DWORD;
    Error_Char:t_Error_Char;
    Error_string:string;

begin
    Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     Cardauswahl := 2;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form1.height := (form1.clientheight * y div ScreenHeightDev)+
                           form1.height - form1.ClientHeight;

          form1.width  := (form1.clientwidth * x div ScreenWidthDev)+
                           form1.Height - form1.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     // öffnen der PCI Mil Karte
     status := PCI_DriverOpen(Cardauswahl);
     if(status <> StatusOK) then begin
        PCI_ErrorString(status, Error_Char); // ausgabe bei fehlerfall
        for i := 1 to Length(Error_Char) do begin
            Error_string := Error_string + Error_Char[i];
        end;
        Application.MessageBox(PChar(Error_string), 'ABBRUCH !', 16);
        Form1.Close;
     end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);

var status:boolean;
    ErrorStatus:_DWORD;

begin

     ErrorStatus := 0;

     IF (Form1.SpeedButton1.Down = true) then begin

        // Puls Input 1
        status := PCI_StatusTest(Cardauswahl, Puls_In1, ErrorStatus);
        if(status = true) then begin
           Form1.Shape1.Brush.Color := clLime;
        end else begin
           Form1.Shape1.Brush.Color := clWhite;
        end;

       // Puls Input 2
        status := PCI_StatusTest(Cardauswahl, Puls_In2, ErrorStatus);
        if(status = true) then begin
           Form1.Shape2.Brush.Color := clLime;
        end else begin
           Form1.Shape2.Brush.Color := clWhite;
        end;

        // Puls Input 3
        status := PCI_StatusTest(Cardauswahl, Puls_In3, ErrorStatus);
        if(status = true) then begin
           Form1.Shape3.Brush.Color := clLime;
        end else begin
           Form1.Shape3.Brush.Color := clWhite;
        end;
     end;

end;

//Exit
procedure TForm1.Button1Click(Sender: TObject);
begin
     Form1.Close;
end;

// Bye....
procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
     PCI_DriverClose(Cardauswahl);
end;

end.
