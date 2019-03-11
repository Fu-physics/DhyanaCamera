#pragma once

#include "CameraControlMode.h"
#include "afxwin.h"
// CameraControl dialog
// class CCameraControlMode;
#define  TUMSG_FPS  WM_USER + 0x222                     // 帧率消息标识宏
#define  TUMSG_MEAN  WM_USER + 0x230                    // 帧率消息标识宏
#define  TUMSG_STD   WM_USER + 0x240                   // 帧率消息标识宏

class CameraControl : public CDialog
{
	DECLARE_DYNAMIC(CameraControl)

public:
	//CameraControl(CWnd* pParent = NULL);   // standard constructor
	//virtual ~CameraControl();
	CameraControl(TUCAM_OPEN &opCam, CCameraControlMode* pParent);
    ~CameraControl();
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值
	void RefreshBtn(BOOL bLiving = FALSE);              // 刷新按钮
	void BtnShow(BOOL mLiving = FALSE);                 // 预览相机按钮状态显示

	enum { IDD = IDD_DLG_CAMCONTROL };

	void  UpdateExposureTime(DWORD dwExp);              // 更新曝光时间 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnLive();                  // 预览按钮
	afx_msg void OnTimer(UINT_PTR nIDEvent);            // 帧率显示

private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针

private:
   void LedShow(BOOL bLed);                     // Led灯按钮状态

public:
	/* 曝光时间 */
	UINT				m_uSec;							// 秒钟
	UINT				m_uMs;							// 毫秒
	UINT				m_uUs;							// 微秒

	UINT                m_uiExpStep;                    // 曝光时间步长
	BOOL                m_bTargetRange;                 // 设置目标值范围   
	CComboBox           m_cmbRes;                       // 分辨率ComboBox
	CButton             m_chkBDep;                      // Bit数切换
	CButton             m_chkAE;                        // 自动曝光按钮
	CSliderCtrl         m_sldTgr;                       // 自动曝光目标值设置
	BOOL                m_bFirst;                       // 统计帧率 
	UINT                m_ucount;                       // 周期计数
	
//  float               m_ffps;                         // 帧率
	float               m_favg;                         // 当前平均帧率
	float               m_ffast;                        // 当前最快帧率
	float               m_fslow;                        // 当前最慢帧率
	int                 m_nGain;                        // 增益模式
	BOOL                m_bCmsEn;                       // 是否存在CMS模式Gain
	CSliderCtrl         m_sldGain;                      // 设置增益
    BOOL                m_bGainRange;                   // 设置增益范围 
    UINT                m_uiMaxGain;                    // 增益最大值
    CComboBox           m_SlaveAddress;
    CString             m_sRegAddress;
	CString             m_sValue;
    BOOL                m_bLedState;                    // Led灯状态

	afx_msg void OnCbnSelchangeCmbRes();                                // 分辨率设置        
	afx_msg void OnBnClickedChkAe();                                    // 自动曝光按钮
	afx_msg void OnNMCustomdrawSldTgr(NMHDR *pNMHDR, LRESULT *pResult); // 自动曝光目标值
	afx_msg void OnDeltaposSpnSec(NMHDR *pNMHDR, LRESULT *pResult);     // 曝光时间 秒
	afx_msg void OnDeltaposSpnMs(NMHDR *pNMHDR, LRESULT *pResult);      // 曝光时间 微秒
	afx_msg void OnDeltaposSpnUs(NMHDR *pNMHDR, LRESULT *pResult);      // 曝光时间 毫秒
	afx_msg void OnBnClickedBtnExptm();                                 // 下曝光时间
	afx_msg void OnBnClickedChkBitofdepth();                            // 图像位数
	afx_msg void OnBnClickedRadioHdr();                                 // HDR函数
	afx_msg void OnBnClickedRadioHighgain();                            // 高增益函数
	afx_msg void OnBnClickedRadioLowgain();                             // 低增益函数
    afx_msg void OnNMCustomdrawSldGain(NMHDR *pNMHDR, LRESULT *pResult);// 增益
	afx_msg LRESULT OnTUMessageFps(WPARAM wParam, LPARAM lParam);       // 帧率显示
	afx_msg LRESULT OnTUMessageMean(WPARAM wParam, LPARAM lParam);      // Mean显示
	afx_msg LRESULT OnTUMessageStd(WPARAM wParam, LPARAM lParam);       // Std显示
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
