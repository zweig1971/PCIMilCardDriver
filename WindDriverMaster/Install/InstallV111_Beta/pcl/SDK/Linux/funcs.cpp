//***************************************************************************
//*																			*
//*	Windows DLL AVPCL.DLL													*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*																			*
//***************************************************************************

// <Funcs.cpp> Alpermann+Velte DLL for AV-PCL

//---------------------------------------------------------------------------
#include "params.h"
//---------------------------------------------------------------------------

#include <stdio.h>
#ifndef LINUX
#include <assert.h>
#endif

//---------------------------------------------------------------------------
#include "avpcl.h"
//---------------------------------------------------------------------------
#ifdef _WIN32
#include "pci_lib.h"
#endif
#ifdef LINUX
#include "pci_lib_linux.h"
#endif
#include "global.h"
//---------------------------------------------------------------------------
#undef	_PCL_EXTERN
#define _PCL_EXTERN
#undef	_PCL_CONST
#define _PCL_CONST
#include "funcs.h"
//---------------------------------------------------------------------------

const char *_PCL_FDEF pclPciGetErrorString(		// @68, get error string
	void										// no parameters needed
) {
	PCI_ErrorString[0] = '\0';
	return(PCI_ErrorString);
}

//---------------------------------------------------------------------------

const char * _PCL_FDEF pclGetErrorString(	// @68, get error string
	void									// no parameter needed
) {
	return(pclErrorString);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclReset(	// $01,	reset: initialize pcl card
	int hdl,			// in:	handle from pclOpen()
	int imode			// in:	operation mode of pcl like PCL_IMODE_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	if (imode == PCL_IMODE_POR) {
/*
		if (!gPciHdl[hdl].porSupported) {
//			return(PCL_ERR_NOT_SUPP);

			DWORD cntrl;
			cntrl = PCI_ReadCNTRL(hPCI);
			DWORD intcsr = PCI_ReadINTCSR(hPCI);

			PCI_WriteINTCSR(hPCI, intcsr & ~1);	// disable linti1

			PCI_WriteCNTRL(hPCI, cntrl);
			Sleep(1);
			PCI_WriteCNTRL(hPCI, cntrl |  0x40000000);	// software reset
			Sleep(20);
			PCI_WriteCNTRL(hPCI, cntrl & ~0x40000000);
			Sleep(300);

			PCI_WriteINTCSR(hPCI, INTCSR_ACK);	// acknowledge
//			PCI_WriteINTCSR(hPCI, intcsr | 1);	// enable linti1

		} else {
*/
			PCI_WriteINTCSR(hPCI, INTCSR_ACK & ~1);	// disable linti1

			PCI_WriteCFG(hPCI, 1);
			Sleep(20);
			PCI_WriteCFG(hPCI, 0);
			Sleep(300);

			PCI_WriteINTCSR(hPCI, INTCSR_ACK);	// acknowledge linti1
//		}
	} else {
		PCI_WriteCMDD0(hPCI, imode);
		if ((result = pclPciExec(hPCI, 0x01)) != PCL_OK) return(result);
	}
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetVersion(	// read pcl firmware version number
	int hdl,					// in:	handle from pclOpen()
	int _PCL_OUT *ident,		// out:	software identification
	int _PCL_OUT *firmware,		// out:	firmware revision number
	int _PCL_OUT *dll			// out:	dll revision number
) {
	if (dll != NULL) {
		*dll = (AVPCL_VER << 8) + AVPCL_SUB;
#ifndef LINUX	
		assert(PCL_VER_DLL == (AVPCL_VER << 8) + AVPCL_SUB);
#endif	
	}

	if (ident != NULL || firmware != NULL) {
		int result;
		PCI_GET_HANDLE(hdl, hPCI);

		if ((result = pclPciExec(hPCI, 0x46)) != PCL_OK) return(result);
		if (ident    != NULL) *ident    = PCI_ReadDATA0(hPCI);
		if (firmware != NULL) *firmware = PCI_ReadDATA1(hPCI);
	}

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetVerSpecial(	// read special pcl firmware version
	int hdl,					// in:	handle from pclOpen()
	int _PCL_OUT *ident1,		// out:	identifier from byte $a
	int _PCL_OUT *ident2,		// out:	identifier from byte $b
	int _PCL_OUT *ident3		// out:	identifier from byte $c
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	if ((result = pclPciExec(hPCI, 0x47)) != PCL_OK) return(result);

	if (ident1 != NULL) *ident1 = PCI_ReadDATA0(hPCI);
	if (ident2 != NULL) *ident2 = PCI_ReadDATA1(hPCI);
	if (ident3 != NULL) *ident3 = PCI_ReadDATA2(hPCI);

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetTc(		// @45, read mixed-register timecode asynchronous
	int hdl,				// in:	handle from pclOpen()
	PCL_TC _PCL_OUT *time	// out	timecode from mixed-register, bcd
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	CHECK_PTR(time);
	if ((result = pclPciExec(hPCI, 0x41)) != PCL_OK) return(result);
	*((BYTE *) time    ) = (BYTE) PCI_ReadDATA0(hPCI);
	*((BYTE *) time + 1) = (BYTE) PCI_ReadDATA1(hPCI);
	*((BYTE *) time + 2) = (BYTE) PCI_ReadDATA2(hPCI);
	*((BYTE *) time + 3) = (BYTE) PCI_ReadDATA3(hPCI);

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetUser(	// read mixed-register user-bits asynchronous
	int hdl,				// in:	handle from pclOpen()
	PCL_USER _PCL_OUT *user	// out:	user-bits from mixed-register
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	CHECK_PTR(user);
	if ((result = pclPciExec(hPCI, 0x42)) != PCL_OK) return(result);
	*((BYTE *) user    ) = (BYTE) PCI_ReadDATA0(hPCI);
	*((BYTE *) user + 1) = (BYTE) PCI_ReadDATA1(hPCI);
	*((BYTE *) user + 2) = (BYTE) PCI_ReadDATA2(hPCI);
	*((BYTE *) user + 3) = (BYTE) PCI_ReadDATA3(hPCI);

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetRegister(	// read register asynchronous
	int hdl,					// in:	handle from pclOpen()
	int reg,					// in:	registers number like PCL_REG_xxx
	PCL_TC _PCL_OUT *time,		// out:	timecode from register, bcd
	int _PCL_OUT *bits,			// out:	misc. bits from byte $3 of register,
								//		meaning depends of '*newd',
								//		like PCL_BITS_xxx_xxx
	PCL_TC _PCL_OUT *user,		// out:	user-bits from register, bcd
	int _PCL_OUT *frame,		// out:	framerate as detected by pcl,
								//		like PCL_FRAME_xxx
	int _PCL_OUT *newd			// out:	identifier of readed timecode,
								//		like PCL_NEWD_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	switch (reg) {
	case PCL_REG_LTC:
	case PCL_REG_VITC:
	case PCL_REG_MIXED:
	case PCL_REG_ATC_LTC:
	case PCL_REG_ATC_VITC1:
	case PCL_REG_ATC_VITC2:
	case PCL_REG_HANC_LTC:
	case PCL_REG_HANC_VITC:
		break;
	default:
		sprintf(PCI_ErrorString, "pclPciGetRegister(): Register 'reg' 0x%X is not available\n", reg);
		return(PCL_ERR_INV_REG);
		break;
	}

	if (
		reg   == PCL_REG_MIXED &&
		time  != NULL &&
		bits  == NULL &&
		user  == NULL &&
		frame == NULL &&
		newd  == NULL
	) return(pclGetTc(hdl, time));

	if (
		reg   == PCL_REG_MIXED &&
		time  == NULL &&
		bits  == NULL &&
		user  != NULL &&
		frame == NULL &&
		newd  == NULL
	) return(pclGetUser(hdl, user));

	PCI_WriteCMDD0(hPCI, reg);
	if ((result = pclPciExec(hPCI, 0x43)) != PCL_OK) return(result);
	if (time != NULL) {
		*((BYTE *) time    ) = (BYTE) PCI_ReadDATA0(hPCI);
		*((BYTE *) time + 1) = (BYTE) PCI_ReadDATA1(hPCI);
		*((BYTE *) time + 2) = (BYTE) PCI_ReadDATA2(hPCI);
		*((BYTE *) time + 3) = (BYTE) PCI_ReadDATA3(hPCI);
	}
	if (bits != NULL) *bits = PCI_ReadDATA4(hPCI);
	if (user != NULL) {
		*((BYTE *) user    ) = (BYTE) PCI_ReadDATA5(hPCI);
		*((BYTE *) user + 1) = (BYTE) PCI_ReadDATA6(hPCI);
		*((BYTE *) user + 2) = (BYTE) PCI_ReadDATA7(hPCI);
		*((BYTE *) user + 3) = (BYTE) PCI_ReadDATA8(hPCI);
	}
	if (frame != NULL) *frame = PCI_ReadDATA9(hPCI);
	if (newd  != NULL) *newd  = PCI_ReadDATAA(hPCI);
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetRegSync(	// read register synchronous
	int hdl,					// in:	handle from pclOpen()
	int reg,					// in:	registers number like PCL_REG_xxx
	PCL_TC _PCL_OUT *time,		// out:	timecode from register, bcd
	int _PCL_OUT *bits,			// out:	misc. bits from byte $3 of register,
								//		meaning depends of '*newd',
								//		like PCL_BITS_xxx_xxx
	PCL_TC _PCL_OUT *user,		// out:	user-bits from register, bcd
	int _PCL_OUT *frame,		// out:	framerate as detected by pcl,
								//		like PCL_FRAME_xxx
	int _PCL_OUT *newd			// out:	identifier of readed timecode,
								//		like PCL_NEWD_xxx
) {
	PCI_GET_HANDLE(hdl, hPCI);
	int intflg = PCI_ReadINTFLGL(hPCI);
	if (gPciHdl[hdl].irq16) intflg |= PCI_ReadINTFLGH(hPCI) << 8;

	switch (reg) {
	case PCL_REG_LTC:		if ((intflg & PCL_IMASK_LTC)       == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_VITC:		if ((intflg & PCL_IMASK_VITC)      == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_MIXED:		if ((intflg & PCL_IMASK_MIXED)     == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_ATC_LTC:	if ((intflg & PCL_IMASK_ATC_LTC)   == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_ATC_VITC1:	if ((intflg & PCL_IMASK_ATC_VITC1) == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_ATC_VITC2:	if ((intflg & PCL_IMASK_ATC_VITC2) == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_HANC_LTC:	if ((intflg & PCL_IMASK_HANC_LTC)  == 0) return(PCL_NO_NEWD);  break;
	case PCL_REG_HANC_VITC:	if ((intflg & PCL_IMASK_HANC_VITC) == 0) return(PCL_NO_NEWD);  break;
		break;

	default:
		sprintf(PCI_ErrorString, "pclPciGetRegSync(): Register 'reg' 0x%X is not available\n", reg);
		return(PCL_ERR_INV_REG);
		break;
	}
	return(pclGetRegister(hdl, reg, time, bits, user, frame, newd));
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetRegStatus(	/* @84,	read register status */
	int hdl,					/* in:	handle from pclOpen() */
	int reg,					/* in:	registers number like PCL_REG_xxx */
	int _PCL_OUT *stat			/* out:	status bytes, like PCI_STAT_xxx */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	CHECK_PTR(stat);
	PCI_WriteCMDD0(hPCI, reg);
	if ((result = pclPciExec(hPCI, 0x44)) != PCL_OK) return(result);
	*((BYTE *) stat    ) = (BYTE) PCI_ReadDATA0(hPCI);
	*((BYTE *) stat + 1) = (BYTE) PCI_ReadDATA1(hPCI);
	*((BYTE *) stat + 2) = 0;
	*((BYTE *) stat + 3) = 0;

	return(PCL_OK);
}

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclIntMask(	// $03, interrupts: set hw interrupt mask
	int hdl,				// in:	handle from pclOpen()
	int imask				// in:	interrupt mask, like PCL_IMASK_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	if (imask != 0) {
		if ((result = pclIntEnable(hPCI, TRUE)) != PCL_OK) return(result);
	}
	PCI_WriteCMDD0(hPCI, imask);
	PCI_WriteCMDD1(hPCI, imask >> 8);
	if ((result = pclPciExec(hPCI, 0x03)) != PCL_OK) return(result);

	gPciHdl[hdl].imask = PCI_ReadDATA0(hPCI);
	if (gPciHdl[hdl].irq16) gPciHdl[hdl].imask |= PCI_ReadDATA1(hPCI) << 8;
	if ((result = pclIntEnable(hPCI, gPciHdl[hdl].imask != 0)) != PCL_OK) return(result);
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclSetIntMask(	// $04, set interrupt bits
	int hdl,					// in:	handle from pclOpen()
	int imask					// in:	interrupt mask, like PCL_IMASK_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	if (imask != 0) {
		if ((result = pclIntEnable(hPCI, TRUE)) != PCL_OK) return(result);
	}
	PCI_WriteCMDD0(hPCI, imask);
	PCI_WriteCMDD1(hPCI, imask >> 8);
	if ((result = pclPciExec(hPCI, 0x04)) != PCL_OK) return(result);
	gPciHdl[hdl].imask = PCI_ReadDATA0(hPCI);
	if (gPciHdl[hdl].irq16) gPciHdl[hdl].imask |= PCI_ReadDATA1(hPCI) << 8;
	if ((result = pclIntEnable(hPCI, gPciHdl[hdl].imask != 0)) != PCL_OK) return(result);
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclResetIntMask(	// $05, reset interrupt bits
	int hdl,					// in:	handle from pclOpen()
	int imask					// in:	interrupt mask, lk. PCL_IMASK_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, imask);
	PCI_WriteCMDD1(hPCI, imask >> 8);
	if ((result = pclPciExec(hPCI, 0x05)) != PCL_OK) return(result);
	gPciHdl[hdl].imask = PCI_ReadDATA0(hPCI);
	if (gPciHdl[hdl].irq16) gPciHdl[hdl].imask |= PCI_ReadDATA1(hPCI) << 8;
	if ((result = pclIntEnable(hPCI, gPciHdl[hdl].imask != 0)) != PCL_OK) return(result);
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclSetIntHandler(	// @69,	set interrupt handler
	int hdl,					// in:	handle from pclOpen()
	PCL_INT_HANDLER intHandler	// in:	address of interrupt handler
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);
	BOOL enabled = PCI_IntIsEnabled(hPCI);

	if ((result = pclIntEnable(hPCI, FALSE)) != PCL_OK) return(result);
	gPciHdl[hdl].intHandler.f = intHandler;
	if ((result = pclIntEnable(hPCI, enabled)) != PCL_OK) return(result);
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclIntAck(	// @70,	interrupt acknowledge
	int hdl,				// in:	handle from pclOpen()
	int imask				// in:	interrupt mask, lk. PCL_IMASK_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, imask);
	if ((result = pclPciExec(hPCI, 0x48)) != PCL_OK) return(result);
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclSetIntPriority(	// @71, set interrupt priority
   int hdl,							// in:	handle from pclOpen()
   int priority						// in:	priority of interrupt handler
) {
	PCI_GET_HANDLE(hdl, hPCI);
	
	if (!PCI_IntIsEnabled(hPCI)) {
		sprintf(PCI_ErrorString, "pclPciSetIntPriority(): Interrupt thread is not running\n");
		return(PCL_ERR_IOCTL);
	}

	if (!PCI_IntPriority(hPCI, priority)){
		sprintf(PCI_ErrorString, "pclPciSetIntPriority(): Can't set priority of interrupt thread\n");
		return(PCL_ERR_IOCTL);
	}

	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclGetThreadHandle(	// @90, get thread handle
   int hdl,							// in:	handle from pclOpen()
   int thread,						// in:	thread to use, like PCL_THREAD_xxx
   HANDLE *phHandle					// out:	handle of thread
) {
	PCI_GET_HANDLE(hdl, hPCI);
	HANDLE hThread;

	switch (thread) {
	case PCL_THREAD_INT:
		hThread = PCL_GetIntHandleThread(hPCI);
		break;
	case PCL_THREAD_EVENT:
		hThread = PCL_GetEventHandleThread(hPCI);
		break;
	default:
		sprintf(PCI_ErrorString, "pclGetThreadHandle(): thread parameter out of range\n");
		return(PCL_ERR_RANGE);
	}

	CHECK_PTR(phHandle);
	*phHandle = hThread;
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

int _PCL_FDEF pclReaderEnable(	// $10, reader on/off
	int hdl,					// in:	handle from pclOpen()
	int rmask					// in:	reader mask, like PCL_RMASK_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, rmask);
	PCI_WriteCMDD1(hPCI, rmask >> 8);
	if ((result = pclPciExec(hPCI, 0x10)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.readerEnable.use = TRUE;
	gPciHdl[hdl].persist.readerEnable.rmask = rmask;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclVitcSetup(	// @65, $14, vitc setup (pcl5/6)
	int hdl,				// in:	handle from pclOpen()
	int line1,				// in:	1st line, hexadecimal
	int line2,				// in:	2nd line, hexadecimal
	int vline				// in:	vitc line mode, like PCL_VLINE_xxx
) {
	int result;
	BYTE err;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, line1);
	PCI_WriteCMDD1(hPCI, line2);
	if ((result = pclPciExec(hPCI, 0x14)) != PCL_OK) return(result);
	if ((err = PCI_ReadDATA0(hPCI)) != 0) {
		sprintf(PCI_ErrorString, "pclPciVitcSetup(): VITC lines out of range, reason: 0x%X\n", err);
		return(PCL_ERR_RANGE);
	}
	PCI_WriteCMDD0(hPCI, vline);
	if ((result = pclPciExec(hPCI, 0x15)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.vitcSetup.use = TRUE;
	gPciHdl[hdl].persist.vitcSetup.line1 = line1;
	gPciHdl[hdl].persist.vitcSetup.line2 = line2;
	gPciHdl[hdl].persist.vitcSetup.vline = vline;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclMixedEnable(	// $20, mixed register: reader on/off
	int hdl,					// in:	handle from pclOpen()
	int rmask					// in:	reader mask, like PCL_RMASK_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, rmask);
	if ((result = pclPciExec(hPCI, 0x20)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.mixedEnable.use = TRUE;
	gPciHdl[hdl].persist.mixedEnable.rmask = rmask;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclPriority(	// $21, priority: mixed reg. tc priority
	int hdl,				// in:	handle from pclOpen()
	int prior				// in:	priority, like PCL_PRIOR_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, prior);
	if ((result = pclPciExec(hPCI, 0x21)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.priority.use = TRUE;
	gPciHdl[hdl].persist.priority.prior = prior;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclTcFrames(	// $30, tc frame rate
	int hdl,				// in:	handle from pclOpen()
	int tcfr				// in:	frame rate, like PCL_TCFR_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, tcfr);
	if ((result = pclPciExec(hPCI, 0x30)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.tcFrames.use = TRUE;
	gPciHdl[hdl].persist.tcFrames.tcfr = tcfr;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclLtcFrames(	// $31, set ltc frame rate
	int hdl,				// in:	handle from pclOpen()
	int ltcfr				// in:	ltc frame rate, like PCL_TCFR_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ltcfr);
	if ((result = pclPciExec(hPCI, 0x31)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.ltcFrames.use = TRUE;
	gPciHdl[hdl].persist.ltcFrames.ltcfr = ltcfr;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclVitcFrames(	// $32, set vitc frame rate
	int hdl,					// in:	handle from pclOpen()
	int vitcfr					// in:	vitc frame rate, like PCL_TCFR_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, vitcfr);
	if ((result = pclPciExec(hPCI, 0x32)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.vitcFrames.use = TRUE;
	gPciHdl[hdl].persist.vitcFrames.vitcfr = vitcfr;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclVitcLevelControl(	// $16, vitc level control
	int hdl,						// in:	handle from pclOpen()
	int vlmode,						// in:	vitc level control mode,
									//		like PCL_VLMODE_xxx
	int vlev,						// in:	vitc level, binary,
									//		like PCL_VLEX_xxx
	int *vlret						// out:	vitc level return, binary,
									//		like PCL_VLEX_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, vlmode);
	PCI_WriteCMDD1(hPCI, vlev);
	if ((result = pclPciExec(hPCI, 0x16)) != PCL_OK) return(result);
	if (vlret != NULL) *vlret = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.vitcLevelControl.use = TRUE;
	gPciHdl[hdl].persist.vitcLevelControl.vlmode = vlmode;
	gPciHdl[hdl].persist.vitcLevelControl.vlev = vlev;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclLtcError(	// $17, get ltc error counter
	int hdl,				// in:	handle from pclOpen()
	int ecmode,				// in:	error counter mode, like PCL_ECMODE_xxx
	int *ecval				// out:	error counter, 8 bit binary
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x17)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.ltcError.use = TRUE;
	gPciHdl[hdl].persist.ltcError.ecmode = ecmode;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclVitcError(	// $18, get vitc error counter
	int hdl,				// in:	handle from pclOpen()
	int ecmode,				// in:	error counter mode, like PCL_EMODE_xxx
	int *ecval				// out:	error counter, 8 bit binary
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x18)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.vitcError.use = TRUE;
	gPciHdl[hdl].persist.vitcError.ecmode = ecmode;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclFlyWheel(	// $19, set flywheel mode
	int hdl,				// in:	handle from pclOpen()
	int fwmode				// in:	fly wheel mode, like PCL_FWMODE_xxx
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, fwmode);
	if ((result = pclPciExec(hPCI, 0x19)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.flyWheel.use = TRUE;
	gPciHdl[hdl].persist.flyWheel.fwmode = fwmode;
#endif
	return(PCL_OK);
}

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAtcLtcError(	/* @85,	$22, get atc ltc error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_ECMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x22)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.atcLtcError.use = TRUE;
	gPciHdl[hdl].persist.atcLtcError.ecmode = ecmode;
#endif
	return(PCL_OK);
}

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAtcVitc1Error(	/* @86,	$23, get atc vitc1 error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x23)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.atcVitc1Error.use = TRUE;
	gPciHdl[hdl].persist.atcVitc1Error.ecmode = ecmode;
#endif
	return(PCL_OK);
}

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclAtcVitc2Error(	/* @87,	$24, get atc vitc2 error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x24)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.atcVitc2Error.use = TRUE;
	gPciHdl[hdl].persist.atcVitc2Error.ecmode = ecmode;
#endif
	return(PCL_OK);
}

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclHancLtcError(	/* @88,	$25, get hanc ltc error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_ECMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x25)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.hancLtcError.use = TRUE;
	gPciHdl[hdl].persist.hancLtcError.ecmode = ecmode;
#endif
	return(PCL_OK);
}

/*-------------------------------------------------------------------------*/

int _PCL_FDEF pclHancVitcError(	/* @89,	$26, get hanc vitc error counter */
	int hdl,					/* in:	handle from pclOpen() */
	int ecmode,					/* in:	error counter mode, like PCL_EMODE_xxx */
	int *ecval					/* out:	error counter, 8 bit binary */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, ecmode);
	if ((result = pclPciExec(hPCI, 0x26)) != PCL_OK) return(result);
	if (ecval != NULL) *ecval = PCI_ReadDATA0(hPCI);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.hancVitcError.use = TRUE;
	gPciHdl[hdl].persist.hancVitcError.ecmode = ecmode;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------

#if defined(_Windows) || defined(_WIN32)

int _PCL_FDEF pclGetDriverInformation(		// @78, get driver information
	int hdl,								// in:	handle from pclOpen()
	PCL_DRIVER_INFORMATION _PCL_OUT *info	// out:	information data structure
) {
	PCI_GET_HANDLE(hdl, hPCI);
	CHECK_PTR(info);
	if (info->size < sizeof(*info)) return(PCL_ERR_RANGE);

	info->hWD = hPCI->hWD;

	{
		WD_VERSION ver;
		BZERO(ver);
		WD_Version(hPCI->hWD, &ver);

		info->sWdVersion.dwVer = ver.dwVer;
		strncpy(info->sWdVersion.cVer, ver.cVer, sizeof(info->sWdVersion.cVer));
	}

	{
		info->wVendorID = gPciHdl[hdl].wVendorID;
		info->wDeviceID = gPciHdl[hdl].wDeviceID;
		info->wSubsystemVendorID = gPciHdl[hdl].wSubsystemVendorID;
		info->wSubsystemDeviceID = gPciHdl[hdl].wSubsystemDeviceID;
	}
	{
		info->intHandler = gPciHdl[hdl].intHandler.p;
		info->chipVer = PCI_ReadVERSION(hPCI);
	}
	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetIdata(	// @79, get idata
	int hdl,				// in:	handle from pclOpen()
	int addr,				// in:	idata address
	int _PCL_OUT *idata		// out:	byte read from idata
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);
	CHECK_PTR(idata);

	PCI_WriteCMDD0(hPCI, addr);
	if ((result = pclPciExec(hPCI, 0x38)) != PCL_OK) return(result);
	*idata = PCI_ReadDATA0(hPCI);

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetPdata(	// @80, get pdata
	int hdl,				// in:	handle from pclOpen()
	int addr,				// in:	pdata address
	int _PCL_OUT *pdata		// out:	byte read from pdata
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);
	CHECK_PTR(pdata);

	PCI_WriteCMDD0(hPCI, addr);
	if ((result = pclPciExec(hPCI, 0x3A)) != PCL_OK) return(result);
	*pdata = PCI_ReadDATA0(hPCI);

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetXdata(	// @83, get xdata
	int hdl,				// in:	handle from pclOpen()
	int addr,				// in:	xdata address
	int _PCL_OUT *xdata		// out:	byte read from xdata
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);
	CHECK_PTR(xdata);

	PCI_WriteCMDD0(hPCI, addr >> 8);
	PCI_WriteCMDD1(hPCI, addr);
	if ((result = pclPciExec(hPCI, 0x3C)) != PCL_OK) return(result);
	*xdata = PCI_ReadDATA0(hPCI);

	return(PCL_OK);
}

//---------------------------------------------------------------------------

#ifdef PCL_OPT_GPI_OUT			/* event driven gpi outputs */

int _PCL_FDEF pclSetGpiEventTime(	/* @81, set gpi event time */
	int hdl,						/* in:	handle from pclOpen() */
	int gpi,						/* in:	number of gpi output (1 or 2) */
	int reg,						/* in:	registers number like PCL_REG_xxx */
	int mode,						/* in:	registers number like PCL_GPIM_xxx */
	PCL_TC time						/* in:	timecode of event, bcd */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	if (reg != PCL_REG_MIXED) {
		sprintf(PCI_ErrorString, "pclPciSetGpiEventTime(): Only mixed register supported\n");
		return(PCL_ERR_INV_REG);
	}
	if (gpi < 1 || gpi > 2) {
		sprintf(PCI_ErrorString, "pclPciSetGpiEventTime(): Only GPI's 1 or 2 supported\n");
		return(PCL_ERR_RANGE);
	}

	int base = 0x24 + gpi * 4;	// 1 ergibt 0x28, 2 ergibt 0x2C

	PCI_WriteCMDD0(hPCI, *((BYTE *) &time + 3));	// hour
	PCI_WriteCMDD1(hPCI, *((BYTE *) &time + 2));	// minute
	if ((result = pclPciExec(hPCI, base + 0)) != PCL_OK) return(result);

	PCI_WriteCMDD0(hPCI, *((BYTE *) &time + 1));	// second
	PCI_WriteCMDD1(hPCI, *((BYTE *) &time + 0));	// frame
	if ((result = pclPciExec(hPCI, base + 1)) != PCL_OK) return(result);

	PCI_WriteCMDD0(hPCI, *((BYTE *) &mode + 0));
	PCI_WriteCMDD1(hPCI, *((BYTE *) &mode + 1));
	if ((result = pclPciExec(hPCI, base + 2)) != PCL_OK) return(result);

#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.setGpiEventTime.use = TRUE;
	gPciHdl[hdl].persist.setGpiEventTime.gpi = gpi;
	gPciHdl[hdl].persist.setGpiEventTime.reg = reg;
	gPciHdl[hdl].persist.setGpiEventTime.mode = mode;
	gPciHdl[hdl].persist.setGpiEventTime.time = time;
#endif

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetGpiEventStatus(	/* @82, get gpi event status */
	int hdl,						/* in:	handle from pclOpen() */
	int gpi,						/* in:	number of gpi output (1 or 2) */
	int _PCL_OUT *status,			/* out:	status like PCL_GPIS_xxx */
	int _PCL_OUT *count				/* out:	pulse counter */
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	if (gpi < 1 || gpi > 2) {
		sprintf(PCI_ErrorString, "pclPciSetGpiEventTime(): Only GPI's 1 or 2 supported\n");
		return(PCL_ERR_RANGE);
	}

	int base = 0x24 + gpi * 4;	// 1 ergibt 0x28, 2 ergibt 0x2C

	if ((result = pclPciExec(hPCI, base + 3)) != PCL_OK) return(result);

	if (status != 0) *status = PCI_ReadDATA0(hPCI);
	if (count  != 0) *count  = PCI_ReadDATA1(hPCI);

	return(PCL_OK);
}

#endif

//---------------------------------------------------------------------------

#ifndef PCL_PASCAL

PCL_TC _PCL_FDEF pclTc2Bcd(	// convert binary timecode to packed bcd
	int hours,				// in:	hours of binary timecode
	int minutes,			// in:	minutes of binary timecode
	int seconds,			// in:	seconds of binary timecode
	int frames				// in:	frames of binary timecode
) {
	return(
		((long) HEX2BCD(hours)   << 24) |
		((long) HEX2BCD(minutes) << 16) |
		((long) HEX2BCD(seconds) <<  8) |
		((long) HEX2BCD(frames)       )
	);
}

#endif

//---------------------------------------------------------------------------

int _PCL_FDEF pclBcd2Tc(	// convert packed bcd timecode to binary
	PCL_TC time,			// in:	packed bcd timecode
	int _PCL_OUT *hours,	// out:	hours of binary timecode
	int _PCL_OUT *minutes,	// out:	minutes of binary timecode
	int _PCL_OUT *seconds,	// out:	seconds of binary timecode
	int _PCL_OUT *frames	// out:	frames of binary timecode
) {
	if (hours	!= NULL) *hours	  = BCD2HEX(HIBYTE(HIWORD(time)));
	if (minutes != NULL) *minutes = BCD2HEX(LOBYTE(HIWORD(time)));
	if (seconds != NULL) *seconds = BCD2HEX(HIBYTE(LOWORD(time)));
	if (frames  != NULL) *frames  = BCD2HEX(LOBYTE(LOWORD(time)));

	return(PCL_OK);
}

//---------------------------------------------------------------------------

#ifndef PCL_PASCAL

long _PCL_FDEF pclTc2Frames(	// convert packed bcd timecode to long
	PCL_TC time,				// in:	packed bcd timecode
	int frate					// in:	frame rate (25 or 30)
) {
	return(
		(long) BCD2HEX(LOBYTE(LOWORD(time)))                   +
		(long) BCD2HEX(HIBYTE(LOWORD(time))) * frate           +
		(long) BCD2HEX(LOBYTE(HIWORD(time))) * frate * 60      +
		(long) BCD2HEX(HIBYTE(HIWORD(time))) * frate * 60 * 60
	);
}

#endif

//---------------------------------------------------------------------------

#if defined PCL_OPT_REALTIME && !defined PCL_PASCAL

#define PCL_PCI_REALTIME_TICK_NANO 9079

//---------------------------------------------------------------------------

PCL_TC _PCL_FDEF pclTcInc(PCL_TC time, unsigned rate)
{
	time += 0x00000001L;
	if ((time & 0x0000000FL) > 0x00000009L) {
		time += 0x00000010L - 0x0000000AL;
	}
	if ((time & 0x000000FFL) > HEX2BCD(rate - 1)) {
		time += 0x00000100L - HEX2BCD(rate);

		if ((time & 0x00000F00L) > 0x00000900L) {
			time += 0x00001000L - 0x00000A00L;
		}
		if ((time & 0x0000FF00L) > 0x00005900L) {
			time += 0x00010000L - 0x00006000L;

			if ((time & 0x000F0000L) > 0x00090000L) {
				time += 0x00100000L - 0x000A0000L;
			}
			if ((time & 0x00FF0000L) > 0x00590000L) {
				time += 0x01000000L - 0x00600000L;

				if ((time & 0x0F000000L) > 0x09000000L) {
					time += 0x10000000L - 0x0A000000L;
				}
				if ((time & 0xFF000000L) > 0x23000000L) {
					time += (PCL_TC) (0x00000000L - 0x24000000L);
				}
			}
		}
	}
	return(time);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclGetRealtime(	// @102, $15, get realtime
	int hdl,					// in:	handle from pclOpen()
	PCL_REALTIME _PCL_OUT *tm	// out:	realtime from pcl
) {
	PCL_TC tim, user;
	WORD tick;
	BYTE dval;
	long micro;
	PCI_GET_HANDLE(hdl, hPCI);
	BYTE ack;
	static BYTE lastAck;
	int framesPerSecond;

	CHECK_PTR(tm);
	tm->ltc.valid = PCL_RTVAL_INVALID;

	ack = PCI_ReadACK(hPCI);
	if (ack == lastAck) {
		sprintf(PCI_ErrorString, "pclPciGetRealtime(): ACK was not changed (%2x)\n", ack);
		return(PCL_ERR_TIMEOUT);
	}
	lastAck = ack;

	*((BYTE *) &tick    ) = (BYTE) PCI_ReadDATA4(hPCI);
	*((BYTE *) &tick + 1) = (BYTE) PCI_ReadDATA5(hPCI);

	*((BYTE *) &tim     ) = (BYTE) PCI_ReadDATA3(hPCI);
	*((BYTE *) &tim  + 1) = (BYTE) PCI_ReadDATA2(hPCI);
	*((BYTE *) &tim  + 2) = (BYTE) PCI_ReadDATA1(hPCI);
	*((BYTE *) &tim  + 3) = (BYTE) PCI_ReadDATA0(hPCI);

	*((BYTE *) &user    ) = (BYTE) PCI_ReadDATA9(hPCI);
	*((BYTE *) &user + 1) = (BYTE) PCI_ReadDATA8(hPCI);
	*((BYTE *) &user + 2) = (BYTE) PCI_ReadDATA7(hPCI);
	*((BYTE *) &user + 3) = (BYTE) PCI_ReadDATA6(hPCI);

	dval                  = (BYTE) PCI_ReadDATAA(hPCI);

#if defined _NTDDK_
	{
		static LARGE_INTEGER t;
		TIME_FIELDS tf;

		RtlTimeToTimeFields(&t, &tf);
		tm->system.day   = tf.Day;
		tm->system.month = tf.Month;
		tm->system.year  = tf.Year;
		tm->system.hour  = tf.Hour;
		tm->system.min   = tf.Minute;
		tm->system.sec   = tf.Second;
		tm->system.milli = tf.Milliseconds;

		PCI_WriteCMD(hPCI, 0x15);
		KeQuerySystemTime(&t);
	}
#elif defined WIN32
	{
		static SYSTEMTIME t;
		int priority;
		DWORD clas;
		HANDLE thread;
		HANDLE process;
		BOOL b;

		tm->system.day   = t.wDay;
		tm->system.month = t.wMonth;
		tm->system.year  = t.wYear;
		tm->system.hour  = t.wHour;
		tm->system.min   = t.wMinute;
		tm->system.sec   = t.wSecond;
		tm->system.milli = t.wMilliseconds;

		process = GetCurrentProcess();
		clas = GetPriorityClass(process);

		thread = GetCurrentThread();
		priority = GetThreadPriority(thread);

		b = SetPriorityClass(process, REALTIME_PRIORITY_CLASS);
		b = SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);

		PCI_WriteCMD(hPCI, 0x15);
//		Sleep(200);
		GetSystemTime(&t);

		b = SetPriorityClass(process, clas);
		b = SetThreadPriority(thread, priority);
	}
#elif defined LINUX
	{
		
	}
#else
	{
		static struct time t;
		static struct date d;
		struct tm st;
		time_t tt;
		struct tm *pt;

		st.tm_sec   = t.ti_sec;
		st.tm_min   = t.ti_min;
		st.tm_hour  = t.ti_hour;
		st.tm_mday  = d.da_day;
		st.tm_mon   = d.da_mon - 1;
		st.tm_year  = d.da_year - 1900;
		st.tm_isdst = 0;

		tt = mktime(&st);
		pt = gmtime(&tt);

		tm->system.day   = pt->tm_mday;
		tm->system.month = pt->tm_mon + 1;
		tm->system.year  = pt->tm_year + 1900;
		tm->system.hour  = pt->tm_hour;
		tm->system.min   = pt->tm_min;
		tm->system.sec   = pt->tm_sec;
		tm->system.milli = t.ti_hund * 10;

		PCI_WriteCMD(hPCI, 0x15);
		gettime(&t);
		getdate(&d);
	}
#endif

	if (tick >= 0x1200) {
		return(PCL_NO_NEWD);
	}
	if (dval == 0) {
		tm->ltc.valid |= PCL_RTVAL_DATE;
	}

	micro = ((long) tick * PCL_PCI_REALTIME_TICK_NANO) / 1000;

	tm->ltc.milli = (WORD) (micro / 1000);
	tm->ltc.micro = (WORD) (micro % 1000);

	{
		static int lastSecond, lastFrame, frate = PCL_FRAME_UNKNOWN, newFrate, change = 0;
		int second, frame;

		frame  = BCD2HEX((int) (tim)      & 0xFF);
		second = BCD2HEX((int) (tim >> 8) & 0xFF);

		if (second == lastSecond + 1) {
			switch (newFrate) {
			case PCL_FRAME_25:
				if (lastFrame < 25) change++; else change = -1;
				break;
			case PCL_FRAME_30DF:
				if (lastFrame > 25) change++; else change = -1;
				break;
			default:
				change = -1;
				break;
			}
			if (change == -1) {
				if (lastFrame < 25) newFrate = PCL_FRAME_25;
				if (lastFrame > 25) newFrate = PCL_FRAME_30DF;
			}
			if (change >= 5) {
				frate = newFrate;
				change = 0;
			}
		}
		lastSecond = second;
		lastFrame = frame;
		tm->ltc.frame = frate;
	}
/*
	{
		tim = 0x01020304;
		tm->ltc.milli = 5;
	}
*/
	if (tm->ltc.frame == PCL_FRAME_30DF) {
		__int64 milli, frame, min, min10;
		int minute = BCD2HEX((int) (tim >> 16) & 0xFF) + BCD2HEX((int) (tim >> 24) & 0xFF) * 60;
#		define milliPerFrame30 1001
#		define milliPerDay (24l*60*60*1000)
#		define framesPerDay (2589408l)
		milli = tm->ltc.milli;
		frame = (
			BCD2HEX((int) (tim)       & 0xFF) +
			BCD2HEX((int) (tim >> 8)  & 0xFF) * 30 +
			BCD2HEX((int) (tim >> 16) & 0xFF) * 30 * 60 +
			BCD2HEX((int) (tim >> 24) & 0xFF) * 30 * 60 * 60
		);
		min   = minute;
/*#ifdef LINUX
		min10 = minute;
		do_div(min10, 10);
#else*/
		min10 = minute / 10;
//#endif	
		frame -= (min - min10) * 2;

		{
			// Die Berechnung in der folgenden Zeile basiert auf der
			// NTSC-Nennfrequenz.  Sie fhrt um Mitternacht zu
			// einem Sprung der LTC-Zeit von 86ms.
//			milli = milli + (frame * milliPerFrame30) / 30;

			// Die folgende Zeile umgeht dies, indem sie nicht von der
			// NTSC-Nennfrequenz ausgeht, sondern den Tag in 'framesPerDay'
			// gleiche Teile einteilt
/*#ifdef LINUX
			milli += (frame * milliPerDay);
			do_div(milli, framesPerDay);
#else*/
			milli = milli + (frame * milliPerDay) / framesPerDay;
//#endif	
		}
/*#ifdef LINUX
		tm->ltc.milli = (int) do_div(milli, 1000);	//milli /= 1000;
		tm->ltc.sec   = (int) do_div(milli, 60);	//milli /= 60;
		tm->ltc.min   = (int) do_div(milli, 60);	//milli /= 60;
		tm->ltc.hour  = (int) do_div(milli, 24);
#else*/
		tm->ltc.milli = (int) (milli % 1000);	milli /= 1000;
		tm->ltc.sec   = (int) (milli % 60);	milli /= 60;
		tm->ltc.min   = (int) (milli % 60);	milli /= 60;
		tm->ltc.hour  = (int) (milli % 24);
//#endif
	} else {
		switch (tm->ltc.frame) {
		case PCL_FRAME_24:
			framesPerSecond = 24;
			break;
		case PCL_FRAME_25:
		default:
			framesPerSecond = 25;
			break;
		case PCL_FRAME_30:
			framesPerSecond = 30;
			break;
		}
		if (tm->ltc.milli * framesPerSecond >= 1000) {
			tim = pclTcInc(tim, framesPerSecond);
			if (tim == 0) {
				tm->ltc.valid &= ~PCL_RTVAL_DATE;
			}
			tm->ltc.milli -= 1000 / framesPerSecond;
		}
		tm->ltc.milli += (BCD2HEX((int) tim & 0xFF) * 1000) / framesPerSecond;	tim >>= 8;
		tm->ltc.sec    = BCD2HEX((int) tim & 0xFF);					tim >>= 8;
		tm->ltc.min    = BCD2HEX((int) tim & 0xFF);					tim >>= 8;
		tm->ltc.hour   = BCD2HEX((int) tim & 0xFF);
	}

	tm->ltc.year  = BCD2HEX((int) user & 0xFF) + 1900;	user >>= 8;
	tm->ltc.month = BCD2HEX((int) user & 0xFF);			user >>= 8;
	tm->ltc.day   = BCD2HEX((int) user & 0xFF);			user >>= 8;
	tm->ltc.tzone = ((int) user >> 1) & 3;
	if (user & 0x01) tm->ltc.valid |= PCL_RTVAL_TIME;

	if (tm->ltc.year < 1990) tm->ltc.year += 100; // y2k
	if (tm->ltc.frame == PCL_FRAME_UNKNOWN) {
		tm->ltc.valid = PCL_RTVAL_INVALID;
		return(PCL_NO_NEWD);
	}

	return(PCL_OK);
}

//---------------------------------------------------------------------------

int _PCL_FDEF pclSetRealtimeMode(	// @103, $1F, set realtime mode
	int hdl,						// in:	handle from pclOpen()
	int mode						// in:	mode for realtime reader
) {
	int result;
	PCI_GET_HANDLE(hdl, hPCI);

	PCI_WriteCMDD0(hPCI, mode);
	if ((result = pclPciExec(hPCI, 0x1F)) != PCL_OK) return(result);
#ifdef PCL_WAKEUP_PERSISTENT
	gPciHdl[hdl].persist.setRealtimeMode.use = TRUE;
	gPciHdl[hdl].persist.setRealtimeMode.mode = mode;
#endif
	return(PCL_OK);
}

//---------------------------------------------------------------------------
/*
int _PCL_FDEF pclGetRealtimeMonitor(
//	int hdl,
	PCL_REALTIME_MONITOR _PCL_OUT *rtm	// out:	realtime monitor data from pcl
) {
	CHECK_PTR(rtm);
	GetSharedMem(rtm);
	return(PCL_OK);
}
*/
#endif

//---------------------------------------------------------------------------
