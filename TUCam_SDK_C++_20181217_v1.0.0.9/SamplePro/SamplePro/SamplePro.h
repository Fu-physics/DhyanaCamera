
// SamplePro.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSampleProApp:
// �йش����ʵ�֣������ SamplePro.cpp
//

class CSampleProApp : public CWinAppEx
{
public:
	CSampleProApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSampleProApp theApp;