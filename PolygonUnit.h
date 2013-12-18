// PolygonUnit.h: interface for the CPolygonUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGONUNIT_H__09C90901_F76D_438F_9FD5_1E9DDBEE1DC5__INCLUDED_)
#define AFX_POLYGONUNIT_H__09C90901_F76D_438F_9FD5_1E9DDBEE1DC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CPolygonUnit : public CBaseUnit  
{
public:
	CPolygonUnit();
	virtual ~CPolygonUnit();
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
	virtual void DrawPolygon(CDC *memDC,CPoint point);
	CPoint StartPoint;
	virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_POLYGONUNIT_H__09C90901_F76D_438F_9FD5_1E9DDBEE1DC5__INCLUDED_)
