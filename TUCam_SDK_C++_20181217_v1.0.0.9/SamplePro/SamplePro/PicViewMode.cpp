// PicViewMode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SamplePro.h"
#include "PicViewMode.h"


// CPicViewMode �Ի���

IMPLEMENT_DYNAMIC(CPicViewMode, CDialog)

CPicViewMode::CPicViewMode(CWnd* pParent /*=NULL*/)
	: CDialog(CPicViewMode::IDD, pParent)
{
}

CPicViewMode::~CPicViewMode()
{
}

void CPicViewMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicViewMode, CDialog)
	ON_WM_SIZE()
//    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPicViewMode ��Ϣ�������

void CPicViewMode::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

//void CPicViewMode::OnClose()
//{
//    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//    CDialog::OnClose();
//}

BOOL CPicViewMode::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
    {  
        return TRUE;  
    }

    return CDialog::PreTranslateMessage(pMsg);
}
