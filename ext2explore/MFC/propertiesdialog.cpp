// propertiesdialog.cpp : implementation file
//

#include "stdafx.h"
#include "afxctl.h"
#include "atltime.h"

#include "ext2explore.h"
#include "propertiesdialog.h"
#include "ext2read.h"


// PropertiesDialog dialog

IMPLEMENT_DYNAMIC(PropertiesDialog, CDialog)

PropertiesDialog::PropertiesDialog(CWnd* pParent /*=NULL*/, DIRENTRY *ent)
	: CDialog(PropertiesDialog::IDD, pParent)
{
	entry = ent;
}

PropertiesDialog::~PropertiesDialog()
{
}

void PropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

int PropertiesDialog::OnInitDialog()
{
	int pic;
	CString str;

	if(EXT2_FT_ISDIR(entry->fileType))
		pic = IDI_ICON_DIR_32;
	else
		pic = IDI_ICON_FILE_32;

	name = (CEdit *)GetDlgItem(IDC_EDIT_NAME);
	name->SetWindowText(entry->fileName);

//	CPictureHolder *im = (CPictureHolder *)GetDlgItem(IDC_STATIC_IMG);
//	im->set

	CStatic *type = (CStatic *)GetDlgItem(IDC_STATIC_TYPE_VAL);
	type->SetWindowText(get_type_string(entry->fileType));

	CStatic *loc = (CStatic *)GetDlgItem(IDC_STATIC_LOCATION_VAL);
	loc->SetWindowText("");

	CStatic *size = (CStatic *)GetDlgItem(IDC_STATIC_SIZE_VAL);
	Cext2exploreApp::GetSizeString(str, entry->ino.i_size);
	size->SetWindowText(str);

	CStatic *dsize = (CStatic *)GetDlgItem(IDC_STATIC_SIZE_DISK_VAL);
	Cext2exploreApp::GetSizeString(str, entry->ino.i_blocks * 512);
	dsize->SetWindowText(str);

	CStatic *created = (CStatic *)GetDlgItem(IDC_STATIC_CREATED_VAL);
	Cext2exploreApp::GetDateString(str, entry->ino.i_ctime);
	created->SetWindowText(str);

	CStatic *modified = (CStatic *)GetDlgItem(IDC_STATIC_MODIFIED_VAL);
	Cext2exploreApp::GetDateString(str, entry->ino.i_mtime);
	modified->SetWindowText(str);

	CStatic *accessed = (CStatic *)GetDlgItem(IDC_STATIC_ACCESSED_VAL);
	Cext2exploreApp::GetDateString(str, entry->ino.i_atime);
	accessed->SetWindowText(str);

	str = "";
	CStatic *oval = (CStatic *)GetDlgItem(IDC_STATIC_OVAL);
	Cext2exploreApp::GetPermString(str, entry->ino.i_mode, 0);
	oval->SetWindowText(str);

	str = "";
	CStatic *gval = (CStatic *)GetDlgItem(IDC_STATIC_GVAL);
	Cext2exploreApp::GetPermString(str, entry->ino.i_mode, 0);
	gval->SetWindowText(str);

	str = "";
	CStatic *otval = (CStatic *)GetDlgItem(IDC_STATIC_OTVAL);
	Cext2exploreApp::GetPermString(str, entry->ino.i_mode, 0);
	otval->SetWindowText(str);

	return 0;
}

BEGIN_MESSAGE_MAP(PropertiesDialog, CDialog)

END_MESSAGE_MAP()


// PropertiesDialog message handlers

