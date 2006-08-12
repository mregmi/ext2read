// ext2exploreView.cpp : implementation of the Cext2exploreView class
//

#include "stdafx.h"
#include "ext2explore.h"

#include "ext2exploreDoc.h"
#include "ext2exploreView.h"
#include "MainFrm.h"

#include "ext2read.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cext2exploreView

IMPLEMENT_DYNCREATE(Cext2exploreView, CListView)

BEGIN_MESSAGE_MAP(Cext2exploreView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// Cext2exploreView construction/destruction

Cext2exploreView::Cext2exploreView()
{
	// TODO: add construction code here

}

Cext2exploreView::~Cext2exploreView()
{
}

void Cext2exploreView::Browse()
{
	int cnt = 0, image = 1, item;
	DIRENTRY entry;
	CListCtrl& ctrl = GetListCtrl();
	CString str;

	ctrl.DeleteAllItems();
	while(get_dir_entry(&entry) != -1)
	{
		if(EXT2_FT_ISDIR(entry.fileType))
			image = 2;
		else
			image = 1;

		item = ctrl.InsertItem(cnt, entry.fileName, image);
		
		if(!EXT2_FT_ISDIR(entry.fileType))
			Cext2exploreApp::GetSizeString(str, entry.ino.i_size);
		else
			str = " ";
		ctrl.SetItemText(item, 1, str);

		ctrl.SetItemText(item, 2, get_type_string(entry.fileType));

		//Cext2exploreApp::GetDateString(str, entry.ino.i_mtime);
		str = get_access(entry.ino.i_mode);
		ctrl.SetItemText(item, 3, str);

		ctrl.SetItemData(item, (DWORD_PTR)entry.inoNum);
	}
}


BOOL Cext2exploreView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style |= LVS_SINGLESEL |  LVS_SORTASCENDING ;
	return CListView::PreCreateWindow(cs);
}

void Cext2exploreView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}


// Cext2exploreView diagnostics

#ifdef _DEBUG
void Cext2exploreView::AssertValid() const
{
	CListView::AssertValid();
}

void Cext2exploreView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

Cext2exploreDoc* Cext2exploreView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cext2exploreDoc)));
	return (Cext2exploreDoc*)m_pDocument;
}
#endif //_DEBUG


// Cext2exploreView message handlers
void Cext2exploreView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	

	if(lpStyleStruct->styleNew & LVS_REPORT)
	{
		CListCtrl& ctrl = GetListCtrl();
		ctrl.SetColumnWidth(0, LVSCW_AUTOSIZE);
		ctrl.SetColumnWidth(1, LVSCW_AUTOSIZE);
		ctrl.SetColumnWidth(2, LVSCW_AUTOSIZE);
		ctrl.SetColumnWidth(3, LVSCW_AUTOSIZE);
	}
}

DWORD_PTR Cext2exploreView::GetSelected(CPoint& point, CString& str)
{
	CRect rct;
	CListCtrl& ctrl = GetListCtrl();

	int count = ctrl.GetItemCount();

	for(int i = 0; i < count; i++)
	{
		ctrl.GetItemRect(i, &rct, LVIR_BOUNDS);
		if(rct.PtInRect(point))
		{
			ctrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			str = ctrl.GetItemText(i, 0);
			return ctrl.GetItemData(i);
		}
	}

	return NULL;
}

void Cext2exploreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CString str;
	uint32_t data;
	CMainFrame *frm = DYNAMIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
	
	data = (uint32_t)GetSelected(point, str);
	frm->SetExt2Selection(data);

	if(str.IsEmpty())
		return;

	TRACE("Left Mouse down  item %s data %d\n", str, data);
}

void Cext2exploreView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CString str;
	uint32_t data;

	data = (uint32_t)GetSelected(point, str);

	CMainFrame *frm = DYNAMIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
	frm->RButtonClicked(data, point, this);

	//TRACE("Right Mouse Down item %s\n", str);
}

void Cext2exploreView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int ret;
	CString str;
	uint32_t data;
	
	data = (uint32_t)GetSelected(point, str);
	if(str.IsEmpty())
		return;

	if(!data)
	{
		ret = mount_partition(str);
	}
	else
	{
		ret = enter_new_dir(data, str);	
	}
	
	if(ret < 0)
		return;

	Browse();

	CMainFrame *frm = DYNAMIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
	frm->update();
//	TRACE("Db Click item %s\n", str);
}
