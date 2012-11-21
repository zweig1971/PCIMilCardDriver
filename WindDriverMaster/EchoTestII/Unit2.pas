unit Unit2;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, UnitMil, StdCtrls, ExtCtrls, Buttons;

type
  TForm2 = class(TForm)
    MainMenu1: TMainMenu;
    Option1: TMenuItem;
    Abour1: TMenuItem;
    N1: TMenuItem;
    IFKAuswhlen1: TMenuItem;
    N2: TMenuItem;
    ReadWriteFCTCodesndern1: TMenuItem;
    N3: TMenuItem;
    SingelStep1: TMenuItem;
    N4: TMenuItem;
    Beenden1: TMenuItem;
    Info1: TMenuItem;
    Panel1: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    Edit1: TEdit;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Panel3: TPanel;
    Panel4: TPanel;
    Panel5: TPanel;
    ListBox1: TListBox;
    Label6: TLabel;
    Button1: TButton;
    Button2: TButton;
    SpeedButton1: TSpeedButton;
    Button3: TButton;
    Button4: TButton;
    Panel6: TPanel;
    ErrorStop_CheckBox: TCheckBox;
    procedure IFKAuswhlen1Click(Sender: TObject);
    procedure ReadWriteFCTCodesndern1Click(Sender: TObject);
    procedure SingelStep1Click(Sender: TObject);
    procedure Beenden1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Info1Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button3Click(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure Edit1KeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Form2: TForm2;

  WriteMilBusTimeOutC:integer;
  ReadMilBusTimeOutC:integer;
  VerifyFailtC:integer;
  FID:integer;

  LoopCounter:longint;
  SendCounter:longint;
  IFKAdress:Byte;
  SendData:integer;
  FktRead:Byte;
  FktWrite:Byte;

  IFKOnline: TStrings;
  IFKCount:integer;
  IFKOnlineIndex:integer;

implementation

uses Unit3, Unit4, Unit5, Unit1, Unit6;

{$R *.DFM}

// Ein Echo durchlauf-Test
procedure echo_durchlauf(var Error:boolean);

var mySendData:_WORD;
    status:_DWORD;
    ReadData:_WORD;

begin
    status   := 0;
    ReadData := 0;
    Error    := false;

    // Auto test oder manuell
     if(Form2.RadioButton1.Checked = True) then begin
       mySendData := LoopCounter mod $10000;
     end else begin
        mySendData := SendData;
     end;

      // Daten versenden und wieder lesen
     PCI_IfkWrite (Cardauswahl, IFKAdress, FktWrite, mySendData, status);
     SendCounter := SendCounter + 2;
     PCI_IfkRead  (Cardauswahl, IFKAdress, FktRead , ReadData, status);
     SendCounter := SendCounter + 1;

     // Abpruefen auf write milbus timeout fehler
     if((status and WriteMilBusTimeOut)<> 0) then begin
         WriteMilBusTimeOutC := WriteMilBusTimeOutC +1;
         Form2.ListBox1.Items.Add('Write TimeOut'+ DateTimeToStr(Now));
         Error := True;
     end;

     //  Abpruefen Unrequested Receiv warnung
     if ((status and UnrequestedReceiv)<> 0) then begin
         Form2.ListBox1.Items.Add('Unreq. Receiv'+ DateTimeToStr(Now));
     end;

     // Pruefen ob timout read oder falsche daten
     if((status and ReadMilBusTimeOut)<> 0) then begin
         ReadMilBusTimeOutC := ReadMilBusTimeOutC +1;
         Form2.ListBox1.Items.Add('Read TimeOut '+ DateTimeToStr(Now));
         Error := True;
     end else if (ReadData <> mySendData) then begin
         Form2.ListBox1.Items.Add(IntToHex(mySendData,4)
         +' / '+IntToHex(ReadData,4)+'  ' + DateTimeToStr(Now));
         VerifyFailtC := VerifyFailtC +1;
         Error := True;
     end;

     // counter erhoehen
     LoopCounter := LoopCounter +1;

     // Ausgabe
     if ( LoopCounter mod 100 = 0) or (Error = True) or
        (Form2.SpeedButton1.Down = False) then begin
         Form2.Panel3.Caption := IntToStr(ReadMilBusTimeOutC);
         Form2.Panel4.Caption := IntToStr(WriteMilBusTimeOutC);
         Form2.Panel5.Caption := IntToStr(VerifyFailtC);
         Form2.Panel6.Caption := IntToStr(SendCounter);
         Form2.Update;
         Application.ProcessMessages;
     end;
end;

procedure echo_dauerlauf();

var  Error:boolean;

begin
    while(Form2.SpeedButton1.Down = True) do begin
       echo_durchlauf(Error);
       if (Form2.ErrorStop_CheckBox.Checked = true) and (Error = true) then begin
          Form2.SpeedButton1.Down := false;
       end;
    end;
end;

// Menue IFK Auswahl
procedure TForm2.IFKAuswhlen1Click(Sender: TObject);
begin
     Application.CreateForm(TForm3, Form3);
     Form3.Show;
end;

// Menue Read/Write
procedure TForm2.ReadWriteFCTCodesndern1Click(Sender: TObject);
begin
     Application.CreateForm(TForm4, Form4);
     Form4.Show;
end;

// Menue SingelStep
procedure TForm2.SingelStep1Click(Sender: TObject);
begin
     Application.CreateForm(TForm5, Form5);
     Form5.Show;
end;

// Menue BEENDEN
procedure TForm2.Beenden1Click(Sender: TObject);
begin
     // Bye...bye...
     Form2.Close;
     Form1.Close;
end;

procedure TForm2.FormCreate(Sender: TObject);

//var x,y:integer;


begin
{     Scaled := true;
     x := Screen.width;
     y := Screen.height;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form2.height := (form2.clientheight * y div ScreenHeightDev)+
                           (form2.height - form2.clientheight);

          form2.width  := (form2.clientwidth * x div ScreenWidthDev)+
                           (form2.height - form2.clientheight);

          scaleBy(x, ScreenwidthDev);
     end;       }

     IFKOnline := TStringList.Create;

     FID := 0;

     // Standartwerte
     LoopCounter := 0;
     FktWrite    := $13;
     FktRead     := $89;
     SendData    := 1;
     SendCounter := 0;
    // Liste loeschen
//    IFKOnline.Clear;
    Form2.ListBox1.Items.Clear;

    // IFKs suchen
    IFKFound(IFKOnline, IFKCount);

    // Erste IFK nehmen
    try
       IFKAdress := StrToInt('$' + IFKOnline[0]);
       IFKOnlineIndex := 0;
    except
       messagebox(0,'Keine IFK gefunden !','Warnung',16);
       IFKAdress := 0;
    end;
end;

// INFO
procedure TForm2.Info1Click(Sender: TObject);
begin
     Application.CreateForm(TForm6, Form6);
     Form6.Show;
end;

// EXIT
procedure TForm2.Button4Click(Sender: TObject);
begin
     // Loop aus
     Form2.SpeedButton1.Down := False;

     // Bye...bye...
     Form2.Close;
     Form1.Close;
end;

// Close Form 3
procedure TForm2.FormClose(Sender: TObject; var Action: TCloseAction);

begin
     // Loop aus
     Form2.SpeedButton1.Down := False;

      // Treiber schliessen
     PCI_DriverClose(Cardauswahl);

     // Bye...bye...
     Form1.Close;
end;

// STEP
procedure TForm2.Button3Click(Sender: TObject);

var Error:boolean;

begin
     //Loop aus
     Form2.SpeedButton1.Down := False;

     echo_durchlauf(Error);
end;

// LOOP
procedure TForm2.SpeedButton1Click(Sender: TObject);

begin
     echo_dauerlauf();
end;

// Reset PCI-Card
procedure TForm2.Button1Click(Sender: TObject);

var status:_DWORD;

begin
     PCI_PCIcardReset(Cardauswahl, status);
end;

// reset
procedure TForm2.Button2Click(Sender: TObject);

begin
    // Loop aus
     Form2.SpeedButton1.Down := False;

     // Ausgabe felder zuruecksetzen
     Form2.Panel3.Caption := '0';
     Form2.Panel4.Caption := '0';
     Form2.Panel5.Caption := '0';
     Form2.Panel6.Caption := '0';

     LoopCounter         := 0;
     WriteMilBusTimeOutC := 0;
     ReadMilBusTimeOutC  := 0;
     VerifyFailtC        := 0;
     SendCounter         := 0;

     Form2.ListBox1.Items.Clear;
//     VerifyFailure.Clear;
//     Form2.ListBox1.Items  := VerifyFailure;
end;

// Send Data Eingabe
procedure TForm2.Edit1KeyPress(Sender: TObject; var Key: Char);
begin
   // Loop aus
   Form2.SpeedButton1.Down := False;

   if key = #13 then begin
      key:= #0;
   end;
end;

// Send Data-eingabe
procedure TForm2.Edit1KeyUp(Sender: TObject; var Key: Word;Shift: TShiftState);
begin
   if(Form2.Edit1.Text <> '') then begin
     try
        SendData := StrToInt('$'+ Form2.Edit1.Text);
      except
        messagebox(0,'Fehlerhafte Eingabe bei Send Data !','Warnung',16);
      end;
   end;
end;

end.
