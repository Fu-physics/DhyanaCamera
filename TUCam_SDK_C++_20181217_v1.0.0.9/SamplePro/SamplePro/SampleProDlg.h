
// SampleProDlg.h : 头文件
//
#pragma once

#include "CameraControlMode.h"
#include "PicViewMode.h"

// CSampleProDlg 对话框
class CSampleProDlg : public CDialog
{
// 构造
public:
	CSampleProDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSampleProDlg();

// 对话框数据
	enum { IDD = IDD_SAMPLEPRO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
    /*刷新界面*/
    void RefreshUI();

    CPicViewMode			m_PicViewMode;		//视图区域
    CCameraControlMode      m_CamCtlMode;       //相机控制区域

// 	CCameraControlMode		*m_CamCtlMode;		//相机控制区域
// 	CPicViewMode			*m_PicViewMode;		//视图区域

	BOOL		m_bShowScroll;					//是否显示滚动条

	CRect		m_recPicView;					//视图区窗口
	CRect		m_recCamCtlView;				//相机控制区窗口

	float		m_fProportion_H;				//高度比
	float		m_fProportion_W;				//宽度比
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
