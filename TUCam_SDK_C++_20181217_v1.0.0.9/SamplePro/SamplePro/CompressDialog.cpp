// CompressDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "Example.h"
#include "CompressDialog.h"
#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int	_fstr(CStringArray &str, LPCSTR s);
LPCSTR _Wstr2Mstr(LPCWSTR InStr);
/////////////////////////////////////////////////////////////////////////////
// CCompressDialog dialog


CCompressDialog::CCompressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCompressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompressDialog)
	//}}AFX_DATA_INIT
}


void CCompressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompressDialog)
	DDX_Control(pDX, IDC_COMBO_COMPRESS, m_CompressorCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompressDialog, CDialog)
	//{{AFX_MSG_MAP(CCompressDialog)
		ON_CBN_SELCHANGE(IDC_COMBO_COMPRESS, OnSelchangeComboCompress)
		ON_BN_CLICKED(IDC_BTN_SETTING, OnBtnSetting)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCompressDialog::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompressDialog message handlers

BOOL CCompressDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// �г���Ҫ֧�ֵ�CODEC
   	EnumVideoCodecs();
	
    // ����Ҫ֧�ֵ�CODEC�����б��
	for(int i=0; i< m_CodecNames.GetSize(); i++)
    {
		m_CompressorCtrl.AddString(m_CodecNames[i]);
    }

    int nSel = m_nIdxFcc < m_CodecNames.GetSize() ? m_nIdxFcc : m_CodecNames.GetSize();
    m_CompressorCtrl.AddString("Full frame(No compression)");	
    m_CompressorCtrl.SetCurSel(nSel);
    nSelIndex = nSel;
    m_CanConfig = FALSE;
    GetDlgItem(IDC_BTN_SETTING)->EnableWindow(m_CanConfig);

    OnSelchangeComboCompress();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// ****************************************************************************
//  EnumVideoCodecs - ��VCM�ж�ȡ��װ����Ƶ���������
//  Decription:
//  Argument:
//  Return:
//  Revision:
//  v1.0 Writen by Tan Tingxi 2008-08-03
// ****************************************************************************
BOOL CCompressDialog::EnumVideoCodecs()
{
	HIC    Hic;
	ICINFO IcInfo;
	
    // ���ԭ�е�����
    m_CodecNames.RemoveAll( );
    m_CodecFcc.RemoveAll( );
    m_CodecIndex.RemoveAll();

	
    // ����VCM�а�װ��CODEC�����˲��ֲ�ʹ�õ�CODEC
	for (int i = 0; ICInfo(ICTYPE_VIDEO, i, &IcInfo); i++)
	{
        Hic = ICOpen(IcInfo.fccType, IcInfo.fccHandler, ICMODE_QUERY);
        if (Hic)
        {
            // ȡ��CODEC��.
            ICGetInfo(Hic, &IcInfo, sizeof(IcInfo));
            DWORD sup = IcInfo.dwFlags&VIDCF_QUALITY;
			
            // ���ظ���CODECȥ���������µ�CODEC���б�
            LPCSTR	my=_Wstr2Mstr(IcInfo.szDescription);
            int nFindFilter =_fstr(m_CodecNames, my);
            if (nFindFilter !=  (-1))
            {
                m_CodecNames.RemoveAt(nFindFilter);
                m_CodecFcc.RemoveAt(nFindFilter);
                m_CodecIndex.RemoveAt(nFindFilter);
            }
			
            // ���˲���Ҫ��CODEC���������¼���
            if (
                (IcInfo.fccHandler == MAKETAG('M', 'P', '4', '2')) || // MP42(0x3234504d)
                (IcInfo.fccHandler == MAKETAG('M', 'P', 'G', '4')) || // MPG4(0x3447504d)
                (IcInfo.fccHandler == MAKETAG('W', 'M', 'V', '3')) || // WMV3(0x33564d57)
                (IcInfo.fccHandler == MAKETAG('A', 'C', 'D', 'V')) || // ACDV(0x56444341)
                (IcInfo.fccHandler == MAKETAG('H', 'F', 'Y', 'U')) || // HFYU(0x55594648)
                (IcInfo.fccHandler == MAKETAG('A', 'V', 'R', 'n')) || // AVRn()
                (IcInfo.fccHandler == MAKETAG('I', 'Y', 'U', 'V')) || // IYUV :  intel MJPG-1 
                (IcInfo.fccHandler == MAKETAG('X', 'V', 'I', 'D')) ||
				(IcInfo.fccHandler == MAKETAG('M', 'S', 'V', 'C'))  // MSVC(0x4356534d)
    			)   // XVID(0x44495658)
            {
            }
            else
            {
				
                ICClose(Hic);            
                 continue;
             }
			
            m_CodecIndex.Add(i);
            m_CodecNames.Add(my);
            m_CodecFcc.Add(IcInfo.fccHandler);
            ICClose(Hic);            
        }
	}//end loop
	
	return (BOOL)m_CodecFcc.GetSize();
}

// ****************************************************************************
//  GetCompressOptions - ��ȡ�û�ѡ���CODEC����
//  Decription:
//  Argument:
//      pOpt - AVIѹ��ѡ�����
//  Return:
//  Revision:
//  v1.0 Writen by Tan Tingxi 2008-08-03
// ****************************************************************************
BOOL CCompressDialog::GetCompressOptions(AVICOMPRESSOPTIONS* pOpt)
{
	HIC    Hic;
	ICINFO IcInfo;
    int    nIndex;


    if (NULL == pOpt)
    {
        return FALSE;
    }

    // if fill frame (non-compress) is selected, using DIB type
    if (m_CodecIndex.GetSize() <= nSelIndex)
    {
        pOpt->fccType           = 0; // IcInfo.fccType; 
        pOpt->fccHandler        = MAKETAG('D', 'I', 'B', ' ');  // "DIB " 
        pOpt->dwKeyFrameEvery   = 0; 
        pOpt->dwQuality         = 0; 
        pOpt->dwBytesPerSecond  = 0; 
        pOpt->dwFlags           = 8; 
        pOpt->lpFormat          = 0; 
        pOpt->cbFormat          = 0; 
        pOpt->lpParms           = 0;//pMem;         // pointer to state data
        pOpt->cbParms           = 0;//dwStateSize;  // sizeof state data
        pOpt->dwInterleaveEvery = 0; 
        return TRUE;
    }

    nIndex = m_CodecIndex[nSelIndex];
    ICInfo(ICTYPE_VIDEO, nIndex, &IcInfo);
    Hic = ICOpen(IcInfo.fccType, IcInfo.fccHandler, ICMODE_QUERY);

    if (Hic)
    {
        // find out the compressor name.
        ICGetInfo(Hic, &IcInfo, sizeof(IcInfo));

        //DWORD dwStateSize = ICGetStateSize(Hic);
        //LPVOID pMem = (LPVOID)GlobalAlloc(GHND, dwStateSize);
        //ICGetState(Hic, pMem, dwStateSize);
        
        pOpt->fccType           = 0;// IcInfo.fccType; 
        pOpt->fccHandler        = IcInfo.fccHandler; 
        pOpt->dwKeyFrameEvery   = 0; 
        pOpt->dwQuality         = 0; 
        pOpt->dwBytesPerSecond  = 0; 
        pOpt->dwFlags           = 8; 
        pOpt->lpFormat          = 0; 
        pOpt->cbFormat          = 0; 
        pOpt->lpParms           = 0;//pMem;         // pointer to state data
        pOpt->cbParms           = 0;//dwStateSize;  // sizeof state data
        pOpt->dwInterleaveEvery = 0; 

        ICClose(Hic);            
        return TRUE;
    }

    return (FALSE);
}

// ****************************************************************************
//  GetCompressorFccHandler - ��ȡ��ǰѡ���CODEC
//  Decription:
//  Argument:
//  Return:
//      ���ֽڵ�CODEC���Ͳ���
//  Revision:
//  v1.0 Writen by Tan Tingxi 2008-08-03
// ****************************************************************************
DWORD CCompressDialog::GetCompressorFccHandler()
{
	HIC    Hic;
	ICINFO IcInfo;
    int    nIndex;

    // if fill frame (non-compress) is selected, using DIB type
    if (m_CodecIndex.GetSize() > nSelIndex)
    {
        nIndex = m_CodecIndex[nSelIndex];
        ICInfo(ICTYPE_VIDEO, nIndex, &IcInfo);
        Hic = ICOpen(IcInfo.fccType, IcInfo.fccHandler, ICMODE_QUERY);

        if (Hic)
        {
            // ��ȡCODEC��ص���Ϣ��������CODEC���ͱ�ʶ.
            ICGetInfo(Hic, &IcInfo, sizeof(IcInfo));
            ICClose(Hic);            
            return IcInfo.fccHandler;
        }
    }

    return MAKETAG('D', 'I', 'B', ' ');;  // "DIB " 
}

int CCompressDialog::GetCurrentFcc()
{
    return nSelIndex;
}

void CCompressDialog::OnSelchangeComboCompress() 
{
	// TODO: Add your control notification handler code here
	HIC    Hic;
	ICINFO IcInfo;
    int    nIndex;
	
    m_CanConfig = FALSE;
	nSelIndex = m_CompressorCtrl.GetCurSel();
    if (m_CodecIndex.GetSize() > nSelIndex)
    {
        nIndex = m_CodecIndex[nSelIndex];
        ICInfo(ICTYPE_VIDEO, nIndex, &IcInfo);
        Hic = ICOpen(IcInfo.fccType, IcInfo.fccHandler, ICMODE_QUERY);
        if (Hic)
        {
            // �����CODEC�����ã����ÿ����ñ�־
            m_CanConfig = ICQueryConfigure(Hic);
        }
        ICClose(Hic);            
    }
	
    GetDlgItem(IDC_BTN_SETTING)->EnableWindow(m_CanConfig);	
}

void CCompressDialog::OnBtnSetting() 
{
	// TODO: Add your control notification handler code here
	HIC    Hic;
	ICINFO IcInfo;
    int    nIndex;
	
    if (m_CanConfig)
    {
        // m_CanConfig = FALSE;
		nSelIndex = m_CompressorCtrl.GetCurSel();
        if (m_CodecIndex.GetSize() > nSelIndex)
        {
            nIndex = m_CodecIndex[nSelIndex];
            ICInfo(ICTYPE_VIDEO, nIndex, &IcInfo);
            Hic = ICOpen(IcInfo.fccType, IcInfo.fccHandler, ICMODE_QUERY);
            if (Hic)
            {
                // ��ʾCODEC�Դ��ĵ����öԻ�
                ICConfigure(Hic, NULL);
            }
            ICClose(Hic);            
        }
    }		
}

void CCompressDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	nSelIndex = m_CompressorCtrl.GetCurSel();

	CDialog::OnOK();
}


// ****************************************************************************
//  _Wstr2Mstr - Wide�����ַ���ת��ΪByte�����ַ���
//  Decription:
//  Argument:
//      InStr - Ҫת����Wide�ַ�����
//  Return:
//  Revision:
//  v1.0 Writen by Tan Tingxi 2008-08-03
// ****************************************************************************
LPCSTR _Wstr2Mstr(LPCWSTR InStr)
{
	static char OutStr[256];
	
	WideCharToMultiByte(CP_ACP,
		WC_COMPOSITECHECK|WC_DEFAULTCHAR, 
		InStr, 
		-1, 
		OutStr, 
		256, 
		NULL, 
		NULL);
	
	return OutStr;
}

// ****************************************************************************
//  _fstr - ��str�����в���s�Ƿ����
//  Decription:
//  Argument:
//  Return:
//  Revision:
//  v1.0 Writen by Tan Tingxi 2008-08-03
// ****************************************************************************
int	_fstr(CStringArray &str, LPCSTR s)
{
	for (int i = 0; i < str.GetSize();i ++)
    {
		if (str[i] == s)
        {
            return i;
        }
    }
	return -1;
}


void CCompressDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCompressDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
