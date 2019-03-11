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
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值
// Dialog Data
	enum { IDD = IDD_DLG_OTHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针
public:
	afx_msg void OnBnClickedBtnFfc();                   // 平场校正
	afx_msg void OnBnClickedChkFfc();                   // 开启平场校正
	afx_msg void OnBnClickedBtnDfc();                   // 坏行坏列校正
	afx_msg void OnBnClickedBtnWrsn();                  // 写序列号

public:

	CString             m_szWRSN;
	CButton             m_chkFFC;
	 /* 平场校正 */
    int                 m_nFFCStep;                     // 平场校正步骤
};
