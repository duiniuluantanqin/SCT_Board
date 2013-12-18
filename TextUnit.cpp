// TextUnit.cpp: implementation of the CTextUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "TextUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextUnit::CTextUnit()
{
	
}

CTextUnit::~CTextUnit()
{
	
}

void CTextUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint;
	int len;
	CString temStr;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	temStr=(CString)pDataInfo.SaveStr;
	len=temStr.GetLength();
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	if(ptRgn != NULL)
	mGraphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=temStr.AllocSysString();
	PointF pointf(sPoint.x,sPoint.y);
	RectF rectf;
	const StringFormat *format=StringFormat::GenericTypographic();
	Font temFont(memDC->m_hDC,&pDataInfo.logfontw);
	mGraphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
	int Red=GetRValue(pDataInfo.FontColor);
	int Green=GetGValue(pDataInfo.FontColor);
	int Blue=GetBValue(pDataInfo.FontColor);
	SolidBrush  mBrush(Color(255,Red,Green,Blue));
	mGraphics.DrawString(WideStr,wcslen(WideStr),&temFont,pointf,format, &mBrush);
	mGraphics.ResetTransform();
	SysFreeString(WideStr);
}

void CTextUnit::DrawText(CDC *memDC,DataInfo &pDataInfo,CString Text)
{
	RectF rectf;
	wchar_t *WideStr;
	WideStr=new wchar_t[Text.GetLength()];
	WideStr=Text.AllocSysString();
	PointF pointf(Rel_StartPoint.x,Rel_StartPoint.y);
	const StringFormat *format=StringFormat::GenericTypographic();
	Graphics mGraphics(memDC->m_hDC);
	Font font(memDC->m_hDC,&pDataInfo.logfontw);
	mGraphics.MeasureString(WideStr,wcslen(WideStr),&font,pointf,format,&rectf);
	Rel_EndPoint.x=Rel_StartPoint.x+rectf.Width;
	Rel_EndPoint.y=Rel_StartPoint.y+rectf.Height;
	int Red=GetRValue(pDataInfo.FontColor);
	int Green=GetGValue(pDataInfo.FontColor);
	int Blue=GetBValue(pDataInfo.FontColor);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	SolidBrush  mBrush(Color(255,Red,Green,Blue));
	mGraphics.DrawString(WideStr,wcslen(WideStr),&font,pointf,format, &mBrush);
	SysFreeString(WideStr);
	double len=sqrt(float(Rel_EndPoint.x*Rel_EndPoint.x)+float(Rel_EndPoint.y*Rel_EndPoint.y));
	double Angle=atan2(float(Rel_EndPoint.y),float(Rel_EndPoint.x))+pDataInfo.RotateAngle/Rate;
	pDataInfo.EndPoint.x=pDataInfo.CenterPoint.x+len*cos(Angle);
	pDataInfo.EndPoint.y=pDataInfo.CenterPoint.y+len*sin(Angle);
	Rel_CenterPoint.x=(Rel_StartPoint.x+Rel_EndPoint.x)/2;
	Rel_CenterPoint.y=(Rel_StartPoint.y+Rel_EndPoint.y)/2;
}

int CTextUnit::ptInLine(CDC *memDC,CPoint point,DataInfo &pDataInfo)
{
	int pos;
	float fontHeight;
	CPoint pt=CaculatePoint(point,pDataInfo);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	int Offset;
	Offset=pt.y-sPoint.y;
	Font font(memDC->m_hDC,&pDataInfo.logfontw);
	Graphics mGraphics(memDC->m_hDC);
	fontHeight=font.GetHeight(&mGraphics);
	pos=Offset/fontHeight;
	return pos;
}

int CTextUnit::ptInRow(CDC *memDC, CPoint point, DataInfo &pDataInfo, CString Text)
{
	RectF rectf;
	wchar_t *WideStr;
	float min,value;
	int flag,preflag;//ֵΪ-1,0,1
	CString str=Text;
	int StrLen=str.GetLength();
	int pos;
	CPoint pt=CaculatePoint(point,pDataInfo);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	int Offset=pt.x-sPoint.x;
	PointF pointf(sPoint.x,sPoint.y);
	const StringFormat *format=StringFormat::GenericTypographic();
	Graphics mGraphics(memDC->m_hDC);
	Font font(memDC->m_hDC,&pDataInfo.logfontw);
	//////////////////////////////////////////////////////////////////
	value=0-Offset;
	if(value<0)
	{
		flag=-1;
		preflag=-1;
	}
	if(value==0)
	{
		flag=0;
		flag=0;
	}
	min=fabs(value);
	pos=0;
	for(int i=1;i<=StrLen;i++)
	{
		Text=str.Left(i);
		WideStr=new wchar_t[Text.GetLength()];
		WideStr=Text.AllocSysString();
		mGraphics.MeasureString(WideStr,wcslen(WideStr),&font,pointf,format,&rectf);
		value=rectf.Width-Offset;
		if(value>0)
			flag=1;
		if(value<0)
			flag=-1;
		if(value==0)
			flag=0;
		if(min>fabs(value))
		{
			min=fabs(value);
			pos=i;
		}
		SysFreeString(WideStr);
		if(preflag!=flag)
			break;
	}
	//////////////////////////////////////////////////////////////////////////////////
	return pos;
}


HRGN CTextUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
    CPoint sPoint,ePoint;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	Graphics graphics(hDC);
    wchar_t *WideStr;
	CString temStr;
	temStr=(CString)pDataInfo.SaveStr;
	int len=temStr.GetLength();
    WideStr=new wchar_t[len];
	WideStr=temStr.AllocSysString();
	PointF pointf(sPoint.x,sPoint.y);
	RectF rectf;
	const StringFormat *format=StringFormat::GenericTypographic();
	Font temFont(hDC,&pDataInfo.logfontw);
	graphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
	SysFreeString(WideStr);
	GraphicsPath path(FillModeAlternate);
    rectf.Inflate(4,4);
	path.AddRectangle(rectf);
    Region Rgn(&path);
    return Rgn.GetHRGN(&graphics);
}

void CTextUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo, HRGN ErasedRgn)
{
	CPoint sPoint;
	int len;
	CString temStr;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	temStr=(CString)pDataInfo.SaveStr;
	len=temStr.GetLength();
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
	mGraphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=temStr.AllocSysString();
	PointF pointf(sPoint.x,sPoint.y);
	RectF rectf;
	const StringFormat *format=StringFormat::GenericTypographic();
	Font temFont(memDC->m_hDC,&pDataInfo.logfontw);
	mGraphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
	int Red=GetRValue(pDataInfo.FontColor);
	int Green=GetGValue(pDataInfo.FontColor);
	int Blue=GetBValue(pDataInfo.FontColor);
	SolidBrush  mBrush(Color(255,Red,Green,Blue));
	mGraphics.DrawString(WideStr,wcslen(WideStr),&temFont,pointf,format, &mBrush);
	mGraphics.ResetTransform();
	SysFreeString(WideStr);
}
