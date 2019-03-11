// Others.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "Others.h"
//#include "afxdialogex.h"


// Others dialog

IMPLEMENT_DYNAMIC(Others, CDialog)

Others::Others(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(Others::IDD, pParent)
	, m_szWRSN(_T(""))
{
	m_opCam                 = opCam;
	m_ccmode                = pParent;
	m_nFFCStep              = 1;
}

//Others::~Others()
//{
//}

void Others::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_FFC, m_chkFFC);
	DDX_Text(pDX, IDC_EDT_WRSN, m_szWRSN);
}


BEGIN_MESSAGE_MAP(Others, CDialog)
	ON_BN_CLICKED(IDC_BTN_FFC, &Others::OnBnClickedBtnFfc)
	ON_BN_CLICKED(IDC_CHK_FFC, &Others::OnBnClickedChkFfc)
	ON_BN_CLICKED(IDC_BTN_DFC, &Others::OnBnClickedBtnDfc)
	ON_BN_CLICKED(IDC_BTN_WRSN, &Others::OnBnClickedBtnWrsn)
END_MESSAGE_MAP()


// Others message handlers

void Others::InitControlRange()
{
	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_CAPA_ATTR  attrCapa;
		// 获取坏点校正
		attrCapa.idCapa = TUIDC_DFTCORRECTION;
		if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
			GetDlgItem(IDC_BTN_DFC)->EnableWindow(FALSE);
		}

		// 获取平场校正
        attrCapa.idCapa = TUIDC_FLTCORRECTION;
        if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
        {
            GetDlgItem(IDC_BTN_FFC)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_FFC)->EnableWindow(FALSE);
        }
        else
        {
            GetDlgItem(IDC_BTN_FFC)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHK_FFC)->EnableWindow(TRUE);
        }
	}
}

void Others::RefreshValue()
{
	if (NULL != m_opCam.hIdxTUCam)
	{
		int nVal;
		// 获取平场校正
		if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_DFTCORRECTION, &nVal))
		{
			m_chkFFC.SetCheck(3 == nVal ? 1 : 0);
		}
	}
}

BOOL Others::PreTranslateMessage(MSG* pMsg)
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

	if(TranslateAccelerator(m_hWnd,m_ccmode->hAccKey,pMsg))  
	{    
		return TRUE;    
	}  

    return CDialog::PreTranslateMessage(pMsg);
}

void Others::OnBnClickedBtnFfc()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        int nVal = 0;

        switch (m_nFFCStep)
        {
        case 1:
            {
                GetDlgItem(IDC_BTN_FFC)->EnableWindow(FALSE);
                GetDlgItem(IDC_CHK_FFC)->EnableWindow(FALSE);
    
                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_FLTCORRECTION, m_nFFCStep); 
                while (1)
                {
                    if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_FLTCORRECTION, &nVal))
                    {
                        if (1 != nVal)
                        {
                            m_nFFCStep = 2;                      
                            break;
                        }
                    }
                } 

                GetDlgItem(IDC_BTN_FFC)->EnableWindow(TRUE);
                GetDlgItem(IDC_CHK_FFC)->EnableWindow(TRUE);
            }
            break;
        case 2:
            {
                GetDlgItem(IDC_BTN_FFC)->EnableWindow(FALSE);
                GetDlgItem(IDC_CHK_FFC)->EnableWindow(FALSE);

                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_FLTCORRECTION, m_nFFCStep); 
                while (1)
                {
                    if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_FLTCORRECTION, &nVal))
                    {
                        if (2 != nVal)
                        {
                            if (0 == nVal)
                            {
                                AfxMessageBox(_T("Do FFC failed!"));
                            }
                            else
                            {
                                m_chkFFC.SetCheck(1);
                                AfxMessageBox(_T("Do FFC succeed!"));
                            }

                            m_nFFCStep = 1;
                            break;
                        }
                    }
                } 

                GetDlgItem(IDC_BTN_FFC)->EnableWindow(TRUE);
                GetDlgItem(IDC_CHK_FFC)->EnableWindow(TRUE);
            }         

            break;
        }
    }
	
}


void Others::OnBnClickedChkFfc()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        int nVal = m_chkFFC.GetCheck() ? 3 : 0;

        if (TUCAMRET_SUCCESS != TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_FLTCORRECTION, nVal))
        {
            AfxMessageBox(_T("Please click the button FFC first!"));
            m_chkFFC.SetCheck(0);
        }
    }
}


void Others::OnBnClickedBtnDfc()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_DFTCORRECTION, 1);

        int nVal = 1;

        while (nVal == 1)
        {
            TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_DFTCORRECTION, &nVal);
        }
    }    
}


void Others::OnBnClickedBtnWrsn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char cSN[TUSN_SIZE] = {0};

    memcpy(&cSN[0], m_szWRSN.GetBuffer(TUSN_SIZE), TUSN_SIZE);
    m_szWRSN.ReleaseBuffer();

    TUCAM_REG_RW regRW;
    regRW.nRegType= TUREG_SN;
    regRW.pBuf    = &cSN[0];
    regRW.nBufSize= TUSN_SIZE;

    if (TUCAMRET_SUCCESS == TUCAM_Reg_Write(m_opCam.hIdxTUCam, regRW))
    {
        if (TUCAMRET_SUCCESS == TUCAM_Reg_Read(m_opCam.hIdxTUCam, regRW))
        {
           // GetDlgItem(IDC_STA_SN)->SetWindowText(cSN);
        }

        AfxMessageBox(_T("Write SN successed!"));
    }
    else
    {
        AfxMessageBox(_T("Write SN failed!"));
    }  
}
