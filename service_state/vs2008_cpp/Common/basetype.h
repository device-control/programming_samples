#ifndef _BASETYPE_H_
#define _BASETYPE_H_

#define ENDIAN_LITTLE (0)
#define ENDIAN_BIG (1)
// for default. 環境によって自動切り替えできるようにする
#define ENDIAN_TYPE ENDIAN_LITTLE

#define TRUE32 (1)
#define FALSE32 (0)
typedef unsigned long bool32;
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

#define USE_IT(x) ((void*)x) // 未使用ワーニング対応

#define _BYTE1(x) ( (x)       & 0xFF )
#define _BYTE2(x) ( (x >>  8) & 0xFF )
#define _BYTE3(x) ( (x >> 16) & 0xFF )
#define _BYTE4(x) ( (x >> 24) & 0xFF )

#if (ENDIAN_TYPE == ENDIAN_LITTLE)
// little
#define ENDIAN16(x) x
#define ENDIAN32(x) x
#define ENDIAN16_SELF(x) x = x
#define ENDIAN32_SELF(x) x = x
#else // #if (ENDIAN_TYPE == ENDIAN_LITTLE)
// big
#define ENDIAN16(x) ((uint16)( _BYTE1(x) << 8 | _BYTE2(x) ))
#define ENDIAN32(x) ((uint32)( _BYTE1(x) << 24 | _BYTE2(x) << 16 | _BYTE3(x) << 8 | _BYTE4(x) ))
#define ENDIAN16_SELF(x) x = ENDIAN16(x)
#define ENDIAN32_SELF(x) x = ENDIAN32(x)
#endif // #if (ENDIAN_TYPE == ENDIAN_LITTLE)

#endif // _BASETYPE_H_
