/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jun 04 12:54:16 2010
 */
/* Compiler settings for D:\MyProjects\VC6\facerecognization\facerecognization.idl:
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

const IID IID_IPGMView = {0xB62F6DD1,0xFF0B,0x4ECB,{0x87,0x84,0xE2,0x52,0x94,0x26,0x78,0xA0}};


const IID LIBID_FACERECOGNIZATIONLib = {0x48151D37,0x580D,0x43CF,{0x97,0x92,0x68,0xAE,0x20,0x56,0x98,0x97}};


const CLSID CLSID_PGMView = {0x26131110,0x9BF7,0x48B1,{0x84,0xBF,0x39,0x51,0xBD,0xB5,0x8D,0xEC}};


#ifdef __cplusplus
}
#endif

