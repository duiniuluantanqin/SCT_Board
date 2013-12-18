// SectorUnit.cpp: implementation of the CSectorUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "SectorUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSectorUnit::CSectorUnit()
{
	
}

CSectorUnit::~CSectorUnit()
{
	
}

void CSectorUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	int Red,Green,Blue;
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
    float dx,dy;
	dx=ePoint.x-sPoint.y;
	dy=ePoint.y-sPoint.y;
	CPoint ptCenter,ptRadius;
	ptCenter.x=sPoint.x+dx*pDataInfo.AllRate[0].xRate;
	ptCenter.y=sPoint.y+dy*pDataInfo.AllRate[0].yRate;
	ptRadius.x=sPoint.x+dx*pDataInfo.AllRate[1].xRate;
	ptRadius.y=sPoint.y+dy*pDataInfo.AllRate[1].yRate;
	dx=ptRadius.x-ptCenter.x;
	dy=ptRadius.y-ptCenter.y;
	float Radius=sqrt(dx*dx+dy*dy);
    CRect rect(ptCenter.x-Radius,ptCenter.y-Radius,ptCenter.x+Radius,ptCenter.y+Radius);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPie(&mBrush,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPie(&mBrush,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	}
	mGraphics.DrawPie(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	mGraphics.ResetTransform();
}

void CSectorUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	CRect rect;
	CPoint lsPoint,lePoint,sPoint;
	int Red,Green,Blue,dx,dy;
	BOOL xflag=false;
	BOOL yflag=false;
	sPoint=pDataInfo.StartPoint;
	if(sPoint.x>ePoint.x)
	{
		dx=sPoint.x;
		sPoint.x=ePoint.x;
		ePoint.x=dx;
		xflag=true;
	}
	if(sPoint.y>ePoint.y)
	{
		dy=sPoint.y;
		sPoint.y=ePoint.y;
		ePoint.y=dy;
		yflag=true;
	}
	if(!xflag&&!yflag)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=sPoint.y;
		rect.right=ePoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
		lsPoint=sPoint;
		lePoint=ePoint;
		pDataInfo.startAngle=270.0f;
		pDataInfo.sweepAngle=90.0f;
	}
	if(xflag&&!yflag)
	{
		rect.left=sPoint.x;
		rect.top=sPoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
		lsPoint.x=ePoint.x;
		lsPoint.y=sPoint.y;
		lePoint.x=sPoint.x;
		lePoint.y=ePoint.y;
		ePoint.x=sPoint.x;
		pDataInfo.startAngle=270.0f;
		pDataInfo.sweepAngle=-90.0f;
	}
	if(xflag&&yflag)
	{
		rect.left=sPoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=ePoint.y;
		lsPoint=ePoint;
		lePoint=sPoint;
		pDataInfo.startAngle=90.0f;
		pDataInfo.sweepAngle=90.0f;
	}
	if(!xflag&&yflag)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=ePoint.x;
		rect.bottom=ePoint.y;
		lsPoint.x=sPoint.x;
		lsPoint.y=ePoint.y;
		lePoint.x=ePoint.x;
		lePoint.y=sPoint.y;
		ePoint.y=sPoint.y;
		pDataInfo.startAngle=90.0f;
		pDataInfo.sweepAngle=-90.0f;
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
		mGraphics.FillPie(&mBrush,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPie(&mBrush,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	}
	mGraphics.DrawPie(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
}

void CSectorUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
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
	 float dx,dy;
	dx=ePoint.x-sPoint.y;
	dy=ePoint.y-sPoint.y;
	CPoint ptCenter,ptRadius;
	ptCenter.x=sPoint.x+dx*pDataInfo.AllRate[0].xRate;
	ptCenter.y=sPoint.y+dy*pDataInfo.AllRate[0].yRate;
	ptRadius.x=sPoint.x+dx*pDataInfo.AllRate[1].xRate;
	ptRadius.y=sPoint.y+dy*pDataInfo.AllRate[1].yRate;
	dx=ptRadius.x-ptCenter.x;
	dy=ptRadius.y-ptCenter.y;
	float Radius=sqrt(dx*dx+dy*dy);
    CRect rect(ptCenter.x-Radius,ptCenter.y-Radius,ptCenter.x+Radius,ptCenter.y+Radius);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPie(&mBrush,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPie(&mBrush,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	}
	mGraphics.DrawPie(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	mGraphics.ResetTransform();
}

HRGN CSectorUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	Graphics graphics(hDC);
    graphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	graphics.RotateTransform(pDataInfo.RotateAngle);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
    float dx,dy;
	dx=ePoint.x-sPoint.y;
	dy=ePoint.y-sPoint.y;
	CPoint ptCenter,ptRadius;
	ptCenter.x=sPoint.x+dx*pDataInfo.AllRate[0].xRate;
	ptCenter.y=sPoint.y+dy*pDataInfo.AllRate[0].yRate;
	ptRadius.x=sPoint.x+dx*pDataInfo.AllRate[1].xRate;
	ptRadius.y=sPoint.y+dy*pDataInfo.AllRate[1].yRate;
	dx=ptRadius.x-ptCenter.x;
	dy=ptRadius.y-ptCenter.y;
	float Radius=sqrt(dx*dx+dy*dy);
    CRect rect(ptCenter.x-Radius,ptCenter.y-Radius,ptCenter.x+Radius,ptCenter.y+Radius);
	graphics.ResetTransform();
	GraphicsPath path;
	path.AddPie(rect.left,rect.top,rect.Width(),rect.Height(),pDataInfo.startAngle,pDataInfo.sweepAngle);
    Region iRgn(&path);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth*2);
	path.Widen(&pen);
	Region oRgn(&path);
	if(pDataInfo.FillStyle!=0)
		oRgn.Union(&iRgn);
	return oRgn.GetHRGN(&graphics);
}

GraphicsPath *CSectorUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
    Graphics graphics(memDC->m_hDC);
    graphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	graphics.RotateTransform(pDataInfo.RotateAngle);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	 float dx,dy;
	dx=ePoint.x-sPoint.y;
	dy=ePoint.y-sPoint.y;
	CPoint ptCenter,ptRadius;
	ptCenter.x=sPoint.x+dx*pDataInfo.AllRate[0].xRate;
	ptCenter.y=sPoint.y+dy*pDataInfo.AllRate[0].yRate;
	ptRadius.x=sPoint.x+dx*pDataInfo.AllRate[1].xRate;
	ptRadius.y=sPoint.y+dy*pDataInfo.AllRate[1].yRate;
	dx=ptRadius.x-ptCenter.x;
	dy=ptRadius.y-ptCenter.y;
	float Radius=sqrt(dx*dx+dy*dy);
    CRect rect(ptCenter.x-Radius,ptCenter.y-Radius,ptCenter.x+Radius,ptCenter.y+Radius);
	graphics.ResetTransform();
	GraphicsPath path,path1;
	path.AddPie(rect.left,rect.top,rect.Width(),rect.Height(),pDataInfo.startAngle,pDataInfo.sweepAngle);
	path1.AddPie(rect.left,rect.top,rect.Width(),rect.Height(),pDataInfo.startAngle,pDataInfo.sweepAngle);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth);
	path1.Widen(&pen);
    path.AddPath(&path1,true);
	return path.Clone();
}
