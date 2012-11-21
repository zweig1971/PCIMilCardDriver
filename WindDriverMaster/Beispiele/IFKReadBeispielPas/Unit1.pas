//---------------------------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// Beispielprogramm zum Senden eines datums
// an eine IFK und wieder einlesen des datums
// mit den DLL- Aufrufen PCI_IfkWrite und
// PCI_IfkRead.
//
// Autor : Zweig,Marcus
//
// Delphi V5
//---------------------------------------------


unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, UnitMil;

type
  TForm1 = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    Panel1: TPanel;
    Edit1: TEdit;
    Button1: TButton;
    Button2: TButton;
    procedure FormCreate(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure Edit1KeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
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
  IFKAdress:_Byte;
  IFKOnline: TStrings;
  IFKCount:integer;
  SendData:_WORD;

implementation

{$R *.DFM}


procedure TForm1.FormCreate(Sender: TObject);

var x,y,i:integer;
    status:_DWORD;
    CardCounter :_DWORD;
    Error_Char:t_Error_Char;
    Error_string:string;

begin

     //-----------------------------------------
     // Fenster je nach auflösung skalieren
     //-----------------------------------------

     Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form1.height := (form1.clientheight * y div ScreenHeightDev)+
                           form1.height - form1.ClientHeight;

          form1.width  := (form1.clientwidth * x div ScreenWidthDev)+
                           form1.Height - form1.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     //----------------------------------------------------
     // PCI-Mil-Karten im PC suchen und die erste nehmen
     //----------------------------------------------------

     // PCI-Mil- Karten zaehlen
     CardCounter := PCI_PCIcardCount();

     // gibt es keine PCI-Mil- Karte -> tschuess
     if(CardCounter = 0) then begin
           Application.MessageBox('NO PCI- CARDS FOUND', 'ABBRUCH !', 16);
           Form1.Close;
     end;

     // erste PCI-Mil-Karte im PC-Bus nehmen
     Cardauswahl := 1;

     // öffnen der PCI Mil Karte
     status := PCI_DriverOpen(Cardauswahl);

     // status auswerten, im fehlerfall messagebox mit fehlermeldung
     if(status <> StatusOK) then begin
       PCI_ErrorString(status, Error_Char);
       for i := 1 to Length(Error_Char) do begin
          Error_string := Error_string + Error_Char[i];
       end;
       Application.MessageBox(PChar(Error_string), 'ABBRUCH !', 16);
       Form1.Close;
     end;

    //---------------------------------------------------------------------
    // IFK am Mil-Bus suchen und in eine liste aufnehmen, die erste nehmen
    //---------------------------------------------------------------------

    // Liste erzeugen
    IFKOnline := TStringList.Create;

    // Liste loeschen
    IFKOnline.Clear;

    // IFKs suchen
    IFKFound(IFKOnline, IFKCount);

    // Erste IFK nehmen gibts keine -> warnung
    try
       IFKAdress := StrToInt('$' + IFKOnline[0]);
    except
       messagebox(0,'Keine IFK gefunden !','Warnung',16);
       IFKAdress := 0;
    end;

end;

// Der EXIT Button
procedure TForm1.Button2Click(Sender: TObject);
begin
     Form1.Close;
end;

// Form wird beendet
procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);

begin
      // Treiber schliessen sehr wichtig !
     PCI_DriverClose(Cardauswahl);
end;

// Sende datum und lese wieder datum von der IFK
procedure TForm1.Button1Click(Sender: TObject);

var status:_DWORD;
    ReadData:_WORD;
    FktRead:_Byte;
    FktWrite:_Byte;

begin
     status   := 0;
     ReadData := 0;

     FktWrite    := $13; // Echo Sollwert
     FktRead     := $89; // Echo Sollwert ruecklesen

     // Datum an IFK versenden
     PCI_IfkWrite (Cardauswahl, IFKAdress, FktWrite, SendData, status);

     // Datum von IFK lesen
     PCI_IfkRead  (Cardauswahl, IFKAdress, FktRead , ReadData, status);

     // Gelesendes datum als hex ausgeben
     Form1.Panel1.Caption := IntToHex(ReadData,3);
end;

// Sende datum eingabe : Kein laestiger beep-ton bei eingabe
procedure TForm1.Edit1KeyPress(Sender: TObject; var Key: Char);

begin
   if key = #13 then begin
      key:= #0;
   end;
end;

// Sende datum eingabe : Eingabe sichern und ueberpruefen
procedure TForm1.Edit1KeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);

begin
   if(Form1.Edit1.Text <> '') then begin
     try
        SendData := StrToInt('$'+ Form1.Edit1.Text);
      except
        messagebox(0,'Fehlerhafte Eingabe bei Send Data !','Warnung',16);
      end;
   end;
end;

end.
