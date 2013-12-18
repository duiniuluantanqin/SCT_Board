// TextUnit.h: interface for the CTextUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTUNIT_H__619B5013_5776_489D_B71E_0981E09BF289__INCLUDED_)
#define AFX_TEXTUNIT_H__619B5013_5776_489D_B71E_0981E09BF289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"

class CTextUnit : public CBaseUnit  
{
public:
	CTextUnit();
	virtual ~CTextUnit();
public:
	void ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn);
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual int ptInRow(CDC *memDC,CPoint point,DataInfo &pDataInfo,CString Text);
	virtual int ptInLine(CDC *memDC,CPoint point,DataInfo &pDataInfo);
	virtual void DrawText(CDC *memDC,DataInfo &pDataInfo,CString Text);
    virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_TEXTUNIT_H__619B5013_5776_489D_B71E_0981E09BF289__INCLUDED_)
