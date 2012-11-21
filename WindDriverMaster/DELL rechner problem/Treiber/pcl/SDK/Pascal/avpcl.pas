{****************************************************************************
*                                                                           *
*  Macros and function prototypes of Windows DLLs AVPCL.DLL and AVPCL32.DLL *
*                                                                           *
*  Copyright (c) 1995-2007 by Alpermann+Velte                               *
*                                                                           *
*****************************************************************************

/* <avpcl.pas> Alpermann+Velte DLL for AV-PCL */
/* @(#)avpcl.pas 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte */

   For documentation see avpcl.h
}

unit avpcl;

{***************************************************************************}
{*}                                                                       {*}
{*}     Interface                                                         {*}
{*}                                                                       {*}
{***************************************************************************}

const PCL_VER_DLL = $0324; // 3.36

{********************************************************** program options }

{ $define PCL_OPT_GETCMD}
{$define PCL_OPT_REALTIME}

{************************************************************* return codes }

const
	PCL_OK			=   0;

	PCL_NO_NEWD		=   1;

	PCL_ERR_INV_PORT	=  -1;
	PCL_ERR_NOT_FOUND	=  -2;
	PCL_ERR_NO_HANDLE	=  -3;
	PCL_ERR_INV_HDL		=  -4;
	PCL_ERR_NOTOPEN		=  -5;
	PCL_ERR_NULL		=  -6;
	PCL_ERR_TIMEOUT		=  -7;
	PCL_ERR_INV_REG		=  -8;
	PCL_ERR_INV_BANK	=  -9;
	PCL_ERR_RANGE		= -10;
	PCL_ERR_INIT		= -11;
	PCL_ERR_IOCTL		= -12;
	PCL_ERR_VER		= -13;
	PCL_ERR_NOT_SUPP	= -14;
	PCL_ERR_UK_CMD		= -15;
	PCL_ERR_OPEN		= -16;

{**************************************************************** constants }

const
	PCL_HANDLES	= 10;

	PCL_REQ_ASYN	= $80;
	PCL_REQ_SYNC	= $40;
	PCL_REQ_PERM	= $20;

{***************************************************************** typedefs }

type
	PCL_INT		= longint;
	PCL_DWORD	= cardinal;

	PCL_INT_P	= ^PCL_INT;
	PCL_DWORD_P	= ^PCL_DWORD;

	PCL_BOOL	= longbool;

	PCL_TC		= PCL_DWORD;
	PCL_TC_P	= ^PCL_TC;

	PCL_USER	= PCL_DWORD;
	PCL_USER_P	= ^PCL_USER;

{******************************************************************* macros }

function HIWORD(l: PCL_DWORD): word;

function LOWORD(l: PCL_DWORD): word;

function BCD2HEX(bcd: byte): byte;

function HEX2BCD(hex: byte): byte;

{*************************************************************** prototypes }

function pclOpen(
	var hdl: PCL_INT;
	base: PCL_INT
): PCL_INT; stdcall

const
	PCL_BASE_PCI_OFS	= $1000;
	PCL_BASE_PCI_1		= $1001;
	PCL_BASE_PCI_2		= $1002;
	PCL_BASE_PCI_3		= $1003;
	PCL_BASE_PCI_4		= $1004;
	PCL_BASE_PCI_5		= $1005;
	PCL_BASE_PCI_6		= $1006;
	PCL_BASE_PCI_7		= $1007;
	PCL_BASE_PCI_8		= $1008;
	PCL_BASE_PCI_9		= $1009;
	PCL_BASE_PCI_10		= $100a;
	PCL_BASE_PCI_LAST	= PCL_BASE_PCI_10;

{---------------------------------------------------------------------------}

function pclClose(
	hdl: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetTc(
	hdl: PCL_INT;
	var time: PCL_TC
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetUser(
	hdl: PCL_INT;
	var user: PCL_USER
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetRegister(
	hdl: PCL_INT;
	reg: PCL_INT;
	var time: PCL_TC;
	var bits: PCL_INT;
	var user: PCL_TC;
	var frame: PCL_INT;
	var newd: PCL_INT
): PCL_INT; stdcall

const
	PCL_REG_MIXED		= $00;
	PCL_REG_LTC		= $02;
	PCL_REG_VITC		= $03;

	PCL_FRAME_24		= $24;
	PCL_FRAME_25		= $25;
	PCL_FRAME_30		= $30;
	PCL_FRAME_30DF		= $B0;

	PCL_NEWD_NONE		= $00;
	PCL_NEWD_VITC		= $80;
	PCL_NEWD_LTC		= $40;

	PCL_BITS_LTC_10		= $01;
	PCL_BITS_LTC_DROP	= $01;
	PCL_BITS_LTC_11		= $02;
	PCL_BITS_LTC_CF		= $02;
	PCL_BITS_LTC_27		= $04;
	PCL_BITS_LTC_43		= $08;
	PCL_BITS_LTC_58		= $10;
	PCL_BITS_LTC_59		= $20;
	PCL_BITS_LTC_WHEEL	= $40;
	PCL_BITS_LTC_DIR	= $80;

	PCL_BITS_VITC_14	= $01;
	PCL_BITS_VITC_DROP	= $01;
	PCL_BITS_VITC_15	= $02;
	PCL_BITS_VITC_CF	= $02;
	PCL_BITS_VITC_35	= $04;
	PCL_BITS_VITC_NF	= $04;
	PCL_BITS_VITC_55	= $08;
	PCL_BITS_VITC_74	= $10;
	PCL_BITS_VITC_75	= $20;
	PCL_BITS_VITC_PF	= $20;
	PCL_BITS_VITC_WHEEL	= $40;

{---------------------------------------------------------------------------}

function pclGetRegSync(
	hdl: PCL_INT;
	reg: PCL_INT;
	var time: PCL_TC;
	var bits: PCL_INT;
	var user: PCL_TC;
	var frame: PCL_INT;
	var newd: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetEvent(
	hdl: PCL_INT;
	var dist1: PCL_TC;
	var dist2: PCL_TC;
	var dist3: PCL_TC;
	var evtc: PCL_INT;
	var evub: PCL_INT;
	var eflg: PCL_INT
): PCL_INT; stdcall

const
	PCL_EVTC_FRU	= $01;
	PCL_EVTC_FRT	= $02;
	PCL_EVTC_SEU	= $04;
	PCL_EVTC_SET	= $08;
	PCL_EVTC_MIN	= $10;
	PCL_EVTC_HOU	= $20;
	PCL_EVTC_SIG12	= $40;
	PCL_EVTC_SIG	= $80;

	PCL_EVUB_UB1	= $01;
	PCL_EVUB_UB2	= $02;
	PCL_EVUB_UB3	= $04;
	PCL_EVUB_UB4	= $08;
	PCL_EVUB_UB5	= $10;
	PCL_EVUB_UB6	= $20;
	PCL_EVUB_UB7	= $40;
	PCL_EVUB_UB8	= $80;

	PCL_EFLG_UMASK	= $0F;
	PCL_EFLG_TMASK	= $F0;

{---------------------------------------------------------------------------}

function pclGetVersion(
	hdl: PCL_INT;
	var ident: PCL_INT;
	var firmware: PCL_INT;
	var dll: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetVerSpecial(
	hdl: PCL_INT;
	var ident1: PCL_INT;
	var ident2: PCL_INT;
	var ident3: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclReset(
	hdl: PCL_INT;
	imode: PCL_INT
): PCL_INT; stdcall

const
	PCL_IMODE_NORMAL	= $00;
	PCL_IMODE_TEST		= $F0;

{---------------------------------------------------------------------------}

function pclIntMask(
	hdl: PCL_INT;
	imask: PCL_INT
): PCL_INT; stdcall

const
	PCL_IMASK_NONE	= $00;
	PCL_IMASK_MIXED	= $01;
	PCL_IMASK_CTL	= $02;
	PCL_IMASK_LTC	= $04;
	PCL_IMASK_VITC	= $08;
	PCL_IMASK_ETIME	= $10;
	PCL_IMASK_EUSER	= $20;
	PCL_IMASK_V1	= $40;
	PCL_IMASK_V2	= $80;

{---------------------------------------------------------------------------}

function pclSetIntMask(
	hdl: PCL_INT;
	imask: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclResetIntMask(
	hdl: PCL_INT;
	imask: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

type PCL_INT_RESULT = record
	counter: PCL_DWORD;
	lost: PCL_DWORD;
	stopped: PCL_BOOL;
end;

type PCL_INT_HANDLER = procedure(
	hdl: PCL_INT;
	flag: PCL_INT;
	const intResult: PCL_INT_RESULT
); cdecl;

function pclSetIntHandler(
	hdl: PCL_INT;
	intHandler: PCL_INT_HANDLER
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclIntAck(
	hdl: PCL_INT;
	imask: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclSetIntPriority(
	hdl: PCL_INT;
	priority: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclReaderEnable(
	hdl: PCL_INT;
	rmask: PCL_INT
): PCL_INT; stdcall

const
	PCL_RMASK_NONE		= $0000;
	PCL_RMASK_CTL		= $0001;
	PCL_RMASK_LTC		= $0002;
	PCL_RMASK_VITC		= $0004;
	PCL_RMASK_ATC_LTC	= $0100;
	PCL_RMASK_ATC_VITC1	= $0200;
	PCL_RMASK_ATC_VITC2	= $0400;
	PCL_RMASK_HANC_LTC	= $0800;
	PCL_RMASK_HANC_VITC	= $1000;
	PCL_RMASK_ALL		= $1F06;

{---------------------------------------------------------------------------}

function pclVitcSetup(
	hdl: PCL_INT;
	line1: PCL_INT;
	line2: PCL_INT;
	vline: PCL_INT
): PCL_INT; stdcall

const
	PCL_VLINE_2LINES	= $00;
	PCL_VLINE_BLOCK		= $01;

{---------------------------------------------------------------------------}

function pclMixedEnable(
	hdl: PCL_INT;
	rmask: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclPriority(
	hdl: PCL_INT;
	prior: PCL_INT
): PCL_INT; stdcall

const
	PCL_PRIOR_VLC	= $01;
	PCL_PRIOR_LVC	= $02;

{---------------------------------------------------------------------------}

function pclTcFrames(
	hdl: PCL_INT;
	tcfr: PCL_INT
): PCL_INT; stdcall

const
	PCL_TCFR_AUTO	= $00;
	PCL_TCFR_24	= $01;
	PCL_TCFR_25	= $02;
	PCL_TCFR_30	= $03;
	PCL_TCFR_30DF	= $04;

{---------------------------------------------------------------------------}

function pclLtcFrames(
	hdl: PCL_INT;
	ltcfr: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclVitcFrames(
	hdl: PCL_INT;
	vitcfr: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclVitcLevelControl(
	hdl: PCL_INT;
	vlmode: PCL_INT;
	vlev: PCL_INT;
	var vlret: PCL_INT
): PCL_INT; stdcall

const
	PCL_VLMODE_AUTO		= $00;
	PCL_VLMODE_AUTO_SET	= $01;
	PCL_VLMODE_AUTO_INIT	= $02;
	PCL_VLMODE_OFF		= $03;
	PCL_VLMODE_OFF_SET	= $04;

	PCL_VLEV_MIN		= $00;
	PCL_VLEV_MAX		= $45;

{---------------------------------------------------------------------------}

function pclLtcSequence(
	hdl: PCL_INT;
	lseq: PCL_INT
): PCL_INT; stdcall

const
	PCL_LSEQ_CHECK		= $00;
	PCL_LSEQ_NOCHECK	= $01;

{---------------------------------------------------------------------------}

function pclLtcError(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

const
	PCL_ECMODE_GET_RESET	= $00;
	PCL_ECMODE_GET		= $01;

{---------------------------------------------------------------------------}

function pclVitcError(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclFlyWheel(
	hdl: PCL_INT;
	fwmode: PCL_INT
): PCL_INT; stdcall

const
	PCL_FWMODE_LTC	= $02;
	PCL_FWMODE_VITC	= $04;
	PCL_FWMODE_ALL	= $06;

{---------------------------------------------------------------------------}

function pclAtcLtcError(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclAtcVitc1Error(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclAtcVitc2Error(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclHancLtcError(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclHancVitcError(
	hdl: PCL_INT;
	ecmode: PCL_INT;
	var ecval: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclTc2Bcd(
	hours: PCL_INT;
	minutes: PCL_INT;
	seconds: PCL_INT;
	frames: PCL_INT
): PCL_TC; stdcall;

{---------------------------------------------------------------------------}

function pclBcd2Tc(
	time: PCL_TC;
	var hours: PCL_INT;
	var minutes: PCL_INT;
	var seconds: PCL_INT;
	var frames: PCL_INT
): PCL_INT; stdcall;

{---------------------------------------------------------------------------}

function pclTc2Frames(
	time: PCL_TC;
	frate: PCL_INT
): longint; stdcall;

{---------------------------------------------------------------------------}

function pclGetErrorString
: PChar; stdcall;

{---------------------------------------------------------------------------}

type
	PCL_HANDLE	= cardinal;
	PCL_WORD	= word;
	PCL_VOID_P	= pointer;

type PCL_DRIVER_INFORMATION = record
	size: PCL_INT;
	hWD: PCL_HANDLE;
	sWdVersion: record
		dwVer: PCL_DWORD;
		cVer: array[0..99] of char;
	end;
	wVendorID: PCL_WORD;
	wDeviceID: PCL_WORD;
	wSubsystemVendorID: PCL_WORD;
	wSubsystemDeviceID: PCL_WORD;
	intHandler: PCL_VOID_P;
	chipVer: PCL_INT
end;

function pclGetDriverInformation(
	hdl: PCL_INT;
	info: PCL_DRIVER_INFORMATION
): PCL_INT; stdcall;

{---------------------------------------------------------------------------}

function pclGetIdata(
	hdl: PCL_INT;
	addr: PCL_INT;
	var idata: PCL_INT
): PCL_INT; stdcall;

{---------------------------------------------------------------------------}

function pclGetPdata(
	hdl: PCL_INT;
	addr: PCL_INT;
	var idata: PCL_INT
): PCL_INT; stdcall;

{********************************************************** program options }

{$ifdef PCL_OPT_GETCMD}

function pclOptGetcmd(
	hdl: PCL_INT;
	var rctl: PCL_INT;
	var full: PCL_FULL
): PCL_INT; stdcall

const
	PCL_RCTF_STATS		= $EF;
	PCL_RCTF_NONE		= $FE;
	PCL_RCTF_UNKNOWN	= $FF;

const PCL_OPT_GETCMD = TRUE;

{$else}

const PCL_OPT_GETCMD = FALSE;

{$endif}

{***************************************************************************}

{$ifdef PCL_OPT_REALTIME}

type PCL_REALTIME = record
	ltc: record
		valid: word;
		micro: word;
		milli: word;
		sec: word;
		min: word;
		hour:word;
		day: word;
		month: word;
		year: word;
		tzone: word;
		frame: word;
		_1: word;
	end;
	system: record
		milli: word;
		sec: word;
		min: word;
		hour: word;
		day: word;
		month: word;
		year: word;
		_0, _1, _2: word;
	end;
end;

const
	PCL_RTVAL_INVALID	= $00;
	PCL_RTVAL_TIME		= $01;
	PCL_RTVAL_DATE		= $02;
	PCL_RTVAL_VALID		= $03;

	PCL_RTZONE_UTC		= $00;
	PCL_RTZONE_MEZ		= $01;
	PCL_RTZONE_MESZ		= $02;
	PCL_RTZONE_UNKNOWN	= $03;

{---------------------------------------------------------------------------}

function pclGetRealtime(
	hdl: PCL_INT;
	var tm: PCL_REALTIME
): PCL_INT; stdcall;

const PCL_OPT_REALTIME = TRUE;

{---------------------------------------------------------------------------}

function pclSetRealtimeMode(
	hdl: PCL_INT;
	mode: PCL_INT
): PCL_INT; stdcall;

{$endif}

{---------------------------------------------------------------------------}

{$ifdef PCL_OPT_GPI_OUT}

function pclSetGpiEventTime(
	hdl: PCL_INT,
	gpi: PCL_INT,
	reg: PCL_INT,
	mode: PCL_INT,
	time: PCL_TC
): PCL_INT; stdcall;

const
	PCL_GPIM_PULSE_NEG	= $014A;

{---------------------------------------------------------------------------}

function pclGetGpiEventStatus(
	hdl: PCL_INT,
	gpi: PCL_INT,
	var status: PCL_INT,
	var count: PCL_INT
): PCL_INT; stdcall;

const
	PCL_GPIS_ACTIVE		= $01;

{$endif}

{***************************************************************************}
{*                                                                         *}
{*      Obsolete Declarations                                              *}
{*                                                                         *}
{***************************************************************************}

type
	PCL_OPRT	= PCL_DWORD;
	PCL_OPRT_P	= ^PCL_OPRT;

	PCL_IPRT	= word;
	PCL_IPRT_P	= ^PCL_IPRT;

	PCL_SHOU	= PCL_DWORD;
	PCL_SHOU_P	= ^PCL_SHOU;

	PCL_SHIN	= PCL_DWORD;
	PCL_SHIN_P	= ^PCL_SHIN;

	PCL_TDAT	= array[0..6] of byte;
	PCL_TDAT_P	= ^PCL_TDAT;

	PCL_TSTR	= array[0..8] of byte;
	PCL_TSTR_P	= ^PCL_TSTR;

	PCL_CDAT	= array[0..7] of byte;
	PCL_CDAT_P	= ^PCL_CDAT;

	PCL_SIND	= array[0..12] of byte;
	PCL_SIND_P	= ^PCL_SIND;

	PCL_STAT	= array[0..10] of byte;
	PCL_STAT_P	= ^PCL_STAT;

	PCL_TRAN	= array[0..15] of byte;
	PCL_TRAN_P	= ^PCL_TRAN;

	PCL_FULL	= array[0..16] of byte;
	PCL_FULL_P	= ^PCL_FULL;

const
	PCL_BASE_0		= $200;
	PCL_BASE_1		= $220;
	PCL_BASE_2		= $240;
	PCL_BASE_FACTORY	= $240;
	PCL_BASE_3		= $260;
	PCL_BASE_4		= $280;
	PCL_BASE_5		= $2A0;
	PCL_BASE_6		= $2C0;
	PCL_BASE_7		= $2E0;
	PCL_BASE_8		= $300;
	PCL_BASE_9		= $320;
	PCL_BASE_A		= $340;
	PCL_BASE_B		= $360;
	PCL_BASE_C		= $380;
	PCL_BASE_D		= $3A0;
	PCL_BASE_E		= $3C0;
	PCL_BASE_F		= $3E0;

const
	PCL_REG_CTL	= $01;
	PCL_REG_MIXED2	= $04;
	PCL_REG_CTL2	= $05;
	PCL_REG_LTC2	= $06;
	PCL_REG_VITC2	= $07;
	PCL_REG_EVENT	= $08;
	PCL_REG_EVENT2	= $09;
	PCL_REG_TRAN	= $0A;
	PCL_REG_STATUS	= $0B;
	PCL_REG_IO	= $0C;
	PCL_REG_SERIN	= $0D;
	PCL_REG_SHIFT	= $0E;
	PCL_REG_CMD	= $0F;
	PCL_REG_BANK	= $10;

const
	PCL_NEWD_CTL	= $20;

const
	PCL_BITS_CTL_SIGN	= $40;
	PCL_BITS_CTL_DIR	= $80;

const
	PCL_IMODE_SONY9S	= $01;
	PCL_IMODE_JVC9S		= $02;
	PCL_IMODE_JVC45		= $03;
	PCL_IMODE_PANA34	= $04;
	PCL_IMODE_PANA25	= $05;
	PCL_IMODE_SONY9		= $06;
	PCL_IMODE_JVC9		= $07;
	PCL_IMODE_SONY33	= $08;
	PCL_IMODE_JVC25		= $09;

{---------------------------------------------------------------------------}

function pclResetTrans(
	hdl: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetTransfer(
	hdl: PCL_INT;
	tran: byte
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetIo(
	hdl: PCL_INT;
	var oprt: PCL_OPRT;
	var ana: PCL_INT;
	var iprt: PCL_IPRT
): PCL_INT; stdcall

const
	PCL_OPRT_1	= $000001;
	PCL_OPRT_2	= $000002;
	PCL_OPRT_3	= $000004;
	PCL_OPRT_4	= $000008;
	PCL_OPRT_5	= $000010;
	PCL_OPRT_6	= $000020;
	PCL_OPRT_7	= $000040;
	PCL_OPRT_8	= $000080;
	PCL_OPRT_9	= $000100;
	PCL_OPRT_10	= $000200;
	PCL_OPRT_11	= $000400;
	PCL_OPRT_12	= $000800;
	PCL_OPRT_13	= $001000;
	PCL_OPRT_14	= $002000;
	PCL_OPRT_15	= $004000;
	PCL_OPRT_16	= $008000;
	PCL_OPRT_17	= $010000;
	PCL_OPRT_18	= $020000;
	PCL_OPRT_19	= $040000;
	PCL_OPRT_20	= $080000;
	PCL_OPRT_21	= $100000;
	PCL_OPRT_22	= $200000;
	PCL_OPRT_23	= $400000;
	PCL_OPRT_24	= $800000;

	PCL_IPRT_1	= $0001;
	PCL_IPRT_2	= $0002;
	PCL_IPRT_3	= $0004;
	PCL_IPRT_4	= $0008;
	PCL_IPRT_5	= $0010;
	PCL_IPRT_6	= $0020;
	PCL_IPRT_7	= $0040;
	PCL_IPRT_8	= $0080;
	PCL_IPRT_9	= $0100;
	PCL_IPRT_10	= $0200;
	PCL_IPRT_11	= $0400;
	PCL_IPRT_12	= $0800;
	PCL_IPRT_13	= $1000;
	PCL_IPRT_14	= $2000;
	PCL_IPRT_15	= $4000;

{---------------------------------------------------------------------------}

function pclSetMixed(
	hdl: PCL_INT;
	time: PCL_TC;
	user: PCL_USER;
	status: PCL_INT;
	newd: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetSio(
	hdl: PCL_INT;
	var ssta: PCL_INT;
	var scmd: PCL_INT;
	var sctl: PCL_INT
): PCL_INT; stdcall

const
	PCL_SSTA_PARITY		= $01;
	PCL_SSTA_FRAMING	= $02;
	PCL_SSTA_OVERRUN	= $04;
	PCL_SSTA_RFULL		= $08;
	PCL_SSTA_TEMPTY		= $10;
	PCL_SSTA_RS232		= $00;
	PCL_SSTA_RS422		= $20;
	PCL_SSTA_DSR		= $40;
	PCL_SSTA_IRQ		= $80;

	PCL_SCTL_38400		= $00;
	PCL_SCTL_50		= $01;
	PCL_SCTL_75		= $02;
	PCL_SCTL_110		= $03;
	PCL_SCTL_135		= $04;
	PCL_SCTL_150		= $05;
	PCL_SCTL_300		= $06;
	PCL_SCTL_600		= $07;
	PCL_SCTL_1200		= $08;
	PCL_SCTL_1800		= $09;
	PCL_SCTL_2400		= $0A;
	PCL_SCTL_3600		= $0B;
	PCL_SCTL_4800		= $0C;
	PCL_SCTL_7200		= $0D;
	PCL_SCTL_9600		= $0E;
	PCL_SCTL_19200		= $0F;

	PCL_SCTL_RS232		= $00;
	PCL_SCTL_RS422		= $10;

	PCL_SCTL_B8		= $00;
	PCL_SCTL_B7		= $20;
	PCL_SCTL_B6		= $40;
	PCL_SCTL_B5		= $60;

	PCL_SCTL_S1		= $00;
	PCL_SCTL_S2		= $80;

	PCL_SCMD_RDIS		= $00;
	PCL_SCMD_RENA		= $01;

	PCL_SCMD_NONE		= $00;
	PCL_SCMD_ODD		= $20;
	PCL_SCMD_EVEN		= $60;
	PCL_SCMD_MARK		= $A0;
	PCL_SCMD_SPACE		= $E0;

{---------------------------------------------------------------------------}

function pclGetTrans(
	hdl: PCL_INT;
	var strf: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclGetSerIn(
	hdl: PCL_INT;
	var sind: PCL_SIND;
	var srcv: PCL_INT;
	var count: PCL_INT
): PCL_INT; stdcall

const
	PCL_SRCV_PARITY		= $01;
	PCL_SRCV_FRAMING	= $02;
	PCL_SRCV_OVERRUN	= $04;
	PCL_SRCV_IBUFOV		= $08;
	PCL_SRCV_MBUFOV		= $10;

{---------------------------------------------------------------------------}

function pclGetShift(
	hdl: PCL_INT;
	var shou: PCL_SHOU;
	var shin: PCL_SHIN;
	var shst: PCL_INT
): PCL_INT; stdcall

const
	PCL_SHST_INEWD	= $01;
	PCL_SHST_OENA	= $80;

{-------------------------------------------------------------------------- }

function pclRawCommand(
	hdl: PCL_INT;
	cmd: PCL_INT;
	var cdat: PCL_CDAT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclDataReq(
	hdl: PCL_INT;
	reg: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclReaderSource(
	hdl: PCL_INT;
	rsrc: PCL_INT
): PCL_INT; stdcall

const
	PCL_RSRC_1	= $01;
	PCL_RSRC_2	= $02;

{---------------------------------------------------------------------------}

function pclCtlRate(
	hdl: PCL_INT;
	ctlrate: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclCtlCount(
	hdl: PCL_INT;
	frps: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclCtlMode(
	hdl: PCL_INT;
	cmode: PCL_INT
): PCL_INT; stdcall

const
	PCL_CMODE_UNSIGNED	= $00;
	PCL_CMODE_SIGNED	= $01;

{---------------------------------------------------------------------------}

function pclCtlReset(
	hdl: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclCtlPreset(
	hdl: PCL_INT;
	time: PCL_TC
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclCtlDir(
	hdl: PCL_INT;
	cdir: PCL_INT
): PCL_INT; stdcall

const
	PCL_CDIR_REV	= $00;
	PCL_CDIR_FWD	= $01;

{---------------------------------------------------------------------------}

function pclEventTime(
	hdl: PCL_INT;
	time: PCL_TC
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclEventUser(
	hdl: PCL_INT;
	user: PCL_USER
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclTranBank(
	hdl: PCL_INT;
	bank: PCL_INT;
	var tstr: PCL_TSTR
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclSioCmd(
	hdl: PCL_INT;
	scmd: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclSioCtl(
	hdl: PCL_INT;
	sctl: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclRecMode(
	hdl: PCL_INT;
	rmode: PCL_INT
): PCL_INT; stdcall

const
	PCL_RMODE_NORMAL	= $00;
	PCL_RMODE_SONY		= $01;
	PCL_RMODE_PANA		= $00;

{---------------------------------------------------------------------------}

function pclTranStr(
	hdl: PCL_INT;
	num: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclTranData(
	hdl: PCL_INT;
	num: PCL_INT;
	var tdat: PCL_TDAT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclModeOut(
	hdl: PCL_INT;
	omode: PCL_INT;
	freq: word
): PCL_INT; stdcall

const
	PCL_OMODE_NORMAL	= $00;
	PCL_OMODE_FREQ		= $01;

{---------------------------------------------------------------------------}

function pclOutPort(
	hdl: PCL_INT;
	oprt: PCL_OPRT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclPortHigh(
	hdl: PCL_INT;
	oprt: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclPortLow(
	hdl: PCL_INT;
	oprt: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclPosPulse(
	hdl: PCL_INT;
	oprt: PCL_OPRT;
	pmode: PCL_INT;
	pwidth: PCL_INT
): PCL_INT; stdcall

const
	PCL_PMODE_INDIV	= $00;
	PCL_PMODE_SAME	= $01;

{---------------------------------------------------------------------------}

function pclNegPulse(
	hdl: PCL_INT;
	oprt: PCL_OPRT;
	pmode: PCL_INT;
	pwidth: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclPosPulseOut(
	hdl: PCL_INT;
	oprt: PCL_INT;
	pwidth: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclNegPulseOut(
	hdl: PCL_INT;
	oprt: PCL_INT;
	pwidth: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclPulseWidth(
	hdl: PCL_INT;
	oprt: PCL_OPRT;
	pwidth: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclAnalogOut(
	hdl: PCL_INT;
	value: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclShiftData(
	hdl: PCL_INT;
	shou: PCL_SHOU
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclShiftWidth(
	hdl: PCL_INT;
	swid: PCL_INT
): PCL_INT; stdcall

const
	PCL_SWID_8	= $00;
	PCL_SWID_16	= $01;
	PCL_SWID_24	= $02;
	PCL_SWID_32	= $03;

{---------------------------------------------------------------------------}

function pclShiftMode(
	hdl: PCL_INT;
	smod: PCL_INT
): PCL_INT; stdcall

const
	PCL_SMODE_DIS	= $80;
	PCL_SMODE_ONE	= $00;
	PCL_SMODE_CONT	= $01;

{---------------------------------------------------------------------------}

function pclRecCtl(
	hdl: PCL_INT;
	rctl: PCL_INT
): PCL_INT; stdcall

const
	PCL_RCTL_STOP	= $01;
	PCL_RCTL_PLAY	= $02;
	PCL_RCTL_REC	= $03;
	PCL_RCTL_STILL	= $04;
	PCL_RCTL_FFWD	= $05;
	PCL_RCTL_ADV	= $06;
	PCL_RCTL_FREV	= $07;
	PCL_RCTL_BACK	= $08;
	PCL_RCTL_TRIMF	= $10;
	PCL_RCTL_TRIMR	= $11;
	PCL_RCTL_EJECT	= $21;
	PCL_RCTL_EDON	= $24;
	PCL_RCTL_EDOFF	= $25;

{---------------------------------------------------------------------------}

function pclRecCtlData(
	hdl: PCL_INT;
	rctl: PCL_INT;
	data: PCL_INT
): PCL_INT; stdcall

const
	PCL_RCTD_VFWD		= $12;
	PCL_RCTD_VREV		= $13;
	PCL_RCTD_JFWD		= $14;
	PCL_RCTD_JREV		= $15;
	PCL_RCTD_SFWD		= $1F;
	PCL_RCTD_SREV		= $20;
	PCL_RCTD_EDPR		= $26;

	PCL_EDPR_AUDIO1		= $01;
	PCL_EDPR_AUDIO2		= $02;
	PCL_EDPR_AUDIO3		= $04;
	PCL_EDPR_TIMECODE	= $04;
	PCL_EDPR_AUDIO4		= $08;
	PCL_EDPR_VIDEO		= $10;
	PCL_EDPR_ASSEMBLE	= $20;
	PCL_EDPR_INSERT		= $40;

{---------------------------------------------------------------------------}

function pclRecCtlTc(
	hdl: PCL_INT;
	rctl: PCL_INT;
	time: PCL_TC
): PCL_INT; stdcall

const
	PCL_RCTC_CUEUP	= $16;
	PCL_RCTC_GOTO	= $16;

{---------------------------------------------------------------------------}

function pclRecStat(
	hdl: PCL_INT;
	var stat: PCL_STAT;
	var ident: PCL_INT
): PCL_INT; stdcall

{---------------------------------------------------------------------------}

function pclIoctlError
: PCL_DWORD; stdcall

{---------------------------------------------------------------------------}

function pclGetBaseAddress(
	index: PCL_INT
): PCL_INT; stdcall

{***************************************************************************}
{*}                                                                       {*}
{*}     Implementation                                                    {*}
{*}                                                                       {*}
{***************************************************************************}

function pclOPEN;			stdcall external 'AVPCL32.DLL' index   2;
function pclCLOSE;			stdcall external 'AVPCL32.DLL' index   3;
function pclRAWCOMMAND;			stdcall external 'AVPCL32.DLL' index   4;
function pclRESET;			stdcall external 'AVPCL32.DLL' index   5;
function pclDATAREQ;			stdcall external 'AVPCL32.DLL' index   6;
function pclINTMASK;			stdcall external 'AVPCL32.DLL' index   7;
function pclSETINTMASK;			stdcall external 'AVPCL32.DLL' index   8;
function pclRESETINTMASK;		stdcall external 'AVPCL32.DLL' index   9;
function pclREADERENABLE;		stdcall external 'AVPCL32.DLL' index  10;
function pclMIXEDENABLE;		stdcall external 'AVPCL32.DLL' index  11;
function pclPRIORITY;			stdcall external 'AVPCL32.DLL' index  12;
function pclTCFRAMES;			stdcall external 'AVPCL32.DLL' index  13;
function pclCTLRATE	;		stdcall external 'AVPCL32.DLL' index  14;
function pclCTLCOUNT	;		stdcall external 'AVPCL32.DLL' index  15;
function pclCTLMODE;			stdcall external 'AVPCL32.DLL' index  16;
function pclCTLRESET;			stdcall external 'AVPCL32.DLL' index  17;
function pclCTLPRESET;			stdcall external 'AVPCL32.DLL' index  18;
function pclCTLDIR;			stdcall external 'AVPCL32.DLL' index  19;
function pclEVENTTIME;			stdcall external 'AVPCL32.DLL' index  20;
function pclEVENTUSER;			stdcall external 'AVPCL32.DLL' index  21;
function pclTRANBANK;			stdcall external 'AVPCL32.DLL' index  22;
function pclSIOCMD;			stdcall external 'AVPCL32.DLL' index  23;
function pclSIOCTL;			stdcall external 'AVPCL32.DLL' index  24;
function pclRECMODE;			stdcall external 'AVPCL32.DLL' index  25;
function pclTRANSTR;			stdcall external 'AVPCL32.DLL' index  26;
function pclTRANDATA;			stdcall external 'AVPCL32.DLL' index  27;
function pclMODEOUT;			stdcall external 'AVPCL32.DLL' index  28;
function pclOUTPORT;			stdcall external 'AVPCL32.DLL' index  29;
function pclPORTHIGH;			stdcall external 'AVPCL32.DLL' index  30;
function pclPORTLOW;			stdcall external 'AVPCL32.DLL' index  31;
function pclPOSPULSE;			stdcall external 'AVPCL32.DLL' index  32;
function pclNEGPULSE;			stdcall external 'AVPCL32.DLL' index  33;
function pclPOSPULSEOUT;		stdcall external 'AVPCL32.DLL' index  34;
function pclNEGPULSEOUT;		stdcall external 'AVPCL32.DLL' index  35;
function pclPULSEWIDTH;			stdcall external 'AVPCL32.DLL' index  36;
function pclANALOGOUT;			stdcall external 'AVPCL32.DLL' index  37;
function pclSHIFTDATA;			stdcall external 'AVPCL32.DLL' index  38;
function pclSHIFTWIDTH;			stdcall external 'AVPCL32.DLL' index  39;
function pclSHIFTMODE;			stdcall external 'AVPCL32.DLL' index  40;
function pclRECCTL;			stdcall external 'AVPCL32.DLL' index  41;
function pclRECCTLDATA;			stdcall external 'AVPCL32.DLL' index  42;
function pclRECCTLTC;			stdcall external 'AVPCL32.DLL' index  43;
function pclRECSTAT;			stdcall external 'AVPCL32.DLL' index  44;
function pclGETTC;			stdcall external 'AVPCL32.DLL' index  45;
function pclGETUSER;			stdcall external 'AVPCL32.DLL' index  46;
function pclGETREGISTER;		stdcall external 'AVPCL32.DLL' index  47;
function pclGETREGSYNC;			stdcall external 'AVPCL32.DLL' index  48;
function pclBCD2TC;			stdcall external 'AVPCL32.DLL' index  49;
function pclTC2BCD;			stdcall external 'AVPCL32.DLL' index  50;
function pclGETEVENT;			stdcall external 'AVPCL32.DLL' index  51;
function pclGETTRANSFER;		stdcall external 'AVPCL32.DLL' index  52;
function pclGETIO;			stdcall external 'AVPCL32.DLL' index  53;
function pclGETSIO;			stdcall external 'AVPCL32.DLL' index  54;
function pclRESETTRANS;			stdcall external 'AVPCL32.DLL' index  55;
function pclGETSERIN;			stdcall external 'AVPCL32.DLL' index  56;
function pclGETSHIFT;			stdcall external 'AVPCL32.DLL' index  57;
function pclGETTRANS;			stdcall external 'AVPCL32.DLL' index  58;
function pclGETVERSION;			stdcall external 'AVPCL32.DLL' index  59;
function pclTC2FRAMES;			stdcall external 'AVPCL32.DLL' index  60;
function pclIOCTLERROR;			stdcall external 'AVPCL32.DLL' index  61;
function pclGETBASEADDRESS;		stdcall external 'AVPCL32.DLL' index  62;
function pclLTCSEQUENCE;		stdcall external 'AVPCL32.DLL' index  63;
function pclREADERSOURCE;		stdcall external 'AVPCL32.DLL' index  64;
function pclVITCSETUP;			stdcall external 'AVPCL32.DLL' index  65;
function pclGETVERSPECIAL;		stdcall external 'AVPCL32.DLL' index  66;
function pclSETMIXED;			stdcall external 'AVPCL32.DLL' index  67;
function pclGETERRORSTRING;		stdcall external 'AVPCL32.DLL' index  68;
function pclSetIntHandler;		stdcall external 'AVPCL32.DLL' index  69;
function pclIntAck;			stdcall external 'AVPCL32.DLL' index  70;
function pclSetIntPriority;		stdcall external 'AVPCL32.DLL' index  71;
function pclVitcLevelControl;		stdcall external 'AVPCL32.DLL' index  72;
function pclLtcError;			stdcall external 'AVPCL32.DLL' index  73;
function pclVitcError;			stdcall external 'AVPCL32.DLL' index  74;
function pclFlyWheel;			stdcall external 'AVPCL32.DLL' index  75;
function pclLtcFrames;			stdcall external 'AVPCL32.DLL' index  76;
function pclVitcFrames;			stdcall external 'AVPCL32.DLL' index  77;
function pclGetDriverInformation;	stdcall external 'AVPCL32.DLL' index  78;
function pclGetIdata;			stdcall external 'AVPCL32.DLL' index  79;
function pclGetPdata;			stdcall external 'AVPCL32.DLL' index  80;
{$ifdef PCL_OPT_GPI_OUT}
function pclSetGpiEventTime;	stdcall external 'AVPCL32.DLL' index  81;
function pclGetGpiEventStatus;	stdcall external 'AVPCL32.DLL' index  82;
{$endif}
function pclAtcLtcError;		stdcall external 'AVPCL32.DLL' index  85;
function pclAtcVitc1Error;		stdcall external 'AVPCL32.DLL' index  86;
function pclAtcVitc2Error;		stdcall external 'AVPCL32.DLL' index  87;
function pclHancLtcError;		stdcall external 'AVPCL32.DLL' index  88;
function pclHancVitcError;		stdcall external 'AVPCL32.DLL' index  89;
{$ifdef PCL_OPT_GETCMD}
function pclOPTGETCMD;			stdcall external 'AVPCL32.DLL' index 100;
{$endif}
{$ifdef PCL_OPT_REALTIME}
Function pclGETREALTIME;		stdcall external 'AVPCL32.DLL' index 102;
Function pclSETREALTIMEMODE;		stdcall external 'AVPCL32.DLL' index 103;
{$endif}

{***************************************************************************}

function BCD2HEX(bcd: byte): byte;
begin
   BCD2HEX := (bcd and $0F) + (((bcd and $F0) shr 4) * 10);
end;

{---------------------------------------------------------------------------}

function HEX2BCD(hex: byte): byte;
begin
   HEX2BCD := ((hex div 10) shl 4) + (hex mod 10);
end;

{---------------------------------------------------------------------------}

function HIWORD(l: PCL_DWORD): word;
var w: record l, h: word; end absolute l;
begin
  HIWORD := w.h;
end;

{---------------------------------------------------------------------------}

function LOWORD(l: PCL_DWORD): word;
var w: record l, h: word; end absolute l;
begin
  LOWORD := w.l;
end;

{---------------------------------------------------------------------------}

end.
