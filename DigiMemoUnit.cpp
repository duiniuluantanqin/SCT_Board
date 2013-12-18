#include "stdafx.h"
#include "DigiMemoUnit.h"


CDigiMemoUnit::CDigiMemoUnit(void)
{
}

CDigiMemoUnit::CDigiMemoUnit(int width,int height)
{
	CavasWidth=width;
	CavasHeight=height;
}

CDigiMemoUnit::~CDigiMemoUnit(void)
{
}

BOOL CDigiMemoUnit::ReadDigiMemoFile(CString FilePath,std::vector<std::vector<temSavePoint>> &ppsPoint,std::vector<START_END_POINT> &pt_Start_End)
{
	CFile file;
	if(!file.Open(FilePath,CFile::modeRead|CFile::typeBinary))
	{
		MessageBox(NULL,"指定文件不存在!",NULL,0);
		return false;
	}

	BYTE buff[40];
	file.Read(buff,40);
	MemoWidth=(buff[34]<<8)+buff[33];
	MemoHeight=(buff[36]<<8)+buff[35];

	WidthRate=CavasWidth/MemoWidth;
	HeightRate=CavasHeight/MemoHeight;

	START_END_POINT ptStartEnd;

	while(file.Read(buff,1))
	{
		switch(buff[0]&0XFF)
		{
		case 0X90: //Layer Data Format

			break;
		case 0X88: //TimeStamp Data Format

			break;
		case 0X80: //Pen State Data Formate:Pen Up

			break;
		case 0X81: //Pen State Data Format:Pen Down
			std::vector<temSavePoint> psPoint;
			ReadObjData(file,psPoint,ptStartEnd);
			ppsPoint.push_back(psPoint);
			pt_Start_End.push_back(ptStartEnd);
			break;
		}
	}

	return true;
}

void CDigiMemoUnit::ReadObjData(CFile &file,std::vector<temSavePoint> &psPoint,START_END_POINT &ptStartEnd)
{
	BYTE buff[1];
	int ldata,hdata;
	int Index=0;
	BOOL isPenUp=false;
	int PenUp;
	temSavePoint pt;
	while(file.Read(buff,1))
	{
		if((buff[0]&0XFF)==0X80) //Pen State Data Format:Pen up
		{
			isPenUp=true;
			PenUp=0;
			continue;
		}
		switch(Index)
		{
		case 0:
		case 2:
			ldata=buff[0];
			Index++;
			break;
		case 1:
			hdata=buff[0]<<7;
			hdata+=ldata;
			pt.point.x=(int)(hdata*WidthRate);
			Index++;
			break;
		case 3:
			hdata=buff[0]<<7;
			hdata+=ldata;
			pt.point.y=(int)((MemoHeight-hdata)*HeightRate);
			psPoint.push_back(pt);
			if(psPoint.size()==1)
			{
				ptStartEnd.ptStart=ptStartEnd.ptEnd=pt.point;
			}
			else
			{
				if(pt.point.x<ptStartEnd.ptStart.x)
					ptStartEnd.ptStart.x=pt.point.x;
				if(pt.point.y<ptStartEnd.ptStart.y)
					ptStartEnd.ptStart.y=pt.point.y;
				if(pt.point.x>ptStartEnd.ptEnd.x)
					ptStartEnd.ptEnd.x=pt.point.x;
				if(pt.point.y>ptStartEnd.ptEnd.y)
					ptStartEnd.ptEnd.y=pt.point.y;
			}
			Index=0;
			break;
		}
		if(isPenUp)
		{
			PenUp++;
			if(PenUp==4)
				break;
		}
	}
}