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

static void print_byte( const Byte p )
{
	for( unsigned u = 0; u < 8; ++u)
	{
		printf("%d ", (p.data >> u) % 2);
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	Byte image[] = {
		{1,0,0,0, 0,0,0,0},
		{0,1,0,0, 0,0,0,0},
		{0,0,1,0, 0,0,0,0},
		{0,0,0,1, 0,0,0,0}
	};

	for( unsigned u = 0; u < sizeof(image); ++u)
	{
		printf("line %d: ", u);
		print_byte( image[u] );
	}

	printf("sizeof byte: %ld\n", sizeof(Byte) );
	printf("sizeof image: %ld\n", sizeof(image) );
}
