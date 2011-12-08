/**
 * Headerfile for lowlevel byte datatype
 */
#ifndef BYTE_H
#define BYTE_H

#include <stdio.h>
#include <stdlib.h>

#pragma pack(1) // define
typedef union
{
	struct // bits
	{
		unsigned int _0:1;
		unsigned int _1:1;
		unsigned int _2:1;
		unsigned int _3:1;
		unsigned int _4:1;
		unsigned int _5:1;
		unsigned int _6:1;
		unsigned int _7:1;
	};
	unsigned char data;
} Byte;

static void print_byte( const Byte b )
{
	for( unsigned u = 0; u < sizeof(Byte); ++u)
	{
		printf("%d ", (b.data >> u) % 2);
	}
	printf("\n");
}

#endif //  BYTE_H
