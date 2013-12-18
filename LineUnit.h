// LineUnit.h: interface for the CLineUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEUNIT_H__946F160C_AE7D_477C_AD7F_F8F2BF7ECDA7__INCLUDED_)
#define AFX_LINEUNIT_H__946F160C_AE7D_477C_AD7F_F8F2BF7ECDA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"
typedef enum enumLineStyle
{
  NormalStyle=0,
  ArrowStyle=100
}enumLineStyle;

class CLineUnit : public CBaseUnit  
{
 public:
	CLineUnit();
	virtual ~CLineUnit();
 public:
	 UINT LineStyle;
 public:
	 virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	 virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
     virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	 virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_LINEUNIT_H__946F160C_AE7D_477C_AD7F_F8F2BF7ECDA7__INCLUDED_)
