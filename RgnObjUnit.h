// RgnObjUnit.h: interface for the CRgnObjUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGNOBJUNIT_H__21FC2246_1298_4D95_A768_C125B913C360__INCLUDED_)
#define AFX_RGNOBJUNIT_H__21FC2246_1298_4D95_A768_C125B913C360__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CRgnObjUnit : public CBaseUnit  
{
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
	CRgnObjUnit();
	virtual ~CRgnObjUnit();
	virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_RGNOBJUNIT_H__21FC2246_1298_4D95_A768_C125B913C360__INCLUDED_)
