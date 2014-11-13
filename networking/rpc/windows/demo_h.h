

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Sep 20 18:58:33 2004
 */
/* Compiler settings for \Documents and Settings\raph\Mes documents\Visual Studio Projects\rpc\demo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , app_config, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __demo_h_h__
#define __demo_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __LGM_INTERFACE_DEFINED__
#define __LGM_INTERFACE_DEFINED__

/* interface LGM */
/* [implicit_handle][version][uuid] */ 

void func1( 
    /* [in] */ short n);

void func2( 
    /* [in] */ short n,
    /* [string][in] */ unsigned char *s);

void func3( 
    /* [out] */ float *f1);


extern handle_t hBinding;


extern RPC_IF_HANDLE LGM_v1_0_c_ifspec;
extern RPC_IF_HANDLE LGM_v1_0_s_ifspec;
#endif /* __LGM_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


