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
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
	void RefreshState();
// Dialog Data
	enum { IDD = IDD_DLG_ROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()

private:

	TUCAM_OPEN          m_opCam;                        // ���������
	CCameraControlMode* m_ccmode;                       // ����ģʽָ��
public:
	afx_msg void OnBnClickedChkRoi();                   // �Զ���ROI����
	afx_msg void OnBnClickedBtnRoi();                   // ��ROI����

	CButton m_bROI;
	UINT m_uiHOffset;                                   // ROIˮƽ����
    UINT m_uiVOffset;                                   // ROI��ֱ����
    UINT m_uiWidth;                                     // ROI���
    UINT m_uiHeight;                                    // ROI�߶�
};
