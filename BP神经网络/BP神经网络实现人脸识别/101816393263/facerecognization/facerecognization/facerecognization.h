/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jun 04 12:54:16 2010
 */
/* Compiler settings for D:\MyProjects\VC6\facerecognization\facerecognization.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __facerecognization_h__
#define __facerecognization_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IPGMView_FWD_DEFINED__
#define __IPGMView_FWD_DEFINED__
typedef interface IPGMView IPGMView;
#endif 	/* __IPGMView_FWD_DEFINED__ */


#ifndef __PGMView_FWD_DEFINED__
#define __PGMView_FWD_DEFINED__

#ifdef __cplusplus
typedef class PGMView PGMView;
#else
typedef struct PGMView PGMView;
#endif /* __cplusplus */

#endif 	/* __PGMView_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IPGMView_INTERFACE_DEFINED__
#define __IPGMView_INTERFACE_DEFINED__

/* interface IPGMView */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPGMView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B62F6DD1-FF0B-4ECB-8784-E252942678A0")
    IPGMView : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPGMFilePath( 
            /* [in] */ BSTR sFilePath,
            VARIANT_BOOL bRefresh) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWidth( 
            /* [retval][out] */ LONG __RPC_FAR *nWidth) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHeight( 
            /* [retval][out] */ LONG __RPC_FAR *nHeight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IncreaseBrightness( 
            /* [in] */ CHAR nQuantity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecreaseBrightness( 
            /* [in] */ CHAR nQuantity) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPGMViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPGMView __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPGMView __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPGMView __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPGMFilePath )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ BSTR sFilePath,
            VARIANT_BOOL bRefresh);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IPGMView __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWidth )( 
            IPGMView __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *nWidth);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHeight )( 
            IPGMView __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *nHeight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IncreaseBrightness )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ CHAR nQuantity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DecreaseBrightness )( 
            IPGMView __RPC_FAR * This,
            /* [in] */ CHAR nQuantity);
        
        END_INTERFACE
    } IPGMViewVtbl;

    interface IPGMView
    {
        CONST_VTBL struct IPGMViewVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPGMView_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPGMView_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPGMView_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPGMView_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPGMView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPGMView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPGMView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPGMView_SetPGMFilePath(This,sFilePath,bRefresh)	\
    (This)->lpVtbl -> SetPGMFilePath(This,sFilePath,bRefresh)

#define IPGMView_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define IPGMView_GetWidth(This,nWidth)	\
    (This)->lpVtbl -> GetWidth(This,nWidth)

#define IPGMView_GetHeight(This,nHeight)	\
    (This)->lpVtbl -> GetHeight(This,nHeight)

#define IPGMView_IncreaseBrightness(This,nQuantity)	\
    (This)->lpVtbl -> IncreaseBrightness(This,nQuantity)

#define IPGMView_DecreaseBrightness(This,nQuantity)	\
    (This)->lpVtbl -> DecreaseBrightness(This,nQuantity)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPGMView_SetPGMFilePath_Proxy( 
    IPGMView __RPC_FAR * This,
    /* [in] */ BSTR sFilePath,
    VARIANT_BOOL bRefresh);


void __RPC_STUB IPGMView_SetPGMFilePath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPGMView_Refresh_Proxy( 
    IPGMView __RPC_FAR * This);


void __RPC_STUB IPGMView_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPGMView_GetWidth_Proxy( 
    IPGMView __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *nWidth);


void __RPC_STUB IPGMView_GetWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPGMView_GetHeight_Proxy( 
    IPGMView __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *nHeight);


void __RPC_STUB IPGMView_GetHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPGMView_IncreaseBrightness_Proxy( 
    IPGMView __RPC_FAR * This,
    /* [in] */ CHAR nQuantity);


void __RPC_STUB IPGMView_IncreaseBrightness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPGMView_DecreaseBrightness_Proxy( 
    IPGMView __RPC_FAR * This,
    /* [in] */ CHAR nQuantity);


void __RPC_STUB IPGMView_DecreaseBrightness_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPGMView_INTERFACE_DEFINED__ */



#ifndef __FACERECOGNIZATIONLib_LIBRARY_DEFINED__
#define __FACERECOGNIZATIONLib_LIBRARY_DEFINED__

/* library FACERECOGNIZATIONLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_FACERECOGNIZATIONLib;

EXTERN_C const CLSID CLSID_PGMView;

#ifdef __cplusplus

class DECLSPEC_UUID("26131110-9BF7-48B1-84BF-3951BDB58DEC")
PGMView;
#endif
#endif /* __FACERECOGNIZATIONLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
