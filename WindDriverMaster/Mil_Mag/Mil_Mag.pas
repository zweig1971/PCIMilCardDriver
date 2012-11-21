unit Mil_Mag;

interface
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, Unit1;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    UPDATE: TButton;
    Panel1: TPanel;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Panel2: TPanel;
    ListBox1: TListBox;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure UPDATEClick(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
    IFKOnline: TStrings
  end;

var
  Form1: TForm1;
  
implementation

uses Unit2, Unit3, Unit4, Unit6, Unit8;

{$R *.DFM}

procedure TForm1.Button1Click(Sender: TObject);
begin
     PCI_DriverClose(1);
     Close
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
     Form2.Show
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
     Form3.Show
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
     Form4.Show
end;

procedure TForm1.Button5Click(Sender: TObject);
begin
     Form6.Show
end;

procedure TForm1.FormCreate(Sender: TObject);

var x,y :integer;
    status :integer;
    CardCounter :integer;
    Error_String :string;
    Error_Char:t_Error_Char;

begin
     // Skalierung des fensters
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

     // PCI- Karten zaehlen
     CardCounter := PCI_PCIcardCount();

     // gibt es mehr als 1 -> auswahl anbieten
     //if(CardCounter > 1) then begin
       //    Form8.Show;
     //end;

     // gibt es gar keine -> tschuess
     if(CardCounter = 0) then begin
           Application.MessageBox('NO PCI- CARDS FOUND', 'ABBRUCH !', 16);
           Form1.Close;
     end;

     // öffnen der PCI Mil Karte
      status := PCI_DriverOpen(Cardauswahl);
     if(status <> StatusOK) then begin
        PCI_ErrorString(status, Error_Char); // ausgabe bei fehlerfall
        Error_String := Error_Char;
        Application.MessageBox(PChar(Error_String), 'ABBRUCH !', 16);
        Form1.Close;
     end;

     // Update wird gedrueckt
     Form1.UPDATE.Click;
   end;

procedure TForm1.UPDATEClick(Sender: TObject);

 var CountFound:integer;

begin
    IFKOnline := TStringList.Create;
    IFKFound(IFKOnline, CountFound); // IFKs suchen
    ListBox1.Items :=IFKOnline;
    Panel2.Caption := IntToStr(CountFound);
end;

end.
