
// Daboluo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDaboluoApp:
// �йش����ʵ�֣������ Daboluo.cpp
//

class CDaboluoApp : public CWinApp
{
public:
	CDaboluoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDaboluoApp theApp;