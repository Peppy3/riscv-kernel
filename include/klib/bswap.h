#ifndef KLIB_BSWAP_H
#define KLIB_BSWAP_H

// Endian swap functions

#include <klib/common.h>

static inline ushort swap16(ushort val) {
	return (val << 8) | (val >> 8);
}

static inline uint swap32(uint val) {
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | (val >> 16);
}

static inline ullong swap64(ullong val) {
	val =	((val << 8) & 0xFF00FF00FF00FF00ULL ) | 
			((val >> 8) & 0x00FF00FF00FF00FFULL );

	val =	((val << 16) & 0xFFFF0000FFFF0000ULL ) | 
			((val >> 16) & 0x0000FFFF0000FFFFULL );
	return (val << 32) | (val >> 32);
}

static inline uint swap32_from_be(uint val) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | (val >> 16);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	return val;
#endif
}

static inline uint swap32_from_le(uint val) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | (val >> 16);
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return val;
#endif
}

static inline ullong swap64_from_be(ullong val) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	val =	((val << 8) & 0xFF00FF00FF00FF00ULL ) | 
			((val >> 8) & 0x00FF00FF00FF00FFULL );

	val =	((val << 16) & 0xFFFF0000FFFF0000ULL ) | 
			((val >> 16) & 0x0000FFFF0000FFFFULL );
	return (val << 32) | (val >> 32);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	return val;
#endif
}

static inline ullong swap64_from_le(ullong val) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	val =	((val << 8) & 0xFF00FF00FF00FF00ULL ) | 
			((val >> 8) & 0x00FF00FF00FF00FFULL );

	val =	((val << 16) & 0xFFFF0000FFFF0000ULL ) | 
			((val >> 16) & 0x0000FFFF0000FFFFULL );
	return (val << 32) | (val >> 32);
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return val;
#endif
}

#endif /* KLIB_BSWAP_H */
