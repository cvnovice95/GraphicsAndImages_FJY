#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
 * FFD8  SOI
 * FFDA  SOS
 * FFD9  EOI
 * offset +12 (<-4949)
 * Counyt GPS　TAGS    2 bytes
 * 0000  GPS VersionID  12 bytes
 * 0001  GPS Latref     12 bytes
 * 0002  GPS Lat        12 bytes
 * 0003  GPS Lonref     12 bytes
 * 0004  GPs Lon        12 bytes
 * GPS INFO
 */
#define TagGpsVer       0x0000  /* GPS版本号 */
#define TagGpsLatitudeRef   0x0001  /* GPS纬度参照（N-北，S-南） */
#define TagGpsLatitude      0x0002  /* GPS纬度（度/x,分/x,秒/x） */
#define TagGpsLongitudeRef  0x0003  /* GPS经度参照（E-东，W-西） */
#define TagGpsLongitude     0x0004  /* GPS经度（度/x,分/x,秒/x） */
#define TagGpsAltitudeRef   0x0005  /* GPS高度参照（0-海拔高度，1-绝对高度） */
#define TagGpsAltitude      0x0006  /* GPS高度 */
#define TagGpsGpsTime       0x0007  /* GPS时间戳 */
#define TagGpsGpsSatellites 0x0008  /* GPS卫星 */
#define TagGpsGpsStatus     0x0009  /* GPS状态（A-实际值，V-插值估算） */
#define TagGpsGpsMeasureMode    0x000a  /* GPS测量方式（2-二维，3-三维） */
/* IFD0 INFO */
#define TagImageDescription 0x010e  /* 图像标题 */
#define TagEquipMake        0x010f  /* 成像设备厂商名 */
#define TagEquipModel       0x0110  /* 成像设备型号 */
#define TagOrientation      0x0112  /* 图像方向 */
#define TagXResolution      0x011a  /* 图像宽方向每分辨率单位 */
#define TagYResolution      0x011b  /* 图像高方向每分辨率单位 */
#define TagResolutionUnit   0x0128  /* 分辨率单位 */
#define TagSoftwareUsed     0x0131  /* 显示固件的版本号 成像设备的软件或硬件名称和版本 */
#define TagDateTime     0x0132  /* 成像时间 */
#define TagArtist       0x013B  /* 作者（相机主、照相者或图像创作者） */
#define TagHostComputer     0x013C  /* 生成图像的计算机 */
#define TagWhitePoint       0x013e  /* 白点色品 */
#define TagPrimaryChromat   0x013f  /* 主色调 */
#define TagYCbCrCoefficients    0x0211  /* YCbCr相关系数 */
#define TagYCbCrSubsampling 0x0212  /* YCbCr压抽样率 */
#define TagYCbCrPositioning 0x0213  /* YCbCr位置 */
#define TagREFBlackWhite    0x0214  /* 参考黑白点 */
#define TagCopyright        0x8298  /* 表示版权信息 */
#define TagExifIFD      0x8769  /* Exif IFD 偏移量 */
#define TagGPSInfo      0x8825  /* GPS Info 偏移量 */
/* Exif IFD */
#define TagExifExposureTime 0x829a  /* 曝光时间 (快门速度的倒数). 单位是秒. */
#define TagExifFNumber      0x829D  /* 拍照时的光圈F-number(F-stop). */
#define TagExifExposureProg 0x8822  /* 拍照时相机使用的曝光程序. '1' 表示手动曝光, '2' 表示正常程序曝光, '3' 表示光圈优先曝光, '4' 表示快门优先曝光, '5' 表示创意程序(慢速程序), '6' 表示动作程序(高速程序), '7'表示 肖像模式, '8' 表示风景模式. */
#define TagExifISOSpeed     0x8827  /* CCD 的感光度, 等效于 Ag-Hr 胶片的速率. */
#define TagExifVer      0x9000  /* Exif 的版本号 */
#define TagExifDTOrig       0x9003  /* 照片在被拍下来的日期/时间 */
#define TagExifDTDigitized  0x9004  /* 照片被数字化时的日期/时间. 通常, 它与DateTimeOriginal(0x9003)具有相同的值 */
#define TagExifCompConfig   0x9101  /* 表示的是像素数据的顺序. 大多数情况下RGB格式使用 '0x04,0x05,0x06,0x00' */
#define TagExifCompBPP      0x9102  /* JPEG (粗略的估计)的平均压缩率. */
#define TagExifShutterSpeed 0x9201  /* 用APEX表示出的快门速度. */
#define TagExifAperture     0x9202  /* 拍照时镜头的光圈. 单位是 APEX. */
#define TagExifBrightness   0x9203  /* 被拍摄对象的明度, 单位是 APEX. */
#define TagExifExposureBias 0x9204  /* 照片拍摄时的曝光补偿. 单位是APEX(EV). */
#define TagExifMaxAperture  0x9205  /* 镜头的最大光圈值. */
#define TagExifSubjectDis   0x9206  /* 到焦点的距离, 单位是米. */
#define TagExifMeteringMode 0x9207  /* 曝光的测光方法. '0' 表示未知, '1' 为平均测光, '2' 为中央重点测光, '3' 是点测光, '4' 是多点测光, '5' 是多区域测光, '6' 部分测光, '255' 则是其他. */
#define TagExifLightSource  0x9208  /* 光源, 实际上是表示白平衡设置. '0' 意味着未知, '1'是日光, '2'是荧光灯, '3' 白炽灯(钨丝), '10' 闪光灯, '17' 标准光A, '18' 标准光B, '19' 标准光C, '20' D55, '21' D65, '22' D75, '255' 为其他. */
#define TagExifFlash        0x9209  /* '0' 表示闪光灯没有闪光, '1' 表示闪光灯闪光, '5' 表示闪光但没有检测反射光, '7' 表示闪光且检测了反射光. */
#define TagExifFocalLength  0x920A  /* 拍摄照片时的镜头的焦距长度. 单位是毫米. */
#define TagExifMakerNote    0x927C  /* 制造商的内部数据 */
#define TagExifUserComment  0x9286  /* 存储用户的注释. */
#define TagExifFPXVer       0xA000  /* 存储FlashPix 的版本信息 */
#define TagExifColorSpace   0xA001  /* 定义色彩空间. DCF 图像必须使用 sRGB 色彩空间因此这个值总是 '1'. 如果这个照片使用了 其他的色彩空间, 这个值是 '65535':未校准(Uncalibrated). */
#define TagExifPixXDim      0xA002  /* 主图像的宽 */
#define TagExifPixYDim      0xA003  /* 主图像的高 */
/* 缩略图 */
#define TagImageWidth       0x0100  /* 图像宽 */
#define TagImageHeight      0x0101  /* 图像高 */
#define TagCompression      0x0103  /* 压缩方式  6--jpeg 1--缩略图不压缩 数据偏移0x0111 数据大小0x0117 */
#define TagPhotometricInterp    0x0106  /* 光测度 2--无压缩缩略图格式RGB 6--无压缩缩略图YCbCr 格式 */
#define TagStripOffsets     0x0111  /* tiff   无压缩缩略图偏移量 */
#define TagSamplesPerPixel  0x0115  /* 每图像点的成份数 */
#define TagStripBytesCount  0x0117  /* tiff   无压缩缩略图偏大小 */
#define TagJPEGInterFormat  0x0201  /* jpeg  缩略图偏移量  jpeg缩略图FFD8-FFD9 */
#define TagJPEGInterLength  0x0202  /* jpeg  缩略图大小 */
#define TagPlanarConfig     0x011c  /* 图像点成份记录方式（面planar记录或块chunky记录） */


/*
 * 游标指向待读
 * DATA FORMAT
 */
#define U_BYTE      0x01            /* 1 */
#define ASCII       0x02            /* 1 */
#define U_SHORT     0x03            /* 2 */
#define U_LONG      0x04            /* 4 */
#define U_RATIONAL  0x05            /* 8   分子4 分母4 */
#define S_BYTE      0x06            /* 1 */
#define Undefined   0x07            /* 1 */
#define S_SHORT     0x08            /* 2 */
#define S_LONG      0x09            /* 4 */
#define S_RATIONAL  0x0A            /* 8  分子4 分母4 */
#define SINGLE_FLOAT    0x0B            /* 4 */
#define DOUBLE_FLOAT    0x0C            /* 8 */
/* JPEG TAG */
#define SOI     0xFFD8
#define SOS     0xFFDA
#define EOI     0xFFD9
#define JFIF_APP0   0xFFE0
#define EXIF_APP1   0xFFE1
/* global offset */
#define OFFSET 0x000C
/* #define DEBUG */
int     DataFormatMapTable[12] = { 1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8 };
unsigned char   soi[2];
unsigned char   app[2];
unsigned char   app_size[2];
char        exif_header[6];
unsigned char   tiff_header[8];
unsigned char   ifd0_tag_count[2];
typedef struct _tag_block {
    unsigned char   tag[2];
    short       dataformat;
    int     itemcount;
    unsigned char   valueOroffset[4];
}tag_block;
typedef struct edit_tag {
    unsigned short  tag;
    short       dataformat;
    int     countitem;
    int     offset;
}_edit_tag;
struct Lat_Lon {
    int degree;
    int unitd;
    int minute;
    int unitm;
    int second;
    int units;
};
int big_or_little;
void ShowDirInfo( unsigned char apparea[], int startpos );


int DataFormatMapBytes( int DataFormat )
{
    if ( DataFormat < 1 || DataFormat > 12 )
    {
        printf( "parameter is error\n" );
    }else{
        return(DataFormatMapTable[DataFormat - 1]);
    }
}


void show_bytes( char *cr, int size )
{
    int i;
    for ( i = 0; i < size; i++ )
    {
        printf( "%x  ", cr[i] );
    }
    printf( "\n" );
}


void show_bytes_u( unsigned char *cr, int size )
{
    int i;
    for ( i = 0; i < size; i++ )
    {
        printf( "%x  ", cr[i] );
    }
    printf( "\n" );
}


int show_tiff_header( unsigned char *cr )
{
    if ( cr[0] == 0x49 && cr[1] == 0x49 )
    {
        big_or_little = 0;
        printf( "[show_tiff_header]Little Endian\n" );
        printf( "[show_tiff_header]TIFF TAG is %x %x\n", cr[3], cr[2] );
        printf( "[show_tiff_header]IFDO OFFSET is %d\n", (int) (cr[7] << 3 * 8 | cr[6] << 2 * 8 | cr[5] << 1 * 8 | cr[4] << 0 * 8) );
        return(0);
    }
    if ( cr[0] == 0x4D && cr[1] == 0x4D )
    {
        big_or_little = 1;
        printf( "[show_tiff_header]BIG Endian\n" );
        printf( "[show_tiff_header]TIFF TAG is %x %x\n", cr[2], cr[3] );
        printf( "[show_tiff_header]IFDO OFFSET is %d\n", (int) (cr[4] << 3 * 8 | cr[5] << 2 * 8 | cr[6] << 1 * 8 | cr[7] << 0 * 8) );
        return(1);
    }
}


void show_tag_block( tag_block* tbb, int size )
{
    printf( "[show_tag_block]show tag info \n" );
    int i;
    for ( i = 0; i < size; i++ )
    {
        if ( big_or_little == 0 )
        {
            printf( "[show_tag_block]item is %d tag is%x %x \n", i, tbb[i].tag[1], tbb[i].tag[0] );
        }else{
            printf( "[show_tag_block]item is %d tag is%x %x \n", i, tbb[i].tag[0], tbb[i].tag[1] );
        }
        printf( "[show_tag_block]item is %d dataformat is %d\n ", i, tbb[i].dataformat );
        printf( "[show_tag_block]item is %d itemcount is %d\n", i, tbb[i].itemcount );
        printf( "[show_tag_block]item is %d valueOroffset raw is %x %x %x %x\n", i, tbb[i].valueOroffset[0], tbb[i].valueOroffset[1], tbb[i].valueOroffset[2], tbb[i].valueOroffset[3] );
        if ( big_or_little == 0 )
        {
            printf( "[show_tag_block]item is %d valueOroffset is %d\n", i, (int) (tbb[i].valueOroffset[3] << 3 * 8 |
                                                  tbb[i].valueOroffset[2] << 2 * 8 |
                                                  tbb[i].valueOroffset[1] << 1 * 8 |
                                                  tbb[i].valueOroffset[0] << 0 * 8) );
        }else{
            printf( "[show_tag_block]item is %d valueOroffset is %d\n", i, (int) (tbb[i].valueOroffset[0] << 3 * 8 |
                                                  tbb[i].valueOroffset[1] << 2 * 8 |
                                                  tbb[i].valueOroffset[2] << 1 * 8 |
                                                  tbb[i].valueOroffset[3] << 0 * 8) );
        }
    }
    printf( "[show_tag_block]show tag info end \n" );
}


int FirstSection( const char *filename )
{
    FILE *fp = fopen( filename, "rb+" );
    if ( fp == NULL )
    {
        printf( "OPEN FILE FAILED!\n" );
        fclose( fp );
        return(-1);
    }else{
        fread( soi, 2, 1, fp );
        fread( app, 2, 1, fp );
        fread( app_size, 2, 1, fp );
        fread( exif_header, 6, 1, fp );
        fread( tiff_header, 8, 1, fp );
        fclose( fp );
        printf( "[FirstSection]--First Section Info Start--\n" );
        printf( "[FirstSection]soi raw is:" );
        show_bytes_u( soi, 2 );
        printf( "[FirstSection]app raw is:" );
        show_bytes_u( app, 2 );
        printf( "[FirstSection]app size raw is:" );
        show_bytes_u( app_size, 2 );
        printf( "[FirstSection]app size is :%d\n", (short) (app_size[0] << 8 | app_size[1]) );
        printf( "[FirstSection]exif_header raw is:" );
        show_bytes( exif_header, 6 );
        printf( "[FirstSection]exif_header info  is:%s\n", exif_header );
        printf( "[FirstSection]tiff_header raw is:" );
        show_bytes_u( tiff_header, 8 );
        printf( "[FirstSection]tiff_header info is :\n" );
        show_tiff_header( tiff_header );
        printf( "[FirstSection]--First Section Info End--\n" );
        return( (short) ( (app_size[0] << 8 | app_size[1]) + 4) );
    }
}


void DealWithTag( tag_block* tb, unsigned char apparea[] )
{
    unsigned char   _Bytes[4];
    short       dataformat;
    int     itemc;
    int     totalbytes;
    dataformat  = tb->dataformat;
    itemc       = tb->itemcount;
    totalbytes  = itemc * DataFormatMapBytes( dataformat );
    _Bytes[0]   = tb->valueOroffset[0];
    _Bytes[1]   = tb->valueOroffset[1];
    _Bytes[2]   = tb->valueOroffset[2];
    _Bytes[3]   = tb->valueOroffset[3];
    int offset;
    if ( big_or_little == 0 )
    {
        offset = (int) (_Bytes[3] << 3 * 8 |
                _Bytes[2] << 2 * 8 |
                _Bytes[1] << 1 * 8 |
                _Bytes[0] << 0 * 8) + 12;
    }else{
        offset = (int) (_Bytes[0] << 3 * 8 |
                _Bytes[1] << 2 * 8 |
                _Bytes[2] << 1 * 8 |
                _Bytes[3] << 0 * 8) + 12;
    }
    switch ( dataformat )
    {
    case U_BYTE:
        if ( totalbytes <= 4 )
        {
            show_bytes_u( tb->valueOroffset, totalbytes );
        }else{
            show_bytes_u( &apparea[offset], totalbytes );
        }
        break;
    case ASCII:
        if ( totalbytes <= 4 )
        {
            printf( "%s \n", tb->valueOroffset );
        }else{
            printf( "%s \n", &apparea[offset] );
        }
        break;
    case U_SHORT:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%d \n", (unsigned short) (_Bytes[3] << 3 * 8 |
                                   _Bytes[2] << 2 * 8 |
                                   _Bytes[1] << 1 * 8 |
                                   _Bytes[0] << 0 * 8) );
            }else{
                printf( "%d \n", (unsigned short) (_Bytes[0] << 3 * 8 |
                                   _Bytes[1] << 2 * 8 |
                                   _Bytes[2] << 1 * 8 |
                                   _Bytes[3] << 0 * 8) );
            }
        }else{
            int c;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    printf( "%d  ", (unsigned short) (apparea[offset + 1 + c * 2] << 8 | apparea[offset + 0 + c * 2]) );
                }else{
                    printf( "%d  ", (unsigned short) (apparea[offset + 0 + c * 2] << 8 | apparea[offset + 1 + c * 2]) );
                }
            }
            printf( "\n" );
        }
        break;
    case U_LONG:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%d \n", (unsigned long) (_Bytes[3] << 3 * 8 |
                                  _Bytes[2] << 2 * 8 |
                                  _Bytes[1] << 1 * 8 |
                                  _Bytes[0] << 0 * 8) );
            }else{
                printf( "%d \n", (unsigned long) (_Bytes[0] << 3 * 8 |
                                  _Bytes[1] << 2 * 8 |
                                  _Bytes[2] << 1 * 8 |
                                  _Bytes[3] << 0 * 8) );
            }
        }else{
            int c;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    printf( "%d  ", (unsigned long) (apparea[offset + 3 + c * 4] << 3 * 8 |
                                     apparea[offset + 2 + c * 4] << 2 * 8 |
                                     apparea[offset + 1 + c * 4] << 1 * 8 |
                                     apparea[offset + 0 + c * 4] << 0 * 8) );
                }else{
                    printf( "%d  ", (unsigned long) (apparea[offset + 0 + c * 4] << 3 * 8 |
                                     apparea[offset + 1 + c * 4] << 2 * 8 |
                                     apparea[offset + 2 + c * 4] << 1 * 8 |
                                     apparea[offset + 3 + c * 4] << 0 * 8) );
                }
            }
            printf( "\n" );
        }
        break;
    case U_RATIONAL:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%d \n", (unsigned long) (_Bytes[3] << 3 * 8 |
                                  _Bytes[2] << 2 * 8 |
                                  _Bytes[1] << 1 * 8 |
                                  _Bytes[0] << 0 * 8) );
            }else{
                printf( "%d \n", (unsigned long) (_Bytes[0] << 3 * 8 |
                                  _Bytes[1] << 2 * 8 |
                                  _Bytes[2] << 1 * 8 |
                                  _Bytes[3] << 0 * 8) );
            }
        }else{
            int     c;
            unsigned long   A, B;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    A = (unsigned long) (apparea[offset + 3 + c * 8] << 3 * 8 |
                                 apparea[offset + 2 + c * 8] << 2 * 8 |
                                 apparea[offset + 1 + c * 8] << 1 * 8 |
                                 apparea[offset + 0 + c * 8] << 0 * 8);
                    B = (unsigned long) (apparea[offset + 7 + c * 8] << 3 * 8 |
                                 apparea[offset + 6 + c * 8] << 2 * 8 |
                                 apparea[offset + 5 + c * 8] << 1 * 8 |
                                 apparea[offset + 4 + c * 8] << 0 * 8);
                }else{
                    A = (unsigned long) (apparea[offset + 0 + c * 8] << 3 * 8 |
                                 apparea[offset + 1 + c * 8] << 2 * 8 |
                                 apparea[offset + 2 + c * 8] << 1 * 8 |
                                 apparea[offset + 3 + c * 8] << 0 * 8);
                    B = (unsigned long) (apparea[offset + 4 + c * 8] << 3 * 8 |
                                 apparea[offset + 5 + c * 8] << 2 * 8 |
                                 apparea[offset + 6 + c * 8] << 1 * 8 |
                                 apparea[offset + 7 + c * 8] << 0 * 8);
                }
                printf( "%lf ", (float) A / B );
            }
            printf( "\n" );
        }
        break;
    case S_BYTE:
        if ( totalbytes <= 4 )
        {
            show_bytes_u( tb->valueOroffset, totalbytes );
        }else{
            show_bytes_u( &apparea[offset], totalbytes );
        }
        break;
    case Undefined:
        if ( totalbytes <= 4 )
        {
            show_bytes_u( tb->valueOroffset, totalbytes );
        }else{
            show_bytes_u( &apparea[offset], totalbytes );
        }
        break;
    case S_SHORT:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%d \n", (short) (_Bytes[3] << 3 * 8 |
                              _Bytes[2] << 2 * 8 |
                              _Bytes[1] << 1 * 8 |
                              _Bytes[0] << 0 * 8) );
            }else{
                printf( "%d \n", (short) (_Bytes[0] << 3 * 8 |
                              _Bytes[1] << 2 * 8 |
                              _Bytes[2] << 1 * 8 |
                              _Bytes[3] << 0 * 8) );
            }
        }else{
            int c;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    printf( "%d  ", (short) (apparea[offset + 1 + c * 2] << 8 | apparea[offset + 0 + c * 2]) );
                }else{
                    printf( "%d  ", (short) (apparea[offset + 0 + c * 2] << 8 | apparea[offset + 1 + c * 2]) );
                }
            }
            printf( "\n" );
        }
        break;
    case S_LONG:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%d \n", (long) (_Bytes[3] << 3 * 8 |
                             _Bytes[2] << 2 * 8 |
                             _Bytes[1] << 1 * 8 |
                             _Bytes[0] << 0 * 8) );
            }else{
                printf( "%d \n", (long) (_Bytes[0] << 3 * 8 |
                             _Bytes[1] << 2 * 8 |
                             _Bytes[2] << 1 * 8 |
                             _Bytes[3] << 0 * 8) );
            }
        }else{
            int c;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    printf( "%d  ", (long) (apparea[offset + 3 + c * 4] << 3 * 8 |
                                apparea[offset + 2 + c * 4] << 2 * 8 |
                                apparea[offset + 1 + c * 4] << 1 * 8 |
                                apparea[offset + 0 + c * 4] << 0 * 8) );
                }else{
                    printf( "%d  ", (long) (apparea[offset + 0 + c * 4] << 3 * 8 |
                                apparea[offset + 1 + c * 4] << 2 * 8 |
                                apparea[offset + 2 + c * 4] << 1 * 8 |
                                apparea[offset + 3 + c * 4] << 0 * 8) );
                }
            }
            printf( "\n" );
        }
        break;
    case S_RATIONAL:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%d \n", (long) (_Bytes[3] << 3 * 8 | _Bytes[2] << 2 * 8 | _Bytes[1] << 1 * 8 | _Bytes[0] << 0 * 8) );
            }else{
                printf( "%d \n", (long) (_Bytes[0] << 3 * 8 | _Bytes[1] << 2 * 8 | _Bytes[2] << 1 * 8 | _Bytes[3] << 0 * 8) );
            }
        }else{
            int     c;
            unsigned long   A, B;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    A = (long) (apparea[offset + 3 + c * 8] << 3 * 8 |
                            apparea[offset + 2 + c * 8] << 2 * 8 |
                            apparea[offset + 1 + c * 8] << 1 * 8 |
                            apparea[offset + 0 + c * 8] << 0 * 8);
                    B = (long) (apparea[offset + 7 + c * 8] << 3 * 8 |
                            apparea[offset + 6 + c * 8] << 2 * 8 |
                            apparea[offset + 5 + c * 8] << 1 * 8 |
                            apparea[offset + 4 + c * 8] << 0 * 8);
                }else{
                    A = (long) (apparea[offset + 0 + c * 8] << 3 * 8 |
                            apparea[offset + 1 + c * 8] << 2 * 8 |
                            apparea[offset + 2 + c * 8] << 1 * 8 |
                            apparea[offset + 3 + c * 8] << 0 * 8);
                    B = (long) (apparea[offset + 4 + c * 8] << 3 * 8 |
                            apparea[offset + 5 + c * 8] << 2 * 8 |
                            apparea[offset + 6 + c * 8] << 1 * 8 |
                            apparea[offset + 7 + c * 8] << 0 * 8);
                }
                printf( "%lf ", (float) A / B );
            }
            printf( "\n" );
        }
        break;
    case SINGLE_FLOAT:
        if ( totalbytes <= 4 )
        {
            if ( big_or_little == 0 )
            {
                printf( "%f \n", (float) (_Bytes[3] << 3 * 8 | _Bytes[2] << 2 * 8 | _Bytes[1] << 1 * 8 | _Bytes[0] << 0 * 8) );
            }else{
                printf( "%f \n", (float) (_Bytes[0] << 3 * 8 | _Bytes[1] << 2 * 8 | _Bytes[2] << 1 * 8 | _Bytes[3] << 0 * 8) );
            }
        }else{
            int c;
            for ( c = 0; c < itemc; c++ )
            {
                if ( big_or_little == 0 )
                {
                    printf( "%f  ", (float) (apparea[offset + 3 + c * 4] << 3 * 8 |
                                 apparea[offset + 2 + c * 4] << 2 * 8 |
                                 apparea[offset + 1 + c * 4] << 1 * 8 |
                                 apparea[offset + 0 + c * 4] << 0 * 8) );
                }else{
                    printf( "%f  ", (float) (apparea[offset + 0 + c * 4] << 3 * 8 |
                                 apparea[offset + 1 + c * 4] << 2 * 8 |
                                 apparea[offset + 2 + c * 4] << 1 * 8 |
                                 apparea[offset + 3 + c * 4] << 0 * 8) );
                }
            }
            printf( "\n" );
        }
        break;
    case DOUBLE_FLOAT:


        /*
         * if(totalbytes<=4){
         *   printf("%d \n",(float)(_Bytes[3]<<3*8|_Bytes[2]<<2*8|_Bytes[1]<<1*8|_Bytes[0]<<0*8));
         * }else{
         *   offset =(int)(_Bytes[3]<<3*8|_Bytes[2]<<2*8|_Bytes[1]<<1*8|_Bytes[0]<<0*8)+12;
         *   int c;
         *   double result;
         *   for(c=0;c<itemc;c++){
         *       result = (double)(apparea[offset+7+c*8]<<(7*8)|
         *                         apparea[offset+6+c*8]<<6*8|
         *                         apparea[offset+5+c*8]<<5*8|
         *                         apparea[offset+4+c*8]<<4*8|
         *                         apparea[offset+3+c*8]<<3*8|
         *                         apparea[offset+2+c*8]<<2*8|
         *                         apparea[offset+1+c*8]<<1*8|
         *                         apparea[offset+0+c*8]<<0*8);
         *       printf("%f \n",result);
         *   }
         *   printf("\n");
         * }
         */
        break;
    default:
        break;
    }
}


void AnalysisTag( tag_block* tb, int tbsize, unsigned char apparea[] )
{
    int     i, offset;
    unsigned short  tag;
    for ( i = 0; i < tbsize; i++ )
    {
        if ( big_or_little == 0 )
        {
            tag = tb[i].tag[1] << 8 | tb[i].tag[0];
            offset  = (int) (tb[i].valueOroffset[3] << 3 * 8 |
                     tb[i].valueOroffset[2] << 2 * 8 |
                     tb[i].valueOroffset[1] << 1 * 8 |
                     tb[i].valueOroffset[0] << 0 * 8);
        }else{
            tag = tb[i].tag[0] << 8 | tb[i].tag[1];
            offset  = (int) (tb[i].valueOroffset[0] << 3 * 8 |
                     tb[i].valueOroffset[1] << 2 * 8 |
                     tb[i].valueOroffset[2] << 1 * 8 |
                     tb[i].valueOroffset[3] << 0 * 8);
        }
        switch ( tag )
        {
        case TagImageDescription:
            printf( "Image Title:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagEquipMake:
            printf( "Imaging device manufacturer name:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagEquipModel:
            printf( "Imaging device model:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagOrientation:
            printf( "Orientation:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagXResolution:
            printf( "XResolution:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagYResolution:
            printf( "YResolution:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagResolutionUnit:
            printf( "ResolutionUnit:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagSoftwareUsed:
            printf( "SoftwareUsed:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagDateTime:
            printf( "DateTime:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagArtist:
            printf( "Artist:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagYCbCrPositioning:
            printf( "YCbCrPositioning:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagCopyright:
            printf( "Copyright:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifExposureTime:
            printf( "ExifExposureTime:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifFNumber:
            printf( "ExifFNumber:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifExposureProg:
            printf( "ExifExposureProg:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifISOSpeed:
            printf( "ExifISOSpeed:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifVer:
            printf( "ExifVersion:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifShutterSpeed:
            printf( "ExifShutterSpeed:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifAperture:
            printf( "ExifAperture:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifBrightness:
            printf( "ExifBrightness:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifExposureBias:
            printf( "ExifExposureBias:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifMaxAperture:
            printf( "ExifMaxAperture:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifSubjectDis:
            printf( "ExifSubjectDis:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifMeteringMode:
            printf( "ExifMeteringMode:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifLightSource:
            printf( "ExifLightSource:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifFlash:
            printf( "ExifFlash:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifFocalLength:
            printf( "ExifFocalLength:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifPixXDim:
            printf( "Image Width pix:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifPixYDim:
            printf( "Image Height pix:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagCompression:
            printf( "Compression:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagPhotometricInterp:
            printf( "PhotometricInterp:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagStripOffsets:
            printf( "StripOffsets(tiff offset):" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagStripBytesCount:
            printf( "StripBytesCount:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagJPEGInterFormat:
            printf( "JPEGInterFormat(smaller jpeg offset):" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagJPEGInterLength:
            printf( "JPEGInterLength:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsVer:
            printf( "GpsVersion:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsLatitudeRef:
            printf( "GpsLatitudeRef:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsLatitude:
            printf( "GpsLatitude is:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsLongitudeRef:
            printf( "GpsLongitudeRef is:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsLongitude:
            printf( "GpsLongitude is:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsAltitudeRef:
            printf( "GpsAltitudeRef is:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagGpsAltitude:
            printf( "GpsAltitude is:" );
            DealWithTag( &tb[i], apparea );
            break;
        case TagExifIFD:
            printf( "ExifIFD Offset is:" );
            DealWithTag( &tb[i], apparea );
            ShowDirInfo( apparea, offset + 12 );
            break;
        case TagGPSInfo:
            printf( "GPS Offset:" );
            DealWithTag( &tb[i], apparea );
            ShowDirInfo( apparea, offset + 12 );
            break;
        default:
            break;
        }
    }
}


void CopyDataArea( const char *filename, unsigned char area[], int size )
{
    FILE * fp = fopen( filename, "rb+" );
    if ( fp == NULL )
    {
        printf( "OPEN FILE FAILE!\n" );
        fclose( fp );
    }else {
        fread( area, size, 1, fp );
        fclose( fp );
    }
}


void ShowDirInfo( unsigned char apparea[], int startpos )
{
    tag_block   * tb;
    int     dir_tag_c, i, j, nextoffset;
#ifdef DEBUG
    printf( "[ShowDirInfo]---Show  Dir Info---\n" );
#endif
    if ( big_or_little == 0 )
    {
        dir_tag_c = (short) (apparea[startpos + 1] << 8 | apparea[startpos + 0]);
    }else{
        dir_tag_c = (short) (apparea[startpos + 0] << 8 | apparea[startpos + 1]);
    }
#ifdef DEBUG
    printf( "[ShowDirInfo]dir tag count is: %d\n", dir_tag_c );
#endif
    tb = (tag_block *) malloc( sizeof(tag_block) * dir_tag_c );
    for ( i = 0; i < dir_tag_c; i++ )
    {
        tb[i].tag[0]    = apparea[startpos + 2 + 12 * i];
        tb[i].tag[1]    = apparea[startpos + 3 + 12 * i];
        if ( big_or_little == 0 )
        {
            tb[i].dataformat    = (short) (apparea[startpos + 5 + 12 * i] << 8 | apparea[startpos + 4 + 12 * i]);
            tb[i].itemcount     = (int) (apparea[startpos + 9 + 12 * i] << 3 * 8 | apparea[startpos + 8 + 12 * i] << 2 * 8 | apparea[startpos + 7 + 12 * i] << 1 * 8 | apparea[startpos + 6 + 12 * i]);
        }else{
            tb[i].dataformat    = (short) (apparea[startpos + 4 + 12 * i] << 8 | apparea[startpos + 5 + 12 * i]);
            tb[i].itemcount     = (int) (apparea[startpos + 6 + 12 * i] << 3 * 8 | apparea[startpos + 7 + 12 * i] << 2 * 8 | apparea[startpos + 8 + 12 * i] << 1 * 8 | apparea[startpos + 9 + 12 * i]);
        }
        tb[i].valueOroffset[0]  = apparea[startpos + 10 + 12 * i];
        tb[i].valueOroffset[1]  = apparea[startpos + 11 + 12 * i];
        tb[i].valueOroffset[2]  = apparea[startpos + 12 + 12 * i];
        tb[i].valueOroffset[3]  = apparea[startpos + 13 + 12 * i];
    }
#ifdef DEBUG
    show_tag_block( tb, dir_tag_c );
    printf( "[ShowDirInfo]dir end raw:" );
    show_bytes_u( &apparea[startpos + 2 + 12 * i], 4 );
#endif
    if ( big_or_little == 0 )
    {
        printf( "[ShowDirInfo]dir end offset is %d \n", (int) (apparea[startpos + 2 + 12 * i + 3] << 3 * 8 |
                                       apparea[startpos + 2 + 12 * i + 2] << 2 * 8 |
                                       apparea[startpos + 2 + 12 * i + 1] << 1 * 8 |
                                       apparea[startpos + 2 + 12 * i + 0] << 0 * 8) + 12 );
    }else{
        printf( "[ShowDirInfo]dir end offset is %d \n", (int) (apparea[startpos + 2 + 12 * i + 0] << 3 * 8 |
                                       apparea[startpos + 2 + 12 * i + 1] << 2 * 8 |
                                       apparea[startpos + 2 + 12 * i + 2] << 1 * 8 |
                                       apparea[startpos + 2 + 12 * i + 3] << 0 * 8) + 12 );
    }

    AnalysisTag( tb, dir_tag_c, apparea );
    if ( big_or_little == 0 )
    {
        nextoffset = (int) (apparea[startpos + 2 + 12 * i + 3] << 3 * 8 |
                    apparea[startpos + 2 + 12 * i + 2] << 2 * 8 |
                    apparea[startpos + 2 + 12 * i + 1] << 1 * 8 |
                    apparea[startpos + 2 + 12 * i + 0] << 0 * 8) + 12;
    }else{
        nextoffset = (int) (apparea[startpos + 2 + 12 * i + 0] << 3 * 8 |
                    apparea[startpos + 2 + 12 * i + 1] << 2 * 8 |
                    apparea[startpos + 2 + 12 * i + 2] << 1 * 8 |
                    apparea[startpos + 2 + 12 * i + 3] << 0 * 8) + 12;
    }
    if ( nextoffset == 0x0C )
    {
        return;
    }else{
        ShowDirInfo( apparea, nextoffset );
    }
}


void ExifTagPos( unsigned char apparea[], int startpos, unsigned short Tag, int*pos )
{
    int     dir_tag_c, i, j, nextoffset;
    unsigned short  TempTag;
    if ( big_or_little == 0 )
    {
        dir_tag_c = (short) (apparea[startpos + 1] << 8 | apparea[startpos + 0]);
    }else{
        dir_tag_c = (short) (apparea[startpos + 0] << 8 | apparea[startpos + 1]);
    }
    for ( i = 0; i < dir_tag_c; i++ )
    {
        if ( big_or_little == 0 )
        {
            TempTag = apparea[startpos + 3 + 12 * i] << 8 | apparea[startpos + 2 + 12 * i];
        }else{
            TempTag = apparea[startpos + 2 + 12 * i] << 8 | apparea[startpos + 3 + 12 * i];
        }
        if ( TempTag == Tag )
        {
            *pos = startpos + 2 + 12 * i;
        }
    }
}


void Load_tag( _edit_tag *etag, int pos, unsigned char apparea[] )
{
    if ( big_or_little == 0 )
    {
        etag->tag       = (short) (apparea[pos + 1] << 8 | apparea[pos + 0]);
        etag->dataformat    = (short) (apparea[pos + 3] << 8 | apparea[pos + 2]);
        etag->countitem     = (int) (apparea[pos + 7] << 3 * 8 |
                         apparea[pos + 6] << 2 * 8 |
                         apparea[pos + 5] << 1 * 8 |
                         apparea[pos + 4] << 0 * 8);
        etag->offset = (int) (apparea[pos + 11] << 3 * 8 |
                      apparea[pos + 10] << 2 * 8 |
                      apparea[pos + 9] << 1 * 8 |
                      apparea[pos + 8] << 0 * 8);
    }else{
        etag->tag       = (short) (apparea[pos + 0] << 8 | apparea[pos + 1]);
        etag->dataformat    = (short) (apparea[pos + 2] << 8 | apparea[pos + 3]);
        etag->countitem     = (int) (apparea[pos + 4] << 3 * 8 |
                         apparea[pos + 5] << 2 * 8 |
                         apparea[pos + 6] << 1 * 8 |
                         apparea[pos + 7] << 0 * 8);
        etag->offset = (int) (apparea[pos + 8] << 3 * 8 |
                      apparea[pos + 9] << 2 * 8 |
                      apparea[pos + 10] << 1 * 8 |
                      apparea[pos + 11] << 0 * 8);
    }
}


void Load_value( int pos, unsigned char apparea[], int size, unsigned char*in )
{
    int i;
    if ( big_or_little = 0 )
    {
        for ( i = 0; i < (size / 4); i++ )
        {
            apparea[pos + i * 4 + 3]    = in[i * 4 + 0];
            apparea[pos + i * 4 + 2]    = in[i * 4 + 1];
            apparea[pos + i * 4 + 1]    = in[i * 4 + 2];
            apparea[pos + i * 4 + 0]    = in[i * 4 + 3];
        }
    }else{
        for ( i = 0; i < (size / 4); i++ )
        {
            apparea[pos + i * 4 + 0]    = in[i * 4 + 0];
            apparea[pos + i * 4 + 1]    = in[i * 4 + 1];
            apparea[pos + i * 4 + 2]    = in[i * 4 + 2];
            apparea[pos + i * 4 + 3]    = in[i * 4 + 3];
        }
    }
}


void printf_load_tag( _edit_tag*etag )
{
    printf( "Tag is %x\n", etag->tag );
    printf( "dataformat is %d \n", etag->dataformat );
    printf( "countitem is %d \n", etag->countitem );
    printf( "offset is %x \n", etag->offset );
}


void EditGPSTag( unsigned char apparea[], int startpos )
{
    int     pos;
    _edit_tag   gpstag;
    _edit_tag   gpsLongitude;
    _edit_tag   gpsLatitude;
    struct Lat_Lon  lon;
    ExifTagPos( apparea, startpos, TagGPSInfo, &pos );
    printf( "[EditGPSTag] gps tag pos is %x \n", pos );
    Load_tag( &gpstag, pos, apparea );
    ExifTagPos( apparea, gpstag.offset + 12, TagGpsLatitude, &pos );
    printf( "[EditGPSTag] gpsLatitudepos tag pos is %x \n", pos );
    Load_tag( &gpsLatitude, pos, apparea );
    ExifTagPos( apparea, gpstag.offset + 12, TagGpsLongitude, &pos );
    printf( "[EditGPSTag] gpsLongitudepos tag pos is %x \n", pos );
    Load_tag( &gpsLongitude, pos, apparea );
    printf_load_tag( &gpsLatitude );
    printf_load_tag( &gpsLongitude );
    lon.degree  = 20;
    lon.unitd   = 1;
    lon.minute  = 12;
    lon.unitm   = 1;
    lon.second  = 10;
    lon.units   = 1;
    Load_value( gpsLatitude.offset + 12, apparea, 24, (unsigned char *) &lon );
    Load_value( gpsLongitude.offset + 12, apparea, 24, (unsigned char *) &lon );
    /* show_bytes_u((unsigned char*)&lon,24); */
}


int getPictureSize( const char * filename )
{
    int CountByte;
    FILE    *fp = fopen( filename, "rb+" );
    if ( fp == NULL )
    {
        printf( "OPEN FILE FAILED!\n" );
        fclose( fp );
        return(-1);
    }else{
        fseek( fp, 0, 2 );
        CountByte = ftell( fp );
        printf( "[getPictureSize] File Size is %d\n", CountByte );
        return(CountByte);
        fclose( fp );
    }
}


int CopyPicData( const char * filename, unsigned char pic[], int size, int seek )
{
    FILE *fp = fopen( filename, "rb+" );
    if ( fp == NULL )
    {
        printf( "OPEN FILE FAILED!\n" );
        fclose( fp );
        return(-1);
    }else{
        fseek( fp, seek, SEEK_SET );
        fread( pic, size, 1, fp );
        fclose( fp );
        return(0);
    }
}


int Save_Jpeg( const char * filename, int size, unsigned char *area, int picsize, unsigned char *pic )
{
    FILE *fp = fopen( filename, "wb+" );
    if ( fp == NULL )
    {
        printf( "OPEN FILE FAILED!\n" );
        fclose( fp );
        return(-1);
    }else{
        fwrite( area, size, 1, fp );
        fwrite( pic, picsize, 1, fp );
        fclose( fp );
    }
}


int main( int argc, char* argv[] )
{
    const char  * filename  = "test.jpg";
    const char  * savename  = "mytest.jpg";
    int     size;
    int     picsize;
    size = FirstSection( filename );
    printf( "[main]%d \n", size );
    unsigned char area[size];
    CopyDataArea( filename, area, size );
    ShowDirInfo( area, 20 );
    picsize = getPictureSize( filename );
    unsigned char picarea[picsize - size];
    CopyPicData( filename, picarea, picsize - size, size );
    EditGPSTag( area, 20 );
    Save_Jpeg( savename, size, area, picsize - size, picarea );
    return(0);
}


