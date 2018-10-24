// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   SignIt_types.h
*		General types and macros used by SignIt.
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#ifndef __SIGNIT_TYPES_H__
#define __SIGNIT_TYPES_H__

/*---------------------------------------------------------------------------------------------------------*/
/*                                        GENERIC TYPES DEFINITIONS                                        */
/*---------------------------------------------------------------------------------------------------------*/
typedef unsigned char  UINT8;                       /* Unsigned  8 bit quantity                            */
typedef signed   char  INT8;                        /* Signed    8 bit quantity                            */
typedef unsigned short UINT16;                      /* Unsigned 16 bit quantity                            */
typedef short          INT16;                       /* Signed   16 bit quantity                            */
typedef float          FP32;                        /* Single precision floating point                     */
typedef double         FP64;                        /* Double precision floating point                     */



#if ((~0ULL>>1) == 0x7FFFFFFFLL)
/*-----------------------------------------------------------------------------------------------------*/
/* unsigned long is 32bit (32bit Linux, 32bit Windows and 64bit Windows                                */
/*-----------------------------------------------------------------------------------------------------*/
typedef unsigned long   UINT32;                 /* Unsigned 32 bit quantity                            */
typedef signed   long   INT32;                  /* Signed   32 bit quantity                            */

#else
/*-----------------------------------------------------------------------------------------------------*/
/* unsigned long is 64bit (mostly on 64bit Linux systems)                                              */
/*-----------------------------------------------------------------------------------------------------*/
typedef unsigned int    UINT32;                 /* Unsigned 32 bit quantity                            */
typedef signed   int    INT32;                  /* Signed   32 bit quantity                            */

#endif

#endif /* __SIGNIT_TYPES_H__ */
