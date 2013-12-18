// SectorUnit.h: interface for the CSectorUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTORUNIT_H__17DD1715_D011_43FF_9523_BDBF2D8C723D__INCLUDED_)
#define AFX_SECTORUNIT_H__17DD1715_D011_43FF_9523_BDBF2D8C723D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CSectorUnit : public CBaseUnit  
{
public:
	CSectorUnit();
	virtual ~CSectorUnit();
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
	virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_SECTORUNIT_H__17DD1715_D011_43FF_9523_BDBF2D8C723D__INCLUDED_)
