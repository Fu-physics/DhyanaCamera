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
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值     
    void UpdateDelayTime(DWORD dwExp);
// Dialog Data
	enum { IDD = IDD_DLG_TRIGER };

	void EnableTriggerControl(BOOL bEnable = FALSE);    // 触发控制使能
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针
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
