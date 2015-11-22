/*
 *  Architecture.h
 *
 *  Created by Atlas on 10/3/10.
 *  Copyright 2010 JTJ. All rights reserved.
 *
 */

#ifndef __ARCHITECTURE_H__
#define __ARCHITECTURE_H__

typedef unsigned char			u8,		*pu8;
typedef signed char				s8,		*ps8;
typedef unsigned short			u16,	*pu16;
typedef short					s16,	*ps16;
typedef unsigned long			u32,	*pu32;
typedef signed long				s32,	*ps32;
typedef unsigned long long		u64,	*pu64;
typedef long long				s64,	*ps64;

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#ifndef INLINECALL
#if defined(__cplusplus) || defined(__GNUC__)
#define INLINECALL inline
#else
#define INLINECALL /* not available */
#endif
#endif

#define ShortSwap(x)  (((((W16)(x))&0x00FF)<<8) | ((((W16)(x))&0xFF00) >> 8))
#define LongSwap(x)   ((((W32)(x))&0xFF000000)>> 24) | (((((W32)(x))&0xFF0000)>> 8)) | (((((W32)(x))&0xFF00)<< 8)) | ((((W32)(x))&0xFF)<< 24)

#if defined (IPHONE)
#define BigShort(x)		ShortSwap(x)
#define LittleShort(x)	(x)
#define BigLong(x)		LongSwap(x)
#define LittleLong(x)	(x)
#define BigFloat(x)		FloatSwap(x)
#define LittleFloat(x)	(x)
#endif

INLINECALL float FloatSwap(float f)
{
	union
	{
		float f;
		s8 b[4];
	} dat1, dat2;
	
	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	
	return dat2.f;
}


#endif
