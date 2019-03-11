#pragma once

#include "CameraControlMode.h"
// Trigger dialog

class Trigger : public CDialog
{
	DECLARE_DYNAMIC(Trigger)

public:
	//Trigger(CWnd* pParent = NULL);   // standard constructor
	//virtual ~Trigger();
	Trigger(TUCAM_OPEN &opCam,CCameraControlMode* pParent); 
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ     
    void UpdateDelayTime(DWORD dwExp);
// Dialog Data
	enum { IDD = IDD_DLG_TRIGER };

	void EnableTriggerControl(BOOL bEnable = FALSE);    // ��������ʹ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��
public:
	afx_msg void OnBnClickedRadioOff();
	afx_msg void OnBnClickedRadioStandard();
	afx_msg void OnBnClickedRadioSynchro();
	afx_msg void OnBnClickedRadioGloabal();
	afx_msg void OnBnClickedRadioTimed();
	afx_msg void OnBnClickedRadioWidth();
	afx_msg void OnBnClickedRadioRising();
	afx_msg void OnBnClickedRadioFalling();
	afx_msg void OnBnClickedChkSttrigger();
	afx_msg void OnBnClickedBtnSnap();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnApply();

	CButton m_chkSFWTri;
	int m_nTriMode;
    int m_nTriExp;
    int m_nTriEdge;
	CEdit m_edtFrame;
    afx_msg void OnDeltaposSpnSec(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpnMs(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpnUs(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnStnClickedStaDelay();
    UINT m_uSec;
    UINT m_uMs;
    UINT m_uUs;
    LONG m_lDelay;
};
