// LineUnit.cpp: implementation of the CLineUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "LineUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineUnit::CLineUnit()
{
	LineStyle=0;
}

CLineUnit::~CLineUnit()
{

}

void CLineUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	int Red,Green,Blue;
	CPoint spoint,epoint;
	spoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	epoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	if(ptRgn != NULL)
	mGraphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	Pen mPen(Color(255,Red,Green,Blue),(float)pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	if(pDataInfo.FillStyle==ArrowStyle)
		mPen.SetEndCap(LineCapArrowAnchor);
    mGraphics.DrawLine(&mPen,spoint.x,spoint.y,epoint.x,epoint.y);
	mGraphics.ResetTransform();
}

void CLineUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	if(LineStyle==ArrowStyle)
		mPen.SetEndCap(LineCapArrowAnchor);
	mGraphics.DrawLine(&mPen,pDataInfo.StartPoint.x,pDataInfo.StartPoint.y,ePoint.x,ePoint.y);
}

void CLineUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
	int Red,Green,Blue;
	CPoint spoint,epoint;
	spoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	epoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Graphics mGraphics(memDC->m_hDC); 
	///////////////////////////////////
	Matrix matrix;
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(ErasedRgn);	
	mGraphics.ExcludeClip(ptRgn);	
	matrix.Reset();
	delete ptRgn;
	////////////////////////////////////
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen mPen(Color(255,Red,Green,Blue),(float)pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	
	if(pDataInfo.FillStyle==ArrowStyle)
		mPen.SetEndCap(LineCapArrowAnchor);
	mGraphics.DrawLine(&mPen,spoint.x,spoint.y,epoint.x,epoint.y);
	mGraphics.ResetTransform();
}

HRGN CLineUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	CPoint spoint,epoint;
	spoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	epoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics graphics(hDC); 
	GraphicsPath path(FillModeWinding);
	path.AddLine(spoint.x,spoint.y,epoint.x,epoint.y);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth+2);
	if(pDataInfo.FillStyle==ArrowStyle)
	  pen.SetEndCap(LineCapArrowAnchor);
	
	path.Widen(&pen);
	Region Rgn(&path);
	return Rgn.GetHRGN(&graphics);
}
