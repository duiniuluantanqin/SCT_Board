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
	void InitKnot(void);  //��ʼ���ڵ�����
	bool Push(FPOINT fpt); //������Ƶ� 
	bool Push( POINT pt , int nZ = 0); //nZΪ����ά����
	void Clear(void);    //������Ƶ�
	void Deboor(float u, FPOINT *pFPoint); //���ݲ���u�������

private:
	int		m_nDegree;   //�������ߴ���
	float	*m_pfknot;   //�����ڵ�����
	int		m_nControl;  
	FPOINT	*m_pControl; //���Ƶ�����
	FPOINT	*m_pCalc;    //���������ϵ� ��ʱ֮��
};
