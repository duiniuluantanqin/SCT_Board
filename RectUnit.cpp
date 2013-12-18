// RectUnit.cpp: implementation of the CRectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "RectUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectUnit::CRectUnit()
{
	
}

CRectUnit::~CRectUnit()
{
	
}

void CRectUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	int Red,Green,Blue;
	double angle= pDataInfo.RotateAngle;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(angle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
    ///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	if(ptRgn != NULL)
	mGraphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillRectangle(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillRectangle(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	mGraphics.DrawRectangle(&mPen,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	mGraphics.ResetTransform();
}

void CRectUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	int Red,Green,Blue,mid;
	CPoint temPoint=pDataInfo.StartPoint;
	if(temPoint.x>ePoint.x)
	{
		mid=ePoint.x;
		ePoint.x=temPoint.x;
		temPoint.x=mid;
	}
	if(temPoint.y>ePoint.y)
	{
		mid=ePoint.y;
		ePoint.y=temPoint.y;
		temPoint.y=mid;
	}
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillRectangle(&mBrush,temPoint.x,temPoint.y,ePoint.x-temPoint.x,ePoint.y-temPoint.y);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillRectangle(&mBrush,temPoint.x,temPoint.y,ePoint.x-temPoint.x,ePoint.y-temPoint.y);
	}
	mGraphics.DrawRectangle(&mPen,temPoint.x,temPoint.y,ePoint.x-temPoint.x,ePoint.y-temPoint.y);
}

void CRectUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
 	
	CPoint sPoint,ePoint;
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
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
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillRectangle(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillRectangle(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	mGraphics.DrawRectangle(&mPen,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	mGraphics.ResetTransform();
}

HRGN CRectUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
    CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics graphics(hDC);
	GraphicsPath path(FillModeAlternate);
	if(pDataInfo.FillStyle==0)
	{
	 path.AddRectangle(Rect(sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y));
	 Pen pen(Color(255,0,0,0),pDataInfo.penWidth*2);
	 path.Widen(&pen);
	}
	else
      path.AddRectangle(Rect(sPoint.x-pDataInfo.penWidth,sPoint.y-pDataInfo.penWidth,ePoint.x-sPoint.x+2*pDataInfo.penWidth,ePoint.y-sPoint.y+2*pDataInfo.penWidth));
    Region Rgn(&path);
    return Rgn.GetHRGN(&graphics);
}

GraphicsPath *CRectUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
    CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics graphics(memDC->m_hDC);
	GraphicsPath path(FillModeAlternate);
    path.AddRectangle(Rect(sPoint.x-pDataInfo.penWidth/2,sPoint.y-pDataInfo.penWidth/2,ePoint.x-sPoint.x+pDataInfo.penWidth,ePoint.y-sPoint.y+pDataInfo.penWidth));
    return path.Clone();
}
