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
	//void  OnRefresh();                                // 初始化
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值
	void RefreshBtn();                                  // 刷新色阶按钮
	void RefreshLevelRange();                           // 刷新色阶范围
// Dialog Data
	enum { IDD = IDD_DLG_MOREPARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针
public:
	
	afx_msg void OnBnClickedChkAtl();                   // 自动左色阶函数
	afx_msg void OnBnClickedChkAtr();                   // 自动右色阶函数

	afx_msg void OnNMCustomdrawSldLlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSldRlevel(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMCustomdrawSldCool(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComCurtemp();            // 设置风扇档位
	afx_msg void OnBnClickedBtnDefault();               // 默认色阶值
	afx_msg void OnTimer(UINT_PTR nIDEvent);            // 相机当前温度显示
    afx_msg void OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL         m_bRLvlRange;                   // 设置右色阶范围
    BOOL         m_bGammaRange;                  // 设置伽玛范围
    BOOL         m_bContrastRange;               // 设置对比度范围
    BOOL         m_bSharpRange;                  // 设置锐化范围
    BOOL		 m_bRGainRange;					 // 设置红色增益范围
    BOOL	     m_bGGainRange;				     // 设置绿色增益范围
    BOOL	     m_bBGainRange;					 // 设置蓝色增益范围
    BOOL		 m_bSatRange;					 // 设置饱和度范围
    BOOL		 m_bClrTempRange;				 // 设置色温范围

	int          m_nMidTemp;                     // 温度换算值
	CSliderCtrl  m_sldLLevel;                    // 左色阶值设置
    CSliderCtrl  m_sldRLevel;                    // 右色阶值设置
    CSliderCtrl  m_sldCool;                      // 温度设置
    CSliderCtrl  m_sldContrast;                  // 对比度设置
    CSliderCtrl  m_sldSharp;                     // 锐化设置
    CSliderCtrl  m_sldGamma;                     // 伽玛设置   
    CSliderCtrl  m_sldRGain;                     // 设置红色增益
    CSliderCtrl  m_sldGGain;                     // 设置绿色增益
    CSliderCtrl  m_sldBGain;                     // 设置蓝色增益
    CSliderCtrl  m_sldSat;                       // 设置饱和度
    CSliderCtrl  m_sldClrTemp;                   // 设置色温

    CComboBox    m_cmbFan;                       // 设置风扇档位按钮
    CButton      m_chkATLeft;                    // 自动左色阶按钮
    CButton      m_chkATRight;                   // 自动右色阶按钮
    CButton      m_chkWB;                        // 自动白平衡按钮

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
