// BaseUnit.cpp: implementation of the CBaseUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "BaseUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseUnit::CBaseUnit()
{
	
}

CBaseUnit::~CBaseUnit()
{

}

void CBaseUnit::ReDraw(CDC *memDCC,DataInfo &pDataInfo)
{
	
}

void CBaseUnit::ReDraw(CDC *memDC,std::vector<DataInfo> &DataArray, DataInfo &pDataInfo)
{
	
}

CPoint CBaseUnit::CaculatePoint(CPoint point,DataInfo pDataInfo)
{
	float xDot=float(point.x-pDataInfo.CenterPoint.x);
	float yDot=float(point.y-pDataInfo.CenterPoint.y);
	double len=sqrt(xDot*xDot+yDot*yDot);
	double m_atan=atan2(yDot,xDot);
	double m_Angle=m_atan-pDataInfo.RotateAngle/Rate;
	point.y=len*sin(m_Angle);
	point.x=len*cos(m_Angle);
	return point;
}

void CBaseUnit::DrawPolyline(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> psPoint)
{
	
}

void CBaseUnit::DrawPenBrush(CDC *memDC,CPoint ePoint,std::vector<temSavePoint> psPoint)
{
	
}

void CBaseUnit::DrawGraphics(CDC *memDC,DataInfo &pDataInfo,CPoint ePoint)
{
	
}

void CBaseUnit::DrawText(CDC *memDC,DataInfo &pDataInfo, CString Text)
{
	
}


int CBaseUnit::ptInLine(CDC *memDC,CPoint point,DataInfo &pDataInfo)
{
	return 0;
}

int CBaseUnit::ptInRow(CDC *memDC, CPoint point, DataInfo &pDataInfo, CString Text)
{
	return 0;
}

void CBaseUnit::SetLineStyle(Pen &pen, int LineStyle)
{
    DashStyle dashStyle;
	switch (LineStyle)
	{
	case 0:
        dashStyle=DashStyleSolid;
        pen.SetDashStyle(dashStyle);
		break;
	case 1:
		dashStyle=DashStyleDash;
		pen.SetDashStyle(dashStyle);
		break;
	case 2:
		dashStyle=DashStyleDot;
		pen.SetDashStyle(dashStyle);
		break;
	case 3:
		dashStyle=(DashStyle)3;
		pen.SetDashStyle(dashStyle);
		break;
	case 4:
		dashStyle=(DashStyle)4;
		pen.SetDashStyle(dashStyle);
		break;
	}
}



void CBaseUnit::SetHatchStyle(HatchStyle &hatchStyle, int FillStyle)
{
	switch(FillStyle)
	{
	case 2:
		hatchStyle=HatchStyleHorizontal;
		break;
	case 3:
        hatchStyle=HatchStyleVertical;
		break;
	case 4:
        hatchStyle=HatchStyleForwardDiagonal;
		break;
	case 5:
		 hatchStyle=HatchStyleBackwardDiagonal;
		break;
	case 6:
		hatchStyle=HatchStyleCross;
		break;
	case 7:
		hatchStyle=HatchStyleDiagonalCross;
		break;
	case 8:
        hatchStyle=HatchStyleLightVertical;
		break;
	case 9:
		hatchStyle=HatchStyleLightHorizontal;
		break;
	case 10:
		HatchStyleDashedHorizontal;
		break;
	case 11:
		hatchStyle=HatchStyleWave;
		break;
	case 12:
		hatchStyle=HatchStyleDiagonalBrick;
		break;
	case 13:
        hatchStyle=HatchStyleHorizontalBrick;
		break;
	case 14:
		hatchStyle=HatchStyleWeave;
		break;
	case 15:
		hatchStyle=HatchStyleSolidDiamond;
		break;
	}
}

void CBaseUnit::DrawPolygon(CDC *memDC,CPoint point)
{
 
}


void CBaseUnit::ErasedDraw(CDC *memDC, DataInfo &pDataInfo,HRGN ErasedRgn)
{

}

HRGN CBaseUnit::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{

 return NULL;
}

GraphicsPath *CBaseUnit::GetObjPath(CDC *memDC, DataInfo &pDataInfo)
{
 return NULL;
}
