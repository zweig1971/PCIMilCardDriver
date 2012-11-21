//***************************************************************************
//*																			*
//*	Windows DLL AVPCL.DLL													*
//*																			*
//*	Copyright (c) 1995-2007 by Alpermann+Velte								*
//*																			*
//***************************************************************************

// <params.h> Alpermann+Velte DLL for AV-PCL

//---------------------------------------------------------------------------

#define AVPCL_VER     3
#define AVPCL_VER_S  "3"
#define AVPCL_SUB    36  // 3.36
#define AVPCL_SUB_S "36" // 3.36

#define VER_LEGAL_COPYRIGHT "(c) Alpermann+Velte e.e. GmbH 1995-2007\0"

//---------------------------------------------------------------------------

#define PCL_TIMEOUT 40                         // timeout in millisekunden

#define PCL_OPT_GETCMD                    // get command bytes from device

//#define PCL_OPT_MTD                                    // read mtd times

#define PCL_OPT_REALTIME                        // pcl as real time source

#define PCL_WAKEUP_PERSISTENT

//#define PERF_COUNT                             // v1/v2 timing hinbiegen

//---------------------------------------------------------------------------

#define _PCL_EXTERN extern
#define _PCL_CONST  const

//---------------------------------------------------------------------------
