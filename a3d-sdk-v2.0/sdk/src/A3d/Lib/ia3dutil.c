/*
 * ia3dutil.c
 *
 * Copyright (c) 1999 Aureal Semiconductor, Inc. - All rights reserved.
 *
 * This code may be used, copied or distributed in accordance with the terms
 * described in the A3D2.0 SDK License Agreement.
 *
 * This file can be compiled as C or C++        
 * If the following two undefined symbols occur
 * during the link phase: CLSID_A3dApi and      
 * IID_IA3d4, you need to include <initguid.h>       
*/

#include <windows.h>
//#include <initguid.h>

#include "ia3dutil.h"
#include "ia3dapi.h"
 
/* Use GUID_ instead of IID_ for both C or C++ */

#if !defined(__cplusplus) || defined(CINTERFACE)
#define GUID_IA3d			&IID_IA3d4
#define GUID_CLSID_A3dApi	&CLSID_A3dApi
#else
#define GUID_IA3d			IID_IA3d4
#define GUID_CLSID_A3dApi	CLSID_A3dApi
#endif

// Current interface version is IA3d4

#define IA3dX_Init(p,a,b,c)		IA3d4_Init(p,a,b,c)
#define IA3dX_Release(p)		IA3d4_Release(p)

/* =============================================================
// RegDBSetKeyValue()	Set the registry database key value
//
// =============================================================*/

static void RegDBSetKeyValue(
	char *szKey,		/* in, key string */
	char *szName,		/* in, name string NULL == Default */
	char *szValue)		/* in, value string */
{
	DWORD dwOptions   = REG_OPTION_NON_VOLATILE;
	REGSAM samDesired =  KEY_ALL_ACCESS;
	HKEY hKey;
	DWORD dwDisposition;

	/* Create the key, even if it already existed. */

	RegCreateKeyEx(
		HKEY_CLASSES_ROOT,	/* handle of an open key  */
        szKey,				/* lpSubKey address of subkey name  */
		0,					/* reserved word */
		"REG_SZ",			/* address of class string */ 
		dwOptions,			/* special options flag */
		samDesired,			/* desired security access */
		NULL,				/* address of key security structure */
		&hKey,				/* address of buffer for opened handle  */
		&dwDisposition);	/* address of disposition value buffer */

	/* Just write to it. */
    RegSetValueEx( 
		  hKey,								/* handle of key to set value for  */
          szName,							/* address of value to set */
		  0,								/* reserved */
		  REG_SZ,							/* flag for value type */
		  (CONST BYTE *)szValue,			/* address of value data */
		  strlen(szValue)					/* length of value buffer */
		  ); 

	/* close the key. */
	RegCloseKey(hKey);
}


/* =============================================================
// A3dRegister()		Registers the COM objects for A3D.
//
// Returns:
//
//   S_OK		
//		
// =============================================================*/

HRESULT A3dRegister(void)
{
    // A3d COM Keys
    RegDBSetKeyValue("A3d",	"",	"A3d Object");
    RegDBSetKeyValue("A3d\\CLSID", "", "{d8f1eee0-f634-11cf-8700-00a0245d918b}");
    RegDBSetKeyValue("CLSID\\{d8f1eee0-f634-11cf-8700-00a0245d918b}", "",  "A3d Object");
	RegDBSetKeyValue("CLSID\\{d8f1eee0-f634-11cf-8700-00a0245d918b}\\InprocServer32", "", "a3d.dll");
	RegDBSetKeyValue("CLSID\\{d8f1eee0-f634-11cf-8700-00a0245d918b}\\InprocServer32", "ThreadingModel",	"Apartment");

    RegDBSetKeyValue("A3dApi", "", "A3dApi Object");
    RegDBSetKeyValue("A3dApi\\CLSID", "", "{92FA2C24-253C-11d2-90FB-006008A1F441}");
    RegDBSetKeyValue("CLSID\\{92FA2C24-253C-11d2-90FB-006008A1F441}", "", "A3dApi Object");
    RegDBSetKeyValue("CLSID\\{92FA2C24-253C-11d2-90FB-006008A1F441}","AppID","{92FA2C24-253C-11D2-90FB-006008A1F441}");
	RegDBSetKeyValue("CLSID\\{92FA2C24-253C-11d2-90FB-006008A1F441}\\InprocServer32", "", "a3dapi.dll");
	RegDBSetKeyValue("CLSID\\{92FA2C24-253C-11d2-90FB-006008A1F441}\\InprocServer32", "ThreadingModel", "Apartment");


    RegDBSetKeyValue("A3dDAL", "", "A3dDAL Object");
    RegDBSetKeyValue("A3dDAL\\CLSID", "", "{442D12A1-2641-11d2-90FB-006008A1F441}");
    RegDBSetKeyValue("CLSID\\{442D12A1-2641-11d2-90FB-006008A1F441}", "", "A3dDAL Object");
    RegDBSetKeyValue("CLSID\\{442D12A1-2641-11d2-90FB-006008A1F441}","AppID","{442D12A1-2641-11D2-90FB-006008A1F441}");
	RegDBSetKeyValue("CLSID\\{442D12A1-2641-11d2-90FB-006008A1F441}\\InprocServer32", "", "a3d.dll");
	RegDBSetKeyValue("CLSID\\{442D12A1-2641-11d2-90FB-006008A1F441}\\InprocServer32", "ThreadingModel", "Apartment");


	return S_OK;
}


/*****************************************************************************
*
*  A3dInitialize()	Register the a3dapi.dll and call COM initialize.
*
*  Return:
*
*    >= 0            COM success
*
*    S_FALSE        already initialized
*    S_OK			first time initialized
*
*    < 0            COM error
*
******************************************************************************/

HRESULT A3dInitialize(void)
{
 HRESULT hr;

 /* COM Initialize */

 hr = CoInitialize(NULL);

 /* Can't register too many times. */
 A3dRegister();

 return hr;
}

/*****************************************************************************
*
*  A3dUninitialize()	Uninitializes A3d Library.  Call at the exit 
*                       of your program if you use any function in this module.
*						Once COM is uninitialized, no further COM objects can
*						be used unil COM is initialized again.
*
******************************************************************************/

void A3dUninitialize(void)
{

   CoUninitialize();

}

/*****************************************************************************
*
*  A3dCreate()		Creates the A3D object. Basically uses COM
*					function CoCreateInstance() to initialize the IA3d3 inteface 
*                   and properly register it.  
*
*  Return:
*
*   >= 0            COM success
*
*   S_OK          
*
*   < 0             COM error
*
* 	E_INVALIDARG	ppA3d is NULL.
*   E_FAIL
*
******************************************************************************/

HRESULT A3dCreate(
	LPGUID  lpGUID,				/* in,  Prefered Driver Guid, NULL ok */
	void **ppA3d,				/* out, A3d pointer */
	IUnknown FAR *pUnkOuter,	/* in,  Outer COM object, for Aggregate only NULL oK*/
	DWORD dwFeatures)			/* in, pass in the features requested by the developer.*/
{
	LPIA3D3       pIA3d = NULL;
	HRESULT       hr;
	
	/* Arg check */
	if (!ppA3d)
	{
		return E_INVALIDARG;
	}
	
	/* Defensively set to NULL */
	*ppA3d = NULL;
	
    /* Just in case has not done A3dInitialzed has not been called. */
	
	A3dInitialize();

	/* COM create the A3d root interface */

	if (FAILED(hr = CoCreateInstance(
		GUID_CLSID_A3dApi,		/* Class Id registerd to a3dapi.dll */
		pUnkOuter,				/* Aggregate COM object */
		CLSCTX_INPROC_SERVER,	/* Code runs on the same Machine */
		GUID_IA3d,				/* Interface that we want */
		(void **)&pIA3d)))		/* Pointer to the requested interface */
    {
		/* Fail to attach to A3dapi.dll */
		return hr;
	}

	/* Fail to construct the A3D class object */
	if (!pIA3d)
		return E_FAIL;

	/* Init A3D */
	if (FAILED(hr = IA3dX_Init(pIA3d, lpGUID, dwFeatures, A3DRENDERPREFS_DEFAULT)))
	{
	  IA3dX_Release(pIA3d);
	  return E_FAIL;
	}
	
	/* Success */
	*ppA3d = pIA3d;
	
	return S_OK;
}


