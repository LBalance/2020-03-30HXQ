
// dlltest1.h : dlltest1 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Cdlltest1App:
// �йش����ʵ�֣������ dlltest1.cpp
//

class Cdlltest1App : public CWinAppEx
{
public:
	Cdlltest1App();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cdlltest1App theApp;
