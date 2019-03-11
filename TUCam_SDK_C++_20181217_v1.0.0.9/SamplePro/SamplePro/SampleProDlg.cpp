
// SampleProDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SamplePro.h"
#include "SampleProDlg.h"
//#include "../SkinSharp/SkinH.h"

//#pragma comment(lib, "../SkinSharp/SkinH.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSampleProDlg �Ի���

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


// CSampleProDlg ��Ϣ�������

BOOL CSampleProDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    //SkinH_Attach();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//ȫ����ʾ
	ShowWindow(SW_SHOWMAXIMIZED);

	CRect rt;
	GetWindowRect(&rt);

	int nWidth  = rt.Width();
	int nHeigth = rt.Height();

 
 	//������ͼ����ģ��
//  m_PicViewMode = new CPicViewMode(this);
 	m_PicViewMode.Create(IDD_DLG_PICVIEW, this);
	m_PicViewMode.MoveWindow(m_recCamCtlView.right, m_recCamCtlView.top, rt.Width() - m_recCamCtlView.Width(), nHeigth);

	//��ʾ����
	int nTemp = (nWidth - m_recCamCtlView.Width() - nHeigth) / 2; 
	int nPos  = m_recCamCtlView.left + nTemp;						//ʹ��ʾ�������
	m_PicViewMode.GetDlgItem(IDC_STA_PIC)->MoveWindow(nPos,m_recCamCtlView.top, nHeigth,nHeigth);

	m_PicViewMode.ShowWindow(SW_SHOW);
 	m_PicViewMode.GetWindowRect(m_recPicView);
 	ScreenToClient(m_recPicView);

    //�����������ģ��
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSampleProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSampleProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
	
void CSampleProDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

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

//ˢ�½���
void CSampleProDlg::RefreshUI()
{    
	CRect rt, rt1;
	GetClientRect(&rt);

	int m_nRenainder   = 0;			//��ʾ�����ȳ���4�������
	int m_nShowView_W  = 0;			//��ʾ������

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

    //�ƶ���������λ�ò����ÿ����߶�
    m_CamCtlMode.MoveWindow(0, 0, m_CamCtlMode.m_nCamCtlView_Width, m_CamCtlMode.m_nFactScreen_Heigth);
    m_PicViewMode.MoveWindow(m_recCamCtlView.right, 0, m_CamCtlMode.m_nCliWidth, m_CamCtlMode.m_nFactScreen_Heigth);
    m_PicViewMode.GetDlgItem(IDC_STA_PIC)->MoveWindow(0, 0, m_CamCtlMode.m_nCliWidth, m_CamCtlMode.m_nFactScreen_Heigth);
    m_PicViewMode.GetDlgItem(IDC_STA_PIC)->Invalidate();

/*
	//�������
	m_fProportion_H = m_CamCtlMode.m_nFactScreen_Heigth / (800 * 1.0f);
	m_fProportion_W = (m_CamCtlMode.m_nFactScreen_Width - m_CamCtlMode.m_nCamCtlView_Width) / (800 * 1.0f);

	//ѡ���С�������м���
	if (m_fProportion_W > m_fProportion_H)
	{
		m_nRenainder   = (int)(800 * m_fProportion_H) % 4;
		m_nShowView_W  = (int)(800 * m_fProportion_H) - m_nRenainder;

		//����ͼƬ��ʾ�����X������
		m_nTemp = (Width - m_CamCtlMode.m_nCamCtlView_Width - m_nShowView_W) / 2;
		m_nPos  = m_recCamCtlView.left + m_nTemp;

		//�ƶ���������λ�ò����ÿ����߶�
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
    // TODO: �ڴ����ר�ô����/����û���
    if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
    {  
        return TRUE;  
    }

    return CDialog::PreTranslateMessage(pMsg);
}
