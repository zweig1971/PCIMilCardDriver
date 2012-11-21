{****************************************************************************
*                                                                           *
*  Demo and Test Program for Windows DLL AVPCL.DLL                          *
*                                                                           *
*  Copyright (c) 1995-2007 by Alpermann+Velte                               *
*                                                                           *
*****************************************************************************

/* <pcltest.pas> Alpermann+Velte DLL for AV-PCL */
/* @(#)pcltest.pas 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte }

{********************************************************** program options }

{---------------------------------------------------------------------------}

program pcltest;

uses
   {$ifdef windows} wincrt {$else} crt {$endif},
   avpcl;

{---------------------------------------------------------------------------}

const VER_S =     '3.07';

procedure fatal(reason: string; result: integer);
begin
   if result < 0 then begin
      write('Fatal Error while ', reason, ': ', result);
      halt;
   end;
end;

{---------------------------------------------------------------------------}

const
  Nib2Char:array[0..15] of char=
    ('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');

{---------------------------------------------------------------------------}

function Nib2Hex(n:byte):string;
begin
   Nib2Hex := Nib2Char[n and $f];
end;

{---------------------------------------------------------------------------}

function Byte2Hex(b:byte):string;
begin
   Byte2Hex := Nib2Char[b shr 4] + Nib2Char[b and $f];
end;

{---------------------------------------------------------------------------}

function Word2Hex(w:word):string;
begin
   Word2Hex := Byte2Hex(hi(w)) + Byte2Hex(lo(w));
end;

{---------------------------------------------------------------------------}

function Long2Hex(l:longint):string;
begin
   Long2Hex := Word2Hex(HIWORD(l)) + Word2Hex(LOWORD(l));
end;

{---------------------------------------------------------------------------}

function Tc2Hex(l:longint):string;
begin
   Tc2Hex := Byte2Hex(hi(HIWORD(l))) + ':' + Byte2Hex(lo(HIWORD(l))) + ':' +
      Byte2Hex(hi(LOWORD(l))) + ':' + Byte2Hex(lo(LOWORD(l)));
end;

{---------------------------------------------------------------------------}

var
   result: integer;
   hdl: integer;
   event: PCL_TC;
   ident, firmware, dll: integer;
   mixed, vitc, ltc, ctl: PCL_TC;
   umixed, uvitc, ultc: PCL_USER;
   ch: char;
   hr, mi, se, fr: integer;
   event1, event2, event3: PCL_TC;
   oprt: PCL_OPRT;
   iprt: PCL_IPRT;
   ana: integer;

   i: integer;
   rctl: integer;
   full: PCL_FULL;
   found: boolean;

const
   running: boolean = TRUE;

begin
   {$ifndef windows}
      clrscr;
   {$endif}
   result := pclOpen(hdl, PCL_BASE_PCI_1);
	if result <> PCL_OK then begin
      fatal('pclOpen', result);
   end;

   gotoxy(1,1);
   writeln('Alpermann+Velte PclTest Version ', VER_S);
   writeln;
   writeln('Mixed (asyn) :');
   writeln('VITC  (sync) :');
   writeln('LTC   (asyn) :');
   writeln('CTL   (sync) :');
   writeln('Event (asyn) :');
   writeln('Ports        :');
   writeln('Version      :');
   writeln;
   writeln('eX)it (esc)');

   begin
      pclGetVersion(hdl, ident, firmware, dll);
      gotoxy(16, 9);
      write('Product: ');
      case firmware and $c0 of
	 0:   write('PCL3');
	 $80: write('PCL10');
	 $c0: write('PCL12');
	 else write('unknown');
      end;
      write(
	 ';  Firmware: V',
	 nib2hex((firmware and $30) shr 4), '.',
	 nib2hex(firmware), ', '
      );
      if ident <> 0 then write('special') else write('no special');
      write('(', ident, ');  DLL: V', dll shr 8, '.', dll and $ff);
   end;

   pclGetRegSync(hdl, PCL_REG_VITC, PCL_TC_P(NIL)^, PCL_INT_P(NIL)^,
      PCL_TC_P(NIL)^, PCL_INT_P(NIL)^, PCL_INT_P(NIL)^);
   pclGetRegSync(hdl, PCL_REG_CTL, PCL_TC_P(NIL)^, PCL_INT_P(NIL)^,
      PCL_TC_P(NIL)^, PCL_INT_P(NIL)^, PCL_INT_P(NIL)^);
   pclGetTc(hdl, event);
   pclEventTime(hdl, event);

   while running do begin
      while not keypressed do begin

	 begin
	    pclGetTc(hdl, mixed);
	    gotoxy(16, 3);
	    write(tc2Hex(mixed));

	    pclGetUser(hdl, umixed);
	    gotoxy(29, 3);
	    write(Long2Hex(umixed));
	 end;

	 result := pclGetRegSync(hdl, PCL_REG_VITC, vitc,
	    PCL_INT_P(NIL)^, uvitc, PCL_INT_P(NIL)^,
	    PCL_INT_P(NIL)^);
	 if result = PCL_OK then begin
	    gotoxy(16, 4);
	    write(Tc2Hex(vitc), '  ', Long2Hex(uvitc));
	 end;

	 begin
	    pclGetRegister(hdl, PCL_REG_LTC, ltc, PCL_INT_P(NIL)^, ultc,
	       PCL_INT_P(NIL)^, PCL_INT_P(NIL)^);
	    pclBcd2Tc(ltc, hr, mi, se, fr);
	    gotoxy(16, 5);
	    write(hr: 2, ':', mi: 2, ':', se: 2, ':', fr: 2, '  ',
	       Long2Hex(ultc));
	 end;

	 result := pclGetRegSync(hdl, PCL_REG_CTL, ctl, PCL_INT_P(NIL)^,
	    PCL_TC_P(NIL)^, PCL_INT_P(NIL)^, PCL_INT_P(NIL)^);
	 if result = PCL_OK then begin
	    gotoxy(16, 6);
	    write(Tc2Hex(ctl));
	 end;

	 begin
	    pclGetEvent(hdl, event1, event2, event3, PCL_INT_P(NIL)^,
	       PCL_INT_P(NIL)^, PCL_INT_P(NIL)^);
	    gotoxy(16, 7);
	    write(Tc2Hex(event1), '  ', Tc2Hex(event2), '  ',
	       Tc2Hex(event3));
	 end;

	 begin
	    pclGetIo(hdl, oprt, ana, iprt);
	    gotoxy(16, 8);
	    write('Out:', Long2Hex(oprt), ' Ana:', -10.0+ana*20/255:5:1,
	       'V In:', Word2Hex(iprt));
	 end;
      end;

      ch := readkey;
      case ch of
	 'x', 'X', #$1b:
	    running := FALSE;
      end;
   end;
   pclClose(hdl);
   gotoxy(1, 24);
end.
