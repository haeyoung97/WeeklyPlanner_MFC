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
	COleDateTime m_vHistory;
	afx_msg void OnBnClickedButtonHistoryOk();
	CProgressCtrl m_progressHistory;
	CStatic m_historyCheck1;
	CStatic m_historyCheck2;
	CStatic m_historyCheck3;
	CStatic m_historyCheck4;
	CStatic m_historyCheck5;
	CStatic m_historyCheck6;
	CStatic m_historyCheck7;
	CStatic m_historyCheck8;
	CStatic m_historyPercent;
	CStatic* m_arrayHistoryCheck[8];
};
