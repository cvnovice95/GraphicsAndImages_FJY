#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(2)
#define U_BYTE		0x0001                  /* 1 */
#define ASCII		0x0002                  /* 1 */
#define U_SHORT		0x0003                  /* 2 */
#define U_LONG		0x0004                  /* 4 */
#define U_RATIONAL	0x0005                  /* 8   分子4 分母4 */
#define S_BYTE		0x0006                  /* 1 */
#define Undefined	0x0007                  /* 1 */
#define S_SHORT		0x0008                  /* 2 */
#define S_LONG		0x0009                  /* 4 */
#define S_RATIONAL	0x000A                  /* 8  分子4 分母4 */
#define SINGLE_FLOAT	0x000B                  /* 4 */
#define DOUBLE_FLOAT	0x000C                  /* 8 */
/* tiff tag struct */
typedef struct tiff_tag {
	unsigned short	tag;
	unsigned short	type;
	unsigned long	length;
	unsigned long	valueOroffset;
}_tiff_tag;
typedef struct tiff_header {
	unsigned short	Endian;
	unsigned short	version;
	unsigned long	FirstIFDoffset;
}_tiff_header;

typedef struct tiff_ifd {
	unsigned short	tag_num;
	_tiff_tag	TagNewSubfileType;
	_tiff_tag	TagImageWidth;
	_tiff_tag	TagImageHeight;
	_tiff_tag	TagBitsPerSample;
	_tiff_tag	TagCompression;
	_tiff_tag	TagPhotometricInterp;
	_tiff_tag	TagStripOffsets;
	_tiff_tag	TagSamplesPerPixel;
	_tiff_tag	TagRowsPerStrip;
	_tiff_tag	TagStripBytesCount;
	_tiff_tag	TagXResolution;
	_tiff_tag	TagYResolution;
	_tiff_tag	TagPlanarConfig;
	_tiff_tag	TagResolutionUnit;
	_tiff_tag	TagPredictor;
	unsigned long	next_ifd_offset;
}_tiff_ifd;

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
_tiff_header		g_tiff_header;
_tiff_ifd		g_tiff_ifd;
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


void Load_tiff_header()
{
	g_tiff_header.Endian		= 0x4949;
	g_tiff_header.version		= 0x002A;
	g_tiff_header.FirstIFDoffset	= 0x00000008;
}


void Load_tiff_tag()
{
	g_tiff_ifd.tag_num				= 0x000f;
	g_tiff_ifd.TagNewSubfileType.tag		= 0x00FE;
	g_tiff_ifd.TagNewSubfileType.type		= U_LONG;
	g_tiff_ifd.TagNewSubfileType.length		= 0x00000001;
	g_tiff_ifd.TagNewSubfileType.valueOroffset	= 0x00000000;
	/* Image Width(pix) */
	g_tiff_ifd.TagImageWidth.tag		= 0x0100;
	g_tiff_ifd.TagImageWidth.type		= U_SHORT;
	g_tiff_ifd.TagImageWidth.length		= 0x00000001;
	g_tiff_ifd.TagImageWidth.valueOroffset	= 0;
	/* Image Height(pix) */
	g_tiff_ifd.TagImageHeight.tag		= 0x0101;
	g_tiff_ifd.TagImageHeight.type		= U_SHORT;
	g_tiff_ifd.TagImageHeight.length	= 0x00000001;
	g_tiff_ifd.TagImageHeight.valueOroffset = 0;

	g_tiff_ifd.TagBitsPerSample.tag			= 0x0102;
	g_tiff_ifd.TagBitsPerSample.type		= U_SHORT;
	g_tiff_ifd.TagBitsPerSample.length		= 0x00000003;
	g_tiff_ifd.TagBitsPerSample.valueOroffset	= 0; /* offset 0x0008 */

	g_tiff_ifd.TagCompression.tag		= 0x0103;
	g_tiff_ifd.TagCompression.type		= U_SHORT;
	g_tiff_ifd.TagCompression.length	= 0x00000001;
	g_tiff_ifd.TagCompression.valueOroffset = 0x00000001;

	g_tiff_ifd.TagPhotometricInterp.tag		= 0x0106;
	g_tiff_ifd.TagPhotometricInterp.type		= U_SHORT;
	g_tiff_ifd.TagPhotometricInterp.length		= 0x00000001;
	g_tiff_ifd.TagPhotometricInterp.valueOroffset	= 0x00000002;

	g_tiff_ifd.TagStripOffsets.tag			= 0x0111;
	g_tiff_ifd.TagStripOffsets.type			= U_LONG;
	g_tiff_ifd.TagStripOffsets.length		= 0x00000001;
	g_tiff_ifd.TagStripOffsets.valueOroffset	= 0x00000008;

	g_tiff_ifd.TagSamplesPerPixel.tag		= 0x0115;
	g_tiff_ifd.TagSamplesPerPixel.type		= U_SHORT;
	g_tiff_ifd.TagSamplesPerPixel.length		= 0x00000001;
	g_tiff_ifd.TagSamplesPerPixel.valueOroffset	= 0x00000003;

	g_tiff_ifd.TagRowsPerStrip.tag			= 0x0116;
	g_tiff_ifd.TagRowsPerStrip.type			= U_LONG;
	g_tiff_ifd.TagRowsPerStrip.length		= 0x00000001;
	g_tiff_ifd.TagRowsPerStrip.valueOroffset	= 135;

	g_tiff_ifd.TagStripBytesCount.tag		= 0x0117;
	g_tiff_ifd.TagStripBytesCount.type		= U_LONG;
	g_tiff_ifd.TagStripBytesCount.length		= 0x00000001;
	g_tiff_ifd.TagStripBytesCount.valueOroffset	= 0;    /* 0x00000140 ; */

	g_tiff_ifd.TagXResolution.tag		= 0x011A;
	g_tiff_ifd.TagXResolution.type		= U_RATIONAL;
	g_tiff_ifd.TagXResolution.length	= 0x00000001;
	g_tiff_ifd.TagXResolution.valueOroffset = 0;            /* offset */

	g_tiff_ifd.TagYResolution.tag		= 0x011B;
	g_tiff_ifd.TagYResolution.type		= U_RATIONAL;
	g_tiff_ifd.TagYResolution.length	= 0x00000001;
	g_tiff_ifd.TagYResolution.valueOroffset = 0;            /* offset */

	g_tiff_ifd.TagPlanarConfig.tag			= 0x011C;
	g_tiff_ifd.TagPlanarConfig.type			= U_SHORT;
	g_tiff_ifd.TagPlanarConfig.length		= 0x00000001;
	g_tiff_ifd.TagPlanarConfig.valueOroffset	= 0x00000001;

	g_tiff_ifd.TagResolutionUnit.tag		= 0x128;
	g_tiff_ifd.TagResolutionUnit.type		= U_SHORT;
	g_tiff_ifd.TagResolutionUnit.length		= 0x00000001;
	g_tiff_ifd.TagResolutionUnit.valueOroffset	= 0x00000002;

	g_tiff_ifd.TagPredictor.tag		= 0x013D;
	g_tiff_ifd.TagPredictor.type		= U_SHORT;
	g_tiff_ifd.TagPredictor.length		= 0x00000001;
	g_tiff_ifd.TagPredictor.valueOroffset	= 0x00000001;

	g_tiff_ifd.next_ifd_offset = 0x00000000;
}


void save_tiff( const char* filename, long BMPImageSize, unsigned char* BMPImageArea, _tiff_header* theader, _tiff_ifd* tifd )
{
	unsigned long	XResolution_num = 96;
	unsigned long	XResolution_den = 1;
	unsigned long	YResolution_num = 96;
	unsigned long	YResolution_den = 1;
	unsigned short	BitsPerSample	= 8;
	/* unsigned short tag_num = 0x000f; */
	FILE* fp = fopen( filename, "wb+" );
	if ( fp == NULL )
	{
		printf( "Open File Failed!" );
		exit( 0 );
	}
	theader->FirstIFDoffset = 8 + BMPImageSize + 16 + 6;
	fwrite( theader, 8, 1, fp );
	fwrite( BMPImageArea, BMPImageSize, 1, fp );
	fwrite( &XResolution_num, 4, 1, fp );
	fwrite( &XResolution_den, 4, 1, fp );
	fwrite( &YResolution_num, 4, 1, fp );
	fwrite( &YResolution_den, 4, 1, fp );
	fwrite( &BitsPerSample, 2, 1, fp );
	fwrite( &BitsPerSample, 2, 1, fp );
	fwrite( &BitsPerSample, 2, 1, fp );
	/* fwrite(&tag_num,2,1,fp); */
	tifd->TagXResolution.valueOroffset	= 8 + BMPImageSize;
	tifd->TagYResolution.valueOroffset	= 8 + BMPImageSize + 8;
	tifd->TagBitsPerSample.valueOroffset	= 8 + BMPImageSize + 8 + 8;
	fwrite( tifd, 1, 186, fp );
	fclose( fp );
}


void ReverseBMP( long BMPImageSize, unsigned char* BMPImageArea )
{
	unsigned char	Temp[BMPImageSize];
	int		i;
	for ( i = 0; i < BMPImageSize; i++ )
	{
		Temp[i] = BMPImageArea[i];
	}
	for ( i = 0; i < BMPImageSize; i++ )
	{
		BMPImageArea[i] = Temp[BMPImageSize - 1 - i];
	}
}


void TwoDimension( long bmpw, long bmph, short bmpbitcount, unsigned char * twod[] )
{
	int i;
	for ( i = 0; i < bmph; i++ )
	{
		twod[i] = (unsigned char *) malloc( ( ( (bmpw * bmpbitcount + 31) / 32) * 4) );
	}
}


void OneToTwo( unsigned char * ImageArea, unsigned char * twod[], long bmph, long ImageSize )
{
	int i, j, k = 0;
	for ( i = 0; i < bmph; i++ )
	{
		for ( j = 0; j < (ImageSize / bmph); j++, k++ )
		{
			twod[i][j] = ImageArea[k];
		}
	}
}


void TwoToOne( unsigned char * ImageArea, unsigned char * twod[], long bmph, long ImageSize )
{
	int i, j, k = 0;
	for ( i = 0; i < bmph; i++ )
	{
		for ( j = 0; j < (ImageSize / bmph); j++, k++ )
		{
			ImageArea[k] = twod[i][j];
		}
	}
}


void ReverseBMP2( unsigned char * ImageArea, unsigned char * twod[], long bmph, long ImageSize )
{
	unsigned	Temp[bmph][ImageSize / bmph];
	int		i, j;
	for ( i = 0; i < bmph; i++ )
	{
		for ( j = 0; j < (ImageSize / bmph); j++ )
		{
			Temp[i][j] = twod[i][j];
			/* twod[i][j] = ImageArea[k]; */
		}
	}
	for ( i = 0; i < bmph; i++ )
	{
		for ( j = 0; j < (ImageSize / bmph); j++ )
		{
			twod[i][j] = Temp[bmph - 1 - i][j];
			/* twod[i][j] = ImageArea[k]; */
		}
	}
}


void freetd( long size, unsigned char * twod[] )
{
	int i;
	for ( i = 0; i < size; i++ )
	{
		free( twod[i] );
		twod[i] = NULL;
	}
}


int main( int argc, char* argv[] )
{
	unsigned char	FileHeader[14];
	unsigned char	InfoHeader[40];
	unsigned char	Header[54];
	unsigned char	* ImageArea = NULL;

	long		ImageSize;
	const char	* filename	= "test.bmp";
	const char	* savefile	= "bmtotif.tif";
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
	unsigned char* td[bid.bmpheight];
	TwoDimension( bid.bmpwidth, bid.bmpheight, bid.bmpbitcount, td );
	ImageSize	= ImageRawSize( bid.bmpwidth, bid.bmpheight, bid.bmpbitcount );
	ImageArea	= (unsigned char *) malloc( ImageSize * sizeof(unsigned char) );
	Read_Bmp( "test.bmp", &bfd, &bid, ImageSize, ImageArea );
	OneToTwo( ImageArea, td, bid.bmpheight, ImageSize );

	/*
	 * deal with data
	 * Save_Bmp("saveb.bmp",&bfd,&bid,ImageSize,ImageArea);
	 * save tiff
	 */
	Load_tiff_header();
	Load_tiff_tag();
	g_tiff_ifd.TagImageWidth.valueOroffset		= bid.bmpwidth;
	g_tiff_ifd.TagImageHeight.valueOroffset		= bid.bmpheight;
	g_tiff_ifd.TagStripBytesCount.valueOroffset	= ImageSize;
	/* ReverseBMP(ImageSize,ImageArea); */
	ReverseBMP2( ImageArea, td, bid.bmpheight, ImageSize );
	TwoToOne( ImageArea, td, bid.bmpheight, ImageSize );
	save_tiff( savefile, ImageSize, ImageArea, &g_tiff_header, &g_tiff_ifd );
	free( ImageArea );
	ImageArea = NULL;
	freetd( bid.bmpheight, td );
	return(0);
}