// PenBrushUnit.cpp: implementation of the CPenBrushUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "PenBrushUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPenBrushUnit::CPenBrushUnit()
{

}

CPenBrushUnit::~CPenBrushUnit()
{

}

void CPenBrushUnit::DrawPenBrush(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &psPoint)
{
	int Red,Green,Blue;	
	Red=GetRValue(penColor);
	Green=GetGValue(penColor);
	Blue=GetBValue(penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	SolidBrush brush(Color(255,Red,Green,Blue));
	UINT Size=psPoint.size();
	CPoint sPoint=psPoint[Size-1].point;
	if(Size==1)
	{
		if(fwidth.find(0)!=fwidth.end())
			fwidth.erase(0);
		float width=psPoint[0].preWidth;
		fwidth.insert(std::map<DWORD,float>::value_type(0,width));
		PushStart(ePoint,0);	
	}
	if(Push(sPoint,ePoint,0))
	{
		RectF headRect;
		RectF tailRect;
		float width,dx,dy;
		PointF lfCenter,rtCenter;
		mGraphics.FillPolygon(&brush,pts,npts);
		mGraphics.DrawPolygon(&Pen(Color(255,Red,Green,Blue),0.5f),pts,npts);
		if(npts==4)
		{
			headRect=RectF((pts[0].X+pts[3].X)/2.0f,(pts[0].Y+pts[3].Y)/2.0f,0.0f,0.0f);
			tailRect=RectF((pts[1].X+pts[2].X)/2.0f,(pts[1].Y+pts[2].Y)/2.0f,0.0f,0.0f);
			dx=pts[3].X-pts[0].X;
			dy=pts[3].Y-pts[0].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			headRect.Inflate(width,width);
			dx=pts[2].X-pts[1].X;
			dy=pts[2].Y-pts[1].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			tailRect.Inflate(width,width);
		}
		else
		{
			headRect=RectF((pts[0].X+pts[9].X)/2.0f,(pts[0].Y+pts[9].Y)/2.0f,0.0f,0.0f);
			tailRect=RectF((pts[4].X+pts[5].X)/2.0f,(pts[4].Y+pts[5].Y)/2.0f,0.0f,0.0f);
			dx=pts[9].X-pts[0].X;
			dy=pts[9].Y-pts[0].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			headRect.Inflate(width,width);
			dx=pts[5].X-pts[4].X;
			dy=pts[5].Y-pts[4].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			tailRect.Inflate(width,width);
		}
		brush.SetColor(Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&brush,headRect);
		mGraphics.FillEllipse(&brush,tailRect);
		temSavePoint addPoint;
		addPoint.point=ePoint;
		addPoint.alpha=psPoint[Size-1].alpha;
		addPoint.preWidth=psPoint[Size-1].preWidth;
		psPoint.push_back(addPoint);
	}
}

void CPenBrushUnit::ReDraw(CDC* memDC,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint,psPoint,pePoint,ptPoint;	
	double dx,dy,xlen,ylen;
	double preWidth,nWidth;
	Matrix matrix;
	int count=1;
	int alpha;
	int v=0;
	int Red,Green,Blue;
	Red=GetRValue(pDataInfo.penColor);
	Green=GetGValue(pDataInfo.penColor);
	Blue=GetBValue(pDataInfo.penColor);
	SolidBrush brush(Color(255,Red,Green,Blue));
	SolidBrush brush1(Color(200,Red,Green,Blue));
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
	int Size=pDataInfo.AllRate.size();
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	if(xlen<1)
		xlen=1;
	if(ylen<1)
		ylen=1;
	if(fwidth.size()!=0)
		fwidth.clear();
	fwidth.insert(std::map<DWORD,float>::value_type(0,pDataInfo.AllRate[0].preWidth));
	alpha=pDataInfo.AllRate[0].alpha;

	psPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	psPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;
	for(int Index=1;Index<Size;Index++)	
	{
		pePoint.x=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pePoint.y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;

		if(Index==1)
			PushStart(psPoint,0);
		if(Push(psPoint,pePoint,0))
		{
			RectF headRect;
			RectF tailRect;
			float width,dx,dy;
			PointF lfCenter,rtCenter;
			mGraphics.FillPolygon(&brush,pts,npts);
			mGraphics.DrawPolygon(&Pen(Color(200,Red,Green,Blue),1),pts,npts);
			if(npts==4)
			{
				headRect=RectF((pts[0].X+pts[3].X)/2.0f,(pts[0].Y+pts[3].Y)/2.0f,0.0f,0.0f);
				tailRect=RectF((pts[1].X+pts[2].X)/2.0f,(pts[1].Y+pts[2].Y)/2.0f,0.0f,0.0f);
				dx=pts[3].X-pts[0].X;
				dy=pts[3].Y-pts[0].Y;
				width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
				headRect.Inflate(width,width);
				dx=pts[2].X-pts[1].X;
				dy=pts[2].Y-pts[1].Y;
				width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
				tailRect.Inflate(width,width);
			}
			else
			{
				headRect=RectF((pts[0].X+pts[9].X)/2.0f,(pts[0].Y+pts[9].Y)/2.0f,0.0f,0.0f);
				tailRect=RectF((pts[4].X+pts[5].X)/2.0f,(pts[4].Y+pts[5].Y)/2.0f,0.0f,0.0f);
				dx=pts[9].X-pts[0].X;
				dy=pts[9].Y-pts[0].Y;
				width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
				headRect.Inflate(width,width);
				dx=pts[5].X-pts[4].X;
				dy=pts[5].Y-pts[4].Y;
				width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
				tailRect.Inflate(width,width);
			}
			mGraphics.FillEllipse(&brush1,headRect);
			mGraphics.FillEllipse(&brush1,tailRect);


			//preWidth=nWidth;
			psPoint=pePoint;
		}
	}
	mGraphics.ResetTransform();
}

void CPenBrushUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{
	CPoint sPoint,ePoint,psPoint,pePoint,ptPoint;
	double dx,dy,xlen,ylen;
	double preWidth,nWidth;
	int count=1;
	int alpha;
	int v=0;
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
	preWidth=pDataInfo.AllRate[0].preWidth;
	alpha=pDataInfo.AllRate[0].alpha;
	psPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	psPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate; 
	int Size=pDataInfo.AllRate.size();
	for(int Index=1;Index<Size;Index++)
	{
		pePoint.x=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pePoint.y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
		dx=(pePoint.x-psPoint.x);
		dy=(pePoint.y-psPoint.y);
		nWidth=pDataInfo.AllRate[0].preWidth;
		float Angle=atan2(dy,dx)*Rate;
		GraphicsPath path(FillModeWinding);
		GraphicsPath path1,path2,path3,path4;
		RectF sRectF(psPoint.x,psPoint.y,0,0);
		sRectF.Inflate(preWidth,preWidth);
		RectF eRectF(pePoint.x,pePoint.y,0,0);
		eRectF.Inflate(nWidth,nWidth);
		///////////////////////////////////////////
		REAL  left=sRectF.GetLeft();
		REAL  top=sRectF.GetTop();
		REAL  bottom=sRectF.GetBottom();
		REAL  right=sRectF.GetRight();
		Point pts0[]={Point((left+right)/2,top),Point((left+right)/2,bottom)};
		matrix.RotateAt(Angle,PointF(psPoint.x,psPoint.y));
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
		matrix.RotateAt(Angle,PointF(pePoint.x,pePoint.y));
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

		preWidth=nWidth;
		psPoint=pePoint;
	}
	mGraphics.ResetTransform();
}

HRGN CPenBrushUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	CPoint sPoint,ePoint,psPoint,pePoint,ptPoint;
	GraphicsPath RgnPath(FillModeWinding);
	double dx,dy,xlen,ylen;
	double preWidth,nWidth;
	int count=1;
	int alpha;
	int v=0;
	Matrix matrix;
	Graphics mGraphics(hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	xlen=ePoint.x-sPoint.x;
	ylen=ePoint.y-sPoint.y;
	preWidth=pDataInfo.AllRate[0].preWidth;
	alpha=pDataInfo.AllRate[0].alpha;
	psPoint.x=sPoint.x+xlen*pDataInfo.AllRate[0].xRate;
	psPoint.y=sPoint.y+ylen*pDataInfo.AllRate[0].yRate;	
	int Size=pDataInfo.AllRate.size();
	for(int Index=1;Index<Size;Index++)
	{
		pePoint.x=sPoint.x+xlen*pDataInfo.AllRate[Index].xRate;
		pePoint.y=sPoint.y+ylen*pDataInfo.AllRate[Index].yRate;
		dx=(pePoint.x-psPoint.x);
		dy=(pePoint.y-psPoint.y);
		nWidth=pDataInfo.AllRate[Index].preWidth;
		float Angle=atan2(dy,dx)*Rate;
		GraphicsPath path(FillModeWinding);
		GraphicsPath path1,path2,path3,path4;
		RectF sRectF(psPoint.x,psPoint.y,0,0);
		sRectF.Inflate(preWidth+4,preWidth+4);
		RectF eRectF(pePoint.x,pePoint.y,0,0);
		eRectF.Inflate(nWidth+4,nWidth+4);
		///////////////////////////////////////////
		REAL  left=sRectF.GetLeft();
		REAL  top=sRectF.GetTop();
		REAL  bottom=sRectF.GetBottom();
		REAL  right=sRectF.GetRight();
		Point pts0[]={Point((left+right)/2,top),Point((left+right)/2,bottom)};
		matrix.RotateAt(Angle,PointF(psPoint.x,psPoint.y));
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
		matrix.RotateAt(Angle,PointF(pePoint.x,pePoint.y));
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

		preWidth=nWidth;
		psPoint=pePoint;
	}
	mGraphics.ResetTransform();
	Region Rgn(&RgnPath);
	return Rgn.GetHRGN(&mGraphics);
}


void CPenBrushUnit::DrawMultiPenBrush(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> &MultipsPoint,MultiInfo &GetMultiInfo,DWORD mouseid)
{
	int Red,Green,Blue;
	Red=GetRValue(GetMultiInfo.penColor);
	Green=GetGValue(GetMultiInfo.penColor);
	Blue=GetBValue(GetMultiInfo.penColor);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	SolidBrush brush(Color(255,Red,Green,Blue));
	UINT Size=MultipsPoint.size();
	CPoint sPoint=MultipsPoint[Size-1].point;
	if(Size==1)
	{
		if(fwidth.find(mouseid)!=fwidth.end())
			fwidth.erase(mouseid);
		fwidth.insert(std::map<DWORD,float>::value_type(mouseid,MultipsPoint[0].preWidth));
		PushStart(ePoint,mouseid);
	}
	if(Push(sPoint,ePoint,mouseid))
	{
		RectF headRect;
		RectF tailRect;
		float width,dx,dy;
		PointF lfCenter,rtCenter;
		mGraphics.FillPolygon(&brush,pts,npts);
		mGraphics.DrawPolygon(&Pen(Color(255,Red,Green,Blue),0.5f),pts,npts);
		if(npts==4)
		{
			headRect=RectF((pts[0].X+pts[3].X)/2.0f,(pts[0].Y+pts[3].Y)/2.0f,0.0f,0.0f);
			tailRect=RectF((pts[1].X+pts[2].X)/2.0f,(pts[1].Y+pts[2].Y)/2.0f,0.0f,0.0f);
			dx=pts[3].X-pts[0].X;
			dy=pts[3].Y-pts[0].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			headRect.Inflate(width,width);
			dx=pts[2].X-pts[1].X;
			dy=pts[2].Y-pts[1].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			tailRect.Inflate(width,width);
		}
		else
		{
			headRect=RectF((pts[0].X+pts[9].X)/2.0f,(pts[0].Y+pts[9].Y)/2.0f,0.0f,0.0f);
			tailRect=RectF((pts[4].X+pts[5].X)/2.0f,(pts[4].Y+pts[5].Y)/2.0f,0.0f,0.0f);
			dx=pts[9].X-pts[0].X;
			dy=pts[9].Y-pts[0].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			headRect.Inflate(width,width);
			dx=pts[5].X-pts[4].X;
			dy=pts[5].Y-pts[4].Y;
			width=sqrt(dx*dx+dy*dy)/2.0f+0.5f;
			tailRect.Inflate(width,width);
		}
		brush.SetColor(Color(255,Red,Green,Blue));
		mGraphics.FillEllipse(&brush,headRect);
		mGraphics.FillEllipse(&brush,tailRect);

		temSavePoint addPoint;
		addPoint.point=ePoint;
		addPoint.alpha=MultipsPoint[Size-1].alpha;
		addPoint.preWidth=MultipsPoint[Size-1].preWidth;
		MultipsPoint.push_back(addPoint);
	}
}

void CPenBrushUnit::PushStart(CPoint point,DWORD mouseid)
{
	FPOINT addpt1,addpt2,addpt3;
	if(top_fptLastEnd.find(mouseid)!=top_fptLastEnd.end())
	top_fptLastEnd.erase(mouseid);
	top_fptLastEnd.insert(std::map<DWORD,FPOINT>::value_type(mouseid,addpt1));

	if(bom_fptLastEnd.find(mouseid)!=bom_fptLastEnd.end())
	bom_fptLastEnd.erase(mouseid);
	bom_fptLastEnd.insert(std::map<DWORD,FPOINT>::value_type(mouseid,addpt2));

	if(m_fptLastEnd.find(mouseid)!=m_fptLastEnd.end())
	 m_fptLastEnd.erase(mouseid);
	m_fptLastEnd.insert(std::map<DWORD,FPOINT>::value_type(mouseid,addpt3));
	
	BOOL addcal=false;
	if(m_bCalCurve.find(mouseid)!=m_bCalCurve.end())
	m_bCalCurve.erase(mouseid);
	m_bCalCurve.insert(std::map<DWORD,BOOL>::value_type(mouseid,addcal));

    CBspine *addspine1=new CBspine();
	CBspine *addspine2=new CBspine();
	CBspine *addspine3=new CBspine();
	if(m_bspline.find(mouseid)!=m_bspline.end())
	{
	 //delete m_bspline[mouseid];
	 m_bspline.erase(mouseid);
	}
	m_bspline.insert(std::map<DWORD,CBspine*>::value_type(mouseid,addspine1));

	if(top_bspline.find(mouseid)!=top_bspline.end())
	{
	  delete top_bspline[mouseid];
	  top_bspline.erase(mouseid);
	}
	top_bspline.insert(std::map<DWORD,CBspine*>::value_type(mouseid,addspine2));

	if(bom_bspline.find(mouseid)!=bom_bspline.end())
	{
	 delete bom_bspline[mouseid];
	 bom_bspline.erase(mouseid);
	}
	bom_bspline.insert(std::map<DWORD,CBspine*>::value_type(mouseid,addspine3));
		
	m_bspline[mouseid]->Push(point, 0);
	m_bspline[mouseid]->Push(point, 0);
	m_bspline[mouseid]->Push(point, 0); 
}

BOOL CPenBrushUnit::Push(CPoint sPoint,CPoint ePoint,DWORD mouseid)
{
	if (sPoint ==ePoint)
		return false;

	float dx = ePoint.x-sPoint.x;
	float dy = ePoint.y-sPoint.y;
	float fDis = sqrt(dx*dx + dy*dy);
	int nZ = 2555 / (fDis*3.5f);
	if (nZ >= 0x1FF) //将Z量化到[0,511]区间
		nZ = 0x1FF;
	if (m_bspline[mouseid]->Push(ePoint, nZ))
	{
		CalcPointsAndConnectLine(mouseid);
	}
	return true;
}

void CPenBrushUnit::CalcPointsAndConnectLine(DWORD mouseid)
{
	FPOINT_NODE fptNode1, fptNode2;
	POSITION ps1, ps2;
	PointF pttop0,pttop1,pttop2,pttop3,ptbom0,ptbom1,ptbom2,ptbom3,pt0,pt1,pt2,pt3;
	//计算起点或者使用上一次曲线的终点
	if (!m_bCalCurve[mouseid])
	{
		m_bspline[mouseid]->Deboor(0.f, &fptNode1.fpt);
	}
	else
	{
		fptNode1.fpt = m_fptLastEnd[mouseid];
	}
	fptNode1.fu = 0.f;
	ps1 = m_fptList.AddHead(fptNode1);

	//计算终点
	m_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
	m_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
	fptNode2.fu = 1.f;
	ps2 = m_fptList.AddTail(fptNode2);
	UINT flag=0;
	CalcBsplineMidPoint(ps1, ps2,pt1,pt2,flag,mouseid);
	pt0=PointF(fptNode1.fpt.fX,fptNode1.fpt.fY);
	pt3=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);


	float g0,g3;
	g0 = fptNode1.fpt.fZ/511.f;
	g3 = fptNode2.fpt.fZ/511.f;
	g0 = 2*g0 - g0*g0;
	g3 = 2*g3 - g3*g3;
	float width0,width1,width2,width3,dwidth,len;
	width0=SetWidthCircleBrush(g0*fwidth[mouseid]);
	width3=SetWidthCircleBrush(g3*fwidth[mouseid]);
	dwidth=(width3-width0)/4.0f;
	width1=width0+dwidth;
	width2=width3-dwidth;

	if(!m_bCalCurve[mouseid])
		width0=width3;
	float dx=pt3.X-pt0.X;
	float dy=pt3.Y-pt0.Y;
	float Radian=atan2(dy,dx);

	pttop0.X=pt0.X+width0*cos(Radian-3.1415926f/2.0f)/2.0f;
	pttop0.Y=pt0.Y+width0*sin(Radian-3.1415926f/2.0f)/2.0f;

	pttop1.X=pt1.X+width1*cos(Radian-3.1415926f/2.0f)/2.0f;
	pttop1.Y=pt1.Y+width1*sin(Radian-3.1415926f/2.0f)/2.0f;

	pttop2.X=pt2.X+width2*cos(Radian-3.1415926f/2.0f)/2.0f;
	pttop2.Y=pt2.Y+width2*sin(Radian-3.1415926f/2.0f)/2.0f;

	pttop3.X=pt3.X+width3*cos(Radian-3.1415926f/2.0f)/2.0f;
	pttop3.Y=pt3.Y+width3*sin(Radian-3.1415926f/2.0f)/2.0f;

	ptbom0.X=pt0.X+width0*cos(Radian+3.1415926f/2.0f)/2.0f;
	ptbom0.Y=pt0.Y+width0*sin(Radian+3.1415926f/2.0f)/2.0f;

	ptbom1.X=pt1.X+width1*cos(Radian+3.1415926f/2.0f)/2.0f;
	ptbom1.Y=pt1.Y+width1*sin(Radian+3.1415926f/2.0f)/2.0f;

	ptbom2.X=pt2.X+width2*cos(Radian+3.1415926f/2.0f)/2.0f;
	ptbom2.Y=pt2.Y+width2*sin(Radian+3.1415926f/2.0f)/2.0f;

	ptbom3.X=pt3.X+width3*cos(Radian+3.1415926f/2.0f)/2.0f;
	ptbom3.Y=pt3.Y+width3*sin(Radian+3.1415926f/2.0f)/2.0f;

	int nZ;
	if (!m_bCalCurve[mouseid])
	{
		top_bspline[mouseid]->Push(CPoint(pttop0.X,pttop0.Y),0);
		top_bspline[mouseid]->Push(CPoint(pttop0.X,pttop0.Y),0);
		top_bspline[mouseid]->Push(CPoint(pttop0.X,pttop0.Y),0);
		bom_bspline[mouseid]->Push(CPoint(ptbom0.X,ptbom0.Y),0);
		bom_bspline[mouseid]->Push(CPoint(ptbom0.X,ptbom0.Y),0);
		bom_bspline[mouseid]->Push(CPoint(ptbom0.X,ptbom0.Y),0);

		m_fptList.RemoveAll();
		dx=pttop3.X-pttop0.X;
		dy=pttop3.Y-pttop0.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		top_bspline[mouseid]->Push(CPoint(pttop3.X,pttop3.Y), nZ);
		top_bspline[mouseid]->Deboor(0.f, &fptNode1.fpt);

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		top_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		top_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[0]=PointF(fptNode1.fpt.fX,fptNode1.fpt.fY);
		pts[1]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);



		m_fptList.RemoveAll();
		dx=ptbom3.X-ptbom0.X;
		dy=ptbom3.Y-ptbom0.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		bom_bspline[mouseid]->Push(CPoint(ptbom3.X,ptbom3.Y), nZ);
		bom_bspline[mouseid]->Deboor(0.f, &fptNode1.fpt);

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		bom_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		bom_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[3]=PointF(fptNode1.fpt.fX,fptNode1.fpt.fY);
		pts[2]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		npts=4;
		m_bCalCurve[mouseid]=true;
	}
	else
	{
		fptNode1.fpt = top_fptLastEnd[mouseid];

		m_fptList.RemoveAll();
		dx=pttop0.X-top_fptLastEnd[mouseid].fX;
		dy=pttop0.Y-top_fptLastEnd[mouseid].fY;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555/((len+1)*1.2f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		top_bspline[mouseid]->Push(CPoint(pttop0.X,pttop0.Y), nZ);

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		top_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		top_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[0]=PointF(fptNode1.fpt.fX,fptNode1.fpt.fY);
		pts[1]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_fptList.RemoveAll();
		dx=pttop1.X-pttop0.X;
		dy=pttop1.Y-pttop0.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		top_bspline[mouseid]->Push(CPoint(pttop1.X,pttop1.Y), nZ);
		fptNode1.fpt = top_fptLastEnd[mouseid];

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		top_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		top_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[2]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_fptList.RemoveAll();
		dx=pttop2.X-pttop1.X;
		dy=pttop2.Y-pttop1.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		top_bspline[mouseid]->Push(CPoint(pttop2.X,pttop2.Y), nZ);
		fptNode1.fpt = top_fptLastEnd[mouseid];

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		top_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		top_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[3]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_fptList.RemoveAll();
		dx=pttop3.X-pttop2.X;
		dy=pttop3.Y-pttop2.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		top_bspline[mouseid]->Push(CPoint(pttop3.X,pttop3.Y), nZ);
		fptNode1.fpt = top_fptLastEnd[mouseid];

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		top_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		top_fptLastEnd[mouseid]= fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[4]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		//////////////////////////////////////////////////////////////////////////////
		fptNode1.fpt = bom_fptLastEnd[mouseid];

		m_fptList.RemoveAll();
		dx=ptbom0.X-bom_fptLastEnd[mouseid].fX;
		dy=ptbom0.Y-bom_fptLastEnd[mouseid].fY;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		bom_bspline[mouseid]->Push(CPoint(ptbom0.X,ptbom0.Y), nZ);

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		bom_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		bom_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[9]=PointF(fptNode1.fpt.fX,fptNode1.fpt.fY);
		pts[8]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		///////////////////////////////////////////////////////////////////////////////////////////////
		m_fptList.RemoveAll();
		dx=ptbom1.X-ptbom0.X;
		dy=ptbom1.Y-ptbom0.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		bom_bspline[mouseid]->Push(CPoint(ptbom1.X,ptbom1.Y), nZ);
		fptNode1.fpt = bom_fptLastEnd[mouseid];

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		bom_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		bom_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[7]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		///////////////////////////////////////////////////////////////////////////////////////////////
		m_fptList.RemoveAll();
		dx=ptbom2.X-ptbom1.X;
		dy=ptbom2.Y-ptbom1.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		bom_bspline[mouseid]->Push(CPoint(ptbom2.X,ptbom2.Y), nZ);
		fptNode1.fpt = bom_fptLastEnd[mouseid];

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		bom_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		bom_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[6]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);
		///////////////////////////////////////////////////////////////////////////////////////////////
		m_fptList.RemoveAll();
		dx=ptbom3.X-ptbom2.X;
		dy=ptbom3.Y-ptbom2.Y;
		len=sqrt(dx*dx+dy*dy);
		nZ = 2555 / ((len+1)*2.0f);
		if (nZ >= 0x1FF) //将Z量化到[0,511]区间
			nZ = 0x1FF;
		bom_bspline[mouseid]->Push(CPoint(ptbom3.X,ptbom3.Y), nZ);
		fptNode1.fpt = bom_fptLastEnd[mouseid];

		fptNode1.fu = 0.f;
		ps1 = m_fptList.AddHead(fptNode1);

		//计算终点
		bom_bspline[mouseid]->Deboor(1.f, &fptNode2.fpt);
		bom_fptLastEnd[mouseid] = fptNode2.fpt; //保存终点以保证曲线的连续性
		fptNode2.fu = 1.f;
		ps2 = m_fptList.AddTail(fptNode2);

		pts[5]=PointF(fptNode2.fpt.fX,fptNode2.fpt.fY);

		npts=10;
	}
	m_fptList.RemoveAll();
}

float CPenBrushUnit::SetWidthCircleBrush( float Width )
{
	int m_nLength=101;
	if (Width < 1.f)
	{
		Width = 1.f;
	}
	else if (Width >m_nLength- 6.f)
	{
		Width = m_nLength - 6.f;
	}

	m_fWidthHalfCircle = Width / 2.f;
	if (m_fWidthHalfCircle >= 3.f)
	{
		m_fBorderInner = 1.0f;
		m_fBorderOuter = 1.0f;
		m_fBorderOuter2 = 1.0f;
	}
	if (m_fWidthHalfCircle >= 2.5f)
	{
		m_fBorderInner = 0.75f;
		m_fBorderOuter = 1.0f;
		m_fBorderOuter2 = 1.25f;
	}
	else if (m_fWidthHalfCircle >= 2.0f)
	{
		m_fBorderInner = 0.75f;
		m_fBorderOuter = 1.0f;
		m_fBorderOuter2 = 1.0f;
	}
	else if (m_fWidthHalfCircle >= 1.25f)
	{
		m_fBorderInner = 0.75f;
		m_fBorderOuter = 0.75f;
		m_fBorderOuter2 = 1.0f;
	}
	else if (m_fWidthHalfCircle >= 0.75f)
	{
		m_fBorderInner = 0.5f;
		m_fBorderOuter = 0.75f;
		m_fBorderOuter2 = 1.0f;
	}
	else if (m_fWidthHalfCircle >= 0.5f)
	{
		m_fBorderInner = 0.5f;
		m_fBorderOuter = 0.75f;
		m_fBorderOuter2 = 0.75f;
	}

	return m_fWidthHalfCircle + m_fBorderOuter + m_fBorderOuter2+m_fBorderInner;
}

void CPenBrushUnit::CalcBsplineMidPoint( POSITION psLeft, POSITION psRight,PointF &pts1,PointF &pts2,UINT &flag,DWORD mouseid)
{
	flag++;
	FPOINT_NODE &fptNode1=m_fptList.GetAt(psLeft), &fptNode2= m_fptList.GetAt(psRight);

	float fDeltaX = fptNode1.fpt.fX - fptNode2.fpt.fX;
	float fDeltaY = fptNode1.fpt.fY - fptNode2.fpt.fY;

	FPOINT_NODE fptNode;
	fptNode.fu = (fptNode1.fu+fptNode2.fu) / 2.f;
	m_bspline[mouseid]->Deboor(fptNode.fu, &fptNode.fpt);

	float fDx1 = fptNode.fpt.fX - fptNode1.fpt.fX;
	float fDy1 = fptNode.fpt.fY - fptNode1.fpt.fY;
	float fDx2 = fptNode2.fpt.fX - fptNode.fpt.fX;
	float fDy2 = fptNode2.fpt.fY - fptNode.fpt.fY;

	float fDotMul = fDx1*fDx2 + fDy1*fDy2;  //向量点乘

	POSITION ps = m_fptList.InsertAfter(psLeft, fptNode);
	if(flag==2)
		pts1=PointF(fptNode.fpt.fX,fptNode.fpt.fY);
	if(flag==3)
		pts2=PointF(fptNode.fpt.fX,fptNode.fpt.fY);
	if(flag<2)
	{
		CalcBsplineMidPoint(psLeft, ps,pts1,pts2,flag,mouseid);
		CalcBsplineMidPoint(ps, psRight,pts1,pts2,flag,mouseid);
	}

}