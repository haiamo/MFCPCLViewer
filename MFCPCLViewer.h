
// MFCPCLViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCPCLViewerApp: 
// �йش����ʵ�֣������ MFCPCLViewer.cpp
//

class CMFCPCLViewerApp : public CWinApp
{
public:
	CMFCPCLViewerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCPCLViewerApp theApp;