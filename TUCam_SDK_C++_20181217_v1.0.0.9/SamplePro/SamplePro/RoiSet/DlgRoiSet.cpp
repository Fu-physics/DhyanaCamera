// DlgRoiSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRoiSet.h"

#include <windows.h>
#pragma   comment(lib,"Msimg32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRoiSet dialog


CDlgRoiSet::CDlgRoiSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRoiSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRoiSet)

//    SetProcessDPIAware();  // 防止截图自动缩放

	m_rectTracker.m_nStyle = CMyTracker::resizeMiddle|CMyTracker::dottedLine;  
	m_rectTracker.m_rect.SetRect(-1,-2,-3,-4);
	m_rectTracker.SetRectColor(RGB(255, 0, 50));
	m_rectTracker.SetResizeCursor(IDC_CURSOR6,IDC_CURSOR5,IDC_CURSOR4,IDC_CURSOR3,IDC_CURSOR2);
    m_hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR1);  
	
	m_bDraw		= FALSE;
	m_bFirstDraw= FALSE;
    m_startPt	= 0;
	m_xScreen	= GetSystemMetrics(SM_CXSCREEN);
	m_yScreen	= GetSystemMetrics(SM_CYSCREEN);

	m_PreviewRt = CRect(300, 100, 900, 600);
 	m_rectTracker.SetPreviewRect(m_PreviewRt);

// 	CRect rect(0, 0, m_xScreen, m_yScreen);
// 	m_pBitmap	= CBitmap::FromHandle(CopyScreenToBitmap(&rect));
// 	m_pBitmap1 = CBitmap::FromHandle(CopyScreenToBitmap(&rect, TRUE));
//     m_pBitmap2	= CBitmap::FromHandle(CopyScreenToBitmap(&rect, TRUE)); 
	//}}AFX_DATA_INIT
}

CDlgRoiSet::~CDlgRoiSet()
{
	m_MemDC.DeleteDC();
	m_tmpDC.DeleteDC();
	m_BacDC.DeleteDC();

	m_pBitmap->DeleteObject();
	m_pBitmap1->DeleteObject();
	m_pBitmap2->DeleteObject();
}

void CDlgRoiSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRoiSet)
	DDX_Control(pDX, IDC_BTN_OK, m_bmpBtnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_bmpBtnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRoiSet, CDialog)
	//{{AFX_MSG_MAP(CDlgRoiSet)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRoiSet message handlers

BOOL CDlgRoiSet::OnInitDialog()
{
	CDialog::OnInitDialog();

    SetWindowPos(&wndTopMost, 0, 0, m_xScreen, m_yScreen+2, NULL/*SWP_SHOWWINDOW*/);
 	ShowToolBar(FALSE);

	CRect rect(0, 0, m_xScreen, m_yScreen);
	m_pBitmap	= CBitmap::FromHandle(CopyScreenToBitmap(&rect));
	m_pBitmap1	= CBitmap::FromHandle(CopyScreenToBitmap(&rect, TRUE));
    m_pBitmap2	= CBitmap::FromHandle(CopyScreenToBitmap(&rect, TRUE)); 

	CClientDC dc(this);
	m_MemDC.CreateCompatibleDC(&dc);
	m_MemDC.SelectObject(m_pBitmap);
	m_tmpDC.CreateCompatibleDC(&dc);
	m_tmpDC.SelectObject(m_pBitmap1);
	m_BacDC.CreateCompatibleDC(&dc);
	m_BacDC.SelectObject(m_pBitmap2);
	
// 	m_bmpBtnOK.SetSkin(IDB_BTN_NORMAL, IDB_BTN_DOWN, IDB_BTN_MOVE, IDB_BTN_ENABLE, 0, 0, 0, 0);
// 	m_bmpBtnCancel.SetSkin(IDB_BTN_LONG_N, IDB_BTN_LONG_D, IDB_BTN_LONG_M, IDB_BTN_LONG_EN, 0, 0, 0, 0);

	return TRUE; 
}


void CDlgRoiSet::OnPaint() 
{
	CPaintDC dc(this);
	
	if(m_bFirstDraw)
	{
		CRect rect;
		m_rectTracker.GetTrueRect(&rect);
	}
	m_tmpDC.BitBlt(0, 0, m_xScreen, m_yScreen, &m_BacDC, 0, 0, SRCCOPY);

	m_tmpDC.BitBlt(m_rectTracker.m_rect.left, m_rectTracker.m_rect.top
			, m_rectTracker.m_rect.Width(), m_rectTracker.m_rect.Height(),&m_MemDC
			, m_rectTracker.m_rect.left, m_rectTracker.m_rect.top, SRCCOPY);
	dc.BitBlt(0, 0, m_xScreen, m_yScreen, &m_tmpDC, 0, 0, SRCCOPY);
	if(m_bFirstDraw)
	{
		m_rectTracker.Draw(&dc);
	}
	
	CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgRoiSet::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDlgRoiSet::OnOK() 
{
	CDialog::OnOK();
}


void CDlgRoiSet::OnCancel() 
{
	if(m_bFirstDraw)
	{
		m_bFirstDraw=FALSE;
		m_bDraw=FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		Invalidate();
	}
	else
	{
		CDialog::OnCancel();
	}
}


void CDlgRoiSet::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDraw)
	{
		if (abs(m_startPt.x-point.x)<25) 
		{
			if (m_startPt.x-point.x>0)
			{
				point.x = m_startPt.x - 25;
			}
			else
			{
				point.x = m_startPt.x + 25;
			}
		}
		
		if (abs(m_startPt.y-point.y)<25) 
		{
			if (m_startPt.y-point.y>0)
			{
				point.y = m_startPt.y - 25;
			}
			else
			{
				point.y = m_startPt.y + 25;
			}
		}

		if (point.x < m_PreviewRt.left)   point.x = m_PreviewRt.left;
		if (point.y < m_PreviewRt.top)    point.y = m_PreviewRt.top;
		if (point.x > m_PreviewRt.right)  point.x = m_PreviewRt.right;
		if (point.y > m_PreviewRt.bottom) point.y = m_PreviewRt.bottom;
		
		m_rectTracker.m_rect.SetRect(m_startPt.x+4, m_startPt.y+4, point.x, point.y);

		Invalidate();
	}
	SetFocus();
	
	CDialog::OnMouseMove(nFlags, point);
}


void CDlgRoiSet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ShowToolBar(FALSE);

	int nHitTest = m_rectTracker.HitTest(point);
	if (!PtInRect(&m_PreviewRt, point))
		return;

	if(nHitTest < 0)
	{
		if(!m_bFirstDraw)
		{
			m_startPt = point;
			m_bDraw = TRUE;
			m_bFirstDraw = TRUE;
			
			CRect rt(point.x-40,point.y-40,point.x+40,point.y+40);

			if (rt.left < m_PreviewRt.left)
			{
				rt.left = m_PreviewRt.left;
			}

			if (rt.right > m_PreviewRt.right)
			{
				rt.right = m_PreviewRt.right;
			}

			if (rt.top < m_PreviewRt.top)
			{
				rt.top = m_PreviewRt.top;
			}

			if (rt.bottom > m_PreviewRt.bottom)
			{
				rt.bottom = m_PreviewRt.bottom;
			}

			m_rectTracker.m_rect.SetRect(rt.left, rt.top, rt.right, rt.bottom);
// 			m_rectTracker.m_rect.SetRect(point.x-40,point.y-40,point.x+40,point.y+40);
			Invalidate();
		}
	}
	else
	{		
		if(m_bFirstDraw)
		{
			m_rectTracker.Track(this,point,TRUE);
			Invalidate();
		}
	}

	Invalidate();

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgRoiSet::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bDraw = FALSE;
	if(abs(m_rectTracker.m_rect.Width())>5 && abs(m_rectTracker.m_rect.Height())>5)
	{
		m_rectTracker.m_rect.NormalizeRect();
		ShowToolBar(TRUE);
		MoveToolBar(CPoint(m_rectTracker.m_rect.right, m_rectTracker.m_rect.bottom));
	}
	else
	{
		ShowToolBar(FALSE);
	}
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgRoiSet::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);
   
	if(nHitTest == 8)
		OnOK();
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDlgRoiSet::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bFirstDraw)
	{
		m_bFirstDraw = FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		ShowToolBar(FALSE);
		Invalidate();
	}
	else
	{
//		OnOK();
		OnCancel();
	}

	CDialog::OnRButtonUp(nFlags, point);
}

BOOL CDlgRoiSet::OnEraseBkgnd(CDC* pDC) 
{
// 	CDC dcCompatible;
// 	dcCompatible.CreateCompatibleDC(pDC);
// 	dcCompatible.SelectObject(m_pBitmap2);
// 
// 	CRect rect;
// 	GetClientRect(&rect);
// 	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgRoiSet::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (pWnd == this && m_rectTracker.SetCursor(this, nHitTest) && !m_bDraw && m_bFirstDraw) 
		return TRUE; 

	SetCursor(m_hCursor);
	
	return TRUE;
	//return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

HBITMAP CDlgRoiSet::CopyScreenToBitmap(LPRECT lpRect, BOOL bUnder)
{
	HDC hScrDC, hMemDC;      
	int nX, nY, nX2, nY2;      
	int nWidth, nHeight;
	HBITMAP hBitmap, hOldBitmap; 

	if (IsRectEmpty(lpRect))
		return NULL;

	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hScrDC);

	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	if (nX < 0)		nX = 0;
	if (nY < 0)		nY = 0;
	if (nX2 > m_xScreen)	nX2 = m_xScreen;
	if (nY2 > m_yScreen)	nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;

	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // 默认值 100% dpi：94
    //        125% dpi：120
    //        150% dpi：144
    int nDpi = GetDeviceCaps(hScrDC, LOGPIXELSY);   // 必须调用
    float fScale = nDpi*1.0f/96;

//     CString sz;
//     sz.Format("dpi:%d wid:%d hei:%d xscr:%d yscr:%d", nDpi, nWidth, nHeight, m_xScreen, m_yScreen);
//     AfxMessageBox(sz);

	if (bUnder)
	{
		BLENDFUNCTION bf; 
		bf.AlphaFormat = 0; 
		bf.BlendFlags = 0; 
		bf.BlendOp = AC_SRC_OVER; 
		bf.SourceConstantAlpha = 128;
		AlphaBlend(hMemDC, 0, 0, nWidth, nHeight, hScrDC, 0, 0, nWidth/**fScale*/, nHeight/**fScale*/, bf);
	}
	else
	{
        BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);     
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	return hBitmap;
}


BOOL CDlgRoiSet::SetPreviewRect(CRect &rt)
{
	m_PreviewRt = rt;
	m_rectTracker.SetPreviewRect(rt);

	return TRUE;
}

void CDlgRoiSet::SetScale(float fScale/* = 1.0f*/)
{
    m_rectTracker.SetScale(fScale);
}


BOOL CDlgRoiSet::GetPreviewRect(CRect &rt)
{
	rt = m_rectTracker.m_rect;
    rt.right += 2;
    rt.bottom+= 2;

	if (rt.IsRectEmpty())
		return FALSE;
	
	return TRUE;
}


void CDlgRoiSet::OnBtnCancel() 
{
	if(m_bFirstDraw)
	{
		m_bFirstDraw = FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		ShowToolBar(FALSE);
		Invalidate();
	}

	OnCancel();
}


void CDlgRoiSet::OnBtnOK() 
{	
	OnOK();
}


void CDlgRoiSet::ShowToolBar(BOOL bVal)
{
	if (bVal)
	{
		GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_OK)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_OK)->ShowWindow(SW_HIDE);
	}
}


void CDlgRoiSet::MoveToolBar(CPoint &pt)
{
	CRect ct;
//  GetDlgItem(IDC_BTN_OK)->GetClientRect(&ct);
//  GetDlgItem(IDC_BTN_CANCEL)->SetWindowPos(NULL, pt.x-1.6*ct.Width()+1, pt.y+2, 0, 0, SWP_NOSIZE);
//  GetDlgItem(IDC_BTN_OK)->SetWindowPos(NULL, pt.x-ct.Width()+1, pt.y+2, 0, 0, SWP_NOSIZE);

	GetDlgItem(IDC_BTN_CANCEL)->MoveWindow(pt.x-110, pt.y+2, 69, 18);
	GetDlgItem(IDC_BTN_OK)->MoveWindow(pt.x-40, pt.y+2, 39, 18);
}

