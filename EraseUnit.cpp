// EraseUnit.cpp: implementation of the CEraseUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "EraseUnit.h"
//#include  "SCTDrawActivexCtl.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//extern CSCTDrawActivexCtrl *active;
extern BOOL isfinish;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEraseUnit::CEraseUnit()
{
	pErasureImage = NULL;
	ptArray=NULL;
	//AllocConsole();
	//hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
}

CEraseUnit::~CEraseUnit()
{

}

HRGN CEraseUnit::EraseOperation(HDC m_hDC,UINT PenWidth)
{
	Graphics mGraphics(m_hDC);
	GraphicsPath GeometryPath;
	Pen pen(Color(255,255,255,0),PenWidth);
	GeometryPath.AddBeziers(ptArray,ptArraySize);
	pen.SetStartCap(LineCapRound);
	pen.SetEndCap(LineCapRound);
	pen.SetLineJoin(LineJoinRound);
	GeometryPath.Widen(&pen);
	Region Rgn(&GeometryPath);
	if(ptArray!=NULL)
	{
		delete [] ptArray;
		ptArray=NULL;
	}
	if(Rgn.IsEmpty(&mGraphics))
		return NULL;
	else
		return Rgn.GetHRGN(&mGraphics);
}

BOOL CEraseUnit::OnMouseDown(UINT nFlags, CPoint point)
{
	if(ptArray!=NULL)
	{
		delete [] ptArray;
		ptArray=NULL;
	}
	ptArray=new PointF [1];
	ptArray[0].X=point.x;
	ptArray[0].Y=point.y;
	ptArraySize=1;
	return true;
}

BOOL CEraseUnit::OnMouseMove(UINT nFlags, CPoint point)
{
	UINT Size;
	PointF *points;
	if(point.x==ptArray[ptArraySize].X&&point.y==ptArray[ptArraySize].Y)
		return false;
	if(ptArraySize==1)
	{
		points=new PointF [5];
		memcpy(points,ptArray,sizeof(PointF));
		points[1].X=points[2].X=points[3].X=(points[0].X+point.x)/2;
		points[1].Y=points[2].Y=points[3].Y=(points[0].Y+point.y)/2;
		points[4].X=point.x;
		points[4].Y=point.y;
		ptArraySize=4;
		delete [] ptArray;
		ptArray=new PointF [5];
		memcpy(ptArray,points,5*sizeof(PointF));
	}
	else
	{
		Size=ptArraySize+1;
		PointF pt=ptArray[ptArraySize];
		points=new PointF [Size+3];
		memcpy(points,ptArray,Size*sizeof(PointF));
		points[Size].X=points[Size+1].X=(pt.X+point.x)/2;
		points[Size].Y=points[Size+1].Y=(pt.Y+point.y)/2;
		points[Size+2].X=point.x;
		points[Size+2].Y=point.y;
		ptArraySize+=3;
		delete [] ptArray;
		ptArray=new PointF [Size+3];
		memcpy(ptArray,points,(Size+3)*sizeof(PointF));
	}
	delete [] points;
	return true;
}

BOOL CEraseUnit::OnMouseUp(UINT nFlags, CPoint point)
{
	UINT Size;
	PointF *points;
	if(ptArraySize==1)
	{  
		points=new PointF [5];
		memcpy(points,ptArray,sizeof(PointF));
		points[1].X=points[2].X=points[3].X=points[4].X=points[0].X+1;
		points[1].Y=points[2].Y=points[3].Y=points[4].Y=points[0].Y+1;
		ptArraySize=4;
		delete [] ptArray;
		ptArray=new PointF [5];
		memcpy(ptArray,points,5*sizeof(PointF));
		delete [] points;
	}
	return true;
}

void CEraseUnit::OnRender(CDC *pDC,UINT PenWidth)
{
	UINT EraserWidth = PenWidth;
	Graphics mGraphics(pDC->m_hDC);
	//DisplayConsole("M",sizeof("M"));
	TextureBrush tBrush(pErasureImage);
	//DisplayConsole("N",sizeof("N"));
	Pen PenBrush(&tBrush,EraserWidth);
	GraphicsPath BeziersPath;
	BeziersPath.AddBeziers(ptArray,ptArraySize);
	PenBrush.SetStartCap(LineCapRound);
	PenBrush.SetEndCap(LineCapRound);
	PenBrush.SetLineJoin(LineJoinRound);
	mGraphics.DrawPath(&PenBrush,&BeziersPath);	
}


/*void CEraseUnit::DisplayConsole(char *info,int size)
	{
		WriteConsole(hConsole,info,size,NULL,NULL);
		//WriteConsole(hConsole,"abc",7,NULL,NULL);
	}*/




