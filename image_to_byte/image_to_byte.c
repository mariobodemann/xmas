/**
 * A simple converter vor a bmp 32bit image to an includeable c-header
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned calc_size( unsigned width, unsigned height );
unsigned calc_bit_width( unsigned width);
void write_header( FILE *file, unsigned width, unsigned height );
void write_footer( FILE *file );
void write_data( FILE *file, unsigned char *buffer, unsigned width, unsigned height);

int main(int argc, char **argv)
{
	// paranoia
	if( argc != 3)
	{
		printf(" %s <INPUT> <OUTPUT>\n", argv[0]);
		return 1;
	}

	// open image
	FILE* file = fopen( argv[1], "r" );
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

	unsigned char *buffer = (unsigned char*)malloc( file_size );
	unsigned bytes_read = fread( buffer, 1, file_size, file );
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
	unsigned width = (unsigned)*((char*)buffer+0x12);
	unsigned height = (unsigned)*((char*)buffer+0x16);

	// create output
	file = fopen(argv[2], "w");
	if( !file )
	{
		printf("Could not open output file\n");
		return 4;
	}

	write_header(file, width, height);
	write_data(file, buffer+54, width, height);
	write_footer(file);

	// cleanup
	fclose(file);
	free( buffer );

	printf("Done converting file\n");
	return 0;
}

unsigned calc_bit_width( unsigned width)
{
	return (ceilf(width/8.0f)*8);
}

unsigned calc_size( unsigned width, unsigned height )
{
	return calc_bit_width(width)*height;
}

void write_header( FILE *file, unsigned width, unsigned height )
{
	fprintf( file,  "// GENERATED IMAGE FILE\n");
	fprintf( file, 	"unsigned g_image_width = %d;\n"
			"unsigned g_image_height = %d;\n"
			"unsigned g_image_size = %d; // size in bits, including filled bits! \n\n"
			, width, height, calc_size(width, height));
	fprintf( file,  "#define _ 0\n"
			"#define X 1\n"
			"#define F 0 // FILL BIT\n\n"
			"Byte g_image[] = {\n");
}

void write_footer( FILE *file )
{
	fprintf(file, "};\n");
}

void write_data( FILE *file, unsigned char *buffer, unsigned width, unsigned height)
{
	unsigned size = calc_size(width,height);
	unsigned bit_width = calc_bit_width(width);
	unsigned buffered = 0;
	fprintf(file, "\t{ ");
	for( unsigned u = 0; u < size; ++u) // ignore rgb parts, and use only b
	{
		// check if new line is needed
		if( ( u % bit_width) == 0 && u != 0)
		{
			// newline
			fprintf(file, " },\n\t{ ");
		}
		// check if next bit will be in a new byte
		else if( (u % 8) == 0 && u != 0)
		{
			fprintf(file, " }, { ");
		}
		// check if next bit is a new nibble
		else if( (u % 4) == 0 && u != 0)
		{
			fprintf(file, " ");
		}


		// check if this is a fill bit
		if( (u % bit_width ) > width-1 )
		{
			// print fill bit
			fprintf(file, "F,");
			buffered ++;
		}
		// print the image bit
		else
		{
			fprintf(file, "%s, ", (buffer[(u-buffered)*3] > 0x0) ? "X" : "_");
//			fprintf(file, "%d,", (u-buffered) % 10);
		}
	} // for loop through image
	fprintf(file, " }, \n");
}
