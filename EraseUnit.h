// EraseUnit.h: interface for the CEraseUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERASEUNIT_H__14FFC7E0_018D_4827_AE25_0562C4110752__INCLUDED_)
#define AFX_ERASEUNIT_H__14FFC7E0_018D_4827_AE25_0562C4110752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"
#include "LineUnit.h"
#include "RectUnit.h"
#include "RoundRectUnit.h"
#include "EllipseUnit.h"
#include "ArcUnit.h"
#include "SectorUnit.h"
#include "PolylineUnit.h"
#include "PenBrushUnit.h"
#include "TextUnit.h"
#include "BitmapUnit.h"
#include "Combination.h"
//#include  "SCTDrawActivexCtl.h"
class CEraseUnit : public CBaseUnit  
{
public:
	CEraseUnit();
	virtual ~CEraseUnit();
public:
	Image *pErasureImage;
private:
	
    PointF *ptArray;
	UINT ptArraySize;
	CLineUnit *ptLineUnit;
    CRectUnit *ptRectUnit;
    CRoundRectUnit *ptRoundRectUnit;
    CEllipseUnit *ptEllipseUnit;
    CArcUnit *ptArcUnit;
    CSectorUnit *ptSectorUnit;
    CPolylineUnit *ptPolylineUnit;
    CPenBrushUnit *ptPenBrushUnit;
    CTextUnit *ptTextUnit;
    CBitmapUnit *ptBmpUnit;
	CCombination *ptCombination;
public:
	// HANDLE hConsole;
	//void DisplayConsole(char *info,int size);
	CPoint sPoint;
	CPoint ePoint;
public:
    HRGN EraseOperation(HDC m_hDC,UINT PenWidth);
	BOOL OnMouseDown(UINT nFlags, CPoint point);
    BOOL OnMouseMove(UINT nFlags, CPoint point);
	BOOL OnMouseUp(UINT nFlags, CPoint point);
	void OnRender(CDC *pDC,UINT PenWidth);
};

#endif // !defined(AFX_ERASEUNIT_H__14FFC7E0_018D_4827_AE25_0562C4110752__INCLUDED_)
