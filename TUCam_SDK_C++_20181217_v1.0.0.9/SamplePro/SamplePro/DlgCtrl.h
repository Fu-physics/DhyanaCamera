#pragma once
#include "afxwin.h"
#include "TUButton.h"


// CDlgCtrl 对话框

class CDlgCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgCtrl)

public:
	CDlgCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCtrl();

// 对话框数据
	enum { IDD = IDD_DLG_CTL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

    CTUButton m_btnInfo;
};
