#pragma once

#include "CameraControlMode.h"
#include "afxcmn.h"
// ImageCapture dialog

#define  TUMSG_CAP  WM_USER + 0x223                      // ������Ϣ��ʶ��

class ImageCapture : public CDialog
{
	DECLARE_DYNAMIC(ImageCapture)

public:
	//ImageCapture(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImageCapture();
	ImageCapture(TUCAM_OPEN &opCam,CCameraControlMode* pParent);
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
	void RefreshBtn(BOOL bLiving = FALSE);              // ˢ�°�ť
	void RefreshBtnRaw(BOOL bRaw = FALSE);              // ��������ˢ��
// Dialog Data
	enum { IDD = IDD_DLG_IMACAPTURE };

private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCapture();                         // ���հ�ť
	afx_msg void OnBnClickedBtnBrowse();                          // �ļ�·��ѡ�� 
	afx_msg void OnBnClickedChkTif();                             // ��tifͼ
	afx_msg void OnBnClickedChkRaw();                             // ��Rawͼ
	afx_msg void OnBnClickedChkJpg();                             // ��jpgͼ
	afx_msg void OnBnClickedChkPng();                             // ��pngͼ
	afx_msg void OnBnClickedBtnRec();                             // ¼��ť
	afx_msg void OnBnClickedBtnCodec();                           // ¼��ѹ����ʽ
	afx_msg void OnTimer(UINT_PTR nIDEvent);                      // ¼���¼���ʾ
	afx_msg LRESULT OnTUMessageCap(WPARAM wParam, LPARAM lParam); // ���ս�����ʾ
    afx_msg void OnDeltaposSpnPicnum(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CString				m_strFilePath;					// �ļ���ַ
	INT                 m_nPicNum;
    CString             m_szPicName;
	CButton             m_chkRecTm;
    CButton             m_chkTif;
    CButton             m_chkRaw;

	 /* ¼�� */
    BOOL                m_bRecording;                   // �Ƿ�����¼��  
    DWORD               m_dwFccHandler;                 // ������������    
    DWORD               m_dwTimeCnt;                    // ¼��ʱ�����
    int                 m_nIdxFcc;

	//int                 m_nCntSave;                   // �������
    int                 m_nFmtSave;                     // �����ʽ
    int                 m_nFmtCnt;                      // ��ʽ����
	float               m_fFps;                         // ֡��

    afx_msg void OnEnChangeEdtPicname();
    afx_msg void OnDestroy();
};
