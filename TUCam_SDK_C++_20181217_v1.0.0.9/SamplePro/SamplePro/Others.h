#pragma once

#include "CameraControlMode.h"
// Others dialog

class Others : public CDialog
{
	DECLARE_DYNAMIC(Others)

public:
	//Others(CWnd* pParent = NULL);   // standard constructor
	//virtual ~Others();
	Others(TUCAM_OPEN &opCam,CCameraControlMode* pParent);
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
// Dialog Data
	enum { IDD = IDD_DLG_OTHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��
public:
	afx_msg void OnBnClickedBtnFfc();                   // ƽ��У��
	afx_msg void OnBnClickedChkFfc();                   // ����ƽ��У��
	afx_msg void OnBnClickedBtnDfc();                   // ���л���У��
	afx_msg void OnBnClickedBtnWrsn();                  // д���к�

public:

	CString             m_szWRSN;
	CButton             m_chkFFC;
	 /* ƽ��У�� */
    int                 m_nFFCStep;                     // ƽ��У������
};
