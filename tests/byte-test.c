/**
 * "Unit" test for byte structure
 */

#include <byte.h>
#include <math.h>

int main(int argc, char **argv)
{
	Byte b4 = { 0, 0, 0, 0, 0, 1, 0, 0 };
	if( b4 != 4 )
	{
		printf("00000100_2 != 4!\n");
		return -1;
	}

        Byte image[] = {
                {0,0,0,0, 0,0,0,0},
                {0,0,0,0, 0,0,0,1},
                {0,0,0,0, 0,0,1,0},
                {0,0,0,0, 0,1,0,0}
                {0,0,0,0, 1,0,0,0}

                {0,0,0,1, 0,0,0,0}
                {0,0,1,0, 0,1,0,0}
                {0,1,0,0, 0,1,0,0}
                {1,0,0,0, 0,1,0,0}
        };

        for( unsigned u = 0; u < sizeof(image); ++u)
        {
		if( pow(2,u) != image[u].data )
		{
			printf(" %d != ", pow(2,u) );
			print_byte(image[u]);
			return -1;
		}
        }

	return 0;
}

