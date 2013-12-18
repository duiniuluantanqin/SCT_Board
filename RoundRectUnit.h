// RoundRectUnit.h: interface for the CRoundRectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUNDRECTUNIT_H__4751EE8A_00A5_411D_A355_DF28DA8C8C8C__INCLUDED_)
#define AFX_ROUNDRECTUNIT_H__4751EE8A_00A5_411D_A355_DF28DA8C8C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CRoundRectUnit : public CBaseUnit  
{
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	CRoundRectUnit();
	virtual ~CRoundRectUnit();
	virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
    virtual void ReDraw(CDC* memDC,DataInfo &pDataInfo);
	virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_ROUNDRECTUNIT_H__4751EE8A_00A5_411D_A355_DF28DA8C8C8C__INCLUDED_)
