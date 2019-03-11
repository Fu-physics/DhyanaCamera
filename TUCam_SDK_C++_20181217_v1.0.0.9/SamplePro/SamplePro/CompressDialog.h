#if !defined(AFX_COMPRESSDIALOG_H__EE86DA5D_8C3A_40B6_B23F_077614B9F798__INCLUDED_)
#define AFX_COMPRESSDIALOG_H__EE86DA5D_8C3A_40B6_B23F_077614B9F798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompressDialog.h : header file
#include "stdafx.h"
#include "resource.h"
#include <vfw.h>

#define MAKETAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))
/////////////////////////////////////////////////////////////////////////////
// CCompressDialog dialog

class CCompressDialog : public CDialog
{
// Construction
public:
	CCompressDialog(CWnd* pParent = NULL);   // standard constructor
	
	// ȡѡ����CODEC����
    BOOL GetCompressOptions(AVICOMPRESSOPTIONS* pOpt); 
	
    // ��ȡ��ǰѡ���CODEC
    DWORD GetCompressorFccHandler();
	
    int GetCurrentFcc();

	// Dialog Data
	//{{AFX_DATA(CCompressDialog)
	enum { IDD = IDD_DLG_COMPRESS };
	CComboBox	m_CompressorCtrl;
    int         m_nIdxFcc;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompressDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// �û�ѡ���CODEC�����ַ���ʾ���磺"DIB "��
	CDWordArray  m_CodecFcc; 
	
    // CODEC������ֵ
	CDWordArray  m_CodecIndex;
	
    // CODEC��ʾ�ڶԻ����е�����
	CStringArray m_CodecNames;
    
    // �㷨�б����ѡ�������ֵ
    int    nSelIndex;
	
    // CODEC�Ƿ�������ñ�־
    BOOL   m_CanConfig;
	
private:
	BOOL EnumVideoCodecs();
	
	// Generated message map functions
	//{{AFX_MSG(CCompressDialog)
	afx_msg void OnSelchangeComboCompress();
	afx_msg void OnBtnSetting();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPRESSDIALOG_H__EE86DA5D_8C3A_40B6_B23F_077614B9F798__INCLUDED_)
