// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class Cext2exploreView;
class CLeftView;

#include "maintoolbar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:
	CImageList *i32x32, *i16x16;
	CEdit *edit;
	unsigned long inode;
// Operations
public:
	void update();
	void RButtonClicked(unsigned long inode, CPoint& pt, CWnd *wnd);
	void SetExt2Selection(unsigned long inode);
	void ShowProperties();
	void CopyExt2File();
	void SaveExt2File();
	void RescanSystem();
	void ScanSystem();

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//virtual void OnLButtonDown(UINT nFlags, CPoint point);

// Implementation
public:
	virtual ~CMainFrame();
	Cext2exploreView* GetRightPane();
	CLeftView* GetLeftPane();
	int AppInit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CMainToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg void OnSelchangeCombo();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};


