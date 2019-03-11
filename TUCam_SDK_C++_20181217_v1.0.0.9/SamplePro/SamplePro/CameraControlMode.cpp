// CameraControlMode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SamplePro.h"
#include "SampleProDlg.h"
#include "CameraControlMode.h"
#include "RoiSet/DlgRoiSet.h"

#include "Information.h"
#include "CameraControl.h"
#include "ImageCapture.h"
#include "ColorControl.h"
#include "MoreParameters.h"
#include "ROI.h"
#include "Trigger.h"
#include "Others.h"

#include <io.h>
#include <process.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "vfw32.lib")

#ifdef _WIN64
#pragma comment(lib, "../sdk/lib/x64/TUCam.lib")
#else
#pragma comment(lib, "../sdk/lib/x86/TUCam.lib")
#endif

#ifdef OPENCV_TEST_CENTER
#include <cv.h>
#include <highgui.h>

#pragma comment (lib, "cxcore.lib")
#pragma comment (lib, "cv.lib")
#pragma comment (lib, "ml.lib")
#pragma comment (lib, "cvaux.lib")
#pragma comment (lib, "highgui.lib")
#pragma comment (lib, "cvcam.lib")

#endif

// CCameraControlMode �Ի���
IMPLEMENT_DYNAMIC(CCameraControlMode, CDialog)

CCameraControlMode::CCameraControlMode(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraControlMode::IDD, pParent)
    , m_nPicNum(0)
    , m_bUseTm(FALSE)
    , m_szPicName(_T(""))
    , m_nRecTm(5)
	, m_uiTriFrame(0)
	, m_nTriMode(0)
	, m_fFps(0.0)
{
	m_strFilePath			= _T("");
	m_nCamCtlView_Heigth	= 0;
	m_nCamCtlView_Width		= 0;
	m_nFactScreen_Width		= 0;
	m_nFactScreen_Heigth	= 0;
	m_nScrollWindow_Y		= 0;

    //m_uiExpStep             = 1;
	//m_bshow                 = FALSE;
    m_bWaitting             = FALSE;
	m_bShowScroll			= FALSE;
    m_bLiving               = FALSE;
    m_bSaving               = FALSE;
    m_bTgrMode              = FALSE;
    m_bTrigger              = FALSE;
	m_bChk                  = FALSE;

    m_hThdDraw              = NULL;
    m_hThdTrigger           = NULL;
    m_hThdWaitForFrame      = NULL;

    m_opCam.hIdxTUCam       = NULL;
    m_opCam.uiIdxOpen       = 0;

    m_itApi.pstrConfigPath  = _T("");
    m_itApi.uiCamCount      = 0;

    m_nFmtSave              = 0;
    m_nFmtCnt               = 0;

    m_nCurWidth             = 0;
    m_nCurHeight            = 0;
    m_nCliWidth             = 0;
    m_nCliHeight            = 0;
    m_fScale                = 1.0f;
    m_nDrawOffX             = 0;
    m_nDrawOffY             = 0;
    m_nDrawWidth            = 0;
    m_nDrawHeight           = 0;

    m_uHOffset              = 0;
    m_uVOffset              = 0;
    m_uWidth                = 100;
    m_uHeight               = 100;
    m_bRecording            = FALSE;
	m_dwFccHandler          = MAKETAG('D', 'I', 'B', ' ');

	for (int i=0; i<PAGE_CNT; ++i) 
	{
		m_tabPage[i] = NULL;
	}
	m_bshow0                = FALSE;
	m_bshow1                = FALSE;
	m_bshow2                = FALSE;
	m_bshow3                = FALSE;
	m_bshow4                = FALSE;
	m_bshow5                = FALSE;
	m_bshow6                = FALSE;
	m_bshow7                = FALSE;
	m_nheight               = 220;


    m_uiCapTimes            = 0;
    m_uiCapFrames           = 0;

    m_nTriMode = TUCCM_SEQUENCE; 
}


CCameraControlMode::~CCameraControlMode()
{
	for (int i=0; i<PAGE_CNT; ++i) 
	{
		if (m_tabPage[i] != NULL) 
		{
			delete [] m_tabPage[i];
			m_tabPage[i] = NULL;
		}
	}
}

void CCameraControlMode::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_TAB, m_tab);
    DDX_Control(pDX, IDC_BTN_INFO, m_btnInfo);
    DDX_Control(pDX, IDC_BTN_ControlControl, m_btnCtrl);
    DDX_Control(pDX, IDC_BTN_ImageCapture, m_btnImgCap);
    DDX_Control(pDX, IDC_BTN_ROI, m_btnROI);
    DDX_Control(pDX, IDC_BTN_Trigger, m_btnTrigger);
    DDX_Control(pDX, IDC_BTN_MorePara, m_btnImgAdjustment);
}


BEGIN_MESSAGE_MAP(CCameraControlMode, CDialog)
	//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
    ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_INFO, &CCameraControlMode::OnBnClickedBtnInfo)
	ON_BN_CLICKED(IDC_BTN_ControlControl, &CCameraControlMode::OnBnClickedBtnControlcontrol)
	ON_BN_CLICKED(IDC_BTN_ImageCapture, &CCameraControlMode::OnBnClickedBtnImagecapture)
	ON_BN_CLICKED(IDC_BTN_ROI, &CCameraControlMode::OnBnClickedBtnRoi)
	ON_BN_CLICKED(IDC_BTN_Trigger, &CCameraControlMode::OnBnClickedBtnTrigger)
	ON_BN_CLICKED(IDC_BTN_MorePara, &CCameraControlMode::OnBnClickedBtnMorepara)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/*char szLogFile[MAX_PATH] = "Time.txt";
FILE* m_Logfp = fopen(szLogFile, "a");

void SampleProLog(int level, LPSTR str, ...)
{
    if (m_Logfp)
    {
        va_list arg_ptr;
        va_start(arg_ptr, str);
        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(m_Logfp, "[%d/%d/%d - %d:%d:%d:%d]", st.wMonth, 
                  st.wDay, st.wYear, st.wHour, st.wMinute, 
                  st.wSecond, st.wMilliseconds);
        vfprintf(m_Logfp, str, arg_ptr);
        fprintf(m_Logfp, "\n");
        fflush(m_Logfp);
    }
}*/
// �ȴ������߳�ʵ��
void __cdecl CCameraControlMode::WaitForFrameThread(LPVOID lParam)
{
    OutputDebugString(_T("Enter wait for frame thread!\n"));

    CCameraControlMode *pIn = (CCameraControlMode *)lParam;

    int nCliWidth  = 0;
    int nCliHeight = 0;

    pIn->m_nDrawOffX  = 0;
    pIn->m_nDrawOffY  = 0;
    pIn->m_nDrawWidth = 0;
    pIn->m_nDrawHeight= 0;
    pIn->m_nCurWidth  = 0;
    pIn->m_nCurHeight = 0;

    while (pIn->m_bWaitting)
    {
		//SampleProLog(1,"start");
        // ����֡��
        pIn->m_dwFrmCnt++;                              // ֡����1
        pIn->m_dwITm = GetTickCount() - pIn->m_dwSTm;   // ������ʱ�� ms
        if (pIn->m_dwITm > 1000)
        {
            pIn->m_fFps = pIn->m_dwFrmCnt * 1000000.0f / pIn->m_dwITm;

            if(NULL != pIn->m_tabPage[1])
                pIn->m_tabPage[1]->PostMessage(TUMSG_FPS, NULL, (LPARAM)pIn->m_fFps);

            pIn->m_dwSTm    = GetTickCount();
            pIn->m_dwFrmCnt = 0;
        }

        if( pIn->m_nTriMode == TUCCM_SEQUENCE )
			pIn->m_frame.ucFormatGet = TUFRM_FMT_USUAl;
		else
			pIn->m_frame.ucFormatGet = TUFRM_FMT_RAW;

        ///pIn->m_frame.ucFormatGet = TUFRM_FMT_USUAl;

        if (TUCAMRET_SUCCESS == TUCAM_Buf_WaitForFrame(pIn->m_opCam.hIdxTUCam, &pIn->m_frame))
        {
			/*TUCAM_FRAMEHEAD     frmhead;
			memcpy(&frmhead, pIn->m_frame.pBuffer, sizeof(TUCAM_FRAMEHEAD));

			UINT   m_nMean      = frmhead.nMean;
			double m_dStd       = frmhead.dblStdVal*1000;

            if(NULL != pIn->m_tabPage[1])
			{
                pIn->m_tabPage[1]->PostMessage(TUMSG_MEAN, NULL, (LPARAM)m_nMean);
				pIn->m_tabPage[1]->PostMessage(TUMSG_STD, NULL, (LPARAM)m_dStd);
			}*/

            // �ı���ʾ����
            if (pIn->m_nCurWidth != pIn->m_frame.usWidth || pIn->m_nCurHeight != pIn->m_frame.usHeight || pIn->m_nCliWidth != nCliWidth || pIn->m_nCliHeight != nCliHeight)
            {
                pIn->m_nCurWidth  = pIn->m_bmpInfo.biWidth  = pIn->m_frame.usWidth;
                pIn->m_nCurHeight = pIn->m_bmpInfo.biHeight = pIn->m_frame.usHeight;

                nCliWidth = pIn->m_nCliWidth;
                nCliHeight= pIn->m_nCliHeight;

                float fScaleX = pIn->m_nCliWidth * 1.0f / pIn-> m_nCurWidth;
                float fScaleY = pIn->m_nCliHeight * 1.0f / pIn->m_nCurHeight;

                pIn->m_fScale = fScaleX > fScaleY ? fScaleY : fScaleX;
                pIn->m_fScale = (float)((int)(pIn->m_fScale * 100) / 100.0f);

                if (pIn->m_fScale < 1)
                {
                    pIn->m_nDrawWidth = (int)(pIn->m_fScale * pIn->m_nCurWidth);
                    pIn->m_nDrawHeight= (int)(pIn->m_fScale * pIn->m_nCurHeight);
                }
                else
                {
                    pIn->m_nDrawWidth = pIn->m_nCurWidth;
                    pIn->m_nDrawHeight= pIn->m_nCurHeight;
					pIn->m_fScale = 1.0f;
                }

                pIn->m_nDrawWidth = max(4, (pIn->m_nDrawWidth >> 2) << 2);
                pIn->m_nDrawHeight= (pIn->m_nDrawHeight >> 2) << 2;

                pIn->m_nDrawOffX = (pIn->m_nCliWidth - pIn->m_nDrawWidth) / 2;
                pIn->m_nDrawOffY = (pIn->m_nCliHeight - pIn->m_nDrawHeight) / 2;
            }

            if (NULL != pIn->m_frame.pBuffer)
            {
                PUCHAR pBuf = pIn->m_frame.pBuffer + pIn->m_frame.usHeader;

                pIn->m_drawing.pFrame     = &pIn->m_frame;
 
                pIn->m_drawing.nDstX      = pIn->m_nDrawOffX;
                pIn->m_drawing.nDstY      = pIn->m_nDrawOffY;
                pIn->m_drawing.nDstWidth  = pIn->m_nDrawWidth;
                pIn->m_drawing.nDstHeight = pIn->m_nDrawHeight;

                pIn->m_drawing.nSrcX      = 0;
                pIn->m_drawing.nSrcY      = 0;
                pIn->m_drawing.nSrcWidth  = pIn->m_nCurWidth;
                pIn->m_drawing.nSrcHeight = pIn->m_nCurHeight;

				//SampleProLog(1,"before draw");
                // ����ͼ��
                TUCAM_Draw_Frame(pIn->m_opCam.hIdxTUCam, &pIn->m_drawing);
				//SampleProLog(1,"after draw");

                /*::DrawDibDraw(pIn->m_hDib, pIn->m_hDC, pIn->m_nDrawOffX, pIn->m_nDrawOffY, pIn->m_nDrawWidth, pIn->m_nDrawHeight,
                    &pIn->m_bmpInfo, pBuf, 0, 0, pIn->m_bmpInfo.biWidth, pIn->m_bmpInfo.biHeight, 0);*/

                // ¼��
                if (pIn->m_bRecording)
                {
                    TUCAM_Rec_AppendFrame(pIn->m_opCam.hIdxTUCam, &pIn->m_frame);
                }

                // ����ͼƬ
                if (pIn->m_bSaving || pIn->m_bTrigger)
                {
                    // �ļ����Ƿ���ڣ��������򴴽�
                    if (!PathIsDirectory(pIn->m_strFilePath))
                    {
                        CreateDirectory(pIn->m_strFilePath, NULL);
                    }
                    
					 // ��ȡϵͳʱ��
                    SYSTEMTIME sysTm;
                    GetLocalTime(&sysTm);

                    CString szFName;
                    szFName.Format(_T("%s\\%s_%d_%d_%02d-%02d-%03d"), pIn->m_strFilePath, pIn->m_szPicName, pIn->m_uiCapTimes, pIn->m_uiCapFrames++,sysTm.wMinute, sysTm.wSecond, sysTm.wMilliseconds);

                    BOOL bRet = TRUE;

                    do 
                    {
                        int nFmt = pIn->m_nFmtSave;
                        TUCAM_FRAME frame;
                        TUCAM_FILE_SAVE fileSave;

                        fileSave.nSaveFmt = nFmt;
                        memcpy(&frame, &pIn->m_frame, sizeof(TUCAM_FRAME));
                        fileSave.pFrame   = &frame; 
                        fileSave.pstrSavePath = (PCHAR)szFName.GetBuffer(0);  // path

                        // Format RAW
                        if (nFmt & TUFMT_RAW)
                        {
                            nFmt &= ~TUFMT_RAW;
                        }

                        if (0 != nFmt)
                        {
                            fileSave.nSaveFmt = nFmt;

                            // Save other format data(TIFF/BMP/PNG/JPEG)
                            if (TUCAMRET_SUCCESS != TUCAM_File_SaveImage(pIn->m_opCam.hIdxTUCam, fileSave))
                            {
                                bRet = FALSE;
                                break;
                            }
                        }                        

                        if (pIn->m_nFmtSave & TUFMT_RAW)
                        {
                            fileSave.nSaveFmt = TUFMT_RAW;

                            // Get RAW data
                            pIn->m_frame.ucFormatGet = TUFRM_FMT_RAW;
                            if (TUCAMRET_SUCCESS != TUCAM_Buf_CopyFrame(pIn->m_opCam.hIdxTUCam, &pIn->m_frame))
                            {
                                bRet = FALSE;
                                break;
                            }

                            // Save RAW data
                            if (TUCAMRET_SUCCESS != TUCAM_File_SaveImage(pIn->m_opCam.hIdxTUCam, fileSave))
                            {
                                bRet = FALSE;
                                break;
                            }
                        }

                    } while (0);

                    if (bRet)
                    {
                        pIn->m_nCntSave++;
                        // pIn->PostMessage(TUMSG_CAP, NULL, (LPARAM)pIn->m_nCntSave);
                        if(pIn->m_tabPage[2] !=NULL && 1 == pIn->m_nCntSave && 1 == pIn->m_nPicNum)
                            pIn->m_tabPage[2]->PostMessage(TUMSG_CAP, NULL, (LPARAM)pIn->m_nCntSave);

                        // ���ս���
                        if (pIn->m_nCntSave >= pIn->m_nPicNum)
                        {
                            pIn->m_bSaving = FALSE;         
                        }
                    }

                }
            }

        }

    }


    SetEvent(pIn->m_hThdWaitForFrame);

    OutputDebugString(_T("Leave wait for frame thread!\n"));

    _endthread();
}

void CCameraControlMode::InitDrawingResource()
{
    int nSize = sizeof(BITMAPINFOHEADER);

    memset(&m_bmpInfo, 0, nSize);
    m_bmpInfo.biSize	    = nSize;
    m_bmpInfo.biPlanes	    = 1;
    m_bmpInfo.biBitCount    = 24;
    m_bmpInfo.biCompression = BI_RGB;

    HWND hDraw = ((CSampleProDlg *)(this->GetParent()))->m_PicViewMode.GetDlgItem(IDC_STA_PIC)->GetSafeHwnd();

    m_hDC = ::GetDC(hDraw);
    m_hDib= ::DrawDibOpen();
}

void CCameraControlMode::UnInitDrawingResource()
{
    if (NULL != m_hDib)
    {
        ::DrawDibClose(m_hDib);
        m_hDib = NULL;
    }

    if (NULL != m_hDC)
    {
        HWND hDraw = ((CSampleProDlg *)(this->GetParent()))->m_PicViewMode.GetDlgItem(IDC_STA_PIC)->GetSafeHwnd();
        ::ReleaseDC(hDraw, m_hDC);
        m_hDC = NULL;
    }
}


void CCameraControlMode::InitInformation()
{
	if (m_tabPage[0] != NULL) ((Information*)m_tabPage[0])->InitInformation();
}

void CCameraControlMode::InitControlRange()
{
	if (m_tabPage[0] != NULL) ((Information*)m_tabPage[0])->InitControlRange();
	if (m_tabPage[1] != NULL) ((CameraControl*)m_tabPage[1])->InitControlRange();
	if (m_tabPage[2] != NULL) ((ImageCapture*)m_tabPage[2])->InitControlRange();
	if (m_tabPage[3] != NULL) ((ROI*)m_tabPage[3])->InitControlRange();
	if (m_tabPage[4] != NULL) ((Trigger*)m_tabPage[4])->InitControlRange();
	if (m_tabPage[5] != NULL) ((MoreParameters*)m_tabPage[5])->InitControlRange();
}

void CCameraControlMode::RefreshValue()
{
	if (m_tabPage[0] != NULL) ((Information*)m_tabPage[0])->RefreshValue();
	if (m_tabPage[1] != NULL) ((CameraControl*)m_tabPage[1])->RefreshValue();
	if (m_tabPage[2] != NULL) ((ImageCapture*)m_tabPage[2])->RefreshValue();
	if (m_tabPage[3] != NULL) ((ROI*)m_tabPage[3])->RefreshValue();
	if (m_tabPage[4] != NULL) ((Trigger*)m_tabPage[4])->RefreshValue();
	if (m_tabPage[5] != NULL) ((MoreParameters*)m_tabPage[5])->RefreshValue();
}

void CCameraControlMode::EnableControl(BOOL bLiving /*= FALSE*/)
{
   
}

void CCameraControlMode::UpdatePicview()
{
	((CSampleProDlg *)this->GetParent())->m_PicViewMode.GetDlgItem(IDC_STA_PIC)->Invalidate();
}

void CCameraControlMode::StartWaitForFrame()
{
    if (m_bWaitting)
    {
        return;
    }

    m_bWaitting = TRUE;

    if (NULL == m_hThdWaitForFrame)
    {
        m_frame.pBuffer     = NULL;

		if( m_nTriMode == TUCCM_SEQUENCE )
			m_frame.ucFormatGet = TUFRM_FMT_USUAl;
		else
			m_frame.ucFormatGet = TUFRM_FMT_RAW;

        m_frame.uiRsdSize   = 1;                            // how many frames do you want

        TUCAM_Buf_Alloc(m_opCam.hIdxTUCam, &m_frame);       // Alloc buffer after set resolution or set ROI attribute
        TUCAM_Cap_Start(m_opCam.hIdxTUCam, m_nTriMode);     // Start capture

        m_dwFrmCnt = 0;                                     // reset frame count
        m_dwSTm    = GetTickCount();                        // reset start tick count

        UpdatePicview();

        m_hThdWaitForFrame = CreateEvent(NULL, TRUE, FALSE, NULL);
        _beginthread(WaitForFrameThread, 0, this);          // Start capturing thread

        if (TUCCM_SEQUENCE != m_nTriMode)
        {
            if(m_tabPage[1] !=NULL)
            {
                ((CameraControl*)m_tabPage[1])->RefreshBtn(FALSE);
                ((CameraControl*)m_tabPage[1])->BtnShow(TRUE);
                ((CameraControl*)m_tabPage[1])->GetDlgItem(IDC_BTN_ONCEAE)->EnableWindow(FALSE);
            }

            if (NULL != m_tabPage[2])          
            {
                ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_STA_TOTAL)->EnableWindow(FALSE);
                ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_EDT_PICNUM)->EnableWindow(FALSE);
                ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_SPN_PICNUM)->EnableWindow(FALSE);
            }

            if(m_tabPage[3] !=NULL)
                ((ROI*)m_tabPage[3])->GetDlgItem(IDC_CHK_ROI)->EnableWindow(FALSE);

        }
        else
        {
            if(NULL != m_tabPage[1])
            {
                ((CameraControl*)m_tabPage[1])->BtnShow(m_bWaitting);
                ((CameraControl*)m_tabPage[1])->GetDlgItem(IDC_BTN_ONCEAE)->EnableWindow(TRUE);
            }

            if (NULL != m_tabPage[2])    
            {
                ((ImageCapture*)m_tabPage[2])->RefreshBtn(TRUE);
                ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_STA_TOTAL)->EnableWindow(TRUE);
                ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_EDT_PICNUM)->EnableWindow(TRUE);
                ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_SPN_PICNUM)->EnableWindow(TRUE);
            }

            if(m_tabPage[3] !=NULL)
                ((ROI*)m_tabPage[3])->GetDlgItem(IDC_CHK_ROI)->EnableWindow(TRUE);

            if(NULL != m_tabPage[4])
                ((Trigger*)m_tabPage[4])->EnableTriggerControl(FALSE);
        }
    }
}

void CCameraControlMode::StopWaitForFrame()
{
    if (!m_bWaitting)
    {
        return;
    }

    m_bSaving   = FALSE;
    m_bWaitting = FALSE;

    if (NULL != m_hThdWaitForFrame)
    {            
        TUCAM_Buf_AbortWait(m_opCam.hIdxTUCam);             // If you called TUCAM_Buf_WaitForFrames()

        WaitForSingleObject(m_hThdWaitForFrame, INFINITE);
        CloseHandle(m_hThdWaitForFrame);	
        m_hThdWaitForFrame = NULL;

        TUCAM_Cap_Stop(m_opCam.hIdxTUCam);                  // Stop capture   
        TUCAM_Buf_Release(m_opCam.hIdxTUCam);               // Release alloc buffer after stop capture and quit drawing thread

        if (TUCCM_SEQUENCE != m_nTriMode)
        {
            if(NULL != m_tabPage[1])
            {
                ((CameraControl*)m_tabPage[1])->RefreshBtn(FALSE);
                ((CameraControl*)m_tabPage[1])->BtnShow(FALSE);
            }
        }
        else
        {
            if(NULL != m_tabPage[1])
            {
                ((CameraControl*)m_tabPage[1])->RefreshBtn(TRUE);
                ((CameraControl*)m_tabPage[1])->BtnShow(m_bWaitting);
            }
        }

        if(NULL != m_tabPage[4])
            ((Trigger*)m_tabPage[4])->EnableTriggerControl(TRUE);

        if (NULL != m_tabPage[2])
        {
            Sleep(100);
            ((ImageCapture*)m_tabPage[2])->RefreshBtn(FALSE);
            ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_STA_CAPTURE)->SetWindowText("");
            ((ImageCapture*)m_tabPage[2])->GetDlgItem(IDC_BTN_CAPTURE)->SetWindowText("Capture");
        }
    }
}


BOOL CCameraControlMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Create a resize gripper
	CRect initRect;						

	GetClientRect(initRect);
	initRect.left = initRect.right - GetSystemMetrics(SM_CXHSCROLL);
	initRect.top  = initRect.bottom - GetSystemMetrics(SM_CYVSCROLL);

	DWORD dwStyle = WS_CHILD | 
		SBS_SIZEBOX | 
		SBS_SIZEBOXBOTTOMRIGHTALIGN | 
		SBS_SIZEGRIP | 
		WS_VISIBLE;

	// ��ʼ������
	//m_fntEdit.CreatePointFont(130, _T("Arial"));
	
    
/*
    // ��ʼ��������Դ
    InitDrawingResource();
*/
    // ��ȡ��ǰ��ַ
	CString strPath;
	GetCurrentDirectory(MAX_PATH, strPath.GetBuffer(MAX_PATH));

    m_itApi.pstrConfigPath =  (LPSTR)(LPCTSTR)strPath;
    m_itApi.uiCamCount = 0;

    // ��ʼ��SDK API����
    TUCAM_Api_Init(&m_itApi);

    if (0 == m_itApi.uiCamCount)
    {
        AfxMessageBox(_T("There is no camera!"));
        return TRUE;   
    }

    
	m_opCam.uiIdxOpen = 0;
    TUCAM_Dev_Open(&m_opCam);

    if (NULL == m_opCam.hIdxTUCam)
    {   
        AfxMessageBox(_T("Open camera failed!"));
        return TRUE;  
    }

    // ��ȡ���ͨ����
    TUCAM_VALUE_INFO valInfo;
    valInfo.nValue = 1;
    valInfo.nID = TUIDI_CAMERA_CHANNELS;
    TUCAM_Dev_GetInfo(m_opCam.hIdxTUCam, &valInfo);

    m_itDraw.hWnd       = ((CSampleProDlg *)(this->GetParent()))->m_PicViewMode.GetDlgItem(IDC_STA_PIC)->GetSafeHwnd();
    m_itDraw.ucChannels = valInfo.nValue;
    m_itDraw.nWidth     = 2048;
    m_itDraw.nHeight    = 2048;
    TUCAM_Draw_Init(m_opCam.hIdxTUCam, m_itDraw);

    // ͼƬ�ļ�����·��
    strcat(strPath.GetBuffer(MAX_PATH), "\\img");
    m_strFilePath = strPath;

	hAccKey = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	// TAB���ƽ���
	CString str;
	for (int i=0; i<PAGE_CNT; ++i) 
	{
		switch (i) 
		{
			case 0: if (m_tabPage[i] == NULL) (m_tabPage[i] = new Information(m_opCam, this));      break;
			case 1: if (m_tabPage[i] == NULL) (m_tabPage[i] = new CameraControl(m_opCam, this));    break;
		    case 2: if (m_tabPage[i] == NULL) (m_tabPage[i] = new ImageCapture(m_opCam, this));     break;
			case 3: if (m_tabPage[i] == NULL) (m_tabPage[i] = new ROI(m_opCam, this));              break;
			case 4: if (m_tabPage[i] == NULL) (m_tabPage[i] = new Trigger(m_opCam, this));          break;
			case 5: if (m_tabPage[i] == NULL) (m_tabPage[i] = new MoreParameters(m_opCam, this));   break;
		}
		m_tabPage[i]->Create(IDD_DLG_INF+i, this);   
		m_tabPage[i]->ShowWindow(/*i==0 ? SW_SHOW :*/ SW_HIDE);
	}

	// m_tab.SetCurSel(0);

    OnBnClickedBtnControlcontrol();

	// ���½���
	UpdateData(FALSE);
	
	return TRUE;
}

void CCameraControlMode::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
 
	//��ǰ��Ļ�߶� < �����������߶ȣ������ù�����
	if (m_nFactScreen_Width < 0 || m_nFactScreen_Heigth < 0 )
	{
		;
	}
	else
	{
		SetScrollSize(m_nFactScreen_Width, m_nCamCtlView_Heigth);
	}
}

void CCameraControlMode::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	int nMaxPos = 0;

	GetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
	nMaxPos = m_Scrollinfo.nMax-m_Scrollinfo.nPage+1;

	switch (nSBCode)
	{
		case SB_BOTTOM:																// �����������ײ�
		{
			ScrollWindow(0, (m_Scrollinfo.nPos-nMaxPos)*STEP_VSCROLL);				// ������Ļ
			m_Scrollinfo.nPos = nMaxPos;
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);							// ���»���λ��	 
		}
		break;

		case SB_TOP:																// ������������
		{
			ScrollWindow(0, (m_Scrollinfo.nPos-m_Scrollinfo.nMin)*STEP_VSCROLL);	// ������Ļ
			m_Scrollinfo.nPos = m_Scrollinfo.nMin;
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
		}
		break;

		case SB_LINEUP:																// �����ϼ�ͷ
		{
			m_Scrollinfo.nPos -= 1;
			if (m_Scrollinfo.nPos < m_Scrollinfo.nMin)
			{
				m_Scrollinfo.nPos = m_Scrollinfo.nMin;
				break;
			}
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
			ScrollWindow(0, STEP_VSCROLL);
		}
		break;

		case SB_LINEDOWN:															// �����¼�ͷ
		{
			m_Scrollinfo.nPos += 1;
			if (m_Scrollinfo.nPos > nMaxPos)
			{
				m_Scrollinfo.nPos = nMaxPos;
				break;
			}
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
			ScrollWindow(0, -STEP_VSCROLL);
		}
		break;

		case SB_PAGEUP:																// ���������Ϸ��հ�����
		{
			int cx = 5;
			cx = min(cx, (m_Scrollinfo.nPos-m_Scrollinfo.nMin));
			m_Scrollinfo.nPos -= cx;
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
			ScrollWindow(0, cx*STEP_VSCROLL);
		}
		break;

		case SB_PAGEDOWN:															// ���������·��հ�����
		{
			int cx = 5;
			cx = min(cx, (nMaxPos-m_Scrollinfo.nPos));
			m_Scrollinfo.nPos += cx;
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
			ScrollWindow(0, -cx*STEP_VSCROLL);
		}
		break;

		case SB_ENDSCROLL:															// ����뿪���飬���������϶�
		break;
		case SB_THUMBPOSITION: 
		break;

		case SB_THUMBTRACK:															// �϶�����
		{
			ScrollWindow(0, (m_Scrollinfo.nPos-nPos)*STEP_VSCROLL);
			m_Scrollinfo.nPos = nPos;
			SetScrollInfo(SB_VERT, &m_Scrollinfo, SIF_ALL);
		}
		break;		
	}

	m_nScrollWindow_Y = m_Scrollinfo.nPos*STEP_VSCROLL;

	Invalidate(FALSE);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

//���ù�����������
void CCameraControlMode::SetScrollSize(int nWidth, int nHeigth)
{
	CRect rt;
	this->GetClientRect(&rt);

	int i = rt.Width();
	int j = rt.Height();

	//���ô�ֱ������
	m_Scrollinfo.cbSize	 = sizeof(m_Scrollinfo);
	m_Scrollinfo.fMask	 = SIF_ALL;

	//������������ͨ�������¹�ϵ���䳤��/���������ȣ���������ͷ��= nPage/(nMax+2),����nPageȡֵ-1ʱ���������᲻����
	m_Scrollinfo.nPage	   = rt.Height() / STEP_VSCROLL;
	m_Scrollinfo.nMax	   = m_nheight / STEP_VSCROLL - 1;   // m_nCamCtlView_Heigth
	m_Scrollinfo.nMin	   = 0;
	m_Scrollinfo.nTrackPos = 0;
	m_Scrollinfo.nPos	   = 0;                              //0

	//���ù�������Ϣ
	SetScrollInfo(SB_VERT, &m_Scrollinfo);

	//���������ֵ�����
	if ((int)m_Scrollinfo.nPage <= m_Scrollinfo.nMax)
	{
		m_bShowScroll = FALSE;
	}
	else
	{
		m_bShowScroll = TRUE;
	}

	//������Ļ���ָ�ԭ״
	ScrollWindow(0, m_nScrollWindow_Y);
	m_nScrollWindow_Y = 0;

	Invalidate();
}

void CCameraControlMode::OnDestroy()
{
    CDialog::OnDestroy();

    if (m_bWaitting)
    {
        StopWaitForFrame();
    }

//     if (m_bLiving || m_bTrigger)
//     {
//         OnBnClickedBtnLive();
//     }

    // TODO: �ڴ˴������Ϣ����������
/*
    UnInitDrawingResource();                // �ͷŻ�����Դ
*/
    TUCAM_Dev_Close(m_opCam.hIdxTUCam);     // �ر����
    TUCAM_Draw_Uninit(m_opCam.hIdxTUCam);   // �ͷŻ�ͼ��Դ
    TUCAM_Api_Uninit();                     // �ͷ�SDK ��Դ����
}


BOOL CCameraControlMode::OnBnClickedBtnLive()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    if (m_bWaitting)
    {
        StopWaitForFrame();
    }
    else
    {
        StartWaitForFrame();

        if(m_tabPage[2] !=NULL)
            ((ImageCapture*)m_tabPage[2])->RefreshBtn(m_bWaitting);

        if(m_tabPage[5] !=NULL)
            ((MoreParameters*)m_tabPage[5])->RefreshBtn();

        if(m_tabPage[4] !=NULL)
            ((Trigger*)m_tabPage[4])->EnableTriggerControl(!m_bWaitting);
    }

//     if (!m_bTgrMode)
//     {
//         OnLivingSequence();
// 		if(m_tabPage[3] !=NULL)
// 			((ImageCapture*)m_tabPage[2])->RefreshBtn(m_bLiving);
// 		if(m_tabPage[5] !=NULL)
// 			((MoreParameters*)m_tabPage[5])->RefreshBtn();
// 		if(m_tabPage[4] !=NULL)
// 			((Trigger*)m_tabPage[4])->EnableTriggerControl(!m_bLiving);
//     }
//     else
//     {
//         OnLivingTrigger();
//     }
	return true;
}

BOOL CCameraControlMode::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if((pMsg->wParam == VK_ESCAPE))
    {  
        return TRUE;  
    }

    if ((pMsg->wParam == VK_RETURN))
    {
        UpdateData();
        return TRUE;
	}

	//if(TranslateAccelerator(m_hWnd,hAccKey,pMsg))  
	//{    
		//return TRUE;    
	//}  

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCameraControlMode::OnBtnRoiFunction()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    BOOL bWaitting = m_bWaitting;

    if (bWaitting)
    {
        StopWaitForFrame();
    }


    TUCAM_ROI_ATTR roiAttr;

    if (m_bChk)
    {
        roiAttr.bEnable = FALSE;
        roiAttr.nVOffset= 0;
        roiAttr.nHOffset= 0;
        roiAttr.nWidth  = m_nDrawWidth;
        roiAttr.nHeight = m_nDrawHeight;

        m_bChk          =FALSE;
        TUCAM_Cap_SetROI(m_opCam.hIdxTUCam, roiAttr);
    }
    else
    {
        roiAttr.bEnable = TRUE;
        roiAttr.nVOffset= ((m_uVOffset >> 2) << 2);
        roiAttr.nHOffset= ((m_uHOffset >> 2) << 2);
        roiAttr.nWidth  = ((m_uWidth >> 3) << 3);
        roiAttr.nHeight = ((m_uHeight >> 2) << 2);
        
		// ǿ��������32*8 | 8*32
		/*if( roiAttr.nHeight < 16 && roiAttr.nWidth < 32) 
			roiAttr.nWidth  = 32;

		if( roiAttr.nHeight < 32 && roiAttr.nWidth < 16) 
			roiAttr.nHeight = 32;*/
		if(roiAttr.nWidth < 32 ) 
			roiAttr.nWidth  = 32;

        m_bChk         =TRUE;

        TUCAM_Cap_SetROI(m_opCam.hIdxTUCam, roiAttr);

        TUCAM_Cap_GetROI(m_opCam.hIdxTUCam, &roiAttr);

        m_uVOffset = roiAttr.nVOffset;
        m_uHOffset = roiAttr.nHOffset;
        m_uWidth   = roiAttr.nWidth;
        m_uHeight  = roiAttr.nHeight;
    }
    

    if (bWaitting)
    {
        StartWaitForFrame();   // ��ʼȡͼ

		// �����ع�����������
		Sleep(2);
		double dblExp=0;
		TUCAM_Prop_GetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dblExp);
		TUCAM_Prop_SetValue(m_opCam.hIdxTUCam, TUIDP_EXPOSURETM, dblExp);
    }

    UpdatePicview();

	return true;
}

BOOL CCameraControlMode::OnChkRoiFunction()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_bChk)
    {
		CRect rc;
		((CSampleProDlg *)this->GetParent())->m_PicViewMode.GetDlgItem(IDC_STA_PIC)->GetWindowRect(&rc);

		rc.left += m_nDrawOffX;
		rc.top  += m_nDrawOffY;
		rc.right = rc.left + m_nDrawWidth;
		rc.bottom= rc.top + m_nDrawHeight;

        CDlgRoiSet dlgRoiSet;
        dlgRoiSet.SetPreviewRect(rc);
        dlgRoiSet.SetScale(1.0f / m_fScale);

        if (dlgRoiSet.DoModal() == IDOK)
        {
            // ��ȡ��ѡ��ߴ缰λ��
            CRect rect;
            dlgRoiSet.GetPreviewRect(rect);
            int nVOff = (int)((rect.top - rc.top) / m_fScale);
            int nHOff = (int)((rect.left - rc.left) / m_fScale);
            int nWid  = (int)(rect.Width() / m_fScale);
            int nHei  = (int)(rect.Height() / m_fScale);

            nVOff = ((nVOff >> 2) << 2);
            nHOff = ((nHOff >> 2) << 2);
            nWid  = ((nWid >> 3) << 3);
            nHei  = ((nHei >> 3) << 3);

            BOOL bWaitting = m_bWaitting;

            if (m_bWaitting)
            {
                StopWaitForFrame();   // ֹͣȡͼ
            }

            TUCAM_ROI_ATTR roiAttr;
            roiAttr.bEnable = TRUE;
            roiAttr.nVOffset= m_uVOffset = ((nVOff >> 2) << 2);
            roiAttr.nHOffset= m_uHOffset = ((nHOff >> 2) << 2);
            roiAttr.nWidth  = m_uWidth   = ((nWid >> 3) << 3);
            roiAttr.nHeight = m_uHeight  = ((nHei >> 3) << 3);

            TUCAM_Cap_SetROI(m_opCam.hIdxTUCam, roiAttr);

            if (bWaitting)
            {
                StartWaitForFrame();   // ��ʼȡͼ
            }
        }
        else
        {
            m_bChk = FALSE;
        }

        UpdatePicview();
    }
    else
    {

        BOOL bWaitting = m_bWaitting;

        if (m_bWaitting)
        {
            StopWaitForFrame();   // ֹͣȡͼ
        }

        TUCAM_ROI_ATTR roiAttr;
        roiAttr.bEnable = FALSE;
        roiAttr.nVOffset= 0;
        roiAttr.nHOffset= 0;
        roiAttr.nWidth  = 0;
        roiAttr.nHeight = 0;

        TUCAM_Cap_SetROI(m_opCam.hIdxTUCam, roiAttr);

        if (bWaitting)
        {
            StartWaitForFrame();   // ��ʼȡͼ
        }

        UpdatePicview();
    }

	return true;
}

BOOL  CCameraControlMode::OnRefreshRoiState()
{
	if (m_tabPage[3] != NULL) ((ROI*)m_tabPage[3])->RefreshState();
	return true;
}

BOOL CCameraControlMode::OnBtnRecord()
{
	if (m_bRecording)
	{	
		m_bRecording = FALSE;
		TUCAM_Rec_Stop(m_opCam.hIdxTUCam);
	}
	else  
	{
		int nPos = 0;
		int nCnt = 1;
		CString szFName;
		CString szTmp;

		// �ļ����Ƿ���ڣ��������򴴽�
		if (!PathIsDirectory(m_strFilePath))
		{
			CreateDirectory(m_strFilePath, NULL);
		}

		szTmp.Format(_T("%s\\%s"), m_strFilePath, m_szPicName);
		szFName = szTmp + _T(".avi");

		do{
			if (-1 == _access(szFName, 0))
			{
				break;
			}

			nPos = szTmp.ReverseFind('_');

			if (-1 != nPos && szTmp.GetLength() - nPos < 4)
			{
				szFName = szTmp.Left(nPos);
				szTmp.Format(_T("%s_%d"), szFName, nCnt++);
				szFName = szTmp + _T(".avi");
			}
			else
			{
				szFName = szTmp;
				szTmp.Format(_T("%s_%d"), szFName, nCnt++);
				szFName = szTmp + _T(".avi");
			}            
		} while(1);

		// If File exists, delete it 
		CFileFind fileFind;

		if (fileFind.FindFile(szFName))
		{
			CFile::Remove(szFName);
		}

		TUCAM_REC_SAVE recSave;
		recSave.fFps    = m_fFps / 1000;
		recSave.nCodec  = m_dwFccHandler;
		recSave.pstrSavePath = szFName.GetBuffer();

		if (TUCAMRET_SUCCESS == TUCAM_Rec_Start(m_opCam.hIdxTUCam, recSave))
		{
			m_bRecording = TRUE;
			SetDlgItemText(IDC_BTN_REC, _T("Stop Record"));                   
		}
	}
	return true;
}

BOOL CCameraControlMode::OnRefreshLevelRange()
{
	if(m_tabPage[5] !=NULL)
		((MoreParameters*)m_tabPage[5])->RefreshLevelRange();
	return true;
}

BOOL CCameraControlMode::OnBtnRefreshRaw(BOOL bRaw)
{
	if(m_tabPage[2] !=NULL)
		((ImageCapture*)m_tabPage[2])->RefreshBtnRaw(bRaw);
	return true;
}

BOOL CCameraControlMode::OnCaptureSnap()
{
	if(m_tabPage[2] !=NULL)
	{
		((ImageCapture*)m_tabPage[2])->OnBnClickedBtnCapture();
	}
	return true;
}

BOOL CCameraControlMode::OnCaptureRecord()
{
	if(m_tabPage[2] !=NULL)
	{
		((ImageCapture*)m_tabPage[2])->OnBnClickedBtnRec();
	}
	return true;
}


BOOL CCameraControlMode::OnCaptureTriger()
{
	if(m_bLiving)
	{
		if(m_tabPage[3] !=NULL)
			((CameraControl*)m_tabPage[1])->OnBnClickedBtnLive();
	}
	/*if(m_tabPage[6] !=NULL)
	{
		int m_tabsel = m_tab.GetCurSel();
		if(m_tabsel !=6 )
		{
			m_tabPage[m_tabsel]->ShowWindow(SW_HIDE);
			m_tab.SetCurSel(6);
			m_tabPage[6]->ShowWindow(SW_SHOW);
			//CString str;
			//str.LoadString(IDS_DLG_CAMCON+5);
			//this->SetWindowText(str);
			// ���½���
			UpdateData(FALSE);
		}
	}*/
    return true;
}

BOOL CCameraControlMode::OnCapturePlay()
{
	if(m_tabPage[1] !=NULL)
	{
		((CameraControl*)m_tabPage[1])->OnBnClickedBtnLive();
	}
    return true;
}
// info ������ť
void CCameraControlMode::OnBnClickedBtnInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    if (NULL == m_tabPage[0])
    {
        return;
    }
    
	m_bshow0 = !m_bshow0;
	CButton *btn = (CButton *)GetDlgItem(IDC_BTN_INFO);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow0)
	{      
		m_tabPage[0]->GetClientRect(&rectLarge);
		m_tabPage[0]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[0]->ShowWindow(SW_SHOW);
//      this->SetDlgItemText(IDC_BTN_INFO, "Camera Information                                     ��");
        m_btnInfo.SetText("Camera Information                                     ��");
	}
	else
	{
		m_tabPage[0]->ShowWindow(SW_HIDE);
//      this->SetDlgItemText(IDC_BTN_INFO, "Camera Information                                     >>");
        m_btnInfo.SetText("Camera Information                                     >>");
	}

	UpdateControl();
}


void CCameraControlMode::OnBnClickedBtnControlcontrol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    if (NULL == m_tabPage[1])
    {
        return;
    }

	m_bshow1 = !m_bshow1;
	CButton *btn = (CButton *)GetDlgItem(IDC_BTN_ControlControl);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow1)
	{      
		m_tabPage[1]->GetClientRect(&rectLarge);
		m_tabPage[1]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[1]->ShowWindow(SW_SHOW);
//		this->SetDlgItemText(IDC_BTN_ControlControl, "Camera Control                                           ��");
        m_btnCtrl.SetText("Camera Control                                           ��");
	}
	else
	{
		m_tabPage[1]->ShowWindow(SW_HIDE);
//      this->SetDlgItemText(IDC_BTN_ControlControl, "Camera Control                                           >>");
        m_btnCtrl.SetText("Camera Control                                           >>");
	}

	UpdateControl();
}

void CCameraControlMode::OnBnClickedBtnImagecapture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    if (NULL == m_tabPage[2])
    {
        return;
    }

	m_bshow2 = !m_bshow2;
	CButton *btn = (CButton *)GetDlgItem(IDC_BTN_ImageCapture);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow2)
	{      
		m_tabPage[2]->GetClientRect(&rectLarge);
		m_tabPage[2]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[2]->ShowWindow(SW_SHOW);
//		this->SetDlgItemText(IDC_BTN_ImageCapture, "Image Capture                                            ��");
        m_btnImgCap.SetText("Image Capture                                            ��");
	} 
	else
	{
		m_tabPage[2]->ShowWindow(SW_HIDE); 
//      this->SetDlgItemText(IDC_BTN_ImageCapture, "Image Capture                                            >>");
        m_btnImgCap.SetText("Image Capture                                            >>");
	} 

	UpdateControl();
}

void CCameraControlMode::OnBnClickedBtnRoi()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (NULL == m_tabPage[3])
    {
        return;
    }

    m_bshow3 = !m_bshow3;
    CButton *btn = (CButton *)GetDlgItem(IDC_BTN_ROI);
    btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
    if(m_bshow3)
    {      
        m_tabPage[3]->GetClientRect(&rectLarge);
        m_tabPage[3]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
        m_tabPage[3]->ShowWindow(SW_SHOW);
//      this->SetDlgItemText(IDC_BTN_ROI, "ROI                                                           ��");
        m_btnROI.SetText("ROI                                                           ��");
    } 
    else
    {
        m_tabPage[3]->ShowWindow(SW_HIDE); 
//      this->SetDlgItemText(IDC_BTN_ROI, "ROI                                                           >>");
        m_btnROI.SetText("ROI                                                           >>");
    }

    UpdateControl();
}

void CCameraControlMode::OnBnClickedBtnTrigger()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (NULL == m_tabPage[4])
    {
        return;
    }

    m_bshow4 = !m_bshow4;
    CButton *btn = (CButton *)GetDlgItem(IDC_BTN_Trigger);
    btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
    if(m_bshow4)
    {      
        m_tabPage[4]->GetClientRect(&rectLarge);
        m_tabPage[4]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
        m_tabPage[4]->ShowWindow(SW_SHOW);
//      this->SetDlgItemText(IDC_BTN_Trigger, "Trigger                                                     ��");
        m_btnTrigger.SetText("Trigger                                                      ��");
    }
    else
    {
        m_tabPage[4]->ShowWindow(SW_HIDE);
//      this->SetDlgItemText(IDC_BTN_Trigger, "Trigger                                                      >>");
        m_btnTrigger.SetText("Trigger                                                      >>");
    }
    UpdateControl();
}

void CCameraControlMode::OnBnClickedBtnMorepara()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    if (NULL == m_tabPage[5])
    {
        return;
    }

	m_bshow5 = !m_bshow5;
	CButton *btn = (CButton *)GetDlgItem(IDC_BTN_MorePara);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow5)
	{      
		m_tabPage[5]->GetClientRect(&rectLarge);
		m_tabPage[5]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[5]->ShowWindow(SW_SHOW);
//		this->SetDlgItemText(IDC_BTN_MorePara, "Image Adjustment                                       ��");
        m_btnImgAdjustment.SetText("Image Adjustment                                       ��");
	}
	else
	{
		m_tabPage[5]->ShowWindow(SW_HIDE);
//      this->SetDlgItemText(IDC_BTN_MorePara, "Image Adjustment                                       >>");
        m_btnImgAdjustment.SetText("Image Adjustment                                       >>");
	}
	UpdateControl();
}


void CCameraControlMode::UpdateControl()
{
    CButton *btn = (CButton *)GetDlgItem(IDC_BTN_INFO);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow0)
	{   
		m_tabPage[0]->ShowWindow(SW_HIDE);
		m_tabPage[0]->GetClientRect(&rectLarge);
		m_tabPage[0]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[0]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ControlControl)->MoveWindow(rectSmall.left,rectSmall.bottom+rectLarge.Height()+5, rectSmall.Width(), rectSmall.Height());
	}
	else
	{
		GetDlgItem(IDC_BTN_ControlControl)->MoveWindow(rectSmall.left,rectSmall.bottom+5, rectSmall.Width(), rectSmall.Height());
	}

	btn = (CButton *)GetDlgItem(IDC_BTN_ControlControl);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow1)
	{     
		m_tabPage[1]->ShowWindow(SW_HIDE);
		m_tabPage[1]->GetClientRect(&rectLarge);
		m_tabPage[1]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[1]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ImageCapture)->MoveWindow(rectSmall.left,rectSmall.bottom+rectLarge.Height()+5, rectSmall.Width(), rectSmall.Height());
	}
	else
	{
		GetDlgItem(IDC_BTN_ImageCapture)->MoveWindow(rectSmall.left,rectSmall.bottom+5, rectSmall.Width(), rectSmall.Height());
	}

	btn = (CButton *)GetDlgItem(IDC_BTN_ImageCapture);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow2)
	{    
		m_tabPage[2]->ShowWindow(SW_HIDE);
		m_tabPage[2]->GetClientRect(&rectLarge);
		m_tabPage[2]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[2]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ROI)->MoveWindow(rectSmall.left,rectSmall.bottom+rectLarge.Height()+5, rectSmall.Width(), rectSmall.Height());
	}
	else
	{
		GetDlgItem(IDC_BTN_ROI)->MoveWindow(rectSmall.left,rectSmall.bottom+5, rectSmall.Width(), rectSmall.Height());
	}

    btn = (CButton *)GetDlgItem(IDC_BTN_ROI);
    btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
    if(m_bshow3)
    {    
        m_tabPage[3]->ShowWindow(SW_HIDE);
        m_tabPage[3]->GetClientRect(&rectLarge);
        m_tabPage[3]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
        m_tabPage[3]->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BTN_Trigger)->MoveWindow(rectSmall.left,rectSmall.bottom+rectLarge.Height()+5, rectSmall.Width(), rectSmall.Height());
    }
    else
    {
        GetDlgItem(IDC_BTN_Trigger)->MoveWindow(rectSmall.left,rectSmall.bottom+5, rectSmall.Width(), rectSmall.Height());
    }

	btn = (CButton *)GetDlgItem(IDC_BTN_Trigger);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow4)
	{    
		m_tabPage[4]->ShowWindow(SW_HIDE);
		m_tabPage[4]->GetClientRect(&rectLarge);
		m_tabPage[4]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[4]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_MorePara)->MoveWindow(rectSmall.left,rectSmall.bottom+rectLarge.Height()+5, rectSmall.Width(), rectSmall.Height());
	}
	else
	{
		GetDlgItem(IDC_BTN_MorePara)->MoveWindow(rectSmall.left,rectSmall.bottom+5, rectSmall.Width(), rectSmall.Height());
	}

	btn = (CButton *)GetDlgItem(IDC_BTN_MorePara);
	btn->GetWindowRect(&rectSmall);
    ScreenToClient(&rectSmall);
	if(m_bshow5)
	{    
		m_tabPage[5]->ShowWindow(SW_HIDE);
		m_tabPage[5]->GetClientRect(&rectLarge);
		m_tabPage[5]->MoveWindow(rectSmall.left, rectSmall.bottom, rectLarge.Width(), rectLarge.Height());
		m_tabPage[5]->ShowWindow(SW_SHOW);
        CRect rect;
        this->GetWindowRect(&rect);
        ScreenToClient(&rect);
        int height = max(rect.Height(),(rectSmall.bottom+ rectLarge.Height()));
        m_nheight = height+m_nScrollWindow_Y;
        SetScrollSize(m_nFactScreen_Width, height);
	}
	else
	{
        CRect rect;
        this->GetWindowRect(&rect);
        ScreenToClient(&rect);
        int height = max(rect.Height(),rectSmall.bottom);
        m_nheight = height+m_nScrollWindow_Y;
        SetScrollSize(m_nFactScreen_Width, height);
	}

    // ���½���
	//UpdateData(FALSE);
}
HBRUSH CCameraControlMode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����

    if (pWnd->GetDlgCtrlID() == IDC_BTN_INFO || pWnd->GetDlgCtrlID() == IDC_BTN_ControlControl || 
        pWnd->GetDlgCtrlID() == IDC_BTN_ImageCapture || pWnd->GetDlgCtrlID() == IDC_BTN_ROI || 
        pWnd->GetDlgCtrlID() == IDC_BTN_Trigger || pWnd->GetDlgCtrlID() == IDC_BTN_MorePara)
    {
        HBRUSH hBrush = CreateSolidBrush(RGB(178, 178, 178));
        pDC->SetBkColor(RGB(178, 178, 178));

        return hBrush;
    }


    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}
