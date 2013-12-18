#pragma once

#include "BaseUnit.h"
typedef struct START_END_POINT
{
  CPoint ptStart;
  CPoint ptEnd;
}START_END_POINT;

class CDigiMemoUnit
{
public:
	CDigiMemoUnit(void);
	CDigiMemoUnit(int width,int height);
	~CDigiMemoUnit(void);
private:
	float CavasWidth;
	float CavasHeight;
	float MemoWidth;
	float MemoHeight;
	float WidthRate;
	float HeightRate;
private:
	void ReadObjData(CFile &file,std::vector<temSavePoint> &psPoint,START_END_POINT &ptStartEnd);
public:
	BOOL ReadDigiMemoFile(CString FilePath,std::vector<std::vector<temSavePoint>> &ppsPoint,std::vector<START_END_POINT> &pt_Start_End);
};

