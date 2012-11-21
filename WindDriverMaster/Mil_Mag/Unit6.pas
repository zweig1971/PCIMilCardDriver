unit Unit6;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, Unit1;

type
  TForm6 = class(TForm)
    Panel1: TPanel;
    Panel2: TPanel;
    Label1: TLabel;
    Panel3: TPanel;
    Panel4: TPanel;
    Label2: TLabel;
    ComboBox1: TComboBox;
    Panel5: TPanel;
    Label3: TLabel;
    Label4: TLabel;
    Edit1: TEdit;
    GroupBox1: TGroupBox;
    Button1: TButton;
    Button2: TButton;
    Label5: TLabel;
    Panel7: TPanel;
    Panel8: TPanel;
    Label6: TLabel;
    Panel9: TPanel;
    GroupBox2: TGroupBox;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Panel10: TPanel;
    Panel11: TPanel;
    Panel12: TPanel;
    GroupBox3: TGroupBox;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Panel13: TPanel;
    Panel14: TPanel;
    Panel15: TPanel;
    Button3: TButton;
    SpeedButton1: TSpeedButton;
    Button4: TButton;
    GroupBox4: TGroupBox;
    Button5: TButton;
    Button6: TButton;
    Edit2: TEdit;
    Button8: TButton;
    Button7: TButton;
    Timer1: TTimer;
    Edit3: TEdit;
    Edit4: TEdit;
    procedure Button4Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure Edit2KeyPress(Sender: TObject; var Key: Char);
    procedure Edit4KeyPress(Sender: TObject; var Key: Char);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
    SchrittwStr: TStrings;
  end;

var
  Form6: TForm6;

const
     Fct_Wr_Sw1     = $06;
     Fct_Wr_Sw2     = $07;
     Fct_Rd_Ist1    = $81;   {Lese ADC-Wert}
     Fct_Rd_Ist2    = $82;
     Fct_Start_Conv = $5F;
     ADC_Conv_Wait  = 20;

implementation

uses Mil_Mag, Unit8;

//uses Unit4, Mil_Mag;

{$R *.DFM}

procedure WriteErrorList(status:integer);

var Error_Char:t_Error_Char;
    errorcount:integer;

begin
    // Fehler auswertung
     if(status <> StatusOK) then begin

        // fehlerindex auswerten
        PCI_ErrorString(status, Error_Char);

        // fehler-counter erhoehen
        errorcount := StrToInt(Form8.Panel4.Caption);
        errorcount := errorcount +1;

        // ausgabe
        Form8.Panel4.Caption := IntToStr(errorcount);

        // in fehler in liste aufnehemen
        with Form8.ErrorList do begin
             Add (Error_Char);
        end;

       // errorliste ausgeben
       Form8.ListBox1.Items := Form8.ErrorList;
       end;
end;

procedure set_output_zero();

begin

     Form6.Panel3.Caption  := '0';
     Form6.Panel7.Caption  := '0000 0000 0000 0000';
     Form6.Panel9.Caption  := '0';
     Form6.Panel10.Caption := '0';
     Form6.Panel11.Caption := '0';
     Form6.Panel12.Caption := '0000 0000 0000 0000';
     Form6.Panel13.Caption := '0';
     Form6.Panel14.Caption := '0';
     Form6.Panel15.Caption := '0000 0000 0000 0000';


end;


// bereitet die daten fuer die ausgabe felder auf
procedure decode (data:string; var data1:string);

var indexq:integer;
    index :integer;

begin
     index := 1;
     data1 := '';

     while index <= 16 do
     begin

          for indexq:= 1 to 4 do
          begin
               data1 := data1 + data[index];
               index := index + 1;
          end;

          data1 := data1 + ' '
     end;
end;


// Wandelt die aufloesungsbits in werte um
procedure Write_Real_10V_Bipol(Data: Word; var DataBits:string);

var Real_Zahl : Real;

begin                        //Bit 15 ist Vorzeichen

  DataBits := '';

  if (Data and $8000) = 0 then
   begin     //positiv
     Real_Zahl := Data * 305.1757813/1000000;
    end
  else
   begin     //negativ
     Data := (not Data) + 1;               //Zweier Komplement
     Real_Zahl := (Data * 305.1757813/1000000)*-1;
   end;
   Str(Real_Zahl:9:6, DataBits);

{*   Real_Zahl := Data * (10/$7FFF);
   Str(Real_Zahl:9:6, DataBits); }

end;

procedure Write_Int_10V_Bipol(DataDec:Real; var DataStr:string);

var data:Real;

begin
{
     data := DataDec*($7FFF/10);
     Str(data:4:0, DataStr);
}

     data := DataDec/(305.1757813/1000000);
     Str(data:4:0, DataStr);

end;

procedure Mil_SW_IW();

var IFKAdress:Byte;
    myStatus:integer;
    status:integer;
    IW1:integer;
    IW2:integer;
    counter:integer;

    BinIW1:String;
    BinIW2:String;
    DecBinIW1:String;
    DecBinIW2:String;
    DataBits:string;

begin

    try
       // ifk-adresse auslesen
       IFKAdress := StrToInt('$'+Form6.ComboBox1.Text);
    except
      Form6.SpeedButton1.Down := False;
      messagebox(0,'Keine IFK angewählt !','Warnung',16);
      raise;
    end;

    // counter erhoehen und ausgabe
    counter := StrToInt(Form6.Panel9.Caption);
    counter := counter + 1;
    Form6.Panel9.Caption := IntToStr(counter);

    // als erstes mus die messung gestartet werden
    status := PCI_FctCodeSnd(Cardauswahl,
                             IFKAdress,
                             Fct_Start_Conv,
                             myStatus);

    WriteErrorList(status);

    // Warten
    PCI_TimerWait (Cardauswahl,
                   ADC_Conv_Wait,
                   0,
                   myStatus);


    // Istwert 1 und Istwert 2 zuruecklesen
    status := PCI_IfkRead(Cardauswahl,
                          IFKAdress,
                          Fct_Rd_Ist1,
                          IW1,
                          myStatus);

    WriteErrorList(status);

    status := PCI_IfkRead(Cardauswahl,
                          IFKAdress,
                          Fct_Rd_Ist2,
                          IW2,
                          myStatus);

    WriteErrorList(status);


    // Daten IW1 anzeigen
    // -----------------------

    // Hex anzeige
    Form6.Panel10.Caption := IntToHex(IW1,4);

    // Bin anzeige
    BinIW1 := IntToBinary(IW1,16);
    decode (BinIW1,DecBinIW1);
    Form6.Panel12.Caption := DecBinIW1;
    Write_Real_10V_Bipol(Word(IW1),DataBits);
    Form6.Panel11.Caption := DataBits;


    // Daten IW2 anzeigen
    // -----------------------

    // Hex anzeige
    Form6.Panel13.Caption := IntToHex(IW2,4);

    // Bin anzeige
    BinIW2 := IntToBinary(IW2,16);
    decode (BinIW2,DecBinIW2);
    Form6.Panel15.Caption := DecBinIW2;
    Write_Real_10V_Bipol(Word(IW2),DataBits);
    Form6.Panel14.Caption := DataBits;
end ;


procedure WriteToIFW_SW(HexDecIn:boolean);

var SW:integer;
    SW_real:real;
    status:integer;
    myStatus:integer;
    counter:integer;
    IFKAdress:integer;
    code:integer;
    tempW:word;

    BinSWStr:string;
    DecBinSWStr:string;
    SWBits:string;

begin

   try
       // ifk-adresse auslesen
       IFKAdress := StrToInt('$'+Form6.ComboBox1.Text);
    except
      Form6.SpeedButton1.Down := False;
      messagebox(0,'Keine IFK angewählt !','Warnung',16);
      raise;
    end;

    // Sollwert auslesen entweder aus dem hex oder dec- feld
    if HexDecIn = TRUE then begin
       try
          SW := StrToInt('$'+Form6.Edit1.Text);
       except
          Form6.SpeedButton1.Down := False;
          messagebox(0,'Hex Wert ungültig','Warnung',16);
          raise;
       end;
    end else begin
       try
          VAL(Form6.Edit2.Text, SW_real, code);
       except
          Form6.SpeedButton1.Down := False;
          messagebox(0,'Dec Wert ungültig','Warnung',16);
          raise;
       end;
    end;

    // Hex umwandeln und in Dec ausgeben
    if HexDecIn = TRUE then begin
       Write_Real_10V_Bipol(Word(SW),SWBits);
       Form6.Edit2.Text := SWBits;
    end else begin
//       SW := round(SW_real*(32767.0/10.0)); //reele zahl in hex umrechnen
//       Form6.Edit1.Text := IntToHex(SW,4); // ausgabe text in hex


       Write_Int_10V_Bipol(SW_real, SWBits);//umwandeln von real in dec_string
       tempW := StrToInt(SWBits); //umwandeln von dec_string nach hex_int
       Form6.Edit1.Text := IntToHex(tempW,4); // ausgabe text in hex
       SW := StrToInt('$'+Form6.Edit1.Text);// umwandeln string nach int
       Write_Real_10V_Bipol(Word(SW),SWBits);// umrechnen des hex in dec.
       Form6.Edit2.Text := SWBits;// ausgabe des dec wert}
    end;

    // Bin anzeige
    BinSWStr := IntToBinary(SW,16);
    decode (BinSWStr,DecBinSWStr);
    Form6.Panel7.Caption := DecBinSWStr;

    // counter erhoehen und ausgabe
    counter := StrToInt(Form6.Panel3.Caption);
    counter := counter + 1;
    Form6.Panel3.Caption := IntToStr(counter);


    // Sollwerte SW1 und SW2 versenden
    status := PCI_IfkWrite(Cardauswahl,
                           IFKAdress,
                           Fct_Wr_Sw1,
                           SW,
                           myStatus);

    WriteErrorList(status);


    // Sollwerte versenden
    status := PCI_IfkWrite(Cardauswahl,
                           IFKAdress,
                           Fct_Wr_Sw2,
                           SW,
                           myStatus);

    WriteErrorList(status);
end;


procedure TForm6.Button4Click(Sender: TObject);

begin
     // STEP-Button
     // LOOP- Aus
     Form6.SpeedButton1.Down := False;
     Mil_SW_IW();
end;


procedure TForm6.Button3Click(Sender: TObject);
begin
     Form6.SpeedButton1.Down := False;
     Close
end;


procedure TForm6.FormCreate(Sender: TObject);

var x,y :integer;

begin
     Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form6.height := (form6.clientheight * y div ScreenHeightDev)+
                           form6.height - form6.ClientHeight;

          form6.width  := (form6.clientwidth * x div ScreenWidthDev)+
                           form6.Height - form6.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;

     // Auswahl aktuell.
     ComboBox1.Items := Form1.IFKOnline;
     ComboBox1.ItemIndex := 0;

     // ausgabe counter null- setzen
     Panel3.Caption := '0';

     // sonstige ausgaben auf null setzen
     set_output_zero();

     // liste fuer die schrittweite erstellen
     SchrittwStr := TStringList.Create;
     SchrittwStr.Clear;
end;


procedure TForm6.Button8Click(Sender: TObject);

var CountFound:Integer;

begin
    // IFK- Liste loeschen
    Form1.IFKOnline.Clear;
    IFKFound(Form1.IFKOnline, CountFound); // IFKs suchen
    ComboBox1.Items := Form1.IFKOnline;
    ComboBox1.ItemIndex := 0;
end;


procedure TForm6.Button7Click(Sender: TObject);
begin
     Form8.Show
end;


procedure TForm6.Timer1Timer(Sender: TObject);
begin
   if (Form6.SpeedButton1.Down = True) then begin
       Mil_SW_IW();
   end;
end;


procedure TForm6.Edit1KeyPress(Sender: TObject; var Key: Char);
begin

   if key = #13 then begin
      key:= #0;
      WriteToIFW_SW(TRUE);
   end;
end;


procedure TForm6.Edit2KeyPress(Sender: TObject; var Key: Char);
begin
   if key = ',' then begin
      key:= '.';
   end else if key = #13 then begin
      key:= #0;
      WriteToIFW_SW(FALSE);
   end;
end;


procedure TForm6.Edit4KeyPress(Sender: TObject; var Key: Char);
begin
   if key = ',' then begin
      key:= '.';
   end else if key = #13 then begin
      key:= #0;
   end;
end;

//Eingegebener wird mit mit rel-wert add
procedure TForm6.Button5Click(Sender: TObject);

var SW_real:real;
    SW_real_p:real;
    code :integer;

    SW_String:string;

begin

    VAL(Form6.Edit2.Text, SW_real, code);
    VAL(Form6.Edit4.Text, SW_real_p, code);

    if code <> 0 then begin
       Form6.SpeedButton1.Down := False;
       messagebox(0,'Wert ungültig','Warnung',16);
       Form6.Edit4.Text := '0';
    end;

     SW_real := SW_real + SW_real_p;
     Str(SW_real:9:6,SW_String);
     Form6.Edit2.Text := SW_String;
     WriteToIFW_SW(FALSE);
end;

// Eingegebener wird mit mit rel-wert sub
procedure TForm6.Button6Click(Sender: TObject);

var SW_real:real;
    SW_real_p:real;
    code :integer;

    SW_String:string;

begin
     VAL(Form6.Edit2.Text, SW_real, code);
     VAL(Form6.Edit4.Text, SW_real_p, code);

     if code <> 0 then begin
        Form6.SpeedButton1.Down := False;
        messagebox(0,'Wert ungültig','Warnung',16);
        Form6.Edit4.Text := '0';
     end;

     SW_real := SW_real - SW_real_p;
     Str(SW_real:9:6,SW_String);
     Form6.Edit2.Text := SW_String;
     WriteToIFW_SW(FALSE);
end;

// Eingegebener wird mit mit hex-wert add
procedure TForm6.Button1Click(Sender: TObject);

var SW:integer;
    SW_add:integer;

begin
     try
        SW     := StrToInt('$'+Form6.Edit1.Text);
        SW_add := StrToInt('$'+Form6.Edit3.Text);
     except
          Form6.SpeedButton1.Down := False;
          messagebox(0,'Wert ungültig','Warnung',16);
          Form6.Edit1.Text := '1';
          Form6.Edit3.Text := '0';
          raise;
     end;

     SW := SW + SW_add;
     Form6.Edit1.Text := IntToHex(SW,4);
     WriteToIFW_SW(TRUE);
end;


// Eingegebener wert wird mit hex-wert sub.
procedure TForm6.Button2Click(Sender: TObject);

var SW:integer;
    SW_add:integer;

begin
     try
        SW     := StrToInt('$'+Form6.Edit1.Text);
        SW_add := StrToInt('$'+Form6.Edit3.Text);
     except
          Form6.SpeedButton1.Down := False;
          messagebox(0,'Wert ungültig','Warnung',16);
          Form6.Edit1.Text := '1';
          Form6.Edit3.Text := '0';
          raise;
     end;

     SW := SW - SW_add;
     Form6.Edit1.Text := IntToHex(SW,4);
     WriteToIFW_SW(TRUE);
end;

end.
