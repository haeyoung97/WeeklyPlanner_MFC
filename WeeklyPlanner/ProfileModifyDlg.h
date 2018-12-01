#pragma once
#include "afxwin.h"


// CProfileModifyDlg ��ȭ �����Դϴ�.

class CProfileModifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProfileModifyDlg)

public:
	CProfileModifyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProfileModifyDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROFILE_MODIFY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnProfileCancel;
	CButton m_btnProfileApply;
	CButton m_btnProfileOpen;
	afx_msg void OnBnClickedButtonModifyCancle();
	afx_msg void OnBnClickedButtonImagefileOpen();
	CString ReturnFilePath();
	CString m_strFilePath;
	CStatic m_profileModalImg;
};
