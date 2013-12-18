// RgnUnit.cpp: implementation of the CRgnUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "RgnUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRgnUnit::CRgnUnit()
{

}

CRgnUnit::~CRgnUnit()
{

}

void CRgnUnit::ReDraw(CDC *memDC, DataInfo &pDataInfo)
{
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
    ///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	mGraphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	int Red=GetRValue(pDataInfo.FillColor);
	int Green=GetGValue(pDataInfo.FillColor);
	int Blue=GetBValue(pDataInfo.FillColor);
	Region Rgn(pDataInfo.hDrawRgn);
	if(pDataInfo.FillStyle<=1)
	{
		SolidBrush mBrush(Color(255,Red,Green,Blue));
		mGraphics.FillRegion(&mBrush,&Rgn);
	}
	else if(pDataInfo.FillStyle>1)
	{
		HatchStyle hatchStyle;
		SetHatchStyle(hatchStyle,pDataInfo.FillStyle);
		int foreRed=GetRValue(pDataInfo.foreColor);
		int foreGreen=GetGValue(pDataInfo.foreColor);
		int foreBlue=GetBValue(pDataInfo.foreColor);
		HatchBrush mBrush(hatchStyle,Color(255,foreRed,foreGreen,foreBlue),Color(255,Red,Green,Blue));
		mGraphics.FillRegion(&mBrush,&Rgn);
	}
	mGraphics.ResetTransform();
}

HRGN CRgnUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
 return NULL;
}