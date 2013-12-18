// PolylineUnit.cpp: implementation of the CPolylineUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "PolylineUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolylineUnit::CPolylineUnit()
{
	cols[0]=RGB(255,0,0);
	cols[1]=RGB(0,255,0);
	cols[2]=RGB(0,0,255);
	cols[3]=RGB(125,200,125);
	LineStyle=PencilStyle;
	ImageByte=NULL;
	ImageSize=0;
	Alpha=255;
	Region rgn(RectF(0,0,0,0));
	pURgn=rgn.Clone();
}

CPolylineUnit::~CPolylineUnit()
{
	if(ImageByte!=NULL)
		GlobalFree(ImageByte);
	delete pURgn;
}

void CPolylineUnit::DrawPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &psPoint)
{	
	if(LineStyle==RainbowStyle)
	{
		DrawRainbowPen(memDC,ePoint,psPoint,true);
		return;
	}
	if(LineStyle==TextureStyle)
	{
		DrawTexturePen(memDC,ePoint,psPoint,true);
		return;
	}
	temSavePoint addPoint;
	addPoint.point=ePoint;
	psPoint.push_back(addPoint);

	UINT Size=psPoint.size();
	if(Size<3)
		return;
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(psPoint[0].point.x,psPoint[0].point.y);
	points[1]=PointF(psPoint[1].point.x,psPoint[1].point.y);
	points[2]=PointF((psPoint[1].point.x+psPoint[2].point.x)/2,(psPoint[1].point.y+psPoint[2].point.y)/2);
	points[3]=PointF(psPoint[2].point.x,psPoint[2].point.y);
	UINT id=3;
	for(UINT Index=3;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+psPoint[Index].point.x)/2,(points[id+1].Y+psPoint[Index].point.y)/2);
		points[id+3]=PointF(psPoint[Index].point.x,psPoint[Index].point.y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);

	Graphics graphics(memDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	BYTE r=GetRValue(penColor);
	BYTE g=GetGValue(penColor);
	BYTE b=GetBValue(penColor);
	Color col=Color(Alpha,r,g,b);
	Pen PenBrush(col,penWidth);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	if(LineStyle==PaibiStyle)
	{
		Matrix matrix(cos(PI/4.0f),sin(PI/4.0f),-0.3*sin(PI/4.0f),0.3*cos(PI/4.0f),0,0);
		PenBrush.MultiplyTransform(&matrix);
	}
	graphics.DrawPath(&PenBrush,&BeziersPath);
	delete [] points;
}

void CPolylineUnit::ReDraw(CDC *memDC,DataInfo &pDataInfo)
{
	if(pDataInfo.AllRate.size()<3)
		return;
	if(pDataInfo.LineStyle==RainbowStyle)
	{
		RedrawRainbow(memDC,pDataInfo);
		return;
	}
	if(pDataInfo.LineStyle==TextureStyle)
	{
		RedrawTexture(memDC,pDataInfo);
		return;
	}
	CPoint sPoint,ePoint,tPoint,tsPoint,ptPoint;
	float xlen,ylen;
	double dx,dy;
	Matrix matrix;
	int v=0;
	int count=1;
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Color colors[]={Color(10,Red,Green,Blue)};
	Graphics graphics(memDC->m_hDC);
	graphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	graphics.RotateTransform(pDataInfo.RotateAngle);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	if(ptRgn!=NULL)
	graphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);

	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	tsPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	tsPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;
	int Size=pDataInfo.AllRate.size();
	PointF *oriPts=new PointF[Size];
	for(int Index=0;Index<Size;Index++)
	{
		oriPts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		oriPts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}	
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(oriPts[0].X,oriPts[0].Y);
	points[1]=PointF(oriPts[1].X,oriPts[1].Y);
	points[2]=PointF((oriPts[1].X+oriPts[2].X)/2,(oriPts[1].Y+oriPts[2].Y)/2);
	points[3]=PointF(oriPts[2].X,oriPts[2].Y);
	UINT id=3;
	for(UINT Index=4;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+oriPts[Index].X)/2,(points[id+1].Y+oriPts[Index].Y)/2);
		points[id+3]=PointF(oriPts[Index].X,oriPts[Index].Y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);

	BYTE r=GetRValue(pDataInfo.penColor);
	BYTE g=GetGValue(pDataInfo.penColor);
	BYTE b=GetBValue(pDataInfo.penColor);
	Color col=Color(pDataInfo.Alpha,r,g,b);
	Pen PenBrush(col,pDataInfo.penWidth);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	if(pDataInfo.LineStyle==PaibiStyle)
	{
		Matrix matrix(cos(PI/4.0f),sin(PI/4.0f),-0.3*sin(PI/4.0f),0.3*cos(PI/4.0f),0,0);
		PenBrush.MultiplyTransform(&matrix);
	}
	graphics.DrawPath(&PenBrush,&BeziersPath);
	delete [] points;
	delete [] oriPts;
	graphics.ResetTransform();
}

void CPolylineUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
	CPoint sPoint,ePoint,tPoint,tsPoint,ptPoint;
	double dx,dy;
	int v=0;
	int count=1;
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Color colors[]={Color(10,Red,Green,Blue)};
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
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);

	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	tsPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	tsPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;
	int Size=pDataInfo.AllRate.size();
	for(int Index=1;Index<Size;Index++)
	{
		tPoint.x=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		tPoint.y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
		dx=(tPoint.x-tsPoint.x);
		dy=(tPoint.y-tsPoint.y);
		float Angle=atan2(dy,dx)*Rate;
		GraphicsPath path(FillModeWinding);
		GraphicsPath path1,path2,path3,path4;
		RectF sRectF(tsPoint.x,tsPoint.y,0,0);
		sRectF.Inflate(pDataInfo.penWidth,pDataInfo.penWidth);
		RectF eRectF(tPoint.x,tPoint.y,0,0);
		eRectF.Inflate(pDataInfo.penWidth,pDataInfo.penWidth);
		///////////////////////////////////////////
		REAL  left=sRectF.GetLeft();
		REAL  top=sRectF.GetTop();
		REAL  bottom=sRectF.GetBottom();
		REAL  right=sRectF.GetRight();
		Point pts0[]={Point((left+right)/2,top),Point((left+right)/2,bottom)};
		matrix.RotateAt(Angle,PointF(tsPoint.x,tsPoint.y));
		matrix.TransformPoints(pts0,2);
		Point LeftTop(pts0[0].X,pts0[0].Y);
		Point LeftBottom(pts0[1].X,pts0[1].Y);
		path1.AddArc(sRectF,270.0f,-180.0f);
		path1.Transform(&matrix);
		matrix.Reset();

		top=eRectF.GetTop();
		bottom=eRectF.GetBottom();
		right=eRectF.GetRight();
		left=eRectF.GetLeft();
		Point pts1[]={Point((right+left)/2,bottom),Point((right+left)/2,top)};
		matrix.RotateAt(Angle,PointF(tPoint.x,tPoint.y));
		matrix.TransformPoints(pts1,2);
		Point RightBottom(pts1[0].X,pts1[0].Y);
		Point RightTop(pts1[1].X,pts1[1].Y);
		path3.AddArc(eRectF,90.0f,-180.0f);
		path3.Transform(&matrix);
		matrix.Reset();
		Point pts[]={LeftTop,RightTop,RightBottom,LeftBottom};
		///////////////////////////////////////////
		path2.AddLine(LeftBottom,RightBottom);
		path4.AddLine(RightTop,LeftTop);
		path.AddPath(&path1,true);
		path.AddPath(&path2,true);
		path.AddPath(&path3,true);
		path.AddPath(&path4,true);

		SolidBrush brush(Color(255,Red,Green,Blue));
		mGraphics.FillPath(&brush,&path);
		Pen pen(Color(255,Red,Green,Blue),1);
		mGraphics.DrawPath(&pen,&path);
		tsPoint=tPoint; 
	}
	mGraphics.ResetTransform();
}

HRGN CPolylineUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint,tPoint,tsPoint,ptPoint;
	float xlen,ylen;
	double dx,dy;
	int v=0;
	int count=1;
	Matrix matrix;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);	
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;

	tsPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	tsPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;
	GraphicsPath RgnPath(FillModeWinding);	
	int Size=pDataInfo.AllRate.size();
	for(int Index=1;Index<Size;Index++)
	{
		tPoint.x=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		tPoint.y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
		dx=(tPoint.x-tsPoint.x);
		dy=(tPoint.y-tsPoint.y);
		float Angle=atan2(dy,dx)*Rate;
		GraphicsPath path(FillModeWinding);
		GraphicsPath path1,path2,path3,path4;
		RectF sRectF(tsPoint.x,tsPoint.y,0,0);
		sRectF.Inflate(pDataInfo.penWidth+2,pDataInfo.penWidth+2);
		RectF eRectF(tPoint.x,tPoint.y,0,0);
		eRectF.Inflate(pDataInfo.penWidth+2,pDataInfo.penWidth+2);
		///////////////////////////////////////////
		REAL  left=sRectF.GetLeft();
		REAL  top=sRectF.GetTop();
		REAL  bottom=sRectF.GetBottom();
		REAL  right=sRectF.GetRight();
		Point pts0[]={Point((left+right)/2,top),Point((left+right)/2,bottom)};
		matrix.RotateAt(Angle,PointF(tsPoint.x,tsPoint.y));
		matrix.TransformPoints(pts0,2);
		Point LeftTop(pts0[0].X,pts0[0].Y);
		Point LeftBottom(pts0[1].X,pts0[1].Y);
		path1.AddArc(sRectF,270.0f,-180.0f);
		path1.Transform(&matrix);
		matrix.Reset();

		top=eRectF.GetTop();
		bottom=eRectF.GetBottom();
		right=eRectF.GetRight();
		left=eRectF.GetLeft();
		Point pts1[]={Point((right+left)/2,bottom),Point((right+left)/2,top)};
		matrix.RotateAt(Angle,PointF(tPoint.x,tPoint.y));
		matrix.TransformPoints(pts1,2);
		Point RightBottom(pts1[0].X,pts1[0].Y);
		Point RightTop(pts1[1].X,pts1[1].Y);
		path3.AddArc(eRectF,90.0f,-180.0f);
		path3.Transform(&matrix);
		matrix.Reset();
		Point pts[]={LeftTop,RightTop,RightBottom,LeftBottom};
		///////////////////////////////////////////
		path2.AddLine(LeftBottom,RightBottom);
		path4.AddLine(RightTop,LeftTop);
		path.AddPath(&path1,true);
		path.AddPath(&path2,true);
		path.AddPath(&path3,true);
		path.AddPath(&path4,true);
		RgnPath.AddPath(&path,false);
		tsPoint=tPoint; 
	}
	Graphics graphics(hDC);
	Region Rgn(&RgnPath);
	return Rgn.GetHRGN(&graphics);
}

void CPolylineUnit::DrawMultiPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &MultipsPoint,MultiInfo &GetMultiInfo,BOOL isAdd)
{
	if(LineStyle==RainbowStyle)
	{
		penWidth=GetMultiInfo.penWidth;
		DrawRainbowPen(memDC,ePoint,MultipsPoint,isAdd);
		return;
	}
	if(LineStyle==TextureStyle)
	{
		penWidth=GetMultiInfo.penWidth;
		DrawTexturePen(memDC,ePoint,MultipsPoint,isAdd);
		return;
	}
	if(isAdd)
	{
		temSavePoint addPoint;
		addPoint.point=ePoint;
		MultipsPoint.push_back(addPoint);
	}

	UINT Size=MultipsPoint.size();
	if(Size<3)
		return;
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(MultipsPoint[0].point.x,MultipsPoint[0].point.y);
	points[1]=PointF(MultipsPoint[1].point.x,MultipsPoint[1].point.y);
	points[2]=PointF((MultipsPoint[1].point.x+MultipsPoint[2].point.x)/2,(MultipsPoint[1].point.y+MultipsPoint[2].point.y)/2);
	points[3]=PointF(MultipsPoint[2].point.x,MultipsPoint[2].point.y);
	UINT id=3;
	for(UINT Index=3;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+MultipsPoint[Index].point.x)/2,(points[id+1].Y+MultipsPoint[Index].point.y)/2);
		points[id+3]=PointF(MultipsPoint[Index].point.x,MultipsPoint[Index].point.y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);

	Graphics graphics(memDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	BYTE r=GetRValue(GetMultiInfo.penColor);
	BYTE g=GetGValue(GetMultiInfo.penColor);
	BYTE b=GetBValue(GetMultiInfo.penColor);
	Color col=Color(GetMultiInfo.Alpha,r,g,b);
	Pen PenBrush(col,GetMultiInfo.penWidth);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	if(LineStyle==PaibiStyle)
	{
		Matrix matrix(cos(PI/4.0f),sin(PI/4.0f),-0.3*sin(PI/4.0f),0.3*cos(PI/4.0f),0,0);
		PenBrush.MultiplyTransform(&matrix);
	}
	graphics.DrawPath(&PenBrush,&BeziersPath);
	delete [] points;
}

void CPolylineUnit::DrawMultiPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &MultipsPoint,MultiInfo &GetMultiInfo,DWORD mouseid)
{
	/*if(LineStyle==RainbowStyle)
	{
		penWidth=GetMultiInfo.penWidth;
		DrawRainbowPen(memDC,ePoint,MultipsPoint,isAdd);
		return;
	}
	if(LineStyle==TextureStyle)
	{
		penWidth=GetMultiInfo.penWidth;
		DrawTexturePen(memDC,ePoint,MultipsPoint,isAdd);
		return;
	}*/
	/*if(isAdd)
	{
		temSavePoint addPoint;
		addPoint.point=ePoint;
		MultipsPoint.push_back(addPoint);
	}*/
	temSavePoint addPoint;
	addPoint.point = ePoint;
	MultipsPoint.push_back(addPoint);
	UINT Size=MultipsPoint.size();
	if(Size<3)
		return;
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(MultipsPoint[0].point.x,MultipsPoint[0].point.y);
	points[1]=PointF(MultipsPoint[1].point.x,MultipsPoint[1].point.y);
	points[2]=PointF((MultipsPoint[1].point.x+MultipsPoint[2].point.x)/2,(MultipsPoint[1].point.y+MultipsPoint[2].point.y)/2);
	points[3]=PointF(MultipsPoint[2].point.x,MultipsPoint[2].point.y);
	UINT id=3;
	for(UINT Index=3;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+MultipsPoint[Index].point.x)/2,(points[id+1].Y+MultipsPoint[Index].point.y)/2);
		points[id+3]=PointF(MultipsPoint[Index].point.x,MultipsPoint[Index].point.y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);

	Graphics graphics(memDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	BYTE r=GetRValue(GetMultiInfo.penColor);
	BYTE g=GetGValue(GetMultiInfo.penColor);
	BYTE b=GetBValue(GetMultiInfo.penColor);
	Color col=Color(GetMultiInfo.Alpha,r,g,b);
	Pen PenBrush(col,GetMultiInfo.penWidth);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	if(LineStyle==PaibiStyle)
	{
		Matrix matrix(cos(PI/4.0f),sin(PI/4.0f),-0.3*sin(PI/4.0f),0.3*cos(PI/4.0f),0,0);
		PenBrush.MultiplyTransform(&matrix);
	}
	graphics.DrawPath(&PenBrush,&BeziersPath);
	delete [] points;
}

void CPolylineUnit::DrawRainbowPen(CDC * memDC, CPoint ePoint, std::vector<temSavePoint> & psPoint,BOOL isAdd)
{
	if(isAdd)
	{
		temSavePoint addPoint;
		addPoint.point=ePoint;
		psPoint.push_back(addPoint);
	}
	UINT Size=psPoint.size();
	if(Size<3)
		return;
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(psPoint[0].point.x,psPoint[0].point.y);
	points[1]=PointF(psPoint[1].point.x,psPoint[1].point.y);
	points[2]=PointF((psPoint[1].point.x+psPoint[2].point.x)/2,(psPoint[1].point.y+psPoint[2].point.y)/2);
	points[3]=PointF(psPoint[2].point.x,psPoint[2].point.y);
	UINT id=3;
	for(UINT Index=4;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+psPoint[Index].point.x)/2,(points[id+1].Y+psPoint[Index].point.y)/2);
		points[id+3]=PointF(psPoint[Index].point.x,psPoint[Index].point.y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);

	Graphics graphics(memDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	BYTE r=GetRValue(cols[0]);
	BYTE g=GetGValue(cols[0]);
	BYTE b=GetBValue(cols[0]);

	Pen PenBrush(Color(Alpha,r,g,b),penWidth);
	PenBrush.SetLineJoin(LineJoinRound);
	BeziersPath.SetFillMode(FillModeWinding);

	REAL compVals[2] = {0.0f, 0.25f};
	PenBrush.SetCompoundArray(compVals,2);
	graphics.DrawPath(&PenBrush,&BeziersPath);

	compVals[0]=0.25f; 
	compVals[1]=0.5f;
	r=GetRValue(cols[1]);
	g=GetGValue(cols[1]);
	b=GetBValue(cols[1]);
	PenBrush.SetCompoundArray(compVals,2);
	PenBrush.SetColor(Color(Alpha,r,g,b));
	graphics.DrawPath(&PenBrush,&BeziersPath);

	compVals[0]=0.5f;
	compVals[1]=0.75f;
	r=GetRValue(cols[2]);
	g=GetGValue(cols[2]);
	b=GetBValue(cols[2]);
	PenBrush.SetCompoundArray(compVals,2);
	PenBrush.SetColor(Color(Alpha,r,g,b));
	graphics.DrawPath(&PenBrush,&BeziersPath);

	compVals[0]=0.75f;
	compVals[1]=1.0f;
	r=GetRValue(cols[3]);
	g=GetGValue(cols[3]);
	b=GetBValue(cols[3]);
	PenBrush.SetCompoundArray(compVals,2);
	PenBrush.SetColor(Color(Alpha,r,g,b));
	graphics.DrawPath(&PenBrush,&BeziersPath);

	delete [] points;
}

void CPolylineUnit::DrawTexturePen(CDC * memDC, CPoint ePoint, std::vector<temSavePoint> & psPoint,BOOL isAdd)
{
	if(isAdd)
	{
		temSavePoint addPoint;
		addPoint.point=ePoint;
		psPoint.push_back(addPoint);
	}
	UINT Size=psPoint.size();
	if(Size<3)
		return;
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(psPoint[0].point.x,psPoint[0].point.y);
	points[1]=PointF(psPoint[1].point.x,psPoint[1].point.y);
	points[2]=PointF((psPoint[1].point.x+psPoint[2].point.x)/2,(psPoint[1].point.y+psPoint[2].point.y)/2);
	points[3]=PointF(psPoint[2].point.x,psPoint[2].point.y);
	UINT id=3;
	for(UINT Index=4;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+psPoint[Index].point.x)/2,(points[id+1].Y+psPoint[Index].point.y)/2);
		points[id+3]=PointF(psPoint[Index].point.x,psPoint[Index].point.y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);

	Graphics graphics(memDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	IStream *pIStream;
	CreateStreamOnHGlobal(ImageByte,true,&pIStream);

	Image image(pIStream);
	TextureBrush brush(&image);
	Pen PenBrush(&brush,penWidth);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	graphics.DrawPath(&PenBrush,&BeziersPath);
	delete [] points;
}

void CPolylineUnit::RedrawRainbow(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint,tPoint,tsPoint,ptPoint;
	float xlen,ylen;
	double dx,dy;
	Matrix matrix;
	int v=0;
	int count=1;
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Color colors[]={Color(10,Red,Green,Blue)};
	Graphics graphics(memDC->m_hDC);
	graphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	graphics.RotateTransform(pDataInfo.RotateAngle);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	graphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);

	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	tsPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	tsPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;
	int Size=pDataInfo.AllRate.size();
	PointF *oriPts=new PointF[Size];
	for(int Index=0;Index<Size;Index++)
	{
		oriPts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		oriPts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}	
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(oriPts[0].X,oriPts[0].Y);
	points[1]=PointF(oriPts[1].X,oriPts[1].Y);
	points[2]=PointF((oriPts[1].X+oriPts[2].X)/2,(oriPts[1].Y+oriPts[2].Y)/2);
	points[3]=PointF(oriPts[2].X,oriPts[2].Y);
	UINT id=3;
	for(UINT Index=4;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+oriPts[Index].X)/2,(points[id+1].Y+oriPts[Index].Y)/2);
		points[id+3]=PointF(oriPts[Index].X,oriPts[Index].Y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);
	BeziersPath.SetFillMode(FillModeWinding);

	BYTE r=GetRValue(pDataInfo.penColor);
	BYTE g=GetGValue(pDataInfo.penColor);
	BYTE b=GetBValue(pDataInfo.penColor);

	Pen PenBrush(Color(Alpha,r,g,b),pDataInfo.penWidth);
	PenBrush.SetLineJoin(LineJoinRound);

	REAL compVals[2] = {0.0f, 0.25f};
	graphics.DrawPath(&PenBrush,&BeziersPath);

	r=GetRValue(pDataInfo.FillColor);
	g=GetGValue(pDataInfo.FillColor);
	b=GetBValue(pDataInfo.FillColor);

	PenBrush.SetColor(Color(Alpha,r,g,b));
	compVals[0]=0.25f; 
	compVals[1]=0.5f;
	PenBrush.SetCompoundArray(compVals,2);
	graphics.DrawPath(&PenBrush,&BeziersPath);

	r=GetRValue(pDataInfo.FontColor);
	g=GetGValue(pDataInfo.FontColor);
	b=GetBValue(pDataInfo.FontColor);

	PenBrush.SetColor(Color(Alpha,r,g,b));
	compVals[0]=0.5f; 
	compVals[1]=0.75f;
	PenBrush.SetCompoundArray(compVals,2);
	graphics.DrawPath(&PenBrush,&BeziersPath);

	r=GetRValue(pDataInfo.foreColor);
	g=GetGValue(pDataInfo.foreColor);
	b=GetBValue(pDataInfo.foreColor);

	PenBrush.SetColor(Color(Alpha,r,g,b));
	compVals[0]=0.75f; 
	compVals[1]=1.0f;
	PenBrush.SetCompoundArray(compVals,2);
	graphics.DrawPath(&PenBrush,&BeziersPath);

	delete [] points;
	delete [] oriPts;
	graphics.ResetTransform();
}

void CPolylineUnit::RedrawTexture(CDC *memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint,tPoint,tsPoint,ptPoint;
	float xlen,ylen;
	double dx,dy;
	Matrix matrix;
	int v=0;
	int count=1;
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	Color colors[]={Color(10,Red,Green,Blue)};
	Graphics graphics(memDC->m_hDC);
	graphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	graphics.RotateTransform(pDataInfo.RotateAngle);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.ScaleTransform(pDataInfo.xScale,pDataInfo.yScale);
	///////////////////////////////////
	Region tRgn;
	Region *ptRgn=tRgn.FromHRGN(pDataInfo.hRgn);
	if(ptRgn != NULL)
	graphics.ExcludeClip(ptRgn);	
	delete ptRgn;
	////////////////////////////////////
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);

	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	tsPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	tsPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;
	int Size=pDataInfo.AllRate.size();
	PointF *oriPts=new PointF[Size];
	for(int Index=0;Index<Size;Index++)
	{
		oriPts[Index].X=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		oriPts[Index].Y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
	}	
	PointF *points=new PointF[4+(Size-3)*3];
	points[0]=PointF(oriPts[0].X,oriPts[0].Y);
	points[1]=PointF(oriPts[1].X,oriPts[1].Y);
	points[2]=PointF((oriPts[1].X+oriPts[2].X)/2,(oriPts[1].Y+oriPts[2].Y)/2);
	points[3]=PointF(oriPts[2].X,oriPts[2].Y);
	UINT id=3;
	for(UINT Index=4;Index<Size;Index++)
	{
		points[id+1]=points[id];
		points[id]=points[id-1];
		points[id+2]=PointF((points[id+1].X+oriPts[Index].X)/2,(points[id+1].Y+oriPts[Index].Y)/2);
		points[id+3]=PointF(oriPts[Index].X,oriPts[Index].Y);
		id=id+3;
	}

	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(points,id+1);


	IStream *pIStream;
	CreateStreamOnHGlobal(pDataInfo.ImageByte,true,&pIStream);

	Image image(pIStream);
	TextureBrush brush(&image);
	Pen PenBrush(&brush,pDataInfo.penWidth);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	graphics.DrawPath(&PenBrush,&BeziersPath);
	delete [] points;
	delete [] oriPts;
	graphics.ResetTransform();
}