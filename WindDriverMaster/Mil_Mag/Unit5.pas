unit Unit5;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Unit1;

type
  TForm5 = class(TForm)
    GroupBox3: TGroupBox;
    Label11: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Form5: TForm5;

implementation

{$R *.DFM}

procedure TForm5.Button1Click(Sender: TObject);
begin
     Close
end;

procedure TForm5.FormCreate(Sender: TObject);

var x,y :integer;

begin
     Scaled := true;
     x := Screen.Width;
     y := Screen.Height;

     if(y <> ScreenHeightDev) or (x <> ScreenWidthDev) then begin
          form5.height := (form5.clientheight * y div ScreenHeightDev)+
                           form5.height - form5.ClientHeight;

          form5.width  := (form5.clientwidth * x div ScreenWidthDev)+
                           form5.Height - form5.clientheight;

          scaleBy(x, ScreenwidthDev);
     end;
end;

end.
