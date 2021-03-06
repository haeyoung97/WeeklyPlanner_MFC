#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "WeeklyPlannerView.h"


// CDdayAddDlg 대화 상자입니다.

class CDdayAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDdayAddDlg)

public:
	CDdayAddDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDdayAddDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DDAY_ADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	CEdit m_editDdayMemo;
	afx_msg void OnEnChangeEditDdayMemo();
	virtual BOOL OnInitDialog();
	CButton m_btnDdayOK;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	CWeeklyPlannerView* pView;
	CEdit m_editNewDdayTitle;
	CEdit m_editNewDdayMemo;
	CMonthCalCtrl m_ctrlDdayCal;
	afx_msg void OnPaint();
	HBITMAP m_hBitmap;
	BITMAP m_hBit;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
