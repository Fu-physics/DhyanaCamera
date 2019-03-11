#pragma once

#include "CameraControlMode.h"
#include "afxcmn.h"
#include "afxwin.h"
// MoreParameters dialog

class MoreParameters : public CDialog
{
	DECLARE_DYNAMIC(MoreParameters)

public:
	//MoreParameters(CWnd* pParent = NULL);   // standard constructor
	//virtual ~MoreParameters();
	MoreParameters(TUCAM_OPEN &opCam,CCameraControlMode* pParent); 
	//void  OnRefresh();                                // ��ʼ��
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
	void RefreshBtn();                                  // ˢ��ɫ�װ�ť
	void RefreshLevelRange();                           // ˢ��ɫ�׷�Χ
// Dialog Data
	enum { IDD = IDD_DLG_MOREPARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��
public:
	
	afx_msg void OnBnClickedChkAtl();                   // �Զ���ɫ�׺���
	afx_msg void OnBnClickedChkAtr();                   // �Զ���ɫ�׺���

	afx_msg void OnNMCustomdrawSldLlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSldRlevel(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMCustomdrawSldCool(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComCurtemp();            // ���÷��ȵ�λ
	afx_msg void OnBnClickedBtnDefault();               // Ĭ��ɫ��ֵ
	afx_msg void OnTimer(UINT_PTR nIDEvent);            // �����ǰ�¶���ʾ
    afx_msg void OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL         m_bRLvlRange;                   // ������ɫ�׷�Χ
    BOOL         m_bGammaRange;                  // ����٤�귶Χ
    BOOL         m_bContrastRange;               // ���öԱȶȷ�Χ
    BOOL         m_bSharpRange;                  // �����񻯷�Χ
    BOOL		 m_bRGainRange;					 // ���ú�ɫ���淶Χ
    BOOL	     m_bGGainRange;				     // ������ɫ���淶Χ
    BOOL	     m_bBGainRange;					 // ������ɫ���淶Χ
    BOOL		 m_bSatRange;					 // ���ñ��Ͷȷ�Χ
    BOOL		 m_bClrTempRange;				 // ����ɫ�·�Χ

	int          m_nMidTemp;                     // �¶Ȼ���ֵ
	CSliderCtrl  m_sldLLevel;                    // ��ɫ��ֵ����
    CSliderCtrl  m_sldRLevel;                    // ��ɫ��ֵ����
    CSliderCtrl  m_sldCool;                      // �¶�����
    CSliderCtrl  m_sldContrast;                  // �Աȶ�����
    CSliderCtrl  m_sldSharp;                     // ������
    CSliderCtrl  m_sldGamma;                     // ٤������   
    CSliderCtrl  m_sldRGain;                     // ���ú�ɫ����
    CSliderCtrl  m_sldGGain;                     // ������ɫ����
    CSliderCtrl  m_sldBGain;                     // ������ɫ����
    CSliderCtrl  m_sldSat;                       // ���ñ��Ͷ�
    CSliderCtrl  m_sldClrTemp;                   // ����ɫ��

    CComboBox    m_cmbFan;                       // ���÷��ȵ�λ��ť
    CButton      m_chkATLeft;                    // �Զ���ɫ�װ�ť
    CButton      m_chkATRight;                   // �Զ���ɫ�װ�ť
    CButton      m_chkWB;                        // �Զ���ƽ�ⰴť

    afx_msg void OnNMCustomdrawSldGamm(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldSharp(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldRchn(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldGchn(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldBchn(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldSat(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldClrtemp(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnWb();
    afx_msg void OnBnClickedChkAb();
    afx_msg void OnBnClickedChkDpcOff();
    afx_msg void OnBnClickedChkDpcLow();
    afx_msg void OnBnClickedChkDpcMed();
    afx_msg void OnBnClickedChkDpcHigh();
    CButton m_chkDPCOff;
    CButton m_chkDPCLow;
    CButton m_chkDPCMed;
    CButton m_chkDPCHigh;
    afx_msg void OnBnClickedBtnBb();
};
