/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu May 17 12:40:44 2018
 */
/* Compiler settings for E:\prj\win\dbc\vsu\vsu.idl:
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

#ifndef __vsu_h__
#define __vsu_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IHcUniverse_FWD_DEFINED__
#define __IHcUniverse_FWD_DEFINED__
typedef interface IHcUniverse IHcUniverse;
#endif 	/* __IHcUniverse_FWD_DEFINED__ */


#ifndef __IHcUniverse2_FWD_DEFINED__
#define __IHcUniverse2_FWD_DEFINED__
typedef interface IHcUniverse2 IHcUniverse2;
#endif 	/* __IHcUniverse2_FWD_DEFINED__ */


#ifndef __IHcUniverse3_FWD_DEFINED__
#define __IHcUniverse3_FWD_DEFINED__
typedef interface IHcUniverse3 IHcUniverse3;
#endif 	/* __IHcUniverse3_FWD_DEFINED__ */


#ifndef __IHcUniverse4_FWD_DEFINED__
#define __IHcUniverse4_FWD_DEFINED__
typedef interface IHcUniverse4 IHcUniverse4;
#endif 	/* __IHcUniverse4_FWD_DEFINED__ */


#ifndef __IHcUniverse5_FWD_DEFINED__
#define __IHcUniverse5_FWD_DEFINED__
typedef interface IHcUniverse5 IHcUniverse5;
#endif 	/* __IHcUniverse5_FWD_DEFINED__ */


#ifndef __IHcUniverse6_FWD_DEFINED__
#define __IHcUniverse6_FWD_DEFINED__
typedef interface IHcUniverse6 IHcUniverse6;
#endif 	/* __IHcUniverse6_FWD_DEFINED__ */


#ifndef __HcUniverse_FWD_DEFINED__
#define __HcUniverse_FWD_DEFINED__

#ifdef __cplusplus
typedef class HcUniverse HcUniverse;
#else
typedef struct HcUniverse HcUniverse;
#endif /* __cplusplus */

#endif 	/* __HcUniverse_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IHcUniverse_INTERFACE_DEFINED__
#define __IHcUniverse_INTERFACE_DEFINED__

/* interface IHcUniverse */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHcUniverse;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("291BDFBF-E9FC-11D3-9BE8-008048C5FDF8")
    IHcUniverse : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ProcessFile( 
            /* [in] */ BSTR File) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClrAllPlu( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHcUniverseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHcUniverse __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHcUniverse __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHcUniverse __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHcUniverse __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHcUniverse __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHcUniverse __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHcUniverse __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IHcUniverse __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFile )( 
            IHcUniverse __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClrAllPlu )( 
            IHcUniverse __RPC_FAR * This);
        
        END_INTERFACE
    } IHcUniverseVtbl;

    interface IHcUniverse
    {
        CONST_VTBL struct IHcUniverseVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHcUniverse_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHcUniverse_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHcUniverse_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHcUniverse_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHcUniverse_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHcUniverse_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHcUniverse_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHcUniverse_Register(This,Machine,LogNum,User,Password)	\
    (This)->lpVtbl -> Register(This,Machine,LogNum,User,Password)

#define IHcUniverse_ProcessFile(This,File)	\
    (This)->lpVtbl -> ProcessFile(This,File)

#define IHcUniverse_ClrAllPlu(This)	\
    (This)->lpVtbl -> ClrAllPlu(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse_Register_Proxy( 
    IHcUniverse __RPC_FAR * This,
    /* [in] */ BSTR Machine,
    /* [in] */ long LogNum,
    /* [in] */ long User,
    /* [in] */ long Password);


void __RPC_STUB IHcUniverse_Register_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse_ProcessFile_Proxy( 
    IHcUniverse __RPC_FAR * This,
    /* [in] */ BSTR File);


void __RPC_STUB IHcUniverse_ProcessFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse_ClrAllPlu_Proxy( 
    IHcUniverse __RPC_FAR * This);


void __RPC_STUB IHcUniverse_ClrAllPlu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHcUniverse_INTERFACE_DEFINED__ */


#ifndef __IHcUniverse2_INTERFACE_DEFINED__
#define __IHcUniverse2_INTERFACE_DEFINED__

/* interface IHcUniverse2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHcUniverse2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("600D3CE0-4F41-11D4-9D03-008048C5FDF8")
    IHcUniverse2 : public IHcUniverse
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Squeeze( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Serial( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Call0( 
            /* [in] */ BSTR ProcName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Call1( 
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetICP( 
            /* [in] */ long Input) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetOCP( 
            /* [in] */ long Output) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrintForm( 
            /* [in] */ BSTR File) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrintComment( 
            /* [in] */ BSTR File) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Unload( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHcUniverse2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHcUniverse2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHcUniverse2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFile )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClrAllPlu )( 
            IHcUniverse2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Squeeze )( 
            IHcUniverse2 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Serial )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call0 )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ BSTR ProcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call1 )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetICP )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ long Input);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOCP )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ long Output);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintForm )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintComment )( 
            IHcUniverse2 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IHcUniverse2 __RPC_FAR * This);
        
        END_INTERFACE
    } IHcUniverse2Vtbl;

    interface IHcUniverse2
    {
        CONST_VTBL struct IHcUniverse2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHcUniverse2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHcUniverse2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHcUniverse2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHcUniverse2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHcUniverse2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHcUniverse2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHcUniverse2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHcUniverse2_Register(This,Machine,LogNum,User,Password)	\
    (This)->lpVtbl -> Register(This,Machine,LogNum,User,Password)

#define IHcUniverse2_ProcessFile(This,File)	\
    (This)->lpVtbl -> ProcessFile(This,File)

#define IHcUniverse2_ClrAllPlu(This)	\
    (This)->lpVtbl -> ClrAllPlu(This)


#define IHcUniverse2_Squeeze(This)	\
    (This)->lpVtbl -> Squeeze(This)

#define IHcUniverse2_Serial(This,pVal)	\
    (This)->lpVtbl -> Serial(This,pVal)

#define IHcUniverse2_Call0(This,ProcName)	\
    (This)->lpVtbl -> Call0(This,ProcName)

#define IHcUniverse2_Call1(This,ProcName,p1)	\
    (This)->lpVtbl -> Call1(This,ProcName,p1)

#define IHcUniverse2_SetICP(This,Input)	\
    (This)->lpVtbl -> SetICP(This,Input)

#define IHcUniverse2_SetOCP(This,Output)	\
    (This)->lpVtbl -> SetOCP(This,Output)

#define IHcUniverse2_PrintForm(This,File)	\
    (This)->lpVtbl -> PrintForm(This,File)

#define IHcUniverse2_PrintComment(This,File)	\
    (This)->lpVtbl -> PrintComment(This,File)

#define IHcUniverse2_Unload(This)	\
    (This)->lpVtbl -> Unload(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_Squeeze_Proxy( 
    IHcUniverse2 __RPC_FAR * This);


void __RPC_STUB IHcUniverse2_Squeeze_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_Serial_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IHcUniverse2_Serial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_Call0_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [in] */ BSTR ProcName);


void __RPC_STUB IHcUniverse2_Call0_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_Call1_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [in] */ BSTR ProcName,
    /* [in] */ VARIANT p1);


void __RPC_STUB IHcUniverse2_Call1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_SetICP_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [in] */ long Input);


void __RPC_STUB IHcUniverse2_SetICP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_SetOCP_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [in] */ long Output);


void __RPC_STUB IHcUniverse2_SetOCP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_PrintForm_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [in] */ BSTR File);


void __RPC_STUB IHcUniverse2_PrintForm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_PrintComment_Proxy( 
    IHcUniverse2 __RPC_FAR * This,
    /* [in] */ BSTR File);


void __RPC_STUB IHcUniverse2_PrintComment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse2_Unload_Proxy( 
    IHcUniverse2 __RPC_FAR * This);


void __RPC_STUB IHcUniverse2_Unload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHcUniverse2_INTERFACE_DEFINED__ */


#ifndef __IHcUniverse3_INTERFACE_DEFINED__
#define __IHcUniverse3_INTERFACE_DEFINED__

/* interface IHcUniverse3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHcUniverse3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A7B1FAE9-40E3-40EB-B28F-B496311A083E")
    IHcUniverse3 : public IHcUniverse2
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegisterEx( 
            /* [in] */ BSTR Machine,
            /* [in] */ long Port,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWatchDir( 
            /* [in] */ BSTR Dir) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrintVoid( 
            BSTR File) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHcUniverse3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHcUniverse3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHcUniverse3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFile )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClrAllPlu )( 
            IHcUniverse3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Squeeze )( 
            IHcUniverse3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Serial )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call0 )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR ProcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call1 )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetICP )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ long Input);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOCP )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ long Output);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintForm )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintComment )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IHcUniverse3 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterEx )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long Port,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWatchDir )( 
            IHcUniverse3 __RPC_FAR * This,
            /* [in] */ BSTR Dir);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintVoid )( 
            IHcUniverse3 __RPC_FAR * This,
            BSTR File);
        
        END_INTERFACE
    } IHcUniverse3Vtbl;

    interface IHcUniverse3
    {
        CONST_VTBL struct IHcUniverse3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHcUniverse3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHcUniverse3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHcUniverse3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHcUniverse3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHcUniverse3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHcUniverse3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHcUniverse3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHcUniverse3_Register(This,Machine,LogNum,User,Password)	\
    (This)->lpVtbl -> Register(This,Machine,LogNum,User,Password)

#define IHcUniverse3_ProcessFile(This,File)	\
    (This)->lpVtbl -> ProcessFile(This,File)

#define IHcUniverse3_ClrAllPlu(This)	\
    (This)->lpVtbl -> ClrAllPlu(This)


#define IHcUniverse3_Squeeze(This)	\
    (This)->lpVtbl -> Squeeze(This)

#define IHcUniverse3_Serial(This,pVal)	\
    (This)->lpVtbl -> Serial(This,pVal)

#define IHcUniverse3_Call0(This,ProcName)	\
    (This)->lpVtbl -> Call0(This,ProcName)

#define IHcUniverse3_Call1(This,ProcName,p1)	\
    (This)->lpVtbl -> Call1(This,ProcName,p1)

#define IHcUniverse3_SetICP(This,Input)	\
    (This)->lpVtbl -> SetICP(This,Input)

#define IHcUniverse3_SetOCP(This,Output)	\
    (This)->lpVtbl -> SetOCP(This,Output)

#define IHcUniverse3_PrintForm(This,File)	\
    (This)->lpVtbl -> PrintForm(This,File)

#define IHcUniverse3_PrintComment(This,File)	\
    (This)->lpVtbl -> PrintComment(This,File)

#define IHcUniverse3_Unload(This)	\
    (This)->lpVtbl -> Unload(This)


#define IHcUniverse3_RegisterEx(This,Machine,Port,LogNum,User,Password)	\
    (This)->lpVtbl -> RegisterEx(This,Machine,Port,LogNum,User,Password)

#define IHcUniverse3_SetWatchDir(This,Dir)	\
    (This)->lpVtbl -> SetWatchDir(This,Dir)

#define IHcUniverse3_PrintVoid(This,File)	\
    (This)->lpVtbl -> PrintVoid(This,File)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse3_RegisterEx_Proxy( 
    IHcUniverse3 __RPC_FAR * This,
    /* [in] */ BSTR Machine,
    /* [in] */ long Port,
    /* [in] */ long LogNum,
    /* [in] */ long User,
    /* [in] */ long Password);


void __RPC_STUB IHcUniverse3_RegisterEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse3_SetWatchDir_Proxy( 
    IHcUniverse3 __RPC_FAR * This,
    /* [in] */ BSTR Dir);


void __RPC_STUB IHcUniverse3_SetWatchDir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse3_PrintVoid_Proxy( 
    IHcUniverse3 __RPC_FAR * This,
    BSTR File);


void __RPC_STUB IHcUniverse3_PrintVoid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHcUniverse3_INTERFACE_DEFINED__ */


#ifndef __IHcUniverse4_INTERFACE_DEFINED__
#define __IHcUniverse4_INTERFACE_DEFINED__

/* interface IHcUniverse4 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHcUniverse4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9B117E3C-8342-4325-AA5D-B460202EDE01")
    IHcUniverse4 : public IHcUniverse3
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFormat( 
            /* [in] */ long nProt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWildcard( 
            /* [in] */ BSTR Mask) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHcUniverse4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHcUniverse4 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHcUniverse4 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFile )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClrAllPlu )( 
            IHcUniverse4 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Squeeze )( 
            IHcUniverse4 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Serial )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call0 )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR ProcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call1 )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetICP )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ long Input);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOCP )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ long Output);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintForm )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintComment )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IHcUniverse4 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterEx )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long Port,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWatchDir )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR Dir);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintVoid )( 
            IHcUniverse4 __RPC_FAR * This,
            BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFormat )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ long nProt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWildcard )( 
            IHcUniverse4 __RPC_FAR * This,
            /* [in] */ BSTR Mask);
        
        END_INTERFACE
    } IHcUniverse4Vtbl;

    interface IHcUniverse4
    {
        CONST_VTBL struct IHcUniverse4Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHcUniverse4_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHcUniverse4_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHcUniverse4_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHcUniverse4_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHcUniverse4_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHcUniverse4_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHcUniverse4_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHcUniverse4_Register(This,Machine,LogNum,User,Password)	\
    (This)->lpVtbl -> Register(This,Machine,LogNum,User,Password)

#define IHcUniverse4_ProcessFile(This,File)	\
    (This)->lpVtbl -> ProcessFile(This,File)

#define IHcUniverse4_ClrAllPlu(This)	\
    (This)->lpVtbl -> ClrAllPlu(This)


#define IHcUniverse4_Squeeze(This)	\
    (This)->lpVtbl -> Squeeze(This)

#define IHcUniverse4_Serial(This,pVal)	\
    (This)->lpVtbl -> Serial(This,pVal)

#define IHcUniverse4_Call0(This,ProcName)	\
    (This)->lpVtbl -> Call0(This,ProcName)

#define IHcUniverse4_Call1(This,ProcName,p1)	\
    (This)->lpVtbl -> Call1(This,ProcName,p1)

#define IHcUniverse4_SetICP(This,Input)	\
    (This)->lpVtbl -> SetICP(This,Input)

#define IHcUniverse4_SetOCP(This,Output)	\
    (This)->lpVtbl -> SetOCP(This,Output)

#define IHcUniverse4_PrintForm(This,File)	\
    (This)->lpVtbl -> PrintForm(This,File)

#define IHcUniverse4_PrintComment(This,File)	\
    (This)->lpVtbl -> PrintComment(This,File)

#define IHcUniverse4_Unload(This)	\
    (This)->lpVtbl -> Unload(This)


#define IHcUniverse4_RegisterEx(This,Machine,Port,LogNum,User,Password)	\
    (This)->lpVtbl -> RegisterEx(This,Machine,Port,LogNum,User,Password)

#define IHcUniverse4_SetWatchDir(This,Dir)	\
    (This)->lpVtbl -> SetWatchDir(This,Dir)

#define IHcUniverse4_PrintVoid(This,File)	\
    (This)->lpVtbl -> PrintVoid(This,File)


#define IHcUniverse4_SetFormat(This,nProt)	\
    (This)->lpVtbl -> SetFormat(This,nProt)

#define IHcUniverse4_SetWildcard(This,Mask)	\
    (This)->lpVtbl -> SetWildcard(This,Mask)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse4_SetFormat_Proxy( 
    IHcUniverse4 __RPC_FAR * This,
    /* [in] */ long nProt);


void __RPC_STUB IHcUniverse4_SetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse4_SetWildcard_Proxy( 
    IHcUniverse4 __RPC_FAR * This,
    /* [in] */ BSTR Mask);


void __RPC_STUB IHcUniverse4_SetWildcard_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHcUniverse4_INTERFACE_DEFINED__ */


#ifndef __IHcUniverse5_INTERFACE_DEFINED__
#define __IHcUniverse5_INTERFACE_DEFINED__

/* interface IHcUniverse5 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHcUniverse5;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3D9551E1-C91A-40CB-9783-EF5AF5DB901F")
    IHcUniverse5 : public IHcUniverse4
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Call5( 
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1,
            /* [in] */ VARIANT p2,
            /* [in] */ VARIANT p3,
            /* [in] */ VARIANT p4,
            /* [in] */ VARIANT p5) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHcUniverse5Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHcUniverse5 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHcUniverse5 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFile )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClrAllPlu )( 
            IHcUniverse5 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Squeeze )( 
            IHcUniverse5 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Serial )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call0 )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR ProcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call1 )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetICP )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ long Input);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOCP )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ long Output);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintForm )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintComment )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IHcUniverse5 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterEx )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long Port,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWatchDir )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR Dir);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintVoid )( 
            IHcUniverse5 __RPC_FAR * This,
            BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFormat )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ long nProt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWildcard )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR Mask);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call5 )( 
            IHcUniverse5 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1,
            /* [in] */ VARIANT p2,
            /* [in] */ VARIANT p3,
            /* [in] */ VARIANT p4,
            /* [in] */ VARIANT p5);
        
        END_INTERFACE
    } IHcUniverse5Vtbl;

    interface IHcUniverse5
    {
        CONST_VTBL struct IHcUniverse5Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHcUniverse5_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHcUniverse5_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHcUniverse5_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHcUniverse5_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHcUniverse5_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHcUniverse5_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHcUniverse5_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHcUniverse5_Register(This,Machine,LogNum,User,Password)	\
    (This)->lpVtbl -> Register(This,Machine,LogNum,User,Password)

#define IHcUniverse5_ProcessFile(This,File)	\
    (This)->lpVtbl -> ProcessFile(This,File)

#define IHcUniverse5_ClrAllPlu(This)	\
    (This)->lpVtbl -> ClrAllPlu(This)


#define IHcUniverse5_Squeeze(This)	\
    (This)->lpVtbl -> Squeeze(This)

#define IHcUniverse5_Serial(This,pVal)	\
    (This)->lpVtbl -> Serial(This,pVal)

#define IHcUniverse5_Call0(This,ProcName)	\
    (This)->lpVtbl -> Call0(This,ProcName)

#define IHcUniverse5_Call1(This,ProcName,p1)	\
    (This)->lpVtbl -> Call1(This,ProcName,p1)

#define IHcUniverse5_SetICP(This,Input)	\
    (This)->lpVtbl -> SetICP(This,Input)

#define IHcUniverse5_SetOCP(This,Output)	\
    (This)->lpVtbl -> SetOCP(This,Output)

#define IHcUniverse5_PrintForm(This,File)	\
    (This)->lpVtbl -> PrintForm(This,File)

#define IHcUniverse5_PrintComment(This,File)	\
    (This)->lpVtbl -> PrintComment(This,File)

#define IHcUniverse5_Unload(This)	\
    (This)->lpVtbl -> Unload(This)


#define IHcUniverse5_RegisterEx(This,Machine,Port,LogNum,User,Password)	\
    (This)->lpVtbl -> RegisterEx(This,Machine,Port,LogNum,User,Password)

#define IHcUniverse5_SetWatchDir(This,Dir)	\
    (This)->lpVtbl -> SetWatchDir(This,Dir)

#define IHcUniverse5_PrintVoid(This,File)	\
    (This)->lpVtbl -> PrintVoid(This,File)


#define IHcUniverse5_SetFormat(This,nProt)	\
    (This)->lpVtbl -> SetFormat(This,nProt)

#define IHcUniverse5_SetWildcard(This,Mask)	\
    (This)->lpVtbl -> SetWildcard(This,Mask)


#define IHcUniverse5_Call5(This,ProcName,p1,p2,p3,p4,p5)	\
    (This)->lpVtbl -> Call5(This,ProcName,p1,p2,p3,p4,p5)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse5_Call5_Proxy( 
    IHcUniverse5 __RPC_FAR * This,
    /* [in] */ BSTR ProcName,
    /* [in] */ VARIANT p1,
    /* [in] */ VARIANT p2,
    /* [in] */ VARIANT p3,
    /* [in] */ VARIANT p4,
    /* [in] */ VARIANT p5);


void __RPC_STUB IHcUniverse5_Call5_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHcUniverse5_INTERFACE_DEFINED__ */


#ifndef __IHcUniverse6_INTERFACE_DEFINED__
#define __IHcUniverse6_INTERFACE_DEFINED__

/* interface IHcUniverse6 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHcUniverse6;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("438CEDC1-8346-4825-8D9B-FE5B39C1C6D1")
    IHcUniverse6 : public IHcUniverse5
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrintError( 
            /* [in] */ BSTR FileName,
            /* [in] */ long eCode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHcUniverse6Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHcUniverse6 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHcUniverse6 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFile )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClrAllPlu )( 
            IHcUniverse6 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Squeeze )( 
            IHcUniverse6 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Serial )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call0 )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR ProcName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call1 )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetICP )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ long Input);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOCP )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ long Output);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintForm )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintComment )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unload )( 
            IHcUniverse6 __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterEx )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR Machine,
            /* [in] */ long Port,
            /* [in] */ long LogNum,
            /* [in] */ long User,
            /* [in] */ long Password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWatchDir )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR Dir);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintVoid )( 
            IHcUniverse6 __RPC_FAR * This,
            BSTR File);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFormat )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ long nProt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWildcard )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR Mask);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Call5 )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR ProcName,
            /* [in] */ VARIANT p1,
            /* [in] */ VARIANT p2,
            /* [in] */ VARIANT p3,
            /* [in] */ VARIANT p4,
            /* [in] */ VARIANT p5);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintError )( 
            IHcUniverse6 __RPC_FAR * This,
            /* [in] */ BSTR FileName,
            /* [in] */ long eCode);
        
        END_INTERFACE
    } IHcUniverse6Vtbl;

    interface IHcUniverse6
    {
        CONST_VTBL struct IHcUniverse6Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHcUniverse6_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHcUniverse6_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHcUniverse6_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHcUniverse6_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHcUniverse6_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHcUniverse6_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHcUniverse6_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHcUniverse6_Register(This,Machine,LogNum,User,Password)	\
    (This)->lpVtbl -> Register(This,Machine,LogNum,User,Password)

#define IHcUniverse6_ProcessFile(This,File)	\
    (This)->lpVtbl -> ProcessFile(This,File)

#define IHcUniverse6_ClrAllPlu(This)	\
    (This)->lpVtbl -> ClrAllPlu(This)


#define IHcUniverse6_Squeeze(This)	\
    (This)->lpVtbl -> Squeeze(This)

#define IHcUniverse6_Serial(This,pVal)	\
    (This)->lpVtbl -> Serial(This,pVal)

#define IHcUniverse6_Call0(This,ProcName)	\
    (This)->lpVtbl -> Call0(This,ProcName)

#define IHcUniverse6_Call1(This,ProcName,p1)	\
    (This)->lpVtbl -> Call1(This,ProcName,p1)

#define IHcUniverse6_SetICP(This,Input)	\
    (This)->lpVtbl -> SetICP(This,Input)

#define IHcUniverse6_SetOCP(This,Output)	\
    (This)->lpVtbl -> SetOCP(This,Output)

#define IHcUniverse6_PrintForm(This,File)	\
    (This)->lpVtbl -> PrintForm(This,File)

#define IHcUniverse6_PrintComment(This,File)	\
    (This)->lpVtbl -> PrintComment(This,File)

#define IHcUniverse6_Unload(This)	\
    (This)->lpVtbl -> Unload(This)


#define IHcUniverse6_RegisterEx(This,Machine,Port,LogNum,User,Password)	\
    (This)->lpVtbl -> RegisterEx(This,Machine,Port,LogNum,User,Password)

#define IHcUniverse6_SetWatchDir(This,Dir)	\
    (This)->lpVtbl -> SetWatchDir(This,Dir)

#define IHcUniverse6_PrintVoid(This,File)	\
    (This)->lpVtbl -> PrintVoid(This,File)


#define IHcUniverse6_SetFormat(This,nProt)	\
    (This)->lpVtbl -> SetFormat(This,nProt)

#define IHcUniverse6_SetWildcard(This,Mask)	\
    (This)->lpVtbl -> SetWildcard(This,Mask)


#define IHcUniverse6_Call5(This,ProcName,p1,p2,p3,p4,p5)	\
    (This)->lpVtbl -> Call5(This,ProcName,p1,p2,p3,p4,p5)


#define IHcUniverse6_PrintError(This,FileName,eCode)	\
    (This)->lpVtbl -> PrintError(This,FileName,eCode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHcUniverse6_PrintError_Proxy( 
    IHcUniverse6 __RPC_FAR * This,
    /* [in] */ BSTR FileName,
    /* [in] */ long eCode);


void __RPC_STUB IHcUniverse6_PrintError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHcUniverse6_INTERFACE_DEFINED__ */



#ifndef __VSULib_LIBRARY_DEFINED__
#define __VSULib_LIBRARY_DEFINED__

/* library VSULib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VSULib;

EXTERN_C const CLSID CLSID_HcUniverse;

#ifdef __cplusplus

class DECLSPEC_UUID("291BDFC0-E9FC-11D3-9BE8-008048C5FDF8")
HcUniverse;
#endif
#endif /* __VSULib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
