/**
 * A simple converter vor a bmp 32bit image to an includeable c-header
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int calc_size( int width, int height );
int calc_bit_width( int width);

void write_header( FILE *file, const char *sourcefile, int width, int height );
void write_footer( FILE *file );
void write_data( FILE *file, char *buffer, int width, int height);

int main(int argc, char **argv)
{
	// paranoia
	if( argc != 3)
	{
		printf(" %s <INPUT> <OUTPUT>\n", argv[0]);
		return 1;
	}

	// open image
	FILE* file = NULL; 
	if( argv[1][0] == '-' && argv[1][1] == '\0')
	{
		file = stdin;
	}
	else
	{
		file = fopen( argv[1], "r" );
	}
	
	if( !file )
	{
		printf("Could not open source.\n");
		return 2;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file);

	if( file_size < 55 )
	{
		printf("Source image is to small\n");
		return 2;
	}

	char *buffer = (char*)malloc( file_size );
	int bytes_read = fread( buffer, 1, file_size, file );
	while( bytes_read  < file_size)
	{
		bytes_read += fread(buffer+bytes_read, 1, file_size-bytes_read, file);
	}
	fclose(file);

	// check image read
	if( buffer[0] != 'B' || buffer[1] != 'M' )
	{
		printf("Source image is not an bmp!\n");
		return 3;
	}
	int width = (int)*(buffer+0x12);
	int height = (int)*(buffer+0x16);
	
	if( width <=1 || height <= 1 )
	{
		printf("Source image's size (%dx%d) is invalid\n", width, height);
		return 4;
	}

	// create output
	if( argv[2][0] == '-' && argv[2][1] == '\0')
	{
		file = stdout;
	}
	else
	{
		file = fopen(argv[2], "w");
	}
	if( !file )
	{
		printf("Could not open output file\n");
		return 4;
	}

	write_header(file, argv[1], width, height);
	write_data(file, buffer+54, width, height);
	write_footer(file);

	// cleanup
	fclose(file);
	free( buffer );

	printf("Done converting file\n");
	return 0;
}

int calc_bit_width( int width)
{
	return (ceilf(width/8.0f)*8);
}

int calc_size( int width, int height )
{
	return calc_bit_width(width)*height;
}

void write_header( FILE *file, const char *sourcefile, int width, int height )
{
	int bit_width = calc_bit_width( width );
	fprintf( file,  "// GENERATED BINARY IMAGE FROM \"%s\"\n", sourcefile);
	fprintf( file, 	"#define g_image_width %d\n"
			"#define g_image_width_in_bytes %d\n"
			"#define g_image_width_padding %d // size of padding per line in bit\n\n"
			"#define g_image_height %d\n"
			"#define g_image_size %d // size in bits, including filled bits! \n\n"
			, width, bit_width/8, bit_width - width, height, calc_size(width, height));
	fprintf( file,  "#define _ 0\n"
			"#define X 1\n"
			"#define P 0 // PADBIT\n\n"
			"Byte g_image[] =\n{\n");
}

void write_footer( FILE *file )
{
	fprintf(file, "};\n");
}

void write_data( FILE *file, char *buffer, int width, int height)
{
	int size = calc_size(width,height);
	int bit_width = calc_bit_width(width);
	int line = 0;
	
	for( int y = 0; y < height; ++y)
	{
		fprintf(file, "\t{ ");
		for( int x = 0; x < width; ++x)
		{
			// check if next bit will be in a new byte
			if( (x % 8) == 0 && x != 0)
			{
				fprintf(file, " }, { ");
			}
			// check if next bit is a new nibble
			else if( (x % 4) == 0 && x != 0)
			{
				fprintf(file, " ");
			}


			// take 24 bit and 4 byte alligned bmps into account
			int line = (height-1-y);
			int byte_boundary = width % 4;
			fprintf(file, "%s,", (buffer[(x+line*width)*3+line*byte_boundary] != 0) ? "X" : "_");
		}
		
		// print fill bits
		for( unsigned u = 0; u < (bit_width -width); ++u)
		{
			if( ((u+width) % 4) == 0)
			{
				fprintf(file, " ");
			}
			fprintf(file, "P,");
		}

		// newline
		fprintf(file, " }, \n");
	}
}
