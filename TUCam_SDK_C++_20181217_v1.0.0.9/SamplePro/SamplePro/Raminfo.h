#pragma once


// Raminfo dialog

class Raminfo : public CDialog
{
	DECLARE_DYNAMIC(Raminfo)

public:
	Raminfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~Raminfo();

// Dialog Data
	enum { IDD = IDD_DLGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioActual();
	afx_msg void OnBnClickedRadioPercent();
	float m_fActual;
	UINT m_nPercent;
	afx_msg void OnDeltaposSpnPercent(NMHDR *pNMHDR, LRESULT *pResult);
};
