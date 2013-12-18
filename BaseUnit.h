// BaseUnit.h: interface for the CBaseUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEUNIT_H__71986F8E_A573_48A1_9DCF_BF4A1D93EB2C__INCLUDED_)
#define AFX_BASEUNIT_H__71986F8E_A573_48A1_9DCF_BF4A1D93EB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"
#include <math.h>
#include <time.h>
#define PI 3.1415926
#define Rate 57.2957805

typedef struct MultiInfo
{
  int prelen;
  CPoint StartPoint;
  COLORREF penColor;
  int penWidth;
  Region *pURgn;
  UINT Alpha;
  DWORD timer;
}MultiInfo;

typedef struct ChartData
{ 
  char *name;
  float number;
  COLORREF color;
  int CharNum;
}ChartData;

typedef struct Chart
{
	int Type;
	char *HeadLine;
	int CharNum;
	COLORREF bkColor;
	std::vector<ChartData> ChartDataArray;
}Chart;

typedef struct RelRate
{
 float xRate;
 float yRate;
 int preWidth;
 int alpha;
}RelRate;//相对储存开始点的比率

typedef struct ComInfo
{
 int ObjIndex;
 double OffsetAngle;
 float xStartRate;
 float yStartRate;
 float xEndRate;
 float yEndRate;
 float xCenterRate;
 float yCenterRate;
 float xScale;
 float yScale;
}ComInfo;

typedef struct DataInfo
{
 int OperationKind;
 BOOL IsCombination;//是否为组合体一部分
 BOOL IsDisjoint;//是否被解组
 BOOL IsErased;//是否需要重绘
 BOOL IsLocked;//是否被锁定
// BOOL IsSaved; //用于智能板擦
 CPoint StartPoint;
 CPoint EndPoint;
 HRGN hRgn;//已擦除区域
 GraphicsPath *pFillPath;//填充区域对象
 GraphicsPath *pOutlinePath;//轮廓区域对象
 float xScale;
 float yScale;//镜像绘制时参数
 int penWidth;
 double preAngle;//弧度
 double RotateAngle;//角度
 float startAngle;//弧的起始角度
 float sweepAngle;//弧的跨度
 CPoint CenterPoint;//旋转时的坐标原点
 BYTE *ImageByte;
 int ImageSize;
 char *SaveStr;
 int charNum;
 LOGFONTW logfontw;
 Chart ChartInfo;
 std::vector<RelRate> AllRate;
 std::vector<ComInfo> ComArray;//被合并对像信息集
 int  PointNum;//手绘时的坐标个数
 int LineStyle;
 int FillStyle;
 int Alpha;
 COLORREF foreColor;//填充时的线条颜色
 COLORREF penColor;
 COLORREF FillColor;
 COLORREF FontColor;
 COLORREF SelectColor;
 char Reserved0[200];
 char Reserved1[200];
 char Reserved2[200];
 char Reserved3[200];
 char Reserved4[1024];
}DataInfo;

typedef struct temSavePoint
{
 CPoint point;
 int preWidth;
 int alpha;
}temSavePoint;//暂存画笔经过的点

class CBaseUnit  
{
 public:
  CPoint Rel_StartPoint;
  CPoint Rel_EndPoint;
  CPoint Rel_CenterPoint;
  LOGFONTW Rel_logfontw;
  CPoint StartPoint;
  COLORREF penColor;
  COLORREF SelectColor;
  int penWidth;
  float arcAngle;
  float tsweepAngle;
  std::vector<RelRate> pRate;
  RectF RotateRect;
  RectF StretchRect;
  RectF MenuRect;
  RectF MoveRect;
  HCURSOR MouseCur;
  int xlen;
  int ylen;//用于字体拉伸
 
 public:
	 CBaseUnit();
	 virtual ~CBaseUnit();
 public:
	 CPoint CaculatePoint(CPoint point,DataInfo pDataInfo);
	 virtual void ReDraw(CDC *memDC,DataInfo &pDataInfo);
	 virtual void ReDraw(CDC *memDC,std::vector<DataInfo> &DataArray, DataInfo &pDataInfo);
 public:
     virtual void DrawPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> psPoint);

 public:
	 virtual void DrawPenBrush(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> psPoint);

 private:
	
 public:
	 virtual HRGN GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo);
	 virtual void ErasedDraw(CDC *memDC,DataInfo &pDataInfo,HRGN ErasedRgn);
	 void SetHatchStyle(HatchStyle &hatchStyle,int FillStyle);
	 void SetLineStyle(Pen &pen,int LineStyle);
	 virtual int ptInRow(CDC *memDC,CPoint point,DataInfo &pDataInfo,CString Text);
	 virtual int ptInLine(CDC *memDC,CPoint point,DataInfo &pDataInfo);
	 virtual void DrawText(CDC *memDC,DataInfo &pDataInfo,CString Text);
	 virtual void DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint);
	 virtual void DrawPolygon(CDC *memDC,CPoint point);
	 virtual GraphicsPath *GetObjPath(CDC *memDC,DataInfo &pDataInfo);
};

#endif // !defined(AFX_BASEUNIT_H__71986F8E_A573_48A1_9DCF_BF4A1D93EB2C__INCLUDED_)
