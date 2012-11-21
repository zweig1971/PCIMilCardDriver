unit Unit1;

interface

uses
  Windows, UnitMil, SysUtils, Forms, StdCtrls;

 type
  TMilMag = class
  private
  public
   procedure ReadFromIfk(IFKAdress:Byte; FunktionCode:Byte; var data:WORD);
   procedure decode (data:string; var data1:string; var data2:string);
   procedure Write_Real_10V_Bipol(Data: Word; var DataBits:string);
   procedure Write_Int_10V_Bipol(DataDec:Real; var DataStr:string);
   procedure ReadIFKStatus(IFKAdress:Byte; Form_MilMag:TForm);
   procedure ReadIW1_IW2(IFKAdress:Byte);
   procedure SendFctToIfk(IFKAdress:Byte; FunktionCode:Byte);
   procedure WriteToIFW_SW(HexDecIn:boolean);
  end;

implementation

 //************************ IFK STATUS AUSLESEN *******************

procedure TMilMag.ReadFromIfk(IFKAdress:Byte; FunktionCode:Byte; var data:WORD);

var myStatus:DWORD;

begin
    PCI_IfkRead(Cardauswahl, IFKAdress, FunktionCode, data, myStatus);// An IFK senden
end;

// bereitet die daten fuer die ausgabe felder auf
procedure TMilMag.decode (data:string; var data1:string; var data2:string);

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
procedure TMilMag.Write_Real_10V_Bipol(Data: Word; var DataBits:string);

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

procedure TMilMag.Write_Int_10V_Bipol(DataDec:Real; var DataStr:string);

var data:Real;

begin
     data := DataDec/(305.1757813/1000000);
     Str(data:4:0, DataStr);
end;


procedure TMilMag.ReadIFKStatus(IFKAdress:Byte; Form_MilMag:TComboBox.Text);

var dataC0:WORD;
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
procedure TMilMag.ReadIW1_IW2(IFKAdress:Byte);

var myStatus:DWORD;
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
procedure TMilMag.SendFctToIfk(IFKAdress:Byte; FunktionCode:Byte);

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

procedure TMilMag.WriteToIFW_SW(HexDecIn:boolean);

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




end.
