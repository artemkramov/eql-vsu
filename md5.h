/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
 */

#ifndef _MD5_H_
#define _MD5_H_ 1

#if !defined(_WIN32)
#ifdef __ICCARM__
#ifndef __I_STDLIB_H
typedef unsigned long uint32;
#endif
#else
#include <sys/types.h>
typedef u_int32_t uint32;
#endif
#else
typedef unsigned __int32 uint32;
#endif /* _WIN32 */

#ifndef POINTER_TYPE
#define POINTER_TYPE    1
typedef unsigned char *POINTER;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* MD5 context. */
typedef struct {
  uint32 state[4];                                   /* state (ABCD) */
  uint32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void MD5_Init (MD5_CTX *);
void MD5_Update (MD5_CTX *, const unsigned char *, unsigned int);
void MD5_Final (unsigned char [16], MD5_CTX *);

#ifdef __cplusplus
}
#endif

#endif
