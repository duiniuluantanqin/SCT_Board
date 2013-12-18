// ArcUnit.h: interface for the CArcUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCUNIT_H__A6B081D6_F006_42C4_85EA_2048FD0C172F__INCLUDED_)
#define AFX_ARCUNIT_H__A6B081D6_F006_42C4_85EA_2048FD0C172F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CArcUnit : public CBaseUnit  
{
public:
	CArcUnit();
	virtual ~CArcUnit();
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn);
	virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	virtual void ReDraw(CDC* memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_ARCUNIT_H__A6B081D6_F006_42C4_85EA_2048FD0C172F__INCLUDED_)
