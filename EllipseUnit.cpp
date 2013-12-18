// EllipseUnit.cpp: implementation of the CEllipseUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "EllipseUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipseUnit::CEllipseUnit()
{
	
}

CEllipseUnit::~CEllipseUnit()
{
	
}

void CEllipseUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	int Red,Green,Blue;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
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
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	mGraphics.DrawEllipse(&mPen,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	mGraphics.ResetTransform();
}

void CEllipseUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
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
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&mBrush,temPoint.x,temPoint.y,ePoint.x-temPoint.x,ePoint.y-temPoint.y);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&mBrush,temPoint.x,temPoint.y,ePoint.x-temPoint.x,ePoint.y-temPoint.y);
	}
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	mGraphics.DrawEllipse(&mPen,temPoint.x,temPoint.y,ePoint.x-temPoint.x,ePoint.y-temPoint.y);
	int xCenter=(ePoint.x+temPoint.x)/2;
	int yCenter=(ePoint.y+temPoint.y)/2;
	mGraphics.DrawLine(&Pen(Color(255,255,0,0),1),xCenter,yCenter,xCenter,temPoint.y);
	mGraphics.DrawLine(&Pen(Color(255,255,0,0),1),xCenter,yCenter,ePoint.x,yCenter);
	////////////////////////////draw headline//////////////////////////////////////////////////
	CString str;
	str.Format("%d Pixel",(ePoint.x-temPoint.x)/2);
	Font myFont(L"Arial", 14);
	mGraphics.DrawString(str.AllocSysString(),str.GetLength(),&myFont,PointF(xCenter+(ePoint.x-temPoint.x)/4,yCenter+3),NULL,&SolidBrush(Color(255,0,0,0)));
	str.Format("%d Pixel",(ePoint.y-temPoint.y)/2);
	mGraphics.DrawString(str.AllocSysString(),str.GetLength(),&myFont,PointF(xCenter+3,yCenter-(ePoint.y-temPoint.y)/4),NULL,&SolidBrush(Color(255,0,0,0)));
	///////////////////////////draw item name list////////////////////////////////////////////
}

void CEllipseUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
	CPoint sPoint,ePoint;
	int Red,Green,Blue;
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
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&mBrush,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	}
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	mGraphics.DrawEllipse(&mPen,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	mGraphics.ResetTransform();
}

HRGN CEllipseUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
    CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics graphics(hDC);
	GraphicsPath path(FillModeAlternate);
	if(pDataInfo.FillStyle==0)
	{
		path.AddEllipse(sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
		Pen pen(Color(255,0,0,0),pDataInfo.penWidth+2);
		path.Widen(&pen);
	}
	else
		path.AddEllipse(Rect(sPoint.x-pDataInfo.penWidth,sPoint.y-pDataInfo.penWidth,ePoint.x-sPoint.x+2*pDataInfo.penWidth,ePoint.y-sPoint.y+2*pDataInfo.penWidth));
    Region Rgn(&path);
    return Rgn.GetHRGN(&graphics);
}

GraphicsPath *CEllipseUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics graphics(memDC->m_hDC);
	GraphicsPath path(FillModeAlternate);
    path.AddEllipse(Rect(sPoint.x-pDataInfo.penWidth/2,sPoint.y-pDataInfo.penWidth/2,ePoint.x-sPoint.x+pDataInfo.penWidth,ePoint.y-sPoint.y+pDataInfo.penWidth));
    return path.Clone();
}
