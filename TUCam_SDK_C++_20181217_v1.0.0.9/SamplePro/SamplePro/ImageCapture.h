#pragma once

#include "CameraControlMode.h"
#include "afxcmn.h"
// ImageCapture dialog

#define  TUMSG_CAP  WM_USER + 0x223                      // 拍照消息标识宏

class ImageCapture : public CDialog
{
	DECLARE_DYNAMIC(ImageCapture)

public:
	//ImageCapture(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImageCapture();
	ImageCapture(TUCAM_OPEN &opCam,CCameraControlMode* pParent);
	void InitControlRange();                            // 初始化控件范围
    void RefreshValue();                                // 刷新控件值
	void RefreshBtn(BOOL bLiving = FALSE);              // 刷新按钮
	void RefreshBtnRaw(BOOL bRaw = FALSE);              // 触发按键刷新
// Dialog Data
	enum { IDD = IDD_DLG_IMACAPTURE };

private:

	TUCAM_OPEN          m_opCam;                        // 打开相机参数
	CCameraControlMode* m_ccmode;                       // 控制模式指针
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // 禁用按键
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCapture();                         // 拍照按钮
	afx_msg void OnBnClickedBtnBrowse();                          // 文件路径选择 
	afx_msg void OnBnClickedChkTif();                             // 存tif图
	afx_msg void OnBnClickedChkRaw();                             // 存Raw图
	afx_msg void OnBnClickedChkJpg();                             // 存jpg图
	afx_msg void OnBnClickedChkPng();                             // 存png图
	afx_msg void OnBnClickedBtnRec();                             // 录像按钮
	afx_msg void OnBnClickedBtnCodec();                           // 录像压缩格式
	afx_msg void OnTimer(UINT_PTR nIDEvent);                      // 录像事件显示
	afx_msg LRESULT OnTUMessageCap(WPARAM wParam, LPARAM lParam); // 拍照进度显示
    afx_msg void OnDeltaposSpnPicnum(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CString				m_strFilePath;					// 文件地址
	INT                 m_nPicNum;
    CString             m_szPicName;
	CButton             m_chkRecTm;
    CButton             m_chkTif;
    CButton             m_chkRaw;

	 /* 录像 */
    BOOL                m_bRecording;                   // 是否正在录像  
    DWORD               m_dwFccHandler;                 // 编码解码器句柄    
    DWORD               m_dwTimeCnt;                    // 录像时间计数
    int                 m_nIdxFcc;

	//int                 m_nCntSave;                   // 保存计数
    int                 m_nFmtSave;                     // 保存格式
    int                 m_nFmtCnt;                      // 格式计数
	float               m_fFps;                         // 帧率

    afx_msg void OnEnChangeEdtPicname();
    afx_msg void OnDestroy();
};
