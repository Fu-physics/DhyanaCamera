#pragma once
#include "afxwin.h"
#include "TUButton.h"


// CDlgCtrl �Ի���

class CDlgCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgCtrl)

public:
	CDlgCtrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCtrl();

// �Ի�������
	enum { IDD = IDD_DLG_CTL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

    CTUButton m_btnInfo;
};
