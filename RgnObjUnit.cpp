// RgnObjUnit.cpp: implementation of the CRgnObjUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "RgnObjUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRgnObjUnit::CRgnObjUnit()
{
	
}

CRgnObjUnit::~CRgnObjUnit()
{
	
}

void CRgnObjUnit::ReDraw(CDC *memDC, DataInfo &pDataInfo)
{
	Graphics graphics(memDC->m_hDC);	
    graphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	graphics.RotateTransform(pDataInfo.RotateAngle);
	graphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	if(ptRgn != NULL)
	graphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.FillColor);
	Green=GetGValue(pDataInfo.FillColor);
	Blue=GetBValue(pDataInfo.FillColor);
	if(pDataInfo.FillStyle==1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		graphics.FillPath(&mBrush,pDataInfo.pFillPath);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		graphics.FillPath(&mBrush,pDataInfo.pFillPath);
	}
    Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Pen mPen(Color(255,Red,Green,Blue),pDataInfo.penWidth/2);
	SetLineStyle(mPen,pDataInfo.LineStyle);
	graphics.DrawPath(&mPen,pDataInfo.pOutlinePath);
	graphics.ResetTransform();
}

HRGN CRgnObjUnit::GetRedrawHRGN(HDC hDC,std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	Graphics graphics(hDC);
	GraphicsPath path;
	path.AddPath(pDataInfo.pOutlinePath,true);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth/2);
	path.Widen(&pen);		
    if(pDataInfo.FillStyle>0)
     path.AddPath(pDataInfo.pFillPath,true);
    Region Rgn(&path);
	return Rgn.GetHRGN(&graphics);

}

GraphicsPath *CRgnObjUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
    Graphics graphics(memDC->m_hDC);
	GraphicsPath path;
	path.AddPath(pDataInfo.pOutlinePath,true);
	Pen pen(Color(255,0,0,0),pDataInfo.penWidth/2);
	path.Widen(&pen);	
    path.AddPath(pDataInfo.pFillPath,true);
	return path.Clone();
}
