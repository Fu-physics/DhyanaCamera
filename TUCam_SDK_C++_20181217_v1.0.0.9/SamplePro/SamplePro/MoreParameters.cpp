// MoreParameters.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "MoreParameters.h"
//#include "afxdialogex.h"


#define IDT_GET_TEMPERATURE   0x64        
#define IDT_SPC_TEMPERATURE   1000  // Interval 1 Second
#define IDT_GET_WHITE_BALANCE 0x67
#define IDT_SPC_WHITE_BALANCE 1000  // Interval 2 Second
#define IDT_GET_AUTO_LFTLEVEL 0x68
#define IDT_SPC_AUTO_LFTLEVEL 1000  // Interval 0.6 Second
#define IDT_GET_AUTO_RGTLEVEL 0x69
#define IDT_SPC_AUTO_RGTLEVEL 1000  // Interval 0.6 Second
// MoreParameters dialog

IMPLEMENT_DYNAMIC(MoreParameters, CDialog)

MoreParameters::MoreParameters(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(MoreParameters::IDD, pParent)
{
	m_opCam              = opCam;
	m_ccmode             = pParent;
    m_bRLvlRange         = TRUE;
    m_bGammaRange        = TRUE;
    m_bContrastRange     = TRUE;
    m_bSharpRange        = TRUE;
    m_bRGainRange        = TRUE;
    m_bGGainRange        = TRUE;
    m_bBGainRange        = TRUE;
    m_bSatRange          = TRUE;
    m_bClrTempRange      = TRUE;
}

//MoreParameters::~MoreParameters()
//{
//}

void MoreParameters::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLD_LLEVEL, m_sldLLevel);
    DDX_Control(pDX, IDC_SLD_RLEVEL, m_sldRLevel);
    DDX_Control(pDX, IDC_SLD_COOL, m_sldCool);
    DDX_Control(pDX, IDC_COM_CURTEMP, m_cmbFan);
    DDX_Control(pDX, IDC_CHK_ATL, m_chkATLeft);
    DDX_Control(pDX, IDC_CHK_ATR, m_chkATRight);
    DDX_Control(pDX, IDC_SLD_CONTRAST, m_sldContrast);
    DDX_Control(pDX, IDC_SLD_SHARP, m_sldSharp);
    DDX_Control(pDX, IDC_SLD_GAMM, m_sldGamma);
    DDX_Control(pDX, IDC_SLD_RCHN, m_sldRGain);
    DDX_Control(pDX, IDC_SLD_GCHN, m_sldGGain);
    DDX_Control(pDX, IDC_SLD_BCHN, m_sldBGain);
    DDX_Control(pDX, IDC_SLD_SAT, m_sldSat);
    DDX_Control(pDX, IDC_SLD_CLRTEMP, m_sldClrTemp);
    DDX_Control(pDX, IDC_CHK_AB, m_chkWB);
    DDX_Control(pDX, IDC_CHK_DPC_OFF, m_chkDPCOff);
    DDX_Control(pDX, IDC_CHK_DPC_LOW, m_chkDPCLow);
    DDX_Control(pDX, IDC_CHK_DPC_MED, m_chkDPCMed);
    DDX_Control(pDX, IDC_CHK_DPC_HIGH, m_chkDPCHigh);
}


BEGIN_MESSAGE_MAP(MoreParameters, CDialog)
    ON_BN_CLICKED(IDC_CHK_ATL, &MoreParameters::OnBnClickedChkAtl)
    ON_BN_CLICKED(IDC_CHK_ATR, &MoreParameters::OnBnClickedChkAtr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_LLEVEL, &MoreParameters::OnNMCustomdrawSldLlevel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_RLEVEL, &MoreParameters::OnNMCustomdrawSldRlevel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_COOL, &MoreParameters::OnNMCustomdrawSldCool)
	ON_CBN_SELCHANGE(IDC_COM_CURTEMP, &MoreParameters::OnCbnSelchangeComCurtemp)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, &MoreParameters::OnBnClickedBtnDefault)
	ON_WM_TIMER()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_CONTRAST, &MoreParameters::OnNMCustomdrawSldContrast)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GAMM, &MoreParameters::OnNMCustomdrawSldGamm)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_SHARP, &MoreParameters::OnNMCustomdrawSldSharp)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_RCHN, &MoreParameters::OnNMCustomdrawSldRchn)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GCHN, &MoreParameters::OnNMCustomdrawSldGchn)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_BCHN, &MoreParameters::OnNMCustomdrawSldBchn)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_SAT, &MoreParameters::OnNMCustomdrawSldSat)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_CLRTEMP, &MoreParameters::OnNMCustomdrawSldClrtemp)
    ON_BN_CLICKED(IDC_BTN_WB, &MoreParameters::OnBnClickedBtnWb)
    ON_BN_CLICKED(IDC_CHK_AB, &MoreParameters::OnBnClickedChkAb)
    ON_BN_CLICKED(IDC_CHK_DPC_OFF, &MoreParameters::OnBnClickedChkDpcOff)
    ON_BN_CLICKED(IDC_CHK_DPC_LOW, &MoreParameters::OnBnClickedChkDpcLow)
    ON_BN_CLICKED(IDC_CHK_DPC_MED, &MoreParameters::OnBnClickedChkDpcMed)
    ON_BN_CLICKED(IDC_CHK_DPC_HIGH, &MoreParameters::OnBnClickedChkDpcHigh)
    ON_BN_CLICKED(IDC_BTN_BB, &MoreParameters::OnBnClickedBtnBb)
END_MESSAGE_MAP()


// MoreParameters message handlers
void MoreParameters::InitControlRange()
{
	if (NULL != m_opCam.hIdxTUCam)
    {
        char szRes[64] = {0};
        TUCAM_CAPA_ATTR  attrCapa;
        TUCAM_PROP_ATTR  attrProp;

		 // 获取自动色阶
        attrCapa.idCapa = TUIDC_ATLEVELS;
        if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
        {
            m_chkATLeft.EnableWindow(FALSE);
            m_chkATRight.EnableWindow(FALSE);
        }
        else
        {
            if (attrCapa.nValMax > 1)
            {
                m_chkATRight.ShowWindow(SW_SHOW);
                m_chkATLeft.ShowWindow(SW_SHOW);
            }
            else
            {
                m_chkATRight.ShowWindow(SW_HIDE);
                m_chkATLeft.ShowWindow(SW_HIDE);
            }

            m_chkATLeft.EnableWindow(TRUE);
            m_chkATRight.EnableWindow(TRUE);
        }

        // 获取色阶范围
        attrProp.nIdxChn= 0;                    // 使用默认通道
        attrProp.idProp = TUIDP_LFTLEVELS;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_sldLLevel.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
            m_sldLLevel.SetPos((int)attrProp.dbValMin);
        }

        attrProp.idProp = TUIDP_RGTLEVELS;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_bRLvlRange = FALSE;
            m_sldRLevel.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
            m_sldRLevel.SetPos((int)attrProp.dbValMax);
        }

        // 获取伽玛范围
        attrProp.idProp = TUIDP_GAMMA;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_bGammaRange = FALSE;
            m_sldGamma.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
            m_sldGamma.SetPos((int)attrProp.dbValMax);

            GetDlgItem(IDC_STA_GAMMA)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_GAMM)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_GAMMA)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_STA_GAMMA)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_GAMM)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_GAMMA)->EnableWindow(FALSE);
        }

        // 获取对比度范围
        attrProp.idProp = TUIDP_CONTRAST;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_bContrastRange = FALSE;
            m_sldContrast.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
            m_sldContrast.SetPos((int)attrProp.dbValMax);

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

        // 获取锐化范围
        attrProp.idProp = TUIDP_SHARPNESS;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_bSharpRange = FALSE;
            m_sldSharp.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
            m_sldSharp.SetPos((int)attrProp.dbValMax);

            GetDlgItem(IDC_STA_SHARP)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_SHARP)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_SHARP)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_STA_SHARP)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_SHARP)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_SHARP)->EnableWindow(FALSE);
        }

        // 获取色温范围
        attrProp.idProp = TUIDP_CLRTEMPERATURE;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_bClrTempRange = FALSE;
            m_sldClrTemp.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);

            GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(TRUE);

            GetDlgItem(IDC_STA_CLRTEMP)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_SLD_CLRTEMP)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_EDT_CLRTEMP)->ShowWindow(SW_SHOW);
        }
        else
        {
            GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(FALSE);

            GetDlgItem(IDC_STA_CLRTEMP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_SLD_CLRTEMP)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDT_CLRTEMP)->ShowWindow(SW_HIDE);
        }

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

            GetDlgItem(IDC_STA_RCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_RCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_RCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_GCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_GCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_GCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_BCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_BCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_BCHN)->EnableWindow(TRUE);

            GetDlgItem(IDC_STA_RCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_SLD_RCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_EDT_RCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_STA_GCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_SLD_GCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_EDT_GCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_STA_BCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_SLD_BCHN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_EDT_BCHN)->ShowWindow(SW_SHOW);

            GetDlgItem(IDC_GROUP_CLRCTRL)->ShowWindow(SW_SHOW);
        }
        else
        {
            GetDlgItem(IDC_STA_RCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_RCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_RCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_GCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_GCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_GCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_BCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_BCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_BCHN)->EnableWindow(FALSE);

            GetDlgItem(IDC_STA_RCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_SLD_RCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDT_RCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STA_GCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_SLD_GCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDT_GCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STA_BCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_SLD_BCHN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDT_BCHN)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_GROUP_CLRCTRL)->ShowWindow(SW_HIDE);
        }

        // 获取自动白平衡
        attrCapa.idCapa = TUIDC_ATWBALANCE;
        if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
        {
            GetDlgItem(IDC_BTN_WB)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_AB)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_AB)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BTN_WB)->ShowWindow(SW_HIDE);
        }
        else
        {
            if (attrCapa.nValMax > 1)
            {
                GetDlgItem(IDC_BTN_WB)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_CHK_AB)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_CHK_AB)->EnableWindow(TRUE);
            }
            else
            {
                GetDlgItem(IDC_CHK_AB)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_BTN_WB)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_BTN_WB)->EnableWindow(TRUE);
            }
        }

        // 获取黑平衡
        attrCapa.idCapa = TUIDC_BLACKBALANCE;
        if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
        {
            GetDlgItem(IDC_BTN_BB)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_BB)->ShowWindow(SW_HIDE);
        }
        else
        {
            GetDlgItem(IDC_BTN_BB)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_BB)->ShowWindow(SW_SHOW);
        }

        // 获取饱和度范围
        attrProp.idProp = TUIDP_SATURATION;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_bSatRange = FALSE;
            m_sldSat.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);

            GetDlgItem(IDC_STA_SAT)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_SAT)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_SAT)->EnableWindow(TRUE);

            GetDlgItem(IDC_STA_SAT)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_SLD_SAT)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_EDT_SAT)->ShowWindow(SW_SHOW);
        }
        else
        {
            GetDlgItem(IDC_STA_SAT)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_SAT)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_SAT)->EnableWindow(FALSE);

            GetDlgItem(IDC_STA_SAT)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_SLD_SAT)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDT_SAT)->ShowWindow(SW_HIDE);
        }

        // 获取降噪范围
        attrProp.idProp = TUIDP_NOISELEVEL;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            GetDlgItem(IDC_GROUP_DPC)->EnableWindow(TRUE);

            GetDlgItem(IDC_CHK_DPC_OFF)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHK_DPC_LOW)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHK_DPC_MED)->EnableWindow(TRUE);
            GetDlgItem(IDC_CHK_DPC_HIGH)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_GROUP_DPC)->EnableWindow(FALSE);

            GetDlgItem(IDC_CHK_DPC_OFF)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_DPC_LOW)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_DPC_MED)->EnableWindow(FALSE);
            GetDlgItem(IDC_CHK_DPC_HIGH)->EnableWindow(FALSE);
        }

		// 获取温度范围
        attrProp.idProp = TUIDP_TEMPERATURE;
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
        {
            m_nMidTemp = (int)(attrProp.dbValMax / 2);
            m_sldCool.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
            m_sldCool.SetPos(m_nMidTemp);

            CString szTemp = _T("0 ℃");
            GetDlgItem(IDC_EDT_COOL)->SetWindowText(szTemp);
        }
        else
        {
            CString szTemp = _T("0 ℃");
            GetDlgItem(IDC_EDT_COOL)->SetWindowText(szTemp);

            m_sldCool.EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_COOL)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_COOL)->EnableWindow(FALSE);

            GetDlgItem(IDC_STA_TEMP)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_CURTMP)->EnableWindow(FALSE);
        }

        // 获取风扇档位范围
        attrCapa.idCapa = TUIDC_FAN_GEAR;
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
        {
            CString szFan;
            int nCnt = attrCapa.nValMax - attrCapa.nValMin + 1;
            
            for (int i=0; i<nCnt; ++i)
            {
                szFan.Format(_T("Fan %d"), (i+1));               
                m_cmbFan.InsertString(i, szFan);
            }

            m_cmbFan.SetCurSel(0);
        } 
        else
        {
            m_cmbFan.EnableWindow(FALSE);
        }
	}
//	SetTimer(IDT_GET_TEMPERATURE, IDT_SPC_TEMPERATURE, NULL);
}

void  MoreParameters::RefreshValue()                              // 初始化值
{
	if (NULL != m_opCam.hIdxTUCam)
    {
		// Set value
		int     nVal  = 0;
        double  dbVal = 0;
		CString szText= _T("");

        // 获取风扇档位
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_FAN_GEAR, &nVal))
        {
            m_cmbFan.SetCurSel(nVal);
        }

		// 获取当前自动色阶状态
        if (TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, &nVal))
        {           
            // 左自动色阶
            if (nVal & 0x01)
            {
                m_chkATLeft.SetCheck(1);

                GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(FALSE);
                GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(FALSE);

                 SetTimer(IDT_GET_AUTO_LFTLEVEL, IDT_SPC_AUTO_LFTLEVEL, NULL);
            }
            else
            {
                m_chkATLeft.SetCheck(0);

                GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(TRUE);
                GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(TRUE);

                KillTimer(IDT_GET_AUTO_LFTLEVEL);
            }

            // 右自动色阶
            if (nVal & 0x02)
            {
                m_chkATRight.SetCheck(1);

                GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(FALSE);
                GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(FALSE);

                SetTimer(IDT_GET_AUTO_RGTLEVEL, IDT_SPC_AUTO_RGTLEVEL, NULL);
            }
            else
            {
                m_chkATRight.SetCheck(0);

                GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(TRUE);
                GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(TRUE);

                KillTimer(IDT_GET_AUTO_RGTLEVEL);
            }
        }
       
        // 获取当前左色阶值
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, &dbVal))
        {    
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);    

            m_sldLLevel.SetPos(nVal);
        }

        // 获取当前右色阶值
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, &dbVal))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);  

            m_sldRLevel.SetPos(nVal);
        }

        // 获取当前伽玛值
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

        // 获取当前锐化值
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_SHARPNESS, &dbVal))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_SHARP)->SetWindowText(szText);  

            m_sldSharp.SetPos(nVal);
        }

        // 获取白平衡状态
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, &nVal))
        {
            if (nVal > 1)
            {
                m_chkWB.SetCheck(1);

                GetDlgItem(IDC_SLD_RCHN)->EnableWindow(FALSE); 
                GetDlgItem(IDC_SLD_GCHN)->EnableWindow(FALSE); 
                GetDlgItem(IDC_SLD_BCHN)->EnableWindow(FALSE); 

                GetDlgItem(IDC_STA_RCHN)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_GCHN)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_BCHN)->EnableWindow(FALSE);

                GetDlgItem(IDC_EDT_RCHN)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_GCHN)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_BCHN)->EnableWindow(FALSE);

                GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(FALSE);
                GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(FALSE);

                GetDlgItem(IDC_BTN_BB)->EnableWindow(FALSE);
            }
            else
            {
                m_chkWB.SetCheck(0);

                GetDlgItem(IDC_SLD_RCHN)->EnableWindow(TRUE); 
                GetDlgItem(IDC_SLD_GCHN)->EnableWindow(TRUE); 
                GetDlgItem(IDC_SLD_BCHN)->EnableWindow(TRUE); 

                GetDlgItem(IDC_STA_RCHN)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_GCHN)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_BCHN)->EnableWindow(TRUE);

                GetDlgItem(IDC_EDT_RCHN)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_GCHN)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_BCHN)->EnableWindow(TRUE);

                // 获取当前色温
                if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CLRTEMPERATURE, &dbVal))
                {  
                    nVal = (int)dbVal;
                    szText.Format(_T("%d"), nVal);
                    GetDlgItem(IDC_EDT_CLRTEMP)->SetWindowText(szText);  

                    m_sldClrTemp.SetPos(nVal);

                    GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(TRUE);
                    GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(TRUE);
                    GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(TRUE);

                }
                else
                {
                    GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(FALSE);
                    GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(FALSE);
                    GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(FALSE);
                }

                TUCAM_CAPA_ATTR attrCapa;
                attrCapa.idCapa = TUIDC_BLACKBALANCE;
                if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
                {
                    GetDlgItem(IDC_BTN_BB)->EnableWindow(TRUE);
                }
                else
                {
                    GetDlgItem(IDC_BTN_BB)->EnableWindow(FALSE);
                }
            }
        }

        // 获取当前Red通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_RCHN)->SetWindowText(szText);  

            m_sldRGain.SetPos(nVal);
        }

        // 获取当前Green通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_GCHN)->SetWindowText(szText);  

            m_sldGGain.SetPos(nVal);
        }

        // 获取当前Blue通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_BCHN)->SetWindowText(szText);  

            m_sldBGain.SetPos(nVal);
        }

        // 获取当前饱和度
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_SATURATION, &dbVal))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_SAT)->SetWindowText(szText);  

            m_sldSat.SetPos(nVal);
        }

        // 获取降噪值
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_NOISELEVEL, &dbVal))
        {  
            nVal = (int)dbVal;
            
            switch (nVal)
            {
            case 1:
                {
                    m_chkDPCOff.SetCheck(FALSE);
                    m_chkDPCLow.SetCheck(TRUE);
                    m_chkDPCMed.SetCheck(FALSE);
                    m_chkDPCHigh.SetCheck(FALSE);
                }
                break;
            case 2:
                {
                    m_chkDPCOff.SetCheck(FALSE);
                    m_chkDPCLow.SetCheck(FALSE);
                    m_chkDPCMed.SetCheck(TRUE);
                    m_chkDPCHigh.SetCheck(FALSE);
                }
                break;
            case 3:
                {
                    m_chkDPCOff.SetCheck(FALSE);
                    m_chkDPCLow.SetCheck(FALSE);
                    m_chkDPCMed.SetCheck(FALSE);
                    m_chkDPCHigh.SetCheck(TRUE);
                }
                break;
            case 0:
            default:
                {
                    m_chkDPCOff.SetCheck(TRUE);
                    m_chkDPCLow.SetCheck(FALSE);
                    m_chkDPCMed.SetCheck(FALSE);
                    m_chkDPCHigh.SetCheck(FALSE);
                }
                break;
            }
        }
	}
}

void MoreParameters::RefreshBtn()
{
	GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(TRUE);
}

void MoreParameters::RefreshLevelRange()
{
	TUCAM_PROP_ATTR  attrProp;
    // 获取色阶范围
    attrProp.nIdxChn= 0;                    // 使用默认通道
    attrProp.idProp = TUIDP_LFTLEVELS;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        m_sldLLevel.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
        m_sldLLevel.SetPos((int)attrProp.dbValMin);
    }

    attrProp.idProp = TUIDP_RGTLEVELS;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        m_bRLvlRange = FALSE;
        m_sldRLevel.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
        m_sldRLevel.SetPos((int)attrProp.dbValMax);
	}

	int     nVal  = 0;
    double  dbVal = 0;
	CString szText= _T("");
	// 获取当前左色阶值
	if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, &dbVal))
	{    
		if (TUCAMRET_SUCCESS == TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, dbVal)) 
		{
			nVal = (int)dbVal;
			szText.Format(_T("%d"), nVal);
			GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);    

			m_sldLLevel.SetPos(nVal);
		}
	}

	// 获取当前右色阶值
	if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, &dbVal))
	{  
		if (TUCAMRET_SUCCESS == TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, dbVal)) 
		{
			nVal = (int)dbVal;
			szText.Format(_T("%d"), nVal);
			GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);  

			m_sldRLevel.SetPos(nVal);
		}
	}
}

void MoreParameters::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    if (IDT_GET_TEMPERATURE == nIDEvent) 
    {
        double dblVal = 1.0f;

        if (NULL != m_opCam.hIdxTUCam)
        {
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_TEMPERATURE, &dblVal))
            {
                CString sz;
                sz.Format(_T("%.2f ℃"), dblVal);
                GetDlgItem(IDC_STA_TEMP)->SetWindowText(sz);
            }
        }
    }
    else if (IDT_GET_WHITE_BALANCE == nIDEvent)
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
            GetDlgItem(IDC_EDT_RCHN)->SetWindowText(szText);  

            m_sldRGain.SetPos(nVal);
        }

        // 获取当前Green通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_GCHN)->SetWindowText(szText);  

            m_sldGGain.SetPos(nVal);
        }

        // 获取当前Blue通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_BCHN)->SetWindowText(szText);  

            m_sldBGain.SetPos(nVal);
        }
    }
    else if (IDT_GET_AUTO_RGTLEVEL == nIDEvent)
    {
        int nVal = 0;
        double dbVal = 1.0f;
        CString szText;
        // 获取当前Red通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, &dbVal))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);  

            m_sldRLevel.SetPos(nVal);
        } 
    }
    else if (IDT_GET_AUTO_LFTLEVEL == nIDEvent)
    {
        int nVal = 0;
        double dbVal = 1.0f;
        CString szText;
        // 获取当前Red通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, &dbVal))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);  

            m_sldLLevel.SetPos(nVal);
        } 
    }
   
    CDialog::OnTimer(nIDEvent);
}

BOOL MoreParameters::PreTranslateMessage(MSG* pMsg)
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

/*
void MoreParameters::OnBnClickedChkAutolevel()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        if (m_chkATLevels.GetCheck())
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, 3);

            GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(FALSE);
        }
        else
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, 0);

            GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(TRUE);


            int nVal = 0;
            double dbVal = 1.0f;
            CString szText;

            // 获取当前左色阶值
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, &dbVal))
            {    
                nVal = (int)dbVal;
                szText.Format(_T("%d"), nVal);
                GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);    

                m_sldLLevel.SetPos(nVal);
            }

            // 获取当前右色阶值
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, &dbVal))
            {  
                nVal = (int)dbVal;
                szText.Format(_T("%d"), nVal);
                GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);  

                m_sldRLevel.SetPos(nVal);
            }
        }
    }
}
*/

void MoreParameters::OnNMCustomdrawSldLlevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (NULL != m_sldLLevel.GetSafeHwnd() && NULL != m_sldRLevel.GetSafeHwnd())
    {
        int nLLevel = m_sldLLevel.GetPos();
        int nRLevel = m_sldRLevel.GetPos();

        if (nLLevel >= nRLevel)
        {
            nRLevel = nLLevel + 1;
            m_sldRLevel.SetPos(nRLevel);

            CString szText = _T("");
            szText.Format(_T("%d"), nRLevel);
            GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);
        }

        // 设置左色阶值
        if (NULL != m_opCam.hIdxTUCam)
        {
            if (TUCAMRET_SUCCESS == TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, (1.0f * m_sldLLevel.GetPos()))) 
            {
                CString szText = _T("");
                szText.Format(_T("%d"), m_sldLLevel.GetPos());
                GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);
            }
        }
    }
	*pResult = 0;
}


void MoreParameters::OnBnClickedChkAtl()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        int nLVal = m_chkATLeft.GetCheck() ? 1 : 0;
        int nRVal = m_chkATRight.GetCheck() ? 1 : 0;
        int nVal  = (nRVal << 1) | nLVal;

        if (0 != nVal)
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_HISTC, 1);
        }
        else
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_HISTC, 0);
        }

        if (m_chkATLeft.GetCheck())
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, nVal);

            GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(FALSE);

            SetTimer(IDT_GET_AUTO_LFTLEVEL, IDT_SPC_AUTO_LFTLEVEL, NULL);
        }
        else
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, nVal);

            GetDlgItem(IDC_STA_LLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_LLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_LLEVEL)->EnableWindow(TRUE);

            KillTimer(IDT_GET_AUTO_LFTLEVEL);

            int nVal = 0;
            double dbVal = 1.0f;
            CString szText;

            // 获取当前左色阶值
            TUCAM_PROP_ATTR attrProp;
            attrProp.nIdxChn= 0;                
            attrProp.idProp = TUIDP_LFTLEVELS;
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
            {
                TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, attrProp.dbValDft);

                nVal = (int)attrProp.dbValDft;
                szText.Format(_T("%d"), nVal);
                GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);    

                m_sldLLevel.SetPos(nVal);
            }
        }
    }
}

void MoreParameters::OnNMCustomdrawSldRlevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (!m_bRLvlRange)
    {
        m_bRLvlRange = TRUE;
        return;
    }

    if (NULL != m_sldLLevel.GetSafeHwnd() && NULL != m_sldRLevel.GetSafeHwnd())
    {
        int nLLevel = m_sldLLevel.GetPos();
        int nRLevel = m_sldRLevel.GetPos();

        if (nLLevel >= nRLevel)
        {
            nLLevel = nRLevel-1;
            nLLevel = nLLevel < 0 ?  0 : nLLevel;
            m_sldLLevel.SetPos(nLLevel);

            CString szText = _T("");
            szText.Format(_T("%d"), nLLevel);
            GetDlgItem(IDC_EDT_LLEVEL)->SetWindowText(szText);
        }

        // 设置右色阶值
        if (NULL != m_opCam.hIdxTUCam)
        {
            if (TUCAMRET_SUCCESS == TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, (1.0f * m_sldRLevel.GetPos()))) 
            {
                CString szText = _T("");
                szText.Format(_T("%d"), m_sldRLevel.GetPos());
                GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);
            }
        }
    }
	*pResult = 0;
}


void MoreParameters::OnBnClickedChkAtr()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        int nLVal = m_chkATLeft.GetCheck() ? 1 : 0;
        int nRVal = m_chkATRight.GetCheck() ? 1 : 0;
        int nVal  = (nRVal << 1) | nLVal;

        if (0 != nVal)
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_HISTC, 1);
        }
        else
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_HISTC, 0);
        }

        if (m_chkATRight.GetCheck())
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, nVal);

            SetTimer(IDT_GET_AUTO_RGTLEVEL, IDT_SPC_AUTO_RGTLEVEL, NULL);

            GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(FALSE);
        }
        else
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, nVal);

            GetDlgItem(IDC_STA_RLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_SLD_RLEVEL)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_RLEVEL)->EnableWindow(TRUE);

            KillTimer(IDT_GET_AUTO_RGTLEVEL);

            int nVal = 0;
            double dbVal = 1.0f;
            CString szText;

            // 获取当前右色阶值
            TUCAM_PROP_ATTR attrProp;
            attrProp.nIdxChn= 0;                
            attrProp.idProp = TUIDP_RGTLEVELS;
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
            {
                TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, attrProp.dbValDft);

                nVal = (int)attrProp.dbValDft;
                szText.Format(_T("%d"), nVal);
                GetDlgItem(IDC_EDT_RLEVEL)->SetWindowText(szText);    

                m_sldRLevel.SetPos(nVal);
            }
        }
    }
}


void MoreParameters::OnNMCustomdrawSldCool(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam && m_sldCool.IsWindowEnabled())
    {
        CString szTemp;
        int nVal = m_sldCool.GetPos();
        szTemp.Format(_T("%d ℃"), (nVal - m_nMidTemp));
        GetDlgItem(IDC_EDT_COOL)->SetWindowText(szTemp);

        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_TEMPERATURE, nVal);
    }
	*pResult = 0;
}


void MoreParameters::OnCbnSelchangeComCurtemp()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_FAN_GEAR, m_cmbFan.GetCurSel());
    }
}


void MoreParameters::OnBnClickedBtnDefault()
{
	// TODO: Add your control notification handler code here
    char szRes[64] = {0};
    TUCAM_CAPA_ATTR  attrCapa;
    TUCAM_PROP_ATTR  attrProp;

    // 获取自动色阶
    attrCapa.idCapa = TUIDC_ATLEVELS;
    if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
    {
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATLEVELS, attrCapa.nValDft);
    }

    // 获取色阶范围
    // 使用默认通道
    attrProp.nIdxChn= 0;                
    attrProp.idProp = TUIDP_LFTLEVELS;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_LFTLEVELS, attrProp.dbValDft);
    }

    attrProp.idProp = TUIDP_RGTLEVELS;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_RGTLEVELS, attrProp.dbValDft);
    }

    // 获取伽玛范围
    attrProp.idProp = TUIDP_GAMMA;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GAMMA, attrProp.dbValDft);
    }

    // 获取对比度范围
    attrProp.idProp = TUIDP_CONTRAST;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CONTRAST, attrProp.dbValDft);
    }

    // 获取锐化范围
    attrProp.idProp = TUIDP_SHARPNESS;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_SHARPNESS, attrProp.dbValDft);
    }

    // 获取色温范围
    attrProp.idProp = TUIDP_CLRTEMPERATURE;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CLRTEMPERATURE, attrProp.dbValDft);
    }

    // 获取色彩增益
    attrProp.idProp = TUIDP_CHNLGAIN;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, attrProp.dbValDft, 1);
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, attrProp.dbValDft, 2);
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, attrProp.dbValDft, 3);
    }

    // 获取自动白平衡
    attrCapa.idCapa = TUIDC_ATWBALANCE;
    if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
    {
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, attrCapa.nValDft);
    }

    // 获取饱和度范围
    attrProp.idProp = TUIDP_SATURATION;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_SATURATION, attrProp.dbValDft);
    }

    // 获取噪声值
    attrProp.idProp = TUIDP_NOISELEVEL;
    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_NOISELEVEL, attrProp.dbValDft);
    }

    RefreshValue();
}

void MoreParameters::OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult)
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

void MoreParameters::OnNMCustomdrawSldGamm(NMHDR *pNMHDR, LRESULT *pResult)
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

void MoreParameters::OnNMCustomdrawSldSharp(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    // 设对锐化值，防止值被改变
    if (!m_bSharpRange)
    {
        m_bSharpRange = TRUE;
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_SHARPNESS, m_sldSharp.GetPos());               

        CString szText = _T("");
        szText.Format(_T("%d"), m_sldSharp.GetPos());
        GetDlgItem(IDC_EDT_SHARP)->SetWindowText(szText);
    }

    *pResult = 0;
}

void MoreParameters::OnNMCustomdrawSldRchn(NMHDR *pNMHDR, LRESULT *pResult)
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
        GetDlgItem(IDC_EDT_RCHN)->SetWindowText(szText);
    }

    *pResult = 0;
}

void MoreParameters::OnNMCustomdrawSldGchn(NMHDR *pNMHDR, LRESULT *pResult)
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
        GetDlgItem(IDC_EDT_GCHN)->SetWindowText(szText);
    }

    *pResult = 0;
}

void MoreParameters::OnNMCustomdrawSldBchn(NMHDR *pNMHDR, LRESULT *pResult)
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
        GetDlgItem(IDC_EDT_BCHN)->SetWindowText(szText);
    }


    *pResult = 0;
}

void MoreParameters::OnNMCustomdrawSldSat(NMHDR *pNMHDR, LRESULT *pResult)
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
        GetDlgItem(IDC_EDT_SAT)->SetWindowText(szText);
    }

    *pResult = 0;
}

void MoreParameters::OnNMCustomdrawSldClrtemp(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    // 设CMS值，防止值被改变
    if (!m_bClrTempRange)
    {
        m_bClrTempRange = TRUE;
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_CLRTEMPERATURE, m_sldClrTemp.GetPos());               

        CString szText = _T("");
        szText.Format(_T("%d"), m_sldClrTemp.GetPos());
        GetDlgItem(IDC_EDT_CLRTEMP)->SetWindowText(szText);


        int nVal = 0;
        double dbVal = 1.0f;
        // 获取当前Red通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_RCHN)->SetWindowText(szText);  

            m_sldRGain.SetPos(nVal);
        }

        // 获取当前Green通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_GCHN)->SetWindowText(szText);  

            m_sldGGain.SetPos(nVal);
        }

        // 获取当前Blue通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_BCHN)->SetWindowText(szText);  

            m_sldBGain.SetPos(nVal);
        }
    }


    *pResult = 0;
}

void MoreParameters::OnBnClickedBtnWb()
{
    // TODO: 在此添加控件通知处理程序代码

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, 1);

        SetTimer(IDT_GET_WHITE_BALANCE, IDT_SPC_WHITE_BALANCE, NULL);
    }
}

void MoreParameters::OnBnClickedChkAb()
{
    // TODO: 在此添加控件通知处理程序代码
    if (NULL != m_opCam.hIdxTUCam)
    {
        if (m_chkWB.GetCheck())
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATWBALANCE, 2);

            GetDlgItem(IDC_SLD_RCHN)->EnableWindow(FALSE); 
            GetDlgItem(IDC_SLD_GCHN)->EnableWindow(FALSE); 
            GetDlgItem(IDC_SLD_BCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(FALSE);

            GetDlgItem(IDC_STA_RCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_GCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_BCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(FALSE);

            GetDlgItem(IDC_EDT_RCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_GCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_BCHN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(FALSE);

            GetDlgItem(IDC_BTN_BB)->EnableWindow(FALSE);
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
                GetDlgItem(IDC_EDT_RCHN)->SetWindowText(szText);  

                m_sldRGain.SetPos(nVal);
            }

            // 获取当前Green通道增益
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
            {  
                nVal = (int)dbVal;
                szText.Format(_T("%.1f"), nVal / 2.0f);
                GetDlgItem(IDC_EDT_GCHN)->SetWindowText(szText);  

                m_sldGGain.SetPos(nVal);
            }

            // 获取当前Blue通道增益
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
            {  
                nVal = (int)dbVal;
                szText.Format(_T("%.1f"), nVal / 2.0f);
                GetDlgItem(IDC_EDT_BCHN)->SetWindowText(szText);  

                m_sldBGain.SetPos(nVal);
            }

            GetDlgItem(IDC_SLD_RCHN)->EnableWindow(TRUE); 
            GetDlgItem(IDC_SLD_GCHN)->EnableWindow(TRUE); 
            GetDlgItem(IDC_SLD_BCHN)->EnableWindow(TRUE); 

            GetDlgItem(IDC_STA_RCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_GCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_STA_BCHN)->EnableWindow(TRUE);

            GetDlgItem(IDC_EDT_RCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_GCHN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDT_BCHN)->EnableWindow(TRUE);

            // 获取色温范围
            TUCAM_PROP_ATTR attrProp;
            attrProp.idProp = TUIDP_CLRTEMPERATURE;
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
            {
                GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(TRUE);
                GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(TRUE);
            }
            else
            {
                GetDlgItem(IDC_STA_CLRTEMP)->EnableWindow(FALSE);
                GetDlgItem(IDC_SLD_CLRTEMP)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_CLRTEMP)->EnableWindow(FALSE);
            }

            TUCAM_CAPA_ATTR attrCapa;
            attrCapa.idCapa = TUIDC_BLACKBALANCE;
            if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
            {
                GetDlgItem(IDC_BTN_BB)->EnableWindow(TRUE);
            }
            else
            {
                GetDlgItem(IDC_BTN_BB)->EnableWindow(FALSE);
            }
        }
    }
}

void MoreParameters::OnBnClickedChkDpcOff()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_chkDPCOff.GetCheck() && !m_chkDPCLow.GetCheck() && !m_chkDPCMed.GetCheck() && !m_chkDPCHigh.GetCheck())
    {
        m_chkDPCOff.SetCheck(TRUE);
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_NOISELEVEL, 0);

        m_chkDPCLow.SetCheck(FALSE);
        m_chkDPCMed.SetCheck(FALSE);
        m_chkDPCHigh.SetCheck(FALSE);
    }
}

void MoreParameters::OnBnClickedChkDpcLow()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_chkDPCOff.GetCheck() && !m_chkDPCLow.GetCheck() && !m_chkDPCMed.GetCheck() && !m_chkDPCHigh.GetCheck())
    {
        m_chkDPCLow.SetCheck(TRUE);
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_NOISELEVEL, 1);

        m_chkDPCOff.SetCheck(FALSE);
        m_chkDPCMed.SetCheck(FALSE);
        m_chkDPCHigh.SetCheck(FALSE);
    }
}

void MoreParameters::OnBnClickedChkDpcMed()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_chkDPCOff.GetCheck() && !m_chkDPCLow.GetCheck() && !m_chkDPCMed.GetCheck() && !m_chkDPCHigh.GetCheck())
    {
        m_chkDPCMed.SetCheck(TRUE);
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_NOISELEVEL, 2);

        m_chkDPCOff.SetCheck(FALSE);
        m_chkDPCLow.SetCheck(FALSE);
        m_chkDPCHigh.SetCheck(FALSE);
    }
}

void MoreParameters::OnBnClickedChkDpcHigh()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!m_chkDPCOff.GetCheck() && !m_chkDPCLow.GetCheck() && !m_chkDPCMed.GetCheck() && !m_chkDPCHigh.GetCheck())
    {
        m_chkDPCHigh.SetCheck(TRUE);
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_NOISELEVEL, 3);

        m_chkDPCOff.SetCheck(FALSE);
        m_chkDPCLow.SetCheck(FALSE);
        m_chkDPCMed.SetCheck(FALSE);
    }
}

void MoreParameters::OnBnClickedBtnBb()
{
    // TODO: 在此添加控件通知处理程序代码

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_BLACKBALANCE, 1);

        int nVal = 0;
        double dbVal = 1.0f;
        CString szText;
        // 获取当前Red通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 1))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_RCHN)->SetWindowText(szText);  

            m_sldRGain.SetPos(nVal);
        }

        // 获取当前Green通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 2))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_GCHN)->SetWindowText(szText);  

            m_sldGGain.SetPos(nVal);
        }

        // 获取当前Blue通道增益
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_CHNLGAIN, &dbVal, 3))
        {  
            nVal = (int)dbVal;
            szText.Format(_T("%.1f"), nVal / 2.0f);
            GetDlgItem(IDC_EDT_BCHN)->SetWindowText(szText);  

            m_sldBGain.SetPos(nVal);
        }
    }
}
