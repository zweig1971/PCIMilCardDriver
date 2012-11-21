unit Unit4;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, Unit1;

type
  TForm4 = class(TForm)
    Panel1: TPanel;
    SpeedButton1: TSpeedButton;
    Button1: TButton;
    Button2: TButton;
    Panel2: TPanel;
    Label1: TLabel;
    Panel3: TPanel;
    Panel5: TPanel;
    Label3: TLabel;
    ComboBox1: TComboBox;
    Panel6: TPanel;
    Panel7: TPanel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Panel8: TPanel;
    Panel9: TPanel;
    Label7: TLabel;
    Label8: TLabel;
    Panel10: TPanel;
    Panel11: TPanel;
    Panel12: TPanel;
    Panel13: TPanel;
    Panel14: TPanel;
    Label9: TLabel;
    Panel15: TPanel;
    Panel16: TPanel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Timer1: TTimer;
    Panel17: TPanel;
    Panel4: TPanel;
    Label2: TLabel;
    Panel18: TPanel;
    Label13: TLabel;
    ListBox1: TListBox;
    Button3: TButton;
    Button5: TButton;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button5Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
    ErrorList: TStrings;
  end;

var
  Form4: TForm4;

implementation

uses Unit5, Mil_Mag;

{$R *.DFM}


procedure ReadFromIfk(IFKAdress:Byte; FunktionCode:Byte;
                      var data:integer;var ErrorList: TStrings);

var myStatus:integer;
    status:integer;
    counter:integer;
    errorcount:integer;
    Error_Char:t_Error_Char;

begin

    myStatus := 0;

    status := PCI_IfkRead(Cardauswahl,
                          IFKAdress,
                          FunktionCode,
                          data,
                          myStatus);// An IFK senden

     // bei fehlerfall in errorlist schreiben
     if(status <> StatusOK) then begin

        // fehlerindex auswerten
        PCI_ErrorString(status, Error_Char);

        // fehler-counter erhoehen
        errorcount := StrToInt(Form4.Panel18.Caption);
        errorcount := errorcount +1;

        // ausgabe
        Form4.Panel18.Caption := IntToStr(errorcount);

        // in fehler in liste aufnehemen
        with ErrorList do begin
             Add (Error_Char);
        end;
      end;

    // counter erhoehen und ausgabe
    counter := StrToInt(Form4.Panel3.Caption);
    counter := counter + 1;
    Form4.Panel3.Caption := IntToStr(counter);

end;

// bereitet die daten fuer die ausgabe felder auf
procedure decode (data:string; var data1:string; var data2:string);

var indexq:integer;
    index :integer;

begin
     index := 1;
     data1 := '';
     data2 := '';

     while index <= 16 do
     begin

          for indexq:= 1 to 4 do
          begin
               if index <= 8 then data1 := data1 + data[index]
               else data2 := data2 + data[index];
               index := index + 1;
          end;

          if index <= 8 then data1 := data1 + ' '
          else if index > 9 then data2 := data2 + ' '

       end;
end;


procedure ReadStatus(var ErrorList: TStrings);

var IFKAdress:Byte;
    dataC0:integer;
    dataC1:integer;
    dataC2:integer;

    FctCode1:Byte;
    FctCode2:Byte;
    FctCode3:Byte;

    dataC0Bin:String;
    dataC1Bin:String;
    dataC2Bin:String;

    dataAusBin1:String;
    dataAusBin2:String;

begin

    FctCode1 := $C0; // Status
    FctCode2 := $C1;
    FctCode3 := $C2;

    try
       // ifk-adresse auslesen
       IFKAdress := StrToInt('$'+Form4.ComboBox1.Text);
    except
      Form4.SpeedButton1.Down := False;
      messagebox(0,'Keine IFK angewählt !','Warnung',16);
      raise;
    end;
    
    // Status C0 bis C2 lesen
    ReadFromIfk(IFKAdress, FctCode1, dataC0, ErrorList);
    ReadFromIfk(IFKAdress, FctCode2, dataC1, ErrorList);
    ReadFromIfk(IFKAdress, FctCode3, dataC2, ErrorList);

    // errorliste ausgeben
    Form4.ListBox1.Items := ErrorList;

    // ausgabe hex
    Form4.Panel14.Caption := IntToHex(dataC0,4);
    Form4.Panel15.Caption := IntToHex(dataC1,4);
    Form4.Panel16.Caption := IntToHex(dataC2,4);

    // status umwandeln in bin
    dataC0Bin := IntToBinary(dataC0,16);
    dataC1Bin := IntToBinary(dataC1,16);
    dataC2Bin := IntToBinary(dataC2,16);

    // aufbereiten der ausgabe
    decode(dataC0Bin, dataAusBin1, dataAusBin2);

    // ausgabe
    Form4.Panel8.Caption := dataAusBin1;
    Form4.Panel9.Caption := dataAusBin2;

   // aufbereiten der ausgabe
    decode(dataC1Bin, dataAusBin1, dataAusBin2);

    // ausgabe
    Form4.Panel11.Caption := dataAusBin1;
    Form4.Panel10.Caption := dataAusBin2;

      // aufbereiten der ausgabe
    decode(dataC2Bin, dataAusBin1, dataAusBin2);

    // ausgabe
    Form4.Panel12.Caption := dataAusBin1;
    Form4.Panel13.Caption := dataAusBin2;

end;


procedure TForm4.Button1Click(Sender: TObject);

begin
     // Step Button
     Form4.SpeedButton1.Down := False;

     ReadStatus(ErrorList);
end;

procedure TForm4.Button2Click(Sender: TObject);

var NullData:String;

begin
     NullData := '0000 0000';

     // Close Button
     Form4.SpeedButton1.Down := False;

    // ausgabe counter null- setzen
     Panel3.Caption := '0';

     // ausgabe error counter null- setzen
     Panel18.Caption := '0';

     // errorliste löschen
     ErrorList.Clear;
     Form4.ListBox1.Items := ErrorList;

     // Bitfelder ruecksetzen
     Form4.Panel8.Caption  := NullData;
     Form4.Panel9.Caption  := NullData;
     Form4.Panel11.Caption := NullData;
     Form4.Panel10.Caption := NullData;
     Form4.Panel12.Caption := NullData;
     Form4.Panel13.Caption := NullData;

     // Hex felder loeschen
     Form4.Panel14.Caption := '0';
     Form4.Panel15.Caption := '0';
     Form4.Panel16.Caption := '0';
     // Fenster schliessen
     Close
end;

procedure TForm4.Button3Click(Sender: TObject);
begin
     // Clear Button

     //loeschen der box
     Form4.ListBox1.Items.Clear;

     // error liste loeschen
     ErrorList.Clear;

     // ausgabe error counter loeschen
     Panel18.Caption := '0';

end;

procedure TForm4.FormCreate(Sender: TObject);

var x,y :integer;

begin
     Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form4.height := (form4.clientheight * y div ScreenHeightDev)+
                           form4.height - form4.ClientHeight;

          form4.width  := (form4.clientwidth * x div ScreenWidthDev)+
                           form4.Height - form4.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     // Auswahl aktuell.
     ComboBox1.Items := Form1.IFKOnline;
     ComboBox1.ItemIndex := 0;

     // ausgabe counter null- setzen
     Panel3.Caption := '0';

     // ausgabe error counter null- setzen
     Panel18.Caption := '0';

     // errorliste erzeugen
     ErrorList := TStringList.Create;
end;

procedure TForm4.SpeedButton1Click(Sender: TObject);
begin
     //Loop Button
end;

procedure TForm4.Timer1Timer(Sender: TObject);
begin
   if (Form4.SpeedButton1.Down = True) then begin
      ReadStatus(ErrorList);
   end;

end;

procedure TForm4.Button5Click(Sender: TObject);

var CountFound:Integer;

begin
    // IFK- Liste loeschen
    Form1.IFKOnline.Clear;
    IFKFound(Form1.IFKOnline, CountFound); // IFKs suchen
    ComboBox1.Items := Form1.IFKOnline;
    ComboBox1.ItemIndex := 0;
end;

end.

