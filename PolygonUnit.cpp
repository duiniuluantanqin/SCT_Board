// PolygonUnit.cpp: implementation of the CPolygonUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "PolygonUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolygonUnit::CPolygonUnit()
{

}

CPolygonUnit::~CPolygonUnit()
{

}

void CPolygonUnit::DrawPolygon(CDC *memDC, CPoint point)
{
	int Red,Green,Blue;
	Red=GetRValue(penColor);
	Green=GetGValue(penColor);
	Blue=GetBValue(penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen pen(Color(255,Red,Green,Blue),1);
	mGraphics.DrawLine(&pen,StartPoint.x,StartPoint.y,point.x,point.y);
}

void CPolygonUnit::ReDraw(CDC *memDC, DataInfo &pDataInfo)
{
	Point *pts;
	pts=new Point[pDataInfo.PointNum];
	double xlen,ylen;
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	int Size=pDataInfo.AllRate.size();
	for(int Index=0;Index<Size;Index++)
	{
		pts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}
	int Red,Green,Blue;
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
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);		
	GraphicsPath path;
	if(pts[0].Equals(pts[Size-1]))
	{
		path.AddPolygon(pts,pDataInfo.PointNum);
		Red=GetRValue(pDataInfo.FillColor);
		Green=GetGValue(pDataInfo.FillColor);
		Blue=GetBValue(pDataInfo.FillColor);
		if(pDataInfo.FillStyle==1)
		{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&path);
		}
		else if(pDataInfo.FillStyle>1)
		{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPath(&mBrush,&path);
		}

	}
	else
		path.AddLines(pts,pDataInfo.PointNum);	
	mGraphics.DrawPath(&mPen,&path);
	mGraphics.ResetTransform();
	delete [] pts;
}

void CPolygonUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
	Point *pts;
	pts=new Point[pDataInfo.PointNum];
	double xlen,ylen;
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	int Size=pDataInfo.AllRate.size();
	for(int Index=0;Index<Size;Index++)
	{
		pts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}
	int Red,Green,Blue;
	Graphics mGraphics(memDC->m_hDC);
	///////////////////////////////////
	Matrix matrix;
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(ErasedRgn);	
	mGraphics.ExcludeClip(ptRgn);
	matrix.Reset();
	delete ptRgn;
	////////////////////////////////////
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillPolygon(&mBrush,pts,pDataInfo.PointNum);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillPolygon(&mBrush,pts,pDataInfo.PointNum);
	}
	mGraphics.DrawPolygon(&mPen,pts,pDataInfo.PointNum);
	mGraphics.ResetTransform();
	delete [] pts;
}

HRGN CPolygonUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	Point *pts;
	pts=new Point[pDataInfo.PointNum];
	double xlen,ylen;
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	int Size=pDataInfo.AllRate.size();
	for(int Index=0;Index<Size;Index++)
	{
		pts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}
	Graphics graphics(hDC);
	GraphicsPath path(FillModeWinding);
	path.AddPolygon(pts,pDataInfo.PointNum);
	GraphicsPath *ppath=path.Clone();
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth+2);
	ppath->Widen(&pen);
	Region Rgn(&path);
	Region oRgn(ppath);
	if(pDataInfo.FillStyle!=0)
		oRgn.Union(&Rgn);
	delete ppath;
	delete [] pts;
	return oRgn.GetHRGN(&graphics);	
}

GraphicsPath *CPolygonUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
	Point *pts;
	pts=new Point[pDataInfo.PointNum];
	double xlen,ylen;
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	int Size=pDataInfo.AllRate.size();
	for(int Index=0;Index<Size;Index++)
	{
		pts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}
	Graphics graphics(memDC->m_hDC);
	GraphicsPath path(FillModeWinding);
	path.AddPolygon(pts,pDataInfo.PointNum);
	GraphicsPath path1;
	path1.AddPolygon(pts,pDataInfo.PointNum);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth);
	path1.Widen(&pen);
	path.AddPath(&path1,true);
	return path.Clone();
}
