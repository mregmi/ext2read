// ext2explore.h : main header file for the ext2explore application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cext2exploreApp:
// See ext2explore.cpp for the implementation of this class
//

class Cext2exploreApp : public CWinApp
{
public:
	Cext2exploreApp();


// Overrides
public:
	virtual BOOL InitInstance();

	static void GetSizeString(CString& str, unsigned long size);
	static void GetDateString(CString& str, unsigned long epoch);
	static void GetPermString(CString& str, unsigned long perm, int of);

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cext2exploreApp theApp;