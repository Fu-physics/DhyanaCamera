// DlgCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SamplePro.h"
#include "DlgCtrl.h"


// CDlgCtrl �Ի���

IMPLEMENT_DYNAMIC(CDlgCtrl, CDialog)

CDlgCtrl::CDlgCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCtrl::IDD, pParent)
{
}

CDlgCtrl::~CDlgCtrl()
{
}

void CDlgCtrl::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_INFO, m_btnInfo);
}


BEGIN_MESSAGE_MAP(CDlgCtrl, CDialog)

END_MESSAGE_MAP()


// CDlgCtrl ��Ϣ�������


