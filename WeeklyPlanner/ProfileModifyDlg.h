#pragma once
#include "afxwin.h"


// CProfileModifyDlg 대화 상자입니다.

class CProfileModifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProfileModifyDlg)

public:
	CProfileModifyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProfileModifyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROFILE_MODIFY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
