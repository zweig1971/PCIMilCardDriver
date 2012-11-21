unit Unit_PCICardSearch;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  UnitMil, Dialogs, ExtCtrls, StdCtrls;

type
  TForm_PCICardSearch = class(TForm)
    Label1: TLabel;
    procedure FormCreate(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Form_PCICardSearch: TForm_PCICardSearch;

implementation

uses MilMag;
{$R *.dfm}

procedure TForm_PCICardSearch.FormCreate(Sender: TObject);

var CardCounter,i :integer;
    status:_DWORD;
    index:integer;
    Error_Char:t_Error_Char;
    Error_string:string;

begin
     status := 0;

     Form_PCICardSearch.Show;
     Form_PCICardSearch.Repaint;

     // PCI- Karten zaehlen
     CardCounter := PCI_PCIcardCount();

     // gibt es gar keine -> tschuess
     if(CardCounter = 0) then begin
           Application.MessageBox('NO PCI- CARDS FOUND', 'ABBRUCH !', 16);
           Form_PCICardSearch.Close;
     end;

     if(CardCounter = 1) then begin
         Cardauswahl := 1;
         Form_PCICardSearch.Label1.Caption := 'FOUND ONE PCI_CARD...';
         Form_PCICardSearch.Update;

         // öffnen der PCI Mil Karte
         status := PCI_DriverOpen(Cardauswahl);
         if(status <> StatusOK) then begin
            PCI_ErrorString(status, Error_Char); // ausgabe bei fehlerfall
            for i := 1 to Length(Error_Char) do begin
                Error_string := Error_string + Error_Char[i];
            end;
            Application.MessageBox(PChar(Error_string), 'ABBRUCH !', 16);
            Form_PCICardSearch.Close;
         end else begin
            Form_PCICardSearch.Visible := FALSE;
            Application.CreateForm(TForm_MilMag, Form_MilMag);
            Application.Run;
            Form_MilMag.Show;
         end;//if(status <> StatusOK) then begin
      end else begin // Es sind mehrere pci-karten da !
        Form_PCICardSearch.Label1.Caption := 'FOUND MORE PCI_CARD...CHECKING';
        Form_PCICardSearch.Update;
        for index:= 1 to CardCounter do begin // Mache alle ducrh
          status := PCI_DriverOpen(index);    // oeffne
          if(status <> StatusOK) then begin
             Form_PCICardSearch.Label1.Caption := 'OPEN PCI_CARD...';
             Form_PCICardSearch.Update;
          end else begin
             Form_PCICardSearch.Label1.Caption := 'OPEN PCI_CARD...OK'; // alles klar eine gefunden
             Form_PCICardSearch.Update;
             Cardauswahl := index;
             Application.CreateForm(TForm_MilMag, Form_MilMag);
             Form_PCICardSearch.Visible := FALSE;
             Application.Run;
             Form_MilMag.Show;
             break;
          end;//if(status <> StatusOK)
        end;// for index:= 1 to CardCounter do begin
        if(status <> StatusOK) then begin
           Application.MessageBox('NO MASTER CARD FOUND', 'ABBRUCH !', 16);
           Form_PCICardSearch.Close;
        end;
      end; // if(CardCounter = 1) then begin
   end;
end.
