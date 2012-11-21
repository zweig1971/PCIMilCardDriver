/*******************************************\
*                                           *
*  Linux Kernel Driver for PCL PCI Devices  *
*                                           *
*        Part of the shared library         *
*                                           *
* (c) 2005-2007 Alpermann + Velte EE GmbH   *
*                                           *
*                   V 3.36                  *
\********************************************
                        *      Source       *
                        *      funcs.h      *
                        ********************/
#ifndef _FUNCS_H
#define _FUNCS_H

#ifdef LINUX
#include "pci_lib_linux.h"
#endif

//---------------------------------------------------------------------------

typedef enum {
	TYPE_UNUSED = 0, // handle frei
	_TYPE_ISA, // pcl-isa
	TYPE_PCI  // pcl-pci
} tHdlType;

//---------------------------------------------------------------------------

_PCL_EXTERN struct {
	tHdlType type;
	//--------------------------
	int imask;
	BOOL irq16;
//	BOOL porSupported;
#ifdef _WIN32
	PCI_HANDLE hPCI;
#endif

	union {
		void *p;
#ifdef _WIN32
		PCL_INT_HANDLER f;
#endif
	} intHandler;

	WORD wVendorID;
	WORD wDeviceID;
	WORD wSubsystemVendorID;
	WORD wSubsystemDeviceID;

#ifdef PCL_WAKEUP_PERSISTENT
	struct {
		struct {
			PCL_BOOL use;
			int rmask;
		} readerEnable;
		struct {
			PCL_BOOL use;
			int line1;
			int line2;
			int vline;
		} vitcSetup;
		struct {
			PCL_BOOL use;
			int rmask;
		} mixedEnable;
		struct {
			PCL_BOOL use;
			int prior;
		} priority;
		struct {
			PCL_BOOL use;
			int tcfr;
		} tcFrames;
		struct {
			PCL_BOOL use;
			int ltcfr;
		} ltcFrames;
		struct {
			PCL_BOOL use;
			int vitcfr;
		} vitcFrames;
		struct {
			PCL_BOOL use;
			int vlmode;
			int vlev;
		} vitcLevelControl;
		struct {
			PCL_BOOL use;
			int ecmode;
		} ltcError;
		struct {
			PCL_BOOL use;
			int ecmode;
		} vitcError;
		struct {
			PCL_BOOL use;
			int fwmode;
		} flyWheel;
		struct {
			PCL_BOOL use;
			int gpi;
			int reg;
			int mode;
			PCL_TC time;
		} setGpiEventTime;
		struct {
			PCL_BOOL use;
			int mode;
		} setRealtimeMode;
		struct {
			PCL_BOOL use;
			int ecmode;
		} atcLtcError;
		struct {
			PCL_BOOL use;
			int ecmode;
		} atcVitc1Error;
		struct {
			PCL_BOOL use;
			int ecmode;
		} atcVitc2Error;
		struct {
			PCL_BOOL use;
			int ecmode;
		} hancLtcError;
		struct {
			PCL_BOOL use;
			int ecmode;
		} hancVitcError;
	} persist;
#endif
#if defined(PERF_COUNT) && !defined(LINUX)
	struct {
		LARGE_INTEGER freq, countV1, countV2;
	} perf;
#endif
} gPciHdl[PCL_HANDLES];

//---------------------------------------------------------------------------

#define CHECK_HANDLE(hdl) \
	if (hdl == NULL) { \
		pclErrorString = "Handle is NULL\n"; \
		return(PCL_ERR_INV_HDL); \
	} // handle 'hdl' auf plausibilit� prfen

//---------------------------------------------------------------------------

#ifdef _WIN32

#define PCI_GET_HANDLE(_hdl, _pci) \
	if (_hdl < 0 || _hdl >= PCL_HANDLES) { \
		pclErrorString = "Parameter '" #_hdl "' is out of range\n"; \
		return(PCL_ERR_INV_HDL); \
	} \
	PCI_HANDLE _pci = gPciHdl[_hdl].hPCI; \
	CHECK_HANDLE(_pci); \
	pclErrorString = pclPciGetErrorString(); \

#endif

#ifdef LINUX

#define PCI_GET_HANDLE(_hdl, _pci) \
	if (_hdl < 0 || _hdl >= PCL_HANDLES) { \
		pclErrorString = "Parameter '" #_hdl "' is out of range\n"; \
		return(PCL_ERR_INV_HDL); \
	} \
	PCI_HANDLE _pci = _hdl; \
	pclErrorString = pclPciGetErrorString(); \

#endif

//---------------------------------------------------------------------------

const char* _PCL_FDEF pclPciGetErrorString(		// @68, get error string
	void										// no parameters needed
);

//---------------------------------------------------------------------------

PCL_BOOL _PCL_FDEF pclPciExec(
	PCI_HANDLE hPCI,
	BYTE cmd
);

//---------------------------------------------------------------------------

int _PCL_FDEF pclIntEnable(	// enable or disable interrupts
	PCI_HANDLE hPCI,		// in:	handle from pclPciOpen()
	PCL_BOOL enable				// in:	enable or disable interrupt
);

//---------------------------------------------------------------------------

#endif // _FUNCS_H
