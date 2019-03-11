#pragma once

#include "CameraControlMode.h"
#include "afxcmn.h"
// Information dialog

class Information : public CDialog
{
	DECLARE_DYNAMIC(Information)

public:
	//Information(CWnd* pParent = NULL);   // standard constructor
	//virtual ~Information();
	Information(TUCAM_OPEN &opCam, CCameraControlMode* pParent);

	void InitInformation();                             // 初始化信息
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值

// Dialog Data
	enum { IDD = IDD_DLG_INF };                        

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()

private:
	 TUCAM_OPEN          m_opCam;                       // 打开相机参数
	 CCameraControlMode* m_ccmode;                      // 控制模式指针
public:

	//CListCtrl m_lvStrings;                            // 报告输出
};
