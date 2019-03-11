// Raminfo.cpp : implementation file
//

#include "stdafx.h"
#include "SamplePro.h"
#include "Raminfo.h"
//#include "afxdialogex.h"


// Raminfo dialog

IMPLEMENT_DYNAMIC(Raminfo, CDialog)

Raminfo::Raminfo(CWnd* pParent /*=NULL*/)
	: CDialog(Raminfo::IDD, pParent)
	, m_fActual(1.00)
	, m_nPercent(70)
{

}

Raminfo::~Raminfo()
{
}

void Raminfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ACTUAL, m_fActual);
	DDX_Text(pDX, IDC_EDT_PERCENT, m_nPercent);
	DDV_MinMaxUInt(pDX, m_nPercent, 0, 100);
}


BEGIN_MESSAGE_MAP(Raminfo, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ACTUAL, &Raminfo::OnBnClickedRadioActual)
	ON_BN_CLICKED(IDC_RADIO_PERCENT, &Raminfo::OnBnClickedRadioPercent)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_PERCENT, &Raminfo::OnDeltaposSpnPercent)
END_MESSAGE_MAP()


// Raminfo message handlers
BOOL Raminfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CButton *)GetDlgItem(IDC_RADIO_ACTUAL))->SetCheck(TRUE);
	float m_fTotal = 8.00;
	float m_fAvaiable = 4.00;
	CString sz;
	sz.Format(_T("Total Physical£º[Mem=%.2fGB] (%.2fGB Available)"), m_fTotal, m_fAvaiable);
	GetDlgItem(IDC_STATIC_RAMINFO)->SetWindowText(sz);
	return true;
}

void Raminfo::OnBnClickedRadioActual()
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_RADIO_ACTUAL))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_PERCENT))->SetCheck(FALSE);
}


void Raminfo::OnBnClickedRadioPercent()
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_RADIO_ACTUAL))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO_PERCENT))->SetCheck(TRUE);
}


void Raminfo::OnDeltaposSpnPercent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMUpDown->iDelta < 0)
	{
		if (m_nPercent >= 100)
		{
			return;
		}
		m_nPercent +=  1;

		CString str;
		str.Format(_T("%d"), m_nPercent);
		GetDlgItem(IDC_EDT_PERCENT)->SetWindowText(str);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		if (m_nPercent == 0)
		{
			return;
		}
		m_nPercent -= 1;     

		CString str;
		str.Format(_T("%d"), m_nPercent);
		GetDlgItem(IDC_EDT_PERCENT)->SetWindowText(str);
	}
	UpdateData(FALSE);
	*pResult = 0;
}
