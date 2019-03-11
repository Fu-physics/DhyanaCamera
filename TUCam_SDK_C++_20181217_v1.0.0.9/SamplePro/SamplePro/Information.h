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

	void InitInformation();                             // ��ʼ����Ϣ
	void InitControlRange();                            // ��ʼ���ؼ���Χ
    void RefreshValue();                                // ˢ�¿ؼ�ֵ

// Dialog Data
	enum { IDD = IDD_DLG_INF };                        

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL PreTranslateMessage(MSG* pMsg);        // ���ð���
	DECLARE_MESSAGE_MAP()

private:
	 TUCAM_OPEN          m_opCam;                       // ���������
	 CCameraControlMode* m_ccmode;                      // ����ģʽָ��
public:

	//CListCtrl m_lvStrings;                            // �������
};
