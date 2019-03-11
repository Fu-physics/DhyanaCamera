// ROI.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "Information.h"
//#include "afxdialogex.h"

// Information dialog

IMPLEMENT_DYNAMIC(Information, CDialog)

Information::Information(TUCAM_OPEN &opCam, CCameraControlMode* pParent)
	: CDialog(Information::IDD, pParent)
{
	m_opCam                 = opCam;
	m_ccmode                = pParent;
}

//Information::~Information()
//{
//}

void Information::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_DLGTUCAMABOUT, m_lvStrings);
}


BEGIN_MESSAGE_MAP(Information, CDialog)
END_MESSAGE_MAP()

BOOL Information::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (NULL != m_opCam.hIdxTUCam)
	{
		BOOL    bUsb = FALSE;
		CString sz;
		TUCAM_VALUE_INFO valInfo;

		// 获取相机类型
		valInfo.nID = TUIDI_CAMERA_MODEL;
		if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
		{
			sz = valInfo.pText;
			GetDlgItem(IDC_STA_CAMERA)->SetWindowText(sz);
		}

		// 获取固件版本
		valInfo.nID = TUIDI_VERSION_FRMW;
		if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
		{
			sz.Format("%x",valInfo.nValue);
			GetDlgItem(IDC_STA_FW)->SetWindowText(sz);
		} 

		// 获取TUCAM-API版本
		valInfo.nID = TUIDI_VERSION_API;
		if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
		{
			sz = valInfo.pText;
			GetDlgItem(IDC_STA_SDKS)->SetWindowText(sz);
		}

		// 获取USB接口类型
		valInfo.nID = TUIDI_BUS;
		if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
		{
			if (0x210 == valInfo.nValue)
			{
				GetDlgItem(IDC_STA_USB)->SetWindowText("2.0");
			}
			else if(0x03 == valInfo.nValue)
			{
				GetDlgItem(IDC_STA_USBNAME)->SetWindowText("CameraLink");
				GetDlgItem(IDC_STA_USB)->SetWindowText("FireBird");
				bUsb = TRUE;
			}
			else if(0x04 == valInfo.nValue)
			{
				GetDlgItem(IDC_STA_USBNAME)->SetWindowText("CameraLink");
				GetDlgItem(IDC_STA_USB)->SetWindowText("Euresys");
				bUsb = TRUE;
			}
			else 
			{
				GetDlgItem(IDC_STA_USB)->SetWindowText("3.0");
			}

		}

		// 读取SN值
		char cSN[TUSN_SIZE] = {0};

		TUCAM_REG_RW regRW;
		regRW.nRegType= TUREG_SN;
		regRW.pBuf    = &cSN[0];
		regRW.nBufSize= TUSN_SIZE;

		if (TUCAMRET_SUCCESS == TUCAM_Reg_Read(m_opCam.hIdxTUCam, regRW))
		{
			GetDlgItem(IDC_STA_SN)->SetWindowText(cSN);
		}
		if(bUsb)
		{
			GetDlgItem(IDC_STA_SNNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_FWNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_SN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_FW)->ShowWindow(SW_HIDE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Information message handlers
void Information::InitInformation()
{
    if (NULL != m_opCam.hIdxTUCam)
    {
		BOOL    bUsb = FALSE;
        CString sz;
        TUCAM_VALUE_INFO valInfo;

        // 获取相机类型
        valInfo.nID = TUIDI_CAMERA_MODEL;
        if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
        {
            sz = valInfo.pText;
            GetDlgItem(IDC_STA_CAMERA)->SetWindowText(sz);
        }

        // 获取固件版本
        valInfo.nID = TUIDI_VERSION_FRMW;
        if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
        {
            sz.Format("%x",valInfo.nValue);
            GetDlgItem(IDC_STA_FW)->SetWindowText(sz);
        } 
        
        // 获取TUCAM-API版本
        valInfo.nID = TUIDI_VERSION_API;
        if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
        {
            sz = valInfo.pText;
            GetDlgItem(IDC_STA_SDKS)->SetWindowText(sz);
        }

        // 获取USB接口类型
        valInfo.nID = TUIDI_BUS;
        if (TUCAMRET_SUCCESS == TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo))
        {
            if (0x210 == valInfo.nValue)
			{
				GetDlgItem(IDC_STA_USB)->SetWindowText("2.0");
			}
			else if(0x03 == valInfo.nValue)
			{
				GetDlgItem(IDC_STA_USBNAME)->SetWindowText("CameraLink");
				GetDlgItem(IDC_STA_USB)->SetWindowText("FireBird");
				bUsb = TRUE;
			}
			else if(0x04 == valInfo.nValue)
			{
				GetDlgItem(IDC_STA_USBNAME)->SetWindowText("CameraLink");
				GetDlgItem(IDC_STA_USB)->SetWindowText("Euresys");
				bUsb = TRUE;
			}
			else 
			{
				GetDlgItem(IDC_STA_USB)->SetWindowText("3.0");
			}
        }

        // 读取SN值
        char cSN[TUSN_SIZE] = {0};

        TUCAM_REG_RW regRW;
        regRW.nRegType= TUREG_SN;
        regRW.pBuf    = &cSN[0];
        regRW.nBufSize= TUSN_SIZE;

        if (TUCAMRET_SUCCESS == TUCAM_Reg_Read(m_opCam.hIdxTUCam, regRW))
        {
            GetDlgItem(IDC_STA_SN)->SetWindowText(cSN);
        }

		if(bUsb)
		{
			GetDlgItem(IDC_STA_SNNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_FWNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_STA_SN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_FW)->ShowWindow(SW_HIDE);
		}
    }

	// 初始化字体
	//m_fntEdit.CreatePointFont(130, _T("Arial"));
    //GetDlgItem(IDC_STA_INFO)->SetFont(&m_fntEdit);
}

void Information::InitControlRange()                          // 初始化范围
{

}

void Information::RefreshValue()                             // 初始化值
{

}

BOOL Information::PreTranslateMessage(MSG* pMsg)
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

    return CDialog::PreTranslateMessage(pMsg);
}
