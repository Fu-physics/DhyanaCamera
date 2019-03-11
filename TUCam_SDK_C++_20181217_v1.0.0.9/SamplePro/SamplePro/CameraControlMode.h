#pragma once

// CCameraControlMode �Ի���
#define UPSPIN					-1				         // �ع�ʱ��༭���ϼ�ͷ
#define DOWNSPIN				1				         // �ع��¼��༭���¼�ͷ
	
#define STEP_VSCROLL			10		
#define STEP_MOVE				50

#include <vfw.h>
#include "../sdk/inc/TUCamApi.h"                         // ����SDKͷ�ļ�
#include "afxwin.h"
#include "afxcmn.h"
#include <dbt.h>
#include "TUButton.h"

#define  PAGE_CNT 6                                     // �ܹ�6��TabС����
#define  MAKETAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

///#include <stdio.h>

#define TUDBG_PRINTF(format,...)	{char dbgMsg[1024] = {0}; sprintf_s(dbgMsg, "" format"", ##__VA_ARGS__); OutputDebugStringA(dbgMsg);}

// the camera frameheader struct
typedef struct _tagTUCAM_FRAMEHEAD
{
    // TSCAM_Buf_WaitForFrame() use this strucTSre. Some members have different direction.
    // [i:o] means, the member is input at TSCAM_Buf_WaitForFrame()
    // [i:i] and [o:o] means always input and output at both function.
    // "input" means application has to set the value before calling.
    // "output" means function fills a value at reTSrning.

    char szSignaTSre[8];    // [out]Copyright+Version: TS+1.0 ['T', 'U', '1', '\0']

    //  The based information
    USHORT usHeader;        // [out] The frame header size
    USHORT usOffset;        // [out] The frame data offset
    USHORT usWidth;         // [out] The frame width
    USHORT usHeight;        // [out] The frame height
    UINT   uiWidthStep;     // [out] The frame width step

    UCHAR  ucDepth;         // [out] The frame data depth
    UCHAR  ucFormat;        // [out] The frame data format
    UCHAR  ucChannels;      // [out] The frame data channels
    UCHAR  ucElemBytes;     // [out] The frame data bytes per element
    UCHAR  ucFormatGet;     // [in]  Which frame data format do you want    see TSFRM_FORMATS

    UINT   uiIndex;         // [in/out] The frame index number
    UINT   uiImgSize;       // [out] The frame size
    UINT   uiRsdSize;       // [in]  The frame reserved size    (how many frames do you want)
    UINT   uiHstSize;       // [out] The frame histogram size

    //  The data
    //puchar pImgData;      // [in/out] Pointer to the image data
    UCHAR  *pBuffer;        // [in/out] The frame buffer
    UINT   *pImgHist;       // [in/out] Pointer to the image histogram data

    USHORT usLLevels;       // [out] The image left levels value
    USHORT usRLevels;       // [out] The image right levels value

    UINT   uiFixHead;       // [out] The image fix head line count
    UINT   uiFixTail;       // [out] The image fix tail line count

    //  The camera information
    USHORT usVID;           // [in/out] The camera vendor ID
    USHORT usPID;           // [in/out] The camera product ID
    USHORT usBcd;           // [in/out] The camera Bcd

    //  ROI information (Whether do roi operator)
    //  Offset X or Offset Y not equal 0
    //  Roi width or height not equal image width or height
    UINT   uiRecvSize;      // [in/out] The image receive size
    UINT   uiOrgWidth;      // [in/out] The image original width
    UINT   uiOrgHeight;     // [in/out] The image original height

    USHORT usOffsetX;       // [in/out] The image offset X
    USHORT usOffsetY;       // [in/out] The image offset Y
    USHORT usRoiWidth;      // [in/out] The image roi width
    USHORT usRoiHeight;     // [in/out] The image roi height

    USHORT usCutLeft;       // [in/out] The image cut left lines
    USHORT usCutTop;        // [in/out] The image cut top lines
    USHORT usCutRight;      // [in/out] The image cut right lines
    USHORT usCutBottom;     // [in/out] The image cut bottom lines

    UCHAR  ucSignificantBit;// [in/out] The image significant bit count

    //  The parameters
    UCHAR  ucSprtFwHeader;  // [in/out] Whether is support firmware header
    UCHAR  ucHMirror;       // [in/out] The mode of horizontal mirror
    UCHAR  ucVMirror;       // [in/out] The mode of vertical mirror
    UCHAR  ucModeMirror;    // [in/out] The mode of Mirror
    UCHAR  ucModeTrigger;   // [in/out] The mode of trigger
    UCHAR  ucATExposure;    // [in/out] The auto exposure state
    UCHAR  ucFilter;        // [in/out] The filter state
    UCHAR  ucDenoise;       // [in/out] The denoise state
    UCHAR  ucSharpness;     // [in/out] The sharpness state
    UCHAR  ucPolar;         // [in/out] The polar state
    UCHAR  ucHistc;         // [in/out] The histogram statistic
    UCHAR  ucChnSelect;     // [in/out] The channel selected
    UCHAR  ucOrderRGB;      // [in/out] The RGB order

    USHORT usLNExposure;    // [in/out] The exposure line time
    USHORT usFNExposure;    // [in/out] The exposure frame time

    double dblExposure;     // [in/out] The exposure time

    USHORT usResolution;    // [in/out] The resolution
    USHORT usPixelClock;    // [in/out] The pixel clock
    USHORT usAETarget;      // [in/out] The auto exposure target gray value
    USHORT usGlobalGain;    // [in/out] The global gain
    USHORT usBlackLevel;    // [in/out] The black level

    USHORT usGamma;         // [in/out] The image gamma value
    USHORT usContrast;      // [in/out] The image contrast value
    USHORT usSaturation;    // [in/out] The image saturation value

    USHORT usRChannel;      // [in/out] The image red channel value
    USHORT usGChannel;      // [in/out] The image green channel value
    USHORT usBChannel;      // [in/out] The image blue channel value

    //  White balance information
    UINT  nA[12];           // [out] The white balance statistical array
    UINT  nCSM[9];          // [out] The color matrix

    USHORT usRGMin;         // [out] The white balance statistical RG min
    USHORT usRGMax;         // [out] The white balance statistical RG max
    USHORT usBGMin;         // [out] The white balance statistical BG min
    USHORT usBGMax;         // [out] The white balance statistical BG max

    USHORT usDlta;          // [out] The white balance statistical delta value
    USHORT usYMin;          // [out] The white balance statistical Y min
    USHORT usYMax;          // [out] The white balance statistical Y max

    //  Statistical data
    UINT   nMean;           // [out] The average of gray value
    UINT   nRMean;          // [out] The average of red channel value
    UINT   nGMean;          // [out] The average of green channel value
    UINT   nBMean;          // [out] The average of blue channel value
    double dblStdVal;       // [out] The standard deviation value
    double dblRStdVal;      // [out] The standard deviation red channel value
    double dblGStdVal;      // [out] The standard deviation green channel value
    double dblBStdVal;      // [out] The standard deviation blue channel value

    //uchar *pBuffer;         // [in/out] The frame buffer

} TUCAM_FRAMEHEAD, *PTUCAM_FRAMEHEAD;

class CCameraControlMode : public CDialog
{
	DECLARE_DYNAMIC(CCameraControlMode)

public:
	CCameraControlMode(CWnd* pParent = NULL);           // ��׼���캯��
	virtual ~CCameraControlMode();
	CDialog *m_tabPage[PAGE_CNT];                       // �ӶԻ���ָ��

	
    void InitDrawingResource();                         // ��ʼ��������Դ
    void UnInitDrawingResource();                       // ����ʼ��������Դ

    void InitInformation();                             // ��ʼ����Ϣ
    void InitControlRange();                            // ��ʼ���ؼ���Χ
    void EnableControl(BOOL bLiving = FALSE);           // �ؼ�ʹ��    
//  void EnableTriggerControl(BOOL bEnable = FALSE);    // ��������ʹ��
    void RefreshValue();                                // ˢ�¿ؼ�ֵ
    void UpdateExposureTime(DWORD dwExp);               // �����ع�ʱ�� 
    void UpdatePicview();                               // ˢ��Ԥ������

    void StartWaitForFrame();  
    void StopWaitForFrame();  
    
// �Ի�������
	enum { IDD = IDD_DLG_CTL}; 

protected:
	//LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam); // ���ô����ƶ�
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	HACCEL hAccKey;
public:

    static void __cdecl WaitForFrameThread(LPVOID lParam); // �ȴ������߳�ʵ��
	
	CFont				m_fntEdit;						// �������ģ�����������
	CString				m_strFilePath;					// �ļ���ַ
	BOOL				m_bShowScroll;					// �Ƿ���ʾ������

	int					m_nCamCtlView_Width;			// ������������
	int					m_nCamCtlView_Heigth;			// ����������߶�
	int					m_nFactScreen_Width;			// ʵ�ʴ��ڿ��
	int					m_nFactScreen_Heigth;			// ʵ�ʴ��ڸ߶�
	int					m_nScrollWindow_Y;				// ��Ļ�������ָ������������ԭ״��

	SCROLLINFO			m_Scrollinfo;					// ��������Ϣ

    /* �����ʼ������ */
    TUCAM_INIT          m_itApi;                        // ��ʼ��SDK����
    TUCAM_OPEN          m_opCam;                        // ���������

    BOOL                m_bWaitting;                    // �����������
    BOOL                m_bLiving;                      // Ԥ�����
    BOOL                m_bSaving;                      // ͼ�񱣴�
    int                 m_nCntSave;                     // �������
    int                 m_nFmtSave;                     // �����ʽ
    int                 m_nFmtCnt;                      // ��ʽ����
    char                m_cPath[MAX_PATH];              // ����·��

    /* λͼ���Ʋ��� */
    HANDLE              m_hThdDraw;                     // ��ʾ�߳̾��
    BITMAPINFOHEADER    m_bmpInfo;                      // λͼͷ����Ϣ
    HDC				    m_hDC;                          // ����λͼDC
    HDRAWDIB		    m_hDib;                         // ����λͼDIB
    TUCAM_FRAME         m_frame;                        // ֡����
	//TUCAM_FRAMEHEAD     m_frmhead;                      // ͷ�ļ�
    TUCAM_DRAW_INIT     m_itDraw;                       // ���Ƴ�ʼ������
    TUCAM_DRAW          m_drawing;                      // ���Ʋ���

    /* ֡�ʼ��� */
    DWORD               m_dwSTm;                        // ��ʼ����
    DWORD               m_dwITm;                        // ���ʱ��
    DWORD               m_dwFrmCnt;                     // ֡��ͳ��
    float               m_fFps;                         // ֡��

    /* ����������� */
    float               m_fScale;                       // ���ű���
    int                 m_nCurWidth;                    // ��ǰ���
    int                 m_nCurHeight;                   // ��ǰ�߶�
    int                 m_nCliWidth;                    // ��ǰ�ͻ��˿��
    int                 m_nCliHeight;                   // ��ǰ�ͻ��˸߶�
    int                 m_nDrawOffX;                    // ˮƽƫ��  
    int                 m_nDrawOffY;                    // ��ֱƫ��
    int                 m_nDrawWidth;                   // ���ƿ��
    int                 m_nDrawHeight;                  // ���Ƹ߶�

    /* ���� */
    BOOL                m_bTgrMode;                     // ����ģʽ
    BOOL                m_bTrigger;                     // ����״̬
    HANDLE              m_hThdTrigger;                  // �����߳̾��

    HANDLE              m_hThdWaitForFrame;             // ���ݲ����߳̾��

    int                 m_nCnt;

	UINT                m_uiTriFrame;                   // ����֡

    /*������������ ImageName+Times+Frames*/
    UINT                m_uiCapTimes;                   // ����
    UINT                m_uiCapFrames;                  // ֡��
    
public:
	/*�������*/
	BOOL OnBnClickedBtnLive();

	/*���ROI��ť*/
	BOOL OnBtnRoiFunction();

	/*ˢ��ROI��ť*/
	BOOL OnRefreshRoiState();

	/*����Զ���ROI����*/
	BOOL OnChkRoiFunction();

	/*���¼��*/
	BOOL OnBtnRecord();

	/*���ɫ�׷�Χˢ��*/
	BOOL OnRefreshLevelRange();

	/*�������Raw��ťˢ��*/
	BOOL OnBtnRefreshRaw(BOOL bRaw = FALSE);

	/*�ı䴰�ڴ�С*/
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/*�������¼�*/
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	/*���ù������ߴ��С*/
	void SetScrollSize(int nWidth, int nHeigth);

	/*�ر�����*/
    afx_msg void OnDestroy();

	/*INFO������ť*/
	afx_msg void OnBnClickedBtnInfo();

    afx_msg void OnBnClickedBtnControlcontrol();
    afx_msg void OnBnClickedBtnImagecapture();
    afx_msg void OnBnClickedBtnMorepara();
    afx_msg void OnBnClickedBtnRoi();
    afx_msg void OnBnClickedBtnTrigger();

	/*���ܽ���ѡ��*/
	//afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    
	/*���ü��̰���*/
    virtual BOOL PreTranslateMessage(MSG* pMsg);



    void UpdateControl();

	INT                 m_nPicNum;    // ����ͼƬ����
    BOOL                m_bUseTm;
    CString             m_szPicName;  // �ļ�������Ϣ
	int                 m_nRecTm;     // ¼���ʱ

    int m_nTriMode;

	/* ¼�� */
    BOOL                m_bRecording;                   // �Ƿ�����¼��  
    
	/* ��ǩҳ */
	//CTabCtrl            m_tab;                          //���һ��CTab����ģ��

	
	/* ROI���� */
	BOOL               m_bChk;
	UINT               m_uHOffset;
    UINT               m_uVOffset;
    UINT               m_uWidth;
    UINT               m_uHeight;

	DWORD              m_dwFccHandler;                // ������������  

	BOOL               OnCaptureSnap();               // View ����
	BOOL               OnCaptureRecord();             // View ¼��
	BOOL               OnCaptureTriger();             // View ����
	BOOL               OnCapturePlay();               // Viww Ԥ��
    
	BOOL               m_bshow0;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow1;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow2;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow3;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow4;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow5;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow6;                      //��ʾ�Ƿ������ı�־λ
	BOOL               m_bshow7;                      //��ʾ�Ƿ������ı�־λ

	CRect              rectSmall;                     //����ʱ��ʾ��С��
	CRect              rectLarge;                     //��չʱ��ʾ���
	int                m_nheight;                     //�������߶�

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    CTUButton m_btnInfo;
    CTUButton m_btnCtrl;
    CTUButton m_btnImgCap;
    CTUButton m_btnROI;
    CTUButton m_btnTrigger;
    CTUButton m_btnImgAdjustment;
};
