#pragma once


// CMainToolBar

class CMainToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CMainToolBar)

public:
	CMainToolBar();
	virtual ~CMainToolBar();

	CComboBox choice;
	CEdit address;
protected:
	DECLARE_MESSAGE_MAP()
};


