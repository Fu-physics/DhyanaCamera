// ColorControl.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "ColorControl.h"
//#include "afxdialogex.h"

#define IDT_GET_WHITE_BALANCE 0x67
#define IDT_SPC_WHITE_BALANCE 1000  // Interval 1 Second
// ColorControl dialog

IMPLEMENT_DYNAMIC(ColorControl, CDialog)

ColorControl::ColorControl(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(ColorControl::IDD, pParent)
{
	m_opCam                 = opCam;
	m_ccmode                = pParent;
	m_bRGainRange           = TRUE;
	m_bGGainRange           = TRUE;
	m_bBGainRange			= TRUE;
	m_bSatRange				= TRUE;
	m_bGammaRange           = TRUE;
  
    m_bContrastRange        = TRUE;
    m_bCMSRange             = TRUE;
}

//ColorControl::~ColorControl()
//{
//}

void ColorControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_ATWB, m_chkATWB);
	DDX_Control(pDX, IDC_SLD_RGAIN, m_sldRGain);
    DDX_Control(pDX, IDC_SLD_GGAIN, m_sldGGain);
    DDX_Control(pDX, IDC_SLD_BGAIN, m_sldBGain);
    DDX_Control(pDX, IDC_SLD_SATURATION, m_sldSat);
    DDX_Control(pDX, IDC_SLD_GAMMA, m_sldGamma);
	DDX_Control(pDX, IDC_SLD_CONTRAST, m_sldContrast);
    
	DDX_Control(pDX, IDC_SLD_CMS, m_sldCMS);
}


BEGIN_MESSAGE_MAP(ColorControl, CDialog)
	ON_BN_CLICKED(IDC_BTN_WB, &ColorControl::OnBnClickedBtnWb)
	ON_BN_CLICKED(IDC_CHK_ATWB, &ColorControl::OnBnClickedChkAtwb)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_RGAIN, &ColorControl::OnNMCustomdrawSldRgain)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GGAIN, &ColorControl::OnNMCustomdrawSldGgain)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_BGAIN, &ColorControl::OnNMCustomdrawSldBgain)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_SATURATION, &ColorControl::OnNMCustomdrawSldSaturation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GAMMA, &ColorControl::OnNMCustomdrawSldGamma)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_CONTRAST, &ColorControl::OnNMCustomdrawSldContrast)
	
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_CMS, &ColorControl::OnNMCustomdrawSldCms)
END_MESSAGE_MAP()


// ColorControl message handlers
void  ColorControl::InitControlRange()
{
	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_CAPA_ATTR  attrCapa;
		TUCAM_PROP_ATTR  attrProp;

		attrProp.nIdxChn= 0;                    // 使用默认通道
		// 获取色彩增益
		attrProp.idProp = TUIDP_CHNLGAIN;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bRGainRange = FALSE;
			m_sldRGain.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
			m_bGGainRange = FALSE;
			m_sldGGain.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
			m_bBGainRange = FALSE;
			m_sldBGain.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
		}

		// 获取饱和度范围
		attrProp.idProp = TUIDP_SATURATION;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bSatRange = FALSE;
			m_sldSat.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
		}
		else
		{
			GetDlgItem(IDC_STA_SATURATION)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLD_SATURATION)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_SATURATION)->EnableWindow(FALSE);
		}

		// 获取色彩矩阵范围
		attrProp.idProp = TUIDP_CLRTEMPERATURE;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bCMSRange = FALSE;
			m_sldCMS.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
		}
		else
		{
			GetDlgItem(IDC_STA_CMS)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLD_CMS)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_CMS)->EnableWindow(FALSE);
		}

		// 获取伽马值
		attrProp.idProp = TUIDP_GAMMA;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bGammaRange = FALSE;
			m_sldGamma.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
		}

		// 获取对比度值
		attrProp.idProp = TUIDP_CONTRAST;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bContrastRange = FALSE;
			m_sldContrast.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);

			GetDlgItem(IDC_STA_CONTRAST)->EnableWindow(TRUE);
			GetDlgItem(IDC_SLD_CONTRAST)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDT_CONTRAST)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STA_CONTRAST)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLD_CONTRAST)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_CONTRAST)->EnableWindow(FALSE);
		}

		// 获取自动白平衡
		attrCapa.idCapa = TUIDC_ATWBALANCE;
		if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
			GetDlgItem(IDC_BTN_WB)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHK_ATWB)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHK_ATWB)->ShowWindow(SW_HIDE);
		}
		else
		{
			if (attrCapa.nValMax > 1)
			{
				GetDlgItem(IDC_BTN_WB)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHK_ATWB)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_CHK_ATWB)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_CHK_ATWB)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_WB)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_WB)->EnableWindow(TRUE);
			}
		}
	}
}
void  ColorControl::RefreshValue()
{
	if (NULL != m_opCam.hIdxTUCam)
	{
		int     nVal  = 0;
		double  dbVal = 0;
		CString szText= _T("");
		// 获取白平衡状态
		if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, &nVal))
		{
			if (nVal > 1)
			{
				m_chkATWB.SetCheck(1);

				GetDlgItem(IDC_SLD_RGAIN)->EnableWindow(FALSE); 
				GetDlgItem(IDC_SLD_GGAIN)->EnableWindow(FALSE); 
				GetDlgItem(IDC_SLD_BGAIN)->EnableWindow(FALSE); 

				GetDlgItem(IDC_STA_RGAIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_STA_GGAIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_STA_BGAIN)->EnableWindow(FALSE);

				GetDlgItem(IDC_EDT_RGAIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDT_GGAIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDT_BGAIN)->EnableWindow(FALSE);
			}
			else
			{
				m_chkATWB.SetCheck(0);

				GetDlgItem(IDC_SLD_RGAIN)->EnableWindow(TRUE); 
				GetDlgItem(IDC_SLD_GGAIN)->EnableWindow(TRUE); 
				GetDlgItem(IDC_SLD_BGAIN)->EnableWindow(TRUE); 

				GetDlgItem(IDC_STA_RGAIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_STA_GGAIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_STA_BGAIN)->EnableWindow(TRUE);

				GetDlgItem(IDC_EDT_RGAIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDT_GGAIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDT_BGAIN)->EnableWindow(TRUE);
			}
		}

		// 获取当前Red通道增益
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.1f"), nVal / 2.0f);
			GetDlgItem(IDC_EDT_RGAIN)->SetWindowText(szText);  

			m_sldRGain.SetPos(nVal);
		}

		// 获取当前Green通道增益
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.1f"), nVal / 2.0f);
			GetDlgItem(IDC_EDT_GGAIN)->SetWindowText(szText);  

			m_sldGGain.SetPos(nVal);
		}

		// 获取当前Blue通道增益
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.1f"), nVal / 2.0f);
			GetDlgItem(IDC_EDT_BGAIN)->SetWindowText(szText);  

			m_sldBGain.SetPos(nVal);
		}

		// 获取当前饱和度
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_SATURATION, &dbVal))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%d"), nVal);
			GetDlgItem(IDC_EDT_SATURATION)->SetWindowText(szText);  

			m_sldSat.SetPos(nVal);
		}

		// 获取当前CMS
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CLRTEMPERATURE, &dbVal))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%d"), nVal);
			GetDlgItem(IDC_EDT_CMS)->SetWindowText(szText);  

			m_sldCMS.SetPos(nVal);
		}

		// 获取当前伽马值
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_GAMMA, &dbVal))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.2f"), nVal / 100.0f);
			GetDlgItem(IDC_EDT_GAMMA)->SetWindowText(szText);  

			m_sldGamma.SetPos(nVal);
		}

		// 获取当前对比度值
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CONTRAST, &dbVal))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%d"), nVal);
			GetDlgItem(IDC_EDT_CONTRAST)->SetWindowText(szText);  

			m_sldContrast.SetPos(nVal);
		}

	}
}

void ColorControl::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IDT_GET_WHITE_BALANCE == nIDEvent)
	{
		KillTimer(IDT_GET_WHITE_BALANCE);

		int nVal = 0;
		double dbVal = 1.0f;
		CString szText;
		// 获取当前Red通道增益
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.1f"), nVal / 2.0f);
			GetDlgItem(IDC_EDT_RGAIN)->SetWindowText(szText);  

			m_sldRGain.SetPos(nVal);
		}

		// 获取当前Green通道增益
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.1f"), nVal / 2.0f);
			GetDlgItem(IDC_EDT_GGAIN)->SetWindowText(szText);  

			m_sldGGain.SetPos(nVal);
		}

		// 获取当前Blue通道增益
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
		{  
			nVal = (int)dbVal;
			szText.Format(_T("%.1f"), nVal / 2.0f);
			GetDlgItem(IDC_EDT_BGAIN)->SetWindowText(szText);  

			m_sldBGain.SetPos(nVal);
		}
	}
		 

    CDialog::OnTimer(nIDEvent);
}

BOOL ColorControl::PreTranslateMessage(MSG* pMsg)
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


void ColorControl::OnBnClickedBtnWb()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, 1);

		SetTimer(IDT_GET_WHITE_BALANCE, IDT_SPC_WHITE_BALANCE, NULL);
	}
}


void ColorControl::OnBnClickedChkAtwb()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        if (m_chkATWB.GetCheck())
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, 2);

            GetDlgItem(IDC_SLD_RGAIN)->EnableWindow(FALSE); 
            GetDlgItem(IDC_SLD_GGAIN)->EnableWindow(FALSE); 
            GetDlgItem(IDC_SLD_BGAIN)->EnableWindow(FALSE); 

            GetDlgItem(IDC_STA_RGAIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_GGAIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_BGAIN)->EnableWindow(FALSE);

            GetDlgItem(IDC_EDT_RGAIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_GGAIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_BGAIN)->EnableWindow(FALSE);
        }
        else
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, 0);

            int nVal = 0;
            double dbVal = 1.0f;
            CString szText;
            // 获取当前Red通道增益
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
            {  
                nVal = (int)dbVal;
                szText.Format(_T("%.1f"), nVal / 2.0f);
                GetDlgItem(IDC_EDT_RGAIN)->SetWindowText(szText);  

                m_sldRGain.SetPos(nVal);
            }

            // 获取当前Green通道增益
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
            {  
                nVal = (int)dbVal;
                szText.Format(_T("%.1f"), nVal / 2.0f);
                GetDlgItem(IDC_EDT_GGAIN)->SetWindowText(szText);  

                m_sldGGain.SetPos(nVal);
            }

            // 获取当前Blue通道增益
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
            {  
                nVal = (int)dbVal;
                szText.Format(_T("%.1f"), nVal / 2.0f);
                GetDlgItem(IDC_EDT_BGAIN)->SetWindowText(szText);  

                m_sldBGain.SetPos(nVal);
            }

            GetDlgItem(IDC_SLD_RGAIN)->EnableWindow(TRUE); 
            GetDlgItem(IDC_SLD_GGAIN)->EnableWindow(TRUE); 
            GetDlgItem(IDC_SLD_BGAIN)->EnableWindow(TRUE); 

            GetDlgItem(IDC_STA_RGAIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_GGAIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_BGAIN)->EnableWindow(TRUE);

            GetDlgItem(IDC_EDT_RGAIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_GGAIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_BGAIN)->EnableWindow(TRUE);

        }
    }
}


void ColorControl::OnNMCustomdrawSldRgain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 设置Red通道，防止值被改变
	if (!m_bRGainRange)
	{
		m_bRGainRange = TRUE;
		return;
	}

	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, (m_sldRGain.GetPos() * 1.0f), 1);               

		CString szText = _T("");
		szText.Format(_T("%.1f"), m_sldRGain.GetPos() / 2.0f);
		GetDlgItem(IDC_EDT_RGAIN)->SetWindowText(szText);
	}

	*pResult = 0;
}

void ColorControl::OnNMCustomdrawSldGgain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 设置Green通道，防止值被改变
	if (!m_bGGainRange)
	{
		m_bGGainRange = TRUE;
		return;
	}

	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, (m_sldGGain.GetPos() * 1.0f), 2);               

		CString szText = _T("");
		szText.Format(_T("%.1f"), m_sldGGain.GetPos() / 2.0f);
		GetDlgItem(IDC_EDT_GGAIN)->SetWindowText(szText);
	}

	*pResult = 0;
}

void ColorControl::OnNMCustomdrawSldBgain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 设置Blue通道，防止值被改变
	if (!m_bBGainRange)
	{
		m_bBGainRange = TRUE;
		return;
	}

	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, (m_sldBGain.GetPos() * 1.0f), 3);               

		CString szText = _T("");
		szText.Format(_T("%.1f"), m_sldBGain.GetPos() / 2.0f);
		GetDlgItem(IDC_EDT_BGAIN)->SetWindowText(szText);
	}

	*pResult = 0;
}

void ColorControl::OnNMCustomdrawSldSaturation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 设置饱和度，防止值被改变
	if (!m_bSatRange)
	{
		m_bSatRange = TRUE;
		return;
	}

	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_SATURATION, m_sldSat.GetPos());               

		CString szText = _T("");
		szText.Format(_T("%d"), m_sldSat.GetPos());
		GetDlgItem(IDC_EDT_SATURATION)->SetWindowText(szText);
	}

	*pResult = 0;
}

void ColorControl::OnNMCustomdrawSldGamma(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 设伽马值，防止值被改变
	if (!m_bGammaRange)
	{
		m_bGammaRange = TRUE;
		return;
	}

	if (NULL != m_opCam.hIdxTUCam)
	{
		TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GAMMA, m_sldGamma.GetPos());               

		CString szText = _T("");
		szText.Format(_T("%.2f"), m_sldGamma.GetPos() / 100.0f);
		GetDlgItem(IDC_EDT_GAMMA)->SetWindowText(szText);
	}

	*pResult = 0;
}


void ColorControl::OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    // 设对比度值，防止值被改变
    if (!m_bContrastRange)
    {
        m_bContrastRange = TRUE;
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CONTRAST, m_sldContrast.GetPos());               

        CString szText = _T("");
        szText.Format(_T("%d"), m_sldContrast.GetPos());
        GetDlgItem(IDC_EDT_CONTRAST)->SetWindowText(szText);
    }

    *pResult = 0;

}


void ColorControl::OnNMCustomdrawSldCms(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    // 设CMS值，防止值被改变
    if (!m_bCMSRange)
    {
        m_bCMSRange = TRUE;
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CLRTEMPERATURE, m_sldCMS.GetPos());               

        CString szText = _T("");
        szText.Format(_T("%d"), m_sldCMS.GetPos());
        GetDlgItem(IDC_EDT_CMS)->SetWindowText(szText);


        int nVal = 0;
        double dbVal = 1.0f;
        // 获取当前Red通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_RGAIN)->SetWindowText(szText);  

            m_sldRGain.SetPos(nVal);
        }

        // 获取当前Green通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_GGAIN)->SetWindowText(szText);  

            m_sldGGain.SetPos(nVal);
        }

        // 获取当前Blue通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_BGAIN)->SetWindowText(szText);  

            m_sldBGain.SetPos(nVal);
        }
    }

    *pResult = 0;
}
