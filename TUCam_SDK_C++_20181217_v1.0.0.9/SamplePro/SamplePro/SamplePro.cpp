
// SamplePro.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SamplePro.h"
#include "SampleProDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleProApp

BEGIN_MESSAGE_MAP(CSampleProApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSampleProApp ����

CSampleProApp::CSampleProApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSampleProApp ����

CSampleProApp theApp;


// CSampleProApp ��ʼ��

BOOL CSampleProApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	HANDLE hMutex=CreateMutex(NULL,TRUE,_T("SamplePro.exe"));


	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		int nState = MessageBox(0,"An instance has already been running,Do you Close it","Close",MB_YESNO);

		if(nState == IDYES)//�û�������"��"��ť
			WinExec("taskkill -f -im SamplePro.exe",SW_HIDE);
	}
	else
	{

		CSampleProDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȷ�������رնԻ���Ĵ���
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȡ�������رնԻ���Ĵ���
		}
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ��������Ӧ�ó������Ϣ�á�
	return FALSE;
}
