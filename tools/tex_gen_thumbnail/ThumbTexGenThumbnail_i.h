/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Aug 05 01:54:00 2003
 */
/* Compiler settings for R:\code\nel\tools\tex_gen\tex_gen_thumbnail\ThumbTexGenThumbnail.idl:
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

#ifndef __ThumbTexGenThumbnail_i_h__
#define __ThumbTexGenThumbnail_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITexGenThumbnailExtractor_FWD_DEFINED__
#define __ITexGenThumbnailExtractor_FWD_DEFINED__
typedef interface ITexGenThumbnailExtractor ITexGenThumbnailExtractor;
#endif 	/* __ITexGenThumbnailExtractor_FWD_DEFINED__ */


#ifndef __TexGenThumbnailExtractor_FWD_DEFINED__
#define __TexGenThumbnailExtractor_FWD_DEFINED__

#ifdef __cplusplus
typedef class TexGenThumbnailExtractor TexGenThumbnailExtractor;
#else
typedef struct TexGenThumbnailExtractor TexGenThumbnailExtractor;
#endif /* __cplusplus */

#endif 	/* __TexGenThumbnailExtractor_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ITexGenThumbnailExtractor_INTERFACE_DEFINED__
#define __ITexGenThumbnailExtractor_INTERFACE_DEFINED__

/* interface ITexGenThumbnailExtractor */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITexGenThumbnailExtractor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D7580208-50CD-4F2D-B3EC-8F5C939A5B9F")
    ITexGenThumbnailExtractor : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITexGenThumbnailExtractorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITexGenThumbnailExtractor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITexGenThumbnailExtractor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITexGenThumbnailExtractor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITexGenThumbnailExtractor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITexGenThumbnailExtractor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITexGenThumbnailExtractor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITexGenThumbnailExtractor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ITexGenThumbnailExtractorVtbl;

    interface ITexGenThumbnailExtractor
    {
        CONST_VTBL struct ITexGenThumbnailExtractorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITexGenThumbnailExtractor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITexGenThumbnailExtractor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITexGenThumbnailExtractor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITexGenThumbnailExtractor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITexGenThumbnailExtractor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITexGenThumbnailExtractor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITexGenThumbnailExtractor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITexGenThumbnailExtractor_INTERFACE_DEFINED__ */



#ifndef __THUMBTEXGENTHUMBNAILLib_LIBRARY_DEFINED__
#define __THUMBTEXGENTHUMBNAILLib_LIBRARY_DEFINED__

/* library THUMBTEXGENTHUMBNAILLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_THUMBTEXGENTHUMBNAILLib;

EXTERN_C const CLSID CLSID_TexGenThumbnailExtractor;

#ifdef __cplusplus

class DECLSPEC_UUID("CC651487-D734-49E4-9BFC-6E6D2D8AF9F7")
TexGenThumbnailExtractor;
#endif
#endif /* __THUMBTEXGENTHUMBNAILLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
