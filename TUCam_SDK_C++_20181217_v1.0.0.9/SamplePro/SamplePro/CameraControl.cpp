// CameraControl.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "CameraControl.h"
#include "Trigger.h"
//#include "afxdialogex.h"


#define IDT_GET_TEMPERATURE     0x64        
#define IDT_SPC_TEMPERATURE     1000  // Interval 1 Second
#define IDT_GET_FPS_RATE        0x68
#define IDT_SPC_FPS_RATE        1000  // Interval 1 Second
#define IDT_GET_ONCE_AE         0x69
#define IDT_SPC_ONCE_AE         500   // Interval 0.5 Second
// CameraControl dialog

IMPLEMENT_DYNAMIC(CameraControl, CDialog)

CameraControl::CameraControl(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(CameraControl::IDD, pParent)
	, m_nGain(0)
	, m_sRegAddress(_T("20"))
	, m_sValue(_T("01"))
{
	m_opCam                 = opCam;
	m_ccmode                = pParent;
	m_uiExpStep             = 1;
	m_bTargetRange          = FALSE;
	m_uSec					= 0;
	m_uMs					= 0;
	m_uUs					= 0;
	m_bFirst                = TRUE;
	m_ucount                = 0;
	m_favg                  = 0.0;
	m_ffast                 = 0.0;
	m_fslow                 = 0.0;
	m_bGainRange            = TRUE;
	m_bLedState             = FALSE;
	m_bCmsEn                = FALSE;
 }

CameraControl::~CameraControl()
{

}

void CameraControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_RES, m_cmbRes);
	DDX_Control(pDX, IDC_CHK_BITOFDEPTH, m_chkBDep);
	DDX_Control(pDX, IDC_CHK_AE, m_chkAE);
	DDX_Control(pDX, IDC_SLD_TGR, m_sldTgr);
	DDX_Control(pDX, IDC_SLD_GAIN, m_sldGain);
	DDX_Text(pDX, IDC_EDT_SEC, m_uSec);
	DDX_Text(pDX, IDC_EDT_MS, m_uMs);
	DDX_Text(pDX, IDC_EDT_US, m_uUs);
	DDX_Control(pDX, IDC_COMBO_SLAADD, m_SlaveAddress);

	DDX_Text(pDX, IDC_EDT_REGADD, m_sRegAddress);
	DDX_Text(pDX, IDC_EDT_VALUE, m_sValue);
	DDX_Control(pDX, IDC_COM__BITOFDEPTH, m_cmbBitOfDepth);
}


BEGIN_MESSAGE_MAP(CameraControl, CDialog)
	ON_BN_CLICKED(IDC_BTN_LIVE, &CameraControl::OnBnClickedBtnLive)
	ON_CBN_SELCHANGE(IDC_CMB_RES, &CameraControl::OnCbnSelchangeCmbRes)
	ON_BN_CLICKED(IDC_CHK_AE, &CameraControl::OnBnClickedChkAe)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_TGR, &CameraControl::OnNMCustomdrawSldTgr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_SEC, &CameraControl::OnDeltaposSpnSec)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_MS, &CameraControl::OnDeltaposSpnMs)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_US, &CameraControl::OnDeltaposSpnUs)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GAIN, &CameraControl::OnNMCustomdrawSldGain)
	ON_BN_CLICKED(IDC_BTN_EXPTM, &CameraControl::OnBnClickedBtnExptm)
	ON_BN_CLICKED(IDC_CHK_BITOFDEPTH, &CameraControl::OnBnClickedChkBitofdepth)
	ON_BN_CLICKED(IDC_RADIO_HDR, &CameraControl::OnBnClickedRadioHdr)
	ON_BN_CLICKED(IDC_RADIO_HIGHGAIN, &CameraControl::OnBnClickedRadioHighgain)
	ON_BN_CLICKED(IDC_RADIO_LOWGAIN, &CameraControl::OnBnClickedRadioLowgain)
	ON_WM_TIMER()
	ON_MESSAGE(TUMSG_FPS, OnTUMessageFps)
	ON_MESSAGE(TUMSG_MEAN, OnTUMessageMean)
	ON_MESSAGE(TUMSG_STD, OnTUMessageStd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_ONCEAE, &CameraControl::OnBnClickedBtnOnceae)
	ON_BN_CLICKED(IDC_BTN_W, &CameraControl::OnBnClickedBtnW)
	ON_BN_CLICKED(IDC_BTN_R, &CameraControl::OnBnClickedBtnR)
	ON_BN_CLICKED(IDC_RADIO_CMS, &CameraControl::OnBnClickedRadioCms)
	ON_CBN_SELCHANGE(IDC_COM__BITOFDEPTH, &CameraControl::OnCbnSelchangeCom)
	ON_BN_CLICKED(IDC_BTN_USBLED, &CameraControl::OnBnClickedBtnUsbled)
END_MESSAGE_MAP()


// CameraControl message handlers

void CameraControl::InitControlRange()
{
	if (NULL != m_opCam.hIdxTUCam)
	{
		char szRes[64] = {0};
		TUCAM_CAPA_ATTR  attrCapa;
		TUCAM_PROP_ATTR  attrProp;
		TUCAM_VALUE_TEXT valText;

		valText.nTextSize = 64;
		valText.pText = &szRes[0];

		m_SlaveAddress.ResetContent();
		m_SlaveAddress.AddString(_T("(0x3b)"));
		m_SlaveAddress.AddString(_T("(0x3c)"));
		m_SlaveAddress.SetCurSel(0);

		// ��ȡ�ֱ��ʷ�Χ
		attrCapa.idCapa = TUIDC_RESOLUTION;
		if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
			CString szRes;
			int nCnt = attrCapa.nValMax - attrCapa.nValMin + 1;
			valText.nID = TUIDC_RESOLUTION;
			m_cmbRes.ResetContent();
			for (int i=0; i<nCnt; ++i)
			{
				valText.dbValue = i;
				TUCAM_Capa_GetValueText(m_opCam.hIdxTUCam, &valText); 
				szRes = valText.pText;
				m_cmbRes.InsertString(i, szRes);
			}

			m_cmbRes.SetCurSel(0);
		}

		// ��ȡλ�� 400BSI
		attrCapa.idCapa = TUIDC_IMGMODESELECT;
		if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
			CString szRes = "12bit";
			int nCnt = attrCapa.nValMax - attrCapa.nValMin + 1;
			m_cmbBitOfDepth.ResetContent();
			for (int i=0; i<nCnt; ++i)
			{
				if(i == 1)
					szRes = "CMS";
				else if(i == 2)
					szRes = "11bit";
				else
					szRes = "12bit";

				m_cmbBitOfDepth.InsertString(i, szRes);
			}
            
			m_cmbBitOfDepth.SetCurSel(0);

			if(nCnt < 2)
			{
				GetDlgItem(IDC_COM__BITOFDEPTH)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_GRP_COM)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_COMBO_SLAADD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDT_REGADD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDT_VALUE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STA_SA)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STA_REG)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STA_VALUE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_W)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_R)->ShowWindow(SW_HIDE);
				m_bCmsEn  = TRUE;
			}
			else
			{
				GetDlgItem(IDC_RADIO_CMS)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHK_BITOFDEPTH)->ShowWindow(SW_HIDE);
				m_bCmsEn  = FALSE;
			}
		}
		else
		{
			GetDlgItem(IDC_COM__BITOFDEPTH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GRP_COM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_SLAADD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDT_REGADD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDT_VALUE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_SA)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_REG)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_VALUE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_W)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_R)->ShowWindow(SW_HIDE);
		}

		// ��ȡλ��
		attrCapa.idCapa = TUIDC_BITOFDEPTH;
		if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
			m_chkBDep.EnableWindow(FALSE);
		}
		else
		{
			if(attrCapa.nValMin != attrCapa.nValMax)
				m_chkBDep.EnableWindow(TRUE);
			else
                m_chkBDep.EnableWindow(FALSE);
		}

		// ��ȡ�Զ��ع�
		attrCapa.idCapa = TUIDC_ATEXPOSURE;
		if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
            GetDlgItem(IDC_BTN_ONCEAE)->ShowWindow(SW_HIDE);

            m_chkAE.ShowWindow(SW_SHOW);
            m_chkAE.EnableWindow(FALSE);
		}
		else
		{
            if (attrCapa.nValMax > 1)
            {
                GetDlgItem(IDC_BTN_ONCEAE)->ShowWindow(SW_SHOW);
                m_chkAE.ShowWindow(SW_HIDE);
            }
            else
            {
                GetDlgItem(IDC_BTN_ONCEAE)->ShowWindow(SW_HIDE);
                m_chkAE.ShowWindow(SW_SHOW);
                m_chkAE.EnableWindow(TRUE);
            }	
		}

		// ��ȡLED��
		attrCapa.idCapa = TUIDC_LEDENBALE;
		if (TUCAMRET_SUCCESS != TUCAM_Capa_GetAttr(m_opCam.hIdxTUCam, &attrCapa))
		{
            GetDlgItem(IDC_BTN_USBLED)->ShowWindow(SW_HIDE);
		}
		

		// ��ȡ�Զ��ع�Ŀ��ֵ
		attrProp.nIdxChn= 0;                    // ʹ��Ĭ��ͨ��
		attrProp.idProp = TUIDP_BRIGHTNESS;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bTargetRange = FALSE;
			m_sldTgr.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);
		}
		else
		{
			m_sldTgr.EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(FALSE);
		}

        // ��ȡ���淶Χ
		attrProp.idProp = TUIDP_GLOBALGAIN;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			m_bGainRange = FALSE;
            m_uiMaxGain  = (UINT)attrProp.dbValMax;

            if (m_uiMaxGain > 6)
            {
                m_sldGain.SetRange((int)attrProp.dbValMin, (int)attrProp.dbValMax);

                GetDlgItem(IDC_STA_GAIN)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_SLD_GAIN)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_EDT_GAIN)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_RADIO_HDR)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_RADIO_HIGHGAIN)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_RADIO_LOWGAIN)->ShowWindow(SW_HIDE);

                GetDlgItem(IDC_STA_GAIN)->EnableWindow(TRUE);
                GetDlgItem(IDC_SLD_GAIN)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_GAIN)->EnableWindow(TRUE);                
            }
            else
            {
                GetDlgItem(IDC_STA_GAIN)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_SLD_GAIN)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_EDT_GAIN)->ShowWindow(SW_HIDE);
                GetDlgItem(IDC_RADIO_HDR)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_RADIO_HIGHGAIN)->ShowWindow(SW_SHOW);
                GetDlgItem(IDC_RADIO_LOWGAIN)->ShowWindow(SW_SHOW);

                GetDlgItem(IDC_RADIO_HDR)->EnableWindow(TRUE);
                GetDlgItem(IDC_RADIO_HIGHGAIN)->EnableWindow(TRUE);
                GetDlgItem(IDC_RADIO_LOWGAIN)->EnableWindow(TRUE);
            }
		} 
		else
		{
            GetDlgItem(IDC_STA_GAIN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_SLD_GAIN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_EDT_GAIN)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_RADIO_HDR)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_RADIO_HIGHGAIN)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_RADIO_LOWGAIN)->ShowWindow(SW_HIDE);

            GetDlgItem(IDC_STA_GAIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLD_GAIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_GAIN)->EnableWindow(FALSE);
		}

		// ��ȡ�ع�ʱ�䷶Χ
		attrProp.idProp = TUIDP_EXPOSURETM;
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp))
		{
			((CButton*)GetDlgItem(IDC_SPN_SEC))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDT_SEC))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_SPN_MS))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDT_MS))->EnableWindow(TRUE);

			m_uiExpStep = (UINT)(attrProp.dbValStep * 1000 + 0.5f);

			if((DWORD)(attrProp.dbValMax * 1000) < 1000000)
			{
				((CButton*)GetDlgItem(IDC_SPN_SEC))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_EDT_SEC))->EnableWindow(FALSE);
			}

			if((DWORD)(attrProp.dbValMax * 1000) < 1000)
			{
				((CButton*)GetDlgItem(IDC_SPN_SEC))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_EDT_SEC))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_SPN_MS))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_EDT_MS))->EnableWindow(FALSE);
			}
		} 
	}
}

void CameraControl::RefreshValue()
{
	if (NULL != m_opCam.hIdxTUCam)
	{
	    // ����
        int     nVal  = 0;
        double  dbVal = 0;
        CString szText= _T("");

        // ��ȡ��ǰ�ֱ���ֵ
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_RESOLUTION, &nVal))
        {
            m_cmbRes.SetCurSel(nVal);
        }

        // ��ȡ��ǰλ��
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_BITOFDEPTH, &nVal))
        {
            if (nVal == 8)
            {
                m_chkBDep.SetCheck(0);
            }
            else
            {
                m_chkBDep.SetCheck(1);
            }
        }

		// ��ȡ��ǰ�ع�ʱ��
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dbVal))
        {
            UpdateExposureTime((DWORD)(dbVal * 1000));
        }


        // ��ȡ��ǰĿ��ֵ
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_BRIGHTNESS, &dbVal))
        {    
            nVal = (int)dbVal;
            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_ATTGR)->SetWindowText(szText);    

            m_sldTgr.SetPos(nVal);
        }

		// ��ȡ��ǰģʽ
		if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
		{    
			m_cmbBitOfDepth.SetCurSel(nVal);
			// ��ȡ��ǰ�ع�ʱ��
		}
		else
		{
			GetDlgItem(IDC_RADIO_CMS)->ShowWindow(SW_HIDE);
		}

        // ��ȡ�Զ��ع�״̬
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_ATEXPOSURE, &nVal))
        {
            // �Զ��ع�
            if (1 == nVal)
            {
                m_chkAE.SetCheck(TRUE);

                m_sldTgr.EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(TRUE);

                GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);

                GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);

                GetDlgItem(IDC_BTN_EXPTM)->EnableWindow(FALSE);
            }
            else
            {
                m_chkAE.SetCheck(FALSE);

                m_sldTgr.EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(FALSE);

                GetDlgItem(IDC_STA_SEC)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_MS)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_US)->EnableWindow(TRUE);

                GetDlgItem(IDC_EDT_SEC)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_MS)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_US)->EnableWindow(TRUE);
                GetDlgItem(IDC_SPN_SEC)->EnableWindow(TRUE);
                GetDlgItem(IDC_SPN_MS)->EnableWindow(TRUE);
                GetDlgItem(IDC_SPN_US)->EnableWindow(TRUE);

                GetDlgItem(IDC_BTN_EXPTM)->EnableWindow(TRUE);
            }
        }

		// ��ȡ��ǰ����ֵ
		if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_GLOBALGAIN, &dbVal))
		{
            m_nGain = (int)dbVal;

            if (m_uiMaxGain > 6)
            {
                m_sldGain.SetPos(m_nGain);

                szText.Format(_T("%d"), m_nGain);
                GetDlgItem(IDC_EDT_GAIN)->SetWindowText(szText);  
            }
            else
            {     
				if(m_bCmsEn)
				{
					if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
					{
						m_nGain = (1 == nVal) ? 3 : m_nGain;
					}
				}

                switch (m_nGain)
                {
                case 1:
                    {
                        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(TRUE);
                        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(FALSE);
                    }
                    break;
                case 2:
                    {
                        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(TRUE);
                        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(FALSE);
                    }
                    break;
                case 3:
                    {
                        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(TRUE);
						//m_cmbBitOfDepth.SetCurSel(1);
                    }
                    break;
                case 0:
                default:
                    {
                        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(TRUE);
                        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(FALSE);
                        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(FALSE);
                    }
                    break;
                }            
            }
		}

         // ��ȡ��ǰLed ��״̬
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_LEDENBALE, &nVal))
        {
			m_bLedState  = (BOOL) nVal;
            LedShow(m_bLedState);
        }

	}
}

void CameraControl::RefreshBtn(BOOL bLiving)
{
	GetDlgItem(IDC_BTN_LIVE)->EnableWindow(bLiving);
}

void CameraControl::BtnShow(BOOL mLiving)
{
	if(mLiving)
    {
		GetDlgItem(IDC_BTN_LIVE)->SetWindowText(_T("Stop"));
        GetDlgItem(IDC_CMB_RES)->EnableWindow(FALSE);
    }
	else
    {
		GetDlgItem(IDC_BTN_LIVE)->SetWindowText(_T("Live"));
        GetDlgItem(IDC_CMB_RES)->EnableWindow(TRUE);
    }
}

void CameraControl::LedShow(BOOL bLed)
{
	if(bLed)
    {
		GetDlgItem(IDC_BTN_USBLED)->SetWindowText(_T("Led On"));
    }
	else
    {
		GetDlgItem(IDC_BTN_USBLED)->SetWindowText(_T("Led Off"));
    }
}

void CameraControl::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDT_GET_FPS_RATE == nIDEvent) 
	{
		CString szFps;
		szFps.Format(_T("%.2f fps"), m_ccmode->m_fFps / 1000.0f);

		GetDlgItem(IDC_STA_FPS)->SetWindowText(szFps); 
	}
    else if (IDT_GET_TEMPERATURE == nIDEvent) 
    {
        double dblVal = 1.0f;

        if (NULL != m_opCam.hIdxTUCam)
        {
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_TEMPERATURE, &dblVal))
            {
                CString sz;
                sz.Format(_T("%.2f ��"), dblVal);
                GetDlgItem(IDC_STA_TEMP)->SetWindowText(sz);
            }
        }
    }
    else if (IDT_GET_ONCE_AE == nIDEvent)
    {
        if (NULL != m_opCam.hIdxTUCam)
        {
            int nVal = 2;
            if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_ATEXPOSURE, &nVal))
            {
                if (0 == nVal)
                {
                    GetDlgItem(IDC_BTN_ONCEAE)->EnableWindow(TRUE);
                    KillTimer(IDT_GET_ONCE_AE);

                    GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(FALSE);

                    GetDlgItem(IDC_STA_SEC)->EnableWindow(TRUE);
                    GetDlgItem(IDC_STA_MS)->EnableWindow(TRUE);
                    GetDlgItem(IDC_STA_US)->EnableWindow(TRUE);

                    GetDlgItem(IDC_EDT_SEC)->EnableWindow(TRUE);
                    GetDlgItem(IDC_EDT_MS)->EnableWindow(TRUE);
                    GetDlgItem(IDC_EDT_US)->EnableWindow(TRUE);
                    GetDlgItem(IDC_SPN_SEC)->EnableWindow(TRUE);
                    GetDlgItem(IDC_SPN_MS)->EnableWindow(TRUE);
                    GetDlgItem(IDC_SPN_US)->EnableWindow(TRUE);

                    GetDlgItem(IDC_BTN_EXPTM)->EnableWindow(TRUE);

                    double dbVal = 0.0f;
                    // ��ȡ��ǰ�ع�ʱ��
                    if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dbVal))
                    {
                        UpdateExposureTime((DWORD)(dbVal * 1000));
                    }
                }
            }
        }
    }
   
    CDialog::OnTimer(nIDEvent);
}

LRESULT CameraControl::OnTUMessageFps(WPARAM wParam, LPARAM lParam)
{   
    int nFps = (int)lParam;
	float m_dfps = nFps / 1000.0f;
    CString szFps;
    szFps.Format(_T("%.2f fps"), m_dfps);
    GetDlgItem(IDC_STA_FPS)->SetWindowText(szFps);

    return 0;
}

LRESULT CameraControl::OnTUMessageMean(WPARAM wParam, LPARAM lParam)
{   
   /* int nMean = (int)lParam;
    CString szMean;
    szMean.Format(_T("Mean = %d"), nMean);
    GetDlgItem(IDC_STA_MEAN)->SetWindowText(szMean);*/

    return 0;
}

LRESULT CameraControl::OnTUMessageStd(WPARAM wParam, LPARAM lParam)
{   
   /* double dStd = (double)lParam;
    CString szStd;
    szStd.Format(_T("Std = %.2f"), dStd/1000.0f);
    GetDlgItem(IDC_STA_STD)->SetWindowText(szStd);*/

    return 0;
}

BOOL CameraControl::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
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

void CameraControl::OnBnClickedBtnLive()
{
	// TODO: Add your control notification handler code here
    if (m_ccmode->m_bWaitting)
    {
        if(NULL != m_ccmode->m_tabPage[4])
        {
            m_ccmode->m_nTriMode = TUCCM_SEQUENCE;
            ((Trigger*)m_ccmode->m_tabPage[4])->EnableTriggerControl(FALSE);

            ((CButton *)m_ccmode->m_tabPage[4]->GetDlgItem(IDC_RADIO_OFF))->SetCheck(TRUE);
            ((CButton *)m_ccmode->m_tabPage[4]->GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(FALSE);
            ((CButton *)m_ccmode->m_tabPage[4]->GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(FALSE);
            ((CButton *)m_ccmode->m_tabPage[4]->GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(FALSE);
        }
    }
    else
    {
        m_ccmode->m_nTriMode = TUCCM_SEQUENCE;
    }
    
	m_ccmode->OnBnClickedBtnLive();
    
	if (!m_ccmode->m_bTgrMode)
	{
		if (m_ccmode->m_bLiving)
		{
			m_bFirst    = TRUE;
			m_favg      = 0.0;
			m_ffast     = 0.0;
			m_fslow     = 0.0;
			m_ucount    = 0;
		}
	}
	else
	{
		if (m_ccmode->m_bTrigger)
		{
			SetTimer(IDT_GET_FPS_RATE, IDT_SPC_FPS_RATE, NULL); // ��ȡ֡��
		}
		else
		{
			KillTimer(IDT_GET_FPS_RATE);                       // �رն�ȡ֡��
		}
	}
}


void CameraControl::OnCbnSelchangeCmbRes()
{
	// TODO: Add your control notification handler code here
	if (NULL != m_opCam.hIdxTUCam)
	{
        BOOL bWaitting = m_ccmode->m_bWaitting;

        if (bWaitting)
        {
            m_ccmode->StopWaitForFrame();
        }

		TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_RESOLUTION, m_cmbRes.GetCurSel());
		m_ccmode->OnRefreshRoiState();
		/*((CSampleProDlg *)this->GetParent())->m_PicViewMode.GetDlgItem(IDC_STA_PIC)->Invalidate();


		// �ر�ROI״̬
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

		GetDlgItem(IDC_BTN_ROI)->SetWindowText(_T("Set ROI"));*/

        m_ccmode->UpdateWindow();
       
        if (bWaitting)
        {
            m_ccmode->StartWaitForFrame();
        }
	}
}


void CameraControl::OnBnClickedChkAe()
{
	// TODO: Add your control notification handler code here
	double dbVal = 0;     

    if (m_chkAE.GetCheck())
    {
        m_sldTgr.EnableWindow(TRUE);        
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATEXPOSURE, 1);

        GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(TRUE);

        GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
        GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
        GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);

        GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
        GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
        GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
        GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);

        GetDlgItem(IDC_BTN_EXPTM)->EnableWindow(FALSE);

        // ��ȡ��ǰĿ��ֵ
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_BRIGHTNESS, &dbVal))
        {    
            int nVal = (int)dbVal;
            CString szText;

            szText.Format(_T("%d"), nVal);
            GetDlgItem(IDC_EDT_ATTGR)->SetWindowText(szText);    

            m_sldTgr.SetPos(nVal);
        }
    }
    else
    {
        m_sldTgr.EnableWindow(FALSE);
        TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATEXPOSURE, 0);

        GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(FALSE);

        GetDlgItem(IDC_STA_SEC)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_MS)->EnableWindow(TRUE);
        GetDlgItem(IDC_STA_US)->EnableWindow(TRUE);

        GetDlgItem(IDC_EDT_SEC)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_MS)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_US)->EnableWindow(TRUE);
        GetDlgItem(IDC_SPN_SEC)->EnableWindow(TRUE);
        GetDlgItem(IDC_SPN_MS)->EnableWindow(TRUE);
        GetDlgItem(IDC_SPN_US)->EnableWindow(TRUE);

        GetDlgItem(IDC_BTN_EXPTM)->EnableWindow(TRUE);

        // ��ȡ��ǰ�ع�ʱ��
        if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dbVal))
        {
            UpdateExposureTime((DWORD)(dbVal * 1000));
        }
    }
}


void CameraControl::OnNMCustomdrawSldTgr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	 // ���Զ��ع�Ŀ��ֵ����ֵֹ���ı�
    if (!m_bTargetRange)
    {
        m_bTargetRange = TRUE;
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        int nPos = m_sldTgr.GetPos();
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_BRIGHTNESS, nPos);          

        CString szText = _T("");
        szText.Format(_T("%d"), nPos);
        GetDlgItem(IDC_EDT_ATTGR)->SetWindowText(szText);  
    }
	*pResult = 0;
}



//�����ع�ʱ�䣨���ӣ�
void CameraControl::OnDeltaposSpnSec(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_uSec = GetDlgItemInt(IDC_EDT_SEC);

	if (pNMUpDown->iDelta < 0)
	{
		if (m_uSec >= 59)
		{
			return;
		}
		m_uSec++;

		CString strSec;
		strSec.Format(_T("%d"), m_uSec);
		GetDlgItem(IDC_EDT_SEC)->SetWindowText(strSec);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		if (m_uSec <= 0)
		{
			return;
		}
		m_uSec--;

		CString strSec;
		strSec.Format(_T("%d"), m_uSec);
		GetDlgItem(IDC_EDT_SEC)->SetWindowText(strSec);
	}

	*pResult = 0;
}

//�����ع��¼������룩
void CameraControl::OnDeltaposSpnMs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_uMs = GetDlgItemInt(IDC_EDT_MS);

//  CString sz;
//  sz.Format(_T("%d\n"), pNMUpDown->iPos);
//  OutputDebugString(sz);

	if (pNMUpDown->iDelta < 0)
	{
		if (m_uMs >= 999)
		{
			return;
		}
		m_uMs++;

		CString strMs;
		strMs.Format(_T("%d"), m_uMs);
		GetDlgItem(IDC_EDT_MS)->SetWindowText(strMs);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		if (m_uMs <= 0)
		{
			return;
		}
		m_uMs--;

		CString strMs;
		strMs.Format(_T("%d"), m_uMs);
		GetDlgItem(IDC_EDT_MS)->SetWindowText(strMs);
	}

    OnBnClickedBtnExptm();

	*pResult = 0;
}

//�����ع�ʱ�䣨΢�룩
void CameraControl::OnDeltaposSpnUs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_uUs = GetDlgItemInt(IDC_EDT_US);

    if (pNMUpDown->iDelta < 0)
	{
		if (m_uUs >= 999)
		{
			return;
		}
		m_uUs += (m_uiExpStep + 1);

		CString strUs;
		strUs.Format(_T("%d"), m_uUs);
		GetDlgItem(IDC_EDT_US)->SetWindowText(strUs);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		if (m_uUs == 0)
		{
			return;
		}

        if (m_uUs < m_uiExpStep)
        {
            m_uUs = m_uiExpStep;
        }
        else
        {
            m_uUs -= (m_uiExpStep - 1);
        }       

		CString strUs;
		strUs.Format(_T("%d"), m_uUs);
		GetDlgItem(IDC_EDT_US)->SetWindowText(strUs);
	}

    OnBnClickedBtnExptm();

	*pResult = 0;
}


void CameraControl::OnBnClickedBtnExptm()
{
	// TODO: Add your control notification handler code here
	UpdateData();

    DWORD dwExp = m_uSec *1000 * 1000 + m_uMs * 1000 + m_uUs;  
    double dblExp = dwExp / 1000.0f;

    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, dblExp);

        TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dblExp);

        UpdateExposureTime((DWORD)(dblExp * 1000));
    }   
}

void CameraControl::UpdateExposureTime(DWORD dwExp)
{
    m_uSec = (dwExp / (1000 * 1000)) % 60;
    m_uMs  = (dwExp / 1000) % 1000;
    m_uUs  = dwExp % 1000;

    CString sz;
    sz.Format(_T("%d"), m_uSec);
    GetDlgItem(IDC_EDT_SEC)->SetWindowText(sz);
    sz.Format(_T("%d"), m_uMs);
    GetDlgItem(IDC_EDT_MS)->SetWindowText(sz);
    sz.Format(_T("%d"), m_uUs);
    GetDlgItem(IDC_EDT_US)->SetWindowText(sz);
}

void CameraControl::OnBnClickedChkBitofdepth()
{
	// TODO: Add your control notification handler code here
    BOOL bWaitting = m_ccmode->m_bWaitting;

    if (bWaitting)
    {
        m_ccmode->StopWaitForFrame();
    }

    if (m_chkBDep.GetCheck())
    {
        // 16bit
        if (NULL != m_opCam.hIdxTUCam)
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_BITOFDEPTH, 16);         
        }
    }
    else
    {
        // 8bit
        if (NULL != m_opCam.hIdxTUCam)
        {
            TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_BITOFDEPTH, 8);         
        }
    }

	m_ccmode->OnRefreshLevelRange();

    if (bWaitting)
    {
        m_ccmode->StartWaitForFrame();
    }
}

void CameraControl::OnBnClickedRadioHdr()
{
	// TODO: Add your control notification handler code here
	m_nGain = 0;

    if (NULL != m_opCam.hIdxTUCam)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(FALSE);

        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GLOBALGAIN, m_nGain);

         /*int nVal = 0;
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
        {
            if (0 != nVal)
            {
                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, 0);
            }
        }*/
    }
}


void CameraControl::OnBnClickedRadioHighgain()
{
	// TODO: Add your control notification handler code here
	m_nGain = 1;

    if (NULL != m_opCam.hIdxTUCam)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(FALSE);

        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GLOBALGAIN, m_nGain);

        /*int nVal = 0;
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
        {
            if (0 != nVal)
            {
                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, 0);
            }
        }*/
    }
}


void CameraControl::OnBnClickedRadioLowgain()
{
	// TODO: Add your control notification handler code here
	m_nGain = 2;

    if (NULL != m_opCam.hIdxTUCam)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(FALSE);

        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GLOBALGAIN, m_nGain);

        /*int nVal = 0;
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
        {
            if (0 != nVal)
            {
                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, 0);
            }
        }*/
    }

}

void CameraControl::OnBnClickedRadioCms()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nGain = 3;

    if (NULL != m_opCam.hIdxTUCam)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_HDR))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_HIGHGAIN))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_LOWGAIN))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_CMS))->SetCheck(TRUE);
        
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GLOBALGAIN, 0);

         /*int nVal = 0;
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
        {
            if (0 != nVal)
            {
                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, 0);
            }
        }*/
    }
}


void CameraControl::OnNMCustomdrawSldGain(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    // �����棬��ֵֹ���ı�
    if (!m_bGainRange)
    {
        m_bGainRange = TRUE;
        return;
    }

    if (NULL != m_opCam.hIdxTUCam)
    {
        int nPos = m_sldGain.GetPos();
        TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_GLOBALGAIN, nPos);          

        CString szText = _T("");
        szText.Format(_T("%d"), nPos);
        GetDlgItem(IDC_EDT_GAIN)->SetWindowText(szText);  
    }


    *pResult = 0;
}

int CameraControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������

    SetTimer(IDT_GET_TEMPERATURE, IDT_SPC_TEMPERATURE, NULL);

    return 0;
}

void CameraControl::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������

    KillTimer(IDT_GET_TEMPERATURE);
}

void CameraControl::OnBnClickedBtnOnceae()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    GetDlgItem(IDC_BTN_ONCEAE)->EnableWindow(FALSE);

    if (NULL != m_opCam.hIdxTUCam)
    {
        if (TUCAMRET_SUCCESS == TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_ATEXPOSURE, 2))
        {
            SetTimer(IDT_GET_ONCE_AE, IDT_SPC_ONCE_AE, NULL);

            GetDlgItem(IDC_EDT_ATTGR)->EnableWindow(TRUE);

            GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
            GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);

            GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
            GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
            GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
            GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);

            GetDlgItem(IDC_BTN_EXPTM)->EnableWindow(FALSE);

            double dbVal = 0.0f;

            // ��ȡ��ǰĿ��ֵ
            if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_BRIGHTNESS, &dbVal))
            {    
                int nVal = (int)dbVal;
                CString szText;

                szText.Format(_T("%d"), nVal);
                GetDlgItem(IDC_EDT_ATTGR)->SetWindowText(szText);    

                m_sldTgr.SetPos(nVal);
            }
        }
        else
        {
            GetDlgItem(IDC_BTN_ONCEAE)->EnableWindow(TRUE);
        }
    }
}

int CStringHexToInt(CString str)
{
	int nRet = 0;
	int count = 1;
	for(int i = str.GetLength()-1; i >= 0; --i)
	{
		int nNum = 0;
		char chTest;
		chTest = str.GetAt(i);       
		if (chTest >= '0' && chTest <= '9')
		{
			nNum = chTest - '0';
		}
		else if (chTest >= 'A' && chTest <= 'F')
		{
			nNum = chTest - 'A' + 10;
		}
		else if (chTest >= 'a' && chTest <= 'f')
		{
			nNum = chTest - 'a' + 10;
		}
		nRet += nNum*count;
		count *= 16;

	}
	return nRet;
}

void CameraControl::OnBnClickedBtnW()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*UpdateData();
	if (NULL != m_opCam.hIdxTUCam)
	{
		UINT8 nSlaveAdd = 0x3b;
		UINT8 nRegAdd   = (UINT8)CStringHexToInt(m_sRegAddress);
		UINT8 nValue    = (UINT8)CStringHexToInt(m_sValue);
		if(m_SlaveAddress.GetCurSel())
			nSlaveAdd = 0x3c;
		else
            nSlaveAdd = 0x3b;

	    TUCAM_Phx_Command(m_opCam.hIdxTUCam, 1,nSlaveAdd,nRegAdd,&nValue);
	}*/
}

void CameraControl::OnBnClickedBtnR()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*UpdateData();
	if (NULL != m_opCam.hIdxTUCam)
	{
		UINT8 nSlaveAdd = 0x3b;
		UINT8 nRegAdd   = (UINT8)CStringHexToInt(m_sRegAddress);
		UINT8 nValue    = (UINT8)CStringHexToInt(m_sValue);
		if(m_SlaveAddress.GetCurSel())
			nSlaveAdd = 0x3c;
		else
			nSlaveAdd = 0x3b;

		if (TUCAMRET_SUCCESS == TUCAM_Phx_Command(m_opCam.hIdxTUCam, 0,nSlaveAdd,nRegAdd,&nValue))
		{
			CString szText;

			szText.Format(_T("%02x"), nValue);
			GetDlgItem(IDC_EDT_VALUE)->SetWindowText(szText);    

		}
	}*/
}

void CameraControl::OnCbnSelchangeCom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	int nVal = 0;
    DWORD dwExp = m_uSec *1000 * 1000 + m_uMs * 1000 + m_uUs;  
    double dblExp = dwExp / 1000.0f;

	if (NULL != m_opCam.hIdxTUCam)
	{
		///int nVal = 0;
		///double dblExp = 0;
		TUCAM_PROP_ATTR  attrProp;
        if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, &nVal))
        {
            if (m_cmbBitOfDepth.GetCurSel() != nVal)
            {
                TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_IMGMODESELECT, m_cmbBitOfDepth.GetCurSel());

				TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, dblExp);

				TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dblExp);


				UpdateExposureTime((DWORD)(dblExp * 1000));

				attrProp.nIdxChn= 0;                    // ʹ��Ĭ��ͨ��
				attrProp.idProp = TUIDP_EXPOSURETM;
				TUCAM_Prop_GetAttr(m_opCam.hIdxTUCam, &attrProp);
				m_uiExpStep = (UINT)(attrProp.dbValStep * 1000 + 0.5f);
            }
        }
	}
}

void CameraControl::OnBnClickedBtnUsbled()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL != m_opCam.hIdxTUCam)
	{
		m_bLedState  = !m_bLedState;
        
		TUCAM_Capa_SetValue(m_opCam.hIdxTUCam, TUIDC_LEDENBALE, m_bLedState);

		LedShow(m_bLedState);
	}
}
