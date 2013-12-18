// Combination.cpp: implementation of the CCombination class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "Combination.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCombination::CCombination()
{
	ptBaseUnit=new CBaseUnit;
	ptLineUnit=new CLineUnit;
	ptRectUnit=new CRectUnit;
	ptRoundRectUnit=new CRoundRectUnit;
	ptEllipseUnit=new CEllipseUnit;
	ptArcUnit=new CArcUnit;
	ptSectorUnit=new CSectorUnit;
	ptPolylineUnit=new CPolylineUnit;
	ptPenBrushUnit=new CPenBrushUnit;
	ptTextUnit=new CTextUnit;
	ptBmpUnit=new CBitmapUnit;
	ptPolygonUnit=new CPolygonUnit;
	ptRgnUnit=new CRgnObjUnit;
}

CCombination::~CCombination()
{
	/*delete ptBaseUnit;
	delete ptLineUnit;
	delete ptRectUnit;
	delete ptRoundRectUnit;
	delete ptEllipseUnit;
	delete ptArcUnit;
	delete ptSectorUnit;
	delete ptPolylineUnit;
	delete ptPenBrushUnit;
	delete ptTextUnit;
	delete ptBmpUnit; */
}

void CCombination::ModifyCombinatedObjs(std::vector<DataInfo> &DataArray, DataInfo &pDataInfo)
{
	DataInfo GetDataInfo;
	ComInfo GetComInfo;
	double len,Angle;
	CPoint tPoint;
	CPoint Rel_StartPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo);
	CPoint Rel_EndPoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo);
	int xlen=Rel_EndPoint.x-Rel_StartPoint.x;
	int ylen=Rel_EndPoint.y-Rel_StartPoint.y;
	int Num;
	int Size=pDataInfo.ComArray.size();
	for(int Index=0;Index<Size;Index++)
	{
		GetComInfo=pDataInfo.ComArray[Index];
		Num=GetComInfo.ObjIndex;
		GetDataInfo=DataArray[Num]; 
		float mid;
		/////////////////////////RotateAngle&&preAngle///////////////////////////////////
		GetDataInfo.RotateAngle=pDataInfo.RotateAngle+GetComInfo.OffsetAngle;
		GetDataInfo.preAngle=pDataInfo.preAngle;
		GetDataInfo.xScale=GetComInfo.xScale;
		GetDataInfo.yScale=GetComInfo.yScale;
		if(pDataInfo.xScale<0)
		{
			mid=GetComInfo.xStartRate;
			GetComInfo.xStartRate=1-GetComInfo.xEndRate;
			GetComInfo.xEndRate=1-mid;
			GetComInfo.xCenterRate=1-GetComInfo.xCenterRate;
		}
		if(pDataInfo.yScale<0)
		{
			mid=GetComInfo.yStartRate;
			GetComInfo.yStartRate=1-GetComInfo.yEndRate;
			GetComInfo.yEndRate=1-mid;
			GetComInfo.yCenterRate=1-GetComInfo.yCenterRate;
		}
		//////////////////StartPoint////////////////////////////////////
		tPoint.x=Rel_StartPoint.x+xlen*GetComInfo.xStartRate;
		tPoint.y=Rel_StartPoint.y+ylen*GetComInfo.yStartRate;
		len=sqrt(float(tPoint.x*tPoint.x)+float(tPoint.y*tPoint.y));
		Angle=atan2(float(tPoint.y),float(tPoint.x))+pDataInfo.RotateAngle/Rate;
		GetDataInfo.StartPoint.x=pDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.StartPoint.y=pDataInfo.CenterPoint.y+len*sin(Angle);
		//////////////////EndPoint///////////////////////////////////////
		tPoint.x=Rel_StartPoint.x+xlen*GetComInfo.xEndRate;
		tPoint.y=Rel_StartPoint.y+ylen*GetComInfo.yEndRate;
		len=sqrt(float(tPoint.x*tPoint.x)+float(tPoint.y*tPoint.y));
		Angle=atan2(float(tPoint.y),float(tPoint.x))+pDataInfo.RotateAngle/Rate;
		GetDataInfo.EndPoint.x=pDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.EndPoint.y=pDataInfo.CenterPoint.y+len*sin(Angle);
		//////////////////CenterPoint////////////////////////////////////////
		tPoint.x=Rel_StartPoint.x+xlen*GetComInfo.xCenterRate;
		tPoint.y=Rel_StartPoint.y+ylen*GetComInfo.yCenterRate;
		len=sqrt(float(tPoint.x*tPoint.x)+float(tPoint.y*tPoint.y));
		Angle=atan2(float(tPoint.y),float(tPoint.x))+pDataInfo.RotateAngle/Rate;
		GetDataInfo.CenterPoint.x=pDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.CenterPoint.y=pDataInfo.CenterPoint.y+len*sin(Angle);
		//////////////////////////////////////////////////////////////
		DataArray[Num]=GetDataInfo; 
		if(DataArray[Num].ComArray.size()>0)
			this->ModifyCombinatedObjs(DataArray,DataArray[Num]);
	}
}

HRGN CCombination::GetRedrawHRGN(HDC hDC, std::vector<DataInfo> &DataArray,DataInfo &pDataInfo)
{
	Graphics graphics(hDC); 
	Region tRgn(Rect(0,0,0,0));
	Matrix matrix;
	HRGN hRgn;
	DataInfo GetDataInfo;
	int Size=pDataInfo.ComArray.size();
	int Num;
	for(int Index=0;Index<Size;Index++)
	{
		Num=pDataInfo.ComArray[Index].ObjIndex;
		GetDataInfo=DataArray[Num];
		switch (GetDataInfo.OperationKind)
		{
		case 0:
			ptBaseUnit=ptLineUnit;
			break;
		case 1:
			ptBaseUnit=ptRectUnit;
			break;
		case 2:
			ptBaseUnit=ptRoundRectUnit;
			break;
		case 3:
			ptBaseUnit=ptEllipseUnit;
			break;
		case 4:
			ptBaseUnit=ptArcUnit;
			break;
		case 5:
			ptBaseUnit=ptSectorUnit;
			break;
		case 6:
			ptBaseUnit=ptPenBrushUnit;
			break;
		case 7:
			ptBaseUnit=ptPolylineUnit;
			break;
		case 8:
			ptBaseUnit=ptTextUnit;
			break;
		case 9:
			ptBaseUnit=ptBmpUnit;
			break;
		case 10:
			ptBaseUnit=this;
			break;
		case 11:
			ptBaseUnit=ptPolygonUnit;
			break;
		case 12:
			ptBaseUnit=ptRgnUnit;
			break;
		}
		hRgn=ptBaseUnit->GetRedrawHRGN(hDC,DataArray,GetDataInfo);
		Region Rgn(hRgn);	 
		matrix.Translate(GetDataInfo.CenterPoint.x,GetDataInfo.CenterPoint.y);
		matrix.Rotate(GetDataInfo.RotateAngle);
		matrix.Scale(GetDataInfo.xScale,GetDataInfo.yScale);
		Rgn.Transform(&matrix);
		matrix.Reset();
		tRgn.Union(&Rgn);
		DeleteObject(hRgn);
	}
	//////////////////////////////////////	
	matrix.Scale(1/GetDataInfo.xScale,1/GetDataInfo.yScale);
	matrix.Rotate(-pDataInfo.RotateAngle);
	matrix.Translate(-pDataInfo.CenterPoint.x,-pDataInfo.CenterPoint.y);
	tRgn.Transform(&matrix);
	matrix.Reset();
	/////////////////////////////////////
	return tRgn.GetHRGN(&graphics);
}

void CCombination::GetCombinatedObjs(std::vector<DataInfo> DataArray,DataInfo pDataInfo,std::map<int,int> &ObjList)
{
	for(int Index=0;Index<pDataInfo.ComArray.size();Index++)
	{

		if(DataArray[pDataInfo.ComArray[Index].ObjIndex].ComArray.size()>0)
			GetCombinatedObjs(DataArray,DataArray[pDataInfo.ComArray[Index].ObjIndex],ObjList);	
		else
			ObjList.insert(std::map<int,int>::value_type(pDataInfo.ComArray[Index].ObjIndex,pDataInfo.ComArray[Index].ObjIndex));
	}
}

void CCombination::ReDraw(CDC *memDC,std::vector<DataInfo> DataArray,std::map<int,int> ObjList)
{	
	DataInfo GetDataInfo; 
	for(int Index=0;Index<DataArray.size();Index++)
	{
		if(ObjList.find(Index)==ObjList.end())
			continue;
		GetDataInfo=DataArray[Index];
		switch (GetDataInfo.OperationKind)
		{
		case 0:
			ptLineUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 1:
			ptRectUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 2:
			ptRoundRectUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 3:
			ptEllipseUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 4:
			ptArcUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 5:
			ptSectorUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 6:
			ptPenBrushUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 7:
			ptPolylineUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 8:
			ptTextUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 9:
			ptBmpUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 11:
			ptPolygonUnit->ReDraw(memDC,GetDataInfo);
			break;
		case 12:
			ptRgnUnit->ReDraw(memDC,GetDataInfo);
			break;
		}
	}
}