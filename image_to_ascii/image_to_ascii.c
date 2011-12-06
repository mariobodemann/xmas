/**
 * A simple converter vor a bmp 32bit image to an includeable c-header
 */
 
#include <stdio.h>
#include <stdlib.h>

void write_header( FILE *file, unsigned width, unsigned height );
void write_footer( FILE *file );
void write_data( FILE *file, unsigned char *buffer, size_t size);

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
	write_data(file, buffer+54, file_size-54);
	write_footer(file);
	
	// cleanup
	fclose(file);
	free( buffer );
	
	printf("Done converting file\n");
	return 0;
}

void write_header( FILE *file, unsigned width, unsigned height )
{
	fprintf( file, 	"unsigned g_image_width = %d;\n"
			"unsigned g_image_height = %d;\n", width, height);
	fprintf( file, "unsigned char g_image[] = {\n");
}

void write_footer( FILE *file )
{
	fprintf(file, "};\n");
}

void write_data( FILE *file, unsigned char *buffer, size_t size)
{
	fprintf(file, "\t");
	for( unsigned u = 0; u < size-1; u+=3) // ignore rgb parts, and use only b
	{
		fprintf(file, "%d, ", buffer[u] == 0xFF);
	}
	fprintf(file,"%d\n", buffer[size-1] == 0xFF);
}
