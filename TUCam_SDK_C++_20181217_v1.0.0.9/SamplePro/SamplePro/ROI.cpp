// ROI.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "ROI.h"
//#include "afxdialogex.h"


// ROI dialog

IMPLEMENT_DYNAMIC(ROI, CDialog)

ROI::ROI(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(ROI::IDD, pParent)
	, m_uiHOffset(0)
    , m_uiVOffset(0)
    , m_uiWidth(200)
    , m_uiHeight(200)
{
	m_opCam          = opCam;
	m_ccmode         = pParent;
}

//ROI::~ROI()
//{
//}

void ROI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHK_ROI, m_bROI);
	DDX_Text(pDX, IDC_EDT_HOFFSET, m_uiHOffset);
    DDX_Text(pDX, IDC_EDT_VOFFSET, m_uiVOffset);
    DDX_Text(pDX, IDC_EDT_WIDTH, m_uiWidth);
    DDX_Text(pDX, IDC_EDT_HEIGHT, m_uiHeight);
}


BEGIN_MESSAGE_MAP(ROI, CDialog)
	ON_BN_CLICKED(IDC_CHK_ROI, &ROI::OnBnClickedChkRoi)
	ON_BN_CLICKED(IDC_BTN_ROI, &ROI::OnBnClickedBtnRoi)
END_MESSAGE_MAP()


// ROI message handlers

void ROI::InitControlRange()
{

}

void ROI::RefreshValue()
{

}

void ROI::RefreshState()
{
	if (m_bROI.GetCheck())
	{
		// 设置ROI控件使能
        GetDlgItem(IDC_STA_HOFF)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_VOFF)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_WIDTH)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_HEIGHT)->EnableWindow(TRUE);

        GetDlgItem(IDC_EDT_HOFFSET)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_VOFFSET)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_WIDTH)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_HEIGHT)->EnableWindow(TRUE);
		m_bROI.SetCheck(FALSE);
		GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set ROI"));
		UpdateData(FALSE);
	}
}

BOOL ROI::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if((pMsg->wParam == VK_ESCAPE))
    {  
        return TRUE;  
    }

    if ((pMsg->wParam == VK_RETURN))
    {
        //UpdateData();
        return TRUE;
    }

	if(TranslateAccelerator(m_hWnd,m_ccmode->hAccKey, pMsg))  
	{    
		return TRUE;    
	}  

    return CDialog::PreTranslateMessage(pMsg);
}

void ROI::OnBnClickedChkRoi()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_bROI.GetCheck())
	{
		m_ccmode->m_bChk = TRUE;
		m_ccmode->OnChkRoiFunction();
		
	    m_uiVOffset = m_ccmode->m_uVOffset;
		m_uiHOffset = m_ccmode->m_uHOffset;
		m_uiWidth   = m_ccmode->m_uWidth ;
		m_uiHeight  = m_ccmode->m_uHeight;

		UpdateData(FALSE);

		if(m_ccmode->m_bChk)
		{
			// 设置ROI控件使能
			GetDlgItem(IDC_STA_HOFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_VOFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_WIDTH)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_HEIGHT)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDT_HOFFSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_VOFFSET)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_WIDTH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_HEIGHT)->EnableWindow(FALSE);

			GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set Full"));
		}
		else
		{
			m_bROI.SetCheck(FALSE);
			GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set ROI"));
		}
	}
	else
    {
        m_ccmode->m_bChk = FALSE;
		m_ccmode->OnChkRoiFunction();

		// 设置ROI控件使能
        GetDlgItem(IDC_STA_HOFF)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_VOFF)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_WIDTH)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_HEIGHT)->EnableWindow(TRUE);

        GetDlgItem(IDC_EDT_HOFFSET)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_VOFFSET)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_WIDTH)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_HEIGHT)->EnableWindow(TRUE);

        GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set ROI"));
    }
}


void ROI::OnBnClickedBtnRoi()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bROI.GetCheck())
	{
		m_ccmode->m_bChk = TRUE;
		m_ccmode->OnBtnRoiFunction();

		m_bROI.SetCheck(FALSE);
		m_bROI.EnableWindow(TRUE);

		GetDlgItem(IDC_STA_HOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_STA_VOFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_STA_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STA_HEIGHT)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDT_HOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_VOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_HEIGHT)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set ROI"));

		UpdateData(FALSE);
	}
	else
	{
		m_ccmode->m_bChk = FALSE;
		m_ccmode->m_uVOffset = m_uiVOffset;
		m_ccmode->m_uHOffset = m_uiHOffset;
		m_ccmode->m_uWidth   = m_uiWidth;
		m_ccmode->m_uHeight  = m_uiHeight;

		m_ccmode->OnBtnRoiFunction();

		m_uiVOffset = m_ccmode->m_uVOffset;
		m_uiHOffset = m_ccmode->m_uHOffset;
		m_uiWidth   = m_ccmode->m_uWidth ;
		m_uiHeight  = m_ccmode->m_uHeight;

		m_bROI.SetCheck(TRUE);
		m_bROI.EnableWindow(FALSE);

		GetDlgItem(IDC_STA_HOFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_STA_VOFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_STA_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STA_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_HOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_VOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set Full"));

		UpdateData(FALSE);
	}
}

