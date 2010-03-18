// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "ext2explore.h"

#include "ext2exploreDoc.h"
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

Cext2exploreDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cext2exploreDoc)));
	return (Cext2exploreDoc*)m_pDocument;
}
#endif //_DEBUG


DWORD_PTR CLeftView::GetSelected(CPoint& point, CString& str)
{
	CRect rct;
	CTreeCtrl& ctrl = GetTreeCtrl();

	int count = ctrl.GetCount();

	for(int i = 0; i < count; i++)
	{
//		ctrl.GetItemRect(i, &rct, LVIR_BOUNDS);
		if(rct.PtInRect(point))
		{
//			ctrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
//			str = ctrl.GetItemText(i, 0);
//			return ctrl.GetItemData(i);
		}
	}

	return NULL;
}

// CLeftView message handlers
void CLeftView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame *frm = DYNAMIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
	frm->update();

	TRACE("Left Mouse down");
}

void CLeftView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame *frm = DYNAMIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
	frm->RButtonClicked(0, point, this);
	TRACE("Right Mouse Down");
}

void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	TRACE("Db Click");
}
