#pragma once


// CPicViewMode �Ի���

class CPicViewMode : public CDialog
{
	DECLARE_DYNAMIC(CPicViewMode)

public:
	CPicViewMode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicViewMode();

// �Ի�������
	enum { IDD = IDD_DLG_PICVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnSize(UINT nType, int cx, int cy);
//    afx_msg void OnClose();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
