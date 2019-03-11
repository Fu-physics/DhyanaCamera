#pragma once


// CPicViewMode 对话框

class CPicViewMode : public CDialog
{
	DECLARE_DYNAMIC(CPicViewMode)

public:
	CPicViewMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicViewMode();

// 对话框数据
	enum { IDD = IDD_DLG_PICVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnSize(UINT nType, int cx, int cy);
//    afx_msg void OnClose();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
