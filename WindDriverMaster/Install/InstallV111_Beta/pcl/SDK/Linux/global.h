//***************************************************************************
//*																			*
//*	Windows DLL AVPCL.DLL													*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*																			*
//***************************************************************************

/* <global.h> Alpermann+Velte DLL for AV-PCL */
/* @(#)global.h 3.36 06/14/2007 (c) 1995-2007 Alpermann+Velte */

#ifndef _PCL_GLOBAL_H
#define _PCL_GLOBAL_H

//---------------------------------------------------------------------------

#ifndef LINUX
#undef  _PCL_FDEF
#define _PCL_FDEF        _stdcall
#define _PCL_NDEF static _fastcall
#endif

//---------------------------------------------------------------------------

_PCL_EXTERN const char *pclErrorString;

//---------------------------------------------------------------------------

#define CHECK_PTR(ptr) \
{ \
	if ((ptr) == NULL) { \
		pclErrorString = "Parameter " #ptr " is NULL\n"; \
		return(PCL_ERR_NULL); \
	} \
} // pointer 'ptr' auf plausibilit� prfen

//---------------------------------------------------------------------------

#define CHECK_RANGE(val, min, max, err) \
{ \
	if ((val) < (min)) { \
		pclErrorString = "Parameter " #val " is less than " #min "\n"; \
		return(err); \
	} \
	if ((val) > (max)) { \
		pclErrorString = "Parameter " #val " is greater than " #max "\n"; \
		return(err); \
	} \
} // wert 'val' auf grenzen 'min/max' prfen

//---------------------------------------------------------------------------

#endif // _PCL_GLOBAL_H

//---------------------------------------------------------------------------
