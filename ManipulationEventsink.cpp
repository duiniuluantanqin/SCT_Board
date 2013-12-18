// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "ManipulationEventsink.h"
#include "CoreObject.h"
#include <math.h>
#include <stdio.h>

static void SaveToFile(LPSTR str)
{
	HANDLE hdl = CreateFile("C:\\debug.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hdl == INVALID_HANDLE_VALUE)
	{
		return;
	}
	SetFilePointer(hdl, 0, NULL, FILE_END);
	DWORD dwWritenSize = 0;
	BOOL bRet = WriteFile(hdl, str, strlen(str),&dwWritenSize, NULL);
	FlushFileBuffers(hdl);	
	CloseHandle(hdl);
}

CManipulationEventSink::CManipulationEventSink(HWND hWnd, CCoreObject *coRef, int iTimerId, BOOL inertia): 
    m_coRef(coRef), 
	m_hWnd(hWnd), 
	m_iTimerId(iTimerId), 
	m_bInertia(inertia), 
	m_pConnPoint(NULL),
	m_cRefCount(1)
{
}

HRESULT STDMETHODCALLTYPE CManipulationEventSink::ManipulationStarted(
	FLOAT x,
	FLOAT y)
{
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CManipulationEventSink::ManipulationDelta(
	FLOAT x,
	FLOAT y,
	FLOAT translationDeltaX,
	FLOAT translationDeltaY,
	FLOAT scaleDelta,
	FLOAT expansionDelta,
	FLOAT rotationDelta,
	FLOAT cumulativeTranslationX,
	FLOAT cumulativeTranslationY,
	FLOAT cumulativeScale,
	FLOAT cumulativeExpansion,
	FLOAT cumulativeRotation)
{
	UNREFERENCED_PARAMETER(cumulativeRotation);
	UNREFERENCED_PARAMETER(cumulativeExpansion);
	UNREFERENCED_PARAMETER(cumulativeScale);
	UNREFERENCED_PARAMETER(cumulativeTranslationX);
	UNREFERENCED_PARAMETER(cumulativeTranslationY);
	UNREFERENCED_PARAMETER(expansionDelta);

	dx=translationDeltaX;
	dy=translationDeltaY;
	Angle=rotationDelta*180.0/3.1415926535;
	Scale=scaleDelta;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CManipulationEventSink::ManipulationCompleted(
	FLOAT x,
	FLOAT y,
	FLOAT cumulativeTranslationX,
	FLOAT cumulativeTranslationY,
	FLOAT cumulativeScale,
	FLOAT cumulativeExpansion,
	FLOAT cumulativeRotation)
{
	UNREFERENCED_PARAMETER(cumulativeRotation);
	UNREFERENCED_PARAMETER(cumulativeExpansion);
	UNREFERENCED_PARAMETER(cumulativeScale);
	UNREFERENCED_PARAMETER(cumulativeTranslationX);
	UNREFERENCED_PARAMETER(cumulativeTranslationY);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	
	return S_OK;
}

HRESULT CManipulationEventSink::SetupInertia(IInertiaProcessor* ip, IManipulationProcessor* mp)
{
	HRESULT hr = S_OK;

	// Set desired properties for inertia events

	// Deceleration for tranlations in pixel / msec^2
	HRESULT hrPutDD = ip->put_DesiredDeceleration(0.003f);

	// Deceleration for rotations in radians / msec^2
	HRESULT hrPutDAD = ip->put_DesiredAngularDeceleration(0.000015f);

	// Set initial origins

// 	HRESULT hrPutIOX = ip->put_InitialOriginX(m_coRef->doDrawing->GetCenterX());
// 	HRESULT hrPutIOY = ip->put_InitialOriginY(m_coRef->doDrawing->GetCenterY());

	FLOAT fVX;
	FLOAT fVY;
	FLOAT fVR;

	HRESULT hrPutVX = mp->GetVelocityX(&fVX);
	HRESULT hrGetVY = mp->GetVelocityY(&fVY);
	HRESULT hrGetAV = mp->GetAngularVelocity(&fVR);

	// Set initial velocities for inertia processor

	HRESULT hrPutIVX = ip->put_InitialVelocityX(fVX);
	HRESULT hrPutIVY = ip->put_InitialVelocityY(fVY);
	HRESULT hrPutIAV = ip->put_InitialAngularVelocity(fVR);

// 	if(FAILED(hrPutDD) || FAILED(hrPutDAD) || FAILED(hrPutIOX) || FAILED(hrPutIOY)
// 		|| FAILED(hrPutVX) || FAILED(hrGetVY) || FAILED(hrGetAV) || FAILED(hrPutIVX)
// 		|| FAILED(hrPutIVY) || FAILED(hrPutIAV))
// 	{
// 		hr = E_FAIL;
// 	}

	return hr;
}

ULONG CManipulationEventSink::AddRef()
{
	return ++m_cRefCount;
}

ULONG CManipulationEventSink::Release()
{
	m_cRefCount--;

	if(m_cRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_cRefCount;
}

HRESULT CManipulationEventSink::QueryInterface(REFIID riid, LPVOID *ppvObj)
{
	HRESULT hr = S_OK;

	if(ppvObj == NULL)
	{
		hr = E_POINTER;
	}

	if(!FAILED(hr))
	{
		*ppvObj = NULL;

		if (IID__IManipulationEvents == riid)
		{
			*ppvObj = static_cast<_IManipulationEvents*>(this);
		}
		else if (IID_IUnknown == riid)
		{
			*ppvObj = static_cast<IUnknown*>(this);
		}

		if(*ppvObj)
		{
			AddRef();
		}
		else
		{
			hr = E_NOINTERFACE;
		}
	}

	return hr;
}

// Set up the connection to a manipulation or inertia processor
BOOL CManipulationEventSink::SetupConnPt(IUnknown *manipulationProc)
{
	BOOL success = FALSE;
	IConnectionPointContainer* pConPointContainer = NULL;

	// Only connect if there isn't already an active connection
	if (m_pConnPoint == NULL)
	{
		// Check if supports connectable objects
		success = SUCCEEDED(manipulationProc->QueryInterface(IID_IConnectionPointContainer, 
			(LPVOID*)&(pConPointContainer)));

		// Get connection point interface
		if(success)
		{
			success = SUCCEEDED(pConPointContainer->FindConnectionPoint(
				_uuidof(_IManipulationEvents), 
				&(m_pConnPoint)));
		}

		// Clean up connection point container
		if (pConPointContainer != NULL)
		{
			pConPointContainer->Release();
			pConPointContainer = NULL;
		}

		// Hook event object to the connection point
		IUnknown* pUnk = NULL;
		if(success)
		{
			// Get pointer to manipulation event sink's IUnknown pointer
			success = SUCCEEDED(QueryInterface(IID_IUnknown, (LPVOID*)&pUnk));
		}

		// Establish connection point to callback interface
		if(success)
		{
			success = SUCCEEDED(m_pConnPoint->Advise(pUnk, &(m_uID)));
		}

		// Clean up IUnknown pointer
		if(pUnk != NULL)
		{
			pUnk->Release();
		}

		if (!success && m_pConnPoint != NULL)
		{
			m_pConnPoint->Release();
			m_pConnPoint = NULL;
		}
	}

	return success;
}

VOID CManipulationEventSink::RemoveConnPt()
{
	// Clean up the connection point associated to this event sink
	if(m_pConnPoint)
	{
		m_pConnPoint->Unadvise(m_uID);
		m_pConnPoint->Release();
		m_pConnPoint = NULL;
	}
}