// Trigger.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "Trigger.h"
#include "CameraControl.h"
#include "ImageCapture.h"
//#include "afxdialogex.h"


// Trigger dialog

IMPLEMENT_DYNAMIC(Trigger, CDialog)

Trigger::Trigger(TUCAM_OPEN &opCam,CCameraControlMode* pParent)
	: CDialog(Trigger::IDD, pParent)
	, m_nTriMode(0)
    , m_nTriExp(0)
    , m_nTriEdge(0)
    , m_uSec(0)
    , m_uMs(0)
    , m_uUs(0)
{
	m_opCam                 = opCam;
	m_ccmode                = pParent;

    m_lDelay                = 0;
}

//Trigger::~Trigger()
//{
//}

void Trigger::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHK_STTRIGGER, m_chkSFWTri);
    DDX_Text(pDX, IDC_EDT_SEC, m_uSec);
    DDX_Text(pDX, IDC_EDT_MS, m_uMs);
    DDX_Text(pDX, IDC_EDT_US, m_uUs);
}


BEGIN_MESSAGE_MAP(Trigger, CDialog)
	ON_BN_CLICKED(IDC_RADIO_OFF, &Trigger::OnBnClickedRadioOff)
	ON_BN_CLICKED(IDC_RADIO_STANDARD, &Trigger::OnBnClickedRadioStandard)
	ON_BN_CLICKED(IDC_RADIO_SYNCHRO, &Trigger::OnBnClickedRadioSynchro)
	ON_BN_CLICKED(IDC_RADIO_GLOABAL, &Trigger::OnBnClickedRadioGloabal)
	ON_BN_CLICKED(IDC_RADIO_TIMED, &Trigger::OnBnClickedRadioTimed)
	ON_BN_CLICKED(IDC_RADIO_WIDTH, &Trigger::OnBnClickedRadioWidth)
	ON_BN_CLICKED(IDC_RADIO_RISING, &Trigger::OnBnClickedRadioRising)
	ON_BN_CLICKED(IDC_RADIO_FALLING, &Trigger::OnBnClickedRadioFalling)
	ON_BN_CLICKED(IDC_CHK_STTRIGGER, &Trigger::OnBnClickedChkSttrigger)
	ON_BN_CLICKED(IDC_BTN_SNAP, &Trigger::OnBnClickedBtnSnap)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &Trigger::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_APPLY, &Trigger::OnBnClickedBtnApply)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_SEC, &Trigger::OnDeltaposSpnSec)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_MS, &Trigger::OnDeltaposSpnMs)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_US, &Trigger::OnDeltaposSpnUs)
    ON_STN_CLICKED(IDC_STA_DELAY, &Trigger::OnStnClickedStaDelay)
END_MESSAGE_MAP()


// Trigger message handlers

void Trigger::InitControlRange()                        // 初始化范围
{
    TUCAM_TRIGGER_ATTR tgrAttr;
    tgrAttr.nTgrMode = TUCCM_SEQUENCE;
    tgrAttr.nFrames  = 1;
    tgrAttr.nDelayTm = 0;
    tgrAttr.nExpMode = TUCTE_EXPTM;
    tgrAttr.nEdgeMode= TUCTD_RISING;
    if (TUCAMRET_SUCCESS == TUCAM_Cap_GetTrigger(m_opCam.hIdxTUCam, &tgrAttr))
    {
        EnableTriggerControl(TRUE);
    }
    else
    {
        EnableTriggerControl(FALSE);
    }
}

void Trigger::RefreshValue()                            // 初始化值
{
    // 获取触发属性
    TUCAM_TRIGGER_ATTR tgrAttr;
    tgrAttr.nTgrMode = TUCCM_SEQUENCE;
    tgrAttr.nFrames  = 1;
    tgrAttr.nDelayTm = 0;
    tgrAttr.nExpMode = TUCTE_EXPTM;
    tgrAttr.nEdgeMode= TUCTD_RISING;
    if (TUCAMRET_SUCCESS == TUCAM_Cap_GetTrigger(m_opCam.hIdxTUCam, &tgrAttr))
    {
        m_nTriMode = tgrAttr.nTgrMode;
        m_nTriExp  = tgrAttr.nExpMode;
        m_nTriEdge = tgrAttr.nEdgeMode;
        m_lDelay   = tgrAttr.nDelayTm;
        UpdateDelayTime(m_lDelay);

        m_ccmode->m_nTriMode   = m_nTriMode;

        UpdateData(FALSE);
    }
}

BOOL Trigger::PreTranslateMessage(MSG* pMsg)
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

void Trigger::EnableTriggerControl(BOOL bEnable /*= FALSE*/)
{
    BOOL bSfwTrigger = FALSE;

    if (bEnable)
    {
        // 获取触发属性
        TUCAM_TRIGGER_ATTR tgrAttr;
        tgrAttr.nTgrMode = TUCCM_SEQUENCE;
        tgrAttr.nFrames  = 1;
        tgrAttr.nDelayTm = 0;
        tgrAttr.nExpMode = TUCTE_EXPTM;
        tgrAttr.nEdgeMode= TUCTD_RISING;

        if (TUCAMRET_SUCCESS == TUCAM_Cap_GetTrigger(m_opCam.hIdxTUCam, &tgrAttr))
        {
            switch (m_nTriMode)
            {
            case TUCCM_TRIGGER_STANDARD:
                {
                    ((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(TRUE);
                    ((CButton *)GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(FALSE);
                    m_chkSFWTri.SetCheck(FALSE);
                }
                break;
            case TUCCM_TRIGGER_SYNCHRONOUS:
                {
                    ((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(TRUE);
                    ((CButton *)GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(FALSE);
                    m_chkSFWTri.SetCheck(FALSE);
                }
                break;
            case TUCCM_TRIGGER_GLOBAL:
                {
                    ((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(TRUE);
                    m_chkSFWTri.SetCheck(FALSE);
                }
                break;
            case TUCCM_TRIGGER_SOFTWARE:
                {
                    ((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(FALSE);
                    m_chkSFWTri.SetCheck(TRUE);
                    bSfwTrigger = TRUE;
                }
                break;
            case TUCCM_SEQUENCE:
            default:
                {
                    ((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(TRUE);
                    ((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(FALSE);
                    ((CButton *)GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(FALSE);
                    m_chkSFWTri.SetCheck(FALSE);
                }
                break;
            }

            if (!m_nTriExp)
            {
                ((CButton *)GetDlgItem(IDC_RADIO_TIMED))->SetCheck(TRUE);
                ((CButton *)GetDlgItem(IDC_RADIO_WIDTH))->SetCheck(FALSE);
            }
            else
            {
                ((CButton *)GetDlgItem(IDC_RADIO_TIMED))->SetCheck(FALSE);
                ((CButton *)GetDlgItem(IDC_RADIO_WIDTH))->SetCheck(TRUE);
            }

            // 触发曝光时间模式
            if (TUCCM_TRIGGER_STANDARD == m_nTriMode && !m_nTriExp)
            {

                GetDlgItem(IDC_STA_DELAY)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_SEC)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_MS)->EnableWindow(TRUE);
                GetDlgItem(IDC_STA_US)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_SEC)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_MS)->EnableWindow(TRUE);
                GetDlgItem(IDC_EDT_US)->EnableWindow(TRUE);
                GetDlgItem(IDC_SPN_SEC)->EnableWindow(TRUE);
                GetDlgItem(IDC_SPN_MS)->EnableWindow(TRUE);
                GetDlgItem(IDC_SPN_US)->EnableWindow(TRUE);
                GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TRUE);
            }
            else
            {
                GetDlgItem(IDC_STA_DELAY)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
                GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);
                GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
            }

            // 触发边沿模式
            if (m_nTriEdge)
            {
                ((CButton *)GetDlgItem(IDC_RADIO_RISING))->SetCheck(TRUE);
                ((CButton *)GetDlgItem(IDC_RADIO_FALLING))->SetCheck(FALSE);
            }
            else
            {
                ((CButton *)GetDlgItem(IDC_RADIO_RISING))->SetCheck(FALSE);
                ((CButton *)GetDlgItem(IDC_RADIO_FALLING))->SetCheck(TRUE);
            }
        } 
    }

    GetDlgItem(IDC_RADIO_OFF)->EnableWindow(bEnable && !bSfwTrigger);
    GetDlgItem(IDC_RADIO_STANDARD)->EnableWindow(bEnable && !bSfwTrigger);
    GetDlgItem(IDC_RADIO_SYNCHRO)->EnableWindow(bEnable && !bSfwTrigger);
    GetDlgItem(IDC_RADIO_GLOABAL)->EnableWindow(bEnable && !bSfwTrigger);
    GetDlgItem(IDC_RADIO_TIMED)->EnableWindow(bEnable && !bSfwTrigger && TUCCM_TRIGGER_SYNCHRONOUS != m_nTriMode);
    GetDlgItem(IDC_RADIO_WIDTH)->EnableWindow(bEnable && !bSfwTrigger);
    GetDlgItem(IDC_RADIO_RISING)->EnableWindow(bEnable && !bSfwTrigger);
    GetDlgItem(IDC_RADIO_FALLING)->EnableWindow(bEnable && !bSfwTrigger);

//     GetDlgItem(IDC_STA_DELAY)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_STA_SEC)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_STA_MS)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_STA_US)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_EDT_SEC)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_EDT_MS)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_EDT_US)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_SPN_SEC)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_SPN_MS)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_SPN_US)->EnableWindow(bEnable && !bSfwTrigger);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(bEnable && !bSfwTrigger);

    m_chkSFWTri.EnableWindow(bEnable);
    GetDlgItem(IDC_BTN_APPLY)->EnableWindow(bEnable);
    GetDlgItem(IDC_BTN_SNAP)->EnableWindow(bEnable);
}

void Trigger::OnBnClickedRadioOff()
{
	// TODO: Add your control notification handler code here
    m_chkSFWTri.SetCheck(FALSE);
	m_nTriMode = TUCCM_SEQUENCE;
	m_ccmode->m_nTriMode = m_nTriMode;

    GetDlgItem(IDC_RADIO_TIMED)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_WIDTH)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_RISING)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_FALLING)->EnableWindow(FALSE);

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);

//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(FALSE);
//    GetDlgItem(IDC_EDT_FRAME)->EnableWindow(FALSE);
//    m_sldDelay.EnableWindow(FALSE);
}


void Trigger::OnBnClickedRadioStandard()
{
	// TODO: Add your control notification handler code here
	m_nTriMode = TUCCM_TRIGGER_STANDARD;
	m_ccmode->m_nTriMode = m_nTriMode;
    GetDlgItem(IDC_RADIO_TIMED)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_WIDTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_RISING)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_FALLING)->EnableWindow(TRUE);

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_STA_MS)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_STA_US)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_EDT_US)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_SPN_US)->EnableWindow(!m_nTriExp);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(!m_nTriExp);

//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(!m_nTriExp);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(!m_nTriExp);
//    m_sldDelay.EnableWindow(!m_nTriExp);
}


void Trigger::OnBnClickedRadioSynchro()
{
	// TODO: Add your control notification handler code here
	m_nTriMode = TUCCM_TRIGGER_SYNCHRONOUS;
    m_nTriExp  = TUCTE_WIDTH;
	m_ccmode->m_nTriMode = m_nTriMode;
    ((CButton *)GetDlgItem(IDC_RADIO_WIDTH))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_RADIO_TIMED))->SetCheck(FALSE);

    GetDlgItem(IDC_RADIO_TIMED)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_WIDTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_RISING)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_FALLING)->EnableWindow(TRUE);

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);

//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(FALSE);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
//    m_sldDelay.EnableWindow(FALSE);
}


void Trigger::OnBnClickedRadioGloabal()
{
	// TODO: Add your control notification handler code here
	m_nTriMode = TUCCM_TRIGGER_GLOBAL;
	m_ccmode->m_nTriMode = m_nTriMode;
    GetDlgItem(IDC_RADIO_TIMED)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_WIDTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_RISING)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_FALLING)->EnableWindow(TRUE);

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);

//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(FALSE);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
//    m_sldDelay.EnableWindow(FALSE);
}


void Trigger::OnBnClickedRadioTimed()
{
	// TODO: Add your control notification handler code here
	m_nTriExp = TUCTE_EXPTM;

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_STA_MS)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_STA_US)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_EDT_US)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_SPN_US)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);

//    m_sldDelay.EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TUCCM_TRIGGER_GLOBAL != m_nTriMode);
}


void Trigger::OnBnClickedRadioWidth()
{
	// TODO: Add your control notification handler code here
	m_nTriExp = TUCTE_WIDTH;

//    m_uiTriDelay = 0;
//	m_ccmode->m_uiTriFrame = m_uiTriFrame;
//    m_sldDelay.SetPos(0);

//    m_sldDelay.EnableWindow(FALSE);
//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(FALSE);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_STA_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDT_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPN_US)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);

    UpdateData(FALSE);
}


void Trigger::OnBnClickedRadioRising()
{
	// TODO: Add your control notification handler code here
	m_nTriEdge = TUCTD_RISING;
}


void Trigger::OnBnClickedRadioFalling()
{
	// TODO: Add your control notification handler code here
	m_nTriEdge = TUCTD_FAILING;

}

void Trigger::OnBnClickedChkSttrigger()
{
	// TODO: Add your control notification handler code here
	BOOL bSfw = m_chkSFWTri.GetCheck();

    if (bSfw)
    {
        m_nTriMode = TUCCM_TRIGGER_SOFTWARE;
		m_ccmode->m_nTriMode = m_nTriMode;
        
    }
    else
    {
        m_nTriMode = TUCCM_SEQUENCE;
		m_ccmode->m_nTriMode = m_nTriMode;
        ((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_SYNCHRO))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_RADIO_GLOABAL))->SetCheck(FALSE);
    }

    GetDlgItem(IDC_RADIO_OFF)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_STANDARD)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_SYNCHRO)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_GLOABAL)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_TIMED)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_WIDTH)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_RISING)->EnableWindow(!bSfw);
    GetDlgItem(IDC_RADIO_FALLING)->EnableWindow(!bSfw);

    GetDlgItem(IDC_STA_DELAY)->EnableWindow(!bSfw);
    GetDlgItem(IDC_STA_SEC)->EnableWindow(!bSfw);
    GetDlgItem(IDC_STA_MS)->EnableWindow(!bSfw);
    GetDlgItem(IDC_STA_US)->EnableWindow(!bSfw);
    GetDlgItem(IDC_EDT_SEC)->EnableWindow(!bSfw);
    GetDlgItem(IDC_EDT_MS)->EnableWindow(!bSfw);
    GetDlgItem(IDC_EDT_US)->EnableWindow(!bSfw);
    GetDlgItem(IDC_SPN_SEC)->EnableWindow(!bSfw);
    GetDlgItem(IDC_SPN_MS)->EnableWindow(!bSfw);
    GetDlgItem(IDC_SPN_US)->EnableWindow(!bSfw);
    GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(!bSfw);

//    m_uiTriFrame = 1;
//    m_uiTriDelay = 0;
//	m_ccmode->m_uiTriFrame = m_uiTriFrame;
//    m_sldDelay.SetPos(1);

//     GetDlgItem(IDC_EDT_DELAY)->EnableWindow(!bSfw);
//     GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(!bSfw);

//    m_sldDelay.EnableWindow(!bSfw);
}


void Trigger::OnBnClickedBtnSnap()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_EDT_PICNAME)->GetWindowText(m_szPicName);

    UpdateData(FALSE);

    if (NULL != m_ccmode->m_tabPage[2])          
        ((ImageCapture*)m_ccmode->m_tabPage[2])->GetDlgItem(IDC_EDT_PICNAME)->GetWindowText(m_ccmode->m_szPicName);


    if (NULL != m_opCam.hIdxTUCam)
    {
        TUCAM_Cap_DoSoftwareTrigger(m_opCam.hIdxTUCam);
    }
}


void Trigger::OnBnClickedBtnUpdate()
{
	// TODO: Add your control notification handler code here
	UpdateData();

    m_lDelay = m_uSec *1000 * 1000 + m_uMs * 1000 + m_uUs; 

    if (m_lDelay > 10000000)
    {
        m_lDelay = 10000000;

        m_uSec = 10;
        m_uMs  = 0;
        m_uUs  = 0;

        UpdateData(FALSE);
    }
}


void Trigger::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here
	UpdateData();

    if (NULL != m_ccmode->m_tabPage[2])          
        ((ImageCapture*)m_ccmode->m_tabPage[2])->GetDlgItem(IDC_EDT_PICNAME)->GetWindowText(m_ccmode->m_szPicName);

    GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
    EnableTriggerControl(FALSE);

    if (m_ccmode->m_bWaitting)
    {
        m_ccmode->StopWaitForFrame();
    }
    else
    {
        if(NULL != m_ccmode->m_tabPage[1])
            ((CameraControl*)m_ccmode->m_tabPage[1])->BtnShow(FALSE);
    }

    if (TUCCM_TRIGGER_SYNCHRONOUS == m_ccmode->m_nTriMode || TUCCM_TRIGGER_GLOBAL == m_ccmode->m_nTriMode || TUCCM_SEQUENCE == m_ccmode->m_nTriMode)
    {
//        m_uiTriFrame = 1;
//       m_uiTriDelay = 0;
//		m_ccmode->m_uiTriFrame = m_uiTriFrame;
//        m_sldDelay.SetPos(0);

        UpdateData(FALSE);
    }

    TUCAM_TRIGGER_ATTR tgrAttr;

    tgrAttr.nTgrMode = m_nTriMode;
    tgrAttr.nExpMode = m_nTriExp;
    tgrAttr.nEdgeMode= m_nTriEdge;
    tgrAttr.nFrames  = 1;
    tgrAttr.nDelayTm = m_lDelay;

	if (m_ccmode->m_nTriMode > TUCCM_SEQUENCE &&  m_ccmode->m_nTriMode < TUCCM_TRIGGER_SOFTWARE)
	{
		tgrAttr.nFrames  = -1;
	}
    m_ccmode->m_bTrigger = (TUCCM_SEQUENCE != m_nTriMode) ? TRUE : FALSE;
    m_ccmode->m_nTriMode = m_nTriMode;

    // 设置触发属性
    TUCAM_Cap_SetTrigger(m_opCam.hIdxTUCam, tgrAttr);

    EnableTriggerControl(TRUE);

    if (TUCCM_SEQUENCE != m_nTriMode)
    {
        m_ccmode->m_nCntSave    = 0;
        m_ccmode->m_uiCapTimes  = (m_ccmode->m_uiCapFrames == 1) ? m_ccmode->m_uiCapTimes : ++m_ccmode->m_uiCapTimes;
        m_ccmode->m_uiCapFrames = 1;
        m_ccmode->StartWaitForFrame();
    }

    GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
}


void Trigger::OnDeltaposSpnSec(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
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

void Trigger::OnDeltaposSpnMs(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
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
    *pResult = 0;
}

void Trigger::OnDeltaposSpnUs(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    m_uUs = GetDlgItemInt(IDC_EDT_US);

    if (pNMUpDown->iDelta < 0)
    {
        if (m_uUs >= 999)
        {
            return;
        }
        m_uUs += 1;

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

        if (m_uUs < 1)
        {
            m_uUs = 1;
        }
        else
        {
            m_uUs -= 1;
        }       

        CString strUs;
        strUs.Format(_T("%d"), m_uUs);
        GetDlgItem(IDC_EDT_US)->SetWindowText(strUs);
    }

    *pResult = 0;
}

void Trigger::OnStnClickedStaDelay()
{
    // TODO: 在此添加控件通知处理程序代码
}

void Trigger::UpdateDelayTime(DWORD dwExp)
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