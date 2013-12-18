#pragma once

typedef struct tagFPOINT
{
	float fX, fY, fZ;
}FPOINT, *PFPOINT;

typedef struct tagFPOINT_NODE
{
	FPOINT fpt;
	float  fu;
}FPOINT_NODE, *PFPOINT_NODE;


class CBspine
{
public:
	CBspine(int nDegree = 3);
	~CBspine(void);

public:
	void InitKnot(void);  //初始化节点向量
	bool Push(FPOINT fpt); //输入控制点 
	bool Push( POINT pt , int nZ = 0); //nZ为第三维坐标
	void Clear(void);    //清除控制点
	void Deboor(float u, FPOINT *pFPoint); //根据参数u计算出点

private:
	int		m_nDegree;   //样条曲线次数
	float	*m_pfknot;   //参数节点向量
	int		m_nControl;  
	FPOINT	*m_pControl; //控制点向量
	FPOINT	*m_pCalc;    //计算曲线上点 临时之用
};
