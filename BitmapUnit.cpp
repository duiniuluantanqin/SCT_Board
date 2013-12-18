// BitmapUnit.cpp: implementation of the CBitmapUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "BitmapUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapUnit::CBitmapUnit()
{
	
}

CBitmapUnit::~CBitmapUnit()
{
	
}

void CBitmapUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	CPoint sPoint=pDataInfo.StartPoint;
	int tem;
	if(sPoint.x>ePoint.x)
	{
		tem=sPoint.x;
		sPoint.x=ePoint.x;
		ePoint.x=tem;
	}
	if(sPoint.y>ePoint.y)
	{
		tem=sPoint.y;
		sPoint.y=ePoint.y;
		ePoint.y=tem;
	}
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	mGraphics.SetInterpolationMode(InterpolationModeHighQuality);
	int Width=ePoint.x-sPoint.x;
	int Height=ePoint.y-sPoint.y;
	
	IStream *pIStream;
	CreateStreamOnHGlobal(pDataInfo.ImageByte,true,&pIStream);
	Image image(pIStream);
	mGraphics.DrawImage(&image,sPoint.x,sPoint.y,Width,Height);
}

void CBitmapUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	CString BmpName;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	int Width=ePoint.x-sPoint.x;
	int Height=ePoint.y-sPoint.y;
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
	IStream *pIStream;
	CreateStreamOnHGlobal(pDataInfo.ImageByte,true,&pIStream);
	Image image(pIStream);
	mGraphics.DrawImage(&image,sPoint.x,sPoint.y,Width,Height);
	mGraphics.ResetTransform();
}

HRGN CBitmapUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	GraphicsPath path(FillModeWinding);
	path.AddRectangle(Rect(sPoint.x-2,sPoint.y-2,ePoint.x-sPoint.x+4,ePoint.y-sPoint.y+4));
	Region Rgn(&path);
	Graphics mGraphics(hDC);
	return Rgn.GetHRGN(&mGraphics);
}

void CBitmapUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo, HRGN ErasedRgn)
{
    CPoint sPoint,ePoint;
	CString BmpName;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	int Width=ePoint.x-sPoint.x;
	int Height=ePoint.y-sPoint.y;
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
	mGraphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	IStream *pIStream;
	CreateStreamOnHGlobal(pDataInfo.ImageByte,true,&pIStream);
	Image image(pIStream);
	mGraphics.DrawImage(&image,sPoint.x,sPoint.y,Width,Height);
	mGraphics.ResetTransform();
}
