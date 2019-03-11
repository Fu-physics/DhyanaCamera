#pragma once

#include "CameraControlMode.h"
// ColorControl dialog

class ColorControl : public CDialog
{
	DECLARE_DYNAMIC(ColorControl)

public:
	//ColorControl(CWnd* pParent = NULL);   // standard constructor
	//virtual ~ColorControl();
	ColorControl(TUCAM_OPEN &opCam,CCameraControlMode* pParent);
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
// Dialog Data
	enum { IDD = IDD_DLG_CLRCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()
private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��
public:
	afx_msg void OnBnClickedBtnWb();                    // ����AWB
	afx_msg void OnBnClickedChkAtwb();                  // ����AWB
	afx_msg void OnNMCustomdrawSldRgain(NMHDR *pNMHDR, LRESULT *pResult);       // ����RGain
	afx_msg void OnNMCustomdrawSldGgain(NMHDR *pNMHDR, LRESULT *pResult);       // ����GGain
	afx_msg void OnNMCustomdrawSldBgain(NMHDR *pNMHDR, LRESULT *pResult);       // ����BGain
	afx_msg void OnNMCustomdrawSldSaturation(NMHDR *pNMHDR, LRESULT *pResult);  // ���ñ��Ͷ�
	afx_msg void OnNMCustomdrawSldGamma(NMHDR *pNMHDR, LRESULT *pResult);       // ����٤��
	afx_msg void OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult);    // ���öԱȶ�
	afx_msg void OnNMCustomdrawSldCms(NMHDR *pNMHDR, LRESULT *pResult);         // ����ɫ��  
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CSliderCtrl         m_sldRGain;                     // ���ú�ɫ����
	CSliderCtrl         m_sldGGain;                     // ������ɫ����
	CSliderCtrl         m_sldBGain;                     // ������ɫ����
	CSliderCtrl         m_sldSat;                       // ���ñ��Ͷ�
	CSliderCtrl         m_sldGamma;                     // ����٤��
	CSliderCtrl         m_sldContrast;                  // ���öԱȶ�
	
	CSliderCtrl         m_sldCMS;                       // ����ɫ��  
	CButton             m_chkATWB;                      // ���ð�ƽ��

	BOOL				m_bRGainRange;					// ���ú�ɫ���淶Χ
	BOOL				m_bGGainRange;				    // ������ɫ���淶Χ
	BOOL				m_bBGainRange;					// ������ɫ���淶Χ
	BOOL				m_bSatRange;					// ���ñ��Ͷȷ�Χ
	BOOL				m_bGammaRange;					// ����٤��ֵ��Χ
    
    BOOL                m_bTargetRange;                 // ����Ŀ��ֵ��Χ   
    BOOL                m_bContrastRange;               // ���öԱȶȷ�Χ   
    BOOL                m_bCMSRange;                    // ����CMS��Χ

	
};
