#pragma once

#include "CameraControlMode.h"
#include "afxwin.h"
// CameraControl dialog
// class CCameraControlMode;
#define  TUMSG_FPS  WM_USER + 0x222                     // ֡����Ϣ��ʶ��
#define  TUMSG_MEAN  WM_USER + 0x230                    // ֡����Ϣ��ʶ��
#define  TUMSG_STD   WM_USER + 0x240                   // ֡����Ϣ��ʶ��

class CameraControl : public CDialog
{
	DECLARE_DYNAMIC(CameraControl)

public:
	//CameraControl(CWnd* pParent = NULL);   // standard constructor
	//virtual ~CameraControl();
	CameraControl(TUCAM_OPEN &opCam, CCameraControlMode* pParent);
    ~CameraControl();
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
	void RefreshBtn(BOOL bLiving = FALSE);              // ˢ�°�ť
	void BtnShow(BOOL mLiving = FALSE);                 // Ԥ�������ť״̬��ʾ

	enum { IDD = IDD_DLG_CAMCONTROL };

	void  UpdateExposureTime(DWORD dwExp);              // �����ع�ʱ�� 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnLive();                  // Ԥ����ť
	afx_msg void OnTimer(UINT_PTR nIDEvent);            // ֡����ʾ

private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��

private:
   void LedShow(BOOL bLed);                     // Led�ư�ť״̬

public:
	/* �ع�ʱ�� */
	UINT				m_uSec;							// ����
	UINT				m_uMs;							// ����
	UINT				m_uUs;							// ΢��

	UINT                m_uiExpStep;                    // �ع�ʱ�䲽��
	BOOL                m_bTargetRange;                 // ����Ŀ��ֵ��Χ   
	CComboBox           m_cmbRes;                       // �ֱ���ComboBox
	CButton             m_chkBDep;                      // Bit���л�
	CButton             m_chkAE;                        // �Զ��عⰴť
	CSliderCtrl         m_sldTgr;                       // �Զ��ع�Ŀ��ֵ����
	BOOL                m_bFirst;                       // ͳ��֡�� 
	UINT                m_ucount;                       // ���ڼ���
	
//  float               m_ffps;                         // ֡��
	float               m_favg;                         // ��ǰƽ��֡��
	float               m_ffast;                        // ��ǰ���֡��
	float               m_fslow;                        // ��ǰ����֡��
	int                 m_nGain;                        // ����ģʽ
	BOOL                m_bCmsEn;                       // �Ƿ����CMSģʽGain
	CSliderCtrl         m_sldGain;                      // ��������
    BOOL                m_bGainRange;                   // �������淶Χ 
    UINT                m_uiMaxGain;                    // �������ֵ
    CComboBox           m_SlaveAddress;
    CString             m_sRegAddress;
	CString             m_sValue;
    BOOL                m_bLedState;                    // Led��״̬

	afx_msg void OnCbnSelchangeCmbRes();                                // �ֱ�������        
	afx_msg void OnBnClickedChkAe();                                    // �Զ��عⰴť
	afx_msg void OnNMCustomdrawSldTgr(NMHDR *pNMHDR, LRESULT *pResult); // �Զ��ع�Ŀ��ֵ
	afx_msg void OnDeltaposSpnSec(NMHDR *pNMHDR, LRESULT *pResult);     // �ع�ʱ�� ��
	afx_msg void OnDeltaposSpnMs(NMHDR *pNMHDR, LRESULT *pResult);      // �ع�ʱ�� ΢��
	afx_msg void OnDeltaposSpnUs(NMHDR *pNMHDR, LRESULT *pResult);      // �ع�ʱ�� ����
	afx_msg void OnBnClickedBtnExptm();                                 // ���ع�ʱ��
	afx_msg void OnBnClickedChkBitofdepth();                            // ͼ��λ��
	afx_msg void OnBnClickedRadioHdr();                                 // HDR����
	afx_msg void OnBnClickedRadioHighgain();                            // �����溯��
	afx_msg void OnBnClickedRadioLowgain();                             // �����溯��
    afx_msg void OnNMCustomdrawSldGain(NMHDR *pNMHDR, LRESULT *pResult);// ����
	afx_msg LRESULT OnTUMessageFps(WPARAM wParam, LPARAM lParam);       // ֡����ʾ
	afx_msg LRESULT OnTUMessageMean(WPARAM wParam, LPARAM lParam);      // Mean��ʾ
	afx_msg LRESULT OnTUMessageStd(WPARAM wParam, LPARAM lParam);       // Std��ʾ
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnOnceae();
	afx_msg void OnBnClickedBtnW();
	afx_msg void OnBnClickedBtnR();

	
	afx_msg void OnBnClickedRadioCms();
	CComboBox m_cmbBitOfDepth;
	afx_msg void OnCbnSelchangeCom();
	afx_msg void OnBnClickedBtnUsbled();
};
