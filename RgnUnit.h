// RgnUnit.h: interface for the CRgnUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGNUNIT_H__5EDE2713_4F6E_4E54_AAA7_2C3DB633BC79__INCLUDED_)
#define AFX_RGNUNIT_H__5EDE2713_4F6E_4E54_AAA7_2C3DB633BC79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CRgnUnit : public CBaseUnit  
{
public:
	CRgnUnit();
	virtual ~CRgnUnit();

public:
	void ReDraw(CDC *memDC, DataInfo &pDataInfo);
	HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
};

#endif // !defined(AFX_RGNUNIT_H__5EDE2713_4F6E_4E54_AAA7_2C3DB633BC79__INCLUDED_)
