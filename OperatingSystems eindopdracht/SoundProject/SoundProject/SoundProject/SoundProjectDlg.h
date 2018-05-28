// SoundProjectDlg.h : header file
//

#pragma once


#include "Wave.h"


// CSoundProjectDlg dialog
class CSoundProjectDlg : public CDialog
{
// Construction
public:
	CSoundProjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOUNDPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CString m_strOpenFile;
	CString m_strOpenFile2;
	CWave m_Wave1, m_Wave2;

public:
	afx_msg void OnBnClickedButtonBrowseOpenFile();
	afx_msg void OnBnClickedButtonBrowseOpenFile2();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPlay2();
	afx_msg void OnBnClickedButtonPause2();
	afx_msg void OnBnClickedButtonStop2();
	afx_msg void OnBnClickedButtonMix();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonSave();

};
