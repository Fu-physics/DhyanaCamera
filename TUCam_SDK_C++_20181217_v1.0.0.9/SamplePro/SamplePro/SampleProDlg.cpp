
// SampleProDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SamplePro.h"
#include "SampleProDlg.h"
//#include "../SkinSharp/SkinH.h"

//#pragma comment(lib, "../SkinSharp/SkinH.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSampleProDlg 对话框

CSampleProDlg::CSampleProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleProDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_fProportion_W			= 0;
	m_fProportion_H			= 0;
}

CSampleProDlg::~CSampleProDlg()
{
// 	if (m_CamCtlMode != NULL)
// 	{
// 		free(m_CamCtlMode);
// 		m_CamCtlMode = NULL;
// 	}
// 	if (m_PicViewMode != NULL)
// 	{
// 		free(m_PicViewMode);
// 		m_PicViewMode = NULL;
// 	}
}

void CSampleProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleProDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSampleProDlg 消息处理程序

BOOL CSampleProDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    //SkinH_Attach();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//全屏显示
	ShowWindow(SW_SHOWMAXIMIZED);

	CRect rt;
	GetWindowRect(&rt);

	int nWidth  = rt.Width();
	int nHeigth = rt.Height();

 
 	//绘制视图区域模块
//  m_PicViewMode = new CPicViewMode(this);
 	m_PicViewMode.Create(IDD_DLG_PICVIEW, this);
	m_PicViewMode.MoveWindow(m_recCamCtlView.right, m_recCamCtlView.top, rt.Width() - m_recCamCtlView.Width(), nHeigth);

	//显示区域
	int nTemp = (nWidth - m_recCamCtlView.Width() - nHeigth) / 2; 
	int nPos  = m_recCamCtlView.left + nTemp;						//使显示区域居中
	m_PicViewMode.GetDlgItem(IDC_STA_PIC)->MoveWindow(nPos,m_recCamCtlView.top, nHeigth,nHeigth);

	m_PicViewMode.ShowWindow(SW_SHOW);
 	m_PicViewMode.GetWindowRect(m_recPicView);
 	ScreenToClient(m_recPicView);

    //绘制相机控制模块
    // 	m_CamCtlMode = new CCameraControlMode(this);
    m_CamCtlMode.Create(IDD_DLG_CTL, this);

    m_CamCtlMode.GetWindowRect(m_recCamCtlView);
    ScreenToClient(m_recCamCtlView);
    m_CamCtlMode.MoveWindow(m_recCamCtlView.left, m_recCamCtlView.top, m_recCamCtlView.Width(),m_recCamCtlView.Height());
    m_CamCtlMode.ShowWindow(SW_SHOW);

    m_CamCtlMode.EnableControl(FALSE);
    m_CamCtlMode.InitInformation();
    m_CamCtlMode.InitControlRange();
    m_CamCtlMode.RefreshValue();

	RefreshUI();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSampleProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSampleProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSampleProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
	
void CSampleProDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if ((m_recCamCtlView.bottom < 0 || m_recCamCtlView.top < 0 || m_recCamCtlView.left < 0 || m_recCamCtlView.right < 0) && 
		(m_recPicView.bottom < 0 || m_recPicView.top < 0 || m_recPicView.left < 0 || m_recPicView.right < 0))
	{
		;
	}
 	else
	{
        if (NULL != m_CamCtlMode.GetSafeHwnd() && NULL != m_PicViewMode.GetSafeHwnd() && NULL != m_PicViewMode.GetDlgItem(IDC_STA_PIC))
        {
            RefreshUI();
        }
	}
}

//刷新界面
void CSampleProDlg::RefreshUI()
{    
	CRect rt, rt1;
	GetClientRect(&rt);

	int m_nRenainder   = 0;			//显示区域宽度除以4后的余数
	int m_nShowView_W  = 0;			//显示区域宽度

	int m_nPos		   = 0; 
	int m_nTemp		   = 0;

	int Heigth = rt.Height();
	int Width  = rt.Width();

	m_CamCtlMode.m_nCamCtlView_Width  = m_recCamCtlView.Width();
	m_CamCtlMode.m_nCamCtlView_Heigth = m_recCamCtlView.Height();

	m_CamCtlMode.m_nFactScreen_Width  = rt.Width();
	m_CamCtlMode.m_nFactScreen_Heigth = rt.Height();

    m_CamCtlMode.m_nCliWidth = Width - m_recCamCtlView.Width();
    m_CamCtlMode.m_nCliHeight= m_CamCtlMode.m_nFactScreen_Heigth;

    //移动各个窗口位置并设置宽度与高度
    m_CamCtlMode.MoveWindow(0, 0, m_CamCtlMode.m_nCamCtlView_Width, m_CamCtlMode.m_nFactScreen_Heigth);
    m_PicViewMode.MoveWindow(m_recCamCtlView.right, 0, m_CamCtlMode.m_nCliWidth, m_CamCtlMode.m_nFactScreen_Heigth);
    m_PicViewMode.GetDlgItem(IDC_STA_PIC)->MoveWindow(0, 0, m_CamCtlMode.m_nCliWidth, m_CamCtlMode.m_nFactScreen_Heigth);
    m_PicViewMode.GetDlgItem(IDC_STA_PIC)->Invalidate();

/*
	//计算比例
	m_fProportion_H = m_CamCtlMode.m_nFactScreen_Heigth / (800 * 1.0f);
	m_fProportion_W = (m_CamCtlMode.m_nFactScreen_Width - m_CamCtlMode.m_nCamCtlView_Width) / (800 * 1.0f);

	//选择较小比例进行计算
	if (m_fProportion_W > m_fProportion_H)
	{
		m_nRenainder   = (int)(800 * m_fProportion_H) % 4;
		m_nShowView_W  = (int)(800 * m_fProportion_H) - m_nRenainder;

		//计算图片显示区域的X轴坐标
		m_nTemp = (Width - m_CamCtlMode.m_nCamCtlView_Width - m_nShowView_W) / 2;
		m_nPos  = m_recCamCtlView.left + m_nTemp;

		//移动各个窗口位置并设置宽度与高度
		m_CamCtlMode.MoveWindow(0, 0, m_CamCtlMode.m_nCamCtlView_Width, m_CamCtlMode.m_nFactScreen_Heigth);
		m_PicViewMode.MoveWindow(m_recCamCtlView.right, 0, Width - m_recCamCtlView.Width(), m_CamCtlMode.m_nFactScreen_Heigth);
		m_PicViewMode.GetDlgItem(IDC_STA_PIC)->MoveWindow(m_nPos, m_recCamCtlView.top, m_nShowView_W, (int)(800 * m_fProportion_H));
	}
	else
	{
		m_nRenainder   = (int)( 800 * m_fProportion_W) % 4;
		m_nShowView_W  = (int)(800 * m_fProportion_W) - m_nRenainder;

		m_nTemp = (Heigth - (int)(800 * m_fProportion_W)) / 2;
		m_nPos  =  m_recPicView.top + m_nTemp;

		m_CamCtlMode.MoveWindow(0, 0, m_CamCtlMode.m_nCamCtlView_Width, m_CamCtlMode.m_nFactScreen_Heigth);
		m_PicViewMode.MoveWindow(m_recCamCtlView.right, 0, Width - m_recCamCtlView.Width(), m_CamCtlMode.m_nFactScreen_Heigth);
		m_PicViewMode.GetDlgItem(IDC_STA_PIC)->MoveWindow(m_recCamCtlView.left, m_nPos, m_nShowView_W, (int)(800 * m_fProportion_W));
	}
*/
}

BOOL CSampleProDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
    {  
        return TRUE;  
    }

    return CDialog::PreTranslateMessage(pMsg);
}
