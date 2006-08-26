// ext2explore.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ext2explore.h"
#include "MainFrm.h"

#include "ext2exploreDoc.h"
#include "LeftView.h"

#include "ext2read.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cext2exploreApp

BEGIN_MESSAGE_MAP(Cext2exploreApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &Cext2exploreApp::OnAppAbout)
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// Cext2exploreApp construction

Cext2exploreApp::Cext2exploreApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cext2exploreApp object

Cext2exploreApp theApp;

void Cext2exploreApp::GetSizeString(CString& str, unsigned long size)
{
	float lsize = (float)size;

	if(size <= 1024)
	{
		str.Format("%d bytes", size);
	}
	else if((size > 1024) && (size <= (1024 * 1024)))
	{
		lsize = (lsize/1024);
		str.Format("%.2f KB (%d bytes)", lsize, size);
	}
	else
	{
		lsize = (lsize/(1024 * 1024));
		str.Format("%.2f MB (%d bytes)", lsize, size);
	}
}

void Cext2exploreApp::GetDateString(CString& str, unsigned long epoch)
{
	CTime time(epoch);
	str = time.Format("%B %d, %Y, %I:%M:%S %p");
}

void Cext2exploreApp::GetPermString(CString& str, unsigned long mode, int of)
{
	char acc[4] = {"---"};

	switch(of)
	{
	case 0:
		acc[0] = (mode & EXT2_S_IRUSR)? 'r':'-';
		acc[1] = (mode & EXT2_S_IWUSR)? 'w':'-';
		acc[2] = (mode & EXT2_S_IXUSR)? 'x':'-';
		break;
	case 1:
		acc[0] = (mode & EXT2_S_IRGRP)? 'r':'-';
		acc[1] = (mode & EXT2_S_IWGRP)? 'w':'-';
		acc[2] = (mode & EXT2_S_IXGRP)? 'x':'-';
		break;
	case 2:
		acc[0] = (mode & EXT2_S_IROTH)? 'r':'-';
		acc[1] = (mode & EXT2_S_IWOTH)? 'w':'-';
		acc[2] = (mode & EXT2_S_IXOTH)? 'x':'-';
		break;
	}

	str.Append((const char *)&acc[0], 1);
	str.Append("\t");
	str.Append((const char *)&acc[1], 1);
	str.Append("\t");
	str.Append((const char *)&acc[2], 1);
}

// Cext2exploreApp initialization

BOOL Cext2exploreApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
//	SetRegistryKey(_T("Ext2read"));
//	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cext2exploreDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open
	//EnableShellOpen();
	//RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	m_pMainWnd->SetWindowText("ext2explore");
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void Cext2exploreApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// Cext2exploreApp message handlers

