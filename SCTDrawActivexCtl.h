#if !defined(AFX_SCTDRAWACTIVEXCTL_H__C9B8E2A0_D87D_4C26_8DAD_9500D2AF53B1__INCLUDED_)
#define AFX_SCTDRAWACTIVEXCTL_H__C9B8E2A0_D87D_4C26_8DAD_9500D2AF53B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCTDrawActivexCtl.h : Declaration of the CSCTDrawActivexCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexCtrl : See SCTDrawActivexCtl.cpp for implementation.
#include "Library.h"
#include "zlib.h"
#include "zconf.h"
#include "CoreObject.h"

#include <winuser.h>
//////////////////////////////////////////
#define MOUSE_CURSOR_ID 0

std::vector<CPoint> ptSector;
typedef struct bkImageInfo
{
	float xScale;
	float yScale;
	BYTE *ImageByte;
	int ImageSize;
	CPoint ptImage;//背影图片的左上点
	int Width;
	int Height;
}bkImageInfo;

typedef struct SelObj
{
	DataInfo SelDataInfo;
	int Index;
}SelObj;

typedef struct PageInfo
{
	std::vector<DataInfo> DataArray;
	std::vector<bkImageInfo> bkImageArray;
	CRect DisplayRect;
	HBITMAP hMemBmp;
	short Operations;
	short PenWidth;
	COLORREF PenColor;
	COLORREF FillColor;
	COLORREF FontColor;
	COLORREF bkColor;
	short LineStyle;
	short FillStyle;
	COLORREF ForeColor;
}PageInfo;

//////////////////////////////////////////
class CSCTDrawActivexCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSCTDrawActivexCtrl)

	// Constructor
public:
	CSCTDrawActivexCtrl();
public:
	CLSID BmpClsid;
	BOOL lbdown1;
	BOOL MultiTouchMode;
	int PlayPageIndex;
	RectF RotateRgn;
	RectF StretchRgn;
	RectF MenuRgn;
	RectF MoveRgn;
	RectF PlayRgn;
	CRect FocusRect;
	CRect ComObjRect;//组合操作时用
	CRect ComRect;//ComObjRect的复制，多选时用
	ULONG_PTR m_GdiplusToken;
	CRect WindowRect;
	HRGN ehRgn;
	BOOL lbdown;
	int PageNum;//现在正在编辑的页面号
	int ObjectIndex;//正在操作的对象索引
	CPoint Rel_StartPoint;//用于旋转及拉伸操作
	CPoint Rel_EndPoint;
	CPoint Rel_CenterPoint;
	DataInfo UndoDataInfo;//用于撤消操作时暂存数据
	///////执行相应操作//////////////
	CBaseUnit *pbBaseUnit;
	CBaseUnit *pbUnit;
	CLineUnit *pbLineUnit;
	CRectUnit *pbRectUnit;
	CRoundRectUnit *pbRoundRectUnit;
	CEllipseUnit *pbEllipseUnit;
	CArcUnit *pbArcUnit;
	CSectorUnit *pbSectorUnit;
	CPolylineUnit *pbPolylineUnit;
	CPenBrushUnit *pbPenBrushUnit;
	CTextUnit *pbTextUnit;
	CBitmapUnit *pbBmpUnit;
	CCombination *pbCombination;
	CEraseUnit *pbEraseUnit;
	CDisjointUnit  *pbDisjoint;
	CSprayBottleUnit SprayBottleUnit;
	CPolygonUnit *pbPolygonUnit;
	CRgnObjUnit *pbRgnObjUnit;
	BOOL BmpFromHandle;
	//////////////数据信息存储////////////////
	BYTE  *ErasedRgnData;//本次擦除区域
	UINT ErasedRgnSize;
	long SelectColor;
	/////////////////////////////////////
	CDC *memDC;
	CDC *xDC;
	HDC hDC;   //xDC->GetSafeHDC();控件在bitblt中使用
	CDC *temDC;
	CBitmap temBmp;
	CBitmap memBmp;
	CPoint temPoint;
	CPoint textPoint;


	DataInfo pDataInfo;
	DataInfo TransferDataInfo;
	int preOperations;
	COLORREF bkColor;
	CRichEditCtrl *pEdit;
	///////////////////
	CPoint Relative_cPoint;
	CPoint Relative_sPoint;
	CPoint Relative_ePoint;
	///////////////////////////
	CRect DisplayRect;
	std::vector<PageInfo> PageArray;
	std::vector<DataInfo> DataArray;
	std::vector<temSavePoint> ptsPoint;
	std::vector<RelRate> tRelRate;
	std::vector<bkImageInfo> bkImageArray;//背景图片组
	std::vector<SelObj> SelObjList;
	std::vector<DataInfo> UndoArray;//撤销数组
	std::vector<DataInfo> RedoArray;//重绘数组
	std::vector<DataInfo> CopyArray;//复制对像保存数组
	std::vector<PageInfo> CopyPageArray;//复制页保存数组
	////////////////////////
	//int MouseInfo;
	int  PointNum;
	int EraserWidth;
	CString BmpName;
	HCURSOR MouseCur;
	BOOL ModifyText;
	char bkBmp[100];
	COLORREF SB_FillStyle;//喷管筒设置
	COLORREF SB_FillColor;
	COLORREF SB_ForeColor;
	BOOL TextReturn;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCTDrawActivexCtrl)
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DotEraseOperation();
	//}}AFX_VIRTUAL

	// Implementation
private:
	HGLRC hGLRC;
	int ChangeIndex;
	std::vector<RectF> ChangeRect;
	Image *ChangeImage;
	Image *StretchImage;
	Image *MenuImage;
	Image *RotateImage;
	Image *PlayImage;
	Image *HeadImage;
	Image *LeftImage;
	Image *RightImage;

	void ChangeText();
	void ChangeCharFormatToLogFont(CHARFORMAT cf,  LOGFONT &lf,  COLORREF &textcolor);
	void ChangeLogFontToCharFormat(CHARFORMAT &cf, const LOGFONT lf, const COLORREF textcolor);
	void ObjZoomOperation(int Index,float mul);
	void AddSelObj(CPoint point );
	BOOL InModifyForeColor(DataInfo tDataInfo, DWORD nColor);
	int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);
	
	BOOL InModifySelObjFont(int Index,Font *pfont);
	BOOL InModifyFillStyle(DataInfo tDataInfo,short nStyle);
	BOOL InModifyLineStyle(DataInfo tDataInfo,int Style);
	BOOL InModifyFontColor(DataInfo tDataInfo,OLE_COLOR nColor);
	BOOL InModifyLineColor(DataInfo tDataInfo,OLE_COLOR nColor);
	BOOL InModifyLineWidth(DataInfo tDataInfo,short nWidth);
	HCURSOR GetMouseCursor(CPoint point);
	BOOL ObjSelected();
	BOOL ObjsSelected();
	BOOL CreatePage();
	BOOL IsInitial;
	LOGFONT DefaultFont;
public:
	~CSCTDrawActivexCtrl();
protected:
	DECLARE_OLECREATE_EX(CSCTDrawActivexCtrl)    // Class factory and guidttrrtu
	DECLARE_OLETYPELIB(CSCTDrawActivexCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSCTDrawActivexCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSCTDrawActivexCtrl)		// Type name and misc status

	// Message maps
	//{{AFX_MSG(CSCTDrawActivexCtrl)
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChangeText();
	afx_msg void OnGesture(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Dispatch maps
	//{{AFX_DISPATCH(CSCTDrawActivexCtrl)
public:
	BOOL isMousetouch;//用于单点书写后，智能板擦保存依据
	BOOL isMultitouch;//用于多点书写后，智能板擦保存依据
	BOOL SearchUsbDevice(USHORT VendorID);
	short m_operation1;
	short m_operations;
	afx_msg void OnOperationsChanged();
	void OngestureChanged(void);
	SHORT m_gesture;
	short m_penWidth;
	afx_msg void OnPenWidthChanged();
	OLE_COLOR m_penColor;
	afx_msg void OnPenColorChanged();
	OLE_COLOR m_fillColor;
	afx_msg void OnFillColorChanged();
	OLE_COLOR m_fontColor;
	afx_msg void OnFontColorChanged();
	CString m_filePath;
	afx_msg void OnFilePathChanged();
	CString m_temBmpPath;
	afx_msg void OnTemBmpPathChanged();
	OLE_COLOR m_bkColor;
	afx_msg void OnBkColorChanged();
	short m_lineStyle;
	afx_msg void OnLineStyleChanged();
	short m_fillStyle;
	afx_msg void OnFillStyleChanged();
	OLE_COLOR m_foreColor;
	afx_msg void OnForeColorChanged();
	CString m_bkBmp;
	afx_msg void OnBkBmpChanged();
	afx_msg BSTR GetReservedProperty0();
	afx_msg void SetReservedProperty0(LPCTSTR lpszNewValue);
	afx_msg BSTR GetReservedProperty1();
	afx_msg void SetReservedProperty1(LPCTSTR lpszNewValue);
	afx_msg BSTR GetReservedProperty2();
	afx_msg void SetReservedProperty2(LPCTSTR lpszNewValue);
	afx_msg BSTR GetReservedProperty3();
	afx_msg void SetReservedProperty3(LPCTSTR lpszNewValue);
	afx_msg BSTR GetReservedProperty4();
	afx_msg void SetReservedProperty4(LPCTSTR lpszNewValue);
	afx_msg BSTR SetOperationEX(LPCTSTR Operations);
	afx_msg BSTR SetLineStyleEX(LPCTSTR LineStyle);
	afx_msg BSTR SetFillStyleEX(LPCTSTR FillStyle);
	afx_msg void CreateNewPage();
	afx_msg BOOL ModifyLineWidth(short nWidth);
	afx_msg BOOL ModifyLineColor(OLE_COLOR nColor);
	afx_msg BOOL ModifyFontColor(OLE_COLOR nColor);
	afx_msg BOOL AddPicture(OLE_HANDLE hBitmap, short Left, short Top, short Width, short Height);
	afx_msg BOOL AddPictureFromFile(LPCTSTR FilePath, short Left, short Top, short Width, short Height);
	afx_msg BOOL AddPictureFromHandle(OLE_HANDLE hBitmap);
	afx_msg short ShowPage(short PageID);
	afx_msg OLE_HANDLE GetPageBmp();
	afx_msg BOOL SetbkImageFromHandle(OLE_HANDLE hBitmap);
	afx_msg BOOL SetCombination();
	afx_msg BOOL SetDisjunction();
	afx_msg BOOL Undo();
	afx_msg BOOL Redo();
	afx_msg BOOL SelectAllObj();
	afx_msg BOOL ErasedRegion(short left, short top, short right, short bottom);
	afx_msg BOOL ZoomIn(float mul);
	afx_msg BOOL DeleteSelObj();
	afx_msg BOOL CopySelObj();
	afx_msg BOOL PasteObj();
	afx_msg BOOL LockSelObj(BOOL IsLocked);
	afx_msg BOOL DeletePage(short PageIndex);
	afx_msg BOOL GetSelObjIsLocked();
	afx_msg short GetSelObjStartPointX();
	afx_msg short GetSelObjStartPointY();
	afx_msg short GetSelObjEndPointX();
	afx_msg short GetSelObjEndPointY();
	afx_msg short GetSelObjCenterPointX();
	afx_msg short GetSelObjCenterPointY();
	afx_msg double GetSelObjRotateAngle();
	afx_msg short GetSelObjLineStyle();
	afx_msg short GetSelObjFillStyle();
	afx_msg short GetSelObjPenWidth();
	afx_msg long GetSelObjFillColor();
	afx_msg long GetSelObjFontColor();
	afx_msg long GetSelObjForeColor();
	afx_msg short GetPageNum();
	afx_msg BOOL CopyPage(short PageIndex);
	afx_msg BOOL PastPage(short PageIndex);
	afx_msg BOOL EraseAll();
	afx_msg long SaveDataToFile(LPCTSTR FilePath);
	afx_msg long ReadDataFromFile(LPCTSTR FilePath);
	afx_msg BOOL ModifyFillStyle(short nStyle);
	afx_msg BOOL ModifyLineStyle(short nStyle);
	afx_msg BOOL MoveUp();
	afx_msg BOOL MoveTop();
	afx_msg BOOL MoveDown();
	afx_msg BOOL MoveBottom();
	afx_msg BOOL ModifyFillColor(OLE_COLOR nColor);
	afx_msg BOOL ModifyForeColor(OLE_COLOR nForeColor);
	afx_msg BOOL DrawPolygon(VARIANT FAR* ptArray, short nSize);
	afx_msg BOOL DrawObj(short ObjType, short Left, short Top, short Right, short Bottom, float Angle);
	afx_msg BOOL SetSprayBottle(short FillStyle, OLE_COLOR FillColor, OLE_COLOR ForeColor);
	afx_msg short GetObjNum();
	afx_msg short GetSelObjType();
	afx_msg BSTR ReadReservedProperty(short PropertyIndex, short ObjIndex);
	afx_msg BOOL SetReservedProperty(short PropertyIndex, short ObjIndex, LPCTSTR Property);
	afx_msg short GetSelObjIndex();
	afx_msg BOOL SelectObjs(short FAR* Objs, short ObjsNum);
	afx_msg BOOL AddSelectObjs(short FAR* Objs, short ObjsNum);
	afx_msg BOOL UnSelectObjs(short FAR* Objs, short ObjsNum);
	afx_msg long GetSelObjPenColor();
	afx_msg BOOL ReplayPage(short PageIndex);
	afx_msg BOOL ModifySelObjFont(OLE_HANDLE hFont);
	afx_msg BOOL InitializeFont(OLE_HANDLE hFont);
	afx_msg OLE_HANDLE GetSelObjFont();
	afx_msg BOOL AddRgnObj(OLE_HANDLE hRgn);
	afx_msg OLE_HANDLE GetSelObjRegion(short SelObjIndex);
	afx_msg BOOL GetSelObjIsErased(short ObjIndex);
	afx_msg BOOL GetObjRect(short FAR* left, short FAR* top, short FAR* right, short FAR* bottom, short ObjIndex);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	// Event maps
	//{{AFX_EVENT(CSCTDrawActivexCtrl)
	void ClickMenuRegion()
	{FireEvent(eventidClickMenuRegion,EVENT_PARAM(VTS_NONE));}
	void DoneReplayPage()
	{FireEvent(eventidDoneReplayPage,EVENT_PARAM(VTS_NONE));}
	void ClickPlayRegion()
	{FireEvent(eventidClickPlayRegion,EVENT_PARAM(VTS_NONE));}
	void PageChanged()
	{FireEvent(eventidPageChanged,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

	// Dispatch and event IDs
public:
	
	void CaculateSelObjsRgn();
	BOOL InModifyFillColor(DataInfo tDataInfo, DWORD nColor);
	BOOL SavePage();
	void FinishPolygon(CPoint point);
	BOOL AddPolygon(CPoint point);
	void ClearData();
	void SetBackGroundBmp();
	void ObjectErase(CPoint point);
	void ClearDataInfo();
	void AddCombination();
	void EditBmp();
	void DoReDraw(CBaseUnit *pTemUnit,BOOL Erased);
	void ReDraw(BOOL Erased=false);
	void FinishBitmap(CPoint point);
	void AddBitmap(CPoint point);
	void RotateText();
	BOOL FinishText(CPoint point);
	void AddText(CPoint point);
	void FinishPenBrush(CPoint point);
	void AddPenBrush(CPoint point);
	void FinishPolyline(CPoint point);
	void AddPolyline(CPoint point);
	void FinishSector(CPoint point);
	void AddSector(CPoint point);
	void FinishArc(CPoint point);
	void AddArc(CPoint point);
	void FinishEllipse(CPoint point);
	void AddEllipse(CPoint point);
	void FinishRoundRect(CPoint point);
	void AddRoundRect(CPoint point);
	CPoint CaculatePoint(CPoint point,CPoint CenterPoint,double RotateAngle);
	void FinishRect(CPoint point);
	void AddRect(CPoint point);
	void FinishLine(CPoint point);
	void CaculateAngle(CPoint point);
	void RotateOperations(CPoint point);
	void MoveOperations(CPoint point);
	void StretchOperations(CPoint point);
	void SelectOperations(CPoint point );
	void AddLine(CPoint point);
	enum {
		dispidgesture = 119,
		dispidCreateNewFile = 118L,
		dispidInsertDigiMemoObjs = 117L,
		dispidGetMultiTouchMode = 116L,
		dispidSetMultiTouch = 115L,
		dispidSetLineKind= 114L,
		dispidAddPolylineImage = 113L,
		dispidSetPolylineColors = 112L,
		dispidSetPolylineStyle = 111L,
		dispidGetSelObjText = 110L,
		dispidSaveAsOtherTypes = 109L,
		dispidGetChartItembkColor = 108L,
		dispidGetChartItemHeadLine = 107L,
		dispidGetChartItemColor = 106L,
		dispidGetChartItemNumber = 105L,
		dispidGetChartItemName = 104L,
		dispidChangeSelChartData = 103L,
		eventidJudgeSelectedImage = 5L,
		dispidGetChartDataNum = 102L,
		dispidFinishChart = 101L,
		dispidAddChartData = 100L,
		dispidBeginChart = 99L,
		dispidSetAlphaProperty = 98L,
		dispidSaveObjToImage = 97L,
		dispidRefreshAll = 96L,
		dispidGetPageThumbnail = 95L,
		dispidStopPlay = 94L,
		dispidStartPlay = 93L,
		dispidChangePageIndex = 92L,
		dispidInsertPage = 91L,
		//{{AFX_DISP_ID(CSCTDrawActivexCtrl)
		dispidOperations = 1L,
		dispidPenWidth = 2L,
		dispidPenColor = 3L,
		dispidFillColor = 4L,
		dispidFontColor = 5L,
		dispidFilePath = 6L,
		dispidTemBmpPath = 7L,
		dispidBkColor = 8L,
		dispidLineStyle = 9L,
		dispidFillStyle = 10L,
		dispidForeColor = 11L,
		dispidBkBmp = 12L,
		dispidReservedProperty0 = 13L,
		dispidReservedProperty1 = 14L,
		dispidReservedProperty2 = 15L,
		dispidReservedProperty3 = 16L,
		dispidReservedProperty4 = 17L,
		dispidSetOperationEx = 18L,
		dispidSetLineStyleEx = 19L,
		dispidSetFillStyleEx = 20L,
		dispidCreateNewPage = 21L,
		dispidModifyLineWidth = 22L,
		dispidModifyLineColor = 23L,
		dispidModifyFontColor = 24L,
		dispidAddPicture = 25L,
		dispidAddPictureFromFile = 26L,
		dispidAddPictureFromHandle = 27L,
		dispidShowPage = 28L,
		dispidGetPageBmp = 29L,
		dispidSetbkImageFromHandle = 30L,
		dispidSetCombination = 31L,
		dispidSetDisjunction = 32L,
		dispidUndo = 33L,
		dispidRedo = 34L,
		dispidSelectAllObj = 35L,
		dispidErasedRegion = 36L,
		dispidZoomIn = 37L,
		dispidDeleteSelObj = 38L,
		dispidCopySelObj = 39L,
		dispidPasteObj = 40L,
		dispidLockSelObj = 41L,
		dispidDeletePage = 42L,
		dispidGetSelObjIsLocked = 43L,
		dispidGetSelObjStartPointX = 44L,
		dispidGetSelObjStartPointY = 45L,
		dispidGetSelObjEndPointX = 46L,
		dispidGetSelObjEndPointY = 47L,
		dispidGetSelObjCenterPointX = 48L,
		dispidGetSelObjCenterPointY = 49L,
		dispidGetSelObjRotateAngle = 50L,
		dispidGetSelObjLineStyle = 51L,
		dispidGetSelObjFillStyle = 52L,
		dispidGetSelObjPenWidth = 53L,
		dispidGetSelObjFillColor = 54L,
		dispidGetSelObjFontColor = 55L,
		dispidGetSelObjForeColor = 56L,
		dispidGetPageNum = 57L,
		dispidCopyPage = 58L,
		dispidPastPage = 59L,
		dispidEraseAll = 60L,
		dispidSaveDataToFile = 61L,
		dispidReadDataFromFile = 62L,
		dispidModifyFillStyle = 63L,
		dispidModifyLineStyle = 64L,
		dispidMoveUp = 65L,
		dispidMoveTop = 66L,
		dispidMoveDown = 67L,
		dispidMoveBottom = 68L,
		dispidModifyFillColor = 69L,
		dispidModifyForeColor = 70L,
		dispidDrawPolygon = 71L,
		dispidDrawObj = 72L,
		dispidSetSprayBottle = 73L,
		dispidGetObjNum = 74L,
		dispidGetSelObjType = 75L,
		dispidReadReservedProperty = 76L,
		dispidSetReservedProperty = 77L,
		dispidGetSelObjIndex = 78L,
		dispidSelectObjs = 79L,
		dispidAddSelectObjs = 80L,
		dispidUnSelectObjs = 81L,
		dispidGetSelObjPenColor = 82L,
		dispidReplayPage = 83L,
		dispidModifySelObjFont = 84L,
		dispidInitializeFont = 85L,
		dispidGetSelObjFont = 86L,
		dispidAddRgnObj = 87L,
		dispidGetSelObjRegion = 88L,
		dispidGetSelObjIsErased = 89L,
		dispidGetObjRect = 90L,
		eventidClickMenuRegion = 1L,
		eventidDoneReplayPage = 2L,
		eventidClickPlayRegion = 3L,
		eventidPageChanged = 4L,
		//}}AFX_DISP_ID
	};
protected:
	VARIANT_BOOL InsertPage(SHORT PageIndex);
	VARIANT_BOOL ChangePageIndex(SHORT FromPageIndex, SHORT ToPageIndex);
	VARIANT_BOOL StartPlay(SHORT PageIndex);
	VARIANT_BOOL StopPlay(void);
	OLE_HANDLE GetPageThumbnail(SHORT PageIndex);
	void RefreshAll(void);
	OLE_HANDLE SaveObjToImage();
private:
	void InCopySelObj(DataInfo &tDataInfo);
	void InPasteObj(int Start,DataInfo &tDataInfo);
protected:
	void SetAlphaProperty(USHORT Alpha);
private:
	void MoveDownObjs(int FromIndex, int ToIndex, DataInfo & GetDataInfo);
	void MoveUpObjs(int FromIndex, int ToIndex, DataInfo  & GetDataInfo);
	void GetOperationType(DataInfo GetDataInfo);
	void ChangeOperations(CPoint point);
	void FinishChangePolygon(void);
	bool InitOpengl(void);
protected:
	void BeginChart(LPCTSTR HeadLine, OLE_COLOR bkColor);
	void AddChartData(LPCTSTR Name, FLOAT Number, OLE_COLOR color);
	void FinishChart(SHORT Type);
private:
	void DrawPieChart(float StartAngle, float SweepAngle,float Red,float Green,float Blue,float rate,HFONT hfont);
public:
	IStream * SaveDataToBmp(void);
private:
	void AddPictureFromData(IStream * pIStream);
	void PieChart(int Size,float Count,HFONT hfont);
protected:
	SHORT GetChartDataNum(void);

	void JudgeSelectedImage(VARIANT_BOOL IsChart)
	{
		FireEvent(eventidJudgeSelectedImage, EVENT_PARAM(VTS_BOOL), IsChart);
	}
	void ChangeSelChartData(void);
private:
	void HistogramChart(int Size,float max, HFONT hfont);
	void DrawHistogram(float Red, float Green, float Blue,float height,float width,float center,float number,CString name, HFONT hfont);
protected:
	BSTR GetChartItemName(SHORT Index);
	FLOAT GetChartItemNumber(USHORT Index);
	OLE_COLOR GetChartItemColor(SHORT Index);
	BSTR GetChartItemHeadLine(void);
	OLE_COLOR GetChartItembkColor(void);
	SHORT SaveAsOtherTypes(SHORT PageID, LPCTSTR FilePath);
public:
	HANDLE hConsole;
	void DisplayConsole(char *info,int size);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void DrawCompass(void);
private:
	void TranslateImageToClipboard(DataInfo ImageInfo);
private:
	virtual BOOL OnTouchInput(CPoint pt, int nInputNumber , int nInputsCount , PTOUCHINPUT pInput);
	BOOL OnTouchInputDown(CPoint pt, PTOUCHINPUT pInput);
	BOOL OnTouchInputMove(CPoint pt, PTOUCHINPUT pInput);
	BOOL OnTouchInputUp(CPoint pt, PTOUCHINPUT pInput);
private:
	std::map<UINT,DataInfo> MultiDataInfo ;
public:
	std::map<UINT,std::vector<temSavePoint>> MultiptsPoint; 
	std::map<UINT,MultiInfo> MultiInfoArray;

	//std::map<UINT,GestureInfo> GestureInfoArray;
private:
	void AddMultiPenBrush(UINT mouseid,CPoint point);
	void FinishMultiPenBrush(UINT mouseid,CPoint point);
	void AddMultiPolyline(UINT mouseid,CPoint point);
	void FinishMultiPolyline(UINT mouseid,CPoint point);
public:
	void InitErasureImage(void);
	void RegistMouse();
	CString m_strInput;
protected:
	BSTR GetSelObjText(void);
	void SetPolylineStyle(USHORT nStyle);
	void SetPolylineColors(OLE_COLOR col1, OLE_COLOR col2, OLE_COLOR col3, OLE_COLOR col4);
	void AddPolylineImage(LPCTSTR ImagePath);
	void SetLineKind(USHORT nStyle);
	void SetMultiTouch(VARIANT_BOOL flag);
	VARIANT_BOOL GetMultiTouchMode(void);
	VARIANT_BOOL InsertDigiMemoObjs(LPCTSTR FilePath);
	VARIANT_BOOL CreateNewFile(void);
	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	void ProcessInputEvent(const TOUCHINPUT* inData);
	void ProcessChanges();
	void DownEvent(CCoreObject* coRef, const TOUCHINPUT* inData, BOOL* bFound);
	void MoveEvent(CCoreObject* coRef, const TOUCHINPUT* inData);
	void UpEvent(CCoreObject* coRef, const TOUCHINPUT* inData);


	CCoreObject* m_object;	
	IManipulationProcessor* manipulationProc;
	// Keeps track of the number contacts being processed
	unsigned int m_uNumContacts;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCTDRAWACTIVEXCTL_H__C9B8E2A0_D87D_4C26_8DAD_9500D2AF53B1__INCLUDED)
