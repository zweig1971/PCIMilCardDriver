unit Info;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    Panel1: TPanel;
    Image1: TImage;
    ButtonOK: TButton;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    procedure ButtonOKClick(Sender: TObject);
  private
    { Private-Deklarationen }
  public
    { Public-Deklarationen }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}


procedure TForm1.ButtonOKClick(Sender: TObject);
begin
  Form1.Close;
end;

end.
