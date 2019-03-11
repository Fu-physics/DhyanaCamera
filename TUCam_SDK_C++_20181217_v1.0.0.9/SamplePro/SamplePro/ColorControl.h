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
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值
// Dialog Data
	enum { IDD = IDD_DLG_CLRCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()
private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针
public:
	afx_msg void OnBnClickedBtnWb();                    // 设置AWB
	afx_msg void OnBnClickedChkAtwb();                  // 锁定AWB
	afx_msg void OnNMCustomdrawSldRgain(NMHDR *pNMHDR, LRESULT *pResult);       // 设置RGain
	afx_msg void OnNMCustomdrawSldGgain(NMHDR *pNMHDR, LRESULT *pResult);       // 设置GGain
	afx_msg void OnNMCustomdrawSldBgain(NMHDR *pNMHDR, LRESULT *pResult);       // 设置BGain
	afx_msg void OnNMCustomdrawSldSaturation(NMHDR *pNMHDR, LRESULT *pResult);  // 设置饱和度
	afx_msg void OnNMCustomdrawSldGamma(NMHDR *pNMHDR, LRESULT *pResult);       // 设置伽马
	afx_msg void OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult);    // 设置对比度
	afx_msg void OnNMCustomdrawSldCms(NMHDR *pNMHDR, LRESULT *pResult);         // 设置色温  
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CSliderCtrl         m_sldRGain;                     // 设置红色增益
	CSliderCtrl         m_sldGGain;                     // 设置绿色增益
	CSliderCtrl         m_sldBGain;                     // 设置蓝色增益
	CSliderCtrl         m_sldSat;                       // 设置饱和度
	CSliderCtrl         m_sldGamma;                     // 设置伽马
	CSliderCtrl         m_sldContrast;                  // 设置对比度
	
	CSliderCtrl         m_sldCMS;                       // 设置色温  
	CButton             m_chkATWB;                      // 设置白平衡

	BOOL				m_bRGainRange;					// 设置红色增益范围
	BOOL				m_bGGainRange;				    // 设置绿色增益范围
	BOOL				m_bBGainRange;					// 设置蓝色增益范围
	BOOL				m_bSatRange;					// 设置饱和度范围
	BOOL				m_bGammaRange;					// 设置伽马值范围
    
    BOOL                m_bTargetRange;                 // 设置目标值范围   
    BOOL                m_bContrastRange;               // 设置对比度范围   
    BOOL                m_bCMSRange;                    // 设置CMS范围

	
};
