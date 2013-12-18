// RoundRectUnit.cpp: implementation of the CRoundRectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "RoundRectUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoundRectUnit::CRoundRectUnit()
{
	
}

CRoundRectUnit::~CRoundRectUnit()
{
	
}

void CRoundRectUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	int Red,Green,Blue,dx,dy;
	GraphicsPath mGraphicsPath;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
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
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	dx=ePoint.x-sPoint.x;
	dy=ePoint.y-sPoint.y;
	if(dx>dy)
	{
		RectF larc(sPoint.x,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(larc,90.0f,180.0f);
		RectF rarc(ePoint.x-dy,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(rarc,270.0f,180.0f);
		mGraphicsPath.AddArc(larc,90.0f,10.0f);
	}
	else
	{ 
		RectF tarc(sPoint.x,sPoint.y,dx,dx);
		mGraphicsPath.AddArc(tarc,180.0f,180.0f);
		RectF barc(sPoint.x,ePoint.y-dx,dx,dx);
		mGraphicsPath.AddArc(barc,0.0f,180.0f);
		mGraphicsPath.AddArc(tarc,180.0f,10.0f);
	}	
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&mGraphicsPath);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&mGraphicsPath);
	}
	mGraphics.DrawPath(&mPen,&mGraphicsPath);
	mGraphics.ResetTransform();
}

void CRoundRectUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	int dx,dy,mid; 
	int Red,Green,Blue;
	CPoint temPoint=pDataInfo.StartPoint;
	if(temPoint.x>ePoint.x)
	{
		mid=temPoint.x;
		temPoint.x=ePoint.x;
		ePoint.x=mid;
	}
	if(temPoint.y>ePoint.y)
	{
		mid=temPoint.y;
		temPoint.y=ePoint.y;
		ePoint.y=mid;
	}
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	GraphicsPath mGraphicsPath;
	dx=ePoint.x-temPoint.x;
	dy=ePoint.y-temPoint.y;
	if(dx>dy)
	{
		RectF larc(temPoint.x,temPoint.y,dy,dy);
		mGraphicsPath.AddArc(larc,90.0f,180.0f);
		RectF rarc(ePoint.x-dy,temPoint.y,dy,dy);
		mGraphicsPath.AddArc(rarc,270.0f,180.0f);
		mGraphicsPath.AddArc(larc,90.0f,10.0f);
	}
	else
	{ 
		RectF tarc(temPoint.x,temPoint.y,dx,dx);
		mGraphicsPath.AddArc(tarc,180.0f,180.0f);
		RectF barc(temPoint.x,ePoint.y-dx,dx,dx);
		mGraphicsPath.AddArc(barc,0.0f,180.0f);
		mGraphicsPath.AddArc(tarc,180.0f,10.0f);
	}	
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&mGraphicsPath);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&mGraphicsPath);
	}
	mGraphics.DrawPath(&mPen,&mGraphicsPath);
}

void CRoundRectUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
	CPoint sPoint,ePoint;
	int Red,Green,Blue,dx,dy;
	GraphicsPath mGraphicsPath;
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
	dx=ePoint.x-sPoint.x;
	dy=ePoint.y-sPoint.y;
	if(dx>dy)
	{
		RectF larc(sPoint.x,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(larc,90.0f,180.0f);
		RectF rarc(ePoint.x-dy,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(rarc,270.0f,180.0f);
		mGraphicsPath.AddArc(larc,90.0f,10.0f);
	}
	else
	{ 
		RectF tarc(sPoint.x,sPoint.y,dx,dx);
		mGraphicsPath.AddArc(tarc,180.0f,180.0f);
		RectF barc(sPoint.x,ePoint.y-dx,dx,dx);
		mGraphicsPath.AddArc(barc,0.0f,180.0f);
		mGraphicsPath.AddArc(tarc,180.0f,10.0f);
	}	
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&mGraphicsPath);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&mGraphicsPath);
	}
	mGraphics.DrawPath(&mPen,&mGraphicsPath);
	mGraphics.ResetTransform();
}

HRGN CRoundRectUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
   
	Graphics mGraphics(hDC);
	GraphicsPath mGraphicsPath;
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	int dx=ePoint.x-sPoint.x;
	int dy=ePoint.y-sPoint.y;
	if(dx>dy)
	{
		RectF larc(sPoint.x,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(larc,90.0f,180.0f);
		RectF rarc(ePoint.x-dy,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(rarc,270.0f,180.0f);
		mGraphicsPath.AddArc(larc,90.0f,10.0f);
	}
	else
	{ 
		RectF tarc(sPoint.x,sPoint.y,dx,dx);
		mGraphicsPath.AddArc(tarc,180.0f,180.0f);
		RectF barc(sPoint.x,ePoint.y-dx,dx,dx);
		mGraphicsPath.AddArc(barc,0.0f,180.0f);
		mGraphicsPath.AddArc(tarc,180.0f,10.0f);
	}	
	mGraphics.ResetTransform();
	Region iRgn(&mGraphicsPath);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth*2);
	mGraphicsPath.Widen(&pen);
    Region oRgn(&mGraphicsPath);
    if(pDataInfo.FillStyle!=0)
     oRgn.Union(&iRgn);
	return oRgn.GetHRGN(&mGraphics);
	////////////////////////////////////////
}

GraphicsPath *CRoundRectUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
    Graphics mGraphics(memDC->m_hDC);
	GraphicsPath mGraphicsPath;
	GraphicsPath path;
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	int dx=ePoint.x-sPoint.x;
	int dy=ePoint.y-sPoint.y;
	if(dx>dy)
	{
		RectF larc(sPoint.x,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(larc,90.0f,180.0f);
		RectF rarc(ePoint.x-dy,sPoint.y,dy,dy);
		mGraphicsPath.AddArc(rarc,270.0f,180.0f);
		mGraphicsPath.AddArc(larc,90.0f,10.0f);
		path.AddArc(rarc,270.0f,180.0f);
		path.AddArc(larc,90.0f,10.0f);
	}
	else
	{ 
		RectF tarc(sPoint.x,sPoint.y,dx,dx);
		mGraphicsPath.AddArc(tarc,180.0f,180.0f);
		RectF barc(sPoint.x,ePoint.y-dx,dx,dx);
		mGraphicsPath.AddArc(barc,0.0f,180.0f);
		mGraphicsPath.AddArc(tarc,180.0f,10.0f);
		path.AddArc(barc,0.0f,180.0f);
		path.AddArc(tarc,180.0f,10.0f);
	}	
	mGraphics.ResetTransform();
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth);
	mGraphicsPath.Widen(&pen);
    path.AddPath(&mGraphicsPath,true);
	return path.Clone();
}
