#pragma once
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxwin.h"
#include "afxcmn.h"


// CHistoryView 대화 상자입니다.

class CHistoryView : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryView)

public:
	CHistoryView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHistoryView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HISTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	CDateTimeCtrl m_ctrlHistory;
//	COleDateTime m_vHistory;
//	CDateTimeCtrl m_ctrlHistory;
	COleDateTime m_vHistory;
	afx_msg void OnBnClickedButtonHistoryOk();
	CListBox m_listHistory;
	CProgressCtrl m_progressHistory;
};
