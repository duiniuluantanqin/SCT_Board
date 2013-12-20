// SCTDrawActivexCtl.cpp : Implementation of the CSCTDrawActivexCtrl ActiveX Control class.

#include "stdafx.h"
#include "direct.h"
#include "SCT Draw Activex.h"
#include "SCTDrawActivexCtl.h"
#include "SCTDrawActivexPpg.h"
#include "StylesDlg.h"
#include "windowsx.h"
#include  "ObjIdl.h"
#include "TWGLClient.h"
#include <math.h>

//////////////////////////////////
#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PI 3.1415926
#define Rate 57.2957805

BOOL isgestureDown = false;//手势信息是否识别成功
BOOL isSaved = false;//第一次保存所绘笔迹
BOOL eraserUp = false;


/////////////////////temp////////////////////////
static void SaveToFile(LPSTR str)
{
	HANDLE hdl = CreateFile("C:\\debug.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hdl == INVALID_HANDLE_VALUE)
	{
		return;
	}
	SetFilePointer(hdl, 0, NULL, FILE_END);
	DWORD dwWritenSize = 0;
	BOOL bRet = WriteFile(hdl, str, strlen(str),&dwWritenSize, NULL);
	FlushFileBuffers(hdl);	
	CloseHandle(hdl);
}

static VOID FillInputData(TOUCHINPUT* inData, DWORD cursor, DWORD eType, DWORD time, int x, int y)
{
	inData->dwID = cursor;
	inData->dwFlags = eType;
	inData->dwTime = time;
	inData->x = x;
	inData->y = y;
}
/////////////////////temp////////////////////////

IMPLEMENT_DYNCREATE(CSCTDrawActivexCtrl, COleControl)
	/////////////////////////////////////////////////////////////////////////////
	// Message map
	BEGIN_MESSAGE_MAP(CSCTDrawActivexCtrl, COleControl)
		//{{AFX_MSG_MAP(CSCTDrawActivexCtrl)
		ON_WM_CREATE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_TIMER()
		ON_WM_LBUTTONDBLCLK()
		ON_EN_CHANGE(IDC_Text, OnChangeText)
		//}}AFX_MSG_MAP
		ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
		ON_WM_SIZE()
		
	END_MESSAGE_MAP()


	/////////////////////////////////////////////////////////////////////////////
	// Dispatch map

	BEGIN_DISPATCH_MAP(CSCTDrawActivexCtrl, COleControl)
		//{{AFX_DISPATCH_MAP(CSCTDrawActivexCtrl)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "Operations", m_operations, OnOperationsChanged, VT_I2)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "PenWidth", m_penWidth, OnPenWidthChanged, VT_I2)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "PenColor", m_penColor, OnPenColorChanged, VT_COLOR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "FillColor", m_fillColor, OnFillColorChanged, VT_COLOR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "FontColor", m_fontColor, OnFontColorChanged, VT_COLOR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "FilePath", m_filePath, OnFilePathChanged, VT_BSTR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "temBmpPath", m_temBmpPath, OnTemBmpPathChanged, VT_BSTR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "bkColor", m_bkColor, OnBkColorChanged, VT_COLOR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "LineStyle", m_lineStyle, OnLineStyleChanged, VT_I2)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "FillStyle", m_fillStyle, OnFillStyleChanged, VT_I2)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "foreColor", m_foreColor, OnForeColorChanged, VT_COLOR)
		DISP_PROPERTY_NOTIFY(CSCTDrawActivexCtrl, "bkBmp", m_bkBmp, OnBkBmpChanged, VT_BSTR)
		DISP_PROPERTY_EX(CSCTDrawActivexCtrl, "ReservedProperty0", GetReservedProperty0, SetReservedProperty0, VT_BSTR)
		DISP_PROPERTY_EX(CSCTDrawActivexCtrl, "ReservedProperty1", GetReservedProperty1, SetReservedProperty1, VT_BSTR)
		DISP_PROPERTY_EX(CSCTDrawActivexCtrl, "ReservedProperty2", GetReservedProperty2, SetReservedProperty2, VT_BSTR)
		DISP_PROPERTY_EX(CSCTDrawActivexCtrl, "ReservedProperty3", GetReservedProperty3, SetReservedProperty3, VT_BSTR)
		DISP_PROPERTY_EX(CSCTDrawActivexCtrl, "ReservedProperty4", GetReservedProperty4, SetReservedProperty4, VT_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetOperationEX", SetOperationEX, VT_BSTR, VTS_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetLineStyleEX", SetLineStyleEX, VT_BSTR, VTS_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetFillStyleEX", SetFillStyleEX, VT_BSTR, VTS_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "CreateNewPage", CreateNewPage, VT_EMPTY, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyLineWidth", ModifyLineWidth, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyLineColor", ModifyLineColor, VT_BOOL, VTS_COLOR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyFontColor", ModifyFontColor, VT_BOOL, VTS_COLOR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "AddPicture", AddPicture, VT_BOOL, VTS_HANDLE VTS_I2 VTS_I2 VTS_I2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "AddPictureFromFile", AddPictureFromFile, VT_BOOL, VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "AddPictureFromHandle", AddPictureFromHandle, VT_BOOL, VTS_HANDLE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ShowPage", ShowPage, VT_I2, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetPageBmp", GetPageBmp, VT_HANDLE, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetbkImageFromHandle", SetbkImageFromHandle, VT_BOOL, VTS_HANDLE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetCombination", SetCombination, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetDisjunction", SetDisjunction, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "Undo", Undo, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "Redo", Redo, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SelectAllObj", SelectAllObj, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ErasedRegion", ErasedRegion, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ZoomIn", ZoomIn, VT_BOOL, VTS_R4)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "DeleteSelObj", DeleteSelObj, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "CopySelObj", CopySelObj, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "PasteObj", PasteObj, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "LockSelObj", LockSelObj, VT_BOOL, VTS_BOOL)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "DeletePage", DeletePage, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjIsLocked", GetSelObjIsLocked, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjStartPointX", GetSelObjStartPointX, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjStartPointY", GetSelObjStartPointY, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjEndPointX", GetSelObjEndPointX, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjEndPointY", GetSelObjEndPointY, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjCenterPointX", GetSelObjCenterPointX, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjCenterPointY", GetSelObjCenterPointY, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjRotateAngle", GetSelObjRotateAngle, VT_R8, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjLineStyle", GetSelObjLineStyle, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjFillStyle", GetSelObjFillStyle, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjPenWidth", GetSelObjPenWidth, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjFillColor", GetSelObjFillColor, VT_I4, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjFontColor", GetSelObjFontColor, VT_I4, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjForeColor", GetSelObjForeColor, VT_I4, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetPageNum", GetPageNum, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "CopyPage", CopyPage, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "PastPage", PastPage, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "EraseAll", EraseAll, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SaveDataToFile", SaveDataToFile, VT_I4, VTS_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ReadDataFromFile", ReadDataFromFile, VT_I4, VTS_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyFillStyle", ModifyFillStyle, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyLineStyle", ModifyLineStyle, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "MoveUp", MoveUp, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "MoveTop", MoveTop, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "MoveDown", MoveDown, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "MoveBottom", MoveBottom, VT_BOOL, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyFillColor", ModifyFillColor, VT_BOOL, VTS_COLOR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifyForeColor", ModifyForeColor, VT_BOOL, VTS_COLOR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "DrawPolygon", DrawPolygon, VT_BOOL, VTS_PVARIANT VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "DrawObj", DrawObj, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_R4)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetSprayBottle", SetSprayBottle, VT_BOOL, VTS_I2 VTS_COLOR VTS_COLOR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetObjNum", GetObjNum, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjType", GetSelObjType, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ReadReservedProperty", ReadReservedProperty, VT_BSTR, VTS_I2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SetReservedProperty", SetReservedProperty, VT_BOOL, VTS_I2 VTS_I2 VTS_BSTR)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjIndex", GetSelObjIndex, VT_I2, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "SelectObjs", SelectObjs, VT_BOOL, VTS_PI2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "AddSelectObjs", AddSelectObjs, VT_BOOL, VTS_PI2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "UnSelectObjs", UnSelectObjs, VT_BOOL, VTS_PI2 VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjPenColor", GetSelObjPenColor, VT_I4, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ReplayPage", ReplayPage, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "ModifySelObjFont", ModifySelObjFont, VT_BOOL, VTS_HANDLE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "InitializeFont", InitializeFont, VT_BOOL, VTS_HANDLE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjFont", GetSelObjFont, VT_HANDLE, VTS_NONE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "AddRgnObj", AddRgnObj, VT_BOOL, VTS_HANDLE)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjRegion", GetSelObjRegion, VT_HANDLE, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetSelObjIsErased", GetSelObjIsErased, VT_BOOL, VTS_I2)
		DISP_FUNCTION(CSCTDrawActivexCtrl, "GetObjRect", GetObjRect, VT_BOOL, VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_I2)
		DISP_STOCKPROP_FONT()
		//}}AFX_DISPATCH_MAP
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "InsertPage", dispidInsertPage, InsertPage, VT_BOOL, VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "ChangePageIndex", dispidChangePageIndex, ChangePageIndex, VT_BOOL, VTS_I2 VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "StartPlay", dispidStartPlay, StartPlay, VT_BOOL, VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "StopPlay", dispidStopPlay, StopPlay, VT_BOOL, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetPageThumbnail", dispidGetPageThumbnail, GetPageThumbnail, VT_HANDLE, VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "RefreshAll", dispidRefreshAll, RefreshAll, VT_EMPTY, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SaveObjToImage", dispidSaveObjToImage, SaveObjToImage,VT_HANDLE, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SetAlphaProperty", dispidSetAlphaProperty, SetAlphaProperty, VT_EMPTY, VTS_UI2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "BeginChart", dispidBeginChart, BeginChart, VT_EMPTY,VTS_BSTR VTS_COLOR)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "AddChartData", dispidAddChartData, AddChartData, VT_EMPTY, VTS_BSTR VTS_R4 VTS_COLOR)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "FinishChart", dispidFinishChart, FinishChart, VT_EMPTY, VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetChartDataNum", dispidGetChartDataNum, GetChartDataNum, VT_I2, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "ChangeSelChartData", dispidChangeSelChartData, ChangeSelChartData, VT_EMPTY, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetChartItemName", dispidGetChartItemName, GetChartItemName, VT_BSTR, VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetChartItemNumber", dispidGetChartItemNumber, GetChartItemNumber, VT_R4, VTS_UI2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetChartItemColor", dispidGetChartItemColor, GetChartItemColor, VT_COLOR, VTS_I2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetChartItemHeadLine", dispidGetChartItemHeadLine, GetChartItemHeadLine, VT_BSTR, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetChartItembkColor", dispidGetChartItembkColor, GetChartItembkColor, VT_COLOR, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SaveAsOtherTypes", dispidSaveAsOtherTypes, SaveAsOtherTypes, VT_I2, VTS_I2 VTS_BSTR)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetSelObjText", dispidGetSelObjText, GetSelObjText, VT_BSTR, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SetPolylineStyle", dispidSetPolylineStyle, SetPolylineStyle, VT_EMPTY, VTS_UI2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SetPolylineColors", dispidSetPolylineColors, SetPolylineColors, VT_EMPTY, VTS_COLOR VTS_COLOR VTS_COLOR VTS_COLOR)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "AddPolylineImage", dispidAddPolylineImage, AddPolylineImage, VT_EMPTY, VTS_BSTR)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SetLineKind", dispidSetLineKind, SetLineKind, VT_EMPTY, VTS_UI2)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "SetMultiTouch", dispidSetMultiTouch, SetMultiTouch, VT_EMPTY, VTS_BOOL)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "GetMultiTouchMode", dispidGetMultiTouchMode, GetMultiTouchMode, VT_BOOL, VTS_NONE)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "InsertDigiMemoObjs", dispidInsertDigiMemoObjs, InsertDigiMemoObjs, VT_BOOL, VTS_BSTR)
		DISP_FUNCTION_ID(CSCTDrawActivexCtrl, "CreateNewFile", dispidCreateNewFile, CreateNewFile, VT_BOOL, VTS_NONE)
		DISP_PROPERTY_NOTIFY_ID(CSCTDrawActivexCtrl, "gesture", dispidgesture, m_gesture, OngestureChanged, VT_I2)
		END_DISPATCH_MAP()


	/////////////////////////////////////////////////////////////////////////////
	// Event map

	BEGIN_EVENT_MAP(CSCTDrawActivexCtrl, COleControl)
		//{{AFX_EVENT_MAP(CSCTDrawActivexCtrl)
		EVENT_CUSTOM("ClickMenuRegion", ClickMenuRegion, VTS_NONE)
		EVENT_CUSTOM("DoneReplayPage", DoneReplayPage, VTS_NONE)
		EVENT_CUSTOM("ClickPlayRegion", ClickPlayRegion, VTS_NONE)
		EVENT_CUSTOM("PageChanged", PageChanged, VTS_NONE)
		EVENT_STOCK_CLICK()
		EVENT_STOCK_DBLCLICK()
		EVENT_STOCK_KEYDOWN()
		EVENT_STOCK_KEYPRESS()
		EVENT_STOCK_KEYUP()
		EVENT_STOCK_MOUSEDOWN()
		EVENT_STOCK_MOUSEMOVE()
		EVENT_STOCK_MOUSEUP()
		//}}AFX_EVENT_MAP
		EVENT_CUSTOM_ID("JudgeSelectedImage", eventidJudgeSelectedImage, JudgeSelectedImage, VTS_BOOL)
	END_EVENT_MAP()


	/////////////////////////////////////////////////////////////////////////////
	// Property pages

	// TODO: Add more property pages as needed.  Remember to increase the count!
	BEGIN_PROPPAGEIDS(CSCTDrawActivexCtrl, 4)
		PROPPAGEID(CSCTDrawActivexPropPage::guid)
		PROPPAGEID(CStylesDlg::guid)
		PROPPAGEID(CLSID_CColorPropPage)
		PROPPAGEID(CLSID_CFontPropPage)
	END_PROPPAGEIDS(CSCTDrawActivexCtrl)


	/////////////////////////////////////////////////////////////////////////////
	// Initialize class factory and guid

	IMPLEMENT_OLECREATE_EX(CSCTDrawActivexCtrl, "SCTDRAWACTIVEX.SCTDrawActivexCtrl.1",
		0x315a11ea, 0x2b5e, 0x4ea2, 0x91, 0x96, 0xad, 0xff, 0x1a, 0x86, 0xbf, 0x58)


		/////////////////////////////////////////////////////////////////////////////
		// Type library ID and version

		IMPLEMENT_OLETYPELIB(CSCTDrawActivexCtrl, _tlid, _wVerMajor, _wVerMinor)


		/////////////////////////////////////////////////////////////////////////////
		// Interface IDs

	const IID BASED_CODE IID_DSCTDrawActivex =
	{ 0x3452693d, 0x9a3d, 0x4b95, { 0xbb, 0xbf, 0xdc, 0xe3, 0x7c, 0xcd, 0, 0xe8 } };
	const IID BASED_CODE IID_DSCTDrawActivexEvents =
	{ 0xc8760923, 0x4dd6, 0x493e, { 0xa4, 0xba, 0x42, 0xd, 0xc7, 0x24, 0x55, 0xfe } };


	/////////////////////////////////////////////////////////////////////////////
	// Control type information

	static const DWORD BASED_CODE _dwSCTDrawActivexOleMisc =
		OLEMISC_ACTIVATEWHENVISIBLE |
		OLEMISC_SETCLIENTSITEFIRST |
		OLEMISC_INSIDEOUT |
		OLEMISC_CANTLINKINSIDE |
		OLEMISC_RECOMPOSEONRESIZE;

	IMPLEMENT_OLECTLTYPE(CSCTDrawActivexCtrl, IDS_SCTDRAWACTIVEX, _dwSCTDrawActivexOleMisc)


		/////////////////////////////////////////////////////////////////////////////
		// CSCTDrawActivexCtrl::CSCTDrawActivexCtrlFactory::UpdateRegistry -
		// Adds or removes system registry entries for CSCTDrawActivexCtrl

		BOOL CSCTDrawActivexCtrl::CSCTDrawActivexCtrlFactory::UpdateRegistry(BOOL bRegister)
	{
		// TODO: Verify that your control follows apartment-model threading rules.
		// Refer to MFC TechNote 64 for more information.
		// If your control does not conform to the apartment-model rules, then
		// you must modify the code below, changing the 6th parameter from
		// afxRegApartmentThreading to 0.

		if (bRegister)
			return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SCTDRAWACTIVEX,
			IDB_SCTDRAWACTIVEX,
			afxRegApartmentThreading,
			_dwSCTDrawActivexOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
		else
			return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}


	/////////////////////////////////////////////////////////////////////////////
	// CSCTDrawActivexCtrl::CSCTDrawActivexCtrl - Constructor

CSCTDrawActivexCtrl::CSCTDrawActivexCtrl():
	manipulationProc(NULL)
{
	InitializeIIDs(&IID_DSCTDrawActivex, &IID_DSCTDrawActivexEvents);

	//AllocConsole();
	//hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
	// TODO: Initialize your control's instance data here.	
	GdiplusStartupInput m_GdiplusStartupInput;
	GdiplusStartup(&m_GdiplusToken,&m_GdiplusStartupInput,NULL);
	//MouseInfo = 0;
	m_gesture =0;
	PageNum=0;
	IsInitial=false;
	SelectColor=0;
	MultiTouchMode=false;
	isMousetouch = false;
	isMultitouch = false;
	//pbUnit;
	pbBaseUnit=new CBaseUnit;
	pbUnit=pbBaseUnit;
	pbLineUnit=new CLineUnit;
	pbRectUnit=new CRectUnit;
	pbRoundRectUnit=new CRoundRectUnit;
	pbEllipseUnit=new CEllipseUnit;
	pbArcUnit=new CArcUnit;
	pbSectorUnit=new CSectorUnit;
	pbPolylineUnit=new CPolylineUnit;
	pbPenBrushUnit=new CPenBrushUnit;
	pbTextUnit=new CTextUnit;
	pbBmpUnit=new CBitmapUnit;
	pbCombination=new CCombination;
	pbEraseUnit=new CEraseUnit;
	pbEraseUnit->pErasureImage=NULL;
	pbDisjoint=new CDisjointUnit;
	pbPolygonUnit=new CPolygonUnit;
	pbRgnObjUnit=new CRgnObjUnit;

	pEdit=new CRichEditCtrl;

	xDC=new CDC;
	memDC=new CDC;
	temDC=new CDC;
	lbdown=false;
	//lbdown1= false;
	preOperations=0;//?
	pbUnit=NULL;
	ModifyText=false;//?
	BmpFromHandle=false;//?
	UndoDataInfo.SelectColor=-1;//初始化（用于撤销操作时暂存数据）的selectColor值
	///////////////////////////
	RotateRgn.X=0;
	RotateRgn.Y=0;
	RotateRgn.Width=0;
	RotateRgn.Height=0;
	StretchRgn.X=0;
	StretchRgn.Y=0;
	StretchRgn.Width=0;
	StretchRgn.Height=0;
	MenuRgn.X=0;
	MenuRgn.Y=0;
	MenuRgn.Width=0;
	MenuRgn.Height=0;
	MoveRgn.X=0;
	MoveRgn.Y=0;
	MoveRgn.Width=0;
	MoveRgn.Height=0;
	PlayRgn.X=0;
	PlayRgn.Y=0;
	PlayRgn.Width=0;
	PlayRgn.Height=0;

	/////////////////////////////////////////////////////////////////////////////////
	m_uNumContacts = 0;
	m_bInObject = FALSE;
	m_object = new (std::nothrow) CCoreObject(pEdit->GetSafeHwnd(), 0);

	if(m_object != NULL)
	{
		m_object->Initialize();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexCtrl::~CSCTDrawActivexCtrl - Destructor

CSCTDrawActivexCtrl::~CSCTDrawActivexCtrl()
{
	// TODO: Cleanup your control's instance data here.
	GdiplusShutdown(m_GdiplusToken);
	//UnregisterTouchWindow();

	/*delete pbUnit;
	delete pbLineUnit;
	delete pbRectUnit;
	delete pbRoundRectUnit;
	delete pbEllipseUnit;
	delete pbArcUnit;
	delete pbSectorUnit;
	delete pbPolylineUnit;
	delete pbPenBrushUnit;
	delete pbTextUnit;
	delete pbBmpUnit;
	delete pbCombination;
	delete pbEraseUnit;
	delete ptsPoint;
	delete pEdit;*/

	if (m_object != NULL)
	{
		delete m_object;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexCtrl::OnDraw - Drawing function

void CSCTDrawActivexCtrl::OnDraw(
	CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	if(SelObjList.size()==1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjSelected();
	if(SelObjList.size()>1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjsSelected();
	if(IsWindow(pEdit->GetSafeHwnd()))
	{
		pEdit->RedrawWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexCtrl::DoPropExchange - Persistence support

void CSCTDrawActivexCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	PX_Short(pPX,_T("Mode Operations"),m_operations,0);
	PX_Short(pPX,_T("Pen Width"),m_penWidth,5);
	PX_Color(pPX,_T("Pen Color"),m_penColor,RGB(0,0,0));
	PX_Color(pPX,_T("Fill Color"),m_fillColor,RGB(255,255,255));
	PX_Color(pPX,_T("Font Color"),m_fontColor,RGB(0,0,0));
	PX_String(pPX,_T("ReadFilePath"),m_filePath,"c:\\text.sct");
	PX_String(pPX,_T("TemBmpLeftpath"),m_temBmpPath,"c:\\Program Files\\BmpTemDir");
	PX_String(pPX,_T("bkBmp"),m_bkBmp,NULL);
	PX_Color(pPX,_T("BackGroundColor"),m_bkColor,RGB(255,255,255));
	PX_Short(pPX,_T("LineStyles"),m_lineStyle,0);
	PX_Short(pPX,_T("FillStyles"),m_fillStyle,0);
	PX_Color(pPX,_T("foreColor"),m_foreColor,RGB(255,0,0));
	// TODO: Call PX_ functions for each persistent custom property.
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexCtrl::OnResetState - Reset control to default state

void CSCTDrawActivexCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexCtrl message handlers

void CSCTDrawActivexCtrl::OnOperationsChanged() //当操作改变时触发此函数
{
	// TODO: Add notification handler code
	if(preOperations==11)
	{
		if(pbUnit!=NULL)
		{
			if(IsWindow(pEdit->GetSafeHwnd()))
			{
				ChangeText();
				pEdit->DestroyWindow();
				CRgn InitRgn;
				InitRgn.CreateRectRgn(0,0,0,0);
				HRGN hRgn=InitRgn.operator HRGN();
				hRgn=pbTextUnit->GetRedrawHRGN(memDC->m_hDC,DataArray,pDataInfo);
				Region Rgn(hRgn);
				Matrix matrix;
				matrix.Translate(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
				matrix.Rotate(pDataInfo.RotateAngle);
				matrix.Scale(pDataInfo.xScale,pDataInfo.yScale);
				Rgn.Transform(&matrix);
				matrix.Reset();
				Graphics graphics(memDC->m_hDC);
				SelectClipRgn(memDC->m_hDC,Rgn.GetHRGN(&graphics));
				memDC->FillSolidRect(&WindowRect,m_bkColor);
				SetBackGroundBmp();	
				ReDraw();
				SelectClipRgn(memDC->m_hDC,NULL);
				DeleteObject(hRgn);
			}
			pbUnit=NULL;
			BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);			
		}
	}
	if(preOperations==9&&m_operations!=9)
	{
		if(IsWindow(pEdit->GetSafeHwnd()))
		{
			FinishText(CPoint(0,0));
			pEdit->DestroyWindow();
		}
		pbUnit=NULL;	
		BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	}

	if(preOperations==9&&m_operations==9)
	{
		if(IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
		}
		return;
	}

	if(m_operations==15)
	{
		//InitErasureImage();
	}
	temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
	preOperations=m_operations;
	////////////////////////////////////////////
	ClearDataInfo();
	SelObjList.clear();
	MouseCur=NULL;
	SelObjList.clear();
	RotateRgn.X=0;
	RotateRgn.Y=0;
	RotateRgn.Width=0;
	RotateRgn.Height=0;
	StretchRgn.X=0;
	StretchRgn.Y=0;
	StretchRgn.Width=0;
	StretchRgn.Height=0;
	MenuRgn.X=0;
	MenuRgn.Y=0;
	MenuRgn.Width=0;
	MenuRgn.Height=0;
	MoveRgn.X=0;
	MoveRgn.Y=0;
	MoveRgn.Width=0;
	MoveRgn.Height=0;
	PlayRgn.X=0;
	PlayRgn.Y=0;
	PlayRgn.Width=0;
	PlayRgn.Height=0;
	////////////////////////////////////////////
	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::OnPenWidthChanged() //笔宽改变时触发此函数
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::OnPenColorChanged() //笔颜色改变时触发此函数
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

int CSCTDrawActivexCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
		
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
		
	//BOOL isSucess = false;
	if(SearchUsbDevice(0x0483))
		RegistMouse();
		
	/*isSucess =(twConnectGestureServer() == TWGL_NOERROR);
	if(!isSucess)
	{
		::Sleep(500);
		isSucess = (twConnectGestureServer() == TWGL_NOERROR);

	}
	if(!isSucess)
		goto end;

	isSucess =(twRegisterGesture(1,gestureDown)==TWGL_NOERROR);
	if(!isSucess)
	{
		::Sleep(500);
		isSucess =(twRegisterGesture(1,gestureDown)==TWGL_NOERROR);
	}
	if(!isSucess)
		goto end;
	isSucess =(twRegisterGesture(1,gestureUp)==TWGL_NOERROR);
	if(!isSucess)
	{

		::Sleep(500);
		isSucess =(twRegisterGesture(1,gestureUp)==TWGL_NOERROR);
	}*/

end:    RegisterTouchWindow();
	IsInitial=true;
	ErasedRgnData=NULL;
	/////////////////////////////////////
	CString str="image/bmp";
	int len=str.GetLength();
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=str.AllocSysString();
	GetEncoderClsid(WideStr,&BmpClsid);//获取image/bmp图像编码器的clsid
	SysFreeString(WideStr);
	////////////////////////////////////////////////
	memset(bkBmp,'\0',100);
	WindowRect.left=0;
	WindowRect.top=0;
	WindowRect.right=GetSystemMetrics(SM_CXSCREEN);
	WindowRect.bottom=GetSystemMetrics(SM_CYSCREEN);
	xDC=GetDC();
	//HWND hwnd = this->m_hWnd;
	hDC=xDC->GetSafeHdc();
	memDC->CreateCompatibleDC(xDC);
	memBmp.CreateCompatibleBitmap(xDC,WindowRect.Width(),WindowRect.Height());
	memDC->SelectObject(&memBmp);
	temDC->CreateCompatibleDC(memDC);
	temBmp.CreateCompatibleBitmap(memDC,WindowRect.Width(),WindowRect.Height());
	temDC->SelectObject(&temBmp);
	memDC->FillSolidRect(&WindowRect,m_bkColor);

       
	
	//////初始化字体/////////////////////
	CFontHolder fh(FALSE);
	static const FONTDESC _fontdescHeading =
	{sizeof (FONTDESC), OLESTR("Arial"), FONTSIZE(20), FW_BOLD, ANSI_CHARSET, FALSE, FALSE, FALSE };
	fh.InitializeFont (&_fontdescHeading);
	SetFont(fh.GetFontDispatch());
	CFontHolder &rFontHolder=InternalGetFont(); 
	HFONT hfont= rFontHolder.GetFontHandle(); 
	CFont *pfont=CFont::FromHandle(hfont);
	pfont->GetLogFont(&DefaultFont);
	CreatePage();
	//////////////////////////////////////////////////////////////////////////////////////////////
	HRSRC hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Stretch), "PNG");
	HGLOBAL hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	DWORD nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	HGLOBAL hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	void* pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	StretchImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Play), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	PlayImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Menu), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	MenuImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Rotate), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	RotateImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_ChangePoint), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	ChangeImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Head), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	HeadImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Left), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	LeftImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);

	hSource=FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_Right), "PNG");
	hGlobal=LoadResource(AfxGetResourceHandle(), hSource);
	LockResource(hGlobal);
	nSize=SizeofResource(AfxGetResourceHandle(), hSource);
	hGlo = GlobalAlloc(GMEM_MOVEABLE, nSize);
	pData = GlobalLock(hGlo);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlo);
	pStream = NULL;
	CreateStreamOnHGlobal(hGlo, TRUE, &pStream);
	RightImage= Image::FromStream(pStream, FALSE);
	FreeResource(hGlo);
	///////////////////////////////////////////////////////////////////////////////////////////////
	InitOpengl();
	return 0;
}

void CSCTDrawActivexCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

BOOL Start=true;
void CSCTDrawActivexCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	////////////////////////////////////////////////////////////
	CPoint tPoint;
	RectF rectf;
	int Index;
	int Size;
	DataInfo temDataInfo;
	Region IniRgn(Rect(0,0,0,0));

	if(MultiInfoArray.size()>0)
		goto end;
	eraserUp = false;
	switch(m_operations)
	{
	case 0:         //view mode  do nothing
		SelectOperations(point);
		break;
	case 1:         //add line
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);//保存当前内存DC中的所有数据
		ClearDataInfo();
		AddLine(point);
		break;
	case 2:         //add rectangle
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
		ClearDataInfo();
		AddRect(point);
		break;
	case 3:         //add round rectangle
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
		ClearDataInfo();
		AddRoundRect(point);
		break;
	case 4:         //add ellipse
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
		ClearDataInfo();
		AddEllipse(point);
		break;
	case 5:         //add arc
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
		if(ptSector.size()==0)
		{
			ClearDataInfo();
			ptSector.push_back(point);
		}
		ptSector.push_back(point);
		DrawCompass();
		break;
	case 6:         //add sector
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
		if(ptSector.size()==0)
		{
			ClearDataInfo();
			ptSector.push_back(point);
		}
		ptSector.push_back(point);
		DrawCompass();
		break;
	case 7:         //add PenBrush
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		ClearDataInfo();
		///////////////////////
		isMousetouch = true;
		AddPenBrush(point);
		break;
	case 8:         //add polyline
		BitBlt(temDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		ClearDataInfo();
		////////////////////////////
		isMousetouch = true;
		AddPolyline(point);
		break;
	case 9:        //add text
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		if(true) 
		{		
			CRect rect(point.x,point.y,point.x+3,point.y+20); 
			if(IsWindow(pEdit->GetSafeHwnd()))
				FinishText(point);
			pEdit->DestroyWindow();
			pEdit->CreateEx(WS_EX_TRANSPARENT,WS_VISIBLE|WS_CHILD|ES_MULTILINE|ES_WANTRETURN,rect,this,IDC_Text);
			pEdit->SetEventMask(pEdit->GetEventMask()|ENM_CHANGE);
			ClearDataInfo();
			AddText(point);
			TextReturn=false;
		}
		break;
	case 10:        //add image
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		if(BmpName!="")
		{
			temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
			ClearDataInfo();
			AddBitmap(point);
		}
		if(BmpFromHandle)
		{
			pDataInfo.StartPoint=point;
		}
		break;
	case 11:        //edit/select model 
		if(IsWindow(pEdit->GetSafeHwnd()))
		{
			ChangeText();
			pEdit->DestroyWindow();
		}
		if(nFlags>1)
			AddSelObj(point);//表示通过键盘的Ctrl,shift
		else
		{
			if(MouseCur==NULL)//当m_operations=11时，首次鼠标按下去时经过此处
			{
				BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
				SelectOperations(point);		
			}
			else
				SetCursor(MouseCur);//此处表示之前已经选择好了对象，现在想要实现移动，选择，拉伸，播放等操作
				//SetCursor(AfxGetApp()->LoadCursor(IDC_PenCur));
				   
		}
		temPoint=point;
		textPoint=point;
		break;
	case 14:  //区域擦除
		temPoint=point;
		break;
	case 15:  //点擦除
		if(!lbdown)
		{
			DisplayConsole("4",sizeof("4"));
		temPoint=point; 
		MouseCur=AfxGetApp()->LoadCursor(IDC_Eraser);
		SetCursor(MouseCur);
		/////////////////////////////
		//pErasedRegion=IniRgn.Clone();
		pbEraseUnit->OnMouseDown(nFlags,point);
		}
		/////////////////////////////
		break;
	case 16: //对象擦除
		ObjectErase(point);
		break;
	case 17://喷筒
		MouseCur=AfxGetApp()->LoadCursor(IDC_SprayBottle);
		SetCursor(MouseCur);
		SelectOperations(point);
		Size=SelObjList.size();
		if(Size==1)
		{
			temDataInfo=SelObjList[0].SelDataInfo;
			Size=temDataInfo.ComArray.size();
			for(Index=0;Index<Size;Index++)
			{
				DataArray[temDataInfo.ComArray[Index].ObjIndex].FillColor=SB_FillColor;
				DataArray[temDataInfo.ComArray[Index].ObjIndex].FillStyle=SB_FillStyle;
				DataArray[temDataInfo.ComArray[Index].ObjIndex].foreColor=SB_ForeColor;
			}
			temDataInfo.FillColor=SB_FillColor;
			temDataInfo.FillStyle=SB_FillStyle;
			temDataInfo.foreColor=SB_ForeColor;
			DataArray[SelObjList[0].Index]=temDataInfo;
			memDC->FillSolidRect(&WindowRect,m_bkColor);
			SetBackGroundBmp();	
			ReDraw();
			BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);  
		}
		SelObjList.clear();
		break;
	case 18://Polygon
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
		if(Start)
		{
			ClearDataInfo();
			AddPolygon(point);
			Start=false;
		}
		else
		{
			int Size=ptsPoint.size();
			if(Size==0)
			{
				Start=true;
				return;
			}
			CPoint tPoint=ptsPoint[Size-1].point;
			CRect rect(tPoint.x,tPoint.y,tPoint.x,tPoint.y);
			rect.InflateRect(10,10);
			if(rect.PtInRect(point))
				return;
			PointNum++;		
			pbPolygonUnit->DrawPolygon(memDC,point);
			pbPolygonUnit->StartPoint=point;
			if(point.x<pDataInfo.StartPoint.x)
				pDataInfo.StartPoint.x=point.x;
			if(point.y<pDataInfo.StartPoint.y)
				pDataInfo.StartPoint.y=point.y;
			if(point.x>pDataInfo.EndPoint.x)
				pDataInfo.EndPoint.x=point.x;
			if(point.y>pDataInfo.EndPoint.y)
				pDataInfo.EndPoint.y=point.y;
			temSavePoint addPoint;
			addPoint.point=point;
			addPoint.alpha=0;
			addPoint.preWidth=0;
			ptsPoint.push_back(addPoint);
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);	
		}
		break;
	case 19:
		MouseCur=AfxGetApp()->LoadCursor(IDC_HandCur);
		SetCursor(MouseCur);
		temPoint=point;
		break;
	}
	lbdown=true;
	//DisplayConsole("4",sizeof("4"));
end:	COleControl::OnLButtonDown(nFlags, point);
}

void CSCTDrawActivexCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//DisplayConsole("6",sizeof("6"));
	if(MultiInfoArray.size()>0/*||(!lbdown)*/)
		goto end;
	if(isgestureDown)
	{
		//OnLButtonDown(nFlags,point);
		//SetCursorPos(point.x+1,point.y+1);
		//for(int i=0;i<10;i++)
		mouse_event(2,1,1,0,0);
		//mouse_event(1,1,1,0,0);
		//for(int i=0;i<10;i++)

		mouse_event(4,1,1,0,0);
		//goto end;
        // MouseCur=AfxGetApp()->LoadCursor(IDC_Edit);
			//SetCursor(MouseCur);
		//DisplayConsole("Z",sizeof("Z"));
	}
	if(eraserUp)
	{
		eraserUp = false;
		Graphics mGraphics(memDC->m_hDC);
			if(pbEraseUnit->OnMouseUp(nFlags,point))
			{
				HRGN hRgn=pbEraseUnit->EraseOperation(hDC,EraserWidth);
				if(hRgn!=NULL)
				{
					ErasedRgnSize=Region::FromHRGN(hRgn)->GetDataSize();
					ErasedRgnData=new BYTE[ErasedRgnSize];
					Region::FromHRGN(hRgn)->GetData(ErasedRgnData,ErasedRgnSize);
					DotEraseOperation();//保证在移动，放大，旋转时，被擦部分也能体现出来
				}
			}
			PageChanged();//事件触发
	}
	else
	{
	switch(m_operations)
	{
	case 0:         //view mode  do nothing
		break;
	case 1: //add line
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(NULL);
		FinishLine(point);
		PageChanged();//事件触发
		break;
	case 2:         //add rectangle
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		FinishRect(point);
		PageChanged();//事件触发
		break;
	case 3:         //add round rectangle
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		FinishRoundRect(point);
		PageChanged();//事件触发
		break;
	case 4:         //add ellipse
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		FinishEllipse(point);
		PageChanged();//事件触发
		break;
	case 5:         //add arc
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		if(ptSector.size()==3)
			FinishArc(point);
		PageChanged();//事件触发
		break;
	case 6:         //add sector
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		if(ptSector.size()==3)
			FinishSector(point);
		PageChanged();//事件触发
		break;
	case 7:         //add penBrush
		//DisplayConsole("j",sizeof("j"));
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		FinishPenBrush(point);
		PageChanged();//事件触发
		break;
	case 8:         //add polyline
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		FinishPolyline(point);
		BitBlt(temDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);//此处为单点书写后在多点书写时要保存单点的书写笔迹
		PageChanged();//事件触发
		break;
	case 9:        //add text
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		pEdit->SetFocus();
		PageChanged();//事件触发
		break;
	case 10:        //add image
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		if(lbdown&&(BmpName!=""||BmpFromHandle))
			FinishBitmap(point);
		PageChanged();//事件触发
		break;
	case 11:        //edit/select mode
		SetROP2(hDC,R2_NOT);
		DrawFocusRect(hDC,&FocusRect);
		///////////////////////////////////////
		if(SelObjList.size())//当选择到了对象时会经过此处
		{
			int Size=SelObjList.size();
			if(Size==1)//所选择到的对象为一个时经过此处
			{
				if(MouseCur==AfxGetApp()->LoadCursor(IDC_Change))//为对象图形形状再编辑时经过
					FinishChangePolygon();
				ObjSelected();//单个对象被选择到，当要UP时完成此次动作的保存到UndoArray向量中去
			}
			else
			{
				CaculateSelObjsRgn();//当所选到的对象数大于1时，首先要计算出所选到的对象的区域
				ObjsSelected();
			}
		}
		else
		{
			CRect rect;
			rect.CopyRect(&FocusRect);
			int Size=DataArray.size();
			for(int Index=0;Index<Size;Index++)
			{
				if(rect.PtInRect(DataArray[Index].CenterPoint)&&!DataArray[Index].IsErased&&!DataArray[Index].IsCombination&&!DataArray[Index].IsDisjoint)
				{		
					SelObj AddSelObj;
					AddSelObj.SelDataInfo=DataArray[Index];
					AddSelObj.Index=Index;
					SelObjList.push_back(AddSelObj);
				}
			}
			Size=SelObjList.size();
			if(Size==1)
				ObjSelected();
			if(Size>1)
			{
				CaculateSelObjsRgn();
				ObjsSelected();	
			}
		}
		if(MouseCur==AfxGetApp()->LoadCursor(IDC_Move)&&SelObjList.size()==1)
		{
			if(SelObjList[0].SelDataInfo.OperationKind==8&&textPoint==point)
			{
				ClearDataInfo();
				pDataInfo=SelObjList[0].SelDataInfo;
				pEdit->DestroyWindow();
				CPoint sPoint=pDataInfo.StartPoint;
				Graphics graphics(hDC);
				wchar_t *WideStr;
				CString temStr=pDataInfo.SaveStr;
				int len=temStr.GetLength();
				WideStr=new wchar_t[len];
				WideStr=temStr.AllocSysString();
				PointF pointf(sPoint.x,sPoint.y);
				RectF rectf;
				const StringFormat *format=StringFormat::GenericTypographic();
				Font temFont(hDC,&pDataInfo.logfontw);
				graphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
				SysFreeString(WideStr);
				Font fontw(hDC,&pDataInfo.logfontw);
				LOGFONTA logfont;
				fontw.GetLogFontA(&graphics,&logfont);
				CHARFORMAT cf;
				ChangeLogFontToCharFormat(cf,logfont,pDataInfo.FontColor);
				CRect rect(sPoint.x,sPoint.y,rectf.X+rectf.Width,rectf.Y+rectf.Height);
				pEdit->Create(WS_VISIBLE|WS_CHILD|ES_MULTILINE|ES_WANTRETURN,rect,this,IDC_Text);
				pEdit->SetDefaultCharFormat(cf);
				pEdit->SetEventMask(pEdit->GetEventMask()|ENM_CHANGE);
				SetDlgItemText(IDC_Text,pDataInfo.SaveStr);
				TextReturn=false;
				pEdit->SendMessage(EN_CHANGE,0,0);
				pEdit->SetFocus();
				pEdit->SetSel(-1,-1);	

			}	
		}
		if(MouseCur==AfxGetApp()->LoadCursor(IDC_Menu))
		{
			ClickMenuRegion();
			MouseCur=NULL;
		}
		if(MouseCur==AfxGetApp()->LoadCursor(IDC_PlayCur))
		{
			ClickPlayRegion();
			MouseCur=NULL;
		}
		if(MouseCur==AfxGetApp()->LoadCursor(IDC_Change))
			MouseCur=NULL;
		FocusRect.SetRect(0,0,0,0);
		PageChanged();//事件触发
		break;
	case 14:   //区域擦除	
		SetROP2(hDC,R2_NOT);
		DrawFocusRect(hDC,&FocusRect);
		ErasedRegion(FocusRect.left,FocusRect.top,FocusRect.right,FocusRect.bottom);
		FocusRect.SetRect(0,0,0,0);
		PageChanged();//事件触发
		break;
	case 15:  //擦除
		if(1)
		{
			DisplayConsole("6",sizeof("6"));
			Graphics mGraphics(memDC->m_hDC);
			if(pbEraseUnit->OnMouseUp(nFlags,point))
			{
				HRGN hRgn;
				if(isgestureDown)
					hRgn=pbEraseUnit->EraseOperation(hDC,EraserWidth);
				else
					hRgn=pbEraseUnit->EraseOperation(hDC,20);
				if(hRgn!=NULL)
				{
					ErasedRgnSize=Region::FromHRGN(hRgn)->GetDataSize();
					ErasedRgnData=new BYTE[ErasedRgnSize];
					Region::FromHRGN(hRgn)->GetData(ErasedRgnData,ErasedRgnSize);
					DotEraseOperation();//保证在移动，放大，旋转时，被擦部分也能体现出来
				}
			}
			PageChanged();//事件触发
		}
		break;
	case 17://SprayBottle
		MouseCur=AfxGetApp()->LoadCursor(IDC_SprayBottle);
		SetCursor(MouseCur);
		break;
	case 18://Polygon
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		PageChanged();//事件触发
		break;
	case 19:
		MouseCur=AfxGetApp()->LoadCursor(IDC_HandCur);
		SetCursor(MouseCur);
		InitErasureImage();
		PageChanged();//事件触发
		break;
	}
	}
    	
		
end:   lbdown=false;
	//lbdown1 = false;
    COleControl::OnLButtonUp(nFlags, point);
}

void CSCTDrawActivexCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		
	if(MultiInfoArray.size()>0)
		goto end;
		
	switch(m_operations)
	{
	case 0:         //view mode  do nothing
		break;
	case 1:         //add line
		if(lbdown)
		{  

			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);//目的是让整个鼠标在移动过程中，始终保持memDC为鼠标在左键点击下去的状态
			pbLineUnit->DrawGraphics(memDC,pDataInfo,point);
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);//将鼠标移动的最新过程

		}
		break;
	case 2:         //add rectangle
		if(lbdown)
		{  
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			pbRectUnit->DrawGraphics(memDC,pDataInfo,point);
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		}
		break;
	case 3:   //add round rectangle
		if(lbdown)
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			pbRoundRectUnit->DrawGraphics(memDC,pDataInfo,point);
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		}
		break;
	case 4:         //add ellipse
		if(lbdown)
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			pbEllipseUnit->DrawGraphics(memDC,pDataInfo,point);
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		}
		break;
	case 5:         //add arc
		if(lbdown)
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			ptSector[ptSector.size()-1]=point;
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			/////////////////////////////////////////////////////////////////////////////////////
			DrawCompass();
			///////////////////////////////////////////////////////////////////////////////////////
			//pbSectorUnit->DrawGraphics(memDC,pDataInfo,point);	 
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
		}
		break;
	case 6:         //add sector
		if(lbdown)
		{  
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			ptSector[ptSector.size()-1]=point;
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			/////////////////////////////////////////////////////////////////////////////////////
			DrawCompass();
			///////////////////////////////////////////////////////////////////////////////////////
			//pbSectorUnit->DrawGraphics(memDC,pDataInfo,point);	 
			BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
		}
		break;
	case 7:         //penBrsuh
		if(lbdown)
		{
			isMousetouch = true;
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			if(ptsPoint.size()==0)
				AddPenBrush(point);
			else
			{
				PointNum++;
				pbPenBrushUnit->DrawPenBrush(memDC,point,ptsPoint);
				pbPenBrushUnit->StartPoint=point;
				if(point.x<pDataInfo.StartPoint.x)
					pDataInfo.StartPoint.x=point.x;
				if(point.y<pDataInfo.StartPoint.y)
					pDataInfo.StartPoint.y=point.y;
				if(point.x>pDataInfo.EndPoint.x)
					pDataInfo.EndPoint.x=point.x;
				if(point.y>pDataInfo.EndPoint.y)
					pDataInfo.EndPoint.y=point.y;
				BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
			}
		}
		break;
	case 8:         //add polyline
		if(lbdown)
		{
			isMousetouch = true;
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			if(ptsPoint.size()==0)
				AddPolyline(point);
			else
			{
				PointNum++;
				BitBlt(memDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),temDC->m_hDC,0,0,SRCCOPY);
				pbPolylineUnit->DrawPolyline(memDC,point,ptsPoint);
				pbPolylineUnit->StartPoint=point;//?好像可以注销掉
				if(point.x<pDataInfo.StartPoint.x)//决定选择时所要绘制的外框的x，y，范围及坐标
					pDataInfo.StartPoint.x=point.x;
				if(point.y<pDataInfo.StartPoint.y)
					pDataInfo.StartPoint.y=point.y;
				if(point.x>pDataInfo.EndPoint.x)
					pDataInfo.EndPoint.x=point.x;
				if(point.y>pDataInfo.EndPoint.y)
					pDataInfo.EndPoint.y=point.y;
				BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);//及时显示鼠标移动轨迹
			}
		}
		break;
	case 9:  //add text
		MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
		SetCursor(MouseCur);
		break;
	case 10: //add image
		if(lbdown&&(BmpName!=""||BmpFromHandle))
		{  
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			pbBmpUnit->DrawGraphics(memDC,pDataInfo,point);
			BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
		}
		break;
	case 11:  //edit/select mode
		if(MouseCur==NULL)
			MouseCur=GetMouseCursor(point);
		if(lbdown)
		{	
			if(MouseCur==AfxGetApp()->LoadCursor(IDC_Move))
				MoveOperations(point);//实现对象移动操作
			if(MouseCur==AfxGetApp()->LoadCursor(IDC_Rotate))
				RotateOperations(point);//实现所选对象旋转操作
			if(MouseCur==AfxGetApp()->LoadCursor(IDC_Edit))
				StretchOperations(point); //实现所选对象拉伸操作
			if(MouseCur==AfxGetApp()->LoadCursor(IDC_Change))
				ChangeOperations(point);//实现所选对象图形形状再编辑操作
			if(MouseCur!=NULL)
				temPoint=point;
			else
			{
				SetROP2(hDC,R2_NOT);//当前绘制的像素值设为屏幕像素值得反，这样可以覆盖掉上次的绘图
				DrawFocusRect(hDC,&FocusRect);
				CRect rect(temPoint,point);
				rect.NormalizeRect();
				DrawFocusRect(hDC,&rect);
				FocusRect.CopyRect(&rect);    
			}

		}
		else
		{
			MouseCur=GetMouseCursor(point);
			if(MouseCur!=NULL)
				SetCursor(MouseCur);
		}
		break;
	case 14:       //区域擦除
		if(lbdown)
		{
			SetROP2(hDC,R2_NOT);
			DrawFocusRect(hDC,&FocusRect);
			CRect rect(temPoint,point);
			rect.NormalizeRect();
			DrawFocusRect(hDC,&rect);
			FocusRect.CopyRect(&rect);  
		}
		break;
	case 15://Erase
		if(lbdown)
		{
				
			if(isgestureDown)
			{
				if(!isSaved && !isMultitouch)
				{
					//DisplayConsole("5",sizeof("5"));
					switch(m_operation1)
					{
					case 1://add line
						//if(abs(point.x-pDataInfo.StartPoint.x)>50 &&abs(point.y-pDataInfo.StartPoint.y)>50)
							//FinishLine(point);
						/*else
							SelectColor--;*/
						//OnOperationsChanged();
						PageChanged();
						break;
					case 2://add rectangle
						//if(abs(point.x-pDataInfo.StartPoint.x)>50 &&abs(point.y-pDataInfo.StartPoint.y)>50)
							//FinishRect(point);
						/*else
							SelectColor--;*/
						//OnOperationsChanged();
						break;
					case 3://add round rectangle
						//if(abs(point.x-pDataInfo.StartPoint.x)>50 &&abs(point.y-pDataInfo.StartPoint.y)>50)
						//FinishRoundRect(point);
						/*else
							SelectColor--;*/
						//OnOperationsChanged();
						break;
					case 4://add ellipse
						//if(abs(point.x-pDataInfo.StartPoint.x)>50 &&abs(point.y-pDataInfo.StartPoint.y)>50)
						//FinishEllipse(point);
						/*else
							SelectColor--;*/
						//OnOperationsChanged();
						break;
					case 5://add arc
						if(ptSector.size()==3)
						{	
						FinishArc(point);
						OnOperationsChanged();
						}
						break;
					case 6://add sector
						if(ptSector.size()==3)
						{
							FinishSector(point);
							OnOperationsChanged();
						}
						break;
					case 7://penBrsuh
						//FinishPenBrush(point);
						OnOperationsChanged();
						break;
					case 8://add polyline
						//FinishPolyline(point);
						OnOperationsChanged();
						break;
					case 9://add text
						pEdit->SetFocus();
						OnOperationsChanged();
						break;
					case 10://add image
						if(lbdown&&(BmpName!=""||BmpFromHandle))
						{
							FinishBitmap(point);
							//OnOperationsChanged();
						}
						break;
					case 11://edit/select mode
						{
							SetROP2(hDC,R2_NOT);
							DrawFocusRect(hDC,&FocusRect);
							///////////////////////////////////////
							if(SelObjList.size())
							{
								int Size=SelObjList.size();
								if(Size==1)
								{
									if(MouseCur==AfxGetApp()->LoadCursor(IDC_Change))
										FinishChangePolygon();
									ObjSelected();
								}
								else
								{
									CaculateSelObjsRgn();
									ObjsSelected();
								}
							}
							else
							{
								CRect rect;
								rect.CopyRect(&FocusRect);
								int Size=DataArray.size();
								for(int Index=0;Index<Size;Index++)
								{
									if(rect.PtInRect(DataArray[Index].CenterPoint)&&!DataArray[Index].IsErased&&!DataArray[Index].IsCombination&&!DataArray[Index].IsDisjoint)
									{		
										SelObj AddSelObj;
										AddSelObj.SelDataInfo=DataArray[Index];
										AddSelObj.Index=Index;
										SelObjList.push_back(AddSelObj);
									}
								}
								Size=SelObjList.size();
								if(Size==1)
									ObjSelected();
								if(Size>1)
								{
									CaculateSelObjsRgn();
									ObjsSelected();	
								}
							}
							if(MouseCur==AfxGetApp()->LoadCursor(IDC_Move)&&SelObjList.size()==1)
							{
								if(SelObjList[0].SelDataInfo.OperationKind==8&&textPoint==point)
								{
									ClearDataInfo();
									pDataInfo=SelObjList[0].SelDataInfo;
									pEdit->DestroyWindow();
									CPoint sPoint=pDataInfo.StartPoint;
									Graphics graphics(hDC);
									wchar_t *WideStr;
									CString temStr=pDataInfo.SaveStr;
									int len=temStr.GetLength();
									WideStr=new wchar_t[len];
									WideStr=temStr.AllocSysString();
									PointF pointf(sPoint.x,sPoint.y);
									RectF rectf;
									const StringFormat *format=StringFormat::GenericTypographic();
									Font temFont(hDC,&pDataInfo.logfontw);
									graphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
									SysFreeString(WideStr);
									Font fontw(hDC,&pDataInfo.logfontw);
									LOGFONTA logfont;
									fontw.GetLogFontA(&graphics,&logfont);
									CHARFORMAT cf;
									ChangeLogFontToCharFormat(cf,logfont,pDataInfo.FontColor);
									CRect rect(sPoint.x,sPoint.y,rectf.X+rectf.Width,rectf.Y+rectf.Height);
									pEdit->Create(WS_VISIBLE|WS_CHILD|ES_MULTILINE|ES_WANTRETURN,rect,this,IDC_Text);
									pEdit->SetDefaultCharFormat(cf);
									pEdit->SetEventMask(pEdit->GetEventMask()|ENM_CHANGE);
									SetDlgItemText(IDC_Text,pDataInfo.SaveStr);
									TextReturn=false;
									pEdit->SendMessage(EN_CHANGE,0,0);
									pEdit->SetFocus();
									pEdit->SetSel(-1,-1);	

								}	
							}
							if(MouseCur==AfxGetApp()->LoadCursor(IDC_Menu))
							{
								ClickMenuRegion();
								MouseCur=NULL;
							}
							if(MouseCur==AfxGetApp()->LoadCursor(IDC_PlayCur))
							{
								ClickPlayRegion();
								MouseCur=NULL;
							}
							if(MouseCur==AfxGetApp()->LoadCursor(IDC_Change))
								MouseCur=NULL;
							FocusRect.SetRect(0,0,0,0);
						}
						OnOperationsChanged();
						break;
					case 14://区域擦除
						SetROP2(hDC,R2_NOT);
						DrawFocusRect(hDC,&FocusRect);
						ErasedRegion(FocusRect.left,FocusRect.top,FocusRect.right,FocusRect.bottom);
						FocusRect.SetRect(0,0,0,0);
						OnOperationsChanged();
						break;
					case 17://SprayBottle
							
						break;
					case 18://Polygon
						break;
					case 19://拖屏
						break;
					}
					temPoint=point; 
					MouseCur=AfxGetApp()->LoadCursor(IDC_Eraser);
					SetCursor(MouseCur);

					pbEraseUnit->OnMouseDown(nFlags,point);
					isSaved = true;
				}
				if(pbEraseUnit->OnMouseMove(nFlags,point))
				{
					DisplayConsole("5",sizeof("5"));
					pbEraseUnit->OnRender(memDC,EraserWidth);
					//DisplayConsole("k",sizeof("k"));
					BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
					//DisplayConsole("l",sizeof("l"));
				}
			}
			else
			{
				MouseCur=AfxGetApp()->LoadCursor(IDC_Eraser);
				SetCursor(MouseCur);
				if(pbEraseUnit->OnMouseMove(nFlags,point))
				{
					pbEraseUnit->OnRender(memDC,20);
					BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
				}
			}
		}
		break;
	case 17://SprayBottle
		MouseCur=AfxGetApp()->LoadCursor(IDC_SprayBottle);
		SetCursor(MouseCur);
		break;
	case 18://Polygon
		if(!Start)
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_PenCur);
			SetCursor(MouseCur);
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			pbPolygonUnit->DrawPolygon(memDC,point);
			BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
		}
		break;
	case 19://拖屏
		if(nFlags)
		{
			int i;
			int xOffset=point.x-temPoint.x;
			int yOffset=point.y-temPoint.y;
			DisplayRect.OffsetRect(xOffset,yOffset);
			int Num=DataArray.size();
			for(i=0;i<Num;i++)
			{
				DataArray[i].StartPoint.Offset(xOffset,yOffset);
				DataArray[i].EndPoint.Offset(xOffset,yOffset);
				DataArray[i].CenterPoint.Offset(xOffset,yOffset);
			}
			Num=bkImageArray.size();
			for(i=0;i<Num;i++)
				bkImageArray[i].ptImage.Offset(xOffset,yOffset);

			CRect rect(0,0,WindowRect.Width(),WindowRect.Height());
			CRgn orgRgn,destRgn;
			orgRgn.CreateRectRgnIndirect(&rect);
			rect.OffsetRect(xOffset,yOffset);
			destRgn.CreateRectRgnIndirect(&rect);
			orgRgn.CombineRgn(&orgRgn,&destRgn,RGN_XOR);
			temDC->BitBlt(xOffset,yOffset,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
			temDC->FillRgn(&orgRgn,&CBrush(m_bkColor));
			memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
			SelectClipRgn(memDC->m_hDC,orgRgn.operator HRGN());
			memDC->FillSolidRect(&WindowRect,m_bkColor);
			SetBackGroundBmp();
			ReDraw();
			SelectClipRgn(memDC->m_hDC,NULL);
			BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);		
			temPoint=point;
		}
		MouseCur=AfxGetApp()->LoadCursor(IDC_HandCur);
		SetCursor(MouseCur);
		break;
	}

    //DisplayConsole("5",sizeof("5"));
		
end:    COleControl::OnMouseMove(nFlags, point);
}

void CSCTDrawActivexCtrl::AddLine(CPoint point) //画线
{
	pDataInfo.OperationKind=0;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillStyle=pbLineUnit->LineStyle;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
}

void CSCTDrawActivexCtrl::SelectOperations(CPoint point) //选择对象时用到此函数
{
	int Index;
	pbUnit=pbBaseUnit;;
	Matrix matrix;
	CRgn InitRgn;
	InitRgn.CreateRectRgn(0,0,0,0);
	HRGN hRgn=InitRgn.operator HRGN();
	Graphics graphics(hDC);
	SelObjList.clear();
	ClearDataInfo();
	int Size=DataArray.size();
	for(Index=Size-1;Index>=0;Index--)
	{
		if(DataArray[Index].IsCombination||DataArray[Index].IsDisjoint||DataArray[Index].IsErased)
			continue;
			//break;
		switch(DataArray[Index].OperationKind)
		{
		case 0:
			pbUnit=pbLineUnit;
			break;
		case 1:
			pbUnit=pbRectUnit;
			break;
		case 2:
			pbUnit=pbRoundRectUnit;
			break;
		case 3:
			pbUnit=pbEllipseUnit;
			break;
		case 4:
			pbUnit=pbArcUnit;
			break;
		case 5:
			pbUnit=pbSectorUnit;
			break;
		case 6:
			pbUnit=pbPenBrushUnit;
			break;
		case 7:
			pbUnit=pbPolylineUnit;
			break;
		case 8:
			pbUnit=pbTextUnit;
			break;
		case 9:
			pbUnit=pbBmpUnit;
			break;
		case 10:
			pbUnit=pbCombination;
			break;
		case 11:
			pbUnit=pbPolygonUnit;
			break;
		case 12:
			pbUnit=pbRgnObjUnit;
			break;
		}
		/////////////////////////////////////////////////
		hRgn=pbUnit->GetRedrawHRGN(hDC,DataArray,DataArray[Index]);
		Region Rgn(hRgn);	 
		matrix.Translate(DataArray[Index].CenterPoint.x,DataArray[Index].CenterPoint.y);
		matrix.Rotate(DataArray[Index].RotateAngle);
		matrix.Scale(DataArray[Index].xScale,DataArray[Index].yScale);
		Rgn.Transform(&matrix);
		matrix.Reset();
		if(Rgn.IsVisible(PointF(point.x,point.y),&graphics))
		{
			pDataInfo=DataArray[Index];
			SelObj AddSelObj;
			AddSelObj.Index=Index;
			AddSelObj.SelDataInfo=DataArray[Index];
			SelObjList.push_back(AddSelObj);
			break;
		}
		DeleteObject(hRgn);
	}	
	if(Index<0)
	{
		pbUnit=pbBaseUnit;
		MouseCur=NULL;
	}
	DeleteObject(hRgn);
}

void CSCTDrawActivexCtrl::StretchOperations(CPoint point) //拉伸对象时用到此函数
{
	SetCursor(MouseCur);
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	Matrix matrix;
	Graphics graphics(memDC->m_hDC);
	////////////////////////////////////////////////////
	int pWidth,pHeight,nWidth,nHeight;
	float xScale,yScale;
	CPoint tPoint;
	//////////////////////拉伸前/////////////////////////////////
	int GetIndex=SelObjList[0].Index;
	int mid;
	Rel_StartPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	Rel_EndPoint=CaculatePoint(point,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	Rel_EndPoint.x=Rel_EndPoint.x/fabs(GetDataInfo.xScale);
	Rel_EndPoint.y=Rel_EndPoint.y/fabs(GetDataInfo.yScale);
	tPoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	pWidth=tPoint.x-Rel_StartPoint.x;
	pHeight=tPoint.y-Rel_StartPoint.y;
	if(abs(Rel_EndPoint.x-Rel_StartPoint.x)<5)
	{
		Rel_EndPoint.x=Rel_StartPoint.x+5;
	}
	if(abs(Rel_EndPoint.y-Rel_StartPoint.y)<5)
	{
		Rel_EndPoint.y=Rel_StartPoint.y+5;
	}
	if(Rel_StartPoint.x>Rel_EndPoint.x)
	{
		mid=Rel_EndPoint.x;
		Rel_EndPoint.x=Rel_StartPoint.x;
		Rel_StartPoint.x=mid;
		GetDataInfo.xScale=-GetDataInfo.xScale;
		int Size=GetDataInfo.ComArray.size();//用于组合对象
		for(int Index=0;Index<Size;Index++)
			GetDataInfo.ComArray[Index].xScale=-GetDataInfo.ComArray[Index].xScale;
	}
	if(Rel_StartPoint.y>Rel_EndPoint.y)
	{
		mid=Rel_EndPoint.y;
		Rel_EndPoint.y=Rel_StartPoint.y;
		Rel_StartPoint.y=mid;
		GetDataInfo.yScale=-GetDataInfo.yScale;
		int Size=GetDataInfo.ComArray.size();//用于组合对象
		for(int Index=0;Index<Size;Index++)
			GetDataInfo.ComArray[Index].yScale=-GetDataInfo.ComArray[Index].yScale;
	}
	nWidth=Rel_EndPoint.x-Rel_StartPoint.x;
	nHeight=Rel_EndPoint.y-Rel_StartPoint.y;
	if(nWidth<=0||nHeight<=0)
		return;
	xScale=(float)nWidth/(float)pWidth;
	yScale=(float)nHeight/(float)pHeight;

	////////////////////////////////////////////////////
	double len=sqrt(float(Rel_StartPoint.x*Rel_StartPoint.x)+float(Rel_StartPoint.y*Rel_StartPoint.y));
	double Angle=atan2(float(Rel_StartPoint.y),float(Rel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(Rel_EndPoint.x*Rel_EndPoint.x)+float(Rel_EndPoint.y*Rel_EndPoint.y));
	Angle=atan2(float(Rel_EndPoint.y),float(Rel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	GetDataInfo.CenterPoint.x=(GetDataInfo.StartPoint.x+GetDataInfo.EndPoint.x)/2;
	GetDataInfo.CenterPoint.y=(GetDataInfo.StartPoint.y+GetDataInfo.EndPoint.y)/2;
	////////////////////////////////////////////////////////////////////////////
	Region Rgn(GetDataInfo.hRgn);
	matrix.Scale(xScale,yScale);
	Rgn.Transform(&matrix);
	matrix.Reset();
	GetDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	///////////////////////////////////////////////////////////////////////////////
	if(GetDataInfo.OperationKind==8)
	{
		CPoint tRel_StartPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
		CPoint tRel_EndPoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
		int Height=tRel_EndPoint.y-tRel_StartPoint.y;
		GetDataInfo.logfontw.lfHeight=Height;
		/////////////////////////////////////////////
		Font tfont(hDC,&GetDataInfo.logfontw);
		CString Str=(CString)GetDataInfo.SaveStr;
		wchar_t *PosStr;
		PosStr=new wchar_t[Str.GetLength()];
		PosStr=Str.AllocSysString();
		const StringFormat *format=StringFormat::GenericTypographic();
		RectF rectf;
		Graphics mGraphics(memDC->m_hDC);
		PointF pointf(tRel_StartPoint.x,tRel_StartPoint.y);
		mGraphics.MeasureString(PosStr,wcslen(PosStr),&tfont,pointf,format,&rectf);
		SizeF size;
		rectf.GetSize(&size);
		SysFreeString(PosStr);
		tRel_EndPoint.x=tRel_StartPoint.x+size.Width;
		tRel_EndPoint.y=tRel_StartPoint.y+size.Height;
		/////////////////////////////////////////////////
		double len=sqrt(float(tRel_StartPoint.x*tRel_StartPoint.x)+float(tRel_StartPoint.y*tRel_StartPoint.y));
		double Angle=atan2(float(tRel_StartPoint.y),float(tRel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
		GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
		len=sqrt(float(tRel_EndPoint.x*tRel_EndPoint.x)+float(tRel_EndPoint.y*tRel_EndPoint.y));
		Angle=atan2(float(tRel_EndPoint.y),float(tRel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
		GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
		GetDataInfo.CenterPoint.x=(GetDataInfo.StartPoint.x+GetDataInfo.EndPoint.x)/2;
		GetDataInfo.CenterPoint.y=(GetDataInfo.StartPoint.y+GetDataInfo.EndPoint.y)/2;
		///////////////////////////////////////////////
	}
	if(GetDataInfo.OperationKind==12)//区域对象
	{
		GetDataInfo.xScale*=xScale;
		GetDataInfo.yScale*=yScale;
	}
	DataArray[GetIndex]=GetDataInfo;
	if(GetDataInfo.OperationKind==10)
		pbCombination->ModifyCombinatedObjs(DataArray,GetDataInfo);
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	////////////////////////////////////////////
	UndoDataInfo=GetDataInfo;
	/////////////////////////////////////////////
}

void CSCTDrawActivexCtrl::MoveOperations(CPoint point)//对象移动时用到
{
	SetCursor(MouseCur);
	int xOffset=point.x-temPoint.x;
	int yOffset=point.y-temPoint.y;
	////////////////////////////////////////////////////	
	DataInfo GetDataInfo;
	int GetIndex;
	int Size=SelObjList.size();
	for(int Index=0;Index<Size;Index++)
	{
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetIndex=SelObjList[Index].Index;
		GetDataInfo.StartPoint.Offset(xOffset,yOffset);
		GetDataInfo.CenterPoint.Offset(xOffset,yOffset);
		GetDataInfo.EndPoint.Offset(xOffset,yOffset);
		SelObjList[Index].SelDataInfo=GetDataInfo;

		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.OperationKind==10)
			pbCombination->ModifyCombinatedObjs(DataArray,GetDataInfo);
	}	
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	////////////////////////////////////////////
	if(Size==1)
		UndoDataInfo=GetDataInfo;
	else
		UndoDataInfo.SelectColor=0;
	////////////////////////////////////////////
}

void CSCTDrawActivexCtrl::RotateOperations(CPoint point)//对象旋转时用到
{
	SetCursor(MouseCur);
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	CaculateAngle(point);
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	////////////////////////////////////////////
	UndoDataInfo=GetDataInfo;
	//////////////////////////////////////
}

void CSCTDrawActivexCtrl::CaculateAngle(CPoint point)//计算对象旋转时的具体角度
{
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	int GetIndex=SelObjList[0].Index;
	double Angle;
	double CosCenter;
	float  xRelativelen,yRelativelen;
	double temAngle;
	double ctbLength,ctlLength,btlLength;
	double SumSide;
	CPoint pt=GetDataInfo.CenterPoint;
	SumSide=(temPoint.x-pt.x)*(temPoint.x-pt.x)+(temPoint.y-pt.y)*(temPoint.y-pt.y);
	ctbLength=sqrt(SumSide);
	SumSide=(point.x-pt.x)*(point.x-pt.x)+(point.y-pt.y)*(point.y-pt.y);
	ctlLength=sqrt(SumSide);
	SumSide=(point.x-temPoint.x)*(point.x-temPoint.x)+(point.y-temPoint.y)*(point.y-temPoint.y);
	btlLength=sqrt(SumSide);
	if(ctbLength*ctlLength<0.005)
		Angle=180;
	else 
	{
		CosCenter=(ctbLength*ctbLength+ctlLength*ctlLength-btlLength*btlLength)/(2*ctbLength*ctlLength);
		Angle=acos(CosCenter);
		Angle=Angle*Rate;
		xRelativelen=point.x-pt.x;
		yRelativelen=point.y-pt.y;
		temAngle=atan2(yRelativelen,xRelativelen);
		if(GetDataInfo.preAngle>temAngle)
			Angle=0-Angle;
		GetDataInfo.RotateAngle=GetDataInfo.RotateAngle+Angle;
		GetDataInfo.preAngle=temAngle;
		GetDataInfo.RotateAngle=fmod(GetDataInfo.RotateAngle,360);
	}
	////////////////////////////////////////////////////
	double len=sqrt(float(Rel_StartPoint.x*Rel_StartPoint.x)+float(Rel_StartPoint.y*Rel_StartPoint.y));
	Angle=atan2(float(Rel_StartPoint.y),float(Rel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(Rel_EndPoint.x*Rel_EndPoint.x)+float(Rel_EndPoint.y*Rel_EndPoint.y));
	Angle=atan2(float(Rel_EndPoint.y),float(Rel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	////////////////////////////////////////////////////
	SelObjList[0].SelDataInfo=GetDataInfo;
	DataArray[GetIndex]=GetDataInfo;
	if(GetDataInfo.OperationKind==10)
		pbCombination->ModifyCombinatedObjs(DataArray,GetDataInfo);
}
void CSCTDrawActivexCtrl::FinishLine(CPoint point)//完成线的绘制
{
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.EndPoint=point;
	pDataInfo.FillStyle=pbLineUnit->LineStyle;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	
	//SelectColor++;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
}

void CSCTDrawActivexCtrl::AddRect(CPoint point)//绘制矩形
{
	pDataInfo.OperationKind=1;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.FillStyle=m_fillStyle;
	pDataInfo.foreColor=m_foreColor;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillColor=m_fillColor;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
}

void CSCTDrawActivexCtrl::FinishRect(CPoint point) //完成矩形的绘制
{
	CRect rect(pDataInfo.StartPoint,point);
	rect.NormalizeRect();
	pDataInfo.StartPoint=rect.TopLeft();
	pDataInfo.EndPoint=rect.BottomRight();
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	//SelectColor++;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
}

void CSCTDrawActivexCtrl::OnFillColorChanged() 
{
	// TODO: Add notification handler code
	SetModifiedFlag();
}

CPoint CSCTDrawActivexCtrl::CaculatePoint(CPoint point,CPoint CenterPoint,double RotateAngle)//计算对象编辑后的相关点的具体位置
{
	float xDot=float(point.x-CenterPoint.x);
	float yDot=float(point.y-CenterPoint.y);
	double len=sqrt(xDot*xDot+yDot*yDot);
	double m_atan=atan2(yDot,xDot);
	double m_Angle=m_atan-RotateAngle/Rate;
	point.y=len*sin(m_Angle);
	point.x=len*cos(m_Angle);
	return point;
}

void CSCTDrawActivexCtrl::AddRoundRect(CPoint point) //绘制圆角矩形
{
	pDataInfo.OperationKind=2;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.FillStyle=m_fillStyle;
	pDataInfo.foreColor=m_foreColor;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillColor=m_fillColor;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
}

void CSCTDrawActivexCtrl::FinishRoundRect(CPoint point) //完成圆角矩形的绘制
{
	CRect rect(pDataInfo.StartPoint,point);
	rect.NormalizeRect();
	pDataInfo.StartPoint=rect.TopLeft();
	pDataInfo.EndPoint=rect.BottomRight();
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	//SelectColor++;
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
}

void CSCTDrawActivexCtrl::AddEllipse(CPoint point) //绘制圆或椭圆
{
	pDataInfo.OperationKind=3;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.FillStyle=m_fillStyle;
	pDataInfo.foreColor=m_foreColor;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillColor=m_fillColor;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
}

void CSCTDrawActivexCtrl::FinishEllipse(CPoint point) //完成圆或者椭圆的绘制
{
	CRect rect(pDataInfo.StartPoint,point);
	rect.NormalizeRect();
	pDataInfo.StartPoint=rect.TopLeft();
	pDataInfo.EndPoint=rect.BottomRight();
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	//SelectColor++;
	memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
	pbEllipseUnit->ReDraw(memDC,pDataInfo);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
}


void CSCTDrawActivexCtrl::AddArc(CPoint point) //绘制弧形无用函数
{
	pDataInfo.OperationKind=4;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	SelectColor++;
}

void CSCTDrawActivexCtrl::FinishArc(CPoint point) //完成弧形的绘制
{

	if(ptSector[0]==ptSector[1]||ptSector[0]==ptSector[2]||ptSector[1]==ptSector[2])
	{
		ptSector.clear();
		return;
	}
		
	pDataInfo.OperationKind=4;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	//SelectColor++;

	int dx,dy;
	dx=pDataInfo.EndPoint.x-pDataInfo.CenterPoint.x;
	dy=pDataInfo.EndPoint.y-pDataInfo.CenterPoint.y;
	float Radius=sqrt(float(dx*dx)+float(dy*dy));
	GraphicsPath path;
	path.AddPie(pDataInfo.CenterPoint.x-Radius,pDataInfo.CenterPoint.y-Radius,2*Radius,2*Radius,pDataInfo.startAngle,pDataInfo.sweepAngle);
	RectF rectf;
	path.GetBounds(&rectf,NULL,NULL);

	pDataInfo.StartPoint=CPoint(rectf.X,rectf.Y);
	pDataInfo.EndPoint=CPoint(rectf.X+rectf.Width,rectf.Y+rectf.Height);
	pDataInfo.CenterPoint=CPoint(rectf.X+rectf.Width/2,rectf.Y+rectf.Height/2);
	CPoint ptOne=ptSector[0];
	CPoint ptTwo=ptSector[1];
	RelRate AddRate;
	AddRate.xRate=float(ptOne.x-rectf.X)/rectf.Width;
	AddRate.yRate=float(ptOne.y-rectf.Y)/rectf.Height;
	pDataInfo.AllRate.push_back(AddRate);
	AddRate.xRate=float(ptTwo.x-rectf.X)/rectf.Width;
	AddRate.yRate=float(ptTwo.y-rectf.Y)/rectf.Height;
	pDataInfo.AllRate.push_back(AddRate);
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	ptSector.clear();

	memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
	pbArcUnit->ReDraw(memDC,pDataInfo);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::AddSector(CPoint point) //绘制扇形无用函数
{
	pDataInfo.OperationKind=5;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.FillStyle=m_fillStyle;
	pDataInfo.foreColor=m_foreColor;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillColor=m_fillColor;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	//SelectColor++;
}

void CSCTDrawActivexCtrl::FinishSector(CPoint point) //完成扇形的绘制
{
	if(ptSector[0]==ptSector[1]||ptSector[0]==ptSector[2]||ptSector[1]==ptSector[2])
	{
		ptSector.clear();
		return;
	}
		
	pDataInfo.OperationKind=5;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.FillStyle=m_fillStyle;
	pDataInfo.foreColor=m_foreColor;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillColor=m_fillColor;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	//SelectColor++;

	int dx,dy;
	dx=pDataInfo.EndPoint.x-pDataInfo.CenterPoint.x;
	dy=pDataInfo.EndPoint.y-pDataInfo.CenterPoint.y;
	float Radius=sqrt(float(dx*dx)+float(dy*dy));
	GraphicsPath path;
	path.AddPie(pDataInfo.CenterPoint.x-Radius,pDataInfo.CenterPoint.y-Radius,2*Radius,2*Radius,pDataInfo.startAngle,pDataInfo.sweepAngle);
	RectF rectf;
	path.GetBounds(&rectf,NULL,NULL);

	pDataInfo.StartPoint=CPoint(rectf.X,rectf.Y);
	pDataInfo.EndPoint=CPoint(rectf.X+rectf.Width,rectf.Y+rectf.Height);
	pDataInfo.CenterPoint=CPoint(rectf.X+rectf.Width/2,rectf.Y+rectf.Height/2);
	CPoint ptOne=ptSector[0];
	CPoint ptTwo=ptSector[1];
	RelRate AddRate;
	AddRate.xRate=float(ptOne.x-rectf.X)/rectf.Width;
	AddRate.yRate=float(ptOne.y-rectf.Y)/rectf.Height;
	pDataInfo.AllRate.push_back(AddRate);
	AddRate.xRate=float(ptTwo.x-rectf.X)/rectf.Width;
	AddRate.yRate=float(ptTwo.y-rectf.Y)/rectf.Height;
	pDataInfo.AllRate.push_back(AddRate);

	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	ptSector.clear();

	memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
	pbSectorUnit->ReDraw(memDC,pDataInfo);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::AddPolyline(CPoint point) //彩虹笔，铅笔，钢笔，荧光笔，纹理笔的绘制
{
	temSavePoint addPoint;//暂存整个绘制过程中的所有点，已方便具体绘点时用到
	addPoint.point=point;
	ptsPoint.push_back(addPoint);

	pDataInfo.OperationKind=7;
	pDataInfo.Alpha=pbPolylineUnit->Alpha;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=point;
	//pDataInfo.StartPoint = CPoint(0,0);
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.LineStyle=pbPolylineUnit->LineStyle;
	if(pbPolylineUnit->LineStyle==RainbowStyle)
	{
		pDataInfo.penColor=pbPolylineUnit->cols[0];
		pDataInfo.FillColor=pbPolylineUnit->cols[1];
		pDataInfo.FontColor=pbPolylineUnit->cols[2];
		pDataInfo.foreColor=pbPolylineUnit->cols[3];
	}
	if(pbPolylineUnit->LineStyle==TextureStyle)
	{
		pDataInfo.ImageSize=pbPolylineUnit->ImageSize;
		IStream* pIStream= NULL;
		CreateStreamOnHGlobal(pbPolylineUnit->ImageByte,TRUE,&pIStream);

		HGLOBAL hGlobal=::GlobalAlloc(GMEM_FIXED,pbPolylineUnit->ImageSize);
		pDataInfo.ImageByte=(BYTE*)::GlobalLock(hGlobal);
		pIStream->Read(pDataInfo.ImageByte,pDataInfo.ImageSize,NULL);
		GlobalUnlock(hGlobal);		
	}
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
	PointNum=0;
	pbPolylineUnit->StartPoint=point;
	//pbPolylineUnit->StartPoint = CPoint(0,0);
	pbPolylineUnit->penColor=m_penColor;
	pbPolylineUnit->penWidth=m_penWidth;
	pbPolylineUnit->SelectColor=DataArray.size();
		
}

void CSCTDrawActivexCtrl::FinishPolyline(CPoint point) //完成彩虹笔，铅笔，钢笔，荧光笔，纹理笔的绘制
{
	//SelectColor++;
	CPoint sPoint;
	float xlen,ylen;
	sPoint=pDataInfo.StartPoint;
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	xlen=pDataInfo.EndPoint.x-sPoint.x;
	ylen=pDataInfo.EndPoint.y-sPoint.y;
	if(xlen==0)
		xlen=1;
	if(ylen==0)
		ylen=1;	
	int Size=ptsPoint.size();
	PointNum=0;
	for(int Index=0;Index<Size;Index++)
	{
		RelRate addRate;
		addRate.xRate=(ptsPoint[Index].point.x-sPoint.x)/xlen;
		addRate.yRate=(ptsPoint[Index].point.y-sPoint.y)/ylen;
		pDataInfo.AllRate.push_back(addRate);
		PointNum++;
	}
	ptsPoint.clear();
	pDataInfo.PointNum=PointNum;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	if(pDataInfo.StartPoint!=pDataInfo.EndPoint)
	{
		pDataInfo.SelectColor = DataArray.size();
		DataArray.push_back(pDataInfo);
		UndoArray.push_back(pDataInfo);
			
		RedoArray.clear();
	}
	pDataInfo.AllRate.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	pbPolylineUnit->pURgn->MakeEmpty();
}

void CSCTDrawActivexCtrl::AddPenBrush(CPoint point)//毛笔的绘制
{
	temSavePoint addPoint;
	addPoint.point=point;
	addPoint.preWidth=m_penWidth;
	addPoint.alpha=200;
	ptsPoint.push_back(addPoint);

	pDataInfo.OperationKind=6;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
	PointNum=0;
	pbPenBrushUnit->prelen=0;
	pbPenBrushUnit->StartPoint=point;
	pbPenBrushUnit->penColor=m_penColor;
	pbPenBrushUnit->penWidth=m_penWidth;
	pbPenBrushUnit->SelectColor=DataArray.size();
}

void CSCTDrawActivexCtrl::FinishPenBrush(CPoint point)//完成毛笔的绘制
{
	CPoint sPoint;
	float xlen,ylen;
	sPoint=pDataInfo.StartPoint;
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	xlen=pDataInfo.EndPoint.x-sPoint.x;
	ylen=pDataInfo.EndPoint.y-sPoint.y;
	if(ptsPoint.size()==0)
	{
		//SelectColor--;	
		goto ret;
	}
	if(xlen<1)
		xlen=1;
	if(ylen<1)
		ylen=1;
	PointNum=0;
	int Size=ptsPoint.size();
	for(int Index=0;Index<Size;Index++)
	{
		RelRate addRate;
		addRate.xRate=(ptsPoint[Index].point.x-sPoint.x)/xlen;
		addRate.yRate=(ptsPoint[Index].point.y-sPoint.y)/ylen;
		addRate.alpha=ptsPoint[Index].alpha;
		addRate.preWidth=ptsPoint[Index].preWidth;
		pDataInfo.AllRate.push_back(addRate);
		PointNum++;
	}
	pDataInfo.PointNum=PointNum;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	if(pDataInfo.StartPoint!=pDataInfo.EndPoint)
	{
		pDataInfo.SelectColor = DataArray.size();
		DataArray.push_back(pDataInfo);
		UndoArray.push_back(pDataInfo);
			
		RedoArray.clear();
		SelObjList.clear();
		SelObj AddSelObj;
		AddSelObj.Index=DataArray.size()-1;
		AddSelObj.SelDataInfo=pDataInfo;
		SelObjList.push_back(AddSelObj);
	}
	//SelectColor++;
ret:  ptsPoint.clear();
		
	pDataInfo.AllRate.clear();

}


void CSCTDrawActivexCtrl::AddText(CPoint point)//文字输入添加
{
	Graphics graphics(hDC);
	CFont cfont;
	cfont.CreateFontIndirect(&DefaultFont);
	HFONT hfont=cfont.operator HFONT(); 
	Font font(memDC->m_hDC,hfont);
	CHARFORMAT cf;
	ChangeLogFontToCharFormat(cf,DefaultFont,m_fontColor);
	pEdit->SetSelectionCharFormat(cf);
	///////////////////////////////////////////////
	pDataInfo.OperationKind=8;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	pDataInfo.RotateAngle=0;
	pDataInfo.preAngle=0;
	pDataInfo.FontColor=m_fontColor;
	pDataInfo.CenterPoint.x=0;
	pDataInfo.CenterPoint.y=0;
	Graphics mGraphics(hDC);
	/////////////////////////////////////////////////////
	font.GetLogFontW(&mGraphics,&pDataInfo.logfontw);
	pbUnit=pbTextUnit;
	pbUnit->Rel_CenterPoint.x=0;
	pbUnit->Rel_CenterPoint.y=0;
	pbUnit->Rel_StartPoint=point;
}

void CSCTDrawActivexCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	COleControl::OnTimer(nIDEvent);
}

void CSCTDrawActivexCtrl::OnFontColorChanged() 
{
	// TODO: Add notification handler code
	SetModifiedFlag();
}


BOOL CSCTDrawActivexCtrl::FinishText(CPoint point)//完成文字输入
{ 
	CString Text;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	GetDlgItemText(IDC_Text,Text);
	pDataInfo.charNum=Text.GetLength();
	if(pDataInfo.charNum==0)
		return false;
	pDataInfo.SaveStr=new char[Text.GetLength()+1];
	strcpy(pDataInfo.SaveStr,Text);
	pDataInfo.SaveStr[Text.GetLength()]='\0';
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	pbUnit->ReDraw(memDC,pDataInfo);
	pbUnit=NULL;
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//SelectColor++;
	return true;
}

void CSCTDrawActivexCtrl::RotateText()//?貌似在整个控件中都没有用到过哈
{
	double Angle=pDataInfo.RotateAngle;
	CPoint cPoint=pDataInfo.CenterPoint; 
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.TranslateTransform(cPoint.x,cPoint.y);
	mGraphics.RotateTransform(Angle);
	Pen pen(Color(255,0,0,0),2);
	pen.SetDashStyle(DashStyleDot);
	mGraphics.DrawRectangle(&pen,Relative_sPoint.x,Relative_sPoint.y,Relative_ePoint.x-Relative_sPoint.x,Relative_ePoint.y-Relative_sPoint.y);
	mGraphics.ResetTransform();
}

void CSCTDrawActivexCtrl::AddBitmap(CPoint point)//绘制图片
{
	pDataInfo.OperationKind=9;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=point;
	pDataInfo.EndPoint=point;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	//pDataInfo.SelectColor=SelectColor;
	pDataInfo.ChartInfo.CharNum=0;
	///////////////////////////////////////////s
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
	CFile file(BmpName,CFile::modeRead|CFile::typeBinary);
	pDataInfo.ImageSize=file.GetLength(); 
	///////////////////////////////////////////////////////
	HGLOBAL hGlobal=::GlobalAlloc(GMEM_FIXED,pDataInfo.ImageSize);
	pDataInfo.ImageByte=(BYTE*)::GlobalLock(hGlobal);
	file.Read(pDataInfo.ImageByte,pDataInfo.ImageSize);  
	GlobalUnlock(hGlobal);	
	file.Close();
	////////////////////////////////////////////////////////  

}

void CSCTDrawActivexCtrl::FinishBitmap(CPoint point)//完成图片的绘制
{
	if(pDataInfo.StartPoint.x>point.x)
	{
		pDataInfo.EndPoint.x=pDataInfo.StartPoint.x;
		pDataInfo.StartPoint.x=point.x;
	}
	else
		pDataInfo.EndPoint.x=point.x;
	if(pDataInfo.StartPoint.y>point.y)
	{
		pDataInfo.EndPoint.y=pDataInfo.StartPoint.y;
		pDataInfo.StartPoint.y=point.y;
	}
	else
		pDataInfo.EndPoint.y=point.y;
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	BmpName="";
	BmpFromHandle=false;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	//SelectColor++;
}

void CSCTDrawActivexCtrl::ReDraw(BOOL Erased)//控件对象重绘是用到
{
	int i;
	int MaxIndex=DataArray.size()-1;
	CLineUnit	*ptLineUnit=new CLineUnit;
	CRectUnit *ptRectUnit=new CRectUnit;
	CRoundRectUnit *ptRoundRectUnit=new CRoundRectUnit;
	CEllipseUnit *ptEllipseUnit=new CEllipseUnit;
	CArcUnit *ptArcUnit=new CArcUnit;
	CSectorUnit *ptSectorUnit=new CSectorUnit;
	CPolylineUnit *ptPolylineUnit=new CPolylineUnit;
	CPenBrushUnit *ptPenBrushUnit=new CPenBrushUnit;
	CTextUnit *ptTextUnit=new CTextUnit;
	CBitmapUnit *ptBmpUnit=new CBitmapUnit;  
	CCombination *ptCombination=new CCombination;
	CPolygonUnit *ptPolygonUnit=new CPolygonUnit;
	CRgnObjUnit *ptRgnObjUnit=new CRgnObjUnit;
	for(i=0;i<=MaxIndex;i++)
	{
		TransferDataInfo=DataArray[i];
		switch (TransferDataInfo.OperationKind)
		{
		case 0:
			DoReDraw(ptLineUnit,Erased);
			break;
		case 1:
			DoReDraw(ptRectUnit,Erased);
			break;
		case 2:
			DoReDraw(ptRoundRectUnit,Erased);
			break;
		case 3:
			DoReDraw(ptEllipseUnit,Erased);
			break;
		case 4:
			DoReDraw(ptArcUnit,Erased);
			break;
		case 5:
			DoReDraw(ptSectorUnit,Erased);
			break;
		case 6:
			DoReDraw(ptPenBrushUnit,Erased);
			break;
		case 7:
			DoReDraw(ptPolylineUnit,Erased);
			break;
		case 8:
			DoReDraw(ptTextUnit,Erased);
			break;
		case 9:
			DoReDraw(ptBmpUnit,Erased);
			break;
		case 10:
			if(!TransferDataInfo.IsCombination&&!TransferDataInfo.IsDisjoint&&!TransferDataInfo.IsErased)
			{
				std::map<int,int> ObjList;
				ptCombination->GetCombinatedObjs(DataArray,TransferDataInfo,ObjList);
				ptCombination->ReDraw(memDC,DataArray,ObjList);
				ObjList.clear();
			}
			break;
		case 11:
			DoReDraw(ptPolygonUnit,Erased);
			break;
		case 12:
			DoReDraw(ptRgnObjUnit,Erased);
			break;
		}
	}
	temDC->BitBlt(0,0,WindowRect.Width(),WindowRect.Height(),memDC,0,0,SRCCOPY);
	delete ptLineUnit;
	delete ptRectUnit;
	delete ptRoundRectUnit;
	delete ptEllipseUnit;
	delete ptArcUnit;
	delete ptSectorUnit;
	delete ptPolylineUnit;
	delete ptPenBrushUnit;
	delete ptTextUnit;
	delete ptBmpUnit;  
	delete ptCombination;
	delete ptPolygonUnit;
}

void CSCTDrawActivexCtrl::DoReDraw(CBaseUnit *ptemUnit,BOOL Erased)
{
	if(!TransferDataInfo.IsErased&&!TransferDataInfo.IsCombination)
		ptemUnit->ReDraw(memDC,TransferDataInfo);
}

void CSCTDrawActivexCtrl::OnFilePathChanged() 
{
	// TODO: Add notification handler code
	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::EditBmp()//?貌似在整个控件中都没有用到此函数
{
	CPoint sPoint,ePoint;
	int tem;
	sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo.CenterPoint,pDataInfo.RotateAngle);
	ePoint=CaculatePoint(pDataInfo.EndPoint,pDataInfo.CenterPoint,pDataInfo.RotateAngle);
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.TranslateTransform(pDataInfo.CenterPoint.x,pDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(pDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen pen(Color(255,0,0,0),2);
	pen.SetDashStyle(DashStyleDot);
	if(sPoint.x>ePoint.x)
	{
		tem=sPoint.x;
		sPoint.x=ePoint.x;
		ePoint.x=tem;
	}
	if(sPoint.y>ePoint.y)
	{
		tem=sPoint.y;
		sPoint.y=ePoint.y;
		ePoint.y=tem;
	}
	mGraphics.DrawRectangle(&pen,sPoint.x,sPoint.y,ePoint.x-sPoint.x,ePoint.y-sPoint.y);
	mGraphics.ResetTransform();
	Relative_cPoint.x=(int)((sPoint.x+ePoint.x)/2);
	Relative_cPoint.y=(int)((sPoint.y+ePoint.y)/2);
	Relative_sPoint=sPoint;
	Relative_ePoint=ePoint;
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	memDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,temDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::OnTemBmpPathChanged() 
{
	// TODO: Add notification handler code
	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//TODO: Add your message handler code here and/or call default
	if(MultiInfoArray.size()>0)
		return;
	if(m_operations==18)
	{
		Start=true;
		FinishPolygon(point);
		memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
		SetBackGroundBmp();
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	}
	else if(m_operations==11)
	{
		if(SelObjList.size()==1&&SelObjList[0].SelDataInfo.ChartInfo.ChartDataArray.size()>0)
			JudgeSelectedImage(true);
		else
			JudgeSelectedImage(false);
	}
	else
		OnLButtonDown(nFlags,point);
	COleControl::OnLButtonDblClk(nFlags, point);
}

void CSCTDrawActivexCtrl::AddCombination()//对象组合时用到
{
	int Size=SelObjList.size();
	DataInfo AddDataInfo;
	DataInfo GetDataInfo;
	for(int Index=0;Index<Size;Index++)
	{
		ComInfo AddComInfo;
		DataArray[SelObjList[Index].Index].IsCombination=true;
		GetDataInfo=DataArray[SelObjList[Index].Index];
		AddComInfo.ObjIndex=SelObjList[Index].Index;
		AddComInfo.xCenterRate=(float)(GetDataInfo.CenterPoint.x-ComObjRect.TopLeft().x)/ComObjRect.Width();
		AddComInfo.yCenterRate=(float)(GetDataInfo.CenterPoint.y-ComObjRect.TopLeft().y)/ComObjRect.Height();
		AddComInfo.xStartRate=(float)(GetDataInfo.StartPoint.x-ComObjRect.TopLeft().x)/ComObjRect.Width();
		AddComInfo.yStartRate=(float)(GetDataInfo.StartPoint.y-ComObjRect.TopLeft().y)/ComObjRect.Height();
		AddComInfo.xEndRate=(float)(GetDataInfo.EndPoint.x-ComObjRect.TopLeft().x)/ComObjRect.Width();
		AddComInfo.yEndRate=(float)(GetDataInfo.EndPoint.y-ComObjRect.TopLeft().y)/ComObjRect.Height();
		AddComInfo.OffsetAngle=GetDataInfo.RotateAngle;
		AddComInfo.xScale=GetDataInfo.xScale;
		AddComInfo.yScale=GetDataInfo.yScale;
		AddDataInfo.ComArray.push_back(AddComInfo);
	}
	ZeroMemory(AddDataInfo.Reserved0,200);
	ZeroMemory(AddDataInfo.Reserved1,200);
	ZeroMemory(AddDataInfo.Reserved2,200);
	ZeroMemory(AddDataInfo.Reserved3,200);
	ZeroMemory(AddDataInfo.Reserved4,1024);
	AddDataInfo.OperationKind=10;
	AddDataInfo.StartPoint=ComObjRect.TopLeft();
	AddDataInfo.EndPoint=ComObjRect.BottomRight();
	AddDataInfo.CenterPoint=ComObjRect.CenterPoint();
	AddDataInfo.preAngle=0;
	AddDataInfo.RotateAngle=0;
	AddDataInfo.penWidth=m_penWidth;
	AddDataInfo.IsCombination=false;
	AddDataInfo.IsLocked=false;
	AddDataInfo.IsDisjoint=false;
	pDataInfo.IsLocked=false;
	AddDataInfo.IsErased=false;
	AddDataInfo.xScale=1;
	AddDataInfo.yScale=1;
	//AddDataInfo.SelectColor=SelectColor++;
	AddDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(AddDataInfo);
	UndoArray.push_back(AddDataInfo);
		
	RedoArray.clear();
	ComObjRect.SetRect(0,0,0,0);	
}

void CSCTDrawActivexCtrl::ClearDataInfo()//每个具体操作的DataInfo信息是不同的，所以当操作改变时，我们要先清理之前保留的DataInfo信息
{
	//int i=0;
	ptSector.clear();
	pDataInfo.ChartInfo.bkColor=RGB(255,255,0);
	pDataInfo.ChartInfo.ChartDataArray.clear();
	pDataInfo.ComArray.clear();
	pDataInfo.OperationKind=-1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.StartPoint.x=-1;
	pDataInfo.StartPoint.y=-1;
	pDataInfo.EndPoint.x=-1;
	pDataInfo.EndPoint.y=-1;
	pDataInfo.penWidth=0;
	pDataInfo.preAngle=0;//弧度
	pDataInfo.RotateAngle=90;//角度
	pDataInfo.startAngle=0;//弧的起始角度
	pDataInfo.sweepAngle=0;//弧的跨度
	pDataInfo.CenterPoint.x=0;//旋转时的坐标原点
	pDataInfo.CenterPoint.y=0;
	pDataInfo.SaveStr=NULL;
	pDataInfo.charNum=0;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	ptsPoint.clear();
	pDataInfo.AllRate.clear();	
	///////////////////////////
	RotateRgn.X=0;
	RotateRgn.Y=0;
	RotateRgn.Width=0;
	RotateRgn.Height=0;
	StretchRgn.X=0;
	StretchRgn.Y=0;
	StretchRgn.Width=0;
	StretchRgn.Height=0;
	MenuRgn.X=0;
	MenuRgn.Y=0;
	MenuRgn.Width=0;
	MenuRgn.Height=0;
	MoveRgn.X=0;
	MoveRgn.Y=0;
	MoveRgn.Width=0;
	MoveRgn.Height=0;
	PlayRgn.X=0;
	PlayRgn.Y=0;
	PlayRgn.Width=0;
	PlayRgn.Height=0;
	///////////////////////////
}

void CSCTDrawActivexCtrl::ObjectErase(CPoint point)//对象擦除时用到
{	
	SelectOperations(point);
	int Size=SelObjList.size();
	if(Size!=1)
		return;
	DataInfo AddDataInfo;
	int GetIndex=SelObjList[0].Index;
	ComInfo AddComInfo;
	AddComInfo.ObjIndex=GetIndex;
	AddComInfo.xScale=-1;
	DataArray[GetIndex].IsErased=true;
	Size=DataArray[GetIndex].ComArray.size();
	for(int Index=0;Index<Size;Index++)
		DataArray[DataArray[GetIndex].ComArray[Index].ObjIndex].IsErased=true;
	AddDataInfo.ComArray.push_back(AddComInfo);
	AddDataInfo.SelectColor=-3;
	UndoArray.push_back(AddDataInfo);
	SelObjList.clear();
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::OnBkColorChanged() //背景改变后可以触发此函数
{
	// TODO: Add notification handler code
	if(IsInitial)
	{
		bkImageArray.clear();
		memDC->FillSolidRect(&WindowRect,m_bkColor);
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
		m_bkBmp="";
		PageChanged();//事件触发
	}
	InitErasureImage();
	SetModifiedFlag();
}


void CSCTDrawActivexCtrl::OnLineStyleChanged() //当线条类型更改后触发此函数
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::OnFillStyleChanged() //当填充类型更改后触发此函数
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::OnForeColorChanged() //前景色更变会触发
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::OnBkBmpChanged() //控件背景图片更改后触发此函数
{
	// TODO: Add notification handler code
	if(IsInitial)
	{
		int Index;
		bkImageInfo addbkImage;
		float Width,Height;
		int Size=bkImageArray.size();
		for(Index=0;Index<Size;Index++)
			if(abs(bkImageArray[Index].ptImage.x-WindowRect.right/2)<10&&abs(bkImageArray[Index].ptImage.y-WindowRect.bottom/2)<10)
				break;    
		CFile file(m_bkBmp,CFile::modeRead|CFile::typeBinary);
		addbkImage.ImageSize=file.GetLength();
		///////////////////////////////////////////////////////
		HGLOBAL hGlobal=::GlobalAlloc(GHND,addbkImage.ImageSize);
		addbkImage.ImageByte=(BYTE*)::GlobalLock(hGlobal);
		file.Read(addbkImage.ImageByte,addbkImage.ImageSize);  
		GlobalUnlock(hGlobal);	
		////////////////////////////////////////////////////////
		file.Close();
		addbkImage.ptImage.x=WindowRect.right/2;//说明背景图片的左上角的点为控件窗口的中心点
		addbkImage.ptImage.y=WindowRect.bottom/2;
		//addbkImage.ptImage.x=0.0;
		//addbkImage.ptImage.y=0.0;
		addbkImage.xScale=1.0f;
		addbkImage.yScale=1.0f;
		/////////////////////////////////////////////////////
		int len=m_bkBmp.GetLength();
		wchar_t *WideStr;
		WideStr=new wchar_t[len];
		WideStr=m_bkBmp.AllocSysString();
		Image mImage(WideStr);
		Width=mImage.GetWidth();
		Height=mImage.GetHeight();
		float mul=Height/Width;
		if(Width>WindowRect.Width())
		{
			Width=WindowRect.Width();
			Height=Width*mul;
		}
		addbkImage.Width=Width;
		addbkImage.Height=Height;
		SysFreeString(WideStr);
		//////////////////////////////////////////
		if(Index<Size)
			bkImageArray[Index]=addbkImage;
		else
			bkImageArray.push_back(addbkImage);
		/////////////////////////////////////
		SetBackGroundBmp();
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		InitErasureImage();
	}
	SetModifiedFlag();
}

void CSCTDrawActivexCtrl::SetBackGroundBmp()//在每次擦除时会调用到此函数
{
	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetInterpolationMode(InterpolationModeHighQuality);//使用高质量的插补算法
	int Size=bkImageArray.size();
	float Width,Height;
	for(int Index=0;Index<Size;Index++)
	{
		Width=bkImageArray[Index].Width;
		Height=bkImageArray[Index].Height;
		mGraphics.TranslateTransform(bkImageArray[Index].ptImage.x,bkImageArray[Index].ptImage.y);  
		mGraphics.ScaleTransform(bkImageArray[Index].xScale,bkImageArray[Index].yScale);
		////////////////////////////////////////////////////////////////////////////////////////////
		IStream *pIStream;
		CreateStreamOnHGlobal(bkImageArray[Index].ImageByte,true,&pIStream);
		Image image(pIStream);
		mGraphics.DrawImage(&image,-Width/2,-Height/2,Width,Height);//-Width/2,-Height/2是相对于背景图片左上角的偏移量
		//mGraphics.DrawImage(&image,0.0,0.0,Width,Height);
		////////////////////////////////////////////////////////////////////////////////////////////////	
		mGraphics.ResetTransform();
	}
	PageChanged();//事件触发
}

void CSCTDrawActivexCtrl::ClearData()//清理页信息
{
	int Size=bkImageArray.size();
	ptSector.clear();
	DataArray.clear();
	bkImageArray.clear();
	ptsPoint.clear();
	UndoArray.clear();
	RedoArray.clear();
	SelObjList.clear();
	CopyArray.clear();
	///////////////////////////
	RotateRgn.X=0;
	RotateRgn.Y=0;
	RotateRgn.Width=0;
	RotateRgn.Height=0;
	StretchRgn.X=0;
	StretchRgn.Y=0;
	StretchRgn.Width=0;
	StretchRgn.Height=0;
	MenuRgn.X=0;
	MenuRgn.Y=0;
	MenuRgn.Width=0;
	MenuRgn.Height=0;
	MoveRgn.X=0;
	MoveRgn.Y=0;
	MoveRgn.Width=0;
	MoveRgn.Height=0;
	PlayRgn.X=0;
	PlayRgn.Y=0;
	PlayRgn.Width=0;
	PlayRgn.Height=0;
	PointNum=0;
	///////////////////////////
	m_fillStyle=0;
	m_fillColor=RGB(255,255,255);
	m_foreColor=RGB(255,0,0);
	m_bkColor=RGB(255,255,255); 
	m_operations=0;
	m_penWidth=5;
	m_penColor=RGB(0,0,0);
	m_fontColor=RGB(0,0,0);
	m_lineStyle=0;
}

BSTR CSCTDrawActivexCtrl::SetOperationEX(LPCTSTR Operations) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult=Operations;
	if(strcmp(Operations,"View Mode")==0)
	{
		m_operations=0;
		goto ret;
	}
	if(strcmp(Operations,"Add Line")==0)
	{
		m_operations=1;
		goto ret;
	}
	if(strcmp(Operations,"Add Rectangle")==0)
	{
		m_operations=2;
		goto ret;
	}
	if(strcmp(Operations,"Add Rounded Rect")==0)
	{
		m_operations=3;
		goto ret;
	}
	if(strcmp(Operations,"Add Ellipse")==0)
	{
		m_operations=4;
		goto ret;
	}
	if(strcmp(Operations,"Add Arc")==0)
	{
		m_operations=5;
		goto ret;
	}
	if(strcmp(Operations,"Add Sector")==0)
	{
		m_operations=6;
		goto ret;
	}
	if(strcmp(Operations,"Add PenBrush")==0)
	{
		m_operations=7;
		goto ret;
	}
	if(strcmp(Operations,"Add Polyline")==0)
	{
		m_operations=8;
		goto ret;
	}
	if(strcmp(Operations,"Add Text")==0)
	{
		m_operations=9;
		goto ret;
	}
	if(strcmp(Operations,"Add Image")==0)
	{
		m_operations=10;
		goto ret;
	}
	if(strcmp(Operations,"Edit/Select Mode")==0)
	{
		m_operations=11;
		goto ret;
	}
	if(strcmp(Operations,"SaveDataToFile")==0)
	{
		m_operations=12;
		goto ret;
	}
	if(strcmp(Operations,"ReadDataFromFile")==0)
	{
		m_operations=13;
		goto ret;
	}
	if(strcmp(Operations,"RgnEraser")==0)
	{
		m_operations=14;
		goto ret;
	}
	if(strcmp(Operations,"DotEraser")==0)
	{
		m_operations=15;
		goto ret;
	}
	if(strcmp(Operations,"ObjectEraser")==0)
	{
		m_operations=16;
		goto ret;
	}
	if(strcmp(Operations,"SprayBottle")==0)
	{
		m_operations=17;
		goto ret;
	}
	if(strcmp(Operations,"Add Polygon")==0)
	{
		m_operations=18;
		goto ret;
	}
	if(strcmp(Operations,"DragScreen")==0)
		m_operations=19;

ret: return strResult.AllocSysString();
}


BSTR CSCTDrawActivexCtrl::SetLineStyleEX(LPCTSTR LineStyle) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult=LineStyle;
	if(strcmp(LineStyle,"Solid")==0)
	{
		m_lineStyle=0;
		goto ret;
	}
	if(strcmp(LineStyle,"Dash")==0)
	{
		m_lineStyle=1;
		goto ret;
	}
	if(strcmp(LineStyle,"Dot")==0)
	{
		m_lineStyle=2;
		goto ret;
	}
	if(strcmp(LineStyle,"DashDot")==0)
	{
		m_lineStyle=3;
		goto ret;
	}
	if(strcmp(LineStyle,"DashDotDot")==0)
		m_lineStyle=4;

ret: return strResult.AllocSysString();
}

BSTR CSCTDrawActivexCtrl::SetFillStyleEX(LPCTSTR FillStyle) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult=FillStyle;
	if(strcmp(FillStyle,"None")==0)
	{
		m_fillStyle=0;
		goto ret;
	}
	if(strcmp(FillStyle,"Solid")==0)
	{
		m_fillStyle=1;
		goto ret;
	}
	if(strcmp(FillStyle,"Horizontal Line")==0)
	{
		m_fillStyle=2;
		goto ret;
	}
	if(strcmp(FillStyle,"Vertical Line")==0)
	{
		m_fillStyle=3;
		goto ret;
	}
	if(strcmp(FillStyle,"ForwardDiagonal")==0)
	{
		m_fillStyle=4;
		goto ret;
	}
	if(strcmp(FillStyle,"BackwardDiagonal")==0)
	{
		m_fillStyle=5;
		goto ret;
	}
	if(strcmp(FillStyle,"Cross Line")==0)
	{
		m_fillStyle=6;
		goto ret;
	}
	if(strcmp(FillStyle,"DiagonalCross")==0)
	{
		m_fillStyle=7;
		goto ret;
	}
	if(strcmp(FillStyle,"LightVertical")==0)
	{
		m_fillStyle=8;
		goto ret;
	}
	if(strcmp(FillStyle,"LightHorizonta")==0)
	{
		m_fillStyle=9;
		goto ret;
	}
	if(strcmp(FillStyle,"DashedHorizontal")==0)
	{
		m_fillStyle=10;
		goto ret;
	}
	if(strcmp(FillStyle,"Wave")==0)
	{
		m_fillStyle=11;
		goto ret;
	}
	if(strcmp(FillStyle,"DiagonalBrick")==0)
	{
		m_fillStyle=12;
		goto ret;
	}
	if(strcmp(FillStyle,"HorizontalBrick")==0)
	{
		m_fillStyle=13;
		goto ret;
	}
	if(strcmp(FillStyle,"Weave")==0)
	{
		m_fillStyle=14;
		goto ret;
	}
	if(strcmp(FillStyle,"SolidDiamond")==0)
		m_fillStyle=15;	
ret: return strResult.AllocSysString();
}

void CSCTDrawActivexCtrl::CreateNewPage() //新建页面时用到
{
	// TODO: Add your dispatch handler code here
	SavePage();
	SelectColor=0;
	lbdown=false;
	preOperations=0;
	pbUnit=NULL;
	ModifyText=false;
	IsInitial=true;
	memset(bkBmp,'\0',100);
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	ClearData();	
	CreatePage();
}

BOOL CSCTDrawActivexCtrl::ModifyLineWidth(short nWidth) //对象在选择后，对其线条的粗细进行更改时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.penWidth=nWidth;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyLineWidth(GetDataInfo,nWidth);
	}
	/////////////////////////////////////////	
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyLineWidth(DataInfo tDataInfo, short nWidth)//对象在选择后，对其线条的粗细进行更改时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].penWidth=nWidth;
		if(DataArray[GetIndex].ComArray.size())
			InModifyLineWidth(DataArray[GetIndex],nWidth);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::ModifyLineColor(OLE_COLOR nColor) //对象在选择后，对其线条的颜色进行设置时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.penColor=nColor;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyLineColor(GetDataInfo,nColor);
	}
	/////////////////////////////////////////	
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyLineColor(DataInfo tDataInfo, OLE_COLOR nColor)//对象在选择后，对其线条的颜色进行设置时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].penColor=nColor;
		if(DataArray[GetIndex].ComArray.size())
			InModifyLineColor(DataArray[GetIndex],nColor);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::ModifyFontColor(OLE_COLOR nColor) //对象在选择后，对其字体的颜色进行设置时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.FontColor=nColor;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyFontColor(GetDataInfo,nColor);
	}
	/////////////////////////////////////////	
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyFontColor(DataInfo tDataInfo, OLE_COLOR nColor)//对象在选择后，对其字体的颜色进行设置时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].FontColor=nColor;
		if(DataArray[GetIndex].ComArray.size())
			InModifyFontColor(DataArray[GetIndex],nColor);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::ModifyLineStyle(short nStyle) //对象在选择后，对其线条类型进行设置时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.LineStyle=nStyle;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyLineStyle(GetDataInfo,nStyle);
	}
	/////////////////////////////////////////		
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyLineStyle(DataInfo tDataInfo, int Style)//对象在选择后，对其线条类型进行设置时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].LineStyle=Style;
		if(DataArray[GetIndex].ComArray.size())
			InModifyLineStyle(DataArray[GetIndex],Style);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::ModifyFillStyle(short nStyle) //对象在选择后，对填充类型进行设置时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.FillStyle=nStyle;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyFillStyle(GetDataInfo,nStyle);
	}
	/////////////////////////////////////////		
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyFillStyle(DataInfo tDataInfo, short nStyle)//对象在选择后，对填充类型进行设置时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].FillStyle=nStyle;
		if(DataArray[GetIndex].ComArray.size())
			InModifyFillStyle(DataArray[GetIndex],nStyle);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::AddPicture(OLE_HANDLE hBitmap, short Left, short Top, short Width, short Height) //导入flash和视频文件时用到
{
	// TODO: Add your dispatch handler code here
	ClearDataInfo();
	////////////////////////////////
	pDataInfo.ChartInfo.CharNum=0;
	pDataInfo.OperationKind=9;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);

	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(memDC->m_hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	Image *pBmp=new Bitmap((HBITMAP)hBitmap,NULL);
	///////////////////////////////////////////////////////////////
	STATSTG statstg;
	HGLOBAL hGlobal;
	IStream* pIStream= NULL;
	CreateStreamOnHGlobal(NULL,TRUE,&pIStream);
	pBmp->Save(pIStream,&BmpClsid);
	pIStream->Stat(&statstg,STATFLAG_NONAME);
	pDataInfo.ImageSize=statstg.cbSize.QuadPart;
	GetHGlobalFromStream(pIStream,&hGlobal);
	pDataInfo.ImageByte=(BYTE*)GlobalLock(hGlobal);
	GlobalUnlock(hGlobal);  

	//////////////////////////////////////////////////////////////
	//SelectColor++;
	/////////////////////////////////////////////////
	graphics.DrawImage(pBmp,Left,Top,Width,Height);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);	
	delete pBmp;
	//////////////////////////////////////////////////
	pDataInfo.ChartInfo.CharNum=0;
	pDataInfo.StartPoint.x=Left;
	pDataInfo.StartPoint.y=Top;
	pDataInfo.EndPoint.x=Left+Width;
	pDataInfo.EndPoint.y=Top+Height;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::AddPolygon(CPoint point)//铅笔，钢笔，荧光笔，纹理笔
{
	temSavePoint addPoint;
	addPoint.point=point;
	addPoint.preWidth=m_penWidth;
	addPoint.alpha=200;
	ptsPoint.push_back(addPoint);

	pDataInfo.OperationKind=11;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=point;
	pDataInfo.penColor=m_penColor;
	pDataInfo.penWidth=m_penWidth;
	pDataInfo.FillStyle=m_fillStyle;
	pDataInfo.FillColor=m_fillColor;
	//pDataInfo.SelectColor=SelectColor;
	pDataInfo.foreColor=m_foreColor;
	pDataInfo.LineStyle=m_lineStyle;
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		
	PointNum=0;
	pbPolygonUnit->StartPoint=point;
	pbPolygonUnit->penColor=m_penColor;
	pbPolygonUnit->penWidth=m_penWidth;
	return true;
}

void CSCTDrawActivexCtrl::FinishPolygon(CPoint point)//完成，铅笔，钢笔，荧光笔，纹理笔
{
	CPoint sPoint;
	float xlen,ylen;
	sPoint=pDataInfo.StartPoint;
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	xlen=pDataInfo.EndPoint.x-sPoint.x;
	ylen=pDataInfo.EndPoint.y-sPoint.y;
	if(xlen<1)
		xlen=1;
	if(ylen<1)
		ylen=1;
	PointNum=0;
	int Size=ptsPoint.size();
	if(Size<3)
		return;
	//SelectColor++;
	CPoint tPoint=ptsPoint[0].point;
	CRect rect(tPoint.x,tPoint.y,tPoint.x,tPoint.y);
	rect.InflateRect(10,10);
	if(rect.PtInRect(point))
		ptsPoint[Size-1]=ptsPoint[0];
	for(int Index=0;Index<Size;Index++)
	{
		RelRate addRate;
		addRate.xRate=(ptsPoint[Index].point.x-sPoint.x)/xlen;
		addRate.yRate=(ptsPoint[Index].point.y-sPoint.y)/ylen;
		pDataInfo.AllRate.push_back(addRate);
		PointNum++;
	}
	if(ptsPoint[0].point==ptsPoint[Size-1].point)
		pDataInfo.AllRate[Size-1]=pDataInfo.AllRate[0];
	pDataInfo.PointNum=PointNum;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
}

BOOL CSCTDrawActivexCtrl::AddPictureFromFile(LPCTSTR FilePath, short Left, short Top, short Width, short Height)//从文件中添加图片 在图片导入时用到
{
	// TODO: Add your dispatch handler code here
	BmpName=FilePath;
	if(Left==0&&Top==0&&Width==0&&Height==0)
		return false;
	pDataInfo.ChartInfo.CharNum=0;
	pDataInfo.OperationKind=9;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=CPoint(Left,Top);
	pDataInfo.EndPoint=CPoint(Left+Width,Top+Height);
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	//pDataInfo.SelectColor=SelectColor;
	///////////////////////////////////////////
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(memDC->m_hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	//SelectColor++;

	CFile file;
	if(file.Open(BmpName,CFile::modeRead|CFile::typeBinary)==false)
		MessageBox("指定文件不存在");
	pDataInfo.ImageSize=file.GetLength(); 
	///////////////////////////////////////////////////////
	HGLOBAL hGlobal=::GlobalAlloc(GMEM_FIXED,pDataInfo.ImageSize);
	pDataInfo.ImageByte=(BYTE*)::GlobalLock(hGlobal);
	file.Read(pDataInfo.ImageByte,pDataInfo.ImageSize);  
	GlobalUnlock(hGlobal);	
	file.Close();
	////////////////////////////////////////////////////////  
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);

	int len=BmpName.GetLength();
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=BmpName.AllocSysString();
	Image image(WideStr);
	graphics.DrawImage(&image,Left,Top,Width,Height);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);	
	SysFreeString(WideStr);	
	BmpName="";
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::AddPictureFromHandle(OLE_HANDLE hBitmap) //在资源库中,如果程序没找到预览图,会用软件内置的一张背影图片与资源文件的图标合成一个预览图,这类资源在单击时就会调用AddPictureFromHandle
{
	// TODO: Add your dispatch handler code here
	temDC->BitBlt(0,0,WindowRect.right,WindowRect.bottom,memDC,0,0,SRCCOPY);
	ClearDataInfo();
	////////////////////////////////
	pDataInfo.ChartInfo.CharNum=0;
	pDataInfo.OperationKind=9;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	//pDataInfo.SelectColor=SelectColor;
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	///////////////////////////////////////////////////////////////////////////
	Image *pBmp=new Bitmap((HBITMAP)hBitmap,NULL);
	///////////////////////////////////////////////////////////////
	STATSTG statstg;
	HGLOBAL hGlobal;
	IStream* pIStream= NULL;
	CreateStreamOnHGlobal(NULL,TRUE,&pIStream);
	pBmp->Save(pIStream,&BmpClsid);
	pIStream->Stat(&statstg,STATFLAG_NONAME);
	pDataInfo.ImageSize=statstg.cbSize.QuadPart;
	GetHGlobalFromStream(pIStream,&hGlobal);
	pDataInfo.ImageByte=(BYTE*)GlobalLock(hGlobal);
	GlobalUnlock(hGlobal);  
	///////////////////////////////////////////////////////////////////////////
	//SelectColor++;
	BmpFromHandle=true;
	return TRUE;
}

short CSCTDrawActivexCtrl::ShowPage(short PageID) //显示页面信息
{
	// TODO: Add your dispatch handler code here
	int Size=PageArray.size();	
	if(PageID<PageArray.size())
	{
		if(PageID==PageNum)
			return PageID;
		/////////////////保存原显示页信息///////////////////
		SavePage();	
		/////////读取将要显示页信息/////////////////////
		ClearData();
		PageInfo GetPage=PageArray[PageID];
		m_bkColor=GetPage.bkColor; 
		m_fillStyle=GetPage.FillStyle;
		m_fillColor=GetPage.FillColor;
		m_foreColor=GetPage.ForeColor;	
		m_operations=GetPage.Operations;
		m_penWidth=GetPage.PenWidth;
		m_penColor=GetPage.PenColor;
		m_fontColor=GetPage.FontColor;
		m_bkColor=GetPage.bkColor;
		m_lineStyle=GetPage.LineStyle;
		int Size=GetPage.DataArray.size();
		//SelectColor=Size;
		for(int i=0;i<Size;i++)
			DataArray.push_back(GetPage.DataArray[i]);
		Size=GetPage.bkImageArray.size();
		for(int k=0;k<Size;k++)
			bkImageArray.push_back(GetPage.bkImageArray[k]);
		Graphics graphics(memDC->m_hDC);
		Bitmap bmp(GetPage.hMemBmp,NULL);
		graphics.DrawImage(&bmp,0,0,WindowRect.Width(),WindowRect.Height());
		BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	}
	else
	{
		AfxMessageBox("您所找的页面不存在!");
		return -1;
	}
	PageNum=PageID;
	return PageID;
}

OLE_HANDLE CSCTDrawActivexCtrl::GetPageThumbnail(SHORT PageIndex)//得到具体页面的缩略图信息
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if(PageIndex<0||PageIndex>=PageArray.size())
		return NULL;
	HBITMAP hBmp;
	if(PageIndex!=PageNum)
		hBmp=(HBITMAP)CopyImage(PageArray[PageIndex].hMemBmp,IMAGE_BITMAP,WindowRect.Width(),WindowRect.Height(),LR_COPYRETURNORG);
	else
	{
		hBmp=(HBITMAP)CopyImage(memBmp.operator HBITMAP(),IMAGE_BITMAP,WindowRect.Width(),WindowRect.Height(),LR_COPYRETURNORG);
	}
	return (unsigned int)hBmp;

}


OLE_HANDLE CSCTDrawActivexCtrl::GetPageBmp() //得到当前页的缩略图时要用到
{
	// TODO: Add your dispatch handler code here
	HBITMAP hBmp=(HBITMAP)CopyImage(memBmp.operator HBITMAP(),IMAGE_BITMAP,WindowRect.Width(),WindowRect.Height(),LR_COPYRETURNORG);
	return (unsigned int)hBmp;
}

BOOL CSCTDrawActivexCtrl::CreatePage()//新建页面
{
	PageInfo AddPage;
	AddPage.bkColor=m_bkColor;
	int Size=DataArray.size();
	for(int i=0;i<Size;i++)
		AddPage.DataArray.push_back(DataArray[i]);
	Size=bkImageArray.size();
	for(int k=0;k<Size;k++)
		AddPage.bkImageArray.push_back(bkImageArray[k]);
	////////////////////////////////////////
	DisplayRect=WindowRect;
	AddPage.FillStyle=m_fillStyle;
	AddPage.FillColor=m_fillColor;
	AddPage.ForeColor=m_foreColor;
	AddPage.bkColor=m_bkColor; 
	AddPage.Operations=m_operations;
	AddPage.PenWidth=m_penWidth;
	AddPage.PenColor=m_penColor;
	AddPage.FontColor=m_fontColor;
	AddPage.bkColor=m_bkColor;
	AddPage.LineStyle=m_lineStyle;
	PageArray.push_back(AddPage);
	PageNum=PageArray.size()-1;
	return true;
}

BOOL CSCTDrawActivexCtrl::SavePage()//保存页面信息
{
	if(PageNum<0)
		return false;
	if(IsWindow(pEdit->GetSafeHwnd()))
	{
		FinishText(CPoint(0,0));
		pEdit->DestroyWindow();
	}
	PageInfo CurrentPage;
	PageArray[PageNum].DataArray.clear();
	CurrentPage.bkColor=m_bkColor;
	int Size=DataArray.size();
	for(int i=0;i<Size;i++)
		CurrentPage.DataArray.push_back(DataArray[i]);
	Size=bkImageArray.size();
	for(int k=0;k<Size;k++)
		CurrentPage.bkImageArray.push_back(bkImageArray[k]);
	CurrentPage.DisplayRect=DisplayRect;
	CurrentPage.FillStyle=m_fillStyle;
	CurrentPage.FillColor=m_fillColor;
	CurrentPage.ForeColor=m_foreColor;
	CurrentPage.Operations=m_operations;
	CurrentPage.PenWidth=m_penWidth;
	CurrentPage.PenColor=m_penColor;
	CurrentPage.FontColor=m_fontColor;
	CurrentPage.bkColor=m_bkColor;
	CurrentPage.LineStyle=m_lineStyle;
	CurrentPage.hMemBmp=(HBITMAP)CopyImage(memBmp.operator HBITMAP(),IMAGE_BITMAP,WindowRect.Width(),WindowRect.Height(),LR_COPYRETURNORG);
	DeleteObject(PageArray[PageNum].hMemBmp);
	PageArray[PageNum]=CurrentPage;
	/*if(CopyPageArray.size()>0)
		CopyPageArray[0]=CurrentPage;*/
	return true;
}


BOOL CSCTDrawActivexCtrl::SetbkImageFromHandle(OLE_HANDLE hBitmap) //设置背景图片
{
	// TODO: Add your dispatch handler code here
	float Width,Height;
	Image *pBmp=new Bitmap((HBITMAP)hBitmap,NULL);	
	Width=pBmp->GetWidth();
	Height=pBmp->GetHeight();
	float mul=Height/Width;
	if(Width>WindowRect.Width())
	{
		Width=WindowRect.Width();
		Height=Width*mul;
	}
	/////////////////////////////////
	int Index;
	bkImageInfo addbkImage;
	addbkImage.Width=Width;
	addbkImage.Height=Height;
	int Size=bkImageArray.size();
	for(Index=0;Index<Size;Index++)
		if(abs(bkImageArray[Index].ptImage.x-WindowRect.right/2)<10&&abs(bkImageArray[Index].ptImage.y-WindowRect.bottom/2)<10)
			break;   
	///////////////////////////////////////////////////////////////
	STATSTG statstg;
	HGLOBAL hGlobal;
	IStream* pIStream= NULL;
	CreateStreamOnHGlobal(NULL,TRUE,&pIStream);
	pBmp->Save(pIStream,&BmpClsid);
	pIStream->Stat(&statstg,STATFLAG_NONAME);
	addbkImage.ImageSize=statstg.cbSize.QuadPart;
	GetHGlobalFromStream(pIStream,&hGlobal);
	addbkImage.ImageByte=(BYTE*)GlobalLock(hGlobal);
	GlobalUnlock(hGlobal);  
	delete pBmp;
	///////////////////////////////////////////////////////////////
	addbkImage.ptImage.x=WindowRect.right/2;
	addbkImage.ptImage.y=WindowRect.bottom/2;
	addbkImage.xScale=1.0f;
	addbkImage.yScale=1.0f;
	if(Index<Size)
		bkImageArray[Index]=addbkImage;
	else
		bkImageArray.push_back(addbkImage);
	/////////////////////////////////////
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::SetCombination() //对象组合时用
{
	// TODO: Add your dispatch handler code here
	if(SelObjList.size()>1)
	{

		BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);	
		AddCombination();	
		///////////////////////////
		MouseCur=NULL;
		SelObjList.clear();
		RotateRgn.X=0;
		RotateRgn.Y=0;
		RotateRgn.Width=0;
		RotateRgn.Height=0;
		StretchRgn.X=0;
		StretchRgn.Y=0;
		StretchRgn.Width=0;
		StretchRgn.Height=0;
		MenuRgn.X=0;
		MenuRgn.Y=0;
		MenuRgn.Width=0;
		MenuRgn.Height=0;
		MoveRgn.X=0;
		MoveRgn.Y=0;
		MoveRgn.Width=0;
		MoveRgn.Height=0;
		PlayRgn.X=0;
		PlayRgn.Y=0;
		PlayRgn.Width=0;
		PlayRgn.Height=0;
		///////////////////////////
	}
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::SetDisjunction() //对象解组
{
	// TODO: Add your dispatch handler code here
	if(SelObjList.size()==1)
	{
		int Index,Size,Num;
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		if(GetDataInfo.ComArray.size()>1)
		{
			GetDataInfo.IsDisjoint=true;
			Size=GetDataInfo.ComArray.size();
			for(Index=0;Index<Size;Index++)
			{
				Num=GetDataInfo.ComArray[Index].ObjIndex;
				DataArray[Num].IsCombination=false;
			}
			/////////////////////////////////////////////
			MouseCur=NULL;
			RotateRgn.X=0;
			RotateRgn.Y=0;
			RotateRgn.Width=0;
			RotateRgn.Height=0;
			StretchRgn.X=0;
			StretchRgn.Y=0;
			StretchRgn.Width=0;
			StretchRgn.Height=0;
			MenuRgn.X=0;
			MenuRgn.Y=0;
			MenuRgn.Width=0;
			MenuRgn.Height=0;
			MoveRgn.X=0;
			MoveRgn.Y=0;
			MoveRgn.Width=0;
			MoveRgn.Height=0;
			PlayRgn.X=0;
			PlayRgn.Y=0;
			PlayRgn.Width=0;
			PlayRgn.Height=0;
			/////////////////////////////////////////////
			DataArray[SelObjList[0].Index]=GetDataInfo;		
			SelObjList.clear();
		}
	}
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::ObjSelected()//对象选择后（单个或者是组合后的对象）要用到这个函数,主要是用来绘制选择好对象后的外框和相应的菜单图标
{
	ChangeRect.clear();
	CPoint sPoint,ePoint;
	int mid,pwidth;
	SelObjList[0].SelDataInfo=DataArray[SelObjList[0].Index];
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	pwidth=GetDataInfo.penWidth;
	if(pwidth<=0)
		pwidth=1;
	sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	sPoint.x*=GetDataInfo.xScale;
	sPoint.y*=GetDataInfo.yScale;
	ePoint.x*=GetDataInfo.xScale;
	ePoint.y*=GetDataInfo.yScale;
	if(sPoint.x>ePoint.x)
	{
		mid=sPoint.x;
		sPoint.x=ePoint.x;
		ePoint.x=mid;
	}
	if(sPoint.y>ePoint.y)
	{
		mid=sPoint.y;
		sPoint.y=ePoint.y;
		ePoint.y=mid;
	}
	MenuRgn.X=(float)(ePoint.x+pwidth+6);
	MenuRgn.Y=(float)(sPoint.y-pwidth-6);
	MenuRgn.Width=0.0f;
	MenuRgn.Height=0.0f;//菜单下拉点
	MenuRgn.Inflate(12,12);
	if(!GetDataInfo.IsLocked)
	{
		RotateRgn.X=(float)(sPoint.x+ePoint.x)/2;
		RotateRgn.Y=(float)(sPoint.y-pwidth-17);
		RotateRgn.Width=0.0f;
		RotateRgn.Height=0.0f;//旋转点
		RotateRgn.Inflate(12,12);
		StretchRgn.X=(float)(ePoint.x+pwidth+6);
		StretchRgn.Y=(float)(ePoint.y+pwidth+6);
		StretchRgn.Width=0.0f;
		StretchRgn.Height=0.0f;//拉伸点
		StretchRgn.Inflate(12,12);
		MoveRgn.X=(float)(sPoint.x-pwidth-6);
		MoveRgn.Y=(float)(sPoint.y-pwidth-6);
		MoveRgn.Width=(float)(ePoint.x-sPoint.x+2*pwidth+12);
		MoveRgn.Height=(float)(ePoint.y-sPoint.y+2*pwidth+12);
	}
	CString str=GetDataInfo.Reserved0;
	if(str.GetLength()!=0||GetDataInfo.OperationKind==8)
	{
		PlayRgn.X=(float)(sPoint.x-pwidth-6);
		PlayRgn.Y=(float)(ePoint.y-pwidth+6);
		PlayRgn.Width=0;
		PlayRgn.Height=0;
		PlayRgn.Inflate(12,12);
	}

	Graphics mGraphics(hDC);
	mGraphics.TranslateTransform(GetDataInfo.CenterPoint.x,GetDataInfo.CenterPoint.y);
	mGraphics.RotateTransform(GetDataInfo.RotateAngle);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	Pen pen(Color(0,0,0),1.5);
	pen.SetDashStyle(DashStyleDot);
	mGraphics.DrawRectangle(&pen,MoveRgn);
	mGraphics.DrawImage(MenuImage,MenuRgn);
	if(!GetDataInfo.IsLocked)
	{
		mGraphics.DrawImage(StretchImage,StretchRgn);
		mGraphics.DrawImage(RotateImage,RotateRgn);
	}

	if(str.GetLength()!=0||GetDataInfo.OperationKind==8)
		mGraphics.DrawImage(PlayImage,PlayRgn);

	Rel_StartPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	Rel_EndPoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	Rel_CenterPoint.x=(int)((Rel_StartPoint.x+Rel_EndPoint.x)/2);
	Rel_CenterPoint.y=(int)((Rel_StartPoint.y+Rel_EndPoint.y)/2);
	///////////////////////////////////////////////////////////////////////////
	if(GetDataInfo.OperationKind==11)
	{
		mGraphics.ScaleTransform(GetDataInfo.xScale/fabs(GetDataInfo.xScale),GetDataInfo.yScale/fabs(GetDataInfo.xScale));
		int Size=GetDataInfo.PointNum;
		std::vector<PointF> pts;
		int Width=Rel_EndPoint.x-Rel_StartPoint.x;
		int Height=Rel_EndPoint.y-Rel_StartPoint.y;
		for(int Index=Size-1;Index>=0;Index--)
		{
			PointF pt;
			pt.X=sPoint.x+Width*GetDataInfo.AllRate[Index].xRate*fabs(GetDataInfo.xScale);
			pt.Y=sPoint.y+Height*GetDataInfo.AllRate[Index].yRate*fabs(GetDataInfo.yScale);
			RectF rectf(pt.X,pt.Y,0,0);
			rectf.Inflate(8,8);
			mGraphics.DrawImage(ChangeImage,rectf);
			ChangeRect.push_back(rectf);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	if(UndoDataInfo.SelectColor!=-1)
	{
		UndoArray.push_back(UndoDataInfo);
		UndoDataInfo.SelectColor=-1;
	}	
	mGraphics.ResetTransform();
	return true;
}

BOOL CSCTDrawActivexCtrl::ObjsSelected()//对象将要组合时会用到这个函数
{
	int Size=SelObjList.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		SelObjList[Index].SelDataInfo=DataArray[GetIndex];
	}
	CPoint sPoint=ComRect.TopLeft();
	CPoint ePoint=ComRect.BottomRight();
	sPoint.Offset(-25,-25);
	ePoint.Offset(25,25);
	MoveRgn.X=(float)(sPoint.x);
	MoveRgn.Y=(float)(sPoint.y);
	MoveRgn.Width=(float)(ePoint.x-sPoint.x);
	MoveRgn.Height=(float)(ePoint.y-sPoint.y);
	Rect rect1(sPoint.x-5,sPoint.y-5,10,10);
	Rect rect2(sPoint.x-5,ePoint.y-5,10,10);
	Rect rect3(ePoint.x-5,ePoint.y-5,10,10);
	Rect rect4(ePoint.x-5,sPoint.y-5,10,10);
	Graphics mGraphics(hDC);
	mGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
	SolidBrush mBrush(Color(255,0,252,0));
	Pen pen1(Color(0,0,0),1.5);
	pen1.SetDashStyle(DashStyleDot);
	mGraphics.DrawRectangle(&pen1,MoveRgn);
	Pen pen(&mBrush,1);
	mGraphics.DrawEllipse(&pen,rect1);
	mGraphics.DrawEllipse(&pen,rect2);
	mGraphics.DrawEllipse(&pen,rect3);
	mGraphics.DrawEllipse(&pen,rect4);
	mGraphics.FillEllipse(&mBrush,rect1);
	mGraphics.FillEllipse(&mBrush,rect2);
	mGraphics.FillEllipse(&mBrush,rect3);
	mGraphics.FillEllipse(&mBrush,rect4);
	/////////////////////////////////////
	if(UndoDataInfo.SelectColor!=-1)
	{
		UndoDataInfo.SelectColor=-2;
		UndoArray.push_back(UndoDataInfo);
		UndoDataInfo.SelectColor=-1;
	}
	else
	{
		UndoDataInfo.ComArray.clear();
		int Size=SelObjList.size();
		int GetIndex;
		DataInfo GetDataInfo;
		for(int Index=0;Index<Size;Index++)
		{
			ComInfo AddComInfo;
			GetDataInfo=SelObjList[Index].SelDataInfo;
			GetIndex=SelObjList[Index].Index;
			AddComInfo.ObjIndex=GetIndex;
			AddComInfo.xStartRate=GetDataInfo.StartPoint.x;
			AddComInfo.yStartRate=GetDataInfo.StartPoint.y;
			AddComInfo.xEndRate=GetDataInfo.EndPoint.x;
			AddComInfo.yEndRate=GetDataInfo.EndPoint.y;
			AddComInfo.xCenterRate=GetDataInfo.CenterPoint.x;
			AddComInfo.yCenterRate=GetDataInfo.CenterPoint.y;
			UndoDataInfo.ComArray.push_back(AddComInfo);
		}
	}
	//////////////////////////////////////
	return true;
}

HCURSOR CSCTDrawActivexCtrl::GetMouseCursor(CPoint point)//得到具体鼠标的ico类型
{
	int Size=SelObjList.size();
	DataInfo GetDataInfo;
	Matrix matrix;
	int GetIndex;
	CRgn InitRgn;
	InitRgn.CreateRectRgn(0,0,0,0);
	HRGN hRgn=InitRgn.operator HRGN();
	Graphics graphics(hDC);
	MouseCur=NULL;
	if(Size==1)
	{
		matrix.Scale(SelObjList[0].SelDataInfo.xScale/fabs(SelObjList[0].SelDataInfo.xScale),SelObjList[0].SelDataInfo.yScale/fabs(SelObjList[0].SelDataInfo.yScale));
		CPoint tPoint=CaculatePoint(point,SelObjList[0].SelDataInfo.CenterPoint,SelObjList[0].SelDataInfo.RotateAngle);
		PointF pointf(tPoint.x,tPoint.y); 
		for(int Index=0;Index<ChangeRect.size();Index++)
		{
			Region Rgn(ChangeRect[Index]);
			Rgn.Transform(&matrix);
			if(Rgn.IsVisible(pointf))
			{
				MouseCur=AfxGetApp()->LoadCursor(IDC_Change);
				ChangeIndex=ChangeRect.size()-Index-1;
				goto ret;
			}
		}
		matrix.Reset();

		if(StretchRgn.Contains(pointf))
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_Edit);
			goto ret;
		}
		if(RotateRgn.Contains(pointf))
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_Rotate);
			goto ret;
		}
		if(MenuRgn.Contains(pointf))
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_Menu);
			goto ret;
		}
		if(PlayRgn.Contains(pointf))
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_PlayCur);
			goto ret;
		}
		if(MoveRgn.Contains(pointf))
		{
			MouseCur=AfxGetApp()->LoadCursor(IDC_Move);
			goto ret;
		}
	}
	if(Size>1)
	{
		for(int Index=0;Index<Size;Index++)
		{
			GetDataInfo=SelObjList[Index].SelDataInfo;
			GetIndex=SelObjList[Index].Index;
			switch(DataArray[GetIndex].OperationKind)
			{
			case 0:
				pbUnit=pbLineUnit;
				break;
			case 1:
				pbUnit=pbRectUnit;
				break;
			case 2:
				pbUnit=pbRoundRectUnit;
				break;
			case 3:
				pbUnit=pbEllipseUnit;
				break;
			case 4:
				pbUnit=pbArcUnit;
				break;
			case 5:
				pbUnit=pbSectorUnit;
				break;
			case 6:
				pbUnit=pbPenBrushUnit;
				break;
			case 7:
				pbUnit=pbPolylineUnit;
				break;
			case 8:
				pbUnit=pbTextUnit;
				break;
			case 9:
				pbUnit=pbBmpUnit;
				break;
			case 10:
				pbUnit=pbCombination;
				break;
			case 11:
				pbUnit=pbPolygonUnit;
				break;
			}
			/////////////////////////////////////////////////
			hRgn=pbUnit->GetRedrawHRGN(hDC,DataArray,DataArray[GetIndex]);
			Region Rgn(hRgn);	 
			matrix.Translate(DataArray[GetIndex].CenterPoint.x,DataArray[GetIndex].CenterPoint.y);
			matrix.Rotate(DataArray[GetIndex].RotateAngle);
			matrix.Scale(DataArray[GetIndex].xScale,DataArray[Index].yScale);
			Rgn.Transform(&matrix);
			matrix.Reset();
			if(Rgn.IsVisible(PointF(point.x,point.y),&graphics))
			{
				pDataInfo=DataArray[Index];
				MouseCur=AfxGetApp()->LoadCursor(IDC_Move);
				break;
			}
		}
	}
ret: pbUnit=pbBaseUnit;
	DeleteObject(hRgn);
	return MouseCur;
}


BOOL CSCTDrawActivexCtrl::Undo() //撤销
{
	// TODO: Add your dispatch handler code here
	int Index,SelColor,Size,GetIndex,RedoSize,inSize,inIndex;
	int xRate =0;
	int yRate =0;
	float xScale = 1;
	float yScale = 1;
	double RotateAngle = 0;
	Matrix matrix;
	Graphics graphics(hDC);
	Size=UndoArray.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo=UndoArray[Size-1];
	SelColor=GetDataInfo.SelectColor;
	RedoArray.push_back(GetDataInfo);	
	UndoArray.pop_back();
	if(SelColor==-2)//移动
	{
		Size=GetDataInfo.ComArray.size();
		RedoSize=RedoArray.size()-1;
		DataInfo RedoDataInfo;
		for(Index=0;Index<Size;Index++)
		{
			ComInfo AddComInfo;
			GetIndex=GetDataInfo.ComArray[Index].ObjIndex;
			AddComInfo.ObjIndex=GetIndex;
			AddComInfo.xStartRate=DataArray[GetIndex].StartPoint.x;
			AddComInfo.yStartRate=DataArray[GetIndex].StartPoint.y;
			AddComInfo.xEndRate=DataArray[GetIndex].EndPoint.x;
			AddComInfo.yEndRate=DataArray[GetIndex].EndPoint.y;
			AddComInfo.xCenterRate=DataArray[GetIndex].CenterPoint.x;
			AddComInfo.yCenterRate=DataArray[GetIndex].CenterPoint.y;
			RedoDataInfo.ComArray.push_back(AddComInfo);
			//////////////////////////////////////////////////////////////////////
			DataArray[GetIndex].StartPoint.x=(int)GetDataInfo.ComArray[Index].xStartRate;
			DataArray[GetIndex].StartPoint.y=(int)GetDataInfo.ComArray[Index].yStartRate;
			DataArray[GetIndex].EndPoint.x=(int)GetDataInfo.ComArray[Index].xEndRate;
			DataArray[GetIndex].EndPoint.y=(int)GetDataInfo.ComArray[Index].yEndRate;
			DataArray[GetIndex].CenterPoint.x=(int)GetDataInfo.ComArray[Index].xCenterRate;
			DataArray[GetIndex].CenterPoint.y=(int)GetDataInfo.ComArray[Index].yCenterRate;

		}
		RedoDataInfo.SelectColor=-2;
		RedoArray[RedoSize]=RedoDataInfo;
	}
	if(SelColor==-3)//区域擦除
	{
		Size=GetDataInfo.ComArray.size();
		RedoSize=RedoArray.size()-1;
		DataInfo RedoDataInfo;
		for(Index=0;Index<Size;Index++)
		{
			ComInfo AddComInfo;
			GetIndex=GetDataInfo.ComArray[Index].ObjIndex;
			AddComInfo.ObjIndex=GetIndex;
			RedoDataInfo.ComArray.push_back(AddComInfo);
			DataArray[GetIndex].IsErased=false;
			inSize=DataArray[GetIndex].ComArray.size();
			for(inIndex=0;inIndex<inSize;inIndex++)
				DataArray[DataArray[GetIndex].ComArray[inIndex].ObjIndex].IsErased=false;
		}
		RedoDataInfo.SelectColor=-3;
		RedoArray[RedoSize]=RedoDataInfo;
	}
	if(SelColor==-4)//点擦除
	{
		Size=GetDataInfo.ComArray.size();
		for(Index=0;Index<Size;Index++)
		{
			Region Rgn(GetDataInfo.hRgn);
			GetIndex=GetDataInfo.ComArray[Index].ObjIndex;
			Region gRgn(DataArray[GetIndex].hRgn);
			matrix.Rotate(-DataArray[GetIndex].RotateAngle);	 
			matrix.Translate(-DataArray[GetIndex].CenterPoint.x,-DataArray[GetIndex].CenterPoint.y);	
			Rgn.Transform(&matrix);
			matrix.Reset();
			gRgn.Exclude(&Rgn);
			DataArray[GetIndex].hRgn=gRgn.GetHRGN(&graphics);
		}
	}
	if(SelColor>=0)
	{
			
		Size=UndoArray.size();
		for(Index=Size-1;Index>=0;Index--)
			if(SelColor==UndoArray[Index].SelectColor)
			{
				xRate = UndoArray[Index].StartPoint.x - DataArray[SelColor].StartPoint.x;
				yRate = UndoArray[Index].StartPoint.y - DataArray[SelColor].StartPoint.y;
				//xScale = UndoArray[Index].xScale/DataArray[SelColor].xScale;
				//yScale = UndoArray[Index].yScale/DataArray[SelColor].yScale;
				//RotateAngle = UndoArray[Index].RotateAngle - UndoArray[Index-1].RotateAngle;
				DataArray[SelColor]=UndoArray[Index];
				break;
			}
			if(Index<0)
			{
				DataArray[SelColor].IsErased=true;
			}
			if(GetDataInfo.OperationKind==10)
			{
				Size=GetDataInfo.ComArray.size();
				for(Index=0;Index<Size;Index++)
				{
					int Size = UndoArray.size();
					DataArray[GetDataInfo.ComArray[Index].ObjIndex].IsCombination=false;
					//
					/*DataArray[GetDataInfo.ComArray[Index].ObjIndex].StartPoint.x += xRate;
					DataArray[GetDataInfo.ComArray[Index].ObjIndex].EndPoint.x += xRate;
					DataArray[GetDataInfo.ComArray[Index].ObjIndex].StartPoint.y += yRate;
					DataArray[GetDataInfo.ComArray[Index].ObjIndex].EndPoint.y += yRate;
					DataArray[GetDataInfo.ComArray[Index].ObjIndex].CenterPoint.x += xRate;
					DataArray[GetDataInfo.ComArray[Index].ObjIndex].CenterPoint.y += yRate;*/
					//DataArray[GetDataInfo.ComArray[Index].ObjIndex].xScale =1;
					//DataArray[GetDataInfo.ComArray[Index].ObjIndex].yScale =1;
					//DataArray[GetDataInfo.ComArray[Index].ObjIndex].RotateAngle -= RotateAngle;
					if(Size>0)
						pbCombination->ModifyCombinatedObjs(DataArray,UndoArray[Size-1]);
				}
					
			}
	}
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::Redo() //重绘
{
	// TODO: Add your dispatch handler code here
	int Index,SelColor,Size,GetIndex,UndoSize,inSize,inIndex;
	int xRate = 0;
	int yRate = 0;
	DataInfo AddDataInfo;
	Size=RedoArray.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo=RedoArray[Size-1];
	SelColor=GetDataInfo.SelectColor;
	UndoArray.push_back(GetDataInfo);	
	RedoArray.pop_back();
	Size=RedoArray.size();
	if(SelColor==-2)//多个对象移动
	{
		DataInfo UndoDataInfo;
		Size=GetDataInfo.ComArray.size();
		UndoSize=UndoArray.size()-1;
		for(Index=0;Index<Size;Index++)
		{
			ComInfo AddComInfo;
			GetIndex=GetDataInfo.ComArray[Index].ObjIndex;
			AddComInfo.ObjIndex=GetIndex;
			AddComInfo.xStartRate=DataArray[GetIndex].StartPoint.x;
			AddComInfo.yStartRate=DataArray[GetIndex].StartPoint.y;
			AddComInfo.xEndRate=DataArray[GetIndex].EndPoint.x;
			AddComInfo.yEndRate=DataArray[GetIndex].EndPoint.y;
			AddComInfo.xCenterRate=DataArray[GetIndex].CenterPoint.x;
			AddComInfo.yCenterRate=DataArray[GetIndex].CenterPoint.y;
			UndoDataInfo.ComArray.push_back(AddComInfo);
			/////////////////////////////////////////////////////////
			DataArray[GetIndex].StartPoint.x=(int)GetDataInfo.ComArray[Index].xStartRate;
			DataArray[GetIndex].StartPoint.y=(int)GetDataInfo.ComArray[Index].yStartRate;
			DataArray[GetIndex].EndPoint.x=(int)GetDataInfo.ComArray[Index].xEndRate;
			DataArray[GetIndex].EndPoint.y=(int)GetDataInfo.ComArray[Index].yEndRate;
			DataArray[GetIndex].CenterPoint.x=(int)GetDataInfo.ComArray[Index].xCenterRate;
			DataArray[GetIndex].CenterPoint.y=(int)GetDataInfo.ComArray[Index].yCenterRate;
		}
		UndoDataInfo.SelectColor=-2;
		UndoArray[UndoSize]=UndoDataInfo;
	}
	if(SelColor==-3)//区域擦除
	{
		Size=GetDataInfo.ComArray.size();
		UndoSize=UndoArray.size()-1;
		DataInfo UndodoDataInfo;
		for(Index=0;Index<Size;Index++)
		{
			ComInfo AddComInfo;
			GetIndex=GetDataInfo.ComArray[Index].ObjIndex;
			AddComInfo.ObjIndex=GetIndex;
			UndoDataInfo.ComArray.push_back(AddComInfo);
			DataArray[GetIndex].IsErased=false;
			inSize=DataArray[GetIndex].ComArray.size();
			for(inIndex=0;inIndex<inSize;inIndex++)
				DataArray[DataArray[GetIndex].ComArray[inIndex].ObjIndex].IsErased=false;

		}
		UndoDataInfo.SelectColor=-3;
		UndoArray[UndoSize]=UndoDataInfo;
	}
	if(SelColor==-4)
	{
		pbUnit=pbBaseUnit;
		Matrix matrix;
		Graphics graphics(hDC);
		int Size=GetDataInfo.ComArray.size();
		for(Index=0;Index<Size;Index++)
		{
			GetIndex=GetDataInfo.ComArray[Index].ObjIndex;
			switch(DataArray[GetIndex].OperationKind)
			{
			case 0:
				pbUnit=pbLineUnit;
				break;
			case 1:
				pbUnit=pbRectUnit;
				break;
			case 2:
				pbUnit=pbRoundRectUnit;
				break;
			case 3:
				pbUnit=pbEllipseUnit;
				break;
			case 4:
				pbUnit=pbArcUnit;
				break;
			case 5:
				pbUnit=pbSectorUnit;
				break;
			case 6:
				pbUnit=pbPenBrushUnit;
				break;
			case 7:
				pbUnit=pbPolylineUnit;
				break;
			case 8:
				pbUnit=pbTextUnit;
				break;
			case 9:
				pbUnit=pbBmpUnit;
				break;
			case 10:
				pbUnit=pbCombination;
				break;
			case 11:
				pbUnit=pbPolygonUnit;
				break;
			}
			/////////////////////////////////////////////////
			HRGN hRgn=pbUnit->GetRedrawHRGN(hDC,DataArray,DataArray[GetIndex]);
			Region tRgn(hRgn);
			Region Rgn(GetDataInfo.hRgn);
			matrix.Rotate(-DataArray[GetIndex].RotateAngle);	 
			matrix.Translate(-DataArray[GetIndex].CenterPoint.x,-DataArray[GetIndex].CenterPoint.y);	
			Rgn.Transform(&matrix);
			matrix.Reset();			
			tRgn.Intersect(&Rgn);
			Region uRgn(DataArray[GetIndex].hRgn);
			uRgn.Union(&tRgn);
			DataArray[GetIndex].hRgn=uRgn.GetHRGN(&graphics);
		}
	}
	if(SelColor>=0)
	{
		Region Rgn(Rect(0,0,0,0));
		Graphics graphics(hDC);
		GetDataInfo.hRgn=Rgn.GetHRGN(&graphics);
		xRate = GetDataInfo.StartPoint.x - DataArray[SelColor].StartPoint.x;
		yRate = GetDataInfo.StartPoint.y - DataArray[SelColor].StartPoint.y;
		DataArray[SelColor]=GetDataInfo;
		DataArray[SelColor].IsErased=false;
		if(GetDataInfo.OperationKind==10)
		{
			Size=GetDataInfo.ComArray.size();
			for(Index=0;Index<Size;Index++)
			{
				DataArray[GetDataInfo.ComArray[Index].ObjIndex].IsCombination=true;
				/*DataArray[GetDataInfo.ComArray[Index].ObjIndex].StartPoint.x += xRate;
				DataArray[GetDataInfo.ComArray[Index].ObjIndex].StartPoint.y += yRate;
				DataArray[GetDataInfo.ComArray[Index].ObjIndex].EndPoint.x += xRate;
				DataArray[GetDataInfo.ComArray[Index].ObjIndex].EndPoint.y += yRate;
				DataArray[GetDataInfo.ComArray[Index].ObjIndex].CenterPoint.x += xRate;
				DataArray[GetDataInfo.ComArray[Index].ObjIndex].CenterPoint.y += yRate;*/
				pbCombination->ModifyCombinatedObjs(DataArray,GetDataInfo);
			}
		}
	}
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::SelectAllObj() //通过一个对象要实现全部选择时调用此函数
{
	// TODO: Add your dispatch handler code here
	CRect rect;
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	/////////////////////////////////////////////////////////////////////////
	MouseCur=NULL;
	SelObjList.clear();
	RotateRgn.X=0;
	RotateRgn.Y=0;
	RotateRgn.Width=0;
	RotateRgn.Height=0;
	StretchRgn.X=0;
	StretchRgn.Y=0;
	StretchRgn.Width=0;
	StretchRgn.Height=0;
	MenuRgn.X=0;
	MenuRgn.Y=0;
	MenuRgn.Width=0;
	MenuRgn.Height=0;
	MoveRgn.X=0;
	MoveRgn.Y=0;
	MoveRgn.Width=0;
	MoveRgn.Height=0;
	PlayRgn.X=0;
	PlayRgn.Y=0;
	PlayRgn.Width=0;
	PlayRgn.Height=0;
	/////////////////////////////////////////////////////////////////////////
	rect.CopyRect(&WindowRect);
	int Size=DataArray.size();
	for(int Index=0;Index<Size;Index++)
	{
		if(rect.PtInRect(DataArray[Index].CenterPoint))
		{		
			SelObj AddSelObj;
			AddSelObj.SelDataInfo=DataArray[Index];
			AddSelObj.Index=Index;
			SelObjList.push_back(AddSelObj);
		}
	}
	Size=SelObjList.size();
	if(Size==1)
		ObjSelected();
	if(Size>1)
	{
		DataInfo tDataInfo;
		int mid;
		CPoint bPoint(WindowRect.right,WindowRect.bottom);
		CPoint tPoint(WindowRect.left,WindowRect.top);
		for(int Index=0;Index<Size;Index++)
		{
			tDataInfo=SelObjList[Index].SelDataInfo;
			mid=(tDataInfo.StartPoint.x<tDataInfo.EndPoint.x)?tDataInfo.StartPoint.x:tDataInfo.EndPoint.x;
			bPoint.x=(bPoint.x<mid)?bPoint.x:mid;
			mid=(tDataInfo.StartPoint.y<tDataInfo.EndPoint.y)?tDataInfo.StartPoint.y:tDataInfo.EndPoint.y;
			bPoint.y=(bPoint.y<mid)?bPoint.y:mid;
			mid=(tDataInfo.StartPoint.x>tDataInfo.EndPoint.x)?tDataInfo.StartPoint.x:tDataInfo.EndPoint.x;
			tPoint.x=(tPoint.x>mid)?tPoint.x:mid;
			mid=(tDataInfo.StartPoint.y>tDataInfo.EndPoint.y)?tDataInfo.StartPoint.y:tDataInfo.EndPoint.y;
			tPoint.y=(tPoint.y>mid)?tPoint.y:mid;
		}
		ComObjRect.SetRect(bPoint,tPoint);
		ComRect.SetRect(bPoint,tPoint);
		ObjsSelected();

	}
	m_operations=11;
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::ErasedRegion(short left, short top, short right, short bottom) //区域擦除
{
	// TODO: Add your dispatch handler code here
	CRect rect(left,top,right,bottom);
	DataInfo AddDataInfo;
	int Size=DataArray.size();
	for(int Index=0;Index<Size;Index++)
	{
		if(rect.PtInRect(DataArray[Index].CenterPoint)&&!DataArray[Index].IsErased)
		{	
			ComInfo AddComInfo;
			AddComInfo.ObjIndex=Index;
			DataArray[Index].IsErased=true;
			AddDataInfo.ComArray.push_back(AddComInfo);
		}
	}
	AddDataInfo.SelectColor=-3;
	UndoArray.push_back(AddDataInfo);
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
	PageChanged();//事件触发
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::ZoomIn(float mul) //屏幕缩小
{
	// TODO: Add your dispatch handler code here
	SelObjList.clear();
	if(mul<0.1)
		return false;
	DisplayRect.InflateRect(DisplayRect.Width()*mul,DisplayRect.Height()*mul);
	int Size=bkImageArray.size();
	int Index;
	CPoint sPoint,ePoint,cPoint;
	for(Index=0;Index<Size;Index++)
	{
		cPoint=bkImageArray[Index].ptImage;
		cPoint.x=(cPoint.x-WindowRect.Width()/2)*mul+WindowRect.Width()/2;
		cPoint.y=(cPoint.y-WindowRect.Height()/2)*mul+WindowRect.Height()/2;
		bkImageArray[Index].ptImage=cPoint;
		bkImageArray[Index].xScale*=mul;
		bkImageArray[Index].yScale*=mul;
	}
	Size=DataArray.size();
	for(Index=0;Index<Size;Index++)
	{
		ObjZoomOperation(Index,mul);
	}
	InitErasureImage();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::DeleteSelObj() //删除选中的对象
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	DataInfo AddDataInfo;
	int GetIndex=SelObjList[0].Index;
	ComInfo AddComInfo;
	AddComInfo.ObjIndex=GetIndex;
	AddComInfo.xScale=-1;
	DataArray[GetIndex].IsErased=true;
	Size=DataArray[GetIndex].ComArray.size();
	for(int Index=0;Index<Size;Index++)
		DataArray[DataArray[GetIndex].ComArray[Index].ObjIndex].IsErased=true;
	AddDataInfo.ComArray.push_back(AddComInfo);
	AddDataInfo.SelectColor=-3;
	UndoArray.push_back(AddDataInfo);
	SelObjList.clear();
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::CopySelObj() //复制对象
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	CopyArray.clear();
	DataInfo GetDataInfo;
	GetDataInfo=SelObjList[0].SelDataInfo;
	InCopySelObj(GetDataInfo);
	CopyArray.push_back(GetDataInfo);
	////////////////////////////////////////////
	if(GetDataInfo.OperationKind==9)
	{
		TranslateImageToClipboard(GetDataInfo);
	}
	////////////////////////////////////////////
	return TRUE;
}

void CSCTDrawActivexCtrl::InCopySelObj(DataInfo &tDataInfo)//复制对象
{
	int Size=tDataInfo.ComArray.size();
	for(int Index=0;Index<Size;Index++)
	{
		DataInfo AddDataInfo;
		AddDataInfo=DataArray[tDataInfo.ComArray[Index].ObjIndex];	
		InCopySelObj(AddDataInfo);
		int Num=CopyArray.size();
		tDataInfo.ComArray[Index].ObjIndex=Num;
		CopyArray.push_back(AddDataInfo);
	}
}

BOOL CSCTDrawActivexCtrl::PasteObj() //粘贴对象
{
	// TODO: Add your dispatch handler code here
	int Index;
	int Size=CopyArray.size();
	if(Size==0)
		return false;
	int Start=DataArray.size();
	for(Index=0;Index<Size;Index++)
	{
		DataArray.push_back(CopyArray[Index]);
		DataArray[Start+Index].SelectColor=Start+Index;
		InPasteObj(Start,DataArray[Start+Index]);
		//UndoArray.push_back(CopyArray[Index]);
		//UndoArray[UndoArray.size()-1].SelectColor=Start+Index;
	}

	DataInfo GetDataInfo=DataArray[Start+Index-1];
	//InPasteObj(Start,GetDataInfo)
	//DataArray[DataArray.size()-1]=GetDataInfo;
	UndoArray[UndoArray.size()-1]=GetDataInfo;
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
	return TRUE;
}


void CSCTDrawActivexCtrl::InPasteObj(int Start,DataInfo &tDataInfo)//粘贴对象
{
	int Size=tDataInfo.ComArray.size();
	for(int Index=0;Index<Size;Index++)
	{
		tDataInfo.ComArray[Index].ObjIndex+=Start;
	}
}

BOOL CSCTDrawActivexCtrl::LockSelObj(BOOL IsLocked) //锁定所选中的对象
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	//////////////////////////////////////////
	MouseCur=NULL;
	RotateRgn.X=0;
	RotateRgn.Y=0;
	RotateRgn.Width=0;
	RotateRgn.Height=0;
	StretchRgn.X=0;
	StretchRgn.Y=0;
	StretchRgn.Width=0;
	StretchRgn.Height=0;
	MenuRgn.X=0;
	MenuRgn.Y=0;
	MenuRgn.Width=0;
	MenuRgn.Height=0;
	MoveRgn.X=0;
	MoveRgn.Y=0;
	MoveRgn.Width=0;
	MoveRgn.Height=0;
	PlayRgn.X=0;
	PlayRgn.Y=0;
	PlayRgn.Width=0;
	PlayRgn.Height=0;
	ComRect.SetRect(0,0,0,0);
	ComObjRect.SetRect(0,0,0,0);
	/////////////////////////////////////////////
	DataArray[SelObjList[0].Index].IsLocked=IsLocked;
	DataInfo GetDataInfo=DataArray[SelObjList[0].Index];
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	SelObjList.clear();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::DeletePage(short PageIndex) //删除页面时用到
{
	// TODO: Add your dispatch handler code here
	int Size=PageArray.size();
	if(PageIndex<0||PageIndex>=Size)
		return false;
	if(PageIndex==PageNum)
	{
		ClearData();
	}
	if(Size==1)
	{
		PageNum=0;
		memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	}
	else
	{
		HGLOBAL hmem;
		for(int Index=0;Index<PageArray[PageIndex].bkImageArray.size();Index++)
		{
			hmem=GlobalHandle(PageArray[PageIndex].bkImageArray[Index].ImageByte);
			GlobalLock(hmem);
			GlobalFree(hmem);
			GlobalUnlock(hmem);
		}
		PageArray[PageIndex].bkImageArray.clear();
		DeleteObject(PageArray[PageIndex].hMemBmp); 
		for(int Index=0;Index<PageArray[PageIndex].DataArray.size();Index++)
		{
			if(PageArray[PageIndex].DataArray[Index].OperationKind==9)
			{
				hmem=GlobalHandle(PageArray[PageIndex].DataArray[Index].ImageByte);
				GlobalLock(hmem);
				GlobalFree(hmem);
				GlobalUnlock(hmem);
			}
		}
		PageArray.erase(PageArray.begin()+PageIndex);
		PageNum=-1;
	}

	return TRUE;
}

BOOL CSCTDrawActivexCtrl::GetSelObjIsLocked()//判断所选对象是否被锁定 
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.IsLocked;
}

short CSCTDrawActivexCtrl::GetSelObjStartPointX() //得到所选中对象的起始点X坐标
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	/////////////////////////////////////////////////////////////
	CPoint sPoint,ePoint,pt1,pt2;
	int mid;
	//////////////////////////////////////////////////
	sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	sPoint.x*=GetDataInfo.xScale;
	sPoint.y*=GetDataInfo.yScale;
	ePoint.x*=GetDataInfo.xScale;
	ePoint.y*=GetDataInfo.yScale;
	pt1.x=sPoint.x;
	pt1.y=ePoint.y;
	pt2.x=ePoint.x;
	pt2.y=sPoint.y;
	////////////////////////////////////////////////////
	double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	double Angle=atan2(float(sPoint.y),float(sPoint.x))+GetDataInfo.RotateAngle/Rate;
	sPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	sPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+GetDataInfo.RotateAngle/Rate;
	ePoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	ePoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt1.x*pt1.x)+float(pt1.y*pt1.y));
	Angle=atan2(float(pt1.y),float(pt1.x))+GetDataInfo.RotateAngle/Rate;
	pt1.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt1.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt2.x*pt2.x)+float(pt2.y*pt2.y));
	Angle=atan2(float(pt2.y),float(pt2.x))+GetDataInfo.RotateAngle/Rate;
	pt2.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt2.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////
	mid=(pt1.x<pt2.x)?pt1.x:pt2.x;
	mid=(sPoint.x<mid)?sPoint.x:mid;
	mid=(mid<ePoint.x)?mid:ePoint.x;
	return mid;
}

short CSCTDrawActivexCtrl::GetSelObjStartPointY()//得到所选对象的起始Y坐标 
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	/////////////////////////////////////////////////////////////
	CPoint sPoint,ePoint,pt1,pt2;
	int mid;
	//////////////////////////////////////////////////
	sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	sPoint.x*=GetDataInfo.xScale;
	sPoint.y*=GetDataInfo.yScale;
	ePoint.x*=GetDataInfo.xScale;
	ePoint.y*=GetDataInfo.yScale;
	pt1.x=sPoint.x;
	pt1.y=ePoint.y;
	pt2.x=ePoint.x;
	pt2.y=sPoint.y;
	////////////////////////////////////////////////////
	double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	double Angle=atan2(float(sPoint.y),float(sPoint.x))+GetDataInfo.RotateAngle/Rate;
	sPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	sPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+GetDataInfo.RotateAngle/Rate;
	ePoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	ePoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt1.x*pt1.x)+float(pt1.y*pt1.y));
	Angle=atan2(float(pt1.y),float(pt1.x))+GetDataInfo.RotateAngle/Rate;
	pt1.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt1.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt2.x*pt2.x)+float(pt2.y*pt2.y));
	Angle=atan2(float(pt2.y),float(pt2.x))+GetDataInfo.RotateAngle/Rate;
	pt2.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt2.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////
	mid=(pt1.y<pt2.y)?pt1.y:pt2.y;
	mid=(sPoint.y<mid)?sPoint.y:mid;
	mid=(mid<ePoint.y)?mid:ePoint.y;
	return mid;
}

short CSCTDrawActivexCtrl::GetSelObjEndPointX() //得到所选对象的终点X坐标
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	/////////////////////////////////////////////////////////////
	CPoint sPoint,ePoint,pt1,pt2;
	int mid;
	//////////////////////////////////////////////////
	sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	sPoint.x*=GetDataInfo.xScale;
	sPoint.y*=GetDataInfo.yScale;
	ePoint.x*=GetDataInfo.xScale;
	ePoint.y*=GetDataInfo.yScale;
	pt1.x=sPoint.x;
	pt1.y=ePoint.y;
	pt2.x=ePoint.x;
	pt2.y=sPoint.y;
	////////////////////////////////////////////////////
	double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	double Angle=atan2(float(sPoint.y),float(sPoint.x))+GetDataInfo.RotateAngle/Rate;
	sPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	sPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+GetDataInfo.RotateAngle/Rate;
	ePoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	ePoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt1.x*pt1.x)+float(pt1.y*pt1.y));
	Angle=atan2(float(pt1.y),float(pt1.x))+GetDataInfo.RotateAngle/Rate;
	pt1.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt1.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt2.x*pt2.x)+float(pt2.y*pt2.y));
	Angle=atan2(float(pt2.y),float(pt2.x))+GetDataInfo.RotateAngle/Rate;
	pt2.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt2.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////
	mid=(pt1.x>pt2.x)?pt1.x:pt2.x;
	mid=(sPoint.x>mid)?sPoint.x:mid;
	mid=(mid>ePoint.x)?mid:ePoint.x;
	return mid;
}

short CSCTDrawActivexCtrl::GetSelObjEndPointY() //得到所选对象的终点Y坐标
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	/////////////////////////////////////////////////////////////
	CPoint sPoint,ePoint,pt1,pt2;
	int mid;
	//////////////////////////////////////////////////
	sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	sPoint.x*=GetDataInfo.xScale;
	sPoint.y*=GetDataInfo.yScale;
	ePoint.x*=GetDataInfo.xScale;
	ePoint.y*=GetDataInfo.yScale;
	pt1.x=sPoint.x;
	pt1.y=ePoint.y;
	pt2.x=ePoint.x;
	pt2.y=sPoint.y;
	////////////////////////////////////////////////////
	double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	double Angle=atan2(float(sPoint.y),float(sPoint.x))+GetDataInfo.RotateAngle/Rate;
	sPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	sPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+GetDataInfo.RotateAngle/Rate;
	ePoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	ePoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt1.x*pt1.x)+float(pt1.y*pt1.y));
	Angle=atan2(float(pt1.y),float(pt1.x))+GetDataInfo.RotateAngle/Rate;
	pt1.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt1.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt2.x*pt2.x)+float(pt2.y*pt2.y));
	Angle=atan2(float(pt2.y),float(pt2.x))+GetDataInfo.RotateAngle/Rate;
	pt2.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt2.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////
	mid=(pt1.y>pt2.y)?pt1.y:pt2.y;
	mid=(sPoint.y>mid)?sPoint.y:mid;
	mid=(mid>ePoint.y)?mid:ePoint.y;
	return mid;
}

short CSCTDrawActivexCtrl::GetSelObjCenterPointX() //得到所选对象的中心点X坐标
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.CenterPoint.x;
}

short CSCTDrawActivexCtrl::GetSelObjCenterPointY() //得到所选对象的中心点Y坐标
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.CenterPoint.y;
}

double CSCTDrawActivexCtrl::GetSelObjRotateAngle() //得到所选对象的旋转角度
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.RotateAngle;
}

short CSCTDrawActivexCtrl::GetSelObjLineStyle() //得到所选对象的线类型
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.LineStyle;
}

short CSCTDrawActivexCtrl::GetSelObjFillStyle() //得到所选对象的填充类型
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.FillStyle;
}

short CSCTDrawActivexCtrl::GetSelObjPenWidth() //得到所选对象的笔宽
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.penWidth;
}

long CSCTDrawActivexCtrl::GetSelObjPenColor() //得到所选对象的笔的颜色
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.penColor;
}

long CSCTDrawActivexCtrl::GetSelObjFillColor() //得到所选对象的填充颜色
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.FillColor;
}

long CSCTDrawActivexCtrl::GetSelObjFontColor() //得到所选对象的字体颜色
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.FontColor;
}

long CSCTDrawActivexCtrl::GetSelObjForeColor() //得到所选对象的前景颜色
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	return GetDataInfo.foreColor;
}

short CSCTDrawActivexCtrl::GetPageNum() //得到页的总数
{
	// TODO: Add your dispatch handler code here
	int Size=PageArray.size();
	return Size;
}

BOOL CSCTDrawActivexCtrl::CopyPage(short PageIndex) //复制页
{
	// TODO: Add your dispatch handler code here
	int Size=PageArray.size();
	if(PageIndex<0||PageIndex>=Size)
		return false;
	CopyPageArray.clear();
	PageInfo AddPage;
	if(PageIndex==PageNum) 
		SavePage();
	AddPage=PageArray[PageIndex];
	CopyPageArray.push_back(AddPage);
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::PastPage(short PageIndex)//粘贴页 
{
	// TODO: Add your dispatch handler code here
	int Size=CopyPageArray.size();
	if(Size<0)
		return false;
	PageInfo AddPage=CopyPageArray[0];
	PageArray[PageIndex]=AddPage;
	if(PageIndex==PageNum)
	{
		ClearData();
		PageInfo GetPage=PageArray[PageIndex];
		m_bkColor=GetPage.bkColor; 
		m_fillStyle=GetPage.FillStyle;
		m_fillColor=GetPage.FillColor;
		m_foreColor=GetPage.ForeColor;	
		m_operations=GetPage.Operations;
		m_penWidth=GetPage.PenWidth;
		m_penColor=GetPage.PenColor;
		m_fontColor=GetPage.FontColor;
		m_bkColor=GetPage.bkColor;
		m_lineStyle=GetPage.LineStyle;
		int Size=GetPage.DataArray.size();
		//SelectColor=Size;
		for(int i=0;i<Size;i++)
			DataArray.push_back(GetPage.DataArray[i]);
		Size=GetPage.bkImageArray.size();
		for(int k=0;k<Size;k++)
			bkImageArray.push_back(GetPage.bkImageArray[k]);

		memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
		SetBackGroundBmp();
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	}
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::EraseAll() //全部删除
{
	// TODO: Add your dispatch handler code here
	int Size=DataArray.size();
	DataInfo GetDataInfo;
	DataInfo UndoDataInfo;
	for(int Index=0;Index<Size;Index++)
	{
		GetDataInfo=DataArray[Index];
		if(!GetDataInfo.IsDisjoint&&!GetDataInfo.IsErased&&!GetDataInfo.IsCombination)
		{
			ComInfo AddComInfo;
			AddComInfo.ObjIndex=Index;
			GetDataInfo.IsErased=true;
			DataArray[Index]=GetDataInfo;
			UndoDataInfo.ComArray.push_back(AddComInfo);
		}
	}
	UndoDataInfo.SelectColor=-3;
	UndoArray.push_back(UndoDataInfo);
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
	PageChanged();//事件触发
	return TRUE;
}

long CSCTDrawActivexCtrl::SaveDataToFile(LPCTSTR FilePath) //保存白板数据
{
	// TODO: Add your dispatch handler code here
	SavePage();
	CFile file(FilePath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	UINT pNum=PageArray.size();
	UINT pIndex,Size,Index;
	IStream* pfIStream= NULL;
	STATSTG statstg;
	HGLOBAL hGlobal;
	CreateStreamOnHGlobal(NULL,TRUE,&pfIStream);
	while(pfIStream->Write(&pNum,sizeof(pNum),NULL)!=S_OK);
	PageInfo GetPageInfo;
	DataInfo wDataInfo;
	bkImageInfo wbkImageInfo;
	for(pIndex=0;pIndex<pNum;pIndex++)
	{
		GetPageInfo=PageArray[pIndex];
		/////////////////////////保存PageInfo信息///////////////////////
		while(pfIStream->Write(&GetPageInfo.Operations,sizeof(GetPageInfo.Operations),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.PenWidth,sizeof(GetPageInfo.PenWidth),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.PenColor,sizeof(GetPageInfo.PenColor),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.FillColor,sizeof(GetPageInfo.FillColor),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.FontColor,sizeof(GetPageInfo.FontColor),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.bkColor,sizeof(GetPageInfo.bkColor),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.LineStyle,sizeof(GetPageInfo.LineStyle),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.FillStyle,sizeof(GetPageInfo.FillStyle),NULL)!=S_OK);
		while(pfIStream->Write(&GetPageInfo.ForeColor,sizeof(GetPageInfo.ForeColor),NULL)!=S_OK);
		///////////////////////////保存DataArray信息/////////////////////
		Size=PageArray[pIndex].DataArray.size();//写入DataArray个数
		int Count=0;
		for(Index=0;Index<Size;Index++)
		{
			wDataInfo=GetPageInfo.DataArray[Index];
			if(wDataInfo.IsDisjoint||wDataInfo.IsErased)
				continue;
			Count++;
		}
		while(pfIStream->Write(&Count,sizeof(Count),NULL)!=S_OK);

		for(Index=0;Index<Size;Index++)
		{	
			wDataInfo=GetPageInfo.DataArray[Index];
			if(wDataInfo.IsDisjoint||wDataInfo.IsErased)
				continue;
			/////////////////////////////////////////////
			while(pfIStream->Write(&wDataInfo.OperationKind,sizeof(wDataInfo.OperationKind),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.IsCombination,sizeof(wDataInfo.IsCombination),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.IsDisjoint,sizeof(wDataInfo.IsDisjoint),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.IsErased,sizeof(wDataInfo.IsErased),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.IsLocked,sizeof(wDataInfo.IsLocked),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.StartPoint,sizeof(wDataInfo.StartPoint),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.EndPoint,sizeof(wDataInfo.EndPoint),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.xScale,sizeof(wDataInfo.xScale),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.yScale,sizeof(wDataInfo.yScale),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.penWidth,sizeof(wDataInfo.penWidth),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.preAngle,sizeof(wDataInfo.preAngle),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.RotateAngle,sizeof(wDataInfo.RotateAngle),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.startAngle,sizeof(wDataInfo.startAngle),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.sweepAngle,sizeof(wDataInfo.sweepAngle),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.CenterPoint,sizeof(wDataInfo.CenterPoint),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.ImageSize,sizeof(int),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.charNum,sizeof(wDataInfo.charNum),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.logfontw,sizeof(wDataInfo.logfontw),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.PointNum,sizeof(wDataInfo.PointNum),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.LineStyle,sizeof(wDataInfo.LineStyle),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.FillStyle,sizeof(wDataInfo.FillStyle),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.foreColor,sizeof(wDataInfo.foreColor),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.penColor,sizeof(wDataInfo.penColor),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.FillColor,sizeof(wDataInfo.FillColor),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.FontColor,sizeof(wDataInfo.FontColor),NULL)!=S_OK);
			while(pfIStream->Write(&Index,sizeof(Index),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.Alpha,sizeof(&wDataInfo.Alpha),NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.Reserved0,200,NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.Reserved1,200,NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.Reserved2,200,NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.Reserved3,200,NULL)!=S_OK);
			while(pfIStream->Write(&wDataInfo.Reserved4,1024,NULL)!=S_OK);
			/////////////////////////////////////////////
			Region Rgn(wDataInfo.hRgn);
			int RgnSize=Rgn.GetDataSize();
			while(pfIStream->Write(&RgnSize,sizeof(RgnSize),NULL)!=S_OK);
			BYTE *RgnData;
			RgnData= new BYTE[RgnSize];
			Rgn.GetData(RgnData,RgnSize);
			while(pfIStream->Write(RgnData,RgnSize,NULL)!=S_OK);
			delete RgnData;
			switch(wDataInfo.OperationKind)
			{
			case 4:
			case 5:
			case 6:
			case 7:
			case 11://毛笔,铅笔与多边形附加信息
				if(true)
				{
					RelRate GetRelRate;
					int GetSize=wDataInfo.AllRate.size();
					while(pfIStream->Write(&GetSize,sizeof(GetSize),NULL)!=S_OK);
					for(int GetIndex=0;GetIndex<GetSize;GetIndex++)
					{
						GetRelRate=wDataInfo.AllRate[GetIndex];
						while(pfIStream->Write(&GetRelRate,sizeof(GetRelRate),NULL)!=S_OK);
					}
					if(wDataInfo.LineStyle==TextureStyle)
						while(pfIStream->Write(wDataInfo.ImageByte,wDataInfo.ImageSize,NULL)!=S_OK);
				}
				break;
			case 8://文字附加信息
				while(pfIStream->Write(wDataInfo.SaveStr,wDataInfo.charNum,NULL)!=S_OK);
				break;
			case 9://图片信息
				if(true)
				{
					while(pfIStream->Write(wDataInfo.ImageByte,wDataInfo.ImageSize,NULL)!=S_OK);
					////////////////////////////////////////////////////////////////////////////////
					while(pfIStream->Write(&wDataInfo.ChartInfo.Type,sizeof(int),NULL)!=S_OK);
					while(pfIStream->Write(&wDataInfo.ChartInfo.bkColor,sizeof(COLORREF),NULL)!=S_OK);
					while(pfIStream->Write(&wDataInfo.ChartInfo.CharNum,sizeof(int),NULL)!=S_OK);
					while(pfIStream->Write(wDataInfo.ChartInfo.HeadLine,wDataInfo.ChartInfo.CharNum,NULL)!=S_OK);
					int cSize=wDataInfo.ChartInfo.ChartDataArray.size();
					while(pfIStream->Write(&cSize,sizeof(cSize),NULL)!=S_OK);
					for(int GetIndex=0;GetIndex<cSize;GetIndex++)
					{ 
						while(pfIStream->Write(&wDataInfo.ChartInfo.ChartDataArray[GetIndex].color,sizeof(COLORREF),NULL)!=S_OK);
						while(pfIStream->Write(&wDataInfo.ChartInfo.ChartDataArray[GetIndex].number,sizeof(float),NULL)!=S_OK);
						while(pfIStream->Write(&wDataInfo.ChartInfo.ChartDataArray[GetIndex].CharNum,sizeof(int),NULL)!=S_OK);
						while(pfIStream->Write(wDataInfo.ChartInfo.ChartDataArray[GetIndex].name,wDataInfo.ChartInfo.ChartDataArray[GetIndex].CharNum,NULL)!=S_OK);
					}
				}
				break;
			case 10:
				if(true)
				{
					ComInfo GetComInfo;
					int GetSize=wDataInfo.ComArray.size();
					while(pfIStream->Write(&GetSize,sizeof(GetSize),NULL)!=S_OK);
					for(int GetIndex=0;GetIndex<GetSize;GetIndex++)
					{
						GetComInfo=wDataInfo.ComArray[GetIndex];
						while(pfIStream->Write(&GetComInfo,sizeof(GetComInfo),NULL)!=S_OK);
					}
				}
				break;
			case 12:
				if(true)
				{
					int i;
					PathData FillPathData,OutlinePathData;
					wDataInfo.pFillPath->GetPathData(&FillPathData);
					wDataInfo.pOutlinePath->GetPathData(&OutlinePathData);
					////////////////////写入填充路径/////////////////////
					while(pfIStream->Write(&FillPathData.Count,sizeof(FillPathData.Count),NULL)!=S_OK);
					for(i=0;i<FillPathData.Count;i++)
					{
						while(pfIStream->Write(&FillPathData.Points[i],sizeof(PointF),NULL)!=S_OK);
						while(pfIStream->Write(&FillPathData.Types[i],sizeof(BYTE),NULL)!=S_OK);
					}
					////////////////////写入轮廓路径/////////////////////
					while(pfIStream->Write(&OutlinePathData.Count,sizeof(OutlinePathData.Count),NULL)!=S_OK);
					for(i=0;i<OutlinePathData.Count;i++)
					{
						while(pfIStream->Write(&OutlinePathData.Points[i],sizeof(PointF),NULL)!=S_OK);
						while(pfIStream->Write(&OutlinePathData.Types[i],sizeof(BYTE),NULL)!=S_OK);
					}
				}
				break;
			}
		}
		//////////////////////////保存bkImageInfo信息///////////////////
		Size=PageArray[pIndex].bkImageArray.size();
		while(pfIStream->Write(&Size,sizeof(Size),NULL)!=S_OK);
		for(Index=0;Index<Size;Index++)
		{
			wbkImageInfo=GetPageInfo.bkImageArray[Index];
			while(pfIStream->Write(&wbkImageInfo,sizeof(wbkImageInfo),NULL)!=S_OK);
			while(pfIStream->Write(wbkImageInfo.ImageByte,wbkImageInfo.ImageSize,NULL)!=S_OK);
		}
	}	
	///////////////////////////////////////////////////////////////
	pfIStream->Stat(&statstg,STATFLAG_NONAME);
	uLong SourceLen=statstg.cbSize.QuadPart;
	/////////////////////////////////////////////
	hGlobal=GlobalAlloc(GMEM_FIXED,SourceLen);
	GetHGlobalFromStream(pfIStream,&hGlobal);
	BYTE *SourceStream;
	SourceStream=(BYTE*)GlobalLock(hGlobal);
	GlobalUnlock(hGlobal);  
	////////////////////////////////////////////	
	file.Write(&SourceLen,sizeof(SourceLen));
	file.Write(SourceStream,SourceLen);
	////////////////////////////////////////////////////////////////		 
	file.Close();
	GlobalFree(hGlobal);
	return 0;
}

long CSCTDrawActivexCtrl::ReadDataFromFile(LPCTSTR FilePath) //读取白板数据
{
	// TODO: Add your dispatch handler code here
	CopyPageArray.clear();
	PageArray.clear();
	ClearDataInfo();
	ClearData();
	PageNum=-1;
	Graphics graphics(memDC->m_hDC);
	CFile file(FilePath,CFile::modeRead|CFile::typeBinary);
	////////////////////////////////////////////////////////////////
	uLong OrigLen;
	file.Read(&OrigLen,sizeof(OrigLen));
	OrigLen*=sizeof(BYTE);
	HGLOBAL hGlobal=::GlobalAlloc(GHND,OrigLen);
	BYTE *OrigStream=(BYTE*)::GlobalLock(hGlobal);
	file.Read(OrigStream,OrigLen);
	::GlobalUnlock(hGlobal);	
	file.Close();	

	IStream *pfIStream;
	CreateStreamOnHGlobal((HGLOBAL)OrigStream,true,&pfIStream);
	////////////////////////////////////////////////////////////////
	UINT pNum;
	UINT pIndex,Size,Index;
	while(pfIStream->Read(&pNum,sizeof(pNum),NULL)!=NULL);
	bkImageInfo wbkImageInfo;
	//SetBackGroundBmp();
	for(pIndex=0;pIndex<pNum;pIndex++)
	{    
		PageInfo AddPageInfo;
		/////////////////////////读取PageInfo信息///////////////////////
		while(pfIStream->Read(&AddPageInfo.Operations,sizeof(AddPageInfo.Operations),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.PenWidth,sizeof(AddPageInfo.PenWidth),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.PenColor,sizeof(AddPageInfo.PenColor),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.FillColor,sizeof(AddPageInfo.FillColor),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.FontColor,sizeof(AddPageInfo.FontColor),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.bkColor,sizeof(AddPageInfo.bkColor),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.LineStyle,sizeof(AddPageInfo.LineStyle),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.FillStyle,sizeof(AddPageInfo.FillStyle),NULL)!=S_OK);
		while(pfIStream->Read(&AddPageInfo.ForeColor,sizeof(AddPageInfo.ForeColor),NULL)!=S_OK);
		///////////////////////////保存DataArray信息/////////////////////
		while(pfIStream->Read(&Size,sizeof(Size),NULL)!=S_OK);

		for(Index=0;Index<Size;Index++)
		{		
			DataInfo rDataInfo;
			///////////////////////////////////////////////////////////////
			while(pfIStream->Read(&rDataInfo.OperationKind,sizeof(rDataInfo.OperationKind),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.IsCombination,sizeof(rDataInfo.IsCombination),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.IsDisjoint,sizeof(rDataInfo.IsDisjoint),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.IsErased,sizeof(rDataInfo.IsErased),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.IsLocked,sizeof(rDataInfo.IsLocked),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.StartPoint,sizeof(rDataInfo.StartPoint),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.EndPoint,sizeof(rDataInfo.EndPoint),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.xScale,sizeof(rDataInfo.xScale),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.yScale,sizeof(rDataInfo.yScale),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.penWidth,sizeof(rDataInfo.penWidth),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.preAngle,sizeof(rDataInfo.preAngle),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.RotateAngle,sizeof(rDataInfo.RotateAngle),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.startAngle,sizeof(rDataInfo.startAngle),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.sweepAngle,sizeof(rDataInfo.sweepAngle),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.CenterPoint,sizeof(rDataInfo.CenterPoint),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.ImageSize,sizeof(int),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.charNum,sizeof(rDataInfo.charNum),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.logfontw,sizeof(rDataInfo.logfontw),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.PointNum,sizeof(rDataInfo.PointNum),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.LineStyle,sizeof(rDataInfo.LineStyle),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.FillStyle,sizeof(rDataInfo.FillStyle),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.foreColor,sizeof(rDataInfo.foreColor),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.penColor,sizeof(rDataInfo.penColor),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.FillColor,sizeof(rDataInfo.FillColor),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.FontColor,sizeof(rDataInfo.FontColor),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.SelectColor,sizeof(rDataInfo.SelectColor),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.Alpha,sizeof(&rDataInfo.Alpha),NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.Reserved0,200,NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.Reserved1,200,NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.Reserved2,200,NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.Reserved3,200,NULL)!=S_OK);
			while(pfIStream->Read(&rDataInfo.Reserved4,1024,NULL)!=S_OK);

			//////////////////////////////////////////////////////////////
			int RgnSize;
			while(pfIStream->Read(&RgnSize,sizeof(RgnSize),NULL)!=S_OK);
			BYTE *RgnData;
			RgnData= new BYTE[RgnSize];
			while(pfIStream->Read(RgnData,RgnSize,NULL)!=S_OK);
			Region Rgn(RgnData,RgnSize);
			rDataInfo.hRgn=Rgn.GetHRGN(&graphics);
			Region *ptRgn=Rgn.FromHRGN(rDataInfo.hRgn);
			if(ptRgn==NULL)
			{
				Region tRgn(Rect(0,0,0,0));
				rDataInfo.hRgn=tRgn.GetHRGN(&graphics);
			}
			delete ptRgn;

			switch(rDataInfo.OperationKind)
			{
			case 4:
			case 5:
			case 6:
			case 7:
			case 11://毛笔,铅笔与多边形附加信息
				if(true)
				{
					RelRate AddRelRate;
					int GetSize;
					while(pfIStream->Read(&GetSize,sizeof(GetSize),NULL)!=S_OK);
					for(int GetIndex=0;GetIndex<GetSize;GetIndex++)
					{
						while(pfIStream->Read(&AddRelRate,sizeof(AddRelRate),NULL)!=S_OK);
						rDataInfo.AllRate.push_back(AddRelRate);	
					}
					if(rDataInfo.LineStyle==TextureStyle)
					{
						HGLOBAL hGl=::GlobalAlloc(GHND,rDataInfo.ImageSize);
						rDataInfo.ImageByte=(BYTE*)::GlobalLock(hGl);
						while(pfIStream->Read(rDataInfo.ImageByte,rDataInfo.ImageSize,NULL)!=S_OK); 
						GlobalUnlock(hGl);
					}
				}
				break;
			case 8://文字附加信息
				rDataInfo.SaveStr=new char[rDataInfo.charNum+1];
				memset(rDataInfo.SaveStr,'\0',rDataInfo.charNum+1);
				while(pfIStream->Read(rDataInfo.SaveStr,rDataInfo.charNum,NULL)!=S_OK);
				break;
			case 9:
				if(true)
				{
					HGLOBAL hGl=::GlobalAlloc(GHND,rDataInfo.ImageSize);
					rDataInfo.ImageByte=(BYTE*)::GlobalLock(hGl);
					while(pfIStream->Read(rDataInfo.ImageByte,rDataInfo.ImageSize,NULL)!=S_OK); 
					GlobalUnlock(hGl);
					/////////////////////////////////////////////////////////////////
					while(pfIStream->Read(&rDataInfo.ChartInfo.Type,sizeof(int),NULL)!=S_OK);
					while(pfIStream->Read(&rDataInfo.ChartInfo.bkColor,sizeof(COLORREF),NULL)!=S_OK);
					while(pfIStream->Read(&rDataInfo.ChartInfo.CharNum,sizeof(int),NULL)!=S_OK);
					rDataInfo.ChartInfo.HeadLine=new char[rDataInfo.ChartInfo.CharNum];
					memset(rDataInfo.ChartInfo.HeadLine,'\0',rDataInfo.ChartInfo.CharNum);
					while(pfIStream->Read(rDataInfo.ChartInfo.HeadLine,rDataInfo.ChartInfo.CharNum,NULL)!=S_OK);
					int cSize;
					while(pfIStream->Read(&cSize,sizeof(cSize),NULL)!=S_OK);
					ChartData AddChartData;
					for(int GetIndex=0;GetIndex<cSize;GetIndex++)
					{		
						while(pfIStream->Read(&AddChartData.color,sizeof(COLORREF),NULL)!=S_OK);
						while(pfIStream->Read(&AddChartData.number,sizeof(float),NULL)!=S_OK);
						while(pfIStream->Read(&AddChartData.CharNum,sizeof(int),NULL)!=S_OK);
						AddChartData.name=new char[AddChartData.CharNum];
						memset(AddChartData.name,'\0',AddChartData.CharNum);
						while(pfIStream->Read(AddChartData.name,AddChartData.CharNum,NULL)!=S_OK);
						rDataInfo.ChartInfo.ChartDataArray.push_back(AddChartData);
					}
				}
				break;
			case 10:
				if(true)
				{	
					int GetSize;
					while(pfIStream->Read(&GetSize,sizeof(GetSize),NULL)!=S_OK);
					for(int GetIndex=0;GetIndex<GetSize;GetIndex++)
					{
						ComInfo AddComInfo;
						while(pfIStream->Read(&AddComInfo,sizeof(AddComInfo),NULL)!=S_OK);
						rDataInfo.ComArray.push_back(AddComInfo);
					}
				}
				break;
			case 12:
				if(true)
				{
					int i;
					int Count;
					BYTE *Types;
					PointF *Points; 
					//////////读填充路径//////////////////
					while(pfIStream->Read(&Count,sizeof(Count),NULL)!=S_OK);
					Types=new BYTE[Count];
					Points=new PointF[Count];
					for(i=0;i<Count;i++)
					{ 
						while(pfIStream->Read(&Points[i],sizeof(PointF),NULL)!=S_OK);
						while(pfIStream->Read(&Types[i],sizeof(BYTE),NULL)!=S_OK);
					}
					GraphicsPath FillPath(Points,Types,Count);
					rDataInfo.pFillPath=FillPath.Clone();
					delete [] Types;
					delete [] Points;
					///////////////读轮廓路径/////////////////
					while(pfIStream->Read(&Count,sizeof(Count),NULL)!=S_OK);
					Types=new BYTE[Count];
					Points=new PointF[Count];
					for(i=0;i<Count;i++)
					{ 
						while(pfIStream->Read(&Points[i],sizeof(PointF),NULL)!=S_OK);
						while(pfIStream->Read(&Types[i],sizeof(BYTE),NULL)!=S_OK);
					}
					GraphicsPath OutlinePath(Points,Types,Count);
					rDataInfo.pOutlinePath=OutlinePath.Clone();
					delete [] Types;
					delete [] Points;
				}
				break;
			}
			AddPageInfo.DataArray.push_back(rDataInfo);
		}

		//////////////////////////保存bkImageInfo信息///////////////////
		while(pfIStream->Read(&Size,sizeof(Size),NULL)!=S_OK);
		for(Index=0;Index<Size;Index++)
		{
			bkImageInfo AddbkImageInfo;
			while(pfIStream->Read(&AddbkImageInfo,sizeof(AddbkImageInfo),NULL)!=S_OK);
			/////////////////////////////////////////////////////////////
			HGLOBAL hGlobal=::GlobalAlloc(GHND,AddbkImageInfo.ImageSize);
			AddbkImageInfo.ImageByte=(BYTE*)::GlobalLock(hGlobal);
			while(pfIStream->Read(AddbkImageInfo.ImageByte,AddbkImageInfo.ImageSize,NULL)!=S_OK);
			::GlobalUnlock(hGlobal);
			AddPageInfo.bkImageArray.push_back(AddbkImageInfo);
		}
		//SetBackGroundBmp();
		PageArray.push_back(AddPageInfo);

		/////////读取将要显示页信息/////////////////////
		ClearData();
		m_bkColor=AddPageInfo.bkColor; 
		m_fillStyle=AddPageInfo.FillStyle;
		m_fillColor=AddPageInfo.FillColor;
		m_foreColor=AddPageInfo.ForeColor;	
		m_operations=AddPageInfo.Operations;
		m_penWidth=AddPageInfo.PenWidth;
		m_penColor=AddPageInfo.PenColor;
		m_fontColor=AddPageInfo.FontColor;
		m_bkColor=AddPageInfo.bkColor;
		m_lineStyle=AddPageInfo.LineStyle;
		int Size=AddPageInfo.DataArray.size();
		//SelectColor=Size;
		for(int i=0;i<Size;i++)
			DataArray.push_back(AddPageInfo.DataArray[i]);
		Size=AddPageInfo.bkImageArray.size();
		for(int k=0;k<Size;k++)
			bkImageArray.push_back(AddPageInfo.bkImageArray[k]);
		//////////////////////////////////////////////////////
		InitErasureImage();
		CRgn Rgn;
		Rgn.CreateRectRgn(WindowRect.left,WindowRect.top,WindowRect.right,WindowRect.bottom);
		SelectClipRgn(memDC->m_hDC,Rgn);	
		memDC->FillSolidRect(&WindowRect,m_bkColor);
		SetBackGroundBmp();
		ReDraw(false);
		PageNum=pIndex;
		PageArray[pIndex].hMemBmp=(HBITMAP)CopyImage(memBmp.operator HBITMAP(),IMAGE_BITMAP,WindowRect.Width(),WindowRect.Height(),LR_COPYRETURNORG);
		SelectClipRgn(memDC->m_hDC,NULL);
		//////////////////////////////////////////////////////////
	}
	ShowPage(0);
	return 0;
}

void CSCTDrawActivexCtrl::DotEraseOperation()//点擦除
{
	pbUnit=pbBaseUnit;
	Matrix matrix;
	DataInfo UndoDataInfo;
	Graphics graphics(hDC);
	int Size=DataArray.size();
	for(int Index=0;Index<Size;Index++)
	{
		if(DataArray[Index].IsDisjoint||DataArray[Index].IsErased)
			continue;
			//break;
		GetOperationType(DataArray[Index]);
		/////////////////////////////////////////////////
		ComInfo AddComInfo;
		HRGN hRgn;
		Region ptRgn(ErasedRgnData,ErasedRgnSize);
		hRgn=pbUnit->GetRedrawHRGN(hDC,DataArray,DataArray[Index]);
		Region Rgn(hRgn);
		matrix.Rotate(-DataArray[Index].RotateAngle);	 
		matrix.Translate(-DataArray[Index].CenterPoint.x,-DataArray[Index].CenterPoint.y);	
		ptRgn.Transform(&matrix);
		matrix.Reset();
		Rgn.Intersect(&ptRgn);
		if(!Rgn.IsEmpty(&graphics))
		{
			AddComInfo.ObjIndex=Index;
			UndoDataInfo.ComArray.push_back(AddComInfo);

			Region tRgn(DataArray[Index].hRgn);
			DeleteObject(DataArray[Index].hRgn);
			tRgn.Union(&Rgn);
			DataArray[Index].hRgn=tRgn.GetHRGN(&graphics);
			DeleteObject(hRgn);
			tRgn.MakeEmpty();
			Rgn.MakeEmpty();
		}
	}
	if(UndoDataInfo.ComArray.size()>0)
	{
		UndoDataInfo.SelectColor=-4;
		UndoDataInfo.hRgn=Region(ErasedRgnData,ErasedRgnSize).GetHRGN(&graphics);
		UndoArray.push_back(UndoDataInfo);
	}	 
	delete [] ErasedRgnData;
}

BOOL CSCTDrawActivexCtrl::MoveUp() //将对象移到上一层时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	int GetIndex=SelObjList[0].Index;
	if(GetIndex==DataArray.size()-1)
		return true;
	int Max=GetIndex+1;
	for(int Index=Max;Index<DataArray.size();Index++)
		if(!DataArray[Index].IsCombination&&!DataArray[Index].IsErased&&!DataArray[Index].IsDisjoint)
		{
			Max=Index+1;
			break;
		}
		std::vector<DataInfo>::iterator iter=DataArray.begin();
		if(Max==DataArray.size())
			DataArray.push_back(GetDataInfo);
		else
			DataArray.insert(iter+Max,GetDataInfo);
		iter=DataArray.begin();
		DataArray.erase(iter+GetIndex);
		SelObjList[0].Index=Max-1;
		SelObjList[0].SelDataInfo=GetDataInfo;
		////////////////////////////////////////////////////////////	
		memDC->FillSolidRect(&WindowRect,m_bkColor);
		SetBackGroundBmp();	 
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		if(SelObjList.size()==1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
			ObjSelected();
		if(SelObjList.size()>1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
			ObjsSelected();
		if(IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->RedrawWindow();
		}
		/////////////////////////////////////////////////////////////
		return TRUE;
}

BOOL CSCTDrawActivexCtrl::MoveTop() //将对象置顶时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	int GetIndex=SelObjList[0].Index;
	if(GetIndex==DataArray.size()-1)
		return true;
	if(GetDataInfo.ComArray.size()>0)
		MoveUpObjs(GetIndex,DataArray.size(),GetDataInfo);
	DataArray.push_back(GetDataInfo);
	std::vector<DataInfo>::iterator iter=DataArray.begin();
	DataArray.erase(iter+GetIndex);
	SelObjList[0].Index=DataArray.size()-1;
	SelObjList[0].SelDataInfo=GetDataInfo;
	////////////////////////////////////////////////////////////	
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(SelObjList.size()==1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjSelected();
	if(SelObjList.size()>1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjsSelected();
	if(IsWindow(pEdit->GetSafeHwnd()))
	{
		pEdit->RedrawWindow();
	}
	////////////////////////////////////////////////////////////*/
	return TRUE;
}
void CSCTDrawActivexCtrl::MoveUpObjs(int FromIndex, int ToIndex, DataInfo  & GetDataInfo)//圆柱体，正方体等外部组合对象移到上一层时调用
{
	for(int Index=0;Index<GetDataInfo.ComArray.size();Index++)
	{
		if(GetDataInfo.ComArray[Index].ObjIndex>FromIndex&&GetDataInfo.ComArray[Index].ObjIndex<ToIndex)
		{
			if(DataArray[GetDataInfo.ComArray[Index].ObjIndex].ComArray.size()>0)
				MoveUpObjs(FromIndex,ToIndex,DataArray[GetDataInfo.ComArray[Index].ObjIndex]);
			GetDataInfo.ComArray[Index].ObjIndex--;
		}
	}
}

BOOL CSCTDrawActivexCtrl::MoveDown() //对象移到下一层时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	int GetIndex=SelObjList[0].Index;
	if(GetIndex==0)
		return true;
	int Min=GetIndex-1;
	for(int Index=Min;Index>=0;Index--)
		if(!DataArray[Index].IsCombination&&!DataArray[Index].IsErased&&!DataArray[Index].IsDisjoint)
		{
			Min=Index;
			break;
		}
		if(GetDataInfo.ComArray.size()>0)
			MoveDownObjs(GetIndex,Min,GetDataInfo);
		std::vector<DataInfo>::iterator iter=DataArray.begin();
		DataArray.insert(iter+Min,GetDataInfo);
		iter=DataArray.begin();
		DataArray.erase(iter+GetIndex+1);
		SelObjList[0].Index=Min;
		SelObjList[0].SelDataInfo=GetDataInfo;
		////////////////////////////////////////////////////////////	
		memDC->FillSolidRect(&WindowRect,m_bkColor);
		SetBackGroundBmp();	 
		ReDraw();
		BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		if(SelObjList.size()==1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
			ObjSelected();
		if(SelObjList.size()>1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
			ObjsSelected();
		if(IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->RedrawWindow();
		}
		/////////////////////////////////////////////////////////////
		return TRUE;
}

BOOL CSCTDrawActivexCtrl::MoveBottom() //对象置底
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return false;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	int GetIndex=SelObjList[0].Index;
	if(GetIndex==0)
		return true;
	if(GetDataInfo.ComArray.size()>0)
		MoveDownObjs(GetIndex,0,GetDataInfo);
	std::vector<DataInfo>::iterator iter=DataArray.begin();
	DataArray.insert(iter,GetDataInfo);
	iter=DataArray.begin();
	DataArray.erase(iter+GetIndex+1);
	SelObjList[0].Index=0;
	SelObjList[0].SelDataInfo=GetDataInfo;
	////////////////////////////////////////////////////////////	
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(SelObjList.size()==1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjSelected();
	if(SelObjList.size()>1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjsSelected();
	if(IsWindow(pEdit->GetSafeHwnd()))
	{
		pEdit->RedrawWindow();
	}
	/////////////////////////////////////////////////////////////
	return TRUE;
}

void CSCTDrawActivexCtrl::MoveDownObjs(int FromIndex, int ToIndex, DataInfo & GetDataInfo)//圆柱体，正方体等外部组合对象移到下一层时调用
{
	for(int Index=0;Index<GetDataInfo.ComArray.size();Index++)
	{
		if(GetDataInfo.ComArray[Index].ObjIndex<FromIndex&&GetDataInfo.ComArray[Index].ObjIndex>=ToIndex)
		{
			if(DataArray[GetDataInfo.ComArray[Index].ObjIndex].ComArray.size()>0)
				MoveDownObjs(FromIndex,ToIndex,DataArray[GetDataInfo.ComArray[Index].ObjIndex]);
			GetDataInfo.ComArray[Index].ObjIndex++;
		}
	}
}

int CSCTDrawActivexCtrl::GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	UINT     num   =   0;                     //   number   of   image   encoders   
	UINT     size   =   0;                   //   size   of   the   image   encoder   array   in   bytes   

	ImageCodecInfo*   pImageCodecInfo   =   NULL;   

	GetImageEncodersSize(&num,   &size);   
	if(size   ==   0)   
		return   -1;     //   Failure   

	pImageCodecInfo   =   (ImageCodecInfo*)(malloc(size));   
	if(pImageCodecInfo   ==   NULL)   
		return   -1;     //   Failure   

	GetImageEncoders(num,   size,   pImageCodecInfo);   

	for(UINT   j   =   0;   j   <   num;   ++j)   
	{   
		if(   wcscmp(pImageCodecInfo[j].MimeType,   format)   ==   0   )   
		{   
			*pClsid   =   pImageCodecInfo[j].Clsid;   
			free(pImageCodecInfo);   
			return   j;     //   Success   
		}           
	}    
	free(pImageCodecInfo);   
	return   -1;     //   Failure   
}

BOOL CSCTDrawActivexCtrl::ModifyFillColor(OLE_COLOR nColor) //选择对象后，要改变其填充颜色时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.FillColor=nColor;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyFillColor(GetDataInfo,nColor);
	}
	/////////////////////////////////////////		
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyFillColor(DataInfo tDataInfo, DWORD nColor)//选择对象后，要改变其填充颜色时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].FillColor=nColor;
		if(DataArray[GetIndex].ComArray.size())
			InModifyFillColor(DataArray[GetIndex],nColor);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::ModifyForeColor(OLE_COLOR nForeColor) //选择对象后，要改变其前景颜色时调用此函数
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size<1)
		return false;
	DataInfo GetDataInfo;
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=SelObjList[Index].Index;
		GetDataInfo=SelObjList[Index].SelDataInfo;
		GetDataInfo.foreColor=nForeColor;
		DataArray[GetIndex]=GetDataInfo;
		if(GetDataInfo.ComArray.size())
			InModifyForeColor(GetDataInfo,nForeColor);
	}
	/////////////////////////////////////////		
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	SetBackGroundBmp();	 
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	if(Size==1)
		ObjSelected();
	else
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifyForeColor(DataInfo tDataInfo, DWORD nColor)//选择对象后，要改变其前景颜色时调用此函数
{
	int Size=tDataInfo.ComArray.size();
	int GetIndex;
	for(int Index=0;Index<Size;Index++)
	{
		GetIndex=tDataInfo.ComArray[Index].ObjIndex;
		DataArray[GetIndex].foreColor=nColor;
		if(DataArray[GetIndex].ComArray.size())
			InModifyForeColor(DataArray[GetIndex],nColor);
	}
	return true;
}

BSTR CSCTDrawActivexCtrl::GetReservedProperty0() //?貌似在整个控件中都没有使用到此函数
{
	CString strResult;
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		strResult=GetDataInfo.Reserved0;
	}
	return strResult.AllocSysString();
}

void CSCTDrawActivexCtrl::SetReservedProperty0(LPCTSTR lpszNewValue) //?貌似在整个控件中都没有使用到此函数
{
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		int GetIndex=SelObjList[0].Index;
		memset(GetDataInfo.Reserved0,'\0',100);
		strcpy(GetDataInfo.Reserved0,lpszNewValue);
		SelObjList[0].SelDataInfo=GetDataInfo;
		DataArray[GetIndex]=GetDataInfo;
	}
	SetModifiedFlag();
}

BSTR CSCTDrawActivexCtrl::GetReservedProperty1() //?貌似在整个控件中都没有使用到此函数
{
	CString strResult;
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		strResult=GetDataInfo.Reserved1;
	}
	return strResult.AllocSysString();
}

void CSCTDrawActivexCtrl::SetReservedProperty1(LPCTSTR lpszNewValue) //?貌似在整个控件中都没有使用到此函数
{
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		int GetIndex=SelObjList[0].Index;
		memset(GetDataInfo.Reserved1,'\0',200);
		strcpy(GetDataInfo.Reserved1,lpszNewValue);
		SelObjList[0].SelDataInfo=GetDataInfo;
		DataArray[GetIndex]=GetDataInfo;
	}
	SetModifiedFlag();
}

BSTR CSCTDrawActivexCtrl::GetReservedProperty2() //?貌似在整个控件中都没有使用到此函数
{
	CString strResult;
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		strResult=GetDataInfo.Reserved2;
	}
	return strResult.AllocSysString();
}

void CSCTDrawActivexCtrl::SetReservedProperty2(LPCTSTR lpszNewValue) //?貌似在整个控件中都没有使用到此函数
{
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		int GetIndex=SelObjList[0].Index;
		memset(GetDataInfo.Reserved2,'\0',200);
		strcpy(GetDataInfo.Reserved2,lpszNewValue);
		SelObjList[0].SelDataInfo=GetDataInfo;
		DataArray[GetIndex]=GetDataInfo;
	}
	SetModifiedFlag();
}

BSTR CSCTDrawActivexCtrl::GetReservedProperty3() //?貌似在整个控件中都没有使用到此函数
{
	CString strResult;
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		strResult=GetDataInfo.Reserved3;
	}
	return strResult.AllocSysString();
}

void CSCTDrawActivexCtrl::SetReservedProperty3(LPCTSTR lpszNewValue) //?貌似在整个控件中都没有使用到此函数
{
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		int GetIndex=SelObjList[0].Index;
		memset(GetDataInfo.Reserved3,'\0',200);
		strcpy(GetDataInfo.Reserved3,lpszNewValue);
		SelObjList[0].SelDataInfo=GetDataInfo;
		DataArray[GetIndex]=GetDataInfo;
	}
	SetModifiedFlag();
}

BSTR CSCTDrawActivexCtrl::GetReservedProperty4() //?貌似在整个控件中都没有使用到此函数
{
	CString strResult;
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		strResult=GetDataInfo.Reserved4;
	}
	return strResult.AllocSysString();
}

void CSCTDrawActivexCtrl::SetReservedProperty4(LPCTSTR lpszNewValue) //?貌似在整个控件中都没有使用到此函数
{
	// TODO: Add your property handler here
	if(SelObjList.size()==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		int GetIndex=SelObjList[0].Index;
		memset(GetDataInfo.Reserved4,'\0',1024);
		strcpy(GetDataInfo.Reserved4,lpszNewValue);
		SelObjList[0].SelDataInfo=GetDataInfo;
		DataArray[GetIndex]=GetDataInfo;
	}
	SetModifiedFlag();
}

BOOL CSCTDrawActivexCtrl::DrawPolygon(VARIANT FAR* ptArray, short nSize) //画多边行
{
	// TODO: Add your dispatch handler code here
	ClearDataInfo();
	AddPolygon(CPoint(ptArray[0].wReserved1,ptArray[0].wReserved2));
	for(int i=1;i<nSize;i++)
	{
		temSavePoint addPoint;
		CPoint point;
		point.x=ptArray[i].wReserved1;
		point.y=ptArray[i].wReserved2;
		addPoint.point=point;
		ptsPoint.push_back(addPoint);
		if(point.x<pDataInfo.StartPoint.x)
			pDataInfo.StartPoint.x=point.x;
		if(point.y<pDataInfo.StartPoint.y)
			pDataInfo.StartPoint.y=point.y;
		if(point.x>pDataInfo.EndPoint.x)
			pDataInfo.EndPoint.x=point.x;
		if(point.y>pDataInfo.EndPoint.y)
			pDataInfo.EndPoint.y=point.y;
	}
	ptsPoint.push_back(ptsPoint[0]);
	FinishPolygon(CPoint(0,0));
	pbPolygonUnit->ReDraw(memDC,pDataInfo);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	PageChanged();//事件触发
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::DrawObj(short ObjType, short Left, short Top, short Right, short Bottom, float Angle) //对象的绘制
{
	// TODO: Add your dispatch handler code here
	Graphics graphics(memDC->m_hDC);
	DataInfo AddDataInfo;
	AddDataInfo.CenterPoint.x=(Left+Right)/2;
	AddDataInfo.CenterPoint.y=(Top+Bottom)/2;
	AddDataInfo.EndPoint.x=Right;
	AddDataInfo.EndPoint.y=Bottom;
	AddDataInfo.StartPoint.x=Left;
	AddDataInfo.StartPoint.y=Top;
	Rel_EndPoint=CaculatePoint(AddDataInfo.EndPoint,AddDataInfo.CenterPoint,0);
	Rel_StartPoint=CaculatePoint(AddDataInfo.StartPoint,AddDataInfo.CenterPoint,0);
	//////////////////////////////////////////////////////////////////////////////
	double len=sqrt(float(Rel_StartPoint.x*Rel_StartPoint.x)+float(Rel_StartPoint.y*Rel_StartPoint.y));
	float tAngle=atan2(float(Rel_StartPoint.y),float(Rel_StartPoint.x))+Angle/Rate;
	AddDataInfo.StartPoint.x=AddDataInfo.CenterPoint.x+len*cos(tAngle);
	AddDataInfo.StartPoint.y=AddDataInfo.CenterPoint.y+len*sin(tAngle);
	len=sqrt(float(Rel_EndPoint.x*Rel_EndPoint.x)+float(Rel_EndPoint.y*Rel_EndPoint.y));
	tAngle=atan2(float(Rel_EndPoint.y),float(Rel_EndPoint.x))+Angle/Rate;
	AddDataInfo.EndPoint.x=AddDataInfo.CenterPoint.x+len*cos(tAngle);
	AddDataInfo.EndPoint.y=AddDataInfo.CenterPoint.y+len*sin(tAngle);
	/////////////////////////////////////////////////////////////////////////////////
	AddDataInfo.FillColor=m_fillColor;
	AddDataInfo.FillStyle=m_fillStyle;
	AddDataInfo.foreColor=m_foreColor;
	Region Rgn(Rect(0,0,0,0));
	AddDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	AddDataInfo.IsCombination=false;
	AddDataInfo.IsDisjoint=false;
	AddDataInfo.IsErased=false;
	AddDataInfo.IsLocked=false;
	AddDataInfo.LineStyle=m_lineStyle;
	AddDataInfo.penColor=m_penColor;
	AddDataInfo.penWidth=m_penWidth;
	AddDataInfo.preAngle=0.0f;
	AddDataInfo.RotateAngle=Angle;
	//AddDataInfo.SelectColor=SelectColor++;
	AddDataInfo.startAngle=270.0f;
	AddDataInfo.sweepAngle=90.0f;
	AddDataInfo.xScale=1;
	AddDataInfo.yScale=1;
	ZeroMemory(AddDataInfo.Reserved0,200);
	ZeroMemory(AddDataInfo.Reserved1,200);
	ZeroMemory(AddDataInfo.Reserved2,200);
	ZeroMemory(AddDataInfo.Reserved3,200);
	ZeroMemory(AddDataInfo.Reserved4,1024);
	switch(ObjType)
	{
	case 1:
		AddDataInfo.OperationKind=0;
		AddDataInfo.FillStyle=0;
		pbUnit=pbLineUnit;
		break;
	case 2:
		AddDataInfo.OperationKind=1;
		pbUnit=pbRectUnit;
		break;
	case 3:
		AddDataInfo.OperationKind=2;
		pbUnit=pbRoundRectUnit;
		break;
	case 4:
		AddDataInfo.OperationKind=3;
		pbUnit=pbEllipseUnit;
		break;
	case 5:
		AddDataInfo.OperationKind=4;
		pbUnit=pbArcUnit;
		break;
	case 6:
		AddDataInfo.OperationKind=5;
		pbUnit=pbSectorUnit;
		break;
	case 18:
		AddDataInfo.OperationKind=11;
		pbUnit=pbPolygonUnit;
		break;
	default:
		pbUnit=pbBaseUnit;
		break;
	}
	AddDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(AddDataInfo);
	UndoArray.push_back(AddDataInfo);
		
	pbUnit->ReDraw(memDC,AddDataInfo);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	SelObj AddSelObj;
	AddSelObj.SelDataInfo=AddDataInfo;
	AddSelObj.Index=DataArray.size()-1;
	SelObjList.clear();
	SelObjList.push_back(AddSelObj);
	PageChanged();//事件触发
	return TRUE;
}



BOOL CSCTDrawActivexCtrl::SetSprayBottle(short FillStyle, OLE_COLOR FillColor, OLE_COLOR ForeColor)//对象填充时要用到 
{
	// TODO: Add your dispatch handler code here
	SB_FillStyle=FillStyle;
	SB_FillColor=FillColor;
	SB_ForeColor=ForeColor;
	return TRUE;
}

short CSCTDrawActivexCtrl::GetObjNum() //得到当前页对象的总数
{
	// TODO: Add your dispatch handler code here
	int Size=DataArray.size();
	return Size;
}

short CSCTDrawActivexCtrl::GetSelObjType() //得到所选对象的所属类型
{
	// TODO: Add your dispatch handler code here
	if(SelObjList.size()==1)
		return SelObjList[0].SelDataInfo.OperationKind;
	if(SelObjList.size()==0)
		return -1;   
	return -2;
}

BSTR CSCTDrawActivexCtrl::ReadReservedProperty(short PropertyIndex, short ObjIndex) //和SetReservedProperty成对出现
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int Size=DataArray.size();
	if(ObjIndex<0||ObjIndex>=Size)
		return false;
	switch(PropertyIndex)
	{
	case 0: 
		strResult=DataArray[ObjIndex].Reserved0;
		break;
	case 1:
		strResult=DataArray[ObjIndex].Reserved1;
		break;
	case 2:
		strResult=DataArray[ObjIndex].Reserved2;
		break;
	case 3:
		strResult=DataArray[ObjIndex].Reserved3;
		break;
	case 4:
		strResult=DataArray[ObjIndex].Reserved4;
		break;
	default:
		strResult="Error";
	}	   
	return strResult.AllocSysString();
}


BOOL CSCTDrawActivexCtrl::SetReservedProperty(short PropertyIndex, short ObjIndex, LPCTSTR Property) //图形绘制时设置属性
{
	// TODO: Add your dispatch handler code here
	/* PropertyIndex的值如下：
		0:保存链接文件路径
        1:记录当前图形是否为立体图
        2:记录函数绘图的参数,本来是想用于函数绘图后,对图形参数的修改的,这个一直没有做好
		3:用于保存图形分割后图形角度
	*/

	CString str=Property;
	if(str=="")
		Property="";
	int Size=DataArray.size();
	if(ObjIndex<0||ObjIndex>=Size)
		return false;
	switch(PropertyIndex)
	{
	case 0: 
		memset(DataArray[ObjIndex].Reserved0,'\0',200);
		strcpy(DataArray[ObjIndex].Reserved0,Property);
		break;
	case 1:
		memset(DataArray[ObjIndex].Reserved1,'\0',200);
		strcpy(DataArray[ObjIndex].Reserved1,Property);
		break;
	case 2:
		memset(DataArray[ObjIndex].Reserved2,'\0',200);
		strcpy(DataArray[ObjIndex].Reserved2,Property);
		break;
	case 3:
		memset(DataArray[ObjIndex].Reserved3,'\0',200);
		strcpy(DataArray[ObjIndex].Reserved3,Property);
		break;
	case 4:
		memset(DataArray[ObjIndex].Reserved4,'\0',1024);
		strcpy(DataArray[ObjIndex].Reserved4,Property);
		break;
	}
	if(SelObjList.size()>0&&SelObjList[0].Index==ObjIndex)
		SelObjList[0].SelDataInfo=DataArray[ObjIndex];
	return TRUE;
}

short CSCTDrawActivexCtrl::GetSelObjIndex() //得到多选对象在整个页面的序号,将在绘制立体图形以及设置对象做对象链接时用到
{
	// TODO: Add your dispatch handler code here
	if(SelObjList.size()==0)
		return -1;
	return SelObjList[0].Index;
}

void CSCTDrawActivexCtrl::AddSelObj(CPoint point)//选择多个对象时，可以在类似用（shift）组合的方式任意添加多个对象进入选择组合中
{
	int Index;
	pbUnit=pbBaseUnit;;
	Matrix matrix;
	HRGN hRgn;
	Graphics graphics(hDC);
	ClearDataInfo();
	int Size=DataArray.size();
	for(Index=Size-1;Index>=0;Index--)
	{
		if(DataArray[Index].IsCombination||DataArray[Index].IsDisjoint||DataArray[Index].IsErased)
			continue;
		switch(DataArray[Index].OperationKind)
		{
		case 0:
			pbUnit=pbLineUnit;
			break;
		case 1:
			pbUnit=pbRectUnit;
			break;
		case 2:
			pbUnit=pbRoundRectUnit;
			break;
		case 3:
			pbUnit=pbEllipseUnit;
			break;
		case 4:
			pbUnit=pbArcUnit;
			break;
		case 5:
			pbUnit=pbSectorUnit;
			break;
		case 6:
			pbUnit=pbPenBrushUnit;
			break;
		case 7:
			pbUnit=pbPolylineUnit;
			break;
		case 8:
			pbUnit=pbTextUnit;
			break;
		case 9:
			pbUnit=pbBmpUnit;
			break;
		case 10:
			pbUnit=pbCombination;
			break;
		case 11:
			pbUnit=pbPolygonUnit;
			break;
		}
		/////////////////////////////////////////////////
		hRgn=pbUnit->GetRedrawHRGN(hDC,DataArray,DataArray[Index]);
		Region Rgn(hRgn);	 
		matrix.Translate(DataArray[Index].CenterPoint.x,DataArray[Index].CenterPoint.y);
		matrix.Rotate(DataArray[Index].RotateAngle);
		matrix.Scale(DataArray[Index].xScale,DataArray[Index].yScale);
		Rgn.Transform(&matrix);
		matrix.Reset();
		if(Rgn.IsVisible(PointF(point.x,point.y),&graphics))
		{
			pDataInfo=DataArray[Index];
			SelObj AddSelObj;
			AddSelObj.Index=Index;
			AddSelObj.SelDataInfo=DataArray[Index];
			SelObjList.push_back(AddSelObj);
			break;
		}
		DeleteObject(hRgn);
	}	
	if(Index<0)
	{
		pbUnit=pbBaseUnit;
		MouseCur=NULL;
	}
	DeleteObject(hRgn);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
}

BOOL CSCTDrawActivexCtrl::SelectObjs(short FAR* Objs, short ObjsNum) //做切割时用到此函数
{
	// TODO: Add your dispatch handler code here
	SelObjList.clear();
	if(ObjsNum<1)
		return false;
	int Num=0;
	int ObjIndex;
	int Size=DataArray.size();
	for(int Index=0;Index<ObjsNum;Index++)
	{
		ObjIndex=Objs[Index];
		if(ObjIndex<0||ObjIndex>=Size)
			continue;
		SelObj AddSelObj;
		AddSelObj.Index=ObjIndex;
		AddSelObj.SelDataInfo=DataArray[ObjIndex];
		SelObjList.push_back(AddSelObj);	
		Num++;
	}
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);

	if(Num==0)
		return false;
	if(Num==1)
		ObjSelected();
	else
	{
		CaculateSelObjsRgn();
		ObjsSelected();
	}
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::AddSelectObjs(short FAR* Objs, short ObjsNum) //?
{
	// TODO: Add your dispatch handler code here
	if(ObjsNum<1)
		return false;
	int ObjIndex;
	int Size=DataArray.size();
	for(int Index=0;Index<ObjsNum;Index++)
	{
		ObjIndex=Objs[Index];
		if(ObjIndex<0||ObjIndex>=Size)
			continue;
		SelObj AddSelObj;
		AddSelObj.Index=ObjIndex;
		AddSelObj.SelDataInfo=DataArray[ObjIndex];
		SelObjList.push_back(AddSelObj);	
	}
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	int Num=SelObjList.size();
	if(Num==0)
		return false;
	if(Num==1)
		ObjSelected();
	else
	{
		CaculateSelObjsRgn();
		ObjsSelected();
	}
	return TRUE;
}


BOOL CSCTDrawActivexCtrl::UnSelectObjs(short FAR* Objs, short ObjsNum) //?
{
	// TODO: Add your dispatch handler code here
	if(ObjsNum<1)
		SelObjList.clear();
	int ObjIndex;
	int Size=DataArray.size();
	std::vector<SelObj>::iterator ite=SelObjList.begin();
	for(int Index=0;Index<ObjsNum;Index++)
	{
		ObjIndex=Objs[Index];
		if(ObjIndex<0||ObjIndex>=Size)
			continue;
		for(int i=0;i<SelObjList.size();i++)
		{
			if(ObjIndex==SelObjList[i].Index)
				SelObjList.erase(ite+i);
		}
	}
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	int Num=SelObjList.size();
	if(Num==0)
		SelObjList.clear();
	else
	{
		if(Num==1)
			ObjSelected();
		else
		{
			CaculateSelObjsRgn();
			ObjsSelected();
		}
	}
	return TRUE;
}

void CSCTDrawActivexCtrl::CaculateSelObjsRgn()//?是不是计算选择到了的对象的区域
{
	int Size=SelObjList.size();
	DataInfo tDataInfo;
	CPoint sPoint,ePoint,pt1,pt2;
	int mid;
	CPoint bPoint(WindowRect.right,WindowRect.bottom);
	CPoint tPoint(WindowRect.left,WindowRect.top);
	for(int Index=0;Index<Size;Index++)
	{
		tDataInfo=SelObjList[Index].SelDataInfo;
		//////////////////////////////////////////////////
		sPoint=CaculatePoint(tDataInfo.StartPoint,tDataInfo.CenterPoint,tDataInfo.RotateAngle);
		ePoint=CaculatePoint(tDataInfo.EndPoint,tDataInfo.CenterPoint,tDataInfo.RotateAngle);
		sPoint.x*=tDataInfo.xScale;
		sPoint.y*=tDataInfo.yScale;
		ePoint.x*=tDataInfo.xScale;
		ePoint.y*=tDataInfo.yScale;
		pt1.x=sPoint.x;
		pt1.y=ePoint.y;
		pt2.x=ePoint.x;
		pt2.y=sPoint.y;
		////////////////////////////////////////////////////
		double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
		double Angle=atan2(float(sPoint.y),float(sPoint.x))+tDataInfo.RotateAngle/Rate;
		sPoint.x=tDataInfo.CenterPoint.x+len*cos(Angle);
		sPoint.y=tDataInfo.CenterPoint.y+len*sin(Angle);
		len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
		Angle=atan2(float(ePoint.y),float(ePoint.x))+tDataInfo.RotateAngle/Rate;
		ePoint.x=tDataInfo.CenterPoint.x+len*cos(Angle);
		ePoint.y=tDataInfo.CenterPoint.y+len*sin(Angle);
		len=sqrt(float(pt1.x*pt1.x)+float(pt1.y*pt1.y));
		Angle=atan2(float(pt1.y),float(pt1.x))+tDataInfo.RotateAngle/Rate;
		pt1.x=tDataInfo.CenterPoint.x+len*cos(Angle);
		pt1.y=tDataInfo.CenterPoint.y+len*sin(Angle);
		len=sqrt(float(pt2.x*pt2.x)+float(pt2.y*pt2.y));
		Angle=atan2(float(pt2.y),float(pt2.x))+tDataInfo.RotateAngle/Rate;
		pt2.x=tDataInfo.CenterPoint.x+len*cos(Angle);
		pt2.y=tDataInfo.CenterPoint.y+len*sin(Angle);
		///////////////////////////////////////////////////
		mid=(pt1.x<pt2.x)?pt1.x:pt2.x;//最小x
		bPoint.x=(bPoint.x<mid)?bPoint.x:mid;
		mid=(sPoint.x<ePoint.x)?sPoint.x:ePoint.x;
		bPoint.x=(bPoint.x<mid)?bPoint.x:mid;
		mid=(pt1.y<pt2.y)?pt1.y:pt2.y;//最小y
		bPoint.y=(bPoint.y<mid)?bPoint.y:mid;
		mid=(sPoint.y<ePoint.y)?sPoint.y:ePoint.y;
		bPoint.y=(bPoint.y<mid)?bPoint.y:mid;

		mid=(pt1.x>pt2.x)?pt1.x:pt2.x;//最大x
		tPoint.x=(tPoint.x>mid)?tPoint.x:mid;
		mid=(sPoint.x>ePoint.x)?sPoint.x:ePoint.x;
		tPoint.x=(tPoint.x>mid)?tPoint.x:mid;

		mid=(pt1.y>pt2.y)?pt1.y:pt2.y;//最大y
		tPoint.y=(tPoint.y>mid)?tPoint.y:mid;
		mid=(sPoint.y>ePoint.y)?sPoint.y:ePoint.y;
		tPoint.y=(tPoint.y>mid)?tPoint.y:mid;
	}
	ComObjRect.SetRect(bPoint,tPoint);
	ComRect.SetRect(bPoint,tPoint);
}

VARIANT_BOOL CSCTDrawActivexCtrl::StartPlay(SHORT PageIndex)//播放当前页
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	int Size=PageArray.size();
	if(PageIndex>=Size||PageIndex<0)
		return VARIANT_FALSE;
	if(PageIndex==PageNum)
		SavePage();
	PlayPageIndex=PageIndex;

	memDC->FillSolidRect(&WindowRect,PageArray[PlayPageIndex].bkColor);

	Graphics mGraphics(memDC->m_hDC);
	mGraphics.SetInterpolationMode(InterpolationModeHighQuality);
	Size=PageArray[PlayPageIndex].bkImageArray.size();
	float Width,Height;
	for(int Index=0;Index<Size;Index++)
	{
		Width=PageArray[PlayPageIndex].bkImageArray[Index].Width;
		Height=PageArray[PlayPageIndex].bkImageArray[Index].Height;
		mGraphics.TranslateTransform(PageArray[PlayPageIndex].bkImageArray[Index].ptImage.x,PageArray[PlayPageIndex].bkImageArray[Index].ptImage.y);  
		mGraphics.ScaleTransform(PageArray[PlayPageIndex].bkImageArray[Index].xScale,PageArray[PlayPageIndex].bkImageArray[Index].yScale);
		////////////////////////////////////////////////////////////////////////////////////////////
		IStream *pIStream;
		CreateStreamOnHGlobal(PageArray[PlayPageIndex].bkImageArray[Index].ImageByte,true,&pIStream);
		Image image(pIStream);
		mGraphics.DrawImage(&image,-Width/2,-Height/2,Width,Height);
		////////////////////////////////////////////////////////////////////////////////////////////////	
		mGraphics.ResetTransform();
	}
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	return VARIANT_TRUE;
}

BOOL CSCTDrawActivexCtrl::ReplayPage(short ObjIndex) //具体对象的回放
{
	// TODO: Add your dispatch handler code here
	int Size=PageArray[PlayPageIndex].DataArray.size();
	if(ObjIndex>=Size||ObjIndex<0)
		return false;
	if(strcmp(PageArray[PlayPageIndex].DataArray[ObjIndex].Reserved1,"L3D")==0)
	{
		return false;
	}
	switch (PageArray[PlayPageIndex].DataArray[ObjIndex].OperationKind)
	{
	case 0:
		pbUnit=pbLineUnit;
		break;
	case 1:
		pbUnit=pbRectUnit;
		break;
	case 2:
		pbUnit=pbRoundRectUnit;
		break;
	case 3:
		pbUnit=pbEllipseUnit;
		break;
	case 4:
		pbUnit=pbArcUnit;
		break;
	case 5:
		pbUnit=pbSectorUnit;
		break;
	case 6:
		pbUnit=pbPenBrushUnit;
		break;
	case 7:
		pbUnit=pbPolylineUnit;
		break;
	case 8:
		pbUnit=pbTextUnit;
		break;
	case 9:
		pbUnit=pbBmpUnit;
		break;
	case 10:
		pbUnit=pbCombination;
		break;
	case 11:
		pbUnit=pbPolygonUnit;
		break;
	}	
	if(strcmp(PageArray[PlayPageIndex].DataArray[ObjIndex].Reserved1,"3D")==0)
	{
		std::map<int,int> ObjList;
		pbCombination->GetCombinatedObjs(PageArray[PlayPageIndex].DataArray,PageArray[PlayPageIndex].DataArray[ObjIndex],ObjList);
		pbCombination->ReDraw(memDC,PageArray[PlayPageIndex].DataArray,ObjList);
		ObjList.clear();
	}
	else
		pbUnit->ReDraw(memDC,PageArray[PlayPageIndex].DataArray[ObjIndex]);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	return TRUE;
}

VARIANT_BOOL CSCTDrawActivexCtrl::StopPlay(void)//停止播放页面
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	PageInfo GetPage=PageArray[PageNum];
	Graphics graphics(memDC->m_hDC);
	Bitmap bmp(GetPage.hMemBmp,NULL);
	graphics.DrawImage(&bmp,0,0,WindowRect.Width(),WindowRect.Height());
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	DoneReplayPage();
	return VARIANT_TRUE;
}

void CSCTDrawActivexCtrl::ObjZoomOperation(int Index,float mul)//页面放大时用到
{
	float Width,Height;
	CPoint sPoint=DataArray[Index].StartPoint;
	CPoint ePoint=DataArray[Index].EndPoint;
	CPoint cPoint=DataArray[Index].CenterPoint;	
	sPoint=CaculatePoint(sPoint,cPoint,DataArray[Index].RotateAngle);
	ePoint=CaculatePoint(ePoint,cPoint,DataArray[Index].RotateAngle);
	if(DataArray[Index].OperationKind==10)
	{
		Width=ePoint.x-sPoint.x;
		Height=ePoint.y-sPoint.y;
		Width*=mul;
		Height*=mul;
		sPoint.x=-Width/2;
		sPoint.y=-Height/2;
		ePoint.x=Width/2;
		ePoint.y=Height/2;
	}
	else
	{
		DataArray[Index].xScale*=mul;
		DataArray[Index].yScale*=mul;
	}
	cPoint.x=(cPoint.x-WindowRect.Width()/2)*mul+WindowRect.Width()/2;
	cPoint.y=(cPoint.y-WindowRect.Height()/2)*mul+WindowRect.Height()/2;
	////////////////////////////////////////////////////////////////
	float len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	float Angle=atan2(float(sPoint.y),float(sPoint.x))+DataArray[Index].RotateAngle/Rate;
	sPoint.x=cPoint.x+len*cos(Angle);
	sPoint.y=cPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+DataArray[Index].RotateAngle/Rate;
	ePoint.x=cPoint.x+len*cos(Angle);
	ePoint.y=cPoint.y+len*sin(Angle);
	/////////////////////////////////////////////////////////////////////
	DataArray[Index].StartPoint=sPoint;
	DataArray[Index].EndPoint=ePoint;
	DataArray[Index].CenterPoint=cPoint;
	if(DataArray[Index].OperationKind==10)
		pbCombination->ModifyCombinatedObjs(DataArray,DataArray[Index]);
}

void CSCTDrawActivexCtrl::ChangeLogFontToCharFormat(CHARFORMAT &cf, const LOGFONT lf, const COLORREF textcolor)
{
	cf.dwMask = CFM_BOLD|CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_ITALIC|\
		CFM_SIZE|CFM_UNDERLINE|CFM_STRIKEOUT; 

	BOOL bIsItalic = lf.lfItalic ;
	BOOL bIsBold = (lf.lfWeight > FW_NORMAL);//? FW_NORMAL:FW_BOLD;
	BOOL bIsUnderline = lf.lfUnderline ;
	BOOL bIsStrickout = lf.lfStrikeOut ;

	cf.dwEffects |= CFE_PROTECTED;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	if(bIsBold) 
	{ 
		cf.dwEffects |= CFE_BOLD; 
	} 
	else
	{
		cf.dwEffects &= ~CFE_BOLD; 
	}

	if(bIsItalic) 
	{ 
		cf.dwEffects |= CFE_ITALIC; 
	} 
	else
	{
		cf.dwEffects &= ~CFE_ITALIC; 
	}

	if(bIsUnderline) 
	{ 
		cf.dwEffects |= CFE_UNDERLINE; 
	} 
	else
	{
		cf.dwEffects &= ~CFE_UNDERLINE; 
	}

	if(bIsStrickout) 
	{ 
		cf.dwEffects |= CFM_STRIKEOUT; 
	} 
	else
	{
		cf.dwEffects &= ~CFM_STRIKEOUT; 
	}

	//设置属性
	cf.yHeight =  -15 * lf.lfHeight;
	cf.crTextColor =textcolor;
	cf.bCharSet = lf.lfCharSet ;
	cf.bPitchAndFamily = lf.lfPitchAndFamily ;
	sprintf(cf.szFaceName, lf.lfFaceName);
}

void CSCTDrawActivexCtrl::ChangeCharFormatToLogFont(CHARFORMAT cf, LOGFONT &lf, COLORREF &textcolor)
{
	lf.lfOutPrecision = 3;
	lf.lfClipPrecision =2;
	lf.lfQuality = 1;
	//粗体
	if((cf.dwEffects &CFE_BOLD )>0) 
	{
		lf.lfWeight = 700;
	}
	else
	{
		lf.lfWeight = 400;
	}

	//斜体 
	if((cf.dwEffects &CFE_ITALIC)>0)
	{
		lf.lfItalic = 1;
	}
	else
	{
		lf.lfItalic = 0;
	}

	//下划线
	if((cf.dwEffects & CFE_UNDERLINE)>0)
	{
		lf.lfUnderline = 1;
	}
	else
	{
		lf.lfUnderline = 0;
	}

	//删除线
	if((cf.dwEffects & CFE_STRIKEOUT)>0)
	{
		lf.lfStrikeOut = 1;
	}
	else
	{
		lf.lfStrikeOut =0;
	}

	lf.lfHeight = (-cf.yHeight/15);
	textcolor = cf.crTextColor;
	lf.lfCharSet = cf.bCharSet;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	sprintf(lf.lfFaceName, cf.szFaceName);
}


BOOL CSCTDrawActivexCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
	{
		if(IsWindow(pEdit->GetSafeHwnd()))
		{
			switch (pMsg->wParam)
			{
			case VK_UP:
				pEdit->SendMessage(WM_KEYDOWN,VK_UP,0);
				break;
			case VK_DOWN:
				pEdit->SendMessage(WM_KEYDOWN,VK_DOWN,0);
				break;
			case VK_LEFT:
				pEdit->SendMessage(WM_KEYDOWN,VK_LEFT,0);
				break;
			case VK_RIGHT:
				pEdit->SendMessage(WM_KEYDOWN,VK_RIGHT,0);
				break;
			case VK_RETURN:
				TextReturn=true;
				break;
			}
		}
	}
	return COleControl::PreTranslateMessage(pMsg);
}

void CSCTDrawActivexCtrl::ChangeText()//用于文字输入改变之后，生效前调用
{
	CString Text;
	GetDlgItemText(IDC_Text,Text);
	CPoint sPoint=CaculatePoint(pDataInfo.StartPoint,pDataInfo.CenterPoint,pDataInfo.RotateAngle);
	int Width=pDataInfo.EndPoint.x-pDataInfo.StartPoint.x;
	int Height=pDataInfo.EndPoint.y-pDataInfo.StartPoint.y;
	CPoint ePoint;
	ePoint.x=sPoint.x+Width;
	ePoint.y=sPoint.y+Height;
	//////////////////////////////////////////////////
	double len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	double Angle=atan2(float(ePoint.y),float(ePoint.x))+pDataInfo.RotateAngle/Rate;
	pDataInfo.EndPoint.x=pDataInfo.CenterPoint.x+len*cos(Angle);
	pDataInfo.EndPoint.y=pDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////////////////////
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	delete [] pDataInfo.SaveStr;
	pDataInfo.charNum=Text.GetLength();
	pDataInfo.SaveStr=new char[Text.GetLength()+1];
	strcpy(pDataInfo.SaveStr,Text);
	pDataInfo.SaveStr[Text.GetLength()]='\0';	
	int Index=SelObjList[0].Index;
	DataArray[Index]=pDataInfo;
	UndoArray.push_back(pDataInfo);
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=Index;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	pbTextUnit->ReDraw(memDC,pDataInfo);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::OnChangeText()
{
	CPoint sPoint,ePoint;
	sPoint=pDataInfo.StartPoint;
	Graphics graphics(hDC);
	wchar_t *WideStr;
	CString temStr;
	GetDlgItemText(IDC_Text,temStr);
	int len=temStr.GetLength();
	WideStr=new wchar_t[len];
	WideStr=temStr.AllocSysString();
	PointF pointf(sPoint.x,sPoint.y);
	RectF rectf;
	const StringFormat *format=StringFormat::GenericTypographic();
	Font temFont(hDC,&pDataInfo.logfontw);
	graphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
	SysFreeString(WideStr);
	CRect rect(rectf.X,rectf.Y,rectf.X+rectf.Width,rectf.Y+rectf.Height);
	pDataInfo.EndPoint.x=rectf.X+rectf.Width;
	pDataInfo.EndPoint.y=rectf.Y+rectf.Height;
	int line=pEdit->GetLineCount();
	if(TextReturn)
	{
		pEdit->MoveWindow(pDataInfo.StartPoint.x,pDataInfo.StartPoint.y,rectf.Width+100,rectf.Height+2*rectf.Height/line);
		TextReturn=false;
	}
	else if(line>0)
		pEdit->MoveWindow(pDataInfo.StartPoint.x,pDataInfo.StartPoint.y,rectf.Width+100,rectf.Height+rectf.Height/line);
}

BOOL CSCTDrawActivexCtrl::ModifySelObjFont(OLE_HANDLE hFont) 
{
	// TODO: Add your dispatch handler code here
	Font tfont(memDC->m_hDC,(HFONT)hFont);
	Graphics mGraphics(memDC->m_hDC);
	int Size=SelObjList.size();
	for(int i=0;i<Size;i++)
	{
		DataInfo GetDataInfo=SelObjList[i].SelDataInfo;
		int Index=SelObjList[i].Index;
		tfont.GetLogFontW(&mGraphics,&GetDataInfo.logfontw);

		//DataArray[Index]=GetDataInfo;
		if(DataArray[Index].ComArray.size())
			InModifySelObjFont(Index, &tfont);

		///////////////////////////////////////////////////////////////////////////////////////
		CPoint tRel_StartPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
		CPoint tRel_EndPoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
		Font tfont(hDC,&GetDataInfo.logfontw);
		CString Str=(CString)GetDataInfo.SaveStr;
		wchar_t *PosStr;
		PosStr=new wchar_t[Str.GetLength()];
		PosStr=Str.AllocSysString();
		const StringFormat *format=StringFormat::GenericTypographic();
		RectF rectf;
		Graphics mGraphics(memDC->m_hDC);
		PointF pointf(tRel_StartPoint.x,tRel_StartPoint.y);
		mGraphics.MeasureString(PosStr,wcslen(PosStr),&tfont,pointf,format,&rectf);
		SizeF size;
		rectf.GetSize(&size);
		SysFreeString(PosStr);
		tRel_EndPoint.x=tRel_StartPoint.x+size.Width;
		tRel_EndPoint.y=tRel_StartPoint.y+size.Height;
		/////////////////////////////////////////////////
		double len=sqrt(float(tRel_StartPoint.x*tRel_StartPoint.x)+float(tRel_StartPoint.y*tRel_StartPoint.y));
		double Angle=atan2(float(tRel_StartPoint.y),float(tRel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
		GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
		len=sqrt(float(tRel_EndPoint.x*tRel_EndPoint.x)+float(tRel_EndPoint.y*tRel_EndPoint.y));
		Angle=atan2(float(tRel_EndPoint.y),float(tRel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
		GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
		GetDataInfo.CenterPoint.x=(GetDataInfo.StartPoint.x+GetDataInfo.EndPoint.x)/2;
		GetDataInfo.CenterPoint.y=(GetDataInfo.StartPoint.y+GetDataInfo.EndPoint.y)/2;
		////////////////////////////////////////////////////////////////////////////////////////
		DataArray[Index]=GetDataInfo;
		SelObjList[i].SelDataInfo=GetDataInfo;
	}
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();	
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	if(SelObjList.size()==1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjSelected();
	if(SelObjList.size()>1&&m_operations==11&&!IsWindow(pEdit->GetSafeHwnd()))
		ObjsSelected();
	return TRUE;
}

BOOL CSCTDrawActivexCtrl::InModifySelObjFont(int Index, Font *pfont)
{
	Graphics graphics(memDC->m_hDC);
	int Size=DataArray[Index].ComArray.size();
	int GetIndex;
	for(int i=0;i<Size;i++)
	{
		GetIndex=DataArray[Index].ComArray[i].ObjIndex;
		pfont->GetLogFontW(&graphics,&DataArray[GetIndex].logfontw);
		if(DataArray[GetIndex].ComArray.size())
			InModifySelObjFont(GetIndex,pfont);


		///////////////////////////////////////////////////////////////////////////////////////
		DataInfo GetDataInfo=DataArray[GetIndex];
		CPoint tRel_StartPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
		CPoint tRel_EndPoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
		/////////////////////////////////////////////
		Font tfont(hDC,&GetDataInfo.logfontw);
		CString Str=(CString)GetDataInfo.SaveStr;
		wchar_t *PosStr;
		PosStr=new wchar_t[Str.GetLength()];
		PosStr=Str.AllocSysString();
		const StringFormat *format=StringFormat::GenericTypographic();
		RectF rectf;
		Graphics mGraphics(memDC->m_hDC);
		PointF pointf(tRel_StartPoint.x,tRel_StartPoint.y);
		mGraphics.MeasureString(PosStr,wcslen(PosStr),&tfont,pointf,format,&rectf);
		SizeF size;
		rectf.GetSize(&size);
		SysFreeString(PosStr);
		tRel_EndPoint.x=tRel_StartPoint.x+size.Width;
		tRel_EndPoint.y=tRel_StartPoint.y+size.Height;
		/////////////////////////////////////////////////
		double len=sqrt(float(tRel_StartPoint.x*tRel_StartPoint.x)+float(tRel_StartPoint.y*tRel_StartPoint.y));
		double Angle=atan2(float(tRel_StartPoint.y),float(tRel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
		GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
		len=sqrt(float(tRel_EndPoint.x*tRel_EndPoint.x)+float(tRel_EndPoint.y*tRel_EndPoint.y));
		Angle=atan2(float(tRel_EndPoint.y),float(tRel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
		GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
		GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
		GetDataInfo.CenterPoint.x=(GetDataInfo.StartPoint.x+GetDataInfo.EndPoint.x)/2;
		GetDataInfo.CenterPoint.y=(GetDataInfo.StartPoint.y+GetDataInfo.EndPoint.y)/2;
		////////////////////////////////////////////////////////////////////////////////////////
		DataArray[GetIndex]=GetDataInfo;
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::InitializeFont(OLE_HANDLE hFont) //初始化字体
{
	// TODO: Add your dispatch handler code here
	CFont *pfont=CFont::FromHandle((HFONT)hFont);
	pfont->GetLogFont(&DefaultFont);
	return TRUE;
}



OLE_HANDLE CSCTDrawActivexCtrl::GetSelObjFont() //得到所选对象的字体
{
	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size!=1)
		return -1;
	Graphics graphics(memDC->m_hDC);
	LOGFONTA logfonta;
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	Font font(memDC->m_hDC,&GetDataInfo.logfontw);
	font.GetLogFontA(&graphics,&logfonta);
	return (long)CreateFontIndirect(&logfonta);
}

BOOL CSCTDrawActivexCtrl::AddRgnObj(OLE_HANDLE hRgn) //?
{
	// TODO: Add your dispatch handler code here
	Graphics graphics(memDC->m_hDC);
	DataInfo AddDataInfo;
	AddDataInfo.OperationKind=12;
	AddDataInfo.FillColor=m_fillColor;
	AddDataInfo.FillStyle=m_fillStyle;
	AddDataInfo.foreColor=m_foreColor;
	Region eRgn(Rect(0,0,0,0));//擦除区域
	AddDataInfo.hRgn=eRgn.GetHRGN(&graphics);
	AddDataInfo.IsCombination=false;
	AddDataInfo.IsDisjoint=false;
	AddDataInfo.IsErased=false;
	AddDataInfo.IsLocked=false;
	AddDataInfo.LineStyle=m_lineStyle;
	AddDataInfo.penColor=m_penColor;
	AddDataInfo.penWidth=m_penWidth;
	AddDataInfo.preAngle=0.0f;
	AddDataInfo.RotateAngle=0.0f;
	//AddDataInfo.SelectColor=SelectColor++;
	AddDataInfo.startAngle=270.0f;
	AddDataInfo.sweepAngle=90.0f;
	AddDataInfo.xScale=1;
	AddDataInfo.yScale=1;
	ZeroMemory(AddDataInfo.Reserved0,200);
	ZeroMemory(AddDataInfo.Reserved1,200);
	ZeroMemory(AddDataInfo.Reserved2,200);
	ZeroMemory(AddDataInfo.Reserved3,200);
	ZeroMemory(AddDataInfo.Reserved4,1024);
	///////////////////////////////////////////////////////////////	
	CRgn *pRgn=CRgn::FromHandle((HRGN)hRgn);//区域对象的区域数据
	CRect RgnRect;
	pRgn->GetRgnBox(&RgnRect);
	AddDataInfo.StartPoint=RgnRect.TopLeft();
	AddDataInfo.EndPoint=RgnRect.BottomRight();
	AddDataInfo.CenterPoint=RgnRect.CenterPoint();
	//////////////////////////////////////////////////////
	Region Rgn((HRGN)hRgn);
	Matrix matrix;
	matrix.Translate(-AddDataInfo.CenterPoint.x,-AddDataInfo.CenterPoint.y);
	Rgn.Transform(&matrix);
	Rect* rects = NULL;
	graphics.GetTransform(&matrix);
	int count = Rgn.GetRegionScansCount(&matrix);
	rects = (Rect*)malloc(count*sizeof(Rect));
	Rgn.GetRegionScans(&matrix, rects, &count); 
	GraphicsPath path1;
	int j;
	for(j = 0; j < count; ++j)
		path1.AddRectangle(rects[j]);
	path1.Outline();	
	AddDataInfo.pFillPath=path1.Clone();
	Pen pen1(Color(0,0,0,0),2);
	path1.Widen(&pen1);

	delete [] rects;
	Region Rgn1(&path1);
	Rgn1.Exclude(&Rgn);
	graphics.GetTransform(&matrix);
	count = Rgn1.GetRegionScansCount(&matrix);
	rects = (Rect*)malloc(count*sizeof(Rect));
	Rgn1.GetRegionScans(&matrix, rects, &count); 

	GraphicsPath path2;
	for(j = 0; j < count; ++j)
		path2.AddRectangle(rects[j]);
	path2.Outline();
	AddDataInfo.pOutlinePath=path2.Clone();
	delete [] rects;
	//////////////////////////////////////////////////////////////////
	AddDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(AddDataInfo);
	UndoArray.push_back(AddDataInfo);
		
	pbRgnObjUnit->ReDraw(memDC,AddDataInfo);
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	SelObj AddSelObj;
	AddSelObj.SelDataInfo=AddDataInfo;
	AddSelObj.Index=DataArray.size()-1;
	SelObjList.clear();
	SelObjList.push_back(AddSelObj);
	PageChanged();//事件触发
	return TRUE;
}

OLE_HANDLE CSCTDrawActivexCtrl::GetSelObjRegion(short SelObjIndex) //?
{
	// TODO: Add your dispatch handler code here
	if(SelObjIndex>=0&&SelObjIndex>=DataArray.size())
		return 0;
	Graphics graphics(hDC);
	DataInfo GetDataInfo=DataArray[SelObjIndex];
	GraphicsPath *pPath=NULL;
	switch(GetDataInfo.OperationKind)
	{
	case 1:
		pPath=pbRectUnit->GetObjPath(memDC,GetDataInfo);
		break;
	case 2:
		pPath=pbRoundRectUnit->GetObjPath(memDC,GetDataInfo);
		break;
	case 3:
		pPath=pbEllipseUnit->GetObjPath(memDC,GetDataInfo);
		break;
	case 5:
		pPath=pbSectorUnit->GetObjPath(memDC,GetDataInfo);
		break;
	case 11:
		pPath=pbPolygonUnit->GetObjPath(memDC,GetDataInfo);
		break;
	case 12:
		pPath=pbRgnObjUnit->GetObjPath(memDC,GetDataInfo);
		break;
	}
	if(pPath==NULL)
		return 0;
	Matrix matrix;
	matrix.Translate(GetDataInfo.CenterPoint.x,GetDataInfo.CenterPoint.y);
	matrix.Rotate(GetDataInfo.RotateAngle);
	matrix.Scale(GetDataInfo.xScale,GetDataInfo.yScale);
	pPath->Transform(&matrix);
	matrix.Reset();
	Region Rgn(pPath);	
	return (long)Rgn.GetHRGN(&graphics);
}

BOOL CSCTDrawActivexCtrl::GetSelObjIsErased(short ObjIndex) //?
{
	// TODO: Add your dispatch handler code here
	int Size=DataArray.size();
	if(ObjIndex>=0&&ObjIndex<Size)
		return DataArray[ObjIndex].IsErased;
	else
		return false;
}

BOOL CSCTDrawActivexCtrl::GetObjRect(short FAR* left, short FAR* top, short FAR* right, short FAR* bottom, short ObjIndex) //?
{
	// TODO: Add your dispatch handler code here
	int Size=DataArray.size();
	if(ObjIndex<0||ObjIndex>=Size)
		return false;
	DataInfo GetDataInfo=DataArray[ObjIndex];
	CPoint sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	CPoint ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	int tem;
	if(GetDataInfo.xScale<0)
	{
		tem=sPoint.x;
		sPoint.x=ePoint.x;
		ePoint.x=tem;
	}
	if(GetDataInfo.yScale<0)
	{
		tem=sPoint.y;
		sPoint.y=ePoint.y;
		ePoint.y=sPoint.y;
	}
	///////////////////////////////////////////////////////////////
	double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	double Angle=atan2(float(sPoint.y),float(sPoint.x))+GetDataInfo.RotateAngle/Rate;
	*left=GetDataInfo.CenterPoint.x+len*cos(Angle);
	*top=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+GetDataInfo.RotateAngle/Rate;
	*right=GetDataInfo.CenterPoint.x+len*cos(Angle);
	*bottom=GetDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////////////////
	return TRUE;
}

VARIANT_BOOL CSCTDrawActivexCtrl::InsertPage(SHORT PageIndex)//插入页操作
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if(PageIndex<0||PageIndex>=PageArray.size())
		return VARIANT_FALSE;

	SavePage();
	SelectColor=0;
	lbdown=false;
	preOperations=0;
	pbUnit=NULL;
	ModifyText=false;
	IsInitial=true;
	memset(bkBmp,'\0',100);
	memDC->FillSolidRect(&WindowRect,m_bkColor);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	ClearData();


	PageInfo AddPage;
	AddPage.bkColor=m_bkColor;
	int Size=DataArray.size();
	for(int i=0;i<Size;i++)
		AddPage.DataArray.push_back(DataArray[i]);
	Size=bkImageArray.size();
	for(int k=0;k<Size;k++)
		AddPage.bkImageArray.push_back(bkImageArray[k]);
	////////////////////////////////////////
	AddPage.FillStyle=m_fillStyle;
	AddPage.FillColor=m_fillColor;
	AddPage.ForeColor=m_foreColor;
	AddPage.bkColor=m_bkColor; 
	AddPage.Operations=m_operations;
	AddPage.PenWidth=m_penWidth;
	AddPage.PenColor=m_penColor;
	AddPage.FontColor=m_fontColor;
	AddPage.bkColor=m_bkColor;
	AddPage.LineStyle=m_lineStyle;
	std::vector<PageInfo>::iterator iter;
	iter=PageArray.begin();
	PageArray.insert(iter+PageIndex,AddPage);
	PageNum=PageIndex;

	return VARIANT_TRUE;
}

VARIANT_BOOL CSCTDrawActivexCtrl::ChangePageIndex(SHORT FromPageIndex, SHORT ToPageIndex)//改变页的页面
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if(FromPageIndex<0||FromPageIndex>=PageArray.size()||ToPageIndex<0||ToPageIndex>PageArray.size())
		return VARIANT_FALSE;
	if(FromPageIndex==PageNum)
		SavePage();
	std::vector<PageInfo>::iterator iter;
	if(ToPageIndex==PageArray.size())
	{
		PageArray.push_back(PageArray[FromPageIndex]); 
	}
	else
	{
		iter=PageArray.begin();
		PageArray.insert(iter+ToPageIndex,PageArray[FromPageIndex]); 
	}
	iter=PageArray.begin();
	if(FromPageIndex>ToPageIndex)
	{
		PageArray.erase(iter+FromPageIndex+1); 
		PageNum=ToPageIndex;
	}
	else
	{
		PageArray.erase(iter+FromPageIndex); 
		PageNum=ToPageIndex-1;
	}
	return VARIANT_TRUE;
}

void CSCTDrawActivexCtrl::RefreshAll(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	///////////////////////////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	//////////////////////////////////////////////////
}

OLE_HANDLE CSCTDrawActivexCtrl::SaveObjToImage() //将对象保存为图片，什么时候用到呢？
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if(SelObjList.size()!=1)
		return NULL;
	temDC->FillSolidRect(&WindowRect,RGB(255,255,255));
	switch (SelObjList[0].SelDataInfo.OperationKind)
	{
	case 0:
		pbLineUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 1:
		pbRectUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 2:
		pbRoundRectUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 3:
		pbEllipseUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 4:
		pbArcUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 5:
		pbSectorUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 6:
		pbPenBrushUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);;
		break;
	case 7:
		pbPolylineUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 8:
		pbTextUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 9:
		pbBmpUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 10:
		if(!SelObjList[0].SelDataInfo.IsCombination&&!SelObjList[0].SelDataInfo.IsDisjoint&&!SelObjList[0].SelDataInfo.IsErased)
		{
			std::map<int,int> ObjList;
			pbCombination->GetCombinatedObjs(DataArray,SelObjList[0].SelDataInfo,ObjList);
			pbCombination->ReDraw(temDC,DataArray,ObjList);
			ObjList.clear();
		}
		break;
	case 11:
		pbPolygonUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	case 12:
		pbRgnObjUnit->ReDraw(temDC,SelObjList[0].SelDataInfo);
		break;
	}
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	/////////////////////////////////////////////////////////////
	CRect rect;
	CPoint sPoint,ePoint,pt1,pt2;
	int mid;
	//////////////////////////////////////////////////
	sPoint=CaculatePoint(GetDataInfo.StartPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	ePoint=CaculatePoint(GetDataInfo.EndPoint,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	sPoint.x*=GetDataInfo.xScale;
	sPoint.y*=GetDataInfo.yScale;
	ePoint.x*=GetDataInfo.xScale;
	ePoint.y*=GetDataInfo.yScale;
	pt1.x=sPoint.x;
	pt1.y=ePoint.y;
	pt2.x=ePoint.x;
	pt2.y=sPoint.y;
	////////////////////////////////////////////////////
	double len=sqrt(float(sPoint.x*sPoint.x)+float(sPoint.y*sPoint.y));
	double Angle=atan2(float(sPoint.y),float(sPoint.x))+GetDataInfo.RotateAngle/Rate;
	sPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	sPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(ePoint.x*ePoint.x)+float(ePoint.y*ePoint.y));
	Angle=atan2(float(ePoint.y),float(ePoint.x))+GetDataInfo.RotateAngle/Rate;
	ePoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	ePoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt1.x*pt1.x)+float(pt1.y*pt1.y));
	Angle=atan2(float(pt1.y),float(pt1.x))+GetDataInfo.RotateAngle/Rate;
	pt1.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt1.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(pt2.x*pt2.x)+float(pt2.y*pt2.y));
	Angle=atan2(float(pt2.y),float(pt2.x))+GetDataInfo.RotateAngle/Rate;
	pt2.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	pt2.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	///////////////////////////////////////////////////
	mid=(pt1.x<pt2.x)?pt1.x:pt2.x;//最小x
	mid=(sPoint.x<mid)?sPoint.x:mid;
	mid=(mid<ePoint.x)?mid:ePoint.x;
	rect.left=mid-20;

	mid=(pt1.y<pt2.y)?pt1.y:pt2.y;//最小y
	mid=(sPoint.y<mid)?sPoint.y:mid;
	mid=(mid<ePoint.y)?mid:ePoint.y;
	rect.top=mid-20;

	mid=(pt1.x>pt2.x)?pt1.x:pt2.x;//最大x
	mid=(sPoint.x>mid)?sPoint.x:mid;
	mid=(mid>ePoint.x)?mid:ePoint.x;
	rect.right=mid+20;

	mid=(pt1.y>pt2.y)?pt1.y:pt2.y;//最大y
	mid=(sPoint.y>mid)?sPoint.y:mid;
	mid=(mid>ePoint.y)?mid:ePoint.y;
	rect.bottom=mid+20;
	///////////////////////////////////////////////////////
	CDC ImageDC;
	CBitmap ImageBmp;

	ImageDC.CreateCompatibleDC(temDC);
	ImageBmp.CreateCompatibleBitmap(temDC,rect.Width(),rect.Height());
	ImageDC.SelectObject(&ImageBmp);
	ImageDC.StretchBlt(0,0,rect.Width(),rect.Height(),temDC,rect.left,rect.top,rect.Width(),rect.Height(),SRCCOPY);

	CBitmap *pMemBmp=ImageDC.GetCurrentBitmap();
	HBITMAP hBmp=(HBITMAP)CopyImage(pMemBmp->operator HBITMAP(),IMAGE_BITMAP,rect.Width(),rect.Height(),LR_COPYRETURNORG);
	return (unsigned int)hBmp;
}

void CSCTDrawActivexCtrl::SetAlphaProperty(USHORT Alpha) //设置对象的透明度，用在荧光笔上
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	pbPolylineUnit->Alpha=Alpha;
	// TODO: Add your dispatch handler code here
}

void CSCTDrawActivexCtrl::GetOperationType(DataInfo GetDataInfo)
{
	switch(GetDataInfo.OperationKind)
	{
	case 0:
		pbUnit=pbLineUnit;
		break;
	case 1:
		pbUnit=pbRectUnit;
		break;
	case 2:
		pbUnit=pbRoundRectUnit;
		break;
	case 3:
		pbUnit=pbEllipseUnit;
		break;
	case 4:
		pbUnit=pbArcUnit;
		break;
	case 5:
		pbUnit=pbSectorUnit;
		break;
	case 6:
		pbUnit=pbPenBrushUnit;
		break;
	case 7:
		pbUnit=pbPolylineUnit;
		break;
	case 8:
		pbUnit=pbTextUnit;
		break;
	case 9:
		pbUnit=pbBmpUnit;
		break;
	case 10:
		pbUnit=pbCombination;
		break;
	case 11:
		pbUnit=pbPolygonUnit;
		break;
	case 12:
		pbUnit=pbRgnObjUnit;
		break;
	}
}

void CSCTDrawActivexCtrl::ChangeOperations(CPoint point)//?什么时候调用
{
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	float Width=Rel_EndPoint.x-Rel_StartPoint.x;
	float Height=Rel_EndPoint.y-Rel_StartPoint.y;
	point=CaculatePoint(point,GetDataInfo.CenterPoint,GetDataInfo.RotateAngle);
	point.x=point.x/fabs(GetDataInfo.xScale);
	point.y=point.y/fabs(GetDataInfo.yScale);
	GetDataInfo.AllRate[ChangeIndex].xRate=float(point.x-Rel_StartPoint.x)/Width;
	GetDataInfo.AllRate[ChangeIndex].yRate=float(point.y-Rel_StartPoint.y)/Height;
	if(GetDataInfo.xScale<0.0)
		GetDataInfo.AllRate[ChangeIndex].xRate=1-GetDataInfo.AllRate[ChangeIndex].xRate;
	if(GetDataInfo.yScale<0.0)
		GetDataInfo.AllRate[ChangeIndex].yRate=1-GetDataInfo.AllRate[ChangeIndex].yRate;
	if(SelObjList[0].SelDataInfo.AllRate[0].xRate-SelObjList[0].SelDataInfo.AllRate[GetDataInfo.AllRate.size()-1].xRate==0.0&&SelObjList[0].SelDataInfo.AllRate[0].yRate-SelObjList[0].SelDataInfo.AllRate[GetDataInfo.AllRate.size()-1].yRate==0.0)
		GetDataInfo.AllRate[0]=GetDataInfo.AllRate[GetDataInfo.AllRate.size()-1];
	SelObjList[0].SelDataInfo=GetDataInfo;
	DataArray[SelObjList[0].Index]=GetDataInfo;

	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::FinishChangePolygon(void)//多边形的顶点变化时会用到这个函数
{
	DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
	std::vector<CPoint> pts;
	int minx=WindowRect.right;
	int miny=WindowRect.bottom;
	int maxx=WindowRect.left;
	int maxy=WindowRect.top;
	float Width=Rel_EndPoint.x-Rel_StartPoint.x;
	float Height=Rel_EndPoint.y-Rel_StartPoint.y;
	for(int Index=0;Index<GetDataInfo.AllRate.size();Index++)
	{
		CPoint AddPt;
		AddPt.x=Rel_StartPoint.x+Width*GetDataInfo.AllRate[Index].xRate;
		AddPt.y=Rel_StartPoint.y+Height*GetDataInfo.AllRate[Index].yRate;
		minx=(minx<AddPt.x)?minx:AddPt.x;
		miny=(miny<AddPt.y)?miny:AddPt.y;
		maxx=(maxx>AddPt.x)?maxx:AddPt.x;
		maxy=(maxy>AddPt.y)?maxy:AddPt.y;
		pts.push_back(AddPt);
	}
	Rel_StartPoint=CPoint(minx,miny);
	Rel_EndPoint=CPoint(maxx,maxy);
	////////////////////////////////////////////////////////////////////////////
	double len=sqrt(float(Rel_StartPoint.x*Rel_StartPoint.x)+float(Rel_StartPoint.y*Rel_StartPoint.y));
	double Angle=atan2(float(Rel_StartPoint.y),float(Rel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(Rel_EndPoint.x*Rel_EndPoint.x)+float(Rel_EndPoint.y*Rel_EndPoint.y));
	Angle=atan2(float(Rel_EndPoint.y),float(Rel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	GetDataInfo.CenterPoint.x=(GetDataInfo.StartPoint.x+GetDataInfo.EndPoint.x)/2;
	GetDataInfo.CenterPoint.y=(GetDataInfo.StartPoint.y+GetDataInfo.EndPoint.y)/2;
	/////////////////////////////////////////////////////////////////////////////
	Width=Rel_EndPoint.x-Rel_StartPoint.x;
	Height=Rel_EndPoint.y-Rel_StartPoint.y;

	for(int Index=0;Index<pts.size();Index++)
	{
		GetDataInfo.AllRate[Index].xRate=float(pts[Index].x-Rel_StartPoint.x)/Width;
		GetDataInfo.AllRate[Index].yRate=float(pts[Index].y-Rel_StartPoint.y)/Height;
	}

	SelObjList[0].SelDataInfo=GetDataInfo;
	DataArray[SelObjList[0].Index]=GetDataInfo;
}

bool CSCTDrawActivexCtrl::InitOpengl(void) //初始化OPenGL
{
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	int Flag=ChoosePixelFormat(hDC,&pfd);
	if(Flag==0)
	{
		Flag=1;
		if(DescribePixelFormat(hDC,1,sizeof(PIXELFORMATDESCRIPTOR),&pfd)==0)
			return false;  
	}

	if(SetPixelFormat(hDC,Flag,&pfd)==false)
		return false;

	hGLRC=NULL;
	hGLRC=wglCreateContext(hDC);
	if(hGLRC==NULL)
		return false;
	if(wglMakeCurrent(hDC,hGLRC)==false)
		return false;

	///////////////////////////////////////////////////////////////////
	int width=WindowRect.Width();
	int height=WindowRect.Height();
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,(GLdouble)width,0.0,(GLdouble)height,-1000,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_SMOOTH);
	///////////////////////////////
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	///////////////////////////////
	return true;
}

void CSCTDrawActivexCtrl::BeginChart(LPCTSTR HeadLine, OLE_COLOR bkColor) //具体的饼状图和柱状图的绘制过程(开始)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	ClearDataInfo();
	pDataInfo.ChartInfo.bkColor=bkColor;
	pDataInfo.ChartInfo.CharNum=strlen(HeadLine)+1;
	pDataInfo.ChartInfo.HeadLine=new char[pDataInfo.ChartInfo.CharNum];
	memset(pDataInfo.ChartInfo.HeadLine,'\0',pDataInfo.ChartInfo.CharNum);
	strcpy(pDataInfo.ChartInfo.HeadLine,HeadLine);
}

void CSCTDrawActivexCtrl::AddChartData(LPCTSTR Name, FLOAT Number, OLE_COLOR color) //具体的饼状图和柱状图的绘制过程（添加具体数据）
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	CString str=Name;
	ChartData AddData;
	AddData.CharNum=str.GetLength()+1;
	AddData.name=new char[AddData.CharNum];
	memset(AddData.name,'\0',AddData.CharNum);
	strcpy(AddData.name,Name);
	AddData.number=Number;
	AddData.color=color;
	pDataInfo.ChartInfo.ChartDataArray.push_back(AddData);
}

void CSCTDrawActivexCtrl::FinishChart(SHORT Type) //具体的饼状图和柱状图的绘制过程（完成）
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int Size=pDataInfo.ChartInfo.ChartDataArray.size();
	if(Size==0)
		return;
	float Count=0;
	float max=0;
	for(int Index=0;Index<Size;Index++)
	{
		Count=Count+pDataInfo.ChartInfo.ChartDataArray[Index].number;
		max=(max>pDataInfo.ChartInfo.ChartDataArray[Index].number)?max:pDataInfo.ChartInfo.ChartDataArray[Index].number;
	}
	if(Count==0)
		return;

	pDataInfo.ChartInfo.Type=Type;
	HFONT font= CreateFont(20, 0, 0, 0,FW_MEDIUM, 0, 0, 0,GB2312_CHARSET,0, 0, 0, FF_MODERN,"宋体");
	HFONT oldfont =(HFONT)SelectObject(hDC, font); 

	float Red=GetRValue(pDataInfo.ChartInfo.bkColor)/255.0;
	float Green=GetGValue(pDataInfo.ChartInfo.bkColor)/255.0;
	float Blue=GetBValue(pDataInfo.ChartInfo.bkColor)/255.0;
	glClearColor(Red,Green,Blue,1.0);
	if(Type==1)
		PieChart(Size,Count,font);
	else
		HistogramChart(Size,max,font);
	//////////////////////////////////HeadLine/////////////////////////////////////////////////
	Graphics graphics(hDC);
	Font gfont(hDC,font);
	SizeF layoutSize(100, 50);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	SizeF stringSize;
	CString str=pDataInfo.ChartInfo.HeadLine;
	str="  "+str;
	int len=str.GetLength();
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=str.AllocSysString();
	graphics.MeasureString(WideStr,len, &gfont, layoutSize, &format, &stringSize);
	float fontWidth=stringSize.Width;
	wchar_t *wchar=L"国";
	graphics.MeasureString(wchar,wcslen(wchar), &gfont, layoutSize, &format, &stringSize);
	float fontHeight=stringSize.Height;
	glColor3f(0.0,0.0,0.0);
	glRasterPos3f(ClientRect.Width()/2-fontWidth/2,ClientRect.Height()-fontHeight,0.0);
	for(int Index=0;Index<wcslen(WideStr);Index++)
	{
		GLuint list=glGenLists(1);
		wglUseFontBitmapsW(hDC,WideStr[Index],1,list);
		glCallList(list);
		glDeleteLists(list,1);
	}
	SysFreeString(WideStr);
	//////////////////////////////////////////////////////////////////////////////////////////////
	IStream *pIStream=SaveDataToBmp();
	AddPictureFromData(pIStream);
	///////////////////////////////////////////////////////////////////////////////
	SelectObject(hDC,oldfont);
}

void CSCTDrawActivexCtrl::PieChart(int Size,float Count,HFONT hfont) //饼状图绘制用到
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
	float Red,Green,Blue;
	float StartAngle,SweepAngle;
	StartAngle=SweepAngle=0.0;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef((50+ClientRect.Width()-200)/2,(ClientRect.Height()-100)/2,0);
	glRotatef(-60,1,0,0);


	for(int Index=0;Index<Size;Index++)
	{
		SweepAngle=2*PI*pDataInfo.ChartInfo.ChartDataArray[Index].number/Count;
		Red=GetRValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		Green=GetGValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		Blue=GetBValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		glColor3f(Red/255,Green/255,Blue/255);
		DrawPieChart(StartAngle,SweepAngle,Red/255,Green/255,Blue/255,pDataInfo.ChartInfo.ChartDataArray[Index].number/Count,hfont);
		StartAngle+=SweepAngle;
	}
	glPopMatrix();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float height=float(15*Size+10*(Size-1))/2.0;
	glPushMatrix();
	glTranslatef(ClientRect.Width()-150,ClientRect.Height()/2,0);
	for(int Index=0;Index<Size;Index++)
	{
		Red=GetRValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		Green=GetGValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		Blue=GetBValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		glColor3f(Red/255,Green/255,Blue/255);
		glBegin(GL_POLYGON);
		glVertex3f(0.0f,height,0.0f);
		glVertex3f(0.0f,height-15,0.0f);
		glVertex3f(15.0f,height-15,0.0f);
		glVertex3f(15.0f,height,0.0f);
		glEnd();
		glColor3f(0.0,0.0,0.0);
		glRasterPos3f(20.0f,height-15,0.0);
		CString str=pDataInfo.ChartInfo.ChartDataArray[Index].name;
		int len=str.GetLength();
		wchar_t *WideStr;
		WideStr=new wchar_t[len];
		WideStr=str.AllocSysString();
		for(int Index=0;Index<wcslen(WideStr);Index++)
		{
			GLuint list=glGenLists(1);
			wglUseFontBitmapsW(hDC,WideStr[Index],1,list);
			glCallList(list);
			glDeleteLists(list,1);
		}
		SysFreeString(WideStr);
		height=height-15-10;
	}
	glPopMatrix();

	//glFlush();
	//SwapBuffers(hDC); 
}

void CSCTDrawActivexCtrl::DrawPieChart(float StartAngle, float SweepAngle,float Red,float Green,float Blue,float rate,HFONT hfont)//绘制饼状图
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
	float z=(float)ClientRect.Height()/10.0;
	float x,y,xRadius,yRadius;
	xRadius=float(ClientRect.Width()-250)/2.0;
	yRadius=float(ClientRect.Height()-150)/2.0;
	yRadius=yRadius/cos(PI/3);

	typedef struct pt
	{
		float x;
		float y;
	}pt;

	std::vector<pt> pts;
	pt Addpt;
	float xmid,ymid;
	for(float Index=0.0;Index<=SweepAngle;Index+=0.0001)
	{
		Addpt.x=xRadius*cos(StartAngle+Index);
		Addpt.y=yRadius*sin(StartAngle+Index);
		pts.push_back(Addpt);
		if(Index-SweepAngle/2<0.0001)
		{
			xmid=Addpt.x;
			ymid=Addpt.y;
		}
	}
	float xlight,ylight,zlight;
	xlight=0;
	ylight=yRadius;
	zlight=yRadius*3;

	glEnable(GL_LIGHTING);
	GLfloat light0_ambient[] = { Red,Green,Blue, 1.0 };
	GLfloat light0_diffuse[] = { Red,Green,Blue,1.0 };
	GLfloat light0_specular[] = { Red,Green,Blue,1.0 };
	GLfloat light0_position[] ={ xlight,ylight,zlight,1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	Addpt.x=0.0;
	Addpt.y=0.0;
	pts.push_back(Addpt);
	pts.push_back(pts[0]);

	int num=pts.size();	
	for(UINT Index=0;Index<num-1;Index++)
	{
		glBegin(GL_POLYGON);
		x=pts[Index].x;
		y=pts[Index].y;
		glNormal3f(x,y,0);
		glVertex3f(x,y,0);
		glVertex3f(x,y,z);
		x=pts[Index+1].x;
		y=pts[Index+1].y;
		glVertex3f(x,y,z);
		glVertex3f(x,y,0);
		glEnd();
	}

	for(UINT Index=0;Index<num-1;Index++)
	{
		glBegin(GL_POLYGON);
		glNormal3f(0,0,-1);
		x=pts[Index].x;
		y=pts[Index].y;
		glVertex3f(x,y,0);
		x=pts[Index+1].x;
		y=pts[Index+1].y;
		glVertex3f(x,y,0);
		glVertex3f(0,0,0);
		glEnd();
	}

	for(UINT Index=0;Index<num-1;Index++)
	{
		glBegin(GL_POLYGON);
		glNormal3f(0,0,1);
		x=pts[Index].x;
		y=pts[Index].y;
		glVertex3f(x,y,z);
		x=pts[Index+1].x;
		y=pts[Index+1].y;
		glVertex3f(x,y,z);
		glVertex3f(0,0,z);
		glEnd();
	}

	glNewList(1,GL_COMPILE);
	for(UINT Index=0;Index<num-3;Index++)
	{
		x=pts[Index].x;
		y=pts[Index].y;
		glVertex3f(x,y,0);
		x=pts[Index+1].x;
		y=pts[Index+1].y;
		glVertex3f(x,y,0);
	} 
	glEndList(); 

	glNewList(2,GL_COMPILE);
	for(UINT Index=0;Index<num-3;Index++)
	{
		x=pts[Index].x;
		y=pts[Index].y;
		glVertex3f(x,y,z);
		x=pts[Index+1].x;
		y=pts[Index+1].y;
		glVertex3f(x,y,z);
	} 
	glEndList();

	glLineWidth(0.5);
	glBegin(GL_LINES);
	glCallList(1);
	glEnd();

	glBegin(GL_LINES);
	glCallList(2);
	glEnd();
	/////////////////////////////////////////////////////////////////////////
	Graphics graphics(hDC);
	Font gfont(hDC,hfont);
	SizeF layoutSize(100, 50);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	SizeF stringSize;
	CString StrRate;
	StrRate.Format("%0.2f%%",rate*100);
	StrRate=" "+StrRate;
	int len=StrRate.GetLength();
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=StrRate.AllocSysString();
	graphics.MeasureString(WideStr,len, &gfont, layoutSize, &format, &stringSize);

	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(0,0,0);

	//glTranslatef(xmid/2-stringSize.Width/2,ymid/2-stringSize.Height/2,45) ; 
	glRasterPos3f(xmid/2-stringSize.Width/2,ymid/2-stringSize.Height/2,z+5);
	//glScalef(20,70,0);

	//GLYPHMETRICSFLOAT gmf[1]; 
	GLuint list=glGenLists(1);

	///////////////////////////////////////////////////////////////
	glPushAttrib(GL_CURRENT_BIT);
	for(int Index=0;Index<wcslen(WideStr);Index++)
	{
		//wglUseFontOutlines(hDC,WideStr[Index], 1, list, 0.0, 0.5, WGL_FONT_POLYGONS, gmf);
		wglUseFontBitmapsW(hDC,WideStr[Index],1,list);
		glCallList(list);
		glDeleteLists(list,1);
	}
	glPopAttrib();
	SysFreeString(WideStr);	
	glPopMatrix();
	/////////////////////////////////////////////////////////////////////////
}

IStream* CSCTDrawActivexCtrl::SaveDataToBmp(void) //保存数据为bmp图片
{
	CRect rect;
	GetClientRect(&rect);
	int width=rect.Width();
	int height=rect.Height();
	//CFile m_BMPFile;
	//m_BMPFile.Open("c:\\1.bmp",CFile::modeCreate|CFile::modeWrite); 
	IStream* pfIStream= NULL;
	CreateStreamOnHGlobal(NULL,TRUE,&pfIStream);

	BITMAPFILEHEADER m_BMPHeader; //BMP文件头
	BITMAPINFO m_BMPInfo; //BMP信息块
	BITMAPINFOHEADER m_BMPInfoHeader; //BMP信息头（即包含在BMP信息块的 信息头）
	RGBQUAD m_BMPRgbQuad; //BMP色彩表（即包含在BMP信息块的色彩表）

	m_BMPHeader.bfType=0x4D42;
	m_BMPHeader.bfSize=3*width*height+0x36; //指示 整个BMP文件字节数，其中0x36是文件头本身的长度
	m_BMPHeader.bfReserved1=0x0;
	m_BMPHeader.bfReserved2=0x0;
	m_BMPHeader.bfOffBits=0x36; //x36是文件头本身的长度
	//以上共占据14个字节
	m_BMPInfoHeader.biSize=sizeof(BITMAPINFOHEADER); //指示 文件信息头大小
	m_BMPInfoHeader.biWidth=width; //图片宽度
	m_BMPInfoHeader.biHeight=height; //图片高度
	m_BMPInfoHeader.biPlanes=1; 
	m_BMPInfoHeader.biBitCount=24; //图片位数，位24位图
	//以上共占据14+16个字节
	m_BMPInfoHeader.biCompression=0; //表示没有压缩
	m_BMPInfoHeader.biSizeImage=0x30; //因为没有压缩，所以可以设置为0
	m_BMPInfoHeader.biXPelsPerMeter=0x0;
	m_BMPInfoHeader.biYPelsPerMeter=0x0;
	m_BMPInfoHeader.biClrUsed=0; //表明使用所有索引色
	m_BMPInfoHeader.biClrImportant=0; //说明对图象显示有重要影响的颜色索引的数目，0表示都重要。
	//以上共占据14+16+24个字节

	m_BMPInfo.bmiHeader=m_BMPInfoHeader;


	//m_BMPFile.Write(&(m_BMPHeader),sizeof(m_BMPHeader));
	//m_BMPFile.Write(&m_BMPInfoHeader,sizeof(m_BMPInfo)-sizeof(m_BMPRgbQuad));
	pfIStream->Write(&(m_BMPHeader),sizeof(m_BMPHeader),NULL);
	pfIStream->Write(&m_BMPInfoHeader,sizeof(m_BMPInfo)-sizeof(m_BMPRgbQuad),NULL);
	////////////////////////////////////////////////////////////////////////////////
	static GLint pixelLength;
	static GLubyte * pixelData;
	pixelLength =width * 3;
	if ( pixelLength % 4 != 0 )
	{
		pixelLength += 4 - pixelLength%4;
	}
	pixelLength *=height;
	pixelData =(GLubyte *)malloc( pixelLength );
	if ( pixelData == 0 )
	{
		printf( "\a\n分配内存失败!" );
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels( 0, 0,width,height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelData);

	//写入像素数据
	//m_BMPFile.Write(pixelData, pixelLength);
	//m_BMPFile.Close();
	pfIStream->Write(pixelData, pixelLength,NULL);
	delete [] pixelData;
	return pfIStream;
}

void CSCTDrawActivexCtrl::AddPictureFromData(IStream * pIStream)
{
	CRect rect;
	GetClientRect(&rect);

	pDataInfo.ChartInfo.CharNum=0;
	pDataInfo.OperationKind=9;
	pDataInfo.IsLocked=false;
	pDataInfo.IsCombination=false;
	pDataInfo.IsErased=false;
	pDataInfo.IsDisjoint=false;
	pDataInfo.StartPoint=CPoint(rect.CenterPoint().x-200,rect.CenterPoint().y-200);
	pDataInfo.EndPoint=CPoint(rect.CenterPoint().x+200,rect.CenterPoint().y+200);
	ZeroMemory(pDataInfo.Reserved0,200);
	ZeroMemory(pDataInfo.Reserved1,200);
	ZeroMemory(pDataInfo.Reserved2,200);
	ZeroMemory(pDataInfo.Reserved3,200);
	ZeroMemory(pDataInfo.Reserved4,1024);
	pDataInfo.xScale=1;
	pDataInfo.yScale=1;
	//pDataInfo.SelectColor=SelectColor;
	///////////////////////////////////////////
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(memDC->m_hDC);
	pDataInfo.hRgn=Rgn.GetHRGN(&graphics);

	STATSTG statstg;
	HGLOBAL hGlobal;
	pIStream->Stat(&statstg,STATFLAG_NONAME);
	pDataInfo.ImageSize=statstg.cbSize.QuadPart;
	GetHGlobalFromStream(pIStream,&hGlobal);
	pDataInfo.ImageByte=(BYTE*)GlobalLock(hGlobal);
	GlobalUnlock(hGlobal);  
	////////////////////////////////////////////////////////  
	pDataInfo.preAngle=0;
	pDataInfo.RotateAngle=0;
	pDataInfo.CenterPoint.y=(pDataInfo.StartPoint.y+pDataInfo.EndPoint.y)/2;
	pDataInfo.CenterPoint.x=(pDataInfo.StartPoint.x+pDataInfo.EndPoint.x)/2;
	pDataInfo.SelectColor = DataArray.size();
	DataArray.push_back(pDataInfo);
	UndoArray.push_back(pDataInfo);
		
	RedoArray.clear();
	SelObjList.clear();
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);

	///////////////////////////////////////////////////////////////////////////
	//SelectColor++;
	BmpFromHandle=true;
	Bitmap bp(pIStream,false);
	graphics.DrawImage(&bp,RectF(rect.CenterPoint().x-200,rect.CenterPoint().y-200,400,400));
	BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
}

SHORT CSCTDrawActivexCtrl::GetChartDataNum(void)//饼状图和柱状图进行二次编辑时调用此函数进行参数重新设置
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	int num=0;
	if(SelObjList.size()==1&&SelObjList[0].SelDataInfo.ChartInfo.ChartDataArray.size()>0)
		num=SelObjList[0].SelDataInfo.ChartInfo.ChartDataArray.size();
	return num;
}

void CSCTDrawActivexCtrl::ChangeSelChartData(void)//饼状图和柱状图进行二次编辑时调用此函数进行参数重新设置
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int Size=pDataInfo.ChartInfo.ChartDataArray.size();
	if(Size==0)
		return;
	float Count=0;
	float max=0;	
	for(int Index=0;Index<Size;Index++)
	{
		Count=Count+pDataInfo.ChartInfo.ChartDataArray[Index].number;
		max=(max>pDataInfo.ChartInfo.ChartDataArray[Index].number)?max:pDataInfo.ChartInfo.ChartDataArray[Index].number;
	}
	if(Count==0)
		return;
	SelObjList[0].SelDataInfo.ChartInfo=pDataInfo.ChartInfo;

	HFONT font= CreateFont(20, 0, 0, 0,FW_MEDIUM, 0, 0, 0,GB2312_CHARSET,0, 0, 0, FF_MODERN,"宋体");
	HFONT oldfont =(HFONT)SelectObject(hDC, font); 
	float Red=GetRValue(pDataInfo.ChartInfo.bkColor)/255.0;
	float Green=GetGValue(pDataInfo.ChartInfo.bkColor)/255.0;
	float Blue=GetBValue(pDataInfo.ChartInfo.bkColor)/255.0;
	glClearColor(Red,Green,Blue,1.0);
	if(pDataInfo.ChartInfo.Type==1)
		PieChart(Size,Count,font);
	else
		HistogramChart(Size,max,font);
	//////////////////////////////////HeadLine/////////////////////////////////////////////////
	Graphics graphics(hDC);
	Font gfont(hDC,font);
	SizeF layoutSize(100, 50);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	SizeF stringSize;
	CString str=pDataInfo.ChartInfo.HeadLine;
	str="  "+str;
	int len=str.GetLength();
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=str.AllocSysString();
	graphics.MeasureString(WideStr,len, &gfont, layoutSize, &format, &stringSize);
	float fontWidth=stringSize.Width;
	wchar_t *wchar=L"国";
	graphics.MeasureString(wchar,wcslen(wchar), &gfont, layoutSize, &format, &stringSize);
	float fontHeight=stringSize.Height;
	glColor3f(0.0,0.0,0.0);
	glRasterPos3f(ClientRect.Width()/2-fontWidth/2,ClientRect.Height()-fontHeight,0.0);
	for(int Index=0;Index<wcslen(WideStr);Index++)
	{
		GLuint list=glGenLists(1);
		wglUseFontBitmapsW(hDC,WideStr[Index],1,list);
		glCallList(list);
		glDeleteLists(list,1);
	}
	SysFreeString(WideStr);
	//////////////////////////////////////////////////////////////////////////////////////////////
	IStream *pIStream=SaveDataToBmp();
	////////////////////////////////////////////////////////////////////////////////
	STATSTG statstg;
	HGLOBAL hGlobal;
	pIStream->Stat(&statstg,STATFLAG_NONAME);
	SelObjList[0].SelDataInfo.ImageSize=statstg.cbSize.QuadPart;
	GetHGlobalFromStream(pIStream,&hGlobal);
	SelObjList[0].SelDataInfo.ImageByte=(BYTE*)GlobalLock(hGlobal);
	GlobalUnlock(hGlobal); 
	GlobalFree(DataArray[SelObjList[0].Index].ImageByte);
	DataArray[SelObjList[0].Index]=SelObjList[0].SelDataInfo;
	///////////////////////////////////////////////////////////////////////////////
	SelectObject(hDC,oldfont);
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
}

void CSCTDrawActivexCtrl::HistogramChart(int Size,float max, HFONT hfont)//用于绘制柱状图
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
	float Red,Green,Blue,rate,Interval;
	rate=(float)(ClientRect.Height()-200)/max;
	Interval=(float)(ClientRect.Width()-150)/(float)Size;

	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(50,50,0);
	////////////////////////////////坐标系////////////////////////////
	glColor3f(0.0f,0.0f,0.0f);
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f,ClientRect.Height()-150.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(ClientRect.Width()-100,0.0f,0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(ClientRect.Width()-100+10.0*cos(PI-PI/6.0),10.0*sin(PI-PI/6.0),0.0f);
	glVertex3f(ClientRect.Width()-100,0.0f,0.0f);
	glVertex3f(ClientRect.Width()-100+10.0*cos(PI+PI/6.0),10.0*sin(PI+PI/6.0),0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(10.0*cos(3*PI/2-PI/6.0),ClientRect.Height()-150.0f+10.0*sin(3*PI/2-PI/6.0),0.0f);
	glVertex3f(0.0f,ClientRect.Height()-150.0f,0.0f);
	glVertex3f(10.0*cos(-PI/2+PI/6.0),ClientRect.Height()-150.0f+10.0*sin(-PI/2+PI/6.0),0.0f);
	glEnd();
	///////////////////////////////////////////////////////////////////////
	glRotatef(-70,1,0,0);
	for(int Index=0;Index<Size;Index++)
	{
		Red=GetRValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		Green=GetGValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		Blue=GetBValue(pDataInfo.ChartInfo.ChartDataArray[Index].color);
		glColor3f(Red/255,Green/255,Blue/255);
		DrawHistogram(Red/255,Green/255,Blue/255,pDataInfo.ChartInfo.ChartDataArray[Index].number*rate,Interval*0.6,Interval*Index+Interval/2.0,pDataInfo.ChartInfo.ChartDataArray[Index].number,pDataInfo.ChartInfo.ChartDataArray[Index].name,hfont);
	}
	glPopMatrix();
}

void CSCTDrawActivexCtrl::DrawHistogram(float Red, float Green, float Blue,float height,float width,float center,float number,CString name, HFONT hfont)//绘制柱状图
{
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(center,0.0,0.0);

	GLfloat light0_ambient[] = { Red,Green,Blue, 1.0 };
	GLfloat light0_diffuse[] = { Red,Green,Blue,1.0 };
	GLfloat light0_specular[] = { Red,Green,Blue,1.0 };
	GLfloat light0_position[] ={ 0.0,-height/cos(PI/10.0),height+height/cos(PI/10.0),1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);	
	glVertex3f(-width/2.0,0.0,0.0);
	glVertex3f(width/2.0,0.0,0.0);
	glVertex3f(width/2.0,width,0.0);
	glVertex3f(-width/2.0,width,0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);	
	glVertex3f(-width/2.0,0.0,0.0);
	glVertex3f(width/2.0,0.0,0.0);
	glVertex3f(width/2.0,0.0,height);
	glVertex3f(-width/2.0,0.0,height);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);	
	glVertex3f(-width/2.0,0.0,0.0);
	glVertex3f(-width/2.0,width,0.0);
	glVertex3f(-width/2.0,width,height);
	glVertex3f(-width/2.0,0.0,height);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);	
	glVertex3f(width/2.0,0.0,0.0);
	glVertex3f(width/2.0,width,0.0);
	glVertex3f(width/2.0,width,height);
	glVertex3f(width/2.0,0.0,height);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);	
	glVertex3f(-width/2.0,width,0.0);
	glVertex3f(width/2.0,width,0.0);
	glVertex3f(width/2.0,width,height);
	glVertex3f(-width/2.0,width,height);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);	
	glVertex3f(-width/2.0,0.0,height);
	glVertex3f(width/2.0,0.0,height);
	glVertex3f(width/2.0,width,height);
	glVertex3f(-width/2.0,width,height);
	glEnd();
	/////////////////////////////////////////////////////////////////////////
	Graphics graphics(hDC);
	Font gfont(hDC,hfont);
	SizeF layoutSize(100, 50);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	SizeF stringSize;
	CString StrRate;
	StrRate.Format("%0.2f",number);
	StrRate="  "+StrRate;
	int len=StrRate.GetLength();
	wchar_t *WideStr;
	WideStr=new wchar_t[len];
	WideStr=StrRate.AllocSysString();
	graphics.MeasureString(WideStr,len, &gfont, layoutSize, &format, &stringSize);

	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);
	glRasterPos3f(-stringSize.Width/2.0,0,height+width*sin(PI/6.0));
	GLuint list=glGenLists(1);
	///////////////////////////////////////////////////////////////
	glPushAttrib(GL_CURRENT_BIT);
	for(int Index=0;Index<wcslen(WideStr);Index++)
	{
		wglUseFontBitmapsW(hDC,WideStr[Index],1,list);
		glCallList(list);
		glDeleteLists(list,1);
	}
	glPopAttrib();
	SysFreeString(WideStr);
	/////////////////////////////////////////////////////////////////////////////////////////////
	name="  "+name;
	len=name.GetLength();
	WideStr=new wchar_t[len];
	WideStr=name.AllocSysString();
	graphics.MeasureString(WideStr,len, &gfont, layoutSize, &format, &stringSize);
	float fontWidth=stringSize.Width;
	wchar_t *wchar=L"国";
	graphics.MeasureString(wchar,wcslen(wchar), &gfont, layoutSize, &format, &stringSize);
	float fontHeight=stringSize.Height;

	glRasterPos3f(-fontWidth/2.0,-2*fontHeight,0.0);
	///////////////////////////////////////////////////////////////
	glPushAttrib(GL_CURRENT_BIT);
	for(int Index=0;Index<wcslen(WideStr);Index++)
	{
		wglUseFontBitmapsW(hDC,WideStr[Index],1,list);
		glCallList(list);
		glDeleteLists(list,1);
	}
	glPopAttrib();
	SysFreeString(WideStr);
	/////////////////////////////////////////////////////////////////////////////////////////////
	glPopMatrix();
}


BSTR CSCTDrawActivexCtrl::GetChartItemName(SHORT Index)//饼状图和柱状图二次编辑时得到具体项的名字
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	strResult=SelObjList[0].SelDataInfo.ChartInfo.ChartDataArray[Index].name;
	return strResult.AllocSysString();
}

FLOAT CSCTDrawActivexCtrl::GetChartItemNumber(USHORT Index) //饼状图和柱状图二次编辑时得到具体项的数值
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return SelObjList[0].SelDataInfo.ChartInfo.ChartDataArray[Index].number;
}

OLE_COLOR CSCTDrawActivexCtrl::GetChartItemColor(SHORT Index) //饼状图和柱状图二次编辑时得到具体项的颜色
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	return SelObjList[0].SelDataInfo.ChartInfo.ChartDataArray[Index].color;
}

BSTR CSCTDrawActivexCtrl::GetChartItemHeadLine(void)//饼状图和柱状图二次编辑时得到整个图形的标题
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	strResult=SelObjList[0].SelDataInfo.ChartInfo.HeadLine;
	return strResult.AllocSysString();
}

OLE_COLOR CSCTDrawActivexCtrl::GetChartItembkColor(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return SelObjList[0].SelDataInfo.ChartInfo.bkColor;
}

SHORT CSCTDrawActivexCtrl::SaveAsOtherTypes(SHORT PageID, LPCTSTR FilePath) //将白板保存为其他格式的文档
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here  
	SavePage();	

	Graphics graphics(hDC);
	Matrix matrix;
	CRect ClientRect;
	ClientRect=WindowRect;

	CDC *SaveAsDC=new CDC;
	CBitmap SaveAsBmp;
	SaveAsDC->CreateCompatibleDC(memDC);
	SaveAsBmp.CreateCompatibleBitmap(memDC,WindowRect.Width(),WindowRect.Height());
	SaveAsDC->SelectObject(&SaveAsBmp);

	int xOffset,yOffset;
	UINT PictureNum=0;
	UINT PictureIndex;
	UINT Index=0;
	UINT Size=PageArray.size();  
	if(PageID>=0)
	{
		Index=PageID;
		Size=PageID+1;
	}
	for(;Index<Size;Index++)
	{
		ClearData();
		PageInfo GetPage=PageArray[Index];
		DisplayRect=GetPage.DisplayRect;
		m_bkColor=GetPage.bkColor; 
		m_fillStyle=GetPage.FillStyle;
		m_fillColor=GetPage.FillColor;
		m_foreColor=GetPage.ForeColor;	
		m_operations=GetPage.Operations;
		m_penWidth=GetPage.PenWidth;
		m_penColor=GetPage.PenColor;
		m_fontColor=GetPage.FontColor;
		m_bkColor=GetPage.bkColor;
		m_lineStyle=GetPage.LineStyle;
		int daSize=GetPage.DataArray.size();
		//SelectColor=Size;
		for(int i=0;i<daSize;i++)
			DataArray.push_back(GetPage.DataArray[i]);
		daSize=GetPage.bkImageArray.size();
		for(int k=0;k<daSize;k++)
			bkImageArray.push_back(GetPage.bkImageArray[k]);

		if(DisplayRect.left>0)
			DisplayRect.left=0;
		if(DisplayRect.top>0)
			DisplayRect.top=0;
		if(DisplayRect.right<WindowRect.right)
			DisplayRect.right=WindowRect.right;
		if(DisplayRect.bottom<WindowRect.bottom)
			DisplayRect.bottom=WindowRect.bottom;

		PictureIndex=0;
		ClientRect.left=DisplayRect.left;
		ClientRect.top=DisplayRect.top;
		ClientRect.right=ClientRect.left+WindowRect.Width();
		ClientRect.bottom=ClientRect.top+WindowRect.Height();

		while(ClientRect.top<DisplayRect.bottom)
		{
			while(ClientRect.left<DisplayRect.right)
			{
				xOffset=ClientRect.CenterPoint().x-WindowRect.CenterPoint().x;
				yOffset=ClientRect.CenterPoint().y-WindowRect.CenterPoint().y;
				//////////////////////////////////////////////////////////////////////////// 
				SaveAsDC->FillSolidRect(&WindowRect,m_bkColor);
				Graphics mGraphics(SaveAsDC->m_hDC);
				mGraphics.SetInterpolationMode(InterpolationModeHighQuality);
				int bkSize=bkImageArray.size();
				float Width,Height;  
				for(int bkIndex=0;bkIndex<bkSize;bkIndex++)
				{
					Width=bkImageArray[bkIndex].Width;
					Height=bkImageArray[bkIndex].Height;
					mGraphics.TranslateTransform(bkImageArray[bkIndex].ptImage.x-xOffset,bkImageArray[bkIndex].ptImage.y-yOffset);  
					mGraphics.ScaleTransform(bkImageArray[bkIndex].xScale,bkImageArray[bkIndex].yScale);
					////////////////////////////////////////////////////////////////////////////////////////////
					IStream *pIStream;
					CreateStreamOnHGlobal(bkImageArray[bkIndex].ImageByte,true,&pIStream);
					Image image(pIStream);
					mGraphics.DrawImage(&image,-Width/2,-Height/2,Width,Height);
					////////////////////////////////////////////////////////////////////////////////////////////////	
					mGraphics.ResetTransform();
				}
				int i;
				int MaxIndex=DataArray.size()-1;
				for(i=0;i<=MaxIndex;i++)
				{
					TransferDataInfo=DataArray[i];
					if(TransferDataInfo.IsErased||TransferDataInfo.IsCombination)
						continue;
					TransferDataInfo.CenterPoint.Offset(-xOffset,-yOffset);
					TransferDataInfo.StartPoint.Offset(-xOffset,-yOffset);
					TransferDataInfo.EndPoint.Offset(-xOffset,-yOffset);
					switch (TransferDataInfo.OperationKind)
					{
					case 0:
						if(true)
						{
							HRGN hRgn=pbLineUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbLineUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 1:
						if(true)
						{
							HRGN hRgn=pbRectUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbRectUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 2:
						if(true)
						{
							HRGN hRgn=pbRoundRectUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbRoundRectUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 3:
						if(true)
						{
							HRGN hRgn=pbEllipseUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbEllipseUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 4:
						if(true)
						{
							HRGN hRgn=pbArcUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbArcUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 5:
						if(true)
						{
							HRGN hRgn=pbSectorUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbSectorUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 6:
						if(true)
						{
							HRGN hRgn=pbPenBrushUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbPenBrushUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 7:
						if(true)
						{
							HRGN hRgn=pbPolylineUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbPolylineUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 8:
						if(true)
						{
							HRGN hRgn=pbTextUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbTextUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 9:
						if(true)
						{
							HRGN hRgn=pbBmpUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbBmpUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 10:
						if(!TransferDataInfo.IsDisjoint)
						{
							if(true)
							{
								HRGN hRgn=pbCombination->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
								Region Rgn(hRgn);	 
								matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
								matrix.Rotate(TransferDataInfo.RotateAngle);
								matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
								Rgn.Transform(&matrix);
								matrix.Reset();
								if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								{
									std::map<int,int> ObjList;
									pbCombination->GetCombinatedObjs(DataArray,TransferDataInfo,ObjList);
									pbCombination->ReDraw(SaveAsDC,DataArray,ObjList);
									ObjList.clear();
								}
							}
						}
						break;
					case 11:
						if(true)
						{
							HRGN hRgn=pbPolygonUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbPolygonUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					case 12:
						if(true)
						{
							HRGN hRgn=pbRgnObjUnit->GetRedrawHRGN(hDC,DataArray,TransferDataInfo);
							Region Rgn(hRgn);	 
							matrix.Translate(TransferDataInfo.CenterPoint.x,TransferDataInfo.CenterPoint.y);
							matrix.Rotate(TransferDataInfo.RotateAngle);
							matrix.Scale(TransferDataInfo.xScale,TransferDataInfo.yScale);
							Rgn.Transform(&matrix);
							matrix.Reset();
							if(Rgn.IsVisible(RectF(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height()),&graphics))
								pbRgnObjUnit->ReDraw(SaveAsDC,TransferDataInfo);
						}
						break;
					}
				}

				////////////////////////////////////////////////////////////////////////
				CBitmap *pMemBmp=SaveAsDC->GetCurrentBitmap();
				HBITMAP hBmp=(HBITMAP)CopyImage(pMemBmp->operator HBITMAP(),IMAGE_BITMAP,WindowRect.Width(),WindowRect.Height(),LR_COPYRETURNORG);

				CString str,strPage,strNum;
				strPage.Format("%d",Index);
				strNum.Format("%d",PictureIndex);
				str=FilePath;
				str=str+"\\";
				str=str+strPage+"_"+strNum+".bmp";
				int len=str.GetLength();
				wchar_t *WideStr;
				WideStr=new wchar_t[len];
				WideStr=str.AllocSysString();

				Bitmap bmp(hBmp,NULL);
				bmp.Save(WideStr,&BmpClsid,0);

				ClientRect.OffsetRect(WindowRect.Width(),0);
				PictureNum++;
				PictureIndex++;
			}
			ClientRect.left=DisplayRect.left;
			ClientRect.right=ClientRect.left+WindowRect.Width();
			ClientRect.OffsetRect(0,WindowRect.Height());
		}
	}
	return PictureNum;
}

void CSCTDrawActivexCtrl::DrawCompass(void) //画扇形时用到此函数
{
	Graphics graphics(memDC->m_hDC);
	HFONT hFont= CreateFont(20, 0, 0, 0,FW_MEDIUM, 0, 0, 0,GB2312_CHARSET,0, 0, 0, FF_MODERN,"宋体");
	float Angle1,Angle2,dx,dy;
	float lRate=float(GetDeviceCaps(hDC,HORZSIZE)/10.0)/float(WindowRect.Width());
	if( ptSector.size()<2)
		return;
	CPoint ptOne,ptTwo,ptThree;
	ptOne=ptSector[0];
	ptTwo=ptSector[1];

	dx=ptTwo.x-ptOne.x;
	dy=ptTwo.y-ptOne.y;
	Angle1=fmod(360.0+atan2(dy,dx)*Rate,360.0);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen mPen(Color(255,0,0,255),2);
	Pen pen(Color(255,255,0,0),2);
	graphics.DrawLine(&mPen,ptOne.x,ptOne.y,ptTwo.x,ptTwo.y);

	float len=sqrt(dx*dx+dy*dy);
	graphics.TranslateTransform(ptOne.x,ptOne.y);
	graphics.RotateTransform(Angle1);
	graphics.DrawLine(&pen,0,5,0,25);
	graphics.DrawLine(&pen,sqrt(dx*dx+dy*dy),5.0,sqrt(dx*dx+dy*dy),25.0);

	CString strFont;
	strFont.Format("%.2f",len*lRate);
	strFont=strFont+"CM";
	wchar_t *WideStr;
	WideStr=new wchar_t[strFont.GetLength()];
	WideStr=strFont.AllocSysString();
	RectF rectf;
	PointF pointf(0.0,10.0);
	const StringFormat *format=StringFormat::GenericTypographic();
	Font temFont(hDC,hFont);
	graphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
	SolidBrush  mBrush(Color(255,255,0,0));
	PointF ptFont((len-rectf.Width)/2.0,10.0);
	graphics.DrawString(WideStr,wcslen(WideStr),&temFont,ptFont,format, &mBrush);
	graphics.DrawLine(&pen,0.0,15.0,ptFont.X,15.0);
	graphics.DrawLine(&pen,ptFont.X+rectf.Width,15.0,len,15.0);
	SysFreeString(WideStr);
	graphics.ResetTransform();
	RectF OneRect(ptOne.x,ptOne.y,0,0);
	OneRect.Inflate(10,10);
	RectF TwoRect(ptTwo.x,ptTwo.y,0,0);
	TwoRect.Inflate(10,10);
	graphics.FillEllipse(&SolidBrush(Color(255,0,255,0)),OneRect);
	graphics.FillEllipse(&SolidBrush(Color(255,0,255,0)),TwoRect);
	if(ptSector.size()==2)
		return;
	ptThree=ptSector[2];
	graphics.DrawLine(&mPen,ptOne.x,ptOne.y,ptThree.x,ptThree.y);
	dx=ptThree.x-ptOne.x;
	dy=ptThree.y-ptOne.y;
	Angle2=fmod(360.0+atan2(dy,dx)*Rate,360.0);
	graphics.DrawArc(&pen,ptOne.x-20,ptOne.y-20,40,40,Angle1,fmod(360.0+Angle2-Angle1,360.0));
	graphics.TranslateTransform(ptOne.x,ptOne.y);
	graphics.RotateTransform(Angle1+fmod(360.0+Angle2-Angle1,360.0)/2);

	strFont.Format("%.2f",fmod(360.0+Angle2-Angle1,360.0));
	strFont=strFont+"℃";
	wchar_t *dWideStr;
	dWideStr=new wchar_t[strFont.GetLength()];
	dWideStr=strFont.AllocSysString();
	graphics.MeasureString(WideStr,wcslen(WideStr),&temFont,pointf,format, &rectf);
	PointF dptFont(25.0,-10.0);
	SolidBrush  dmBrush(Color(255,255,0,0));
	graphics.DrawString(WideStr,wcslen(dWideStr),&temFont,dptFont,format, &dmBrush);
	graphics.ResetTransform();

	RectF ThreeRect(ptThree.x,ptThree.y,0,0);
	ThreeRect.Inflate(10,10);
	graphics.FillEllipse(&SolidBrush(Color(255,0,255,0)),ThreeRect);
	pDataInfo.CenterPoint=ptOne;
	pDataInfo.EndPoint=ptTwo;
	pDataInfo.startAngle=Angle1;
	pDataInfo.sweepAngle=fmod(360.0+Angle2-Angle1,360.0);
}


void CSCTDrawActivexCtrl::TranslateImageToClipboard(DataInfo ImageInfo)//图片复制时用到此函数
{    
	IStream *pIStream;
	CreateStreamOnHGlobal(ImageInfo.ImageByte,true,&pIStream);
	Image image(pIStream);

	UINT Height=image.GetHeight();
	UINT Width=image.GetWidth();
	if(Height>WindowRect.Height())
		Height=WindowRect.Height();
	if(Width>WindowRect.Width())
		Width=WindowRect.Width();

	CDC ImageDC;
	CBitmap ImageBmp;


	ImageDC.CreateCompatibleDC(temDC);
	ImageBmp.CreateCompatibleBitmap(temDC,Width,Height);
	ImageDC.SelectObject(&ImageBmp);
	Graphics mGraphics(temDC->m_hDC);

	temDC->FillSolidRect(&WindowRect,RGB(255,255,255));
	mGraphics.DrawImage(&image,0,0,Width,Height);

	ImageDC.StretchBlt(0,0,Width,Height,temDC,0,0,Width,Height,SRCCOPY);

	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP,ImageBmp.m_hObject);
	CloseClipboard();
}

BOOL CSCTDrawActivexCtrl::OnTouchInput(CPoint pt, int nInputNumber , int nInputsCount , PTOUCHINPUT pInput)
{    
	if ((pInput->dwFlags & TOUCHEVENTF_DOWN) == TOUCHEVENTF_DOWN) // Touch Down event
	{
		return OnTouchInputDown(pt, pInput);
	}
	else if ((pInput->dwFlags & TOUCHEVENTF_MOVE) == TOUCHEVENTF_MOVE) // Touch Move event
	{
		return OnTouchInputMove(pt, pInput);
	}
	else if ((pInput->dwFlags & TOUCHEVENTF_UP) == TOUCHEVENTF_UP) // Touch Move event
	{
		return OnTouchInputUp(pt, pInput);
	}
	return true;
}

BOOL CSCTDrawActivexCtrl::OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput)
{
		
		
	////////////////////////////////////////////////////////////
	MultiptsPoint[pInput->dwID].clear();
	MultiptsPoint.erase(pInput->dwID);
	MultiDataInfo[pInput->dwID].AllRate.clear();
	MultiDataInfo.erase(pInput->dwID);
	MultiInfoArray.erase(pInput->dwID);
	////////////////////////////////////////////////////////
		
	switch(m_operations)
	{
	case 7:
		isMultitouch = true;
		if(!MultiTouchMode)
		{
			if(MultiInfoArray.size()==0)
				AddMultiPenBrush(pInput->dwID,pt);
		}
		
		else
		{
			AddMultiPenBrush(pInput->dwID,pt);
		}
		break;
	case 8:
		isMultitouch = true;
		if(!MultiTouchMode)
		{
			if(MultiInfoArray.size()==0)
				AddMultiPolyline(pInput->dwID,pt);
		}
		else
		{
			AddMultiPolyline(pInput->dwID,pt);
		}
		break;
	}
	////////////////////////////////////////////////////////////////////
	
	//DisplayConsole("1",sizeof("1"));
		
	return true;
}

BOOL CSCTDrawActivexCtrl::OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput)
{
	DWORD timer=GetTickCount();
	std::map<UINT,MultiInfo>::iterator iter=MultiInfoArray.begin();
	while(iter!=MultiInfoArray.end())
	{
		if(timer-iter->second.timer>2000)
		{
			MultiptsPoint[iter->first].clear();
			MultiptsPoint.erase(iter->first);
			MultiDataInfo[iter->first].AllRate.clear();
			MultiDataInfo.erase(iter->first);
			MultiInfoArray.erase(iter->first);
			break;
		}	
		iter++;
	}
	switch(m_operations)
	{
	case 7:
		isMultitouch = true;
		if(MultiptsPoint.find(pInput->dwID)!=MultiptsPoint.end()&&MultiInfoArray.find(pInput->dwID)!=MultiInfoArray.end())
		{
			MultiInfoArray[pInput->dwID].timer=timer;
			UINT Size=MultiptsPoint[pInput->dwID].size();
			if(MultiptsPoint[pInput->dwID][Size-1].point!=pt)
			{
				pbPenBrushUnit->DrawMultiPenBrush(memDC,pt,MultiptsPoint[pInput->dwID],MultiInfoArray[pInput->dwID],pInput->dwID);
				MultiInfoArray[pInput->dwID].StartPoint=pt;
				if(pt.x<MultiDataInfo[pInput->dwID].StartPoint.x)
					MultiDataInfo[pInput->dwID].StartPoint.x=pt.x;
				if(pt.y<MultiDataInfo[pInput->dwID].StartPoint.y)
					MultiDataInfo[pInput->dwID].StartPoint.y=pt.y;
				if(pt.x>MultiDataInfo[pInput->dwID].EndPoint.x)
					MultiDataInfo[pInput->dwID].EndPoint.x=pt.x;
				if(pt.y>MultiDataInfo[pInput->dwID].EndPoint.y)
					MultiDataInfo[pInput->dwID].EndPoint.y=pt.y;
				BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
			}
		}
		else
		{
			MultiptsPoint[pInput->dwID].clear();
			MultiptsPoint.erase(pInput->dwID);
			MultiDataInfo[pInput->dwID].AllRate.clear();
			MultiDataInfo.erase(pInput->dwID);
			MultiInfoArray.erase(pInput->dwID);

			if(!MultiTouchMode)
			{
				if(MultiInfoArray.size()==0)
					AddMultiPenBrush(pInput->dwID,pt);
			}
			else
			{
				AddMultiPenBrush(pInput->dwID,pt);
			}

		}

		break;
	case 8:
		isMultitouch = true;
		if(MultiptsPoint.find(pInput->dwID)!=MultiptsPoint.end()&&MultiInfoArray.find(pInput->dwID)!=MultiInfoArray.end())
		{
			UINT Size=MultiptsPoint[pInput->dwID].size();
			MultiInfoArray[pInput->dwID].timer=timer;
			if(MultiptsPoint[pInput->dwID][Size-1].point!=pt)
			{
				BitBlt(memDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),temDC->m_hDC,0,0,SRCCOPY); 
				std::map<UINT,std::vector<temSavePoint>>::iterator iter=MultiptsPoint.begin();
				while(iter!=MultiptsPoint.end())
				{
					if(iter->first==pInput->dwID)
						pbPolylineUnit->DrawMultiPolyline(memDC,pt,iter->second,MultiInfoArray[pInput->dwID],true);
					else
						pbPolylineUnit->DrawMultiPolyline(memDC,pt,iter->second,MultiInfoArray[pInput->dwID],false);
					iter++;
				}
				MultiInfoArray[pInput->dwID].StartPoint=pt;
				if(pt.x<MultiDataInfo[pInput->dwID].StartPoint.x)
					MultiDataInfo[pInput->dwID].StartPoint.x=pt.x;
				if(pt.y<MultiDataInfo[pInput->dwID].StartPoint.y)
					MultiDataInfo[pInput->dwID].StartPoint.y=pt.y;
				if(pt.x>MultiDataInfo[pInput->dwID].EndPoint.x)
					MultiDataInfo[pInput->dwID].EndPoint.x=pt.x;
				if(pt.y>MultiDataInfo[pInput->dwID].EndPoint.y)
					MultiDataInfo[pInput->dwID].EndPoint.y=pt.y;
				BitBlt(hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
			}
		}
		else
		{
			MultiptsPoint[pInput->dwID].clear();
			MultiptsPoint.erase(pInput->dwID);
			MultiDataInfo[pInput->dwID].AllRate.clear();
			MultiDataInfo.erase(pInput->dwID);
			MultiInfoArray.erase(pInput->dwID);

			if(!MultiTouchMode)
			{
				if(MultiInfoArray.size()==0)
					AddMultiPolyline(pInput->dwID,pt);
			}
			else
			{
				AddMultiPolyline(pInput->dwID,pt);
			}

		}
		break;
	case 15:
		{
			if(isgestureDown)
			{
				if(!isSaved)
				{
						
					if(isMultitouch)
					{
							
						switch(m_operation1)
						{
						case 7:
							FinishMultiPenBrush(pInput->dwID,pt);
							DisplayConsole("FinishMultiPenBrush",sizeof("FinishMultiPenBrush"));
							OnOperationsChanged();
							break;
						case 8:
							//DisplayConsole("FinishMultiPolyline11",sizeof("FinishMultiPolyline11"));
							BitBlt(memDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),temDC->m_hDC,0,0,SRCCOPY);
							pbPolylineUnit->DrawMultiPolyline(memDC,pt,MultiptsPoint[pInput->dwID],MultiInfoArray[pInput->dwID],false);
							BitBlt(temDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
							FinishMultiPolyline(pInput->dwID,pt);
							//DisplayConsole("FinishMultiPolyline",sizeof("FinishMultiPolyline"));
							OnOperationsChanged();
							break;
						}
						MultiptsPoint[pInput->dwID].clear();
						//DisplayConsole("A",sizeof("A"));
						MultiptsPoint.erase(pInput->dwID);
						//DisplayConsole("B",sizeof("B"));
						MultiDataInfo[pInput->dwID].AllRate.clear();
						//DisplayConsole("C",sizeof("C"));
						MultiDataInfo.erase(pInput->dwID);
						//DisplayConsole("D",sizeof("D"));
						MultiInfoArray.erase(pInput->dwID);
						//DisplayConsole("e",sizeof("e"));
						//MultiptsPoint.clear();
						//DisplayConsole("f",sizeof("f"));
						//MultiDataInfo.clear();
						//DisplayConsole("g",sizeof("g"));
						//MultiInfoArray.clear();
						//DisplayConsole("h",sizeof("h"));
						if(MultiInfoArray.size()==0)
						{
							DisplayConsole("4",sizeof("4"));
						lbdown=true;
						isSaved = true;
						temPoint=pt; 
						MouseCur=AfxGetApp()->LoadCursor(IDC_Eraser);
						SetCursor(MouseCur);
						pbEraseUnit->OnMouseDown(0,pt);
						}
						//DisplayConsole("i",sizeof("i"));

					}
				}
			}
		}
		break;
	}
	//DisplayConsole("2",sizeof("2"));
		
	return true;
}

BOOL CSCTDrawActivexCtrl::OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput)
{
	if(isSaved) 
		goto end;
	switch(m_operations)
	{
	case 7:
		FinishMultiPenBrush(pInput->dwID,pt);
		PageChanged();//事件触发
		break;
	case 8:
		BitBlt(memDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),temDC->m_hDC,0,0,SRCCOPY);
		pbPolylineUnit->DrawMultiPolyline(memDC,pt,MultiptsPoint[pInput->dwID],MultiInfoArray[pInput->dwID],false);
		BitBlt(temDC->m_hDC,0,0,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
		FinishMultiPolyline(pInput->dwID,pt);
		PageChanged();//事件触发
		break;
	}
	MultiptsPoint[pInput->dwID].clear();
	MultiptsPoint.erase(pInput->dwID);
	MultiDataInfo[pInput->dwID].AllRate.clear();
	MultiDataInfo.erase(pInput->dwID);
	MultiInfoArray.erase(pInput->dwID);
    //DisplayConsole("3",sizeof("3"));
		
end:    return true;
}

void CSCTDrawActivexCtrl::AddMultiPenBrush(UINT mouseid,CPoint point) //添加多点毛笔功能
{
	temSavePoint addPoint;
	addPoint.point=point;
	addPoint.preWidth=m_penWidth;
	addPoint.alpha=200;
	std::vector<temSavePoint> AddtemSavePoint;
	AddtemSavePoint.push_back(addPoint);
	MultiptsPoint.insert(std::map<UINT,std::vector<temSavePoint>>::value_type(mouseid,AddtemSavePoint));

	DataInfo AddDataInfo;
	AddDataInfo.OperationKind=6;
	AddDataInfo.xScale=1;
	AddDataInfo.yScale=1;
	AddDataInfo.IsLocked=false;
	AddDataInfo.IsCombination=false;
	AddDataInfo.IsErased=false;
	AddDataInfo.IsDisjoint=false;
	AddDataInfo.StartPoint=point;
	AddDataInfo.penColor=m_penColor;
	AddDataInfo.penWidth=m_penWidth;
	//AddDataInfo.SelectColor=SelectColor;
	ZeroMemory(AddDataInfo.Reserved0,200);
	ZeroMemory(AddDataInfo.Reserved1,200);
	ZeroMemory(AddDataInfo.Reserved2,200);
	ZeroMemory(AddDataInfo.Reserved3,200);
	ZeroMemory(AddDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));//代表已擦除区域
	Graphics graphics(hDC);
	AddDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	MultiDataInfo.insert(std::map<UINT,DataInfo>::value_type(mouseid,AddDataInfo));
		

	MultiInfo AddMultiInfo;
	AddMultiInfo.prelen=0;
	AddMultiInfo.StartPoint=point;
	AddMultiInfo.penColor=m_penColor;
	AddMultiInfo.penWidth=m_penWidth;
	AddMultiInfo.timer=GetTickCount();
	MultiInfoArray.insert(std::map<UINT,MultiInfo>::value_type(mouseid,AddMultiInfo));
}

void CSCTDrawActivexCtrl::FinishMultiPenBrush(UINT mouseid,CPoint point) //完成多点毛笔书写时用到
{
	CPoint sPoint;
	float xlen,ylen;
	sPoint=MultiDataInfo[mouseid].StartPoint;
	MultiDataInfo[mouseid].preAngle=0;
	MultiDataInfo[mouseid].RotateAngle=0;
	xlen=MultiDataInfo[mouseid].EndPoint.x-sPoint.x;
	ylen=MultiDataInfo[mouseid].EndPoint.y-sPoint.y;
	if(MultiptsPoint[mouseid].size()==0)
	{
		//SelectColor--;	
		goto ret;
	}
	//SelectColor++;
	if(xlen<1)
		xlen=1;
	if(ylen<1)
		ylen=1;
	UINT ptNum=0;
	int Size=MultiptsPoint[mouseid].size();
	for(int Index=0;Index<Size;Index++)
	{
		RelRate addRate;
		addRate.xRate=(MultiptsPoint[mouseid][Index].point.x-sPoint.x)/xlen;
		addRate.yRate=(MultiptsPoint[mouseid][Index].point.y-sPoint.y)/ylen;
		addRate.alpha=MultiptsPoint[mouseid][Index].alpha;
		addRate.preWidth=MultiptsPoint[mouseid][Index].preWidth;
		MultiDataInfo[mouseid].AllRate.push_back(addRate);
		ptNum++;
	}
	MultiDataInfo[mouseid].PointNum=ptNum;
	MultiDataInfo[mouseid].CenterPoint.x=(MultiDataInfo[mouseid].StartPoint.x+MultiDataInfo[mouseid].EndPoint.x)/2;
	MultiDataInfo[mouseid].CenterPoint.y=(MultiDataInfo[mouseid].StartPoint.y+MultiDataInfo[mouseid].EndPoint.y)/2;
	if(MultiDataInfo[mouseid].StartPoint!=MultiDataInfo[mouseid].EndPoint)
	{
		MultiDataInfo[mouseid].SelectColor = DataArray.size();
		DataArray.push_back(MultiDataInfo[mouseid]);
		UndoArray.push_back(MultiDataInfo[mouseid]);
			
		RedoArray.clear();
		SelObjList.clear();
		SelObj AddSelObj;
		AddSelObj.Index=DataArray.size()-1;
		AddSelObj.SelDataInfo=pDataInfo;
		SelObjList.push_back(AddSelObj);
	}
ret:    MultiptsPoint[mouseid].clear();
	MultiptsPoint.erase(mouseid);
	MultiDataInfo[mouseid].AllRate.clear();
	MultiDataInfo.erase(mouseid);
	MultiInfoArray.erase(mouseid);
}

void CSCTDrawActivexCtrl::AddMultiPolyline(UINT mouseid,CPoint point) //多点画笔
{
	temSavePoint addPoint;
	addPoint.point=point;
	addPoint.preWidth=m_penWidth;
	std::vector<temSavePoint> AddtemSavePoint;
	AddtemSavePoint.push_back(addPoint);
	MultiptsPoint.insert(std::map<UINT,std::vector<temSavePoint>>::value_type(mouseid,AddtemSavePoint));

	DataInfo AddDataInfo;
	AddDataInfo.OperationKind=7;
	AddDataInfo.Alpha=pbPolylineUnit->Alpha;
	AddDataInfo.xScale=1;
	AddDataInfo.yScale=1;
	AddDataInfo.IsLocked=false;
	AddDataInfo.IsCombination=false;
	AddDataInfo.IsErased=false;
	AddDataInfo.IsDisjoint=false;
	AddDataInfo.StartPoint=point;
	AddDataInfo.penColor=m_penColor;
	AddDataInfo.penWidth=m_penWidth;
	//AddDataInfo.SelectColor=SelectColor;
	AddDataInfo.LineStyle=pbPolylineUnit->LineStyle;
	if(pbPolylineUnit->LineStyle==RainbowStyle)
	{
		AddDataInfo.penColor=pbPolylineUnit->cols[0];
		AddDataInfo.FillColor=pbPolylineUnit->cols[1];
		AddDataInfo.FontColor=pbPolylineUnit->cols[2];
		AddDataInfo.foreColor=pbPolylineUnit->cols[3];
	}
	if(pbPolylineUnit->LineStyle==TextureStyle)
	{
		AddDataInfo.ImageSize=pbPolylineUnit->ImageSize;
		IStream* pIStream= NULL;
		CreateStreamOnHGlobal(pbPolylineUnit->ImageByte,TRUE,&pIStream);

		HGLOBAL hGlobal=::GlobalAlloc(GMEM_FIXED,pbPolylineUnit->ImageSize);
		AddDataInfo.ImageByte=(BYTE*)::GlobalLock(hGlobal);
		while(pIStream->Read(AddDataInfo.ImageByte,AddDataInfo.ImageSize,NULL)!=S_OK);
		GlobalUnlock(hGlobal);
	}
	ZeroMemory(AddDataInfo.Reserved0,200);
	ZeroMemory(AddDataInfo.Reserved1,200);
	ZeroMemory(AddDataInfo.Reserved2,200);
	ZeroMemory(AddDataInfo.Reserved3,200);
	ZeroMemory(AddDataInfo.Reserved4,1024);
	Region Rgn(Rect(0,0,0,0));
	Graphics graphics(hDC);
	AddDataInfo.hRgn=Rgn.GetHRGN(&graphics);
	MultiDataInfo.insert(std::map<UINT,DataInfo>::value_type(mouseid,AddDataInfo));
		

	MultiInfo AddMultiInfo;
	AddMultiInfo.prelen=0;
	AddMultiInfo.StartPoint=point;
	AddMultiInfo.penColor=m_penColor;
	AddMultiInfo.penWidth=m_penWidth;
	AddMultiInfo.Alpha=pbPolylineUnit->Alpha;
	AddMultiInfo.timer=GetTickCount();
	Region rgn(RectF(0,0,0,0));
	AddMultiInfo.pURgn=rgn.Clone();
	MultiInfoArray.insert(std::map<UINT,MultiInfo>::value_type(mouseid,AddMultiInfo));
}

void CSCTDrawActivexCtrl::FinishMultiPolyline(UINT mouseid,CPoint point) //完成多点画笔功能
{
	CPoint sPoint;
	float xlen,ylen;
	sPoint=MultiDataInfo[mouseid].StartPoint;
	MultiDataInfo[mouseid].preAngle=0;
	MultiDataInfo[mouseid].RotateAngle=0;
	xlen=MultiDataInfo[mouseid].EndPoint.x-sPoint.x;
	ylen=MultiDataInfo[mouseid].EndPoint.y-sPoint.y;
	if(xlen==0)
		xlen=1;
	if(ylen==0)
		ylen=1;	
	int Size=MultiptsPoint[mouseid].size();
	UINT ptNum=0;
	for(int Index=0;Index<Size;Index++)
	{
		RelRate addRate;
		addRate.xRate=(MultiptsPoint[mouseid][Index].point.x-sPoint.x)/xlen;
		addRate.yRate=(MultiptsPoint[mouseid][Index].point.y-sPoint.y)/ylen;
		MultiDataInfo[mouseid].AllRate.push_back(addRate);
		ptNum++;
	}
	//SelectColor++;
	MultiptsPoint[mouseid].clear();
	MultiptsPoint.erase(mouseid);
	MultiDataInfo[mouseid].PointNum=ptNum;
	MultiDataInfo[mouseid].CenterPoint.x=(MultiDataInfo[mouseid].StartPoint.x+MultiDataInfo[mouseid].EndPoint.x)/2;
	MultiDataInfo[mouseid].CenterPoint.y=(MultiDataInfo[mouseid].StartPoint.y+MultiDataInfo[mouseid].EndPoint.y)/2;
	if(MultiDataInfo[mouseid].StartPoint != MultiDataInfo[mouseid].EndPoint)
	{
		MultiDataInfo[mouseid].SelectColor = DataArray.size();
		DataArray.push_back(MultiDataInfo[mouseid]);
		UndoArray.push_back(MultiDataInfo[mouseid]);
			
		RedoArray.clear();
		SelObjList.clear();
		
	}
	MultiDataInfo[mouseid].AllRate.clear();
	MultiDataInfo.erase(mouseid);
	SelObj AddSelObj;
	AddSelObj.Index=DataArray.size()-1;
	AddSelObj.SelDataInfo=pDataInfo;
	SelObjList.push_back(AddSelObj);
	//MultiInfoArray[mouseid].pURgn->MakeEmpty();
	MultiInfoArray[mouseid].pURgn=NULL;
	MultiInfoArray.erase(mouseid);
}

void CSCTDrawActivexCtrl::InitErasureImage(void)//做对象擦除时通过此处来做擦除的纹理背景
{
	//DisplayConsole("o",sizeof("o"));
	BitBlt(temDC->m_hDC,WindowRect.left,WindowRect.top,WindowRect.Width(),WindowRect.Height(),memDC->m_hDC,0,0,SRCCOPY);
	//DisplayConsole("p",sizeof("p"));
	Graphics mGraphics(memDC->m_hDC);
	//DisplayConsole("q",sizeof("q"));
	memDC->FillSolidRect(WindowRect,m_bkColor);
	//DisplayConsole("r",sizeof("r"));
		
	//DisplayConsole("s",sizeof("s"));
	//if(pbEraseUnit->pErasureImage=NULL)
	if(bkImageArray.size()!=0)
	{
		SetBackGroundBmp();
		//delete pbEraseUnit->pErasureImage;
		//pbEraseUnit->pErasureImage=NULL;
	}
	CBitmap *pBmp=memDC->GetCurrentBitmap();
	pbEraseUnit->pErasureImage=new Bitmap((HBITMAP)pBmp->GetSafeHandle(),NULL);
	////////////////////////////////////////////////////////////////////////////////
	BitBlt(memDC->m_hDC,WindowRect.left,WindowRect.top,WindowRect.Width(),WindowRect.Height(),temDC->m_hDC,0,0,SRCCOPY);
	//isfinish = true;
}


BSTR CSCTDrawActivexCtrl::GetSelObjText(void) //文字工具选择时会用到
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult="";

	// TODO: Add your dispatch handler code here
	int Size=SelObjList.size();
	if(Size==1)
	{
		DataInfo GetDataInfo=SelObjList[0].SelDataInfo;
		strResult=GetDataInfo.SaveStr;
	}
	return strResult.AllocSysString();
}


void CSCTDrawActivexCtrl::SetPolylineStyle(USHORT nStyle) //设置具体的笔型（钢笔，荧光笔，纹理笔，铅笔）
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	pbPolylineUnit->LineStyle=nStyle;
}


void CSCTDrawActivexCtrl::SetPolylineColors(OLE_COLOR col1, OLE_COLOR col2, OLE_COLOR col3, OLE_COLOR col4) //设置笔的颜色
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	pbPolylineUnit->cols[0]=col1;
	pbPolylineUnit->cols[1]=col2;
	pbPolylineUnit->cols[2]=col3;
	pbPolylineUnit->cols[3]=col4;
}


void CSCTDrawActivexCtrl::AddPolylineImage(LPCTSTR ImagePath)//添加纹理笔的图片背景时要用到
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	CFile ImageFile;
	if(ImageFile.Open(ImagePath,CFile::modeRead|CFile::typeBinary))
	{
		if(pbPolylineUnit->ImageByte!=NULL)
			GlobalFree(pbPolylineUnit->ImageByte);
		pbPolylineUnit->ImageSize=ImageFile.GetLength(); 
		HGLOBAL hGlobal=::GlobalAlloc(GMEM_FIXED,pbPolylineUnit->ImageSize);
		pbPolylineUnit->ImageByte=(BYTE*)::GlobalLock(hGlobal);
		ImageFile.Read(pbPolylineUnit->ImageByte,pbPolylineUnit->ImageSize);  
		GlobalUnlock(hGlobal);	
		ImageFile.Close();
	}
}


void CSCTDrawActivexCtrl::SetLineKind(USHORT nStyle)//设置画线的类型
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	pbLineUnit->LineStyle=nStyle;
}


void CSCTDrawActivexCtrl::SetMultiTouch(VARIANT_BOOL flag)//设置是否开启多点功能
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	MultiTouchMode=flag;
}


VARIANT_BOOL CSCTDrawActivexCtrl::GetMultiTouchMode(void)//得到当前是否是多点模式
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return MultiTouchMode;
}


VARIANT_BOOL CSCTDrawActivexCtrl::InsertDigiMemoObjs(LPCTSTR FilePath)//插入数码记事簿
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	std::vector<std::vector<temSavePoint>> ppsPoint;
	std::vector<START_END_POINT> pt_Start_End;

	CDigiMemoUnit DigiMemo(WindowRect.Width(),WindowRect.Height());
	if(!DigiMemo.ReadDigiMemoFile(FilePath,ppsPoint,pt_Start_End))
		return VARIANT_FALSE;

	ptsPoint.clear();
	for(int i=0;i<ppsPoint.size();i++)
	{
		AddPolyline(ppsPoint[i][0].point);
		ptsPoint=ppsPoint[i];
		ptsPoint.insert(ptsPoint.begin(),ppsPoint[i][0]);
		pDataInfo.StartPoint=pt_Start_End[i].ptStart;
		pDataInfo.EndPoint=pt_Start_End[i].ptEnd;
		pDataInfo.penWidth=2;
		FinishPolyline(ppsPoint[i][ppsPoint[i].size()-1].point);
		ppsPoint[i].clear();
		ptsPoint.clear();
	}
	ppsPoint.clear();
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	return VARIANT_TRUE;
}


VARIANT_BOOL CSCTDrawActivexCtrl::CreateNewFile(void)//新建一个白板文件
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	ClearData();
	return VARIANT_TRUE;
}


void CSCTDrawActivexCtrl::DisplayConsole(char *info,int size)
{
	WriteConsole(hConsole,info,size,NULL,NULL);
}



BOOL CSCTDrawActivexCtrl::SearchUsbDevice(USHORT VendorID)
{
	bool LastDevice = false;
	bool MyDeviceDetected = false;
	DWORD MemberIndex = 0;
	DWORD Length;
	DWORD Required;
	BOOLEAN result;
	HDEVINFO hDevInfo;
	HANDLE DeviceHandle;
	GUID HidGuid;
	HIDD_ATTRIBUTES                Attributes;
	SP_DEVICE_INTERFACE_DATA       devInfoData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;

	HidD_GetHidGuid(&HidGuid);
	hDevInfo = SetupDiGetClassDevs(&HidGuid,NULL,NULL,DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
	devInfoData.cbSize = sizeof(devInfoData);
	do
	{
		result = SetupDiEnumDeviceInterfaces(hDevInfo,
			0,
			&HidGuid,
			MemberIndex,
			&devInfoData);
		if(result)
		{
			result = SetupDiGetDeviceInterfaceDetail(hDevInfo,
				&devInfoData,
				NULL,
				0,
				&Length,
				NULL);
			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			result = SetupDiGetDeviceInterfaceDetail(hDevInfo,
				&devInfoData,
				detailData,
				Length,
				&Required,
				NULL);
			DeviceHandle = CreateFile(detailData->DevicePath,
				GENERIC_READ|GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING,
				0,
				NULL);
			Attributes.Size = sizeof(Attributes);
			result = HidD_GetAttributes(DeviceHandle,&Attributes);
			if(Attributes.VendorID == VendorID)
			{
				MyDeviceDetected = TRUE;

			}
			else
			{
				CloseHandle(DeviceHandle);
			}
			free(detailData);
		}
		else
		{
			LastDevice = true;
		}
		MemberIndex++;
	}while(!LastDevice && !MyDeviceDetected);
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return MyDeviceDetected;
}


void CSCTDrawActivexCtrl::OngestureChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here

	SetModifiedFlag();
}


LRESULT CSCTDrawActivexCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_INPUT)
	{
		UINT dwSize;
		
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

		LPBYTE pData = new BYTE[dwSize];

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pData, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
		{
			RAWINPUT *pInput = (RAWINPUT*)pData;
			
			if (pInput->header.dwType == RIM_TYPEHID)
			{
				int bufsizw = pInput->data.hid.dwSizeHid;
				int height,width;
					//if (bufsizw == 38)
				//{
					BYTE buf[38];
					memcpy_s(buf, 38, (BYTE*)pInput->data.hid.bRawData, 38);
					m_strInput.Format(_T("%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x_%x"),
						buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
						buf[8], buf[9], buf[10], buf[11], buf[12], buf[13],buf[14], buf[15], buf[16], buf[17], buf[18], buf[19], buf[20], buf[21],
						buf[22], buf[23], buf[24], buf[25], buf[26], buf[27],buf[28],buf[29],buf[30],buf[31],buf[32],buf[33],buf[34],buf[35],buf[36],buf[37]);
					height = buf[3];
					width = buf[4];
					if((buf[1]==0x90) && (buf[5]==0x01))
					{
						eraserUp = false;
						isgestureDown = true;
						m_operation1 = m_operations;
						m_operations = 15;
						EraserWidth =height<width?height:width ;
						DisplayConsole("9",sizeof("9"));
					}
					else if((buf[1]==0x90) && (buf[5]==0x00))
					{
						DisplayConsole("8",sizeof("8"));
						eraserUp = true;
						isMultitouch = false;
						isgestureDown = false;
						//lbdown = true;
						isSaved = false;
						m_operations = m_operation1;
						//active->m_operations = 15;
						if(m_operation1 == 8)
							OnOperationsChanged();	
					}
					UpdateData(FALSE);
				//}
			}
		}

		delete[] pData;
	}
	else if (WM_TOUCH == message)
	{
		if (11 == m_operations)
		{
			OnGesture(wParam, lParam);
		}	
		return 0;
	}
	return COleControl::WindowProc(message, wParam, lParam);
		
}

void CSCTDrawActivexCtrl::RegistMouse()
{
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x06;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = this->m_hWnd;
	if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
		AfxMessageBox(_T("regist input failed!"));
}

void CSCTDrawActivexCtrl::OnGesture(WPARAM wParam, LPARAM lParam)
{
	POINT ptInputs;
	int iNumContacts = LOWORD(wParam);
	HTOUCHINPUT hInput = (HTOUCHINPUT)lParam;
	PTOUCHINPUT pInputs = new (std::nothrow) TOUCHINPUT[iNumContacts];
	if(pInputs != NULL)
	{
		if(GetTouchInputInfo(hInput, iNumContacts, pInputs, sizeof(TOUCHINPUT)))
		{
			for(int i = 0; i < iNumContacts; i++)
			{
				ptInputs.x = pInputs[i].x/100;	
				ptInputs.y = pInputs[i].y/100;
				//::ScreenToClient(g_hWnd, &ptInputs);
				pInputs[i].x = ptInputs.x;
				pInputs[i].y = ptInputs.y;
				ProcessInputEvent(&pInputs[i]);
			}
			ProcessChanges();
		}
	}

	delete [] pInputs;
	CloseTouchInputHandle(hInput);
}

void CSCTDrawActivexCtrl::ProcessInputEvent(const TOUCHINPUT* inData)
{
	DWORD dwCursorID = inData->dwID;
	DWORD dwEvent = inData->dwFlags;

	BOOL success = TRUE;

	// Check if contacts should be incremented
	if((dwEvent & TOUCHEVENTF_DOWN) && (dwCursorID != MOUSE_CURSOR_ID))
	{
		m_uNumContacts++;
	}

	// Screen the types of inputs and the number of contacts
	if((m_uNumContacts == 0) && (dwCursorID != MOUSE_CURSOR_ID))
	{
		return;
	}
	else if((m_uNumContacts > 0) && (dwCursorID == MOUSE_CURSOR_ID))
	{
		return;
	}

	// Check if contacts should be decremented
	if((dwEvent & TOUCHEVENTF_UP) && (dwCursorID != MOUSE_CURSOR_ID))
	{
		m_uNumContacts--;
	}    

	// Find the object and associate the cursor id with the object
	if(dwEvent & TOUCHEVENTF_DOWN)
	{
		DownEvent(m_object, inData);
	}
	else if(dwEvent & TOUCHEVENTF_MOVE)
	{
 		MoveEvent(m_object, inData);
	}
	else if(dwEvent & TOUCHEVENTF_UP)
	{
 		UpEvent(m_object, inData);
	}
}

void CSCTDrawActivexCtrl::DownEvent(CCoreObject* coRef, const TOUCHINPUT* inData)
{
	DWORD dwCursorID = inData->dwID;
	DWORD dwPTime = inData->dwTime;
	int x = inData->x;
	int y = inData->y;
	BOOL success = TRUE;
	CPoint StartPoint, EndPoint;

	std::vector<SelObj>::iterator iter = SelObjList.begin();
	if (iter != SelObjList.end())
	{
		StartPoint = iter->SelDataInfo.StartPoint;
		EndPoint = iter->SelDataInfo.EndPoint;
	}
	else
	{
		m_bInObject = FALSE;
		return;
	}
	
	//TODO:
	if(/*x > StartPoint.x && x < EndPoint.x && y > StartPoint.y && y < EndPoint.y*/1)
	{
		// Feed values to the Manipulation Processor
		success = SUCCEEDED(coRef->manipulationProc->ProcessDownWithTime(dwCursorID, (FLOAT)x, (FLOAT)y, dwPTime));
		if (!success)
		{
			coRef->manipulationProc->CompleteManipulation();
			m_bInObject = FALSE;
			return;
		}
		m_bInObject = TRUE;
	}
}

void CSCTDrawActivexCtrl::MoveEvent(CCoreObject* coRef, const TOUCHINPUT* inData)
{
	DWORD dwCursorID  = inData->dwID;
	DWORD dwPTime = inData->dwTime;
	int x = (inData->x);
	int y = (inData->y);

	if (!m_bInObject)
	{
		return;
	}

	coRef->manipulationProc->ProcessMoveWithTime(dwCursorID, (FLOAT)x, (FLOAT)y, dwPTime);
	float dx    = coRef->manipulationEventSink->dx;
	float dy    = coRef->manipulationEventSink->dy;
	float Angle = coRef->manipulationEventSink->Angle;
	float Scale = coRef->manipulationEventSink->Scale;

	DataInfo GetDataInfo;
	int GetIndex;
	std::vector<SelObj>::iterator iter = SelObjList.begin();
	if (iter != SelObjList.end())
	{
		GetDataInfo = iter->SelDataInfo;
		GetIndex    = iter->Index;
	}
	else
	{
		return;
	}
	
	GetDataInfo.CenterPoint.x += dx;
	GetDataInfo.CenterPoint.y += dy;

	GetDataInfo.RotateAngle=GetDataInfo.RotateAngle+Angle;
	GetDataInfo.RotateAngle=fmod(GetDataInfo.RotateAngle,360);
	GetDataInfo.xScale *= Scale;
	GetDataInfo.yScale *= Scale;

	GetDataInfo.xScale = (GetDataInfo.xScale > 2) ? 2 : GetDataInfo.xScale;
	GetDataInfo.yScale = (GetDataInfo.yScale > 2) ? 2 : GetDataInfo.yScale;

	GetDataInfo.xScale = (GetDataInfo.xScale < 0.5) ? 0.5 : GetDataInfo.xScale;
	GetDataInfo.yScale = (GetDataInfo.yScale < 0.5) ? 0.5 : GetDataInfo.yScale;

	double len=sqrt(float(Rel_StartPoint.x*Rel_StartPoint.x)+float(Rel_StartPoint.y*Rel_StartPoint.y));
	Angle=atan2(float(Rel_StartPoint.y),float(Rel_StartPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.StartPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.StartPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);
	len=sqrt(float(Rel_EndPoint.x*Rel_EndPoint.x)+float(Rel_EndPoint.y*Rel_EndPoint.y));
	Angle=atan2(float(Rel_EndPoint.y),float(Rel_EndPoint.x))+GetDataInfo.RotateAngle/Rate;
	GetDataInfo.EndPoint.x=GetDataInfo.CenterPoint.x+len*cos(Angle);
	GetDataInfo.EndPoint.y=GetDataInfo.CenterPoint.y+len*sin(Angle);

	SelObjList[0].SelDataInfo=GetDataInfo;
	DataArray[GetIndex]=GetDataInfo;

	//////////////////////////
	memDC->FillSolidRect(0,0,WindowRect.Width(),WindowRect.Height(),m_bkColor);
	SetBackGroundBmp();
	ReDraw();
	SelectClipRgn(memDC->m_hDC,NULL);
	BitBlt(hDC,0,0,WindowRect.right,WindowRect.bottom,memDC->m_hDC,0,0,SRCCOPY);
	////////////////////////////////////////////
	UndoDataInfo=GetDataInfo;		
}

void CSCTDrawActivexCtrl::UpEvent(CCoreObject* coRef, const TOUCHINPUT* inData)
{
	DWORD dwCursorID  = inData->dwID;
	DWORD dwPTime = inData->dwTime;
	int x = (inData->x);
	int y = (inData->y);

	if (!m_bInObject)
	{
		return;
	}

	coRef->manipulationProc->ProcessUpWithTime(dwCursorID, (FLOAT)x, (FLOAT)y, dwPTime);
	m_bInObject = FALSE;
}

void CSCTDrawActivexCtrl::ProcessChanges()
{

}
