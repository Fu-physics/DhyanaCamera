// ImageCapture.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "ImageCapture.h"
//#include "afxdialogex.h"
#include "CompressDialog.h"
#include <io.h>
#include "Raminfo.h"

#define IDT_GET_RECORD_TIME   0x66
#define IDT_SPC_RECORD_TIME   1000  // Interval 1 Second
#define IDT_GET_CAPTURE_MSG   0x67
#define IDT_SPC_CAPTURE_MSG   300   // Interval 0.3 Second

// ImageCapture dialog

IMPLEMENT_DYNAMIC(ImageCapture, CDialog)

ImageCapture::ImageCapture(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(ImageCapture::IDD, pParent)
	, m_nPicNum(0)
    , m_szPicName(_T(""))
{
	m_opCam                 = opCam;
	m_ccmode                = pParent;
	m_nFmtSave              = 0;
    m_nFmtCnt               = 0;

	m_bRecording            = FALSE;
    m_dwFccHandler          = MAKETAG('D', 'I', 'B', ' ');
    m_dwTimeCnt             = 0;
}

ImageCapture::~ImageCapture()
{

}

void ImageCapture::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_PATH, m_strFilePath);
    DDX_Text(pDX, IDC_EDT_PICNUM, m_nPicNum);
    DDX_Control(pDX, IDC_CHK_TIF, m_chkTif);
    DDX_Control(pDX, IDC_CHK_RAW, m_chkRaw);
    DDX_Text(pDX, IDC_EDT_PICNAME, m_szPicName);
}


BEGIN_MESSAGE_MAP(ImageCapture, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CAPTURE, &ImageCapture::OnBnClickedBtnCapture)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &ImageCapture::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_CHK_TIF, &ImageCapture::OnBnClickedChkTif)
	ON_BN_CLICKED(IDC_CHK_RAW, &ImageCapture::OnBnClickedChkRaw)
	ON_BN_CLICKED(IDC_BTN_REC, &ImageCapture::OnBnClickedBtnRec)
	ON_BN_CLICKED(IDC_BTN_CODEC, &ImageCapture::OnBnClickedBtnCodec)
	ON_MESSAGE(TUMSG_CAP, OnTUMessageCap)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_PICNUM, &ImageCapture::OnDeltaposSpnPicnum)
    ON_EN_CHANGE(IDC_EDT_PICNAME, &ImageCapture::OnEnChangeEdtPicname)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// ImageCapture message handlers

void ImageCapture::InitControlRange()
{
	RefreshBtn();
    SetTimer(IDT_GET_CAPTURE_MSG, IDT_SPC_CAPTURE_MSG, NULL);
}

void ImageCapture::RefreshValue()
{
	// 拍照
	m_strFilePath = m_ccmode->m_strFilePath;
	m_nPicNum = 1;
	m_nFmtCnt = 1;
	m_nFmtSave = TUFMT_TIF;//TUFMT_TIF
	m_chkTif.SetCheck(TRUE);
	m_szPicName = _T("TS");
	m_ccmode->m_szPicName = m_szPicName;
	m_ccmode->m_nFmtSave  = m_nFmtSave;
	m_ccmode->m_nPicNum   = m_nPicNum;
	m_ccmode->m_nFmtCnt   = m_nFmtCnt;
    m_nIdxFcc             = 0xFF;

	UpdateData(FALSE);
}

void ImageCapture::RefreshBtn(BOOL bLiving)
{
	GetDlgItem(IDC_BTN_CAPTURE)->EnableWindow(bLiving);
	GetDlgItem(IDC_BTN_REC)->EnableWindow(bLiving);
}

void ImageCapture::RefreshBtnRaw(BOOL bRaw)
{
	if(bRaw)
		m_chkRaw.EnableWindow(bRaw);
	else
	{
		m_chkRaw.SetCheck(bRaw);      
		m_chkRaw.EnableWindow(bRaw);
	}
}

void ImageCapture::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    if (IDT_GET_RECORD_TIME == nIDEvent)
    {
        m_dwTimeCnt++;

        CString sz;
        sz.Format(_T("%02d : %02d"), m_dwTimeCnt / 60, m_dwTimeCnt % 60);
        GetDlgItem(IDC_STA_RECTM)->SetWindowText(sz);
    }
    else if (IDT_GET_CAPTURE_MSG == nIDEvent)
    {
        int nCntCap = m_ccmode->m_nCntSave * m_ccmode->m_nFmtCnt;
        int nCntTol = m_ccmode-> m_nPicNum * m_ccmode->m_nFmtCnt;

        if (0 == m_ccmode->m_nCntSave || nCntCap == nCntTol || !m_ccmode->m_bWaitting || !m_ccmode->m_bSaving)
        {
            GetDlgItem(IDC_STA_CAPTURE)->SetWindowText("");
            GetDlgItem(IDC_BTN_CAPTURE)->SetWindowText("Capture");
        }
        else
        {
            CString sz;
            sz.Format(_T("Capturing %d/%d success!"), nCntCap, nCntTol);
            GetDlgItem(IDC_STA_CAPTURE)->SetWindowText(sz);
        }
    }

    CDialog::OnTimer(nIDEvent);
}

LRESULT ImageCapture::OnTUMessageCap(WPARAM wParam, LPARAM lParam)
{
    int nCntCap = (int)lParam * m_ccmode->m_nFmtCnt;
    int nCntTol = m_ccmode-> m_nPicNum * m_ccmode->m_nFmtCnt;

    CString sz;
    sz.Format(_T("Capturing %d/%d success!"), nCntCap, nCntTol);
    GetDlgItem(IDC_STA_CAPTURE)->SetWindowText(sz);

    if (nCntCap == nCntTol || !m_ccmode->m_bWaitting || !m_ccmode->m_bSaving)
    {
        Sleep(500);
        GetDlgItem(IDC_STA_CAPTURE)->SetWindowText("");
        GetDlgItem(IDC_BTN_CAPTURE)->SetWindowText("Capture");
    }

    return 0;
}

BOOL ImageCapture::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if((pMsg->wParam == VK_ESCAPE))
    {  
        return TRUE;  
    }

    if ((pMsg->wParam == VK_RETURN))
    {
        UpdateData();
        return TRUE;
    }

	if(TranslateAccelerator(m_hWnd,m_ccmode->hAccKey,pMsg))  
	{    
		return TRUE;    
	}  

    return CDialog::PreTranslateMessage(pMsg);
}

void ImageCapture::OnBnClickedBtnCapture()
{
	// TODO: Add your control notification handler code here
	UpdateData();

    if (m_ccmode->m_bSaving)
    {
        m_ccmode->m_bSaving  = FALSE;

        Sleep(600);

        GetDlgItem(IDC_STA_CAPTURE)->SetWindowText("");
        GetDlgItem(IDC_BTN_CAPTURE)->SetWindowText("Capture");
    }
    else
    {
        m_ccmode->m_szPicName = m_szPicName;
        m_ccmode->m_nPicNum   = m_nPicNum;
        m_ccmode->m_nCntSave  = 0;
        m_ccmode->m_uiCapFrames = 1;
        m_ccmode->m_uiCapTimes++;

        if (m_szPicName == "")
        {
            AfxMessageBox(_T("Capture picture name must be not empty!"));
            return;
        }

        if (m_nPicNum > 0 && m_nFmtCnt > 0)
        {
            m_ccmode->m_nCntSave = 0;         // 保存计数
            m_ccmode->m_bSaving  = TRUE;

            GetDlgItem(IDC_BTN_CAPTURE)->SetWindowText("Stop Capture");
        }
        else
        {
            AfxMessageBox(_T("Capture picture number and format must be more than 1!"));
        }
    }
}


void ImageCapture::OnBnClickedBtnBrowse()
{
	// TODO: Add your control notification handler code here
	CString szTitle;
    szTitle.LoadString(IDS_SELECT_TITTLE);

    TCHAR szDir[MAX_PATH];
    BROWSEINFO bi; 
    ITEMIDLIST*pidl; 

    bi.hwndOwner	= NULL; 
    bi.pidlRoot		= NULL;
    bi.pszDisplayName = szDir; 
    bi.lpszTitle	= szTitle/*_T("Select a directory")*/; 
    bi.ulFlags		= 80 ;
//  bi.ulFlags		= BIF_RETURNONLYFSDIRS/*80*/; 
    bi.lpfn			= NULL; 
    bi.lParam		= 0; 
    bi.iImage		= 0;

    pidl = SHBrowseForFolder(&bi);

    if (pidl && SHGetPathFromIDList(pidl, szDir))
    {
        if (szDir[3]) _tcscat(szDir, _T("\\"));		
        GetDlgItem(IDC_EDT_PATH)->SetWindowText(szDir);
        m_strFilePath = szDir;
		m_ccmode->m_strFilePath =m_strFilePath;
    }
}  



void ImageCapture::OnBnClickedChkTif()
{
	// TODO: Add your control notification handler code here
	if (m_chkTif.GetCheck())
	{
		m_nFmtSave |= TUFMT_TIF; //
		m_nFmtCnt++;
	}
	else
	{
		m_nFmtSave &= ~TUFMT_TIF;
		m_nFmtCnt--;
	}
	m_ccmode->m_nFmtSave  = m_nFmtSave;
	m_ccmode->m_nFmtCnt   = m_nFmtCnt;
}


void ImageCapture::OnBnClickedChkRaw()
{
	// TODO: Add your control notification handler code here
	if (m_chkRaw.GetCheck())
	{
		m_nFmtSave |= TUFMT_RAW;
		m_nFmtCnt++;
	}
	else
	{
		m_nFmtSave &= ~TUFMT_RAW;
		m_nFmtCnt--;
	}
	m_ccmode->m_nFmtSave  = m_nFmtSave;
	m_ccmode->m_nFmtCnt   = m_nFmtCnt;
}


void ImageCapture::OnBnClickedBtnRec()
{
	// TODO: Add your control notification handler code here
    UpdateData();
    m_ccmode->m_szPicName = m_szPicName;

	if (m_ccmode->m_bRecording)
    {	
        m_ccmode->OnBtnRecord();
        KillTimer(IDT_GET_RECORD_TIME);
        SetDlgItemText(IDC_BTN_REC, _T("Record"));
        SetDlgItemText(IDC_STA_RECTM, _T(""));

		GetDlgItem(IDC_BTN_CODEC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);    
        GetDlgItem(IDC_BTN_CAPTURE)->EnableWindow(TRUE);    

    }
	else  
	{
		m_dwTimeCnt = 0;
		m_ccmode->OnBtnRecord();
		SetDlgItemText(IDC_BTN_REC, _T("Stop Record"));                   
		SetTimer(IDT_GET_RECORD_TIME, IDT_SPC_RECORD_TIME, NULL);

		GetDlgItem(IDC_BTN_CODEC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);   
		GetDlgItem(IDC_BTN_CAPTURE)->EnableWindow(FALSE); 
	}
}


void ImageCapture::OnBnClickedBtnCodec()
{
	// TODO: Add your control notification handler code here
	CCompressDialog dlgCompress;

    dlgCompress.m_nIdxFcc = m_nIdxFcc;
    if (IDOK == dlgCompress.DoModal())
    {
        m_dwFccHandler = dlgCompress.GetCompressorFccHandler();
        m_nIdxFcc      = dlgCompress.GetCurrentFcc();
		m_ccmode->m_dwFccHandler = m_dwFccHandler;
    }
}

void ImageCapture::OnDeltaposSpnPicnum(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    m_nPicNum = GetDlgItemInt(IDC_EDT_PICNUM);

    if (pNMUpDown->iDelta < 0)
    {
        if (m_nPicNum >= 999)
        {
            return;
        }
        m_nPicNum += 1;

        CString strUs;
        strUs.Format(_T("%d"), m_nPicNum);
        GetDlgItem(IDC_EDT_PICNUM)->SetWindowText(strUs);
    }
    else if (pNMUpDown->iDelta > 0)
    {
        if (m_nPicNum == 0)
        {
            return;
        }

        if (m_nPicNum < 2)
        {
            m_nPicNum = 1;
        }
        else
        {
            m_nPicNum -= 1;
        }       

        CString strUs;
        strUs.Format(_T("%d"), m_nPicNum);
        GetDlgItem(IDC_EDT_PICNUM)->SetWindowText(strUs);
    }

    *pResult = 0;
}

void ImageCapture::OnEnChangeEdtPicname()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码

    CString szTemp = m_szPicName;

    UpdateData();

//  GetDlgItem(IDC_EDT_PICNUM)->GetWindowText(szTemp);

    if (-1 != m_szPicName.Find('\\') || -1 != m_szPicName.Find('/') || -1 != m_szPicName.Find(':') || -1 != m_szPicName.Find('?') || -1 != m_szPicName.Find('<') || -1 != m_szPicName.Find('>') || -1 != m_szPicName.Find('|') || -1 != m_szPicName.Find(','))
    {
        m_szPicName = szTemp;
        UpdateData(FALSE);
    }        
}

void ImageCapture::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    KillTimer(IDT_GET_CAPTURE_MSG);
}
