// Comination.h: interface for the CComination class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMINATION_H__FA019566_72EF_457C_B633_C43B15721DE3__INCLUDED_)
#define AFX_COMINATION_H__FA019566_72EF_457C_B633_C43B15721DE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseUnit.h"
#include "LineUnit.h"
#include "RectUnit.h"
#include "RoundRectUnit.h"
#include "EllipseUnit.h"
#include "ArcUnit.h"
#include "SectorUnit.h"
#include "PolylineUnit.h"
#include "PenBrushUnit.h"
#include "TextUnit.h"
#include "BitmapUnit.h"

class CComination : public CBaseUnit  
{
public:
	CComination();
	virtual ~CComination();

};

#endif // !defined(AFX_COMINATION_H__FA019566_72EF_457C_B633_C43B15721DE3__INCLUDED_)
