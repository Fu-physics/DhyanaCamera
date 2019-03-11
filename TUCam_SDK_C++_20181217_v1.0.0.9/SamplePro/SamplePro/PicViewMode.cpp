// PicViewMode.cpp : 实现文件
//

#include "stdafx.h"
#include "SamplePro.h"
#include "PicViewMode.h"


// CPicViewMode 对话框

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


// CPicViewMode 消息处理程序

void CPicViewMode::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

//void CPicViewMode::OnClose()
//{
//    // TODO: 在此添加消息处理程序代码和/或调用默认值
//
//    CDialog::OnClose();
//}

BOOL CPicViewMode::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
    {  
        return TRUE;  
    }

    return CDialog::PreTranslateMessage(pMsg);
}
