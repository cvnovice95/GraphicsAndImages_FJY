#include <stdio.h>
#include <stdlib.h>
/*
 * x86  little endian
 * FileHeader Total Bytes is 14
 */
typedef struct tagbmpfileheader
{
	unsigned short	bmptype;
	long		bmpsize; /* BMP file size */
	unsigned short	bmpreserved0;
	unsigned short	bmpreserved1;
	long		bmpoffset;
}bmpfileheader;
/* InfoHeader Total Bytes is 40 */
typedef struct tagbmpfileinfoheader
{
	long	bmpinfosize;
	long	bmpwidth;
	long	bmpheight;
	short	bmpplanes;
	short	bmpbitcount;
	long	bmpcompression;
	long	bmpimagesize;    /* BMP image size */
	long	bmpXPelsPerMete;
	long	bmpYPelsPerMete;
	long	bmpClrUsed;
	long	bmpClrImportant;
}bmpfileinfoheader;
/*
 * bitcount =1 ,8byte
 * bitcount =4 ,64byte
 * bitcount =8 ,1024byte
 */
typedef struct tagrgbpalette {
	unsigned char	rgbBlue;
	unsigned char	rgbGreen;
	unsigned char	rgbRed;
	unsigned char	rgbReserved;
}rgbpalette;

bmpfileheader		bfd;
bmpfileinfoheader	bid;
rgbpalette		brp;

void Show_Byte( unsigned char* sc, int size )
{
	int i;
	for ( i = 0; i < size; i++ )
	{
		printf( "%x ", sc[i] );
		if ( (i + 1) % 16 == 0 )
		{
			printf( "\n" );
		}
	}
}


void Show_Bmp_FileHeader( bmpfileheader* _bfd )
{
	printf( "BMP File Header Info\n" );
	printf( "BMP Type is %x\n", _bfd->bmptype );
	printf( "BMP Size is %d Bytes\n", _bfd->bmpsize );
	printf( "BMP Reserved0 is %x\n", _bfd->bmpreserved0 );
	printf( "BMP Reserved1 is %x\n", _bfd->bmpreserved1 );
	printf( "BMP Raw Data Offset is %d Bytes\n", _bfd->bmpoffset );
	printf( "====BMP File Header End====\n" );
}


void Show_Bmp_InfoHeader( bmpfileinfoheader* _bid )
{
	printf( "BMP File Info\n" );
	printf( "BMP Info Size is %d Bytes\n", _bid->bmpinfosize );
	printf( "BMP Width is %d  Pixel\n", _bid->bmpwidth );
	printf( "BMP Height is %d Pixel\n", _bid->bmpheight );
	printf( "BMP Planes is %d\n", _bid->bmpplanes );
	printf( "BMP BitCount is %d\n", _bid->bmpbitcount );
	printf( "BMP Compression is %d\n", _bid->bmpcompression );
	printf( "BMP ImageSize is %d Bytes\n", _bid->bmpimagesize );
	printf( "BMP XPelsPerMete is %d\n", _bid->bmpXPelsPerMete );
	printf( "BMP YPelsPerMete is %d\n", _bid->bmpYPelsPerMete );
	printf( "BMP ClrUsed is %d\n", _bid->bmpClrUsed );
	printf( "BMP ClrImportant is %d\n", _bid->bmpClrImportant );
	printf( "====BMP Info Header End====\n" );
}


void  Read_FileHeader( const char * filename, unsigned char FileHeader[] )
{
	FILE *fp = fopen( filename, "rb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fread( FileHeader, 14, 1, fp );
	fclose( fp );
}


void  Read_InfoHeader( const char * filename, unsigned char InfoHeader[] )
{
	FILE *fp = fopen( filename, "rb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fseek( fp, 14, SEEK_SET );
	fread( InfoHeader, 40, 1, fp );
	fclose( fp );
}


void Read_Header( const char * filename, unsigned char Header[] )
{
	FILE *fp = fopen( filename, "rb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fseek( fp, 0, SEEK_SET );
	fread( Header, 54, 1, fp );
	fclose( fp );
}


void Load_bmpfile_struct( const char* filename, bmpfileheader* _bfd )
{
	unsigned char	Header[54];
	FILE		*fp = fopen( filename, "rb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fseek( fp, 0, SEEK_SET );
	fread( Header, 54, 1, fp );
	fclose( fp );
	_bfd->bmptype		= Header[0] << 8 | Header[1];
	_bfd->bmpsize		= Header[5] << (8 * 3) | Header[4] << (8 * 2) | Header[3] << (8 * 1) | Header[3] << (8 * 0);
	_bfd->bmpreserved0	= Header[7] << 8 | Header[6];
	_bfd->bmpreserved1	= Header[9] << 8 | Header[8];
	_bfd->bmpoffset		= Header[13] << (8 * 3) | Header[12] << (8 * 2) | Header[11] << (8 * 1) | Header[10] << (8 * 0);
}


void Load_bmpinfo_struct( const char* filename, bmpfileinfoheader* _bid )
{
	unsigned char	Header[54];
	FILE		*fp = fopen( filename, "rb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fseek( fp, 0, SEEK_SET );
	fread( Header, 54, 1, fp );
	fclose( fp );
	_bid->bmpinfosize	= Header[17] << (8 * 3) | Header[16] << (8 * 2) | Header[15] << (8 * 1) | Header[14] << (8 * 0);
	_bid->bmpwidth		= Header[21] << (8 * 3) | Header[20] << (8 * 2) | Header[19] << (8 * 1) | Header[18] << (8 * 0);
	_bid->bmpheight		= Header[25] << (8 * 3) | Header[24] << (8 * 2) | Header[23] << (8 * 1) | Header[22] << (8 * 0);
	_bid->bmpplanes		= Header[27] << 8 | Header[26];
	_bid->bmpbitcount	= Header[29] << 8 | Header[28];
	_bid->bmpcompression	= Header[33] << (8 * 3) | Header[32] << (8 * 2) | Header[31] << (8 * 1) | Header[30] << (8 * 0);
	_bid->bmpimagesize	= Header[37] << (8 * 3) | Header[36] << (8 * 2) | Header[35] << (8 * 1) | Header[34] << (8 * 0);
	_bid->bmpXPelsPerMete	= Header[41] << (8 * 3) | Header[40] << (8 * 2) | Header[39] << (8 * 1) | Header[38] << (8 * 0);
	_bid->bmpYPelsPerMete	= Header[45] << (8 * 3) | Header[44] << (8 * 2) | Header[43] << (8 * 1) | Header[42] << (8 * 0);
	_bid->bmpClrUsed	= Header[49] << (8 * 3) | Header[48] << (8 * 2) | Header[47] << (8 * 1) | Header[46] << (8 * 0);
	_bid->bmpClrImportant	= Header[53] << (8 * 3) | Header[52] << (8 * 2) | Header[51] << (8 * 1) | Header[50] << (8 * 0);
}


long ImageRawSize( long bmpw, long bmph, short bmpbitcount )
{
	return(bmph * ( ( (bmpw * bmpbitcount + 31) / 32) * 4) );
}


void Save_Bmp( const char* filename, bmpfileheader* _bfd, bmpfileinfoheader* _bid, long ImageSize, unsigned char* ImageArea )
{
	FILE* fp = fopen( filename, "wb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fwrite( _bfd, 14, 1, fp );
	fwrite( _bid, 40, 1, fp );
	fwrite( ImageArea, ImageSize, 1, fp );
	fclose( fp );
}


void Read_Bmp( const char* filename, bmpfileheader* _bfd, bmpfileinfoheader* _bid, long ImageSize, unsigned char* ImageArea )
{
	FILE* fp = fopen( filename, "rb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	fread( _bfd, 14, 1, fp );
	fread( _bid, 40, 1, fp );
	fread( ImageArea, ImageSize, 1, fp );
	fclose( fp );
}


void GrayProcessing( unsigned char * ImageArea, long ImageSize )
{
	int i;
	for ( i = 0; i < ImageSize; i = i++ )
	{
		unsigned char gray;
		gray			= (ImageArea[i] + ImageArea[i + 1] + ImageArea[i + 2]) / 3;
		ImageArea[i]		= gray;
		ImageArea[i + 1]	= gray;
		ImageArea[i + 2]	= gray;
	}
}


void BinaryProcessing( unsigned char * ImageArea, long ImageSize, long threshold )
{
	int i;
	for ( i = 0; i < ImageSize; i = i++ )
	{
		if ( ImageArea[i] < threshold )
		{
			ImageArea[i] = 0x00;
		}else{
			ImageArea[i] = 0xff;
		}
	}
}


int main( int argc, char* argv[] )
{
	unsigned char	FileHeader[14];
	unsigned char	InfoHeader[40];
	unsigned char	Header[54];
	unsigned char	* ImageArea;
	long		ImageSize;
	const char	* filename = "num.bmp";
	Read_FileHeader( filename, FileHeader );
	Show_Byte( FileHeader, 14 );
	printf( "\n======================\n" );
	Read_InfoHeader( filename, InfoHeader );
	Show_Byte( InfoHeader, 40 );
	printf( "\n======================\n" );
	Read_Header( filename, Header );
	Show_Byte( Header, 54 );
	printf( "\n======================\n" );
	Load_bmpfile_struct( filename, &bfd );
	Show_Bmp_FileHeader( &bfd );
	Load_bmpinfo_struct( filename, &bid );
	Show_Bmp_InfoHeader( &bid );

	/* Calculate Image Area Size */
	ImageSize	= ImageRawSize( bid.bmpwidth, bid.bmpheight, bid.bmpbitcount );
	ImageArea	= (unsigned char *) malloc( ImageSize );
	Read_Bmp( "num.bmp", &bfd, &bid, ImageSize, ImageArea );
	/* deal with data */
	GrayProcessing( ImageArea, ImageSize );
	BinaryProcessing( ImageArea, ImageSize, 50 );
	Save_Bmp( "save2.bmp", &bfd, &bid, ImageSize, ImageArea );
	free( ImageArea );
	return(0);
}