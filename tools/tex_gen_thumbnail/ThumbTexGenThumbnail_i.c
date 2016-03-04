/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Aug 05 01:54:00 2003
 */
/* Compiler settings for R:\code\nel\tools\tex_gen\tex_gen_thumbnail\ThumbTexGenThumbnail.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ITexGenThumbnailExtractor = {0xD7580208,0x50CD,0x4F2D,{0xB3,0xEC,0x8F,0x5C,0x93,0x9A,0x5B,0x9F}};


const IID LIBID_THUMBTEXGENTHUMBNAILLib = {0xD16D4DBB,0xF1D7,0x43D2,{0x8F,0x83,0x89,0x04,0x4C,0x1A,0x80,0x06}};


const CLSID CLSID_TexGenThumbnailExtractor = {0xCC651487,0xD734,0x49E4,{0x9B,0xFC,0x6E,0x6D,0x2D,0x8A,0xF9,0xF7}};


#ifdef __cplusplus
}
#endif

