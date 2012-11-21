unit Unit3;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, Unit1;

type
  TForm3 = class(TForm)
    Panel1: TPanel;
    Label1: TLabel;
    Panel2: TPanel;
    SpeedButton1: TSpeedButton;
    Button1: TButton;
    Button2: TButton;
    Panel3: TPanel;
    Label2: TLabel;
    ComboBox1: TComboBox;
    Panel4: TPanel;
    Label3: TLabel;
    Label4: TLabel;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Label5: TLabel;
    Panel5: TPanel;
    Timer1: TTimer;
    Edit1: TEdit;
    Button7: TButton;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure Panel2Click(Sender: TObject);
    procedure Panel5Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Edit1Change(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Form3: TForm3;

implementation

uses Mil_Mag;

{$R *.DFM}

// Liest die IFK vom eingabefeld & wandelt diese in hex
// um und behandelt fehler
procedure TransIFK(var IFKAdress:Byte);

begin
    try
       // ifk-adresse auslesen
       IFKAdress := StrToInt('$'+Form3.ComboBox1.Text);
    except
      Form3.SpeedButton1.Down := False;
      messagebox(0,'Keine IFK angewählt !','Warnung',16);
      raise;
    end;
end;

procedure SendFctToIfk(IFKAdress:Byte; FunktionCode:Byte);

var myStatus:integer;
    status:integer;
    counter:integer;
    SendFunktionCode:integer;
    Error_String :string;
    Error_Char:t_Error_Char;

begin

    myStatus := 0;

     status := PCI_FctCodeSnd(Cardauswahl, IFKAdress,
                             FunktionCode, myStatus);// An IFK senden

     // ausgabe bei fehlerfall
     if(status <> StatusOK) then begin
        PCI_ErrorString(status, Error_Char);
        Error_String := Error_Char;
        Application.MessageBox(PChar(Error_String), 'ABBRUCH !', 16);
     end;

    // counter erhoehen und ausgabe
    counter :=  StrToInt(Form3.Panel2.Caption);
    counter := counter + 1;
    Form3.Panel2.Caption := IntToStr(counter);

    //  ausfct+adress errechnen und ausgabe
    SendFunktionCode := FunktionCode;
    SendFunktionCode := SendFunktionCode shl 8; // 8 bit nach links
    SendFunktionCode := SendFunktionCode + IFKAdress;
    Form3.Panel5.Caption := IntToHex(SendFunktionCode,6);
end;

procedure TForm3.Button2Click(Sender: TObject);
begin
     // CLOSE- Button
     Form3.SpeedButton1.Down := False;

     // ausgabe Counter null- setzen
     Panel2.Caption := '0';

     // ausgabe fct+adress null- setzen
     Panel5.Caption := '0';
     
     Close
end;

procedure TForm3.Button1Click(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
     // STEP BUTTON
     SpeedButton1.Down := False;

     // IFK Nummern feld einlesen und auf fehler pruefen
     TransIFK(IFKAdress);

    try
      // fct-code auslesen
      FunktionCode := StrToInt('$'+Edit1.Text);
    except
      messagebox(0,'Ungültige Eingabe ! Nur HEX zulässig !','Warnung',16);
      raise;
    end;
    // versenden
    SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm3.FormCreate(Sender: TObject);

var x,y :integer;

begin
     Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     // Fenster scalieren
     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form3.height := (form3.clientheight * y div ScreenHeightDev)+
                           form3.height - form3.ClientHeight;

          form3.width  := (form3.clientwidth * x div ScreenWidthDev)+
                           form3.Height - form3.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     // Auswahl aktuall.
     ComboBox1.Items := Form1.IFKOnline;
     ComboBox1.ItemIndex := 0;

     // ausgabe Counter null- setzen
     Panel2.Caption := '0';

     // ausgabe fct+adress null- setzen
     Panel5.Caption := '0';
end;

procedure TForm3.Button3Click(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
     // Reset- Button
     FunktionCode := $01;//reset

     SpeedButton1.Down := False;

     try
        // ifk-adresse auslesen
        IFKAdress := StrToInt('$'+ComboBox1.Text);
     except
           Form3.SpeedButton1.Down := False;
           messagebox(0,'Keine IFK angewählt !','Warnung',16);
           raise;
      end;

     // senden des resets an die ifk
     SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm3.Button4Click(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
     // EIN- Button
     FunktionCode := $02;//EIN

     SpeedButton1.Down := False;

      try
        // ifk-adresse auslesen
        IFKAdress := StrToInt('$'+ComboBox1.Text);
     except
           Form3.SpeedButton1.Down := False;
           messagebox(0,'Keine IFK angewählt !','Warnung',16);
           raise;
      end;

     // senden des EIN an die ifk
     SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm3.Button5Click(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
     // AUS- Button
     FunktionCode := $03;//aus

     SpeedButton1.Down := False;

     try
        // ifk-adresse auslesen
        IFKAdress := StrToInt('$'+ComboBox1.Text);
     except
           Form3.SpeedButton1.Down := False;
           messagebox(0,'Keine IFK angewählt !','Warnung',16);
           raise;
      end;
     // senden des AUS an die ifk
     SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm3.SpeedButton1Click(Sender: TObject);
begin
     // LOOP Button
end;

procedure TForm3.Panel2Click(Sender: TObject);
begin
     // Loop- Counter Panel
end;

procedure TForm3.Panel5Click(Sender: TObject);
begin
     // FCT + Adresse Ausgabe
end;

procedure TForm3.Timer1Timer(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin

   if (Form3.SpeedButton1.Down = True) then begin

       // IFK Nummern feld einlesen und auf fehler pruefen
       TransIFK(IFKAdress);

       try
          // fct-code auslesen
          FunktionCode := StrToInt('$'+Edit1.Text);
       except
             Form3.SpeedButton1.Down := False;
             messagebox(0,'Ungültige Eingabe ! Nur HEX zulässig !','Warnung',16);
             raise;
       end;

       // versenden
       SendFctToIfk(IFKAdress, FunktionCode);
    end;
end;

procedure TForm3.Edit1Change(Sender: TObject);
begin
     SpeedButton1.Down := False;
end;

// UPDATE Button
procedure TForm3.Button7Click(Sender: TObject);

var CountFound:Integer;

begin
    // IFK- Liste loeschen
    Form1.IFKOnline.Clear;
    IFKFound(Form1.IFKOnline, CountFound); // IFKs suchen
    ComboBox1.Items := Form1.IFKOnline;
    ComboBox1.ItemIndex := 0;
end;


procedure TForm3.Edit1KeyPress(Sender: TObject; var Key: Char);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
   if key = #13 then begin
      key:= #0;

      SpeedButton1.Down := False;
      // IFK Nummern feld einlesen und auf fehler pruefen
      TransIFK(IFKAdress);

      try
         // fct-code auslesen
         FunktionCode := StrToInt('$'+Edit1.Text);
      except
         messagebox(0,'Ungültige Eingabe ! Nur HEX zulässig !','Warnung',16);
      raise;
      end;

    // versenden
    SendFctToIfk(IFKAdress, FunktionCode);

   end;
end;
end.


