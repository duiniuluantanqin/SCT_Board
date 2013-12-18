// PolylineUnit.h: interface for the CPolylineUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYLINEUNIT_H__13B11BCA_133C_46E6_96C0_79227D30E76F__INCLUDED_)
#define AFX_POLYLINEUNIT_H__13B11BCA_133C_46E6_96C0_79227D30E76F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"
typedef enum enumPloylineStyle
{
	PencilStyle=0,
	RainbowStyle=1,
	TextureStyle=2,
	PaibiStyle=3
}enumPloylineStyle;

class CPolylineUnit : public CBaseUnit  
{
public:
	UINT LineStyle;
	COLORREF cols[4];
	BYTE *ImageByte;
	ULONG ImageSize;
	int Alpha;
	Region *pURgn;
public:
	CPolylineUnit();
	virtual ~CPolylineUnit();
public:
	virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	virtual void DrawPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &psPoint);
	virtual void ReDraw(CDC* memDC,DataInfo &pDataInfo);
private:
	void DrawRainbowPen(CDC * memDC, CPoint ePoint, std::vector<temSavePoint> & psPoint,BOOL isAdd);
	void DrawTexturePen(CDC * memDC, CPoint ePoint, std::vector<temSavePoint> & psPoint,BOOL isAdd);
	void RedrawRainbow(CDC *memDC,DataInfo &pDataInfo);
	void RedrawTexture(CDC *memDC,DataInfo &pDataInfo);
public:
	void DrawMultiPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &MultipsPoint,MultiInfo &GetMultiInfo,BOOL isAdd);
	void DrawMultiPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &MultipsPoint,MultiInfo &GetMultiInfo,DWORD mouseid);
};

#endif // !defined(AFX_POLYLINEUNIT_H__13B11BCA_133C_46E6_96C0_79227D30E76F__INCLUDED_)
