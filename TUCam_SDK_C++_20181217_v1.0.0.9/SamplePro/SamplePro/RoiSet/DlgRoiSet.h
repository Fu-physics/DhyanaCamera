#if !defined(AFX_DLGROISET_H__1430162B_05C4_4349_8AF4_68A68C5D3AFB__INCLUDED_)
#define AFX_DLGROISET_H__1430162B_05C4_4349_8AF4_68A68C5D3AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRoiSet.h : header file
//
#ifndef MYTRACKER_
#include "MyTracker.h"
#endif

#include "../Resource.h"
//#include "CustomControl/xSkinButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRoiSet dialog

class CDlgRoiSet : public CDialog
{
// Construction
public:
	CDlgRoiSet(CWnd* pParent = NULL);   // standard constructor
	~CDlgRoiSet();

	BOOL GetPreviewRect(CRect &rt); //���ò������νӿ�
	BOOL SetPreviewRect(CRect &rt); //��ȡ��ѡ���νӿ�

    void SetScale(float fScale = 1.0f);

protected:	
	void ShowToolBar(BOOL bVal);
	void MoveToolBar(CPoint &pt);

	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);//�������浽λͼ

protected:
	int			m_xScreen;
	int			m_yScreen;
	CDC			m_MemDC;
	CDC			m_tmpDC;
	CDC			m_BacDC;
	BOOL		m_bDraw;            //��ȡ״̬
	BOOL		m_bFirstDraw;       //�״ν�ȡ
	CRect		m_PreviewRt;		//������Χ
	CPoint		m_startPt;			//��ʼ����	
    HCURSOR		m_hCursor;          //Ĭ�Ϲ��
	CBitmap*	m_pBitmap;          //ǰ��λͼ
	CBitmap*	m_pBitmap1;			//����λͼ
	CBitmap*	m_pBitmap2;			//����λͼ
	CMyTracker	m_rectTracker;		//��Ƥ����

	CButton		m_bmpBtnOK;
	CButton		m_bmpBtnCancel;

// Dialog Data
	//{{AFX_DATA(CDlgRoiSet)
	enum { IDD = IDD_DLG_ROISET };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRoiSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map funcions
	//{{AFX_MSG(CDlgRoiSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROISET_H__1430162B_05C4_4349_8AF4_68A68C5D3AFB__INCLUDED_)
