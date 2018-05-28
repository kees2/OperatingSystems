// SoundProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoundProject.h"
#include "SoundProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSoundProjectDlg dialog




CSoundProjectDlg::CSoundProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundProjectDlg::IDD, pParent)
	, m_strOpenFile(_T(""))
	, m_strOpenFile2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSoundProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPEN_FILE_PATH, m_strOpenFile);
	DDX_Text(pDX, IDC_EDIT_OPEN_FILE_PATH2, m_strOpenFile2);
}

BEGIN_MESSAGE_MAP(CSoundProjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_OPEN_FILE, &CSoundProjectDlg::OnBnClickedButtonBrowseOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_OPEN_FILE2, &CSoundProjectDlg::OnBnClickedButtonBrowseOpenFile2)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSoundProjectDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CSoundProjectDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSoundProjectDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PLAY2, &CSoundProjectDlg::OnBnClickedButtonPlay2)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE2, &CSoundProjectDlg::OnBnClickedButtonPause2)
	ON_BN_CLICKED(IDC_BUTTON_STOP2, &CSoundProjectDlg::OnBnClickedButtonStop2)
	ON_BN_CLICKED(IDC_BUTTON_MIX, &CSoundProjectDlg::OnBnClickedButtonMix)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CSoundProjectDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSoundProjectDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CSoundProjectDlg message handlers

BOOL CSoundProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PAUSE2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MIX)->EnableWindow(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSoundProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSoundProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSoundProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSoundProjectDlg::OnBnClickedButtonBrowseOpenFile()
{
	// Find first .WAV file
	CFileDialog fileDlg(TRUE, NULL, NULL, 4|2, _T("WAV Files (*.wav)|*.wav||"), this, NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		// Load first .WAV file
		m_strOpenFile = fileDlg.GetPathName();
		m_Wave1.Load((LPTSTR)((LPCTSTR)m_strOpenFile));
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MIX)->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonBrowseOpenFile2()
{
	// Find second .WAV file
	CFileDialog fileDlg(TRUE, NULL, NULL, 4|2, _T("WAV Files (*.wav)|*.wav||"), this, NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		// Load second .WAV file
		m_strOpenFile2 = fileDlg.GetPathName();
		m_Wave2.Load((LPTSTR)((LPCTSTR)m_strOpenFile2));
		GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonPlay()
{
	// Play first file
	if (!m_Wave1.IsPlaying())
	{
		m_Wave1.Play();
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonPause()
{
	// Pause first file
	if (!m_Wave1.IsPaused())
	{
		m_Wave1.Pause();
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonStop()
{
	// Stop first file
	if (!m_Wave1.IsStopped())
	{
		m_Wave1.Stop();
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonPlay2()
{
	// Play second file
	if (!m_Wave2.IsPlaying())
	{
		m_Wave2.Play();
		GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PAUSE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(TRUE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonPause2()
{
	// Pause second file
	if (!m_Wave2.IsPaused())
	{
		m_Wave2.Pause();
		GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(TRUE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonStop2()
{
	// Stop second file
	if (!m_Wave2.IsStopped())
	{
		m_Wave2.Stop();
		GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PAUSE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP2)->EnableWindow(FALSE);
	}
}

void CSoundProjectDlg::OnBnClickedButtonMix()
{
	// Mix files
	if (!m_Wave1.IsPlaying() && !m_Wave2.IsPlaying())
	{
		if (m_Wave1.Mix(m_Wave2))
		{
			GetDlgItem(IDC_BUTTON_MIX)->EnableWindow(FALSE);
			MessageBox(_T("Files mixed...\nPlay first file to hear it!"), _T("Success..."), MB_OK | MB_ICONINFORMATION);
		}
		else
			MessageBox(_T("Files could not be mixed...\nCheck file attributes!"), _T("Error..."), MB_OK | MB_ICONERROR);
	}
}

void CSoundProjectDlg::OnBnClickedButtonAbout()
{
	// Show about dialog
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CSoundProjectDlg::OnBnClickedButtonSave()
{
	// Save mixed file
	if ((m_Wave1.IsValid()) && (m_Wave1.Save(_T("mixer_test.wav"))))
		MessageBox(_T("Mixed file saved as: mixer_test.wav"), _T("Info..."), MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(_T("File not loaded...\nPlease load the file first!"), _T("Error..."), MB_OK | MB_ICONERROR);
}
