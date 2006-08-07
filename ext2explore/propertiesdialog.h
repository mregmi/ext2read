#pragma once


// PropertiesDialog dialog
#include "ext2read.h"

class PropertiesDialog : public CDialog
{
	DECLARE_DYNAMIC(PropertiesDialog)

public:
	PropertiesDialog(CWnd* pParent = NULL, DIRENTRY *ent = NULL);   // standard constructor
	virtual ~PropertiesDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROPERTIES };
	DIRENTRY *entry;
	CEdit *name;
	CCheckListBox *ur;

private:
	void GetSizeString(CString& str, unsigned long size);
	void GetDateString(CString& str, unsigned long epoch);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual int OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckUr();
};
