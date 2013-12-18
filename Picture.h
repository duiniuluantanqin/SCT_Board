// Picture.h : Declaration of the CPicture

#ifndef __PICTURE_H_
#define __PICTURE_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPicture
class ATL_NO_VTABLE CPicture : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPicture, &CLSID_Picture>,
	public IDispatchImpl<IPicture, &IID_IPicture, &LIBID_SCTDRAWACTIVEXLib>
{
public:
	CPicture()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PICTURE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPicture)
	COM_INTERFACE_ENTRY(IPicture)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IPicture
public:
};

#endif //__PICTURE_H_
