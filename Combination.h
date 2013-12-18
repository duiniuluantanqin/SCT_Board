// Combination.h: interface for the CCombination class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBINATION_H__C4237FF2_CBC0_44EC_B356_0C24A162C16B__INCLUDED_)
#define AFX_COMBINATION_H__C4237FF2_CBC0_44EC_B356_0C24A162C16B__INCLUDED_

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
#include "PolygonUnit.h"
#include "RgnObjUnit.h"

class CCombination : public CBaseUnit  
{
public:
 CBaseUnit *ptBaseUnit;
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
 CPolygonUnit *ptPolygonUnit;
 CRgnObjUnit *ptRgnUnit;
public:
	CCombination();
	virtual ~CCombination();
public:
	virtual void ReDraw(CDC *memDC,std::vector<DataInfo> DataArray,std::map<int,int> ObjList);
	void	GetCombinatedObjs(std::vector<DataInfo> DataArray,DataInfo pDataInfo,std::map<int,int> &ObjList);
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ModifyCombinatedObjs(std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
};

#endif // !defined(AFX_COMBINATION_H__C4237FF2_CBC0_44EC_B356_0C24A162C16B__INCLUDED_)
