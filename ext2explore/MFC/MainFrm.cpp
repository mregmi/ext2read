// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ext2explore.h"

#include "LeftView.h"
#include "ext2exploreView.h"
#include "propertiesdialog.h"
#include "maintoolbar.h"

#include "MainFrm.h"

#include "ext2read.h"
#include "platform.h"
#include "partition.h"
#include "parttypes.h"

extern "C" struct sys sys;
extern "C" struct ext2partition ext2part;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_SAVE_FILE, SaveExt2File)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnViewStyle)
	ON_COMMAND(ID_TOOLS_PROPERTIES, ShowProperties)
	ON_COMMAND(ID_TOOLS_COPY, CopyExt2File)
	ON_COMMAND(ID_FILE_RESCANSYSTEM, RescanSystem)
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	inode = 0;
}

CMainFrame::~CMainFrame()
{
	delete i32x32;
	delete i16x16;
	
	free_system();
}

void CMainFrame::ScanSystem()
{
	char pathname[20];
	struct partition *p;
	struct disk *dsk;
	int ret, cnt = 0, itm;

	CString str;

	Cext2exploreView* explorer = GetRightPane();
	CLeftView* treeview = GetLeftPane();
	
	CListCtrl &list = explorer->GetListCtrl();
	CTreeCtrl &tree = treeview->GetTreeCtrl();

	sys.ndisks = get_ndisks();

	LOG_INFO("No of disks %d\n", sys.ndisks);

	for(int i = 0; i < sys.ndisks; i++)
	{
		get_nthdevice(pathname, sys.ndisks);
		LOG_INFO("Scanning %s\n", pathname);
		ret = scan_partitions(pathname, i);
		if(ret < 0)
		{
			LOG_INFO("Scanning of %s failed: ", pathname);
			continue;
		}
	}

	print_ext2();

	for(int i = 0; i < sys.ndisks; i++)
	{
		dsk = &sys.dsk[i];
		for(int j = 0; j < dsk->totalpart; j++)
		{
			p = &dsk->part[j];
			if(p->systemId != EXT2)
				continue;

			str = p->linux_name;
			//str.Append("-");
			//str.Append(part_type(p->systemId));
			itm = list.InsertItem(cnt, str, 0);
			list.SetItemData(itm, (DWORD_PTR)0);
			tree.InsertItem(str, 0, 0);
			m_wndToolBar.choice.AddString(str);
			cnt++;
		}
	}
}

int CMainFrame::AppInit()
{
	Cext2exploreView* explorer = GetRightPane();
	CLeftView* treeview = GetLeftPane();
	
	CListCtrl &list = explorer->GetListCtrl();
	CTreeCtrl &tree = treeview->GetTreeCtrl();

	i32x32 = new CImageList;
	i32x32->Create(32, 32, ILC_COLORDDB | ILC_MASK, 3, 0);
	i32x32->Add(AfxGetApp()->LoadIcon(IDI_ICON_DRIVE_32));
	i32x32->Add(AfxGetApp()->LoadIcon(IDI_ICON_FILE_32));
	i32x32->Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_32));
	
	i16x16 = new CImageList;
	i16x16->Create(16, 16, ILC_COLORDDB | ILC_MASK, 3, 0);
	i16x16->Add(AfxGetApp()->LoadIcon(IDI_ICON_DRIVE_16));
	i16x16->Add(AfxGetApp()->LoadIcon(IDI_ICON_FILE_16));
	i16x16->Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_16));

	list.SetImageList(i32x32, LVSIL_NORMAL);
	list.SetImageList(i16x16, LVSIL_SMALL);

	tree.SetImageList(i16x16, LVSIL_NORMAL);

	log_init();
	ScanSystem();

	list.InsertColumn(0, "File name ");
	list.InsertColumn(1, "Size ");
	list.InsertColumn(2, "Type ");
	list.InsertColumn(3, "Permission");

	return 0;
}

void CMainFrame::update()
{
	m_wndToolBar.address.SetWindowText(ext2part.path);
/*	if(m_wndToolBar.choice.GetCurSel() == CB_ERR)
	{

	}
*/

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	int index = 0;
	CRect rect;
	while (m_wndToolBar.GetItemID(index) != IDC_SPACING1) index++;

	m_wndToolBar.SetButtonInfo(index, IDC_SPACING1, TBBS_SEPARATOR, 200);
    m_wndToolBar.GetItemRect(index, &rect);
	
	rect.top+=2;
    rect.bottom += 200;

    // then .Create the combo box and show it
    if (!m_wndToolBar.address.Create(WS_CHILD|WS_VISIBLE,
                                       rect, &m_wndToolBar, IDC_EDIT))
    {
        TRACE0("Failed to create Address bar in toolbar\n");
        return FALSE;
    }
    m_wndToolBar.address.ShowWindow(SW_SHOW);

	index = 0;
	while (m_wndToolBar.GetItemID(index) != IDC_SPACING2) index++;

	m_wndToolBar.SetButtonInfo(index, IDC_SPACING2, TBBS_SEPARATOR, 150);
    m_wndToolBar.GetItemRect(index, &rect);
	rect.top+=2;
    rect.bottom += 200;

    // then .Create the combo box and show it
    if (!m_wndToolBar.choice.Create(WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL| 
                                       CBS_DROPDOWNLIST|CBS_HASSTRINGS,
                                       rect, &m_wndToolBar, IDC_COMBO))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }
    m_wndToolBar.choice.ShowWindow(SW_SHOW);

	AppInit();

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(Cext2exploreView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


Cext2exploreView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	Cext2exploreView* pView = DYNAMIC_DOWNCAST(Cext2exploreView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	
	return pView;
}


void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	if (!pCmdUI)
		return;

	// TODO: customize or extend this code to handle choices on the View menu

	Cext2exploreView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the View menu
	Cext2exploreView* pView = GetRightPane();

	// if the right-hand pane has been created and is a Cext2exploreView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
		{
			dwStyle |= LVS_SORTASCENDING;
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
		}
	}
}

void CMainFrame::RButtonClicked(unsigned long ino, CPoint& pt, CWnd *wnd)
{
	CMenu *popup;

	wnd->ClientToScreen(&pt);
	inode = ino;
	popup = GetMenu()->GetSubMenu(3);
	//popup->CreatePopupMenu();

	popup->EnableMenuItem(ID_TOOLS_CREATEFILE, MF_GRAYED | MF_BYCOMMAND);
	popup->EnableMenuItem(ID_TOOLS_CREATEFOLDER, MF_GRAYED | MF_BYCOMMAND);
	popup->EnableMenuItem(ID_TOOLS_CUT, MF_GRAYED | MF_BYCOMMAND);
	popup->EnableMenuItem(ID_TOOLS_PASTE, MF_GRAYED | MF_BYCOMMAND);
	popup->EnableMenuItem(ID_TOOLS_RENAME, MF_GRAYED | MF_BYCOMMAND);
	popup->EnableMenuItem(ID_TOOLS_DELETE, MF_GRAYED | MF_BYCOMMAND);

	if(!ino)
	{
		popup->EnableMenuItem(ID_TOOLS_PROPERTIES, MF_GRAYED | MF_BYCOMMAND);
		popup->EnableMenuItem(ID_TOOLS_COPY, MF_GRAYED | MF_BYCOMMAND);
	}
	else
	{
		popup->EnableMenuItem(ID_TOOLS_PROPERTIES, MF_ENABLED | MF_BYCOMMAND);
		popup->EnableMenuItem(ID_TOOLS_COPY, MF_ENABLED | MF_BYCOMMAND);
	}


	popup->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
}

void CMainFrame::ShowProperties()
{
	PropertiesDialog *dlg;
	DIRENTRY entry;
	int ret;

	
	if(!inode)
		return;

	ret = get_dentry_inode(&entry, inode);
	if(ret < 0)
		return;
	
	dlg = new PropertiesDialog(this, &entry);
	dlg->DoModal();

	delete dlg;
}

void CMainFrame::CopyExt2File()
{
	if(!inode)
		return;

}

void CMainFrame::SaveExt2File()
{
	CFileDialog *fdlg;
	DIRENTRY entry;
	CString str;
	int ret;

	TRACE("We got here\n");
	if(!inode)
		return;

	ret = get_dentry_inode(&entry, inode);
	if(ret < 0)
		return;

	if(!EXT2_FT_ISREG(entry.fileType) && !EXT2_FT_ISDIR(entry.fileType))
		return;

	fdlg = new CFileDialog(false, NULL, entry.fileName);
	if(fdlg->DoModal() != IDOK)
		return;
	
	str = fdlg->GetPathName();
	//str.Append(fdlg->GetFileName());
	
	if(fdlg)
		ret = save_file(str, inode);
	if(ret < 0)
	{
		MessageBox("Error Saving file: ", "Error Saving File");
	}

	delete fdlg;
}

void CMainFrame::SetExt2Selection(unsigned long ino)
{
	inode = ino;
}

void CMainFrame::RescanSystem()
{
	Cext2exploreView* explorer = GetRightPane();
	CLeftView* treeview = GetLeftPane();
	
	CListCtrl &list = explorer->GetListCtrl();
	CTreeCtrl &tree = treeview->GetTreeCtrl();

	list.DeleteAllItems();
	tree.DeleteAllItems();

	for (int i = m_wndToolBar.choice.GetCount()-1; i >= 0; i--)
	{
		m_wndToolBar.choice.DeleteString( i );
	}
	
	free_system();

	ScanSystem();
}

void CMainFrame::OnSelchangeCombo()
{
	CString str;
	int idx;

	idx = m_wndToolBar.choice.GetCurSel();
	m_wndToolBar.choice.GetLBText(idx, str);

	RescanSystem();
	mount_partition(str);
	idx = m_wndToolBar.choice.FindString(0, str);
	m_wndToolBar.choice.SetCurSel(idx);
	GetRightPane()->Browse();

}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}
