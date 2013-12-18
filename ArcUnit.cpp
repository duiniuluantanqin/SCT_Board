// ArcUnit.cpp: implementation of the CArcUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "ArcUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArcUnit::CArcUnit()
{

}

CArcUnit::~CArcUnit()
{

}

void CArcUnit::ReDraw(CDC* memDC,DataInfo &pDataInfo)
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
	//pDataInfo.startAngle = 0;
	//pDataInfo.sweepAngle = 90;
	mGraphics.DrawArc(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	//mGraphics.DrawPie(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	mGraphics.ResetTransform();
}

void CArcUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	CRect rect;
	CPoint sPoint;
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
		pDataInfo.startAngle=270.0f;
		pDataInfo.sweepAngle=90.0f; 
	}
	if(xflag&&!yflag)
	{
		pDataInfo.startAngle=270.0f;
		pDataInfo.sweepAngle=-90.0f;
	}
	if(xflag&&yflag)
	{
		pDataInfo.startAngle=90.0f;
		pDataInfo.sweepAngle=90.0f; 
	}
	if(!xflag&&yflag)
	{
		pDataInfo.startAngle=90.0f;
		pDataInfo.sweepAngle=-90.0f;
	}
	if(int(pDataInfo.startAngle)==270&&int(pDataInfo.sweepAngle)==90)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=sPoint.y;
		rect.right=ePoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
	}
	if(int(pDataInfo.startAngle)==270&&int(pDataInfo.sweepAngle)==-90)
	{
		rect.left=sPoint.x;
		rect.top=sPoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
	}
	if(int(pDataInfo.startAngle)==90&&int(pDataInfo.sweepAngle)==90)
	{
		rect.left=sPoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=ePoint.y;
	}
	if(int(pDataInfo.startAngle)==90&&int(pDataInfo.sweepAngle)==-90)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=ePoint.x;
		rect.bottom=ePoint.y;
	}
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor); 
	Blue=GetBValue(pDataInfo.penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	mGraphics.DrawArc(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
}

void CArcUnit::ErasedDraw(CDC *memDC,DataInfo &pDataInfo, HRGN ErasedRgn)
{
	CRect rect;
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
	if(int(pDataInfo.startAngle)==270&&int(pDataInfo.sweepAngle)==90)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=sPoint.y;
		rect.right=ePoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
	}
	if(int(pDataInfo.startAngle)==270&&int(pDataInfo.sweepAngle)==-90)
	{
		rect.left=sPoint.x;
		rect.top=sPoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
	}
	if(int(pDataInfo.startAngle)==90&&int(pDataInfo.sweepAngle)==90)
	{
		rect.left=sPoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=ePoint.y;
	}
	if(int(pDataInfo.startAngle)==90&&int(pDataInfo.sweepAngle)==-90)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=ePoint.x;
		rect.bottom=ePoint.y;
	}
	mGraphics.DrawArc(&mPen,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	mGraphics.ResetTransform();	
}

HRGN CArcUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	CRect rect;
	CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	Graphics mGraphics(hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	if(int(pDataInfo.startAngle)==270&&int(pDataInfo.sweepAngle)==90)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=sPoint.y;
		rect.right=ePoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
	}
	if(int(pDataInfo.startAngle)==270&&int(pDataInfo.sweepAngle)==-90)
	{
		rect.left=sPoint.x;
		rect.top=sPoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=2*ePoint.y-sPoint.y;
	}
	if(int(pDataInfo.startAngle)==90&&int(pDataInfo.sweepAngle)==90)
	{
		rect.left=sPoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=2*ePoint.x-sPoint.x;
		rect.bottom=ePoint.y;
	}
	if(int(pDataInfo.startAngle)==90&&int(pDataInfo.sweepAngle)==-90)
	{
		rect.left=2*sPoint.x-ePoint.x;
		rect.top=2*sPoint.y-ePoint.y;
		rect.right=ePoint.x;
		rect.bottom=ePoint.y;
	}
	mGraphics.ResetTransform();
	GraphicsPath path(FillModeWinding);
	path.AddArc(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,pDataInfo.startAngle,pDataInfo.sweepAngle);
	Pen pen(Color(255,255,0,0),pDataInfo.penWidth+3);
	path.Widen(&pen);
	Region Rgn(&path);
	return Rgn.GetHRGN(&mGraphics);
}
