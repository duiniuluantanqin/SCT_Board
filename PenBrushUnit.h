// PenBrushUnit.h: interface for the CPenBrushUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PENBRUSHUNIT_H__7D75D63E_2B91_49C3_ADEC_3D16B685D780__INCLUDED_)
#define AFX_PENBRUSHUNIT_H__7D75D63E_2B91_49C3_ADEC_3D16B685D780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"
#include "Bspine.h"
class CPenBrushUnit : public CBaseUnit  
{
public:
	CPenBrushUnit();
	virtual ~CPenBrushUnit();
public:
	int prelen;
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
    virtual void DrawPenBrush(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &psPoint);
	virtual void ReDraw(CDC* memDC,DataInfo &pDataInfo);
public:
	virtual void DrawMultiPenBrush(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &MultipsPoint,MultiInfo &GetMultiInfo,DWORD mouseid);
private:
	std::map<DWORD,CBspine*> m_bspline;
	std::map<DWORD,CBspine*> top_bspline;
	std::map<DWORD,CBspine*> bom_bspline;
	std::map<DWORD,FPOINT> top_fptLastEnd;
	std::map<DWORD,FPOINT> bom_fptLastEnd;
	std::map<DWORD,FPOINT> m_fptLastEnd;

	float m_fWidthHalfCircle;
	float m_fBorderInner;
	float m_fBorderOuter;
	float m_fBorderOuter2;

 	std::map<DWORD,BOOL> m_bCalCurve;
	
	CList<FPOINT_NODE> m_fptList;

	PointF pts[10];
	UINT npts;
	std::map<DWORD,float> fwidth;
private:
	void PushStart(CPoint point,DWORD mouseid);
	BOOL Push(CPoint sPoint,CPoint ePoint,DWORD mouseid);
	void CalcPointsAndConnectLine(DWORD mouseid);
	float SetWidthCircleBrush( float fWidth);
	void CalcBsplineMidPoint( POSITION psLeft, POSITION psRight,PointF &pts1,PointF &pts2,UINT &flag,DWORD mouseid);
};

#endif // !defined(AFX_PENBRUSHUNIT_H__7D75D63E_2B91_49C3_ADEC_3D16B685D780__INCLUDED_)
