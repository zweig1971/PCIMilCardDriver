unit MilMag;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  UnitMil,Dialogs, StdCtrls, ExtCtrls, Menus;

type
  TForm_MilMag = class(TForm)
    GroupBoxIFKamBus: TGroupBox;
    GroupBoxIFKStatus: TGroupBox;
    GroupBoxSendFCTCode: TGroupBox;
    GroupBoxSollDataWrite: TGroupBox;
    PanelSendFCTC: TPanel;
    PanelSendFCTHex: TPanel;
    Label3: TLabel;
    InputSendFCTCode: TEdit;
    Label4: TLabel;
    ButtonReset: TButton;
    ButtonEin: TButton;
    ButtonAus: TButton;
    Panel2: TPanel;
    Label5: TLabel;
    PanelFktAdress: TPanel;
    Panel3: TPanel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    PanelStatC0MSB: TPanel;
    PanelStatC1MSB: TPanel;
    PanelStatC2MSB: TPanel;
    PanelAdrC0LSB: TPanel;
    PanelAdrC1LSB: TPanel;
    PanelAdrC2LSB: TPanel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    PanelDataC0: TPanel;
    PanelDataC1: TPanel;
    PanelDataC2: TPanel;
    Label13: TLabel;
    Label14: TLabel;
    Panel14: TPanel;
    GroupBox3: TGroupBox;
    GroupBox4: TGroupBox;
    Label17: TLabel;
    Label18: TLabel;
    PanelVoltHexIW1: TPanel;
    PanelVoltDecIW1: TPanel;
    Label19: TLabel;
    Label20: TLabel;
    PanelVoltHexIw2: TPanel;
    PanelVoltDecIW2: TPanel;
    ListBoxIFK: TListBox;
    Panel1: TPanel;
    Label1: TLabel;
    ComboBoxActIFK: TComboBox;
    ButtonExit: TButton;
    Panel11: TPanel;
    Panel15: TPanel;
    Label15: TLabel;
    Label16: TLabel;
    EditVolthex: TEdit;
    EditVoltdec: TEdit;
    GroupBox1: TGroupBox;
    ButtonVoltHexPlus: TButton;
    EditVoltHexStep: TEdit;
    ButtonVoltHexMinus: TButton;
    GroupBox2: TGroupBox;
    ButtonVoltDecPlus: TButton;
    EditVoltDecStep: TEdit;
    ButtonVoltDecMinus: TButton;
    ButtonSendFctCode: TButton;
    ButtonSendSollW: TButton;
    TimerIFKamBus: TTimer;
    PanelAnzahlIFK: TPanel;
    TimerStatIW1_IW2: TTimer;
    MainMenu1: TMainMenu;
    Info1: TMenuItem;
    Info2: TMenuItem;
    ButtonEinP: TButton;
    ButtonAusP: TButton;
    procedure ButtonAusPClick(Sender: TObject);
    procedure ButtonEinPClick(Sender: TObject);
    procedure Info2Click(Sender: TObject);
    procedure FormMilMagClose(Sender: TObject; var Action: TCloseAction);
    procedure ButtonSendSollWClick(Sender: TObject);
    procedure EditVoltDecSKP(Sender: TObject; var Key: Char);
    procedure EditVoltDecKP(Sender: TObject; var Key: Char);
    procedure EditVoltHexKP(Sender: TObject; var Key: Char);
    procedure ButtonVoltDecMinusClick(Sender: TObject);
    procedure ButtonVoltDecPlusClick(Sender: TObject);
    procedure ButtonVoltHexMinusClick(Sender: TObject);
    procedure ButtonVoltHexPlusClick(Sender: TObject);
    procedure ButtonSendFctCodeClick(Sender: TObject);
    procedure ButtonAusClick(Sender: TObject);
    procedure ButtonEinClick(Sender: TObject);
    procedure ButtonResetClick(Sender: TObject);
    procedure ButtonExitClick(Sender: TObject);
    procedure TimerStatIW1_IW2Timer(Sender: TObject);
    procedure TimerIFKamBusTimer(Sender: TObject);

    procedure FormCreate(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
    IFKOnline: TStrings;
  end;

var
  Form_MilMag: TForm_MilMag;

const
     Fct_Wr_Sw1     = $06;
     Fct_Wr_Sw2     = $07;
     Fct_Rd_Ist1    = $81;   {Lese ADC-Wert}
     Fct_Rd_Ist2    = $82;
     Fct_Start_Conv = $5F;
     ADC_Conv_Wait  = 20;


implementation

uses Unit_PCICardSearch, Info;

{$R *.dfm}

//************************ IFK STATUS AUSLESEN *******************

procedure ReadFromIfk(IFKAdress:Byte; FunktionCode:Byte;
                      var data:WORD);

var myStatus:DWORD;

begin
    PCI_IfkRead(Cardauswahl, IFKAdress, FunktionCode, data, myStatus);// An IFK senden
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
     data := DataDec/(305.1757813/1000000);
     Str(data:4:0, DataStr);
end;


procedure ReadIFKStatus();

var IFKAdress:Byte;
    dataC0:WORD;
    dataC1:WORD;
    dataC2:WORD;

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
       IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
    except
//      messagebox(0,'Keine IFK angewählt !','Warnung',16);
      raise;
    end;

    // Status C0 bis C2 lesen
    ReadFromIfk(IFKAdress, FctCode1, dataC0);
    ReadFromIfk(IFKAdress, FctCode2, dataC1);
    ReadFromIfk(IFKAdress, FctCode3, dataC2);

    // ausgabe hex
    Form_MilMag.PanelDataC0.Caption := IntToHex(dataC0,4);
    Form_MilMag.PanelDataC1.Caption := IntToHex(dataC1,4);
    Form_MilMag.PanelDataC2.Caption := IntToHex(dataC2,4);

    // status umwandeln in bin
    dataC0Bin := IntToBinary(dataC0,16);
    dataC1Bin := IntToBinary(dataC1,16);
    dataC2Bin := IntToBinary(dataC2,16);

    // aufbereiten der ausgabe
    decode(dataC0Bin, dataAusBin1, dataAusBin2);

    // ausgabe
    Form_MilMag.PanelStatC0MSB.Caption := dataAusBin1;
    Form_MilMag.PanelAdrC0LSB.Caption  := dataAusBin2;

   // aufbereiten der ausgabe
    decode(dataC1Bin, dataAusBin1, dataAusBin2);

    // ausgabe
    Form_MilMag.PanelStatC1MSB.Caption := dataAusBin1;
    Form_MilMag.PanelAdrC1LSB.Caption  := dataAusBin2;

      // aufbereiten der ausgabe
    decode(dataC2Bin, dataAusBin1, dataAusBin2);

    // ausgabe
    Form_MilMag.PanelStatC2MSB.Caption := dataAusBin1;
    Form_MilMag.PanelAdrC2LSB.Caption  := dataAusBin2;
end;


//************************************************************
// ****************** IW 1 und IW 2 auslesen *****************
procedure ReadIW1_IW2();

var IFKAdress:Byte;
    myStatus:DWORD;
    IW1:WORD;
    IW2:WORD;
    DataBits:string;

begin

    try
       // ifk-adresse auslesen
       IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
    except
 //      messagebox(0,'Keine IFK angewählt !','Warnung',16);
      raise;
    end;

    // als erstes mus die messung gestartet werden
    PCI_FctCodeSnd(Cardauswahl, IFKAdress, Fct_Start_Conv, myStatus);

    // Warten
    PCI_TimerWait (Cardauswahl, ADC_Conv_Wait, 0, myStatus);


    // Istwert 1 und Istwert 2 zuruecklesen
    PCI_IfkRead(Cardauswahl,IFKAdress, Fct_Rd_Ist1, IW1,myStatus);
    PCI_IfkRead(Cardauswahl,IFKAdress, Fct_Rd_Ist2, IW2,myStatus);

    // Daten IW1 anzeigen
    // -----------------------

    // Hex anzeige
    Form_MilMag.PanelVoltHexIW1.Caption := IntToHex(IW1,4);

    // Dez anzeige
    Write_Real_10V_Bipol(Word(IW1),DataBits);
    Form_MilMag.PanelVoltDecIW1.Caption := DataBits;


    // Daten IW2 anzeigen
    // -----------------------

    // Hex anzeige
    Form_MilMag.PanelVoltHexIw2.Caption := IntToHex(IW2,4);

    // Dez anzeige
    Write_Real_10V_Bipol(Word(IW2),DataBits);
    Form_MilMag.PanelVoltDecIW2.Caption := DataBits;


end ;

 //************************************************************
// ****************** FTC senden und in form ausgebene********
procedure SendFctToIfk(IFKAdress:Byte; FunktionCode:Byte);

var myStatus:DWORD;
    SendFunktionCode:integer;

begin
     myStatus := 0;

     PCI_FctCodeSnd(Cardauswahl, IFKAdress,FunktionCode, myStatus);// An IFK senden

     //  ausfct+adress errechnen und ausgabe
     SendFunktionCode := FunktionCode;
     SendFunktionCode := SendFunktionCode shl 8; // 8 bit nach links
     SendFunktionCode := SendFunktionCode + IFKAdress;
     Form_MilMag.PanelFktAdress.Caption := IntToHex(SendFunktionCode,6);
end;

//************************************************************
// ****************** weiss ich noch nicht *******************

procedure WriteToIFW_SW(HexDecIn:boolean);

var SW:integer;
    SW_real:real;
    myStatus:DWORD;
    IFKAdress:integer;
    code:integer;
    tempW:word;

    SWBits:string;

begin

       // ifk-adresse auslesen
       IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);

    // Sollwert auslesen entweder aus dem hex oder dec- feld
    if HexDecIn = TRUE then begin
       try
          SW := StrToInt('$'+Form_MilMag.EditVolthex.Text);
       except
           messagebox(0,'Hex Wert ungültig','Warnung',16);
          raise;
       end;
    end else begin
       try
          VAL(Form_MilMag.EditVoltdec.Text, SW_real, code);
       except
          messagebox(0,'Dec Wert ungültig','Warnung',16);
          raise;
       end;
    end;

    // Hex umwandeln und in Dec ausgeben
    if HexDecIn = TRUE then begin
       Write_Real_10V_Bipol(Word(SW),SWBits);
       Form_MilMag.EditVoltdec.Text := SWBits;
    end else begin
       Write_Int_10V_Bipol(SW_real, SWBits);//umwandeln von real in dec_string
       tempW := StrToInt(SWBits); //umwandeln von dec_string nach hex_int
       Form_MilMag.EditVolthex.Text := IntToHex(tempW,4); // ausgabe text in hex
       SW := StrToInt('$'+Form_MilMag.EditVolthex.Text);// umwandeln string nach int
       Write_Real_10V_Bipol(Word(SW),SWBits);// umrechnen des hex in dec.
       Form_MilMag.EditVoltdec.Text := SWBits;// ausgabe des dec wert}
    end;

    // Sollwerte SW1 und SW2 versenden
    PCI_IfkWrite(Cardauswahl,IFKAdress,Fct_Wr_Sw1,SW,myStatus);

    // Sollwerte versenden
    PCI_IfkWrite(Cardauswahl,IFKAdress,Fct_Wr_Sw2,SW,myStatus);
end;


//************************************************************
procedure TForm_MilMag.FormCreate(Sender: TObject);

var CountFound:Integer;

begin
     // IFK liste erzeugen
     IFKOnline := TStringList.Create;
     IFKOnline.Clear;
     IFKFound(IFKOnline, CountFound); // IFKs suchen
     Form_MilMag.ListBoxIFK.Items := IFKOnline;
     Form_MilMag.PanelAnzahlIFK.Caption := IntToStr(CountFound);
     Form_MilMag.ComboBoxActIFK.Items := IFKOnline;
     Form_MilMag.ComboBoxActIFK.ItemIndex := 0;
     Form_MilMag.Refresh;
     //---------------------------------------------

end;

procedure TForm_MilMag.TimerIFKamBusTimer(Sender: TObject);

var CountFound:integer;

begin
     IFKOnline.Clear;
     IFKFound(IFKOnline, CountFound); // IFKs suchen
     Form_MilMag.ListBoxIFK.Items := IFKOnline;
     Form_MilMag.PanelAnzahlIFK.Caption := IntToStr(CountFound);
     Form_MilMag.ComboBoxActIFK.Items := IFKOnline;
     Form_MilMag.Refresh;
end;

procedure TForm_MilMag.TimerStatIW1_IW2Timer(Sender: TObject);

var IFKAdress:byte;

begin
      IFKAdress := 0;

      if Form_MilMag.ComboBoxActIFK.Text <> '' then begin
        try
          IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
        except
        raise;
        end;

        If IFKAdress <> 0 then begin
          ReadIFKStatus();
          ReadIW1_IW2();
        end;
      end;
end;

procedure TForm_MilMag.ButtonExitClick(Sender: TObject);
begin
//      PCI_DriverClose(Cardauswahl);
      Close
end;



procedure TForm_MilMag.ButtonResetClick(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
      // Reset- Button
      FunktionCode := $01;//reset

      // ifk-adresse auslesen
      IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
      // senden des resets an die ifk
      SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm_MilMag.ButtonEinClick(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
      // EIN- Button
      FunktionCode := $02;//EIN

       // ifk-adresse auslesen
      IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
      // senden des EIN an die ifk
      SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm_MilMag.ButtonAusClick(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
      // EIN- Button
      FunktionCode := $03;//AUS

       // ifk-adresse auslesen
      IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
      // senden des EIN an die ifk
      SendFctToIfk(IFKAdress, FunktionCode);
end;


procedure TForm_MilMag.ButtonSendFctCodeClick(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
      // ifk-adresse auslesen
      IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);

    try
      // fct-code auslesen
      FunktionCode := StrToInt('$'+Form_MilMag.InputSendFCTCode.Text);
    except
      messagebox(0,'Ungültige Eingabe ! Nur HEX zulässig !','Warnung',16);
      raise;
    end;
    // versenden
    SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm_MilMag.ButtonVoltHexPlusClick(Sender: TObject);

var SW:integer;
    SW_add:integer;

begin
     try
         SW     := StrToInt('$'+Form_MilMag.EditVolthex.Text);
         SW_add := StrToInt('$'+Form_MilMag.EditVolthexStep.Text);
     except
          messagebox(0,'Wert ungültig','Warnung',16);
          raise;
     end;

     SW := SW + SW_add;
     Form_MilMag.EditVolthex.Text := IntToHex(SW,4);
     WriteToIFW_SW(TRUE);
     ReadIW1_IW2();
     ReadIFKStatus();
end;


procedure TForm_MilMag.ButtonVoltHexMinusClick(Sender: TObject);

var SW:integer;
    SW_add:integer;

begin
     try
        SW     := StrToInt('$'+Form_MilMag.EditVolthex.Text);
        SW_add := StrToInt('$'+Form_MilMag.EditVolthexStep.Text);
     except
          messagebox(0,'Wert ungültig','Warnung',16);
          raise;
     end;

     SW := SW - SW_add;
     Form_MilMag.EditVolthex.Text := IntToHex(SW,4);
     WriteToIFW_SW(TRUE);
     ReadIW1_IW2();
     ReadIFKStatus();
end;


procedure TForm_MilMag.ButtonVoltDecPlusClick(Sender: TObject);

var SW_real:real;
    SW_real_p:real;
    code :integer;
    SW_String:string;

begin
    VAL(Form_MilMag.EditVoltdec.Text, SW_real, code);
    VAL(Form_MilMag.EditVoltDecStep.Text, SW_real_p, code);

    if code <> 0 then begin
       messagebox(0,'Wert ungültig','Warnung',16);
    end else begin
       SW_real := SW_real + SW_real_p;
       Str(SW_real:9:6,SW_String);
       Form_MilMag.EditVoltdec.Text := SW_String;
       WriteToIFW_SW(FALSE);
       ReadIW1_IW2();
       ReadIFKStatus();
    end;
end;


procedure TForm_MilMag.ButtonVoltDecMinusClick(Sender: TObject);

var SW_real:real;
    SW_real_p:real;
    code :integer;
    SW_String:string;

begin
     VAL(Form_MilMag.EditVoltdec.Text, SW_real, code);
     VAL(Form_MilMag.EditVoltDecStep.Text, SW_real_p, code);

     if code <> 0 then begin
        messagebox(0,'Wert ungültig','Warnung',16);
     end else begin
       SW_real := SW_real - SW_real_p;
       Str(SW_real:9:6,SW_String);
       Form_MilMag.EditVoltdec.Text := SW_String;
       WriteToIFW_SW(FALSE);
       ReadIW1_IW2();
       ReadIFKStatus();
     end;
end;

procedure TForm_MilMag.EditVoltHexKP(Sender: TObject; var Key: Char);
begin
   if key = #13 then begin
      key:= #0;
      WriteToIFW_SW(TRUE);
      ReadIW1_IW2();
      ReadIFKStatus();
   end;
end;


procedure TForm_MilMag.EditVoltDecKP(Sender: TObject; var Key: Char);
begin
   if key = ',' then begin
      key:= '.';
   end else if key = #13 then begin
      key:= #0;
      WriteToIFW_SW(FALSE);
      ReadIW1_IW2();
      ReadIFKStatus();
   end;
end;

procedure TForm_MilMag.EditVoltDecSKP(Sender: TObject; var Key: Char);
begin
    if key = ',' then begin
      key:= '.';
   end else if key = #13 then begin
      key:= #0;
   end;
end;

procedure TForm_MilMag.ButtonSendSollWClick(Sender: TObject);
begin
    WriteToIFW_SW(TRUE);
    ReadIW1_IW2();
    ReadIFKStatus();
end;

procedure TForm_MilMag.FormMilMagClose(Sender: TObject;
  var Action: TCloseAction);
begin
      TimerStatIW1_IW2.Enabled := False;
      TimerIFKamBus.Enabled := False;
      PCI_DriverClose(Cardauswahl);
      Form_PCICardSearch.Close;
end;

procedure TForm_MilMag.Info2Click(Sender: TObject);
begin
  Application.CreateForm(TForm1, Form1);
  Form1.Show;
end;

procedure TForm_MilMag.ButtonEinPClick(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
      // EIN- Button
      FunktionCode := $14;//EIN

       // ifk-adresse auslesen
      IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
      // senden des EIN an die ifk
      SendFctToIfk(IFKAdress, FunktionCode);
end;

procedure TForm_MilMag.ButtonAusPClick(Sender: TObject);

var IFKAdress:Byte;
    FunktionCode:Byte;

begin
      // EIN- Button
      FunktionCode := $15;//AUS

       // ifk-adresse auslesen
      IFKAdress := StrToInt('$'+Form_MilMag.ComboBoxActIFK.Text);
      // senden des EIN an die ifk
      SendFctToIfk(IFKAdress, FunktionCode);
end;

end.
