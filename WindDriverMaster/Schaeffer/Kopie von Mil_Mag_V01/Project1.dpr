program Project1;

uses
  Forms,
  Unit_PCICardSearch in 'Unit_PCICardSearch.pas' {Form_PCICardSearch},
  MilMag in 'MilMag.pas' {Form_MilMag},
  UnitMil in '..\..\EchoTestII\UnitMil.pas',
  Info in 'Info.pas' {Form1},
  Unit1 in 'Unit1.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'MilMag';
  Application.CreateForm(TForm_PCICardSearch, Form_PCICardSearch);
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
