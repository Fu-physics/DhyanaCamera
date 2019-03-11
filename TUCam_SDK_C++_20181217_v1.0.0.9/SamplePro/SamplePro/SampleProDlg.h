
// SampleProDlg.h : ͷ�ļ�
//
#pragma once

#include "CameraControlMode.h"
#include "PicViewMode.h"

// CSampleProDlg �Ի���
class CSampleProDlg : public CDialog
{
// ����
public:
	CSampleProDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSampleProDlg();

// �Ի�������
	enum { IDD = IDD_SAMPLEPRO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
    /*ˢ�½���*/
    void RefreshUI();

    CPicViewMode			m_PicViewMode;		//��ͼ����
    CCameraControlMode      m_CamCtlMode;       //�����������

// 	CCameraControlMode		*m_CamCtlMode;		//�����������
// 	CPicViewMode			*m_PicViewMode;		//��ͼ����

	BOOL		m_bShowScroll;					//�Ƿ���ʾ������

	CRect		m_recPicView;					//��ͼ������
	CRect		m_recCamCtlView;				//�������������

	float		m_fProportion_H;				//�߶ȱ�
	float		m_fProportion_W;				//��ȱ�
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
