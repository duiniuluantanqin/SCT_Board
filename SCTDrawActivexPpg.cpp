// SCTDrawActivexPpg.cpp : Implementation of the CSCTDrawActivexPropPage property page class.

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "SCTDrawActivexPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSCTDrawActivexPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSCTDrawActivexPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSCTDrawActivexPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCTDrawActivexPropPage, "SCTDRAWACTIVEX.SCTDrawActivexPropPage.1",
	0x221095a9, 0x86be, 0x4c40, 0x9b, 0x11, 0x57, 0xdb, 0xd3, 0xee, 0x54, 0x63)


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexPropPage::CSCTDrawActivexPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCTDrawActivexPropPage

BOOL CSCTDrawActivexPropPage::CSCTDrawActivexPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SCTDRAWACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexPropPage::CSCTDrawActivexPropPage - Constructor

CSCTDrawActivexPropPage::CSCTDrawActivexPropPage() :
	COlePropertyPage(IDD, IDS_SCTDRAWACTIVEX_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSCTDrawActivexPropPage)
	m_Operations = -1;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexPropPage::DoDataExchange - Moves data between page and properties

void CSCTDrawActivexPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSCTDrawActivexPropPage)
	DDP_CBIndex(pDX, IDC_Operations, m_Operations, _T("Operations") );
	DDX_CBIndex(pDX, IDC_Operations, m_Operations);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexPropPage message handlers
