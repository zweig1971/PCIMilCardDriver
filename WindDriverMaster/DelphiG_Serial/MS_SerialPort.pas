unit MS_SerialPort;

interface

uses Forms, Classes, Windows, SysUtils, StdCtrls, Messages;

const MySerialPort_CVersion = 'SIO 3.3.0 041119';

type TSIOStopBit   = (sb1, sb15, sb2);
     TSIOParity    = (pEven, pOdd, pNone, pMark, pSpace);
     TSIODTRControl= (dcDisable, dcEnable, dcHandshake);
     TSIORTSControl= (rcDisable, rcEnable, rcHandshake, rcToggle);


const C_StandardBaudrate = 9600;
      C_StandardData     = 8;
      C_StandardParity   = pNone;
      C_RTSDTR_Std       = 3;
      WM_MySIOSndMsg     = WM_User+$101;

      COK_MySerialPort   = true;
      C_COM              = 'COM';


type TMySerialPort = class;

TMySerialPortThread = class(TThread)

   private    FOwner: TMySerialPort;

   protected  procedure Execute; override;

   public     property Owner: TMySerialPort read FOwner write FOwner; end;

//***************************************************************************************
//***
//***
//***
//***************************************************************************************

TMySerialPort = class(TComponent)

     private   FOwnBuffer: array[0..255] of char;
               FpOwnBuff:  LongWord;

               FHandle    : THandle;

               FThread    : TMySerialPortThread;
               FOverlapped: TOverlapped;
               FThreadPrio: TThreadPriority;

               FComNr    : integer;
               FComboBox : TComboBox;
               FCheckUpTo: integer;

               FParity   : TSIOParity;
               FStopBit  : TSIOStopBit;
               FBaudRate : cardinal;
               FDataBits : cardinal;

               FRTSControl: TSIORTSControl;
               FDTRControl: TSIODTRControl;
               FRTSMode   : integer;
               FDTRMode   : integer;

               FInBufferSize : integer;
               FOutBufferSize: integer;
               FInBufHMin    : cardinal;
               FInBufHMax    : cardinal;

               FEvtChar     : Char;
               FEofChar     : Char;
               FErrorChar   : Char;
               FXonChar     : Char;
               FXoffChar    : Char;
               FLineEndChar : Char;
               FXonLim      : Word;
               FXoffLim     : Word;
               FParityCheck : Boolean;
               FOutxCtsFlow : Boolean;
               FOutxDsrFlow : Boolean;
               FDsrSensiv   : Boolean;
               FTXContOnXoff: Boolean;
               FOutX        : Boolean;
               FInX         : Boolean;
               FUseErrorChar: Boolean;
               FDiscardNull : Boolean;
               FAbortOnError: Boolean;

               FWaitEvent: boolean;

               FOnBREAK  : TNotifyEvent;
               FOnCTS    : TNotifyEvent;
               FOnDSR    : TNotifyEvent;
               FOnERROR  : TNotifyEvent;
               FOnRING   : TNotifyEvent;
               FOnRLSD   : TNotifyEvent;
               FOnRXCHAR : TNotifyEvent;
               FOnRXFLAG : TNotifyEvent;
               FOnTXEMPTY: TNotifyEvent;

               FUseForm  : TForm;
               FSndMsgTo : HWND;

               FErrMsg   : boolean;
               FSIOError : integer;
               FSIOWinErr: integer;
               FIntCount : integer;
               FInt997Err: integer;
               FInt87Err : integer;

               FSIOTestBool: boolean;

     private   procedure MyIntProg(Sender: TObject);

               procedure ShowSIOError(Error: integer);
               procedure CreateCOMList(AnzCOM: integer);
               function  COMPortExists(portNr: Integer): boolean;

               procedure SetThreadPriority (aValue : TThreadPriority);
               procedure SetBoolean (index : Integer; aValue : Boolean);
               procedure SetChar    (index : Integer; aValue : Char);
               procedure SetWord    (index : Integer; aValue : Word);
               procedure SetInteger (index : Integer; aValue : Integer);
               procedure SetCardinal(index : Integer; aValue : Cardinal);

               procedure SetParity  (aValue : TSIOParity);
               procedure SetStopBit (aValue : TSIOStopBit);
               procedure SetRTSControl(aValue : TSIORTSControl);
               procedure SetDTRControl(aValue : TSIODTRControl);
               function  SetSIOReg(MitTest: boolean): boolean;

               function GetActive : Boolean;
               function GetStateBoolean      (index : Integer) : Boolean;
               function GetStateInteger      (index : Integer) : Integer;
               function GetModemStatusBoolean(index : Integer) : Boolean;
               function GetBaudrate(Baud: integer): cardinal;
               function GetDataBits(Data: integer): cardinal;
               function GetStopBits(Stop: integer): TSIOStopBit;
               function GetParity  (Par:  char):    TSIOParity;

     public    constructor MyCreate(aOwner : TComponent; CB: TComboBox);
               destructor Destroy; override;

               procedure OpenComStream;
               procedure CloseComStream;
               procedure ClearSIOBuffer;
               procedure OwnDelay(msec: cardinal);

               procedure SetSIORegister;
               procedure SetL0Parameter(Baud, Data, Stop: integer; Parity: char);

               procedure ClearErrors;

               procedure Set_DTR;
               procedure Set_RTS;
               procedure Reset_DTR;
               procedure Reset_RTS;
               procedure StartReceiveData;
               procedure StopReceiveData;
               procedure SetSIOBreak;
               procedure ClrSIOBreak;

               function  GetModemStatus: cardinal;

     public    function  WriteBuffer(const buffer; bufferSize : Integer) : Cardinal;
               procedure TransmitCommChar( c : Char);
               procedure WritePChar (      p : PChar);
               procedure WriteString(const s : String);
               procedure WriteByte  (      b : Byte);
               procedure WriteChar  (      c : Char);
               procedure WriteWord  (      w : Word);
               procedure WriteDWord (      w : LongInt);

               function  ReadBuffer (var buffer; bufferSize : Integer) : Cardinal;
               procedure ReadString (var s: String);
               function  ReadCharII (var c: Char): boolean;
               function  ReadByteII (var c: Byte): boolean;
               procedure ReadByte   (var b: Byte);
               procedure ReadChar   (var c: Char);
               procedure ReadWord   (var w: Word);
               procedure ReadDWord  (var w: LongInt);

     public    property Overlapped: TOverlapped read FOverlapped;
               property Active    : Boolean     read GetActive;
               property Handle    : THandle     read FHandle;
               property SIOError  : integer     read FSIOError;
               property SIOWinErr : integer     read FSIOWinErr;

               property CTSHold : Boolean index 0 read GetStateBoolean; // Tx waiting for CTS signal
               property DSRHold : Boolean index 1 read GetStateBoolean; // Tx waiting for DSR signal
               property RLSDHold: Boolean index 2 read GetStateBoolean; // Tx waiting for RLSD signal
               property XoffHold: Boolean index 3 read GetStateBoolean; // Tx waiting, XOFF char rec'd
               property XoffSent: Boolean index 4 read GetStateBoolean; // Tx waiting, XOFF char sent
               property Eof     : Boolean index 5 read GetStateBoolean; // EOF character sent
               property Txim    : Boolean index 6 read GetStateBoolean; // character waiting for Tx

               property InQueue : Integer index 0 read GetStateInteger; // bytes in input buffer
               property OutQueue: Integer index 1 read GetStateInteger; // bytes in output buffer
               property InQueueF: Integer index 2 read GetStateInteger;

               property ModemCTS_ON : Boolean index 0 read GetModemStatusBoolean;
               property ModemDSR_ON : Boolean index 1 read GetModemStatusBoolean;
               property ModemRING_ON: Boolean index 2 read GetModemStatusBoolean;
               property ModemRLSD_ON: Boolean index 3 read GetModemStatusBoolean;


     public    property SetForm:        TForm           read FUseForm    write FUseForm;
               property SndMsgTo:       HWND            read FSndMsgTo   write FSndMsgTo;
               property ThreadPriority: TThreadPriority read FThreadPrio write SetThreadPriority default tpNormal;

               property Parity    : TSIOParity     read FParity     write SetParity     default pNone;
               property StopBit   : TSIOStopBit    read FStopBit    write SetStopBit    default sb2;
               property RTSControl: TSIORTSControl read FRTSControl write SetRTSControl default rcDisable;
               property DTRControl: TSIODTRControl read FDTRControl write SetDTRControl default dcDisable;

               property ComNr        : Integer index 0 read FComNr         write SetInteger default 1;
               property InBufferSize : Integer index 1 read FInBufferSize  write SetInteger default 2048;
               property OutBufferSize: Integer index 2 read FOutBufferSize write SetInteger default 2048;
               property IntCount     : Integer index 3 read FIntCount      write SetInteger default 0;
               property Int997Err    : Integer index 4 read FInt997Err     write SetInteger default 0;
               property Int87Err     : Integer index 5 read FInt87Err      write SetInteger default 0;
               property RTSMode      : Integer index 6 read FRTSMode       write SetInteger default 0;
               property DTRMode      : Integer index 7 read FDTRMode       write SetInteger default 0;

               property InBufHMin    :Cardinal index 0 read FInBufHMin     write SetCardinal default 0;
               property InBufHMax    :Cardinal index 1 read FInBufHMax     write SetCardinal default 0;
               property DataBits     :Cardinal index 2 read FDataBits      write SetCardinal default C_StandardData;
               property BaudRate     :Cardinal index 3 read FBaudRate      write SetCardinal default C_StandardBaudrate;

               property LineEndChar  : Char    index 0 read FLineEndChar   write SetChar default #0;
               property XonChar      : Char    index 1 read FXonChar       write SetChar default #17;
               property XoffChar     : Char    index 2 read FXoffChar      write SetChar default #19;
               property ErrorChar    : Char    index 3 read FErrorChar     write SetChar default #255;
               property EOFChar      : Char    index 4 read FeofChar       write SetChar default #13;
               property EventChar    : Char    index 5 read FEvtChar       write SetChar default #13;

               property XonLimit     : Word    index 0 read FXonLim        write SetWord default 400;
               property XoffLimit    : Word    index 1 read FXoffLim       write SetWord default 1600;

               property ParityCheck  : Boolean index  0 read FParityCheck  write SetBoolean default false;
               property OutxCTSFlow  : Boolean index  1 read FOutxCtsFlow  write SetBoolean default false;
               property OutxDSRFlow  : Boolean index  2 read FOutxDsrFlow  write SetBoolean default false;
               property DSRSensiv    : Boolean index  3 read FDSRSensiv    write SetBoolean default false;
               property TXContOnXoff : Boolean index  4 read FTXContOnXoff write SetBoolean default false;
               property OutXFlowCtrl : Boolean index  5 read FOutX         write SetBoolean default false;
               property InXFlowCtrl  : Boolean index  6 read FInX          write SetBoolean default false;
               property UseErrorChar : Boolean index  7 read FUseErrorChar write SetBoolean default false;
               property DiscardNull  : Boolean index  8 read FDiscardNull  write SetBoolean default false;
               property AbortOnError : Boolean index  9 read FAbortOnError write SetBoolean default false;
               property WaitComEvent : Boolean index 10 read FWaitEvent    write SetBoolean default false;
               property ShowErrMsg   : Boolean index 11 read FErrMsg       write SetBoolean default false;
               property SIOTestBool  : Boolean index 12 read FSIOTestBool  write SetBoolean default false;

               property OnBreak  : TNotifyEvent read FOnBREAK   write FOnBREAK;
               property OnCts    : TNotifyEvent read FOnCTS     write FOnCTS;
               property OnDsr    : TNotifyEvent read FOnDSR     write FOnDSR;
               property OnError  : TNotifyEvent read FOnERROR   write FOnERROR;
               property OnRing   : TNotifyEvent read FOnRING    write FOnRING;
               property OnRLSD   : TNotifyEvent read FOnRLSD    write FOnRLSD;
               property OnRxChar : TNotifyEvent read FOnRXCHAR  write FOnRXCHAR;
               property OnRxFlag : TNotifyEvent read FOnRxFlag  write FOnRXFLAG;
               property OnTxEmpty: TNotifyEvent read FOnTXEMPTY write FOnTXEMPTY;

  end;

implementation


//***************************************************************************************
//***
//***  Diverses
//***
//***************************************************************************************

procedure TMySerialPort.OwnDelay(msec: cardinal);
          var e: cardinal;

          begin
          if (msec=0) then exit;

          e:= GetTickCount + msec;

          repeat Application.ProcessMessages;

                 until (GetTickCount>=e); end;


//***************************************************************************************
//***
//***  Feststellen, ob es die gewünschte Schnittstelle gibt
//***
//***************************************************************************************

function TMySerialPort.COMPortExists(portNr : Integer) : boolean;
         var cc  : TCommConfig;
             s   : shortstring;
             size: Cardinal;
         begin

         s:=C_COM+IntToStr(portNr)+#0;

         size:=sizeof(TCommConfig);

         result:=GetDefaultCommConfig(@s[1], cc, size); end;


procedure TMySerialPort.CreateCOMList(AnzCOM: integer);
          var i : Integer;
          begin

          if not assigned(FComboBox) then exit;

          if (AnzCOM<0) then AnzCom    := FCheckUpTo
                        else FCheckUpTo:= AnzCOM;

          FComboBox.Items.Clear; if AnzCom=0 then exit;

          for i:=1 to AnzCOM do

              if COMPortExists(i) then FComboBox.Items.Add(C_COM+IntToStr (i)); end;


//***************************************************************************************
//***
//*** Fehler anzeigen
//***
//***************************************************************************************

procedure TMySerialPort.ShowSIOError(Error: integer);
          var ca: array[0..128] of char;
              pca: PChar;
              s1,s2: string;
          begin

          FSIOError:=Error; FSIOWinErr:=GetLastError;

          if FErrMsg then begin

             PCA:=addr(CA); PCA^:=#0;
             FormatMessage(
                           FORMAT_MESSAGE_ALLOCATE_BUFFER or FORMAT_MESSAGE_FROM_SYSTEM,
                           NIL,
                           GetLastError,
                           0, //
                           addr(PCA),
                           0,
                           NIL);

             case FSIOWinErr of 0: s1:='*** no error occurred ??? ***';
                                2: s1:='file not found';
                                5: s1:='access denied';
                              123: s1:='device not found';
                              else s1:=format('unknown system-identifier: (%d)', [FSIOWinErr]); end;

             case FSIOError of 1: s2:=': COM-Port does not exist';
                               2: s2:=': Though the COM-Port exists, there is no access';
                               3: s2:=' reading GETCOMMSTATE in SETSIOREG';
                               4: s2:=' writing SETCOMMSTATE in SETSIOREG';
                               5: s2:=' writing SETCOMMMASK in OPEN';
                               6: s2:=' SETUPCOMM in OPEN';
                               7: s2:=' setting break in SETCOMMBREAK';
                               8: s2:=' clearing braek in CLEARCOMMBREAK';
                               9: s2:=' error in WaitCommEvent';
                             else s2:=format(': unknown applications-identifier: (%d)', [FSIOError]); end;

             Application.MessageBox(PChar(    'Error'+s2+
                                          ^M^J+
                                          ^M^J+format('SIO-Handle COM%d: 0x%x', [FComNr, Fhandle])+
                                          ^M^J+
                                          ^M^J+format('Error number: %d', [FSIOWinErr])+
                                          ^M^J'error text from application: '+s1+
                                          ^M^J'error text from system: '+string(PCA)+
                                          ^M^J'This error may be critical.'+
                                          ^M^J'Close the application and restart.'+
                                          ^M^J),
                                          'Error in modul SERIALPORT', 0); end;
             SetLastError(0); end;


procedure TMySerialPort.ClearErrors;
          var err     : DWord;
              ComStat : TComStat;
              s       : shortstring;
          begin

          ClearCommError(FHandle, err, @ComStat);

          if (err<>0) then begin

             s:=' ';
             if (err and CE_RxParity)= CE_RxParity then s:=s+'Parity; ';
             if (err and CE_Frame)   = CE_Frame    then s:=s+'Framing; ';
             if (err and CE_Overrun) = CE_Overrun  then s:=s+'Overrun; ';
             if (err and CE_Break)   = CE_Break    then s:=s+'Break; ';
             if (err and CE_RxOver)  = CE_RxOver   then begin

                if (length(s)<3) then s:=s+'without Level-0-Error'+^M^J
                                 else s:=s+^M^J;

                s:=s+' buffer full; cleared'; ClearSioBuffer; end;

             Application.MessageBox(PChar(    ' Error on Serial COM-Port during receive '+
                                          ^M^J+
                                          ^M^J' Error number: '+format('0x%x', [err])+
                                          ^M^J+
                                          ^M^J+ s+
                                          ^M^J),
                                          'Error on Serial COM-Port', 0);  end;

          end;

//***************************************************************************************
//***
//***  Hier fängt die Wertzuweisnungen an Propertys an
//***
//***************************************************************************************

procedure TMySerialPort.SetThreadPriority(aValue: TThreadPriority); begin FThreadPrio:=aValue; end;

procedure TMySerialPort.SetParity (aValue: TSIOParity);  begin FParity :=aValue; end;
procedure TMySerialPort.SetStopBit(aValue: TSIOStopBit); begin FStopBit:=aValue; end;

procedure TMySerialPort.SetRTSControl(aValue: TSIORTSControl); begin FRTSControl:= aValue; end;
procedure TMySerialPort.SetDTRControl(aValue: TSIODTRControl); begin FDTRControl:= aValue; end;


procedure TMySerialPort.SetChar(index: Integer; aValue: Char); begin

          case index of 0: FLineEndChar := aValue;
                        1: FXonChar     := aValue;
                        2: FXoffChar    := aValue;
                        3: FErrorChar   := aValue;
                        4: FeofChar     := aValue;
                        5: FevtChar     := aValue; end; end;


procedure TMySerialPort.SetWord(index: Integer; aValue: Word); begin

          case index of 0: FXonLim := aValue;
                        1: FXoffLim:= aValue; end; end;


procedure TMySerialPort.SetInteger(index: Integer; aValue: Integer); begin

          case index of 0: FComNr        := aValue;
                        1: FInBufferSize := aValue;
                        2: FOutBufferSize:= aValue;
                        3: FIntCount     := aValue;
                        4: FInt997Err    := aValue;
                        5: FInt87Err     := aValue;
                        6: FRTSMode      := aValue;
                        7: FDTRMode      := aValue; end; end;

procedure TMySerialPort.SetCardinal(index: Integer; aValue: cardinal); begin

          case index of 0: FInBufHMin := aValue;
                        1: FInBufHMax := aValue;
                        2: FDataBits  := aValue;
                        3: FBaudRate  := aValue; end; end;

procedure TMySerialPort.SetBoolean(index: Integer; aValue: Boolean); begin

          case index of 0: FParityCheck := aValue;
                        1: FOutxCtsFlow := aValue;
                        2: FOutxDsrFlow := aValue;
                        3: FDsrSensiv   := aValue;
                        4: FTXContOnXoff:= aValue;
                        5: FOutX        := aValue;
                        6: FInX         := aValue;
                        7: FUseErrorChar:= aValue;
                        8: FDiscardNull := aValue;
                        9: FAbortOnError:= aValue;
                       10: FWaitEvent   := aValue;
                       11: FErrMsg      := aValue;
                       12: FSIOTestBool := aValue; end; end;

//***************************************************************************************
//***
//***  Reale Werte nach Index-Werte wandeln
//***
//***************************************************************************************

function TMySerialPort.GetActive: Boolean; begin result:=(FHandle<>0); end;


function TMySerialPort.GetBaudrate(Baud: integer): cardinal;
//          const anz= 19;
//                BArr: array[1..Anz] of record own: TSIOBaudrate; value: integer; end =
//                      ((own: br75;     value:     75; ),
//                       (own: br110;    value:    110; ),
//                       (own: br134;    value:    134; ),
//                       (own: br150;    value:    150; ),
//                       (own: br300;    value:    300; ),
//                       (own: br600;    value:    600; ),
//                       (own: br1200;   value:   1200; ),
//                       (own: br1800;   value:   1800; ),
//                       (own: br2400;   value:   2400; ),
//                       (own: br4800;   value:   4800; ),
//                       (own: br7200;   value:   7200; ),
//                       (own: br9600;   value:   9600; ),
//                       (own: br14400;  value:  14400; ),
//                       (own: br19200;  value:  19200; ),
//                       (own: br38400;  value:  38400; ),
//                       (own: br57600;  value:  57600; ),
//                       (own: br115200; value: 115200; ),
//                       (own: br128000; value: 128000; ),
//                       (own: br256000; value: 256000; ));
//         var i: integer;
//
           begin result:=Baud;
//
//         if (Baud<75) or (Baud>1500000) then result:=9600;
//
//         for i:=1 to anz do if (BArr[i].value=Baud) then result:=BArr[i].own;
//
           end;

function TMySerialPort.GetDataBits(Data: integer): cardinal; // ownDataBits;
//        const anz=5;
//              BArr: array[1..Anz] of record own: ownDataBits; value: integer; end =
//                     ((own: db4;  value: 4; ),
//                      (own: db5;  value: 5; ),
//                      (own: db6;  value: 6; ),
//                      (own: db7;  value: 7; ),
//                      (own: db8;  value: 8; ));
//       var i: integer;
//
         begin result:=Data;
//
//       for i:=1 to anz do if (BArr[i].value=Data) then result:=BArr[i].own;
//
         end;


function TMySerialPort.GetParity(Par: char): TSIOParity;
          const anz=5;
                BArr: array[1..Anz] of record own: TSIOParity; value: char; end =
                      ((own: pEven;  value: 'E'; ),
                       (own: pOdd;   value: 'O'; ),
                       (own: pNone;  value: 'N'; ),
                       (own: pMark;  value: 'M'; ),
                       (own: pSpace; value: 'S'; ));
         var i: integer;

         begin result:=pNone;

         for i:=1 to anz do if (BArr[i].value=Par) then result:=BArr[i].own; end;


function TMySerialPort.GetStopBits(Stop: integer): TSIOStopBit;
          const anz=3;
                BArr: array[1..Anz] of record own: TSIOStopBit; value: integer; end =
                      ((own: sb1;  value: 1; ),
                       (own: sb15; value: 3; ),
                       (own: sb2;  value: 2; ));
         var i: integer;

         begin result:=sb1;

         for i:=1 to anz do if (BArr[i].value=Stop) then result:=BArr[i].own; end;

//***************************************************************************************
//***
//***  Register im SIO-Controller schreiben
//***
//***************************************************************************************

function TMySerialPort.SetSIOReg(MitTest: boolean): boolean;

         const par  : Array[0..4] of Integer = (EVENPARITY, ODDPARITY, NOPARITY, MARKPARITY, SPACEPARITY);
               sbits: Array[0..2] of Integer = (ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS);
               rctrl: Array[0..3] of Integer = (RTS_CONTROL_DISABLE, RTS_CONTROL_ENABLE, RTS_CONTROL_HANDSHAKE, RTS_CONTROL_TOGGLE);
               dctrl: Array[0..2] of Integer = (DTR_CONTROL_DISABLE, DTR_CONTROL_ENABLE, DTR_CONTROL_HANDSHAKE);

         var dcb: TDCB;
             ok: boolean;

         begin result:=false;

         ok:=GetCommState(FHandle, dcb);
         if (MitTest and not(ok)) then begin ShowSIOError(3); exit; end;

         dcb.DCBLength:= sizeof(dcb);
         dcb.BaudRate := FBaudrate; // bauds[Ord(FBaudRate)];
         dcb.XonLim   := FXonLim;
         dcb.XoffLim  := FXoffLim;
         dcb.ByteSize := FDataBits; // dbits[Ord(FDataBits)];
         dcb.Parity   := par[Ord(FParity)];
         dcb.StopBits := sbits[Ord(FStopBit)];
         dcb.XonChar  := FXonChar;
         dcb.XoffChar := FXoffChar;
         dcb.ErrorChar:= FErrorChar;
         dcb.EofChar  := FEofChar;
         dcb.EvtChar  := FEvtChar;

         dcb.Flags := 1 or
                      (ord(FParityCheck)       shl  1) or
                      (ord(FOutxCtsFlow)       shl  2) or
                      (ord(FOutxDsrFlow)       shl  3) or
                      (dctrl[ord(FDTRControl)] shl  4) or
                      (ord(FDsrSensiv)         shl  6) or
                      (ord(FTXContOnXoff)      shl  7) or
                      (ord(FOutX)              shl  8) or
                      (ord(FInX)               shl  9) or
                      (ord(FUseErrorChar)      shl 10) or
                      (ord(FDiscardNull)       shl 11) or
                      (ord(FInX)               shl 12) or
                      (rctrl[ord(FRTSControl)] shl 13) or
                      (ord(FAbortOnError)      shl 15);

         ok:=SetCommState(FHandle, dcb);
         if (MitTest and not(ok)) then begin ShowSIOError(4); exit; end;

         result:=true; end;


procedure TMySerialPort.SetSIORegister; begin SetSIOReg(false); end;

procedure TMySerialPort.SetL0Parameter(Baud, Data, Stop: integer; Parity: char); begin

          FBaudrate:=GetBaudrate(Baud);
          FDataBits:=GetDataBits(Data);
          FStopBit:= GetStopBits(Stop);
          FParity:=  GetParity(Parity); SetSIOReg(false); end;

// procedure TMySerialPort.Set_DTR;   begin FDTRControl:=dcEnable;  SetSIOReg(false); end;
// procedure TMySerialPort.Set_RTS;   begin FRTSControl:=rcEnable;  SetSIOReg(false); end;
// procedure TMySerialPort.Reset_DTR; begin FDTRControl:=dcDisable; SetSIOReg(false); end;
// procedure TMySerialPort.Reset_RTS; begin FRTSControl:=rcDisable; SetSIOReg(false); end;


procedure TMySerialPort.Set_DTR;
          begin
          case FDTRMode of 0: begin FDTRControl:=dcEnable;  EscapeCommFunction(Handle, SETDTR); end;
                           1: begin FDTRControl:=dcDisable; EscapeCommFunction(Handle, CLRDTR); end;
                           2: begin FDTRControl:=dcDisable; EscapeCommFunction(Handle, CLRDTR); end;
                           3: begin FDTRControl:=dcEnable;  EscapeCommFunction(Handle, SETDTR); end; end; end;

procedure TMySerialPort.Reset_DTR;
          begin
          case FDTRMode of 0: begin FDTRControl:=dcEnable;  EscapeCommFunction(Handle, SETDTR); end;
                           1: begin FDTRControl:=dcDisable; EscapeCommFunction(Handle, CLRDTR); end;
                           2: begin FDTRControl:=dcEnable;  EscapeCommFunction(Handle, SETDTR); end;
                           3: begin FDTRControl:=dcDisable; EscapeCommFunction(Handle, CLRDTR); end; end; end;

procedure TMySerialPort.Set_RTS;
          begin
          case FRTSMode of 0: begin FRTSControl:=rcEnable;  EscapeCommFunction(Handle, SETRTS); end;
                           1: begin FRTSControl:=rcDisable; EscapeCommFunction(Handle, CLRRTS); end;
                           2: begin FRTSControl:=rcDisable; EscapeCommFunction(Handle, CLRRTS); end;
                           3: begin FRTSControl:=rcEnable;  EscapeCommFunction(Handle, SETRTS); end; end; end;

procedure TMySerialPort.Reset_RTS;
          begin
          case FRTSMode of 0: begin FRTSControl:=rcEnable;  EscapeCommFunction(Handle, SETRTS); end;
                           1: begin FRTSControl:=rcDisable; EscapeCommFunction(Handle, CLRRTS); end;
                           2: begin FRTSControl:=rcEnable;  EscapeCommFunction(Handle, SETRTS); end;
                           3: begin FRTSControl:=rcDisable; EscapeCommFunction(Handle, CLRRTS); end; end; end;


procedure TMySerialPort.SetSIOBreak; begin if not(SetCommBreak(FHandle))   then ShowSIOError(7); end;
procedure TMySerialPort.ClrSIOBreak; begin if not(ClearCommBreak(FHandle)) then ShowSIOError(8); end;


//***************************************************************************************
//***
//*** Serielle Schnittstelle öffnen und Schließen
//***
//***************************************************************************************

procedure TMySerialPort.OpenComStream;
          var ct: _CommTimeOuts;
              s1: shortstring;
               i: integer;
          begin

          FSIOError:=0; FSIOWinErr:=0;

          FIntCount:=0; FInt997Err:=0; FInt87Err:=0;

          if (FHandle<>0) then CloseComStream;

          if not COMPortExists(FComNr) then begin FSIOError:=1; exit; end;

          s1:=format(C_COM+'%d:', [FComNr])+#0;

          FHandle := CreateFile (@s1[1],
                                 GENERIC_READ OR GENERIC_WRITE,
                                 0,             // not overlapped I/O
                                 nil,           // no security attributes
                                 OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                                 FILE_FLAG_OVERLAPPED OR FILE_ATTRIBUTE_NORMAL,
                                 0              // hTemplate must be NULL for comm devices
                                 );

          with FOverlapped do begin
               Internal:=0;
               InternalHigh:=0;
               Offset:=0;
               OffsetHigh:=0;
               hEvent:=0; end; // CreateEvent(nil,false,false,nil); end;

          if (FHandle=0) or (FHandle=$0FFFFFFFF) then begin ShowSIOError(2); FHandle:=0; exit; end;

          if not(SetSIOReg(true)) then begin CloseComStream; exit; end;

          i:=0;
          if assigned(FOnBreak)   then i:=i or EV_Break;
          if assigned(FOnCts)     then i:=i or EV_CTS;
          if assigned(FOnDSR)     then i:=i or EV_DSR;
          if assigned(FOnError)   then i:=i or EV_Err;
          if assigned(FOnRing)    then i:=i or EV_Ring;
          if assigned(FOnRLSD)    then i:=i or EV_RLSD;
          if assigned(FOnRxChar)  then i:=i or EV_RxChar;
          if assigned(FOnRxFlag)  then i:=i or EV_RxFlag;
          if assigned(FOnTxEmpty) then i:=i or EV_TxEmpty;
          if not SetCommMask(FHandle,i) then begin ShowSIOError(5); CloseComStream; Exit; end;

          if (FInBufferSize<128)   then FInBufferSize:=128;
          if (FInBufferSize>$8000) then FInBufferSize:=$8000;

          FInBufHMax:=trunc(FInBufferSize*0.9);
          FInBufHMin:=trunc(FInBufferSize*0.1);

          if not(SetupComm (FHandle, FInBufferSize, FOutBufferSize)) then begin ShowSIOError(6); CloseComStream; Exit; end;

          GetCommTimeOuts(Handle, CT);         //
          CT.WriteTotalTimeoutConstant:=0;     // Ohne System-Handshake, da das nicht funktioniert
          CT.WriteTotalTimeoutMultiplier:=0;   //
          SetCommTimeOuts(Handle, CT);

          FThread := TMySerialPortThread.Create(true);
          FThread.Owner := self;
          FThread.Priority := FThreadPrio;
          FThread.FreeOnTerminate := true;
          FThread.Resume;

          FIntCount :=0;
          FInt997Err:=0;
          FInt87Err :=0;

          end;


procedure TMySerialPort.CloseComStream; begin

          if (FHandle=0) or not(Assigned(FThread)) then exit;

          FThread.Terminate;
          if (Win32Platform=1) then TransmitCommChar(#00);  // ??? Muss sein, da sich der Thread sonst nicht beendet ???

       // FThread.WaitFor; FThread.Free;

          OwnDelay(100); CloseHandle(FHandle); FHandle:=0; end;

//***************************************************************************************
//***
//***  So wird also eine Instanz angelegt und gelöscht (oder so)
//***
//***************************************************************************************

constructor TMySerialPort.MyCreate(aOwner : TComponent; CB: TComboBox);

            begin

            Create(aOwner);

            FHandle           := 0;

            FInBufferSize     := 4096;
            FOutBufferSize    := 4096;

            FThreadPrio       := tpNormal;

            FWaitEvent        := false;

            FComNr            := 1;
            FBaudRate         := C_StandardBaudrate;
            FDataBits         := C_StandardData;
            FParity           := C_StandardParity;
            FStopBit          := sb2;

            FLineEndChar := #0;

            FParityCheck := false;
            FOutxCtsFlow := false;
            FOutxDsrFlow := false;
            FDsrSensiv   := false;
            FTXContOnXoff:= false;
            FOutX        := false;
            FInX         := false;
            FUseErrorChar:= false;
            FDiscardNull := false;
            FAbortOnError:= true;

            FRTSControl  := rcDisable;
            FDTRControl  := dcDisable;

            FErrMsg      := true;

            FRTSMode := C_RTSDTR_Std;
            FDTRMode := C_RTSDTR_Std;

            FUseForm  :=nil;
            FSndMsgTo :=0;

            FOnBreak  :=nil;
            FOnCts    :=nil;
            FOnDSR    :=MyIntProg;
            FOnError  :=nil;
            FOnRing   :=nil;
            FOnRLSD   :=nil;
            FOnRxChar :=nil;
            FOnRxFlag :=nil;
            FOnTxEmpty:=nil;

            FComboBox:= CB;
            CreateCOMList(16); end;


destructor TMySerialPort.Destroy; begin

           CloseComStream;

           inherited Destroy; end;

//***************************************************************************************
//***
//***   Daten auf Serielle Schnittstelle schreiben
//***
//***************************************************************************************

procedure TMySerialPort.ClearSIOBuffer; begin

          FpOwnBuff:=0; PurgeComm(Handle, Purge_RxClear or Purge_TxClear); end;


procedure TMySerialPort.StopReceiveData;
          var err     : DWord;
              ComStat : TComStat;
          begin

          if (FRTSMode<2) or (FRTSControl=rcDisable) then exit;

          ComStat.cbInQue:=High(FInBufHMax); ClearCommError(FHandle, err, @ComStat);

          if (ComStat.cbInQue > FInBufHMax) then Reset_RTS;

          end;


procedure TMySerialPort.StartReceiveData;
          var err     : DWord;
              ComStat : TComStat;
          begin

          if (FRTSMode<2) or (FRTSControl=rcEnable) then exit;

          ComStat.cbInQue:=High(FInBufHMin); ClearCommError(FHandle, err, @ComStat);

          if ComStat.cbInQue < FInBufHMin then Set_RTS; end;


function TMySerialPort.WriteBuffer(const buffer; bufferSize: Integer): DWord; begin

         WriteFile(FHandle, buffer, bufferSize, result, @fOverlapped); end;

procedure TMySerialPort.WritePChar (      p : PChar);   begin WriteBuffer(p^, StrLen(p));   end;
procedure TMySerialPort.WriteString(const s : String);  begin WriteBuffer(s[1], Length(s)); end;
procedure TMySerialPort.WriteByte  (      b : Byte);    begin WriteBuffer(b, 1);            end;
procedure TMySerialPort.WriteChar  (      c : Char);    begin WriteBuffer(c, 1);            end;
procedure TMySerialPort.WriteWord  (      w : Word);    begin WriteBuffer(w, 2);            end;
procedure TMySerialPort.WriteDWord (      w : LongInt); begin WriteBuffer(w, 4);            end;

procedure TMySerialPort.TransmitCommChar(c: Char); begin Windows.TransmitCommChar(FHandle, c); end;

//***************************************************************************************
//***
//***   Daten von Serieller Schnittstelle lesen
//***
//***************************************************************************************

function  TMySerialPort.ReadCharII(var c: Char): boolean;
          var res: DWord;
              inq: LongWord;
              Buffer: array[0..255] of char;
          begin

          result:=false;
          InQ:=InQueue; if (InQ>200) then InQ:=200;

          if (FpOwnBuff=0) then begin
                               if (InQ=0) then exit;
                               ReadFile(FHandle, Buffer, InQ, res, @FOverlapped);
                               FpOwnBuff:=res;

                               asm  push esi; push edi; push ecx; push ebx

                                    lea  esi, [Buffer]
                                    mov  ebx, self
                                    lea  edi, [ebx+TMySerialPort.FOwnBuffer]
                                    mov  ecx, [ebx+TMySerialPort.FpOwnBuff]
                                    add  edi, ecx
                                    dec  edi

                               @@l1:
                                    mov  al, byte [ss:esi]; inc  esi
                                    mov  [edi], al        ; dec  edi
                                    loop @@l1

                                    pop ebx; pop ecx; pop edi; pop esi; end;

                               StartReceiveData; end;

          dec(FpOwnBuff); c:=FOwnBuffer[FpOwnBuff];

          result:=true; end;


function  TMySerialPort.ReadByteII(var c: byte): boolean;
          var i: char;
          begin

          result:=ReadCharII(i); c:=ord(i); end;


function TMySerialPort.ReadBuffer(var buffer; bufferSize: Integer): DWord; begin

         ReadFile(FHandle, buffer, bufferSize, result, @FOverlapped);

         StartReceiveData; end;



procedure TMySerialPort.ReadString (var s : String);
          var c: Char;
              i,j: integer;
          begin
          s := ''; j:=InQueue;

          for i:=1 to j do begin

              ReadBuffer(c, 1); if (c=FLineEndChar) then break;

              s:=s+c; end; end;


procedure TMySerialPort.ReadByte (var b : Byte);    begin if InQueue<1 then b:=0  else ReadBuffer(b, 1); end;
procedure TMySerialPort.ReadChar (var c : Char);    begin if InQueue<1 then c:=#0 else ReadBuffer(c, 1); end;
procedure TMySerialPort.ReadWord (var w : Word);    begin if InQueue<2 then w:=0  else ReadBuffer(w, 2); end;
procedure TMySerialPort.ReadDWord(var w : LongInt); begin if InQueue<4 then w:=0  else ReadBuffer(w, 4); end;

//***************************************************************************************
//***
//*** Betriebssystemzugriffe für Status der Seriellen Schnittstelle
//***
//***************************************************************************************

function TMySerialPort.GetStateBoolean(index : Integer) : Boolean;
         var err    : DWord;
             ComStat: TComStat;
         begin

         result:=false; if (FHandle=0) then exit;

         if ClearCommError(FHandle, err, @ComStat) then

            case index of 0: result:=(fCtlHold in ComStat.Flags);
                          1: result:=(fDsrHold in ComStat.Flags);
                          2: result:=(fRlsHold in ComStat.Flags);
                          3: result:=(fXoffHold in ComStat.Flags);
                          4: result:=(fXOffSent in ComStat.Flags);
                          5: result:=(fEof in ComStat.Flags);
                          6: result:=(fTxim in ComStat.Flags); end; end;


function TMySerialPort.GetStateInteger(index : Integer) : Integer;
         var err     : DWord;
             ComStat : TComStat;
         begin

         result:=0; if (FHandle=0) then exit;

         if ClearCommError(FHandle, err, @ComStat) then

            case index of 0: result:=ComStat.cbInQue;
                          1: result:=ComStat.cbOutQue;
                          2: result:=ComStat.cbInQue + FpOwnBuff; end; end;


function TMySerialPort.GetModemStatusBoolean(index : Integer) : Boolean;
         var ModemStat : DWord;
         begin

         result:=false; if (FHandle=0) then exit;

         if GetCommModemStatus (FHandle, ModemStat) then

            case index of 0: result:=(ModemStat and MS_CTS_ON = MS_CTS_ON);
                          1: result:=(ModemStat and MS_DSR_ON = MS_DSR_ON);
                          2: result:=(ModemStat and MS_RING_ON = MS_RING_ON);
                          3: result:=(ModemStat and MS_RLSD_ON = MS_RLSD_ON); end; end;

                          
function TMySerialPort.GetModemStatus: cardinal;
         var s: cardinal;
         begin

         GetCommModemStatus(FHandle, s); result:=s; end;

//***************************************************************************************
//***
//*** Thread für Status der Seriellen Schnittstelle
//***
//***************************************************************************************

procedure TMySerialPort.MyIntProg(Sender: TObject); begin

          if FSndMsgTo<>0
             then if assigned(FUseForm)
                     then FUseForm.PerForm(WM_MySIOSndMsg,0,500)
                     else SendMessage(FSndMsgTo,WM_MySIOSndMsg,0,500)
             else begin
                  if FSIOTestBool then EscapeCommFunction(Handle, SETDTR)
                                  else EscapeCommFunction(Handle, CLRDTR);
                  FSIOTestBool:=not(FSIOTestBool);

                  end; end;
//
// Ich empfehle, die ganze Sache wie folgt zu testen:
//
// Zuerst mit der Umschaltung der DRT-Leitung. Gemessen werden muss der zeitliche Abstand
//    zwichen DSR und DTR z.B. mit einem Oszilloskop. Pro Impuls - also pro entweder ein
//    oder zwei Interrupts, das kommt darauf an, wie breit der Impuls ist - muss die DTR-
//    Leitung einmal umgeschaltet werden. Dann wurde der Impuls erkannt.
// Danach mit SendMessage. In der Message-Routine in F_MainMask steht auch das Umschalten
//    der DTR-Leitung drinnen. Auch hier wieder nachmessen. Erst dann kann man sagen, ob
//    jeder Impuls erkannt worden ist.
//
// Wo die Berechnung des Impulsabstabstandes gemacht wird - ob in WaitCommEvent, in MyIntProg
//    oder im Messages-Handler, ist eigentlich egal.
//
// Beachtet werden muss, dass pro Impuls entweder ein Interrupt oder zwei Interrupts kommen
//    können, jenachdem wie breit der Impuls ist. Diese Tatsache muss die Software erkennen.
//    Bei einer maximalen Umdrehungesgeschwindigkeit 100Imp/sec (=10ms), kann man alle
//    Interrups, die schneller sind als 9ms ignorieren. 9m wäre die Impulsbreite bei einer
//    Umdrehungsgeschwindigkeit von 0.6 Imp/sec - also vernachlässigbar.
//

procedure TMySerialPortThread.Execute;
          var evtMask : Cardinal;
              LError  : integer;
          begin

          if not(FOwner.FWaitEvent) then exit;

while (true) do begin

      if Terminated then exit;

      evtmask:=0;

      with FOwner do begin
      if not WaitCommEvent(Handle, evtMask, @Overlapped) then begin
         LError:=GetLastError;
         if (LError=997) then begin LError:=0; inc(FInt997Err); end; // Überlappender Vorrang wird bearbeitet
         if (LError= 87) then begin LError:=0; inc(FInt87Err);  end; // Falscher Parameter
         if (LError<> 0) then begin FOwner.ShowSIOError(9); exit; end; end;

      if (evtMask<>0) then begin inc(FIntCount);

if not(Terminated) and ((evtMask and EV_BREAK) <>0) and Assigned(FOnBreak)  then FOnBreak(FOwner);
if not(Terminated) and ((evtMask and EV_CTS)   <>0) and Assigned(FOnCTS)    then FOnCts(FOwner);
if not(Terminated) and ((evtMask and EV_DSR)   <>0) and Assigned(FOnDSR)    then FOnDsr(FOwner);
if not(Terminated) and ((evtMask and EV_ERR)   <>0) and Assigned(FOnError)  then FOnError(FOwner);
if not(Terminated) and ((evtMask and EV_RING)  <>0) and Assigned(FOnRing)   then FOnRing(FOwner);
if not(Terminated) and ((evtMask and EV_RLSD)  <>0) and Assigned(FOnRLSD)   then FOnRLSD(FOwner);
if not(Terminated) and ((evtMask and EV_RXCHAR)<>0) and Assigned(FOnRxChar) then FOnRxChar(FOwner);
if not(Terminated) and ((evtMask and EV_RXFLAG)<>0) and Assigned(FOnRxFlag) then FOnRxFlag(FOwner);
if not(Terminated) and ((evtMask and EV_TXEMPTY)<>0)and Assigned(FOnTxEmpty)then FOnTxEmpty(FOwner);

      end; end; end; end;

end.

