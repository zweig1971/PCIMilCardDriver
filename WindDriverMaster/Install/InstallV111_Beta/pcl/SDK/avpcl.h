/***************************************************************************\
*																			*
*	Macros and function prototypes of Windows DLL AVPCL32					*
*																			*
*	Copyright (c) 1995-2007 by Alpermann+Velte								*
*																			*
\***************************************************************************/

/* <avpcl.h> Alpermann+Velte DLL for AV-PCL */
/* @(#)avpcl.h 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte */

#ifndef _AV_AVPCL_H
#define _AV_AVPCL_H

/***************************************************************************/

/*	Features
	--------

	This DLL (PCI version 3.xx) allows access to the following products of
	Alpermann+Velte:

		PCL-PCI LV 3V/5V:	Single VITC/LTC reader with 3V/5V PCI interface
		PCL-PCI LV:			Single VITC/LTC reader with 5V PCI interface
		PCL-PCI D:			Single DVITC/LTC reader with 5V PCI interface

	The DLL ist function compatible to the ISA version 2.xx.

****************************************************************************/

#if defined(_Windows) || defined(_WIN32)
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/

/*	Basics
	------

	AVPCL exports many #defines, typedefs and functions. To avoid naming
	conflicts, all identifiers start with a prefix: 'PCL_xxx' for #defines
	and typedefs and 'pclXxx' for functions.  Identifiers starting with
	a underbar (_) are for internal use only. */

/***************************************************************************/

/*	DLL Version
	-----------
	Version of this DLL at compile time.  Compare this with the dll
	version returned by	pclGetVersion() at run time to be sure not to
	use an outdated version of the DLL. */

#define PCL_VER_DLL 0x0324 // 3.36

/***************************************************************************/

/*	Program options
	---------------

	This options enable special features of AVPCL.  To use any of these,
	a special PCL firmware EPROM is needed. */

//#define PCL_OPT_GETCMD		/* get command bytes from device */

//#define PCL_OPT_MTD			/* read mtd times */

#define PCL_OPT_REALTIME		/* pcl as real time source */

#define PCL_OPT_GPI_OUT			/* event driven gpi outputs */

/***************************************************************************/

/*	Error codes
	-----------

	Most functions return a int error code. A value greater or equal to
	zero means 'no error', negative values mean 'error'. A list of
	possible return codes is shown below: */

#define PCL_OK					  0 /* function successfully completed */

#define PCL_NO_NEWD				  1 /* no new data received */

#define PCL_ERR_INV_PORT		 -1 /* invalid port id */
#define PCL_ERR_NOT_FOUND		 -2 /* pcl hardware not found */
#define PCL_ERR_NO_HANDLE		 -3 /* no free handle found */
#define PCL_ERR_INV_HDL			 -4 /* invalid handle */
#define PCL_ERR_NOTOPEN			 -5 /* handle not opened by pclOpen() */
#define PCL_ERR_NULL			 -6 /* null pointer assignment */
#define PCL_ERR_TIMEOUT			 -7 /* timeout while waiting for pcl ready */
#define PCL_ERR_INV_REG			 -8 /* invalid register number */
#define PCL_ERR_INV_BANK		 -9 /* invalid transmit bank number */
#define PCL_ERR_RANGE			-10 /* parameter out of range */
#define PCL_ERR_INIT			-11 /* can't initialize library */
#define PCL_ERR_IOCTL			-12 /* error in device driver while ioctl */
#define PCL_ERR_VER				-13 /* version of dll and sys different */
#define PCL_ERR_NOT_SUPP		-14 /* function not supported */
#define PCL_ERR_UK_CMD			-15 /* unknown command */
#define PCL_ERR_OPEN			-16 /* can't open WinDriver lib */

/***************************************************************************/

/*	Handles
	-------

	To access the PCL card, you need to open a communication port with
	pclOpen() described later. It is possible to handle several PCL cards
	simultaneous, you need to open a communication port for each PCL card.
	You find the number of ports available in PCL_HANDLES: */

#define PCL_HANDLES				10 /* number of handles available */

/*	Access modes
	------------

	There are three different modes to access a timecode reader register.
	This modes are named PCL_REQ_xxx: */

/*		PCL_REQ_xxx						timecode request mode */
#define PCL_REQ_ASYN			0x80 /* asynchron */
#define PCL_REQ_SYNC			0x40 /* synchron */
#define PCL_REQ_PERM			0x20 /* permanent */

/***************************************************************************/

/*	BCD/Hex conversion
	------------------

	Several parameters of the PCL card are coded BCD. To convert binary
	numbers to BCD and vice versa, you might find the following macros
	useful: */

#define BCD2HEX(bcd) (((bcd) & 0x0F) + ((((bcd) & 0xF0) >> 4) * 10))
								/* convert unsigned char from bcd to hex */

#define HEX2BCD(hex) ((((hex) / 10) << 4) + ((hex) % 10))
								/* convert unsigned char from hex to bcd */

/*	Some Macros from windows.h
	-------------------------- */

#ifndef MAKEWORD
#define MAKEWORD(a, b)	((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif

#ifndef MAKELONG
#define MAKELONG(a, b)	((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif

#ifndef LOWORD
#define LOWORD(l)		((WORD)(l))
#endif

#ifndef HIWORD
#define HIWORD(l)		((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif

#ifndef LOBYTE
#define LOBYTE(w)		((BYTE)(w))
#endif

#ifndef HIBYTE
#define HIBYTE(w)		((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif

/***************************************************************************/

/*	Typedefs
	-------- */

/* Some Typedefs from windows.h */

#if !defined(WINAPI) || defined(_NTDDK_)

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char BYTE;							/* size is 1 */
#endif // !_BYTE_DEFINED

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short WORD;						/* size is 2 */
#endif // !_WORD_DEFINED

#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
typedef unsigned long DWORD;						/* size is 4 */
#endif // !_DWORD_DEFINED

#endif

typedef int   PCL_BOOL;

typedef DWORD PCL_TC;		/* timecode, bcd */

/*	Timecode is coded BCD in a double word.
	0xhhmmssff with hh = hours, mm = minutes, ss = seconds, ff = frames
	e.g. 0x12345621 means 12:34:56:21 */

typedef DWORD PCL_USER;		/* user-bits, bcd */

/*	User-Bits are coded similar to timecode described above. */

typedef BYTE  PCL_FULL[16]; /* full pcl register */

/*	Pcl data corresponds to data $0 to $F of any register of PCL card */

/***************************************************************************/

/*	Calling conventions
	------------------- */

#if defined(_WINNT) || defined(_NTDDK_) || defined(_WIN32)
#	define _PCL_FDEF _stdcall
#	define _PCL_IN 
#	define _PCL_OUT 
#elif defined(LINUX)
#	define _PCL_FDEF
#	define _PCL_IN 
#	define _PCL_OUT
#else
#	ifndef PASCAL
#		define PASCAL _pascal
#	endif
#	ifndef FAR
#		define FAR _far
#	endif
#	define _PCL_FDEF FAR PASCAL
#	define _PCL_IN	FAR
#	define _PCL_OUT  FAR
#endif

/***************************************************************************\
*																			*
* Function Prototypes														*
*																			*
\***************************************************************************/

/*	The functions are described as followed:

	int _PCL_FDEF pclRecStat(	/# @44,	$91, recorder status request #/
		int hdl,				/# in:	handle from pclOpen() #/
		BYTE _PCL_OUT *stat,	/# out:	recorder status, like PCL_STAT #/
		int _PCL_OUT *ident		/# out:	recorder identification flag #/
	);

	'@44'  is the decimal dll function number as defined in the *.def file.
	'$91'  is the hex function number as described in the PCL manual.
	'in:'  are parameters passing something *to* the function.
	'out:' are parameters passing something *from* the function. */

/*-------------------------------------------------------------------------*/

#if !defined(_Windows) && !defined(LINUX)

int _PCL_FDEF pclInitLib(	/* initialize library avpcl */
	void					/* no parameters needed */
);

#endif

/* Initialization is only needed with DOS drivers.  Windows DLLs are
	initialized automatically */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclOpen(	/* @2,	open pcl communication channel */
	int _PCL_OUT *hdl,  /* out:	handle to access channel */
	unsigned base		/* in:	base address of pcl card, like
								PCL_BASE_xxx */
);

#define PCL_BASE_PCI_OFS	0x1000 /* offset to address PCL PCI cards */
#define PCL_BASE_PCI_1		0x1001 /* 1st PCL PCI card */
#define PCL_BASE_PCI_2		0x1002 /* 2nd PCL PCI card */
#define PCL_BASE_PCI_3		0x1003 /* 3rd PCL PCI card */
#define PCL_BASE_PCI_4		0x1004 /* 4th PCL PCI card */
#define PCL_BASE_PCI_5		0x1005 /* 5th PCL PCI card */
#define PCL_BASE_PCI_6		0x1006 /* 6th PCL PCI card */
#define PCL_BASE_PCI_7		0x1007 /* 7th PCL PCI card */
#define PCL_BASE_PCI_8		0x1008 /* 8th PCL PCI card */
#define PCL_BASE_PCI_9		0x1009 /* 9th PCL PCI card */
#define PCL_BASE_PCI_10		0x100A /* 10th PCL PCI card */
#define PCL_BASE_PCI_LAST	PCL_BASE_PCI_10

/*	Before you can use any of the pclXxx() functions, you have to get a
	handle by calling pclOpen().  Use this handle to call subsequent
	pclXxx functions.  Don't forget to call pclClose() before your
	program terminates.

	Example:

	// Simple program to read a single timecode value
	#inlcude <stdio.h>
	#include "avpcl.h"
	void main(void)
	{
		int hdl;
		PCL_TC time;
		if (pclOpen(&hdl, PCL_BASE_PCI_1) == PCL_OK) {
			pclGetTc(hdl, &time);
			printf("Current Timecode is: %8X\n", time);
			pclClose(hdl);
		} else {
			puts("Can't open PCL card");
		}
	}
*/

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclClose(	/* @3,	close pcl communication channel */
	int hdl				/* in:	handle from pclOpen() */
);

/*	pclClose() terminates the connection to the pcl card.  See pclOpen()
	for a example. */

/*---------------------------------------------------- read mixed register */

int _PCL_FDEF pclGetTc(		/* @45,	read mixed-register timecode
									asynchronous */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_TC _PCL_OUT *time	/* out:	timecode from mixed-register, bcd */
);

/*	This is the simplest way to get the time from the mixed register,
	but it isn't the most efficient, because it sends a request to the pcl
	card and waits for the response.  If you need to get a timecode
	permanetly, it's better to use pclGetRegSync. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetUser(	/* @46,	read mixed-register user-bits asynchr. */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_USER _PCL_OUT *user	/* out:	user-bits from mixed-register */
);

/*	The simplest way to get the user bits from the mixed register of the
	pcl card.  See pclGetTc for details. */

/*------------------------------------------------- read timecode register */

int _PCL_FDEF pclGetRegister(	/* @47,	read register asynchronous */
	int hdl,					/* in:	handle from pclOpen() */
	int reg,					/* in:	registers number like PCL_REG_xxx */
	PCL_TC _PCL_OUT *time,		/* out:	timecode from register, bcd */
	int _PCL_OUT *bits,			/* out:	misc. bits from byte $3 of register,
										meaning depends of '*newd',
										like PCL_BITS_xxx_xxx */
	PCL_TC _PCL_OUT *user,		/* out:	user-bits from register, bcd */
	int _PCL_OUT *frame,		/* out:	framerate as detected by pcl,
										like PCL_FRAME_xxx */
	int _PCL_OUT *newd			/* out:	identifier of readed timecode,
										like PCL_NEWD_xxx */
);

/* Note:  Set several 'out' parameters to NULL if you don't need them */

/*		PCL_REG_xxx					register numbers */
#define PCL_REG_MIXED		0x00 /* mixed register */
#define PCL_REG_LTC			0x02 /* ltc register */
#define PCL_REG_VITC		0x03 /* vitc register */

#define PCL_REG_ATC_LTC		0x11 /* atc ltc register */
#define PCL_REG_ATC_VITC1	0x12 /* atc vitc 1 register */
#define PCL_REG_ATC_VITC2	0x13 /* atc vitc 2 register */
#define PCL_REG_HANC_LTC	0x14 /* hanc ltc register */
#define PCL_REG_HANC_VITC	0x15 /* hanc vitc register */

/*		PCL_FRAME_xxx				timecode framerate from register */
#define PCL_FRAME_UNKNOWN	0x00 /* unknown */
#define PCL_FRAME_24		0x24 /* 24 */
#define PCL_FRAME_25		0x25 /* 25 */
#define PCL_FRAME_30		0x30 /* 30 */
#define PCL_FRAME_30DF		0xB0 /* 30df */

/*		PCL_NEWD_xxx				timecode new data flag */
#define PCL_NEWD_NONE		0x00 /* no new data */
#define PCL_NEWD_VITC		0x80 /* vitc */
#define PCL_NEWD_LTC		0x40 /* ltc */

/*		PCL_BITS_xxx_xxx			timecode bits */

/*		PCL_BITS_xxx				generic bits */
#define PCL_BITS_DROP		0x01 /* drop */
#define PCL_BITS_CF			0x02 /* cf */
#define PCL_BITS_WHEEL		0x40 /* fly wheel mode */

/*		PCL_BITS_LTC_xxx			ltc bits */
#define PCL_BITS_LTC_10		0x01 /* bit 10 */
#define PCL_BITS_LTC_DROP	0x01 /* drop */
#define PCL_BITS_LTC_11		0x02 /* bit 11 */
#define PCL_BITS_LTC_CF		0x02 /* cf */
#define PCL_BITS_LTC_27		0x04 /* bit 27 */
#define PCL_BITS_LTC_43		0x08 /* bit 43 */
#define PCL_BITS_LTC_58		0x10 /* bit 58 */
#define PCL_BITS_LTC_59		0x20 /* bit 59 */
#define PCL_BITS_LTC_WHEEL	0x40 /* fly wheel mode */
#define PCL_BITS_LTC_DIR	0x80 /* direction */

/*		PCL_BITS_VITC_xxx			vitc bits */
#define PCL_BITS_VITC_14	0x01 /* bit 14 */
#define PCL_BITS_VITC_DROP	0x01 /* drop */
#define PCL_BITS_VITC_15	0x02 /* bit 15 */
#define PCL_BITS_VITC_CF	0x02 /* cf */
#define PCL_BITS_VITC_35	0x04 /* bit 35 */
#define PCL_BITS_VITC_NF	0x04 /* ntsc field mark */
#define PCL_BITS_VITC_55	0x08 /* bit 55 */
#define PCL_BITS_VITC_74	0x10 /* bit 74 */
#define PCL_BITS_VITC_75	0x20 /* bit 75 */
#define PCL_BITS_VITC_PF	0x20 /* pal field mark */
#define PCL_BITS_VITC_WHEEL	0x40 /* fly wheel mode */

/*	If you need additional informations from any timecode register of
	the pcl card, you can use pclGetRegister().

	pclGetRegister(hdl, PCL_REG_MIXED, &time, NULL, NULL, NULL, NULL);
	is equivalent to pclGetTc(hdl, &time). */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetRegSync(	/* @48,	read register synchronous */
	int hdl,					/* in:	handle from pclOpen() */
	int reg,					/* in:	registers number like PCL_REG_xxx */
	PCL_TC _PCL_OUT *time,		/* out:	timecode from register, bcd */
	int _PCL_OUT *bits,			/* out:	misc. bits from byte $3 of register,
										meaning depends of '*newd',
										like PCL_BITS_xxx_xxx */
	PCL_TC _PCL_OUT *user,		/* out:	user-bits from register, bcd */
	int _PCL_OUT *frame,		/* out:	framerate as detected by pcl,
										like PCL_FRAME_xxx */
	int _PCL_OUT *newd			/* out:	identifier of readed timecode,
										like PCL_NEWD_xxx */
);

/*	Note:
	Set several 'out' parameters to NULL if you don't need them */

/*	pclGetRegSync doesn't wait for the pcl to
	respond.  pclGetRegSync first checks if there is a new timecode in the
	specified register.  If no, it returns PCL_NO_NEWD.  If there is
	a new timecode, it reads the value, sends a new request to
	the pcl and returns PCL_OK.  pclGetRegSync never waits for the pcl card!

	If there is a call to pclGetTc() similar to this...

		while (running) {
			PCL_TC time;
			pclGetTc(hdl, &time);
			doAnything(time)
		}

	...it might be replaced with something like that:

		pclGetRegSync(hdl, PCL_REG_MIXED, NULL, NULL, NULL, NULL, NULL)
		while (running) {
			PCL_TC time;
			if (
				pclGetRegSync(
					hdl,
					PCL_REG_MIXED, &time,
					NULL, NULL, NULL, NULL
				) == PCL_OK
			) {
				doAnything(time);
			} else {
				doAnythingElse();
			}
		}

	Please note, that pclGetRegSync() only returns PCL_OK if there is a new
	timecode readed in the specified register.  This is opposite to
	pclGetTc() which allways returns a timecode.  In the example above,
	with pclGetTc() doAnything() is called in every loop, but with
	pclGetRegSync() it is only called if there is a new timecode readed
	by the pcl card.  The first call to pclGetRegSync() outside the loop
	gurantees that doAnything() isn't called with a timecode that was
	readed before the program starts. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetRegStatus(	/* @84,	read register status */
	int hdl,					/* in:	handle from pclOpen() */
	int reg,					/* in:	registers number like PCL_REG_xxx */
	int _PCL_OUT *stat			/* out:	status bytes, like PCL_STAT_xxx */
);

#define	PCL_STAT_DBB1					0x00FF	// ATC DBB1 mask
#define	PCL_STAT_DBB1_LTC				0x0000	// ATC Longitudinal time code
#define	PCL_STAT_DBB1_VITC1				0x0001	// ATC Vertical interval time code #1
#define	PCL_STAT_DBB1_VITC2				0x0002	// ATC Vertical interval time code #2
#define	PCL_STAT_DBB1_FILM_DB_TRANS		0x0006	// ATC Film data block (transferred)
#define	PCL_STAT_DBB1_PROD_DB_TRANS		0x0007	// ATC Production data block (transferred)
#define	PCL_STAT_DBB1_VTAPE_DB_LOC		0x007D	// ATC Video tape data block (locally generated)
#define	PCL_STAT_DBB1_FILM_DB_LOC		0x007E	// ATC Film data block (locally generated)
#define	PCL_STAT_DBB1_PROD_DB_LOC		0x007F	// ATC Production data block (locally generated)

#define	PCL_STAT_DBB2					0xFF00	// ATC DBB2 mask
#define	PCL_STAT_DBB2_VITC_LINE_SELECT	0x1F00	// ATC VITC line select
#define	PCL_STAT_DBB2_VITC_LINE_DUP		0x2000	// ATC VITC line duplication
#define	PCL_STAT_DBB2_TC_VALIDITY		0x4000	// ATC time code validity
#define	PCL_STAT_DBB2_PROCESS_BIT		0x8000	// ATC (User bits) process bits

#define	PCL_STAT_HANC					0xFF00	// HANC VITC status mask
#define	PCL_STAT_HANC_VITC_LINE_SELECT	0x3F00	// HANC VITC line select
#define	PCL_STAT_HANC_VITC_LINE_SELECT_DIV	0x100	// HANC VITC line select divisor
#define	PCL_STAT_HANC_VITC_LINE_DUP		0x4000	// HANC VITC line duplication
#define	PCL_STAT_HANC_VALIDITY			0x8000	// HANC VITC validity flag

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetVersion(	/* @59,	read pcl firmware version number */
	int hdl,					/* in:	handle from pclOpen() */
	int _PCL_OUT *ident,		/* out:	software identification, != 0
										means special design */
	int _PCL_OUT *firmware,		/* out:	pcl firmware revision number:

										bit 76543210
											--------
											00			PCL3/PCL6
											01			reserved
											10			PCL10/PCL5
											11			PCL12
											  xx		main version, 0..3
												xxxx	sub version,  0..F */

	int _PCL_OUT *dll			/* out: dll revision number:

											111111
										bit 543210987654321
											---------------
											xxxxxxxx		main version
													xxxxxxx sub version  */
);

/*	Note: Set several 'out' parameters to NULL if you don't need them */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetVerSpecial(	/* @66,	read special pcl firmware version */
	int hdl,					/* in:	handle from pclOpen() */
	int _PCL_OUT *ident1,		/* out:	identifier from byte $a */
	int _PCL_OUT *ident2,		/* out:	identifier from byte $b */
	int _PCL_OUT *ident3		/* out:	identifier from byte $c */
);

/*	Note: Set several 'out' parameters to NULL if you don't need them */

/***************************************************************************\
*																			*
* Commands																	*
*																			*
\***************************************************************************/

/* Please refer to chapter C1 (description of commands) for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclReset(		/* @5,	$01, reset: initialize pcl card */
	int hdl,				/* in:	handle from pclOpen() */
	int imode				/* in:	operation mode of pcl, like
									PCL_IMODE_xxx */
);

/*		PCL_IMODE_xxx				operation mode of pcl card */
#define PCL_IMODE_NORMAL	0x00 /* normal mode */
#define PCL_IMODE_POR		0x80 /* force power-on reset */
#define PCL_IMODE_TEST		0xF0 /* reserved (test mode) */

/*	Use pclReset() if you want to change the operating mode of the pcl card.
	It also performs a soft reset, resetting all register values to
	their power-on values. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclIntMask(	/* @7,	$03, interrupts: set hw interrupt mask */
	int hdl,				/* in:	handle from pclOpen() */
	int imask				/* in:	interrupt mask, like PCL_IMASK_xxx */
);

/*		PCL_IMASK_xxx				interrupt mask */
#define PCL_IMASK_NONE		0x0000 /* disable all interrups */
#define PCL_IMASK_MIXED		0x0001 /* mix disable/enable */
#define PCL_IMASK_LTC		0x0004 /* ltc disable/enable */
#define PCL_IMASK_VITC		0x0008 /* vitc disable/enable */
#define PCL_IMASK_V1		0x0040 /* odd video field */
#define PCL_IMASK_V2		0x0080 /* even video field */

#define PCL_IMASK_ATC_LTC	0x0100 /* atc ltc disable/enable */
#define PCL_IMASK_ATC_VITC1	0x0200 /* atc vitc 1 disable/enable */
#define PCL_IMASK_ATC_VITC2	0x0400 /* atc vitc 2 disable/enable */
#define PCL_IMASK_HANC_LTC	0x0800 /* hanc ltc disable/enable */
#define PCL_IMASK_HANC_VITC	0x1000 /* hanc vitc disable/enable */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSetIntMask(	/* @8,	$04, set interrupt bits */
	int hdl,					/* in:	handle from pclOpen() */
	int imask					/* in:	interrupt mask, like PCL_IMASK_xxx */
);

/*	Set one or more bits of the interrupt mask, i.e. enable it. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclResetIntMask(	/* @9,	$05, reset interrupt bits */
	int hdl,					/* in:	handle from pclOpen() */
	int imask					/* in:	interrupt mask, lk. PCL_IMASK_xxx */
);

/*	Reset one or more bits of the interrupt mask, i.e. disable it. */

/*-------------------------------------------------------------------------*/

#if defined(_Windows) || defined(_WIN32)

typedef struct
{
	DWORD counter;	/* number of interrupts received */
	DWORD lost; 	/* number of interrupts not yet dealt with */
	BOOL stopped;	/* was interrupt disabled during wait */
} PCL_INT_RESULT;

typedef void (__cdecl *PCL_INT_HANDLER) (	/* prototype of interrupt
													handler */
	int hdl,								/* in:	handle from pclOpen() */
	int flag,								/* in:	interupt flag, like
													PCL_IMASK_xxx */
	const PCL_INT_RESULT *intResult			/* in:	interrupt result */
);

int _PCL_FDEF pclSetIntHandler(	/* @69,	set interrupt handler */
	int hdl,					/* in:	handle from pclOpen() */
	PCL_INT_HANDLER intHandler	/* in:	address of interrupt handler */
);

/*	Set the interrupt handler routine.  That is a callback function
	that is called for every hardware interrupt.  See the declaration of
	PCL_INT_HANDLER above.
	The handler gets a PCL Handle (hdl), a interrupt request flag (flag)
	and a pointer to a interrupt information structure (intResult).  See
	declaration of PCL_INT_RESULT above.  With that informations the
	handler is able to get the new time code, the interrupt request is set
	from.
	If several interrupts requests are pending simultaneously, the
	handler is called several times, each time with another flag bit set,
	MSB first.
	There is no need to acknowledge the interrupt request.  This is done
	by the PCL driver automatically.
*/

#endif

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclIntAck(	/* @70,	interrupt acknowledge */
	int hdl,				/* in:	handle from pclOpen() */
	int imask				/* in:	interrupt mask, lk. PCL_IMASK_xxx */
);

/*	Under normal conditions, this function is never needed, because
	the PCL driver acknowledges every interrupt automatically.
*/

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSetIntPriority(	/* @71, set interrupt priority */
   int hdl,							/* in:	handle from pclOpen() */
   int priority						/* in:	priority of interrupt handler */
);

/*	Set the priority of the interrupt handler thread.  Setting priority
	to THREAD_PRIORITY_TIME_CRITICAL is a possible way to make the
	interupt handling of Windows more strict to the real world.
	Refer to SetThreadPriority() in the Win32 documentation for details.
*/

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetThreadHandle(	// @90, get thread handle
   int hdl,							// in:	handle from pclOpen()
   int thread,						// in:	thread to use, like PCL_THREAD_xxx
   HANDLE *phHandle					// out:	handle of thread
);

/*		PCL_THREAD_xxx				thread number */
#define PCL_THREAD_INT		0x0001 /* interrupt thread */
#define PCL_THREAD_EVENT	0x0002 /* event handler thread */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclReaderEnable(	/* @10,	$10, reader on/off */
	int hdl,					/* in:	handle from pclOpen() */
	int rmask					/* in:	reader mask, like PCL_RMASK_xxx */
);

/*		PCL_RMASK_xxx				reader mask */
#define PCL_RMASK_NONE		0x0000 /* disable all readers */
#define PCL_RMASK_LTC		0x0002 /* ltc off/on */
#define PCL_RMASK_VITC		0x0004 /* vitc off/on */
#define PCL_RMASK_ATC_LTC	0x0100 /* atc ltc off/on */
#define PCL_RMASK_ATC_VITC1	0x0200 /* atc vitc1 off/on */
#define PCL_RMASK_ATC_VITC2	0x0400 /* atc vitc2 off/on */
#define PCL_RMASK_HANC_LTC	0x0800 /* hanc ltc off/on */
#define PCL_RMASK_HANC_VITC	0x1000 /* hanc vitc off/on */
#define PCL_RMASK_ALL		0x1F06 /* enable all readers */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclVitcSetup(	/* @65,	$14, vitc setup (pcl5/6) */
	int hdl,				/* in:	handle from pclOpen() */
	int line1,				/* in:	1st line, hexadecimal */
	int line2,				/* in:	2nd line, hexadecimal */
	int vline				/* in:	vitc line mode, like PCL_VLINE_xxx */
);

/*		PCL_VLINE_xxx				vitc line mode */
#define PCL_VLINE_2LINES	0x00 /* 2-lines mode */
#define PCL_VLINE_BLOCK		0x01 /* block mode */

/*	Note: pclVitcSetup is not supported by PCL firmware version less or
		equal V1.D */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclMixedEnable(	/* @11,	$20, mixed register: reader on/off */
	int hdl,					/* in:	handle from pclOpen() */
	int rmask					/* in:	reader mask, like PCL_RMASK_xxx */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclPriority(	/* @12,	$21, priority: mixed reg. tc priority */
	int hdl,				/* in:	handle from pclOpen() */
	int prior				/* in:	priority, like PCL_PRIOR_xxx */
);

/*		PCL_PRIOR_xxx				timecode priority */
#define PCL_PRIOR_VLC		0x01 /* vitc-ltc-ctl */
#define PCL_PRIOR_LVC		0x02 /* ltc-vitc-ctl */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclTcFrames(	/* @13,	$30, tc frame rate */
	int hdl,				/* in:	handle from pclOpen() */
	int tcfr				/* in:	frame rate, like PCL_TCFR_xxx */
);

/*		PCL_TCFR_xxx				tc frame rate */
#define PCL_TCFR_AUTO		0x00 /* automatic */
#define PCL_TCFR_24			0x01 /* 24 frames */
#define PCL_TCFR_25			0x02 /* 25 frames */
#define PCL_TCFR_30			0x03 /* 30 frames non frop mode */
#define PCL_TCFR_30DF		0x04 /* 30 frames drop mode */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclLtcFrames(	/* @76,	$31, set ltc frame rate */
	int hdl,				/* in:	handle from pclOpen() */
	int ltcfr				/* in:	ltc frame rate, like PCL_TCFR_xxx */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclVitcFrames(	/* @77,	$32, set vitc frame rate */
	int hdl,					/* in:	handle from pclOpen() */
	int vitcfr					/* in:	vitc frame rate, like PCL_TCFR_xxx */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclVitcLevelControl(	/* @72,	$16, vitc level control */
	int hdl,						/* in:	handle from pclOpen() */
	int vlmode,						/* in:	vitc level control mode,
											like PCL_VLMODE_xxx */
	int vlev,						/* in:	vitc level, binary,
											like PCL_VLEX_xxx */
	int *vlret						/* out:	vitc level return, binary,
											like PCL_VLEX_xxx */
);

/*		PCL_VLMODE_xxx					vitc level control mode */
#define PCL_VLMODE_AUTO			0x00 /* automatic (default) */
#define PCL_VLMODE_AUTO_SET		0x01 /* automatic with preset value */
#define PCL_VLMODE_AUTO_INIT	0x02 /* automatic with initialization */
#define PCL_VLMODE_OFF			0x03 /* switch automatic off */
#define PCL_VLMODE_OFF_SET		0x04 /* switch automatic off, preset value */

/*		PCL_VLEV_xxx					vitc level range */
#define PCL_VLEV_MIN			0x00 /* minimum value */
#define PCL_VLEV_MAX			0x45 /* maximum value */

/* Note:  Set 'out' parameter to NULL if you don't need it */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclLtcSequence(	/* @63,	$11, ltc sequence */
	int hdl,					/* in:	handle from pclOpen() */
	int lseq					/* in:	ltc sequence, like PCL_LSEQ_xxx */
);

/*		PCL_LSEQ_xxx				ltc sequence */
#define PCL_LSEQ_CHECK		0x00 /* sequence check on */
#define PCL_LSEQ_NOCHECK	0x01 /* sequence check off */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclLtcError(	/* @73,	$17, get ltc error counter */
	int hdl,				/* in:	handle from pclOpen() */
	int ecmode,				/* in:	error counter mode, like PCL_ECMODE_xxx */
	int *ecval				/* out:	error counter, 8 bit binary */
);

/*		PCL_ECMODE_xxx					error counter mode */
#define PCL_ECMODE_INITIALIZE	0x00 /* get error counter, initialize counter afterwards */
#define PCL_ECMODE_GET			0x01 /* get error counter, leave it unchanged */
#define PCL_ECMODE_GET_RESET	0x02 /* get error counter, reset it afterwards */

/* Note:  Set 'out' parameter to NULL if you don't need it */

/* Note:  To use PCL_ECMODE_GET_RESET, you need PCL PCI Firmware Version 4
          or higher */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclVitcError(	/* @74,	$18, get vitc error counter */
	int hdl,				/* in:	handle from pclOpen() */
	int ecmode,				/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval				/* out:	error counter, 8 bit binary */
);

/* Note:  Set 'out' parameter to NULL if you don't need it */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclFlyWheel(	/* @75,	$19, set flywheel mode */
	int hdl,				/* in:	handle from pclOpen() */
	int fwmode				/* in:	fly wheel mode, like PCL_FWMODE_xxx */
);

/*		PCL_FWMODE_xxx				fly wheel mode bit mask */
#define PCL_FWMODE_OFF		0x00 /* disable all fly wheel counters */
#define PCL_FWMODE_LTC		0x02 /* enable ltc fly wheel counter */
#define PCL_FWMODE_VITC		0x04 /* enable vitc fly wheel counter */
#define PCL_FWMODE_ALL		0x06 /* enable both fly wheel counters */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAtcLtcError(	/* @85,	$22, get atc ltc error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_ECMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
);

/* Note:  Set 'out' parameter to NULL if you don't need it */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAtcVitc1Error(	/* @86,	$23, get atc vitc1 error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
);

/* Note:  Set 'out' parameter to NULL if you don't need it */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAtcVitc2Error(	/* @87,	$24, get atc vitc2 error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
);

/* Note:  Set 'out' parameter to NULL if you don't need it */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclHancLtcError(	/* @88,	$25, get hanc ltc error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_ECMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
);

/* Note:  Set 'out' parameter to NULL if you don't need it */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclHancVitcError(	/* @89,	$26, get hanc vitc error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
);

/* Note:  Set 'out' parameter to NULL if you don't need it */

/***************************************************************************\
*																			*
* Internal functions														*
*																			*
\***************************************************************************/

/* The following functions are internal to AVPCL32.DLL.  Not data is
	transfered to or from PCL card. */

/*-------------------------------------------------------------------------*/

PCL_TC _PCL_FDEF pclTc2Bcd(	/* @50,	convert binary timecode to packed bcd */
	int hours,				/* in:	hours of binary timecode */
	int minutes,			/* in:	minutes of binary timecode */
	int seconds,			/* in:	seconds of binary timecode */
	int frames				/* in:	frames of binary timecode */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclBcd2Tc(	/* @49,	convert packed bcd timecode to binary */
	PCL_TC time,			/* in:	packed bcd timecode */
	int _PCL_OUT *hours,	/* out:	hours of binary timecode */
	int _PCL_OUT *minutes,	/* out:	minutes of binary timecode */
	int _PCL_OUT *seconds,	/* out:	seconds of binary timecode */
	int _PCL_OUT *frames	/* out:	frames of binary timecode */
);

/* Note:  Set several 'out' parameters to NULL if you don't need them */

/*-------------------------------------------------------------------------*/

long _PCL_FDEF pclTc2Frames(	/* @60,	convert packed bcd timecode to long */
	PCL_TC time,				/* in:	packed bcd timecode */
	int frate					/* in:	frame rate (25 or 30) */
);

/*-------------------------------------------------------------------------*/

const char * _PCL_FDEF pclGetErrorString(	/* @68, get error string */
	void									/* no parameter needed */
);

/*	Note: The function returns a pointer to a error string, if the error
	happens in the low-level part of the device driver.  Otherwise
	it returns a pointer to an empty string. */

/*-------------------------------------------------------------------------*/

#if defined(_Windows) || defined(_WIN32)

typedef struct {
	int size;
	HANDLE hWD;
	struct
	{
	    DWORD dwVer;
	    CHAR cVer[100];
	} sWdVersion;
	WORD wVendorID;
	WORD wDeviceID;
	WORD wSubsystemVendorID;
	WORD wSubsystemDeviceID;
	void *intHandler;
	int chipVer;
} PCL_DRIVER_INFORMATION;

int _PCL_FDEF pclGetDriverInformation(		/* @78, get driver information */
	int hdl,								/* in:	handle from pclOpen() */
	PCL_DRIVER_INFORMATION _PCL_OUT *info	/* out:	information data structure */
);

/*	Note: The function returns internal informations about the driver.
    For normal operation, this function is never needed.
	However, you may use the field wSubsystemDeviceID to detect to what type
	of card you're connected:
*/
#endif

#define PCL_PCI_SUBSYSTEM_DEVICE_ID_LV_5V	0x2221
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_D_5V	0x2431
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_LV_3V	0x2862
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_TS_5V	0x2905
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_TS_3V	0x2906
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_D_3V	0x2940
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_L_3V	0x3025
#define PCL_PCI_SUBSYSTEM_DEVICE_ID_HD_3V	0x3068


/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetIdata(	/* @79, get idata */
	int hdl,				/* in:	handle from pclOpen() */
	int addr,				/* in:	idata address */
	int _PCL_OUT *idata		/* out:	byte read from idata */
);

/*	Note: The function returns internal informations about the driver.
    For normal operation, this function is never needed. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetPdata(	/* @80, get pdata */
	int hdl,				/* in:	handle from pclOpen() */
	int addr,				/* in:	pdata address */
	int _PCL_OUT *pdata		/* out:	byte read from pdata */
);

/*	Note: The function returns internal informations about the driver.
    For normal operation, this function is never needed. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetXdata(	/* @83, get xdata */
	int hdl,				/* in:	handle from pclOpen() */
	int addr,				/* in:	xdata address */
	int _PCL_OUT *xdata		/* out:	byte read from xdata */
);

/*	Note: The function returns internal informations about the driver.
    For normal operation, this function is never needed. */

/***************************************************************************/

#ifdef PCL_OPT_GPI_OUT			/* event driven gpi outputs */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSetGpiEventTime(	/* @81, set gpi event time */
	int hdl,						/* in:	handle from pclOpen() */
	int gpi,						/* in:	number of gpi output (1 or 2) */
	int reg,						/* in:	registers number like PCL_REG_xxx */
	int mode,						/* in:	registers number like PCL_GPIM_xxx */
	PCL_TC time						/* in:	timecode of event, bcd */
);

/*		PCL_GPIM_xxx					   gpi pulse mode */
#define PCL_GPIM_PULSE_NEG		0x014A	/* set comparator to negative pulse */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetGpiEventStatus(	/* @82, get gpi event status */
	int hdl,						/* in:	handle from pclOpen() */
	int gpi,						/* in:	number of gpi output (1 or 2) */
	int _PCL_OUT *status,			/* out:	status like PCL_GPIS_xxx */
	int _PCL_OUT *count				/* out:	pulse counter */
);

/*		PCL_GPIS_xxx					   gpi pulse mode */
#define PCL_GPIS_ACTIVE			0x01	/* gpi event active */

/*-------------------------------------------------------------------------*/

#endif // PCL_OPT_GPI_OUT		/* event driven gpi outputs */

/***************************************************************************\
*																			*
* Program options															*
*																			*
\***************************************************************************/

/*	This options use special features of PCL card.  To use any of these,
	a special PCL firmware EPROM is needed. */

/***************************************************************************/

#ifdef PCL_OPT_MTD	/* read mtd times */

int _PCL_FDEF pclMtdGetStr(	/* @101,	get mtd time as string */
	int hdl,				/* in:	handle from pclOpen() */
	int mtd,				/* in:	mtd register like PCL_MTDREG_xxx */
	char _PCL_OUT *str,		/* out:	asciiz string, max. 10 chars */
	BYTE _PCL_OUT *state	/* out:	state of time like PCL_MTDSTAT_xxx */
);

/* Note:  Set several 'out' parameters to NULL if you don't need them */

#define PCL_MTD_STR			11	/* size of *str parameter */

/*		PCL_MTDREG_xxx				mtd time */
#define PCL_MTDREG_A		0x0A /* mtd time a (display mode a) */
#define PCL_MTDREG_B		0x0B /* mtd time b (display mode b) */
#define PCL_MTDREG_C		0x0C /* mtd time c (display mode c) */
#define PCL_MTDREG_D		0x0D /* mtd time d (display mode d) */
#define PCL_MTDREG_E		0x0E /* mtd time e (display mode e) */
#define PCL_MTDREG_F		0x0F /* mtd time f (display mode f, maz tc) */
#define PCL_MTDREG_H		0x02 /* mtd time h (display mode 2, time) */
#define PCL_MTDREG_2		PCL_MTDREG_H
#define PCL_MTDREG_I		0x03 /* mtd time i (display mode 3, date) */
#define PCL_MTDREG_3		PCL_MTDREG_I
#define PCL_MTDREG_T		0x06 /* mtd time   (display mode 6) */
#define PCL_MTDREG_6		PCL_MTDREG_T

/*		PCL_MTDSTAT_xxx				mtd status */
#define PCL_MTDSTAT_BLINK	0x01 /* mtd time is blinking */
#define PCL_MTDSTAT_OFF		0x02 /* mtd time is off */
#define PCL_MTDSTAT_ERRLTC	0x04 /* no mtd time available */

#endif // PCL_OPT_MTD

/***************************************************************************/

#ifdef PCL_OPT_REALTIME

typedef struct {
	struct {			/* time from pcl5 */
		WORD valid;		/* validation flags like PCL_RTVAL_xxx */
		WORD micro;		/* microseconds */
		WORD milli;		/* milliseconds */
		WORD sec;		/* seconds */
		WORD min;		/* minutes */
		WORD hour	;	/* hours (0-23) */
		WORD day;		/* day of month (1-31) */
		WORD month;		/* month (1-12) */
		WORD year;		/* year (1990-2089) */
		WORD tzone;		/* time zone like PCL_RTZONE_xxx */
		WORD frame;		/* time zone like PCL_FRAME_xxx */
		WORD _1;		/* reserved */
	} ltc;
	struct {			/* time from system clock (gmt) */
		WORD milli;		/* milliseconds */
		WORD sec;		/* seconds */
		WORD min;		/* minutes */
		WORD hour;		/* hours (0-23) */
		WORD day;		/* day of month (1-31) */
		WORD month;		/* month (0-11) */
		WORD year;		/* year (minus 1900) */
		WORD _0, _1, _2;/* reserved */
	} system;
} PCL_REALTIME;

/*		PCL_RTVAL_xxx				realtime validation */
#define PCL_RTVAL_INVALID	0x00 /* nothing valid */
#define PCL_RTVAL_TIME		0x01 /* time is valid (sync bit in user) */
#define PCL_RTVAL_DATE		0x02 /* date is valid (plausible and no wrap) */
#define PCL_RTVAL_VALID		0x03 /* everything is valid */

/*		PCL_RTZONE_xxx				time zone */
#define PCL_RTZONE_UTC		0x00 /* utc (from tz bits in user) */
#define PCL_RTZONE_MEZ		0x01 /* mez (from tz bits in user) */
#define PCL_RTZONE_MESZ		0x02 /* mesz (from tz bits in user) */
#define PCL_RTZONE_UNKNOWN	0x03 /* timezone unknown */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetRealtime(	/* @102,	$15, get realtime */
	int hdl,					/* in:	handle from pclOpen() */
	PCL_REALTIME _PCL_OUT *tm	/* out:	realtime from pcl */
);

/*
	return values:

	PCL_NO_NEWD:		PCL didn't read a ltc within the last 40ms.
	PCL_ERR_TIMEOUT:	PCL didn't respond to command.
*/

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSetRealtimeMode(	/* @103, $1F, set realtime mode */
	int hdl,						/* in: handle from pclOpen() */
	int mode						/* in: mode for realtime reader */
);

#endif // PCL_OPT_REALTIME

/***************************************************************************\
*																			*
*	Obsolete Functions														*
*																			*
\***************************************************************************/

/*	The following declaration are only for backward campatibility to the
	ISA version 2.xx of the DLL.  With this PCI version 3.xx, any of these
	function will return PCL_ERR_NOT_SUPP. */

/*-------------------------------------------------------------------------*/

/*	Typedefs
	-------- */

typedef DWORD PCL_OPRT;		/* output port bits (pcl3) */

/*	Output port bits are coded in a double word. O1 corresponds to bit 0,
	O24 to bit 23. Bit 24 to 31 are ignored and should be set to zero.
	e.g. 0x00810001 has O1, O17 and O23 set and all other outputs reset. */

typedef WORD  PCL_IPRT;		/* input port bits (pcl3) */

/*	Input port bits are coded in a single word. I1 corresponds to bit 0,
	I16 to bit 15. e.g. 0x8101 has I1, I9 and I16 set and all other inputs
	reset. */

typedef DWORD PCL_SHOU;		/* shift out data (pcl3) */

/*	Shift out data is coded in a double word. It corresponds to data $0 to $3
	of register $E of PCL card */

typedef DWORD PCL_SHIN;		/* shift in data (pcl3) */

/*	Shift in data is coded in a double word. It corresponds to data $4 to $7
	of register $E of PCL card */

typedef BYTE  PCL_TDAT[7];  /* transmit data */

/*	Transmit data corresponds to command $6C of PCL card */

typedef BYTE  PCL_TSTR[8];  /* transmit string */

/*	Transmit string corresponds to commands $60 up to $67 of PCL card */

typedef BYTE  PCL_CDAT[8];  /* command data */

/*	Command data corresponds to data $0 to $7 of register $F PCL card */

typedef BYTE  PCL_SIND[12]; /* serial input data */

/*	Serial input data corresponds to data $0 to $B of register $D of PCL */

typedef BYTE  PCL_STAT[10]; /* recorder status data */

/*	Status data corresponds to data $0 to $9 of register $B of PCL card */

typedef BYTE  PCL_TRAN[15]; /* transfer register */

/*	Transfer data corresponds to data $0 to $E of register $A of PCL card */

/*-------------------------------------------------------------------------*/

/*	Function Prototypes
	------------------- */

/*-------------------------------------------------------------------------*/
// pclOpen(), @2

/*		PCL_BASE_xxx		pcl base address			A5	A6	A7	A8	*/
/*												SW1		.1	.2	.3	.4	*/
#define PCL_BASE_0			0x0200 /* 512				on	on	on	on	*/
#define PCL_BASE_1			0x0220 /* 544				off	on	on	on	*/
#define PCL_BASE_2			0x0240 /* 576				on	off	on	on	*/
#define PCL_BASE_FACTORY	0x0240 /* factory setting	on	off	on	on	*/
#define PCL_BASE_3			0x0260 /* 608				off	off	on	on	*/
#define PCL_BASE_4			0x0280 /* 640				on	on	off	on	*/
#define PCL_BASE_5			0x02A0 /* 672				off	on	off	on	*/
#define PCL_BASE_6			0x02C0 /* 704				on	off	off	on	*/
#define PCL_BASE_7			0x02E0 /* 736				off	off	off	on	*/
#define PCL_BASE_8			0x0300 /* 768				on	on	on	off	*/
#define PCL_BASE_9			0x0320 /* 800				off	on	on	off	*/
#define PCL_BASE_A			0x0340 /* 832				on	off	on	off	*/
#define PCL_BASE_B			0x0360 /* 864				off	off	on	off	*/
#define PCL_BASE_C			0x0380 /* 896				on	on	off	off	*/
#define PCL_BASE_D			0x03A0 /* 928				off	on	off	off	*/
#define PCL_BASE_E			0x03C0 /* 960				on	off	off	off	*/
#define PCL_BASE_F			0x03E0 /* 992				off	off	off	off	*/

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclRawCommand(	/* @4,	send any command to pcl */
	int hdl,					/* in:	handle from pclOpen() */
	int cmd,					/* in:	command byte */
	const BYTE _PCL_IN *cdat	/* in:	command data, like PCL_CDAT */
);

/*	Note: This function is not available with PCL PCI. */

/*-------------------------------------------------------------------------*/
// pclReset(), @5

#define PCL_IMODE_SONY9S	0x01 /* sony 9-pin control, tc = serial
										(pcl3/6) */
#define PCL_IMODE_JVC9S		0x02 /* jvc pr900 9-pin control, tc = serial
										(pcl3/6) */
#define PCL_IMODE_JVC45		0x03 /* jvc parallel control (pcl3) */
#define PCL_IMODE_PANA34	0x04 /* panasonic ag-7500 parallel control
										(pcl3) */
#define PCL_IMODE_PANA25	0x05 /* panasonic 7330 serial + vitc (pcl3/6) */
#define PCL_IMODE_SONY9		0x06 /* sony 9-pin control, tc = ltc/vitc
										(pcl3/6) */
#define PCL_IMODE_JVC9		0x07 /* jvc pr900 9-pin control, tc=ltc/vitc
										(pcl3/6) */
#define PCL_IMODE_SONY33	0x08 /* sony-5, 33-pin control (pcl3) */
#define PCL_IMODE_JVC25		0x09 /* jvc br-s605 etc. tc = ltc/vitc
										(pcl3/6) */
#define PCL_IMODE_PANAIA	0x0A /* panasonic ia232tc serial (pcl6) */
#define PCL_IMODE_PANATC	0x0B /* panasonic ia232tc tc = ltc/vitc
										(pcl6) */
#define PCL_IMODE_SOSIM		0x0C /* simualtion of a sony protocol (pcl6) */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclDataReq(	/* @6,	$02, data request:
										transfer data to reg. $A */
	int hdl,				/* in:	handle from pclOpen() */
	int reg					/* in:	number of register, like PCL_REG_xxx */
);

/*	Notes:
	pclDataReq is not supported by PCL firmware version less or
		equal V1.3. */

/*-------------------------------------------------------------------------*/
// pclIntMask(), @7

#define PCL_IMASK_CTL		0x02 /* ctl disable/enable */
#define PCL_IMASK_ETIME		0x10 /* event time irq disable/enable */
#define PCL_IMASK_EUSER		0x20 /* event user irq disable/enable */

/*-------------------------------------------------------------------------*/
// pclReaderEnable(), @10

#define PCL_RMASK_CTL		0x01 /* ctl off/on */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclCtlRate(	/* @14,	$__, ctl rate for timecode drop-outs */
	int hdl,				/* in:	handle from pclOpen() */
	int ctlrate				/* in:	ctl rate */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclCtlCount(	/* @15,	$__, ctl rate for independent counter */
	int hdl,				/* in:	handle from pclOpen() */
	int frps				/* in:	frames per second, bcd */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclCtlMode(	/* @16,	$40, ctl mode */
	int hdl,				/* in:	handle from pclOpen() */
	int cmode				/* in:	ctl mode, like PCL_CMODE_xxx */
);

/*		PCL_CMODE_xxx				ctl mode */
#define PCL_CMODE_UNSIGNED	0x00 /* ctl without sign */
#define PCL_CMODE_SIGNED	0x01 /* ctl with sign */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclCtlReset(	/* @17,	$41, ctl reset */
	int hdl					/* in:	handle from pclOpen() */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclCtlPreset(	/* @18,	$42, ctl preset */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_TC time				/* in:	ctl preset value, bcd */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclCtlDir(	/* @19,	$44, ctl direction */
	int hdl,				/* in:	handle from pclOpen() */
	int cdir				/* in:	ctl direction, like PCL_CDIR_xxx */
);

/*		PCL_CDIR_xxx				ctl direction */
#define PCL_CDIR_REV		0x00 /* h = reverse, l = forward */
#define PCL_CDIR_FWD		0x01 /* l = reverse, h = forward */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclEventTime(	/* @20,	$50, set event time */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_TC time				/* in:	event time, bcd */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclEventUser(	/* @21,	$51, set event user */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_USER user			/* in:	event time, bcd */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclTranBank(		/* @22,	$60-$67, set transmit bank 1-8 */
	int hdl,					/* in:	handle from pclOpen() */
	int bank,					/* in:	transmit bank, 1-8 */
	const BYTE _PCL_IN *tstr	/* in:	transmit data, like PCL_TSTR */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSioCmd(	/* @23,	$68, set sio command reg */
	int hdl,				/* in:	handle from pclOpen() */
	int scmd				/* in:	sio command reg, like PCL_SCMD_xxx */
);

/*		PCL_SCMD_xxx				sio command reg */
/*									bit 0: receiver disable/enable */
#define PCL_SCMD_RDIS		0x00 /* receiver disable */
#define PCL_SCMD_RENA		0x01 /* receiver enable */
/*									bit 7-5: parity mode */
#define PCL_SCMD_NONE		0x00 /* no parity trans/rec */
#define PCL_SCMD_ODD		0x20 /* odd parity trans/rec */
#define PCL_SCMD_EVEN		0x60 /* even parity trans/rec */
#define PCL_SCMD_MARK		0xA0 /* mark parity trans/rec */
#define PCL_SCMD_SPACE		0xE0 /* space parity trans/rec */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSioCtl(	/* @24,	$69, set sio control reg */
	int hdl,				/* in:	handle from pclOpen() */
	int sctl				/* in:	sio control reg, like PCL_SCTL_xxx */
);

/*		PCL_SCTL_xxx				sio control reg */
/*									bit 3-0: baudrate */
#define PCL_SCTL_38400		0x00 /* 38400 bps */
#define PCL_SCTL_50			0x01 /* 50 bps */
#define PCL_SCTL_75			0x02 /* 75 bps */
#define PCL_SCTL_110		0x03 /* 109.92 bps */
#define PCL_SCTL_135		0x04 /* 134.58 bps */
#define PCL_SCTL_150		0x05 /* 150 bps */
#define PCL_SCTL_300		0x06 /* 300 bps */
#define PCL_SCTL_600		0x07 /* 600 bps */
#define PCL_SCTL_1200		0x08 /* 1200 bps */
#define PCL_SCTL_1800		0x09 /* 1800 bps */
#define PCL_SCTL_2400		0x0A /* 2400 bps */
#define PCL_SCTL_3600		0x0B /* 3600 bps */
#define PCL_SCTL_4800		0x0C /* 4800 bps */
#define PCL_SCTL_7200		0x0D /* 7200 bps */
#define PCL_SCTL_9600		0x0E /* 9600 bps */
#define PCL_SCTL_19200		0x0F /* 19200 bps */
/*									bit 4: rs232/422 select */
#define PCL_SCTL_RS232		0x00 /* rs-232 */
#define PCL_SCTL_RS422		0x10 /* rs-422 */
/*									bit 6-5: word length */
#define PCL_SCTL_B8			0x00 /* 8 bits/word */
#define PCL_SCTL_B7			0x20 /* 7 bits/word */
#define PCL_SCTL_B6			0x40 /* 6 bits/word */
#define PCL_SCTL_B5			0x60 /* 5 bits/word */
/*									bit 7: stop bits */
#define PCL_SCTL_S1			0x00 /* 1 stop bit */
#define PCL_SCTL_S2			0x80 /* 2 stop bits */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclRecMode(	/* @25,	$6a, set receive mode */
	int hdl,				/* in:	handle from pclOpen() */
	int rmode				/* in:	receive mode, like PCL_RMODE_xxx */
);

/*		PCL_RMODE_xxx				receive mode */
#define PCL_RMODE_NORMAL	0x00 /* normal mode, byte by byte */
#define PCL_RMODE_SONY		0x01 /* sony protocol, controlling device */
#define PCL_RMODE_PANA		0x00 /* panasonic prot., controlling device */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclTranStr(	/* @26,	$6b, transmit string */
	int hdl,				/* in:	handle from pclOpen() */
	int num					/* in:	number of bytes to transmit */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclTranData(		/* @27,	$6c, transmit data */
	int hdl,					/* in:	handle from pclOpen() */
	int num,					/* in:	number of bytes to transmit */
	const BYTE _PCL_IN *tdat	/* in:	transmit data, like PCL_TDAT */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclModeOut(	/* @28,	$70, mode output (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	int omode,				/* in:	output mode, like PCL_OMODE_xxx */
	WORD freq				/* in:	port pb7 frequency */
);

/*		PCL_OMODE_xxx				mode output */
#define PCL_OMODE_NORMAL	0x00 /* normal */
#define PCL_OMODE_FREQ		0x01 /* port pb7 frequency */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclOutPort(	/* @29,	$71, output ports (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_OPRT oprt			/* in:	ports 1-24, like PCL_OPRT_xxx */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclPortHigh(	/* @30,	$72, set port to h (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	int oprt				/* in:	number -1 of port ($00-$17) */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclPortLow(	/* @31,	$73, set port to l (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	int oprt				/* in:	number -1 of port ($00-$17) */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclPosPulse(	/* @32,	$74, pos. pulses (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_OPRT oprt,			/* in:	ports 1-24, like PCL_OPRT_xxx */
	int pmode,				/* in:	pulse width mode, like PCL_PMODE_xxx */
	int pwidth				/* in:	pulse width */
);

/*		PCL_PMODE_xxx				pulse width mode */
#define PCL_PMODE_INDIV		0x00 /* individual pulse width */
#define PCL_PMODE_SAME		0x01 /* same pulse width for all ports conc. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclNegPulse(	/* @33,	$75, neg. pulses (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_OPRT oprt,			/* in:	ports 1-24, like PCL_OPRT_xxx */
	int pmode,				/* in:	pulse width mode, like PCL_PMODE_xxx */
	int pwidth				/* in:	pulse width */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclPosPulseOut(	/* @34,	$76, pos. pulse out (pcl3) */
	int hdl,					/* in:	handle from pclOpen() */
	int oprt,					/* in:	number -1 of port ($00-$17) */
	int pwidth					/* in:	pulse width */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclNegPulseOut(	/* @35,	$77, neg. pulse out (pcl3) */
	int hdl,					/* in:	handle from pclOpen() */
	int oprt,					/* in:	number -1 of port ($00-$17) */
	int pwidth					/* in:	pulse width */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclPulseWidth(	/* @36,	$78, pulse width: preset pulse width
											(pcl3) */
	int hdl,					/* in:	handle from pclOpen() */
	PCL_OPRT oprt,				/* in:	ports 1-24, like PCL_OPRT_xxx */
	int pwidth					/* in:	pulse width */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAnalogOut(	/* @37,	$79, analog out (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	int value				/* in:	(-10 + value * 20/256) volt */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclShiftData(	/* @38,	$80, shift out data (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_SHOU shou			/* in:	shift out data */
);

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclShiftWidth(	/* @39,	$81, shift out width (pcl3) */
	int hdl,					/* in:	handle from pclOpen() */
	int swid					/* in:	shift out width, like PCL_SWID_xxx */
);

/*		PCL_SWID_xxx				shift out width */
#define PCL_SWID_8			0x00 /* 8 bit */
#define PCL_SWID_16			0x01 /* 16 bit */
#define PCL_SWID_24			0x02 /* 24 bit */
#define PCL_SWID_32			0x03 /* 32 bit */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclShiftMode(	/* @40,	$82, shift out mode (pcl3) */
	int hdl,				/* in:	handle from pclOpen() */
	int smode				/* in:	shift out mode, like PCL_SMODE_xxx */
);

/*		PCL_SMODE_xxx				shift out mode */
#define PCL_SMODE_DIS		0x80 /* disable */
#define PCL_SMODE_ONE		0x00 /* one shot */
#define PCL_SMODE_CONT		0x01 /* continous */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclRecCtl(	/* @41,	$90, recorder control */
	int hdl,				/* in:	handle from pclOpen() */
	int rctl				/* in:	command, like PCL_RCTL_xxx */
);

/*		PCL_RCTL_xxx				recorder control command */
#define PCL_RCTL_STOP		0x01 /* stop */
#define PCL_RCTL_PLAY		0x02 /* play */
#define PCL_RCTL_REC		0x03 /* rec */
#define PCL_RCTL_STILL		0x04 /* still/pause */
#define PCL_RCTL_FFWD		0x05 /* fast forward */
#define PCL_RCTL_ADV		0x06 /* frame advance */
#define PCL_RCTL_FREV		0x07 /* fast reverse */
#define PCL_RCTL_BACK		0x08 /* frame reverse (back) */
#define PCL_RCTL_TRIMF		0x10 /* fine trim forward (1.4+) */
#define PCL_RCTL_TRIMR		0x11 /* fine trim reverse (1.4+) */
#define PCL_RCTL_EJECT		0x21 /* eject */
#define PCL_RCTL_EDON		0x24 /* edit on */
#define PCL_RCTL_EDOFF		0x25 /* edit off */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclRecCtlData(	/* @42,	$90, recorder control, with data */
	int hdl,					/* in:	handle from pclOpen() */
	int rctl,					/* in:	command, like PCL_RCTD_xxx */
	int data					/* in:	data, depends on command */
);

/*		PCL_RCTD_xxx				recorder control command, variable */
#define PCL_RCTD_VFWD		0x12 /* var forward */
#define PCL_RCTD_VREV		0x13 /* var reverse */
#define PCL_RCTD_JFWD		0x14 /* jog forward */
#define PCL_RCTD_JREV		0x15 /* jog reverse */
#define PCL_RCTD_SFWD		0x1F /* shtl forward */
#define PCL_RCTD_SREV		0x20 /* shtl reverse */
#define PCL_RCTD_EDPR		0x26 /* edit preset */

/*		PCL_EDPR_xxx				edit preset data */
#define PCL_EDPR_AUDIO1		0x01 /* audio 1 */
#define PCL_EDPR_AUDIO2		0x02 /* audio 2 */
#define PCL_EDPR_AUDIO3		0x04 /* audio 3 */
#define PCL_EDPR_TIMECODE	0x04 /* timecode */
#define PCL_EDPR_AUDIO4		0x08 /* audio 4 */
#define PCL_EDPR_VIDEO		0x10 /* video */
#define PCL_EDPR_ASSEMBLE	0x20 /* assemble */
#define PCL_EDPR_INSERT		0x40 /* insert */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclRecCtlTc(	/* @43,	$90, recorder control, with timecode */
	int hdl,				/* in:	handle from pclOpen() */
	int rctl,				/* in:	command, like PCL_RCTC_xxx */
	PCL_TC time				/* in:	timecode, depends on command */
);

/*		PCL_RCTC_xxx				recorder control command, with tc */
#define PCL_RCTC_CUEUP		0x16 /* cue up with data (goto) */
#define PCL_RCTC_GOTO		0x16 /* goto (cue up with data) */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclRecStat(	/* @44,	$91, recorder status request */
	int hdl,				/* in:	handle from pclOpen() */
	BYTE _PCL_OUT *stat,	/* out:	recorder status, like PCL_STAT */
	int _PCL_OUT *ident		/* out:	recorder identification flag */
);

/* Note:  Set several 'out' parameters to NULL if you don't need them */

/*-------------------------------------------------------------------------*/
// pclGetRegister(), @47

#define PCL_REG_CTL			0x01 /* ctl register */
#define PCL_REG_MIXED2		0x04 /* 2nd mixed register (pcl2 only) */
#define PCL_REG_CTL2		0x05 /* 2nd ctl register (pcl2 only) */
#define PCL_REG_LTC2		0x06 /* 2nd ltc register (pcl2 only) */
#define PCL_REG_VITC2		0x07 /* 2nd vitc register (pcl2 only) */
#define PCL_REG_EVENT		0x08 /* timecode comparator (event) */
#define PCL_REG_EVENT2		0x09 /* 2nd timecode comparator (event,
									pcl2 only) */
#define PCL_REG_TRAN		0x0A /* transfer register */
#define PCL_REG_STATUS		0x0B /* status */
#define PCL_REG_IO			0x0C /* i/o */
#define PCL_REG_SERIN		0x0D /* serial input data */
#define PCL_REG_SHIFT		0x0E /* shift register */
#define PCL_REG_CMD			0x0F /* commands */
#define PCL_REG_BANK		0x10 /* bank select */

#define PCL_NEWD_CTL		0x20 /* ctl */

/*		PCL_BITS_CTL_xxx			ctl bits */
#define PCL_BITS_CTL_SIGN	0x40 /* sign */
#define PCL_BITS_CTL_DIR	0x80 /* direction */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetEvent(	/* @51,	read event register */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_TC _PCL_OUT *dist1,	/* out:	event distance 1 bcd */
	PCL_TC _PCL_OUT *dist2,	/* out:	event distance 2 bcd */
	PCL_TC _PCL_OUT *dist3,	/* out:	event distance 3 bcd */
	int _PCL_OUT *evtc,		/* out:	status event time, like PCL_EVTC_xxx */
	int _PCL_OUT *evub,		/* out:	status event user, like PCL_EVUB_xxx */
	int _PCL_OUT *eflg		/* out:	event found flag, like PCL_EFLG_xxx */
);

/*	Notes:
	Set several 'out' parameters to NULL if you don't need them. */

/*		PCL_EVTC_xxx				status event time */
#define PCL_EVTC_FRU		0x01 /* units frames of distance 3 <> 0 */
#define PCL_EVTC_FRT		0x02 /* tens frames of distance 3 <> 0 */
#define PCL_EVTC_SEU		0x04 /* units seconds of distance 3 <> 0 */
#define PCL_EVTC_SET		0x08 /* tens seconds of distance 3 <> 0 */
#define PCL_EVTC_MIN		0x10 /* minutes of distance 3 <> 0 */
#define PCL_EVTC_HOU		0x20 /* hours of distance 3 <> 0 */
#define PCL_EVTC_SIG12		0x40 /* sign mod.12 hours */
#define PCL_EVTC_SIG		0x80 /* sign absolute */

/*		PCL_EVUB_xxx				status event user */
#define PCL_EVUB_UB1		0x01 /* 1st event user <> mixed reg. user */
#define PCL_EVUB_UB2		0x02 /* 2nd event user <> mixed reg. user */
#define PCL_EVUB_UB3		0x04 /* 3rd event user <> mixed reg. user */
#define PCL_EVUB_UB4		0x08 /* 4th event user <> mixed reg. user */
#define PCL_EVUB_UB5		0x10 /* 5th event user <> mixed reg. user */
#define PCL_EVUB_UB6		0x20 /* 6th event user <> mixed reg. user */
#define PCL_EVUB_UB7		0x40 /* 7th event user <> mixed reg. user */
#define PCL_EVUB_UB8		0x80 /* 8th event user <> mixed reg. user */

/*		PCL_EFLG_xxx				event found flag */
#define PCL_EFLG_UMASK		0x0F /* event user found */
#define PCL_EFLG_TMASK		0xF0 /* event time found */

/*	Please refer to description of register $8 in chapter C1 for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetTransfer(	/* @52,	read transfer register $A */
	int hdl,					/* in:	handle from pclOpen() */
	BYTE _PCL_OUT *tran			/* out:	recorder status, like PCL_TRAN */
);

/*	Notes:
	pclGetTransfer is not supported by PCL firmware version less or
		equal V1.3
	Please refer to description of register $A in chapter C1 for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetIo(			/* @53,	read i/o ports (pcl3) */
	int hdl,					/* in:	handle from pclOpen() */
	PCL_OPRT _PCL_OUT *oprt,	/* out:	output ports 1-24,
										like PCL_OPRT_xxx */
	int _PCL_OUT *ana,			/* out:	analog output */
	PCL_IPRT _PCL_OUT *iprt		/* out:	input ports 1-16,
										like PCL_IPRT_xxx */
);

/*	Notes:
	Set several 'out' parameters to NULL if you don't need them */

/*		PCL_OPRT_xxx					 output port bits */
#define PCL_OPRT_1			0x000001l /* output port 1 */
#define PCL_OPRT_2			0x000002l /* output port 2 */
#define PCL_OPRT_3			0x000004l /* output port 3 */
#define PCL_OPRT_4			0x000008l /* output port 4 */
#define PCL_OPRT_5			0x000010l /* output port 5 */
#define PCL_OPRT_6			0x000020l /* output port 6 */
#define PCL_OPRT_7			0x000040l /* output port 7 */
#define PCL_OPRT_8			0x000080l /* output port 8 */
#define PCL_OPRT_9			0x000100l /* output port 9 */
#define PCL_OPRT_10			0x000200l /* output port 10 */
#define PCL_OPRT_11			0x000400l /* output port 11 */
#define PCL_OPRT_12			0x000800l /* output port 12 */
#define PCL_OPRT_13			0x001000l /* output port 13 */
#define PCL_OPRT_14			0x002000l /* output port 14 */
#define PCL_OPRT_15			0x004000l /* output port 15 */
#define PCL_OPRT_16			0x008000l /* output port 16 */
#define PCL_OPRT_17			0x010000l /* output port 17 */
#define PCL_OPRT_18			0x020000l /* output port 18 */
#define PCL_OPRT_19			0x040000l /* output port 19 */
#define PCL_OPRT_20			0x080000l /* output port 20 */
#define PCL_OPRT_21			0x100000l /* output port 21 */
#define PCL_OPRT_22			0x200000l /* output port 22 */
#define PCL_OPRT_23			0x400000l /* output port 23 */
#define PCL_OPRT_24			0x800000l /* output port 24 */

/*		PCL_IPRT_xxx				  input port bits */
#define PCL_IPRT_1			0x0001 /* input port 1 */
#define PCL_IPRT_2			0x0002 /* input port 2 */
#define PCL_IPRT_3			0x0004 /* input port 3 */
#define PCL_IPRT_4			0x0008 /* input port 4 */
#define PCL_IPRT_5			0x0010 /* input port 5 */
#define PCL_IPRT_6			0x0020 /* input port 6 */
#define PCL_IPRT_7			0x0040 /* input port 7 */
#define PCL_IPRT_8			0x0080 /* input port 8 */
#define PCL_IPRT_9			0x0100 /* input port 9 */
#define PCL_IPRT_10			0x0200 /* input port 10 */
#define PCL_IPRT_11			0x0400 /* input port 11 */
#define PCL_IPRT_12			0x0800 /* input port 12 */
#define PCL_IPRT_13			0x1000 /* input port 13 */
#define PCL_IPRT_14			0x2000 /* input port 14 */
#define PCL_IPRT_15			0x4000 /* input port 15 */

/*	Please refer to description of register $C for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetSio(	/* @54,	read sio registers */
	int hdl,				/* in:	handle from pclOpen() */
	int _PCL_OUT *ssta,		/* out:	sio status reg, like PCL_SSTA_xxx */
	int _PCL_OUT *scmd,		/* out:	sio command reg, like PCL_SCMD_xxx */
	int _PCL_OUT *sctl		/* out:	sio control reg, like PCL_SCTL_xxx */
);

/*	Notes:
	Set several 'out' parameters to NULL if you don't need them. */

/*		PCL_SSTA_xxx				sio control reg */
#define PCL_SSTA_PARITY		0x01 /* parity error */
#define PCL_SSTA_FRAMING	0x02 /* framing error */
#define PCL_SSTA_OVERRUN	0x04 /* overrun error */
#define PCL_SSTA_RFULL		0x08 /* receiver full */
#define PCL_SSTA_TEMPTY		0x10 /* transmitter empty */
#define PCL_SSTA_RS232		0x00 /* rs232 */
#define PCL_SSTA_RS422		0x20 /* rs422 */
#define PCL_SSTA_DSR		0x40 /* state of dsr */
#define PCL_SSTA_IRQ		0x80 /* irq */

/*	Please refer to description of register $C in chapter C1 for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclResetTrans(	/* @55,	reset sio transmit flag */
	int hdl						/* in:	handle from pclOpen() */
);

/*	Notes:
	Please refer to description of register $C, byte $9 in chapter C1
		for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetSerIn(	/* @56,	read serial input data */
	int hdl,				/* in:	handle from pclOpen() */
	BYTE _PCL_OUT *sind,	/* out:	serial input data, like PCL_SIND */
	int _PCL_OUT *srcv,		/* out:	status receive, like PCL_SRCV_xxx */
	int _PCL_OUT *count		/* out:	number of buffered bytes in *sind */
);

/*	Notes:
	Set several 'out' parameters to NULL if you don't need them. */

/*		PCL_SRCV_xxx				sio control reg */
#define PCL_SRCV_PARITY		0x01 /* parity error */
#define PCL_SRCV_FRAMING	0x02 /* framing error */
#define PCL_SRCV_OVERRUN	0x04 /* overrun error */
#define PCL_SRCV_IBUFOV		0x08 /* irq buffer overflow */
#define PCL_SRCV_MBUFOV		0x10 /* main buffer overflow */

/*	Please refer to description of register $C, byte $0-$9 in chapter C1
	for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetShift(		/* @57,	read shift in/out data (pcl3) */
	int hdl,					/* in:	handle from pclOpen() */
	PCL_SHOU _PCL_OUT *shou,	/* out:	shift out data 0-31 */
	PCL_SHIN _PCL_OUT *shin,	/* out:	shift out data 0-31 */
	int _PCL_OUT *shst			/* out:	status shift, like PCL_SHST_xxx */
);

/*	Notes:
	Set several 'out' parameters to NULL if you don't need them. */

/*		PCL_SHST_xxx				status shift */
#define PCL_SHST_INEWD		0x01 /* shift in new data flag */
#define PCL_SHST_OENA		0x80 /* reflects shift out enable bit */

/* Please refer to description of register $E for details. */

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclGetTrans(	/* @58,	read sio transmit flag */
	int hdl,				/* in:	handle from pclOpen() */
	int _PCL_OUT *strf		/* out:	sio transmit flag */
);

/*	Notes:
	Please refer to description of register $C, byte $9 in chapter C1
		for details. */

/*-------------------------------------------------------------------------*/

DWORD _PCL_FDEF pclIoctlError(	/* @61,	get last ioctl error */
	void						/*		no parameters needed */
);								/*	result: last ioctl error if any pclXxx
										function returned PCL_ERR_IOCTL */

/*	Notes:
	This function is only useful with the windows nt device driver
		avpclnt.sys.  With avpcl.dll or avpcl32.dll (win95 version) this
		function always returns 0 (no error). */

/*-------------------------------------------------------------------------*/

unsigned _PCL_FDEF pclGetBaseAddress(	/* @62,	get pcl base address */
	int index							/* in:	index to IoPortAddress in
												registry */
);

/*	Notes:
	pclGetBaseAddress() reads the base address of the pcl card from the
		Windows NT device driver.  It is only useful under Windows NT.  Under
		all other platforms, pclGetBaseAddress() returns 0.

		index = 0 returns the DWORD registry entry 'HKEY_LOCAL_MACHINE\
				SYSTEM\CurrentControlSet\Services\AvPclNT\Parameters\
				IoPortAddress',
		index = 1 returns the DWORD registry entry 'HKEY_LOCAL_MACHINE\
				SYSTEM\CurrentControlSet\Services\AvPclNT\Parameters\
				IoPortAddress1',
		index = 2 returns the DWORD registry entry 'HKEY_LOCAL_MACHINE\
				SYSTEM\CurrentControlSet\Services\AvPclNT\Parameters\
				IoPortAddress2'
		and so on.
		If the registry entry doesn't exist, pclGetBaseAddress() returns 0.
*/

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclReaderSource(	/* @64,	$12, reader source (pcl5/6) */
	int hdl,					/* in:	handle from pclOpen() */
	int rsrc					/* in:	reader source, like PCL_RSRC_xxx */
);

/*		PCL_RSRC_xxx				reader source */
#define PCL_RSRC_1			0x01 /* source 1 */
#define PCL_RSRC_2			0x02 /* source 2 (only special design;
												2 x ltc input */
/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclSetMixed(	/* @67,	$22, set mixed register */
	int hdl,				/* in:	handle from pclOpen() */
	PCL_TC time,			/* in:	timecode */
	PCL_USER user,			/* in:	userbits */
	int status,				/* in:	status like PCL_BITS_xxx_xxx */
	int newd				/* in:	new data flag like PCL_NEWD_xxx */
);

/*-------------------------------------------------------------------------*/

#ifdef PCL_OPT_GETCMD	/* get command bytes from device */

int _PCL_FDEF pclOptGetcmd(	/* @100,	get command bytes from device */
	int hdl,				/* in:	handle from pclOpen() */
	int _PCL_OUT *rctl,		/* out:	command, like PCL_RCTx_xxx */
	BYTE _PCL_OUT *full		/* out:	sony command bytes, like PCL_FULL */
);

/*		PCL_RCTF_xxx				recorder control status */
#define PCL_RCTF_STATS		0xEF /* status sense */
#define PCL_RCTF_NONE		0xFE /* no new cmd */
#define PCL_RCTF_UNKNOWN	0xFF /* unknown cmd */

#endif // PCL_OPT_GETCMD

/***************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /****************************************************** _AV_AVPCL_H */
