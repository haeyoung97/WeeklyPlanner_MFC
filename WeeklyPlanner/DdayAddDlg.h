#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "WeeklyPlannerView.h"
#include "atltime.h"


// CDdayAddDlg ��ȭ �����Դϴ�.

class CDdayAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDdayAddDlg)

public:
	CDdayAddDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDdayAddDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DDAY_ADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_editDdayMemo;
	afx_msg void OnEnChangeEditDdayMemo();
	virtual BOOL OnInitDialog();
	CButton m_btnDdayOK;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnMcnSelchangeMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult);
	CMonthCalCtrl m_ctrlDdayCal;
	afx_msg void OnMcnSelchangeMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult);
	CWeeklyPlannerView* pView;
	CTime tSelected;
	CTime tNow;
};
