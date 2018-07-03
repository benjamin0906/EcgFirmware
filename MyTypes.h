/* 
 * File:   MyTypes.h
 * Author: Benjamin Bodnár
 *
 * Created on 2018. június 30., 15:09
 */

#ifndef MYTYPES_H
#define	MYTYPES_H

#define TRUE    1
#define FALSE   0

#ifdef PIC_16BIT
typedef unsigned char       uint8;
typedef unsigned int        uint16;
typedef unsigned long       uint32;
typedef unsigned long long  uint64;
typedef signed char         sint8;
typedef signed int          sint16;
typedef signed long         sint32;
typedef signed long         sint64;
//typedef unsigned char       boolean;
#endif

#endif	/* MYTYPES_H */

