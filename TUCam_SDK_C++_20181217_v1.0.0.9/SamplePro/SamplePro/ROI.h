#pragma once

#include "CameraControlMode.h"
// ROI dialog

class ROI : public CDialog
{
	DECLARE_DYNAMIC(ROI)

public:
	//ROI(CWnd* pParent = NULL);   // standard constructor
	//virtual ~ROI();
	ROI(TUCAM_OPEN &opCam,CCameraControlMode* pParent);
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值
	void RefreshState();
// Dialog Data
	enum { IDD = IDD_DLG_ROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针
public:
	afx_msg void OnBnClickedChkRoi();                   // 自定义ROI区域
	afx_msg void OnBnClickedBtnRoi();                   // 做ROI区域

	CButton m_bROI;
	UINT m_uiHOffset;                                   // ROI水平方向
    UINT m_uiVOffset;                                   // ROI垂直方向
    UINT m_uiWidth;                                     // ROI宽度
    UINT m_uiHeight;                                    // ROI高度
};
