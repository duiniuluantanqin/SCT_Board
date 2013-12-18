#include "StdAfx.h"
#include ".\bspine.h"

CBspine::CBspine(int nDegree)
	: m_nDegree(nDegree), m_nControl(0)
{
	ASSERT(m_nDegree > 0);

	m_pfknot	= new float [2*(m_nDegree+1)];
	m_pControl	= new FPOINT[m_nDegree+1];
	m_pCalc		= new FPOINT[m_nDegree+1];
	InitKnot();
}

CBspine::~CBspine(void)
{
	if (m_pCalc != NULL)
		delete [] m_pCalc, m_pCalc = NULL;
	if (m_pControl != NULL)
		delete [] m_pControl, m_pControl=NULL;
	if (m_pfknot != NULL)
		delete [] m_pfknot, m_pfknot =NULL;
}

void CBspine::InitKnot( void )
{
	//均匀周期曲线，将控制点控制的曲线段(即第m_nDegree段)参数规格化到[0,1]区间
	for (int i=0; i<=2*m_nDegree+1; i++) 
		m_pfknot[i] = i-m_nDegree;
}


bool CBspine::Push( FPOINT fpt )
{
	if (m_nControl != m_nDegree+1)
	{
		m_pControl[m_nControl] = fpt;
		m_nControl++;
		return m_nControl == m_nDegree+1 ? true : false;
	}

	for (int i=0; i<m_nDegree; i++)
	{
		m_pControl[i] = m_pControl[i+1];
	}
	m_pControl[m_nDegree] = fpt;
	return true;
}

bool CBspine::Push( POINT pt , int nZ)
{
	FPOINT fpt;
	fpt.fX = pt.x;
	fpt.fY = pt.y;
	fpt.fZ = nZ;
	return Push(fpt);
}

void CBspine::Clear( void )
{
	m_nControl = 0;
}

void CBspine::Deboor( float u, FPOINT *pFPoint )
{
	ASSERT(m_nControl == m_nDegree+1 &&
		u>=m_pfknot[m_nDegree] && u<=m_pfknot[m_nDegree+1]);

	for (int i=0; i<=m_nDegree; i++)  //控制点复制到计算数组中
	{
		m_pCalc[i] = m_pControl[i];
	}

	for (int k=1; k<=m_nDegree; k++)
	{
		for (int j=m_nDegree; j>=k; j--)
		{
			float t = (u - m_pfknot[j]) / (m_pfknot[j+m_nDegree-k+1] - m_pfknot[j]);
			m_pCalc[j].fX = m_pCalc[j].fX*t + m_pCalc[j-1].fX*(1.f-t);
			m_pCalc[j].fY = m_pCalc[j].fY*t + m_pCalc[j-1].fY*(1.f-t);
			m_pCalc[j].fZ = m_pCalc[j].fZ*t + m_pCalc[j-1].fZ*(1.f-t);
		}
	}

	*pFPoint = m_pCalc[m_nDegree];
}

