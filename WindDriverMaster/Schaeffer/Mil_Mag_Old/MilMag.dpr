program MilMag;

uses
  Forms,
  Mil_Mag in 'Mil_Mag.pas' {Form1},
  Unit1 in 'Unit1.pas',
  Unit2 in 'Unit2.pas' {Form2},
  Unit3 in 'Unit3.pas' {Form3},
  Unit4 in 'Unit4.pas' {Form4},
  Unit6 in 'Unit6.pas' {Form6},
  Unit8 in 'Unit8.pas' {Form8};

{$R *.RES}
begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm3, Form3);
  Application.CreateForm(TForm4, Form4);
  Application.CreateForm(TForm6, Form6);
  Application.CreateForm(TForm8, Form8);
  Application.Run;
end.
