//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// UnitMil.pas
//
// Autor           : Zweig,Marcus
// Version         : 1.05
// letzte Änderung : 02.08.03
//------------------------------

unit UnitMil;

interface

uses
  Classes, SysUtils;

var IFKNr : integer;
    Cardauswahl :integer;
    error : Boolean;
    IFKOnline: TStrings;

{type  (DataSend,CommandSend); }
type _TimerBase  = (Timerbase_10us, Timerbase_1ms);

type t_Error_Char = array [1..25] of char;


const

{ Bitmasken für status_1 }
Interlock          = $1;
Dta_Rdy		   = $2;
Dta_Req		   = $4;
Trm_Rdy		   = $8;
Mil_Rcv_Rdy	   = $10;
Mil_Fifo_Full      = $20;
Mil_Fifo_On	   = $40;
Cmd_Rcv		   = $80;
Timeout_1	   = $100;
Timeout_2	   = $200;
Debounce_On	   = $400;
Frei_D11	   = $800;
Evt_Fifo_Not_Empty = $1000;
Evt_Fifo_Full	   = $2000;
EvFilter_On	   = $4000;
Master_Slave	   = $8000;

{ Bitmasken fuer das Control Register 1
 (Ctrl_Reg1, nur setzen)
 --------------------------------------}
Ctrl_EvFilter_On     = $1;
Ctrl_Debounce_On     = $2;
Ctrl_Mil_Fifo_On     = $4;
Ctrl_En_Intr1	     = $8;
Ctrl_En_Intr2	     = $10;
Ctrl_EvFilter_8_12   = $20;
Ctrl_Puls1_Rahmen_On = $40;
Ctrl_Puls2_Rahmen_On = $80;
Ctrl_Puls3_Rahmen_On = $100;
Ctrl_Puls4_Rahmen_On = $200;
Ctrl_Puls5_Rahmen_On = $400;

 { Bitmasken fuer den das EventFilter
  ------------------------------------}
EvFilter_EventOn       = $1;
EvFilter_EventTimerRun = $2;
EvFilter_Start_Puls1   = $4;
EvFilter_Stop_Puls1    = $8;
EvFilter_Start_Puls2   = $10;
EvFilter_Stop_Puls2    = $20;

{ Fehlermeldungen
 ----------------}

StatusOK		  = $0;
VendorID_Error		  = $1;
DeviceID_Error		  = $2;
CountCards_Error	  = $4;
Open_Failure		  = $8;
IncorrectWinDriver	  = $10;
InvalidHandleValue	  = $20;
Failed_Open_WinDrv_Device = $40;
MilBus_Busy		  = $80;
TestTimerError	   	  = $100;
Fifo_Full		  = $200;
CardNrError		  = $400;
NoPCICartOpen		  = $800;
NoPCMil		   	  = $1000;
ReadMilBusTimeOut	  = $2000;
WriteMilBusTimeOut	  = $4000;
FifoNotEmpty		  = $8000;
FifoNotCleared		  = $10000;
NoDataToRead	   	  = $20000;
EventNrError		  = $40000;
IFKNrError		  = $80000;
FailedEnableIntr	  = $100000;
IntMaskError		  = $200000;
GeneralError              = $400000;
MasterSlaveError	  = $800000;
UnrequestedReceiv	  = $1000000;

type sonstiges =(EventOn, EventOff, EventTimerClear, EventTimerRun,
          EventClear, Puls1_On, Puls1_Off, Puls2_On, Puls2_Off,
          Cntrl_EvFilter_On,Cntrl_EvFilter_Off, Cntrl_Debounce_On,
          Cntrl_Debounce_Off,Cntrl_Mil_FiFo_On, Cntrl_Mil_FiFo_Off,
          Cntrl_En_Intr1, Cntrl_Dis_Intr1,Cntrl_En_Intr2,
          Cntrl_Dis_Intr2, Cntrl_EvFilter_12_Bit, Cntrl_EvFilter_8_Bit,
          Cntrl_Puls1_Rahmen_On, Cntrl_Puls1_Rahmen_Off, Cntrl_Puls2_Rahmen_On,
	  Cntrl_Puls2_Rahmen_Off, Cntrl_Puls3_Rahmen_On, Cntrl_Puls3_Rahmen_Off,
	  Cntrl_Puls4_Rahmen_On, Cntrl_Puls4_Rahmen_Off, Cntrl_Puls5_Rahmen_On,
	  Cntrl_Puls5_Rahmen_Off, PulsBuchse_1, PulsBuchse_2, Puls, RahmenPuls);


procedure MILDRIVER_VersionNr(var t_version_char);
                              cdecl; external 'PCIMilTreiber.dll';

function PCI_PCIcardCount(): Integer;
                         cdecl; external 'PCIMilTreiber.dll';

function PCI_DriverOpen(CardNr:Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_IfkOnline(CardNr:Integer;IFCNr:Byte;var ReturnIFKNR:Byte;
                       var ErrorStatus:Integer): Integer;
                       cdecl; external 'PCIMilTreiber.dll';

function PCI_IfkRead(CardNr:Integer; IFKAdress:Byte; IFKFunktionCode:Byte;
                     var data:Integer; var ErrorStatus:Integer): Integer;
                     cdecl; external 'PCIMilTreiber.dll';

function PCI_IfkWrite(CardNr:Integer; IFKAdress:Byte; IFKFunktionCode:Byte;
                      data:integer; var ErrorStatus:Integer): Integer;
                      cdecl; external 'PCIMilTreiber.dll';

function PCI_PCIcardReset(CardNr:Integer; var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';

function PCI_FctCodeSnd(CardNr:Integer; IFKAdress:Byte; FunktionCode:Byte;
                        var ErrorStatus: Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_ErrorString(status:Integer; var t_Error_Char):integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_TimerWait (CardNr:Integer; Time:Integer; TimeBase:Byte;
                        var ErrorStatus:Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_TimerSet (CardNr:Integer; Time:Integer; TimeBase:Byte;
                       var ErrorStatus:Integer): Integer;
                       cdecl; external 'PCIMilTreiber.dll';

function PCI_StatusRegRead (CardNr:Integer; var StatusReg:Integer;
                            var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_MilBusWrite (CardNr:Integer; data:Integer;
                          var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';


function PCI_DriverClose(CardNr:Integer):Integer;cdecl; external 'PCIMilTreiber.dll';

function PCI_IntMaskRead (CardNr:Integer; var data:Integer;
                          var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';

function PCI_IntMaskWrite(CardNr:Integer; data:Integer;
                          var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';

function PCI_IntEnable(CardNr:Integer; var ErrorStatus:Integer): Integer;
                       cdecl; external 'PCIMilTreiber.dll';

function PCI_IntDisable(CardNr:Integer; var ErrorStatus:Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_IntWait(CardNr:Integer; var IntAktiv:Integer;
                     var ErrorStatus:Integer): Integer;
                     cdecl; external 'PCIMilTreiber.dll';

function PCI_IntWaitStop(CardNr:Integer; var ErrorStatus:Integer): Integer;
                         cdecl; external 'PCIMilTreiber.dll';

function PCI_CntrlRegSet(CardNr:Integer; SetValue:Byte;
                         var ErrorStatus:Integer): Integer;
                         cdecl; external 'PCIMilTreiber.dll';

function PCI_CntrlRegRead(CardNr:Integer; var CntrlRegValue:Byte;
                          var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';

function PCI_EvFilterSet(CardNr:Integer; EventCodeNr:Integer; SetEV:Byte;
                         var ErrorStatus:Integer): Integer;
                         cdecl; external 'PCIMilTreiber.dll';

function PCI_EvFiFoRead(CardNr:Integer; var EventCodeNr:Integer;
                        var EVFifoNotEmpty:Boolean;
                        var ErrorStatus:Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_EvFilterRead(CardNr:Integer; EventCodeNr:Integer;
                          var FilterValue:Byte;
                          var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';


function PCI_StatusTest(CardNr:Integer; StatusBit:Integer;
                        var ErrorStatus:Integer): Boolean;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_MilBusRead(CardNr:Integer; var data:Integer;
                        var Fifo_not_empty:Boolean;
                        var ErrorStatus:Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_EvTimerRead(CardNr:Integer; var Time:Integer;
                         var ErrorStatus:Integer): Integer;
                         cdecl; external 'PCIMilTreiber.dll';


function PCI_PulsOutSet(CardNr:Integer; PulsBuchse:Integer;
                        EventOn:Integer; EventOff:Integer;
                        PulsRahmen:Integer; OnOff:Boolean;
                        var ErrorStatus:Integer): Integer;
                        cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls1Start(CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls1Stop (CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls2Start(CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls2Stop (CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls3Start(CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls3Stop (CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls4Start(CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls4Stop (CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls5Start(CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';

function PCI_SoftPuls5Stop (CardNr:Integer; var ErrorStatus:Integer): Integer;
                            cdecl; external 'PCIMilTreiber.dll';


function PCI_EvFilterClear(CardNr:Integer; var ErrorStatus:Integer): Integer;
                           cdecl; external 'PCIMilTreiber.dll';

function PCI_EvFiFioClear(CardNr:Integer; var ErrorStatus:Integer): Integer;
                          cdecl; external 'PCIMilTreiber.dll';


// Sucht IFKs am Bus und gibt diese als TStrings zurueck
procedure IFKFound(var IFKOnline: TStrings; var CountFound: Integer);

// umwandlung, wie der name schon sagt
function intToBinary(value:LongInt; digits:Byte):string;

implementation

procedure IFKFound(var IFKOnline: TStrings; var CountFound: Integer);

 var counter:integer;
     found:integer;
     ReturnIFKNr:Byte;
     myStatus:integer;

 begin
     counter := 1;
     CountFound := 0;
     myStatus := 0;


     while(counter <> 255) do
     begin
        found := PCI_IfkOnline(Cardauswahl, counter, ReturnIFKNr, myStatus);
        if(Found = StatusOK) then begin
           CountFound := CountFound + 1;
           with IFKOnline do
           begin
             {counter := IntToHex(counter,3);}
              Add(IntToHex(counter,3));
           end;
        end;
        counter := counter + 1;
     end;
  end;


function intToBinary(value:LongInt; digits:Byte):string;

var i:Byte;
    mask:LongInt;

begin
     SetLength (result, digits);
     for i := 0 to digits-1 do begin
         mask := 1 shl i;
         if(mask and value) = mask then
             result[digits-i] := '1'
         else
             result[digits-i] := '0'
     end
end;

end.
