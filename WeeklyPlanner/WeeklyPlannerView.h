
// WeeklyPlannerView.h : CWeeklyPlannerView 클래스의 인터페이스
//

#pragma once
#include "afxwin.h"
#include "atlimage.h"
#include "afxcmn.h"
#include "atltime.h"
#include "afxdtctl.h"

class CWeeklyPlannerView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CWeeklyPlannerView();
	DECLARE_DYNCREATE(CWeeklyPlannerView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_WEEKLYPLANNER_FORM };
#endif

// 특성입니다.
public:
	CWeeklyPlannerDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CWeeklyPlannerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnMessagemodify;
	afx_msg void OnClickedMessageModifyButton();
	bool m_bModifyBtn;
	CEdit m_EditMessage;
	afx_msg void OnEnChangeMessage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_ProfilePhoto;
	CImage m_ProfileImg;
	afx_msg void OnClickedTodoCheckbox1();
	int m_nTodoDone;
	CButton m_TodoCheck1;
	CProgressCtrl m_TodoAchivePrgs;
	void UpdateTodoProgressBar(CButton* m_checkBtn);
	CButton m_TodoCheck2;
	CButton m_TodoCheck3;
	CButton m_TodoCheck4;
	CButton m_TodoCheck5;
	CButton m_TodoCheck6;
	CButton m_TodoCheck7;
	CButton m_TodoCheck8;
	afx_msg void OnClickedTodoCheckbox2();
	afx_msg void OnClickedTodoCheckbox3();
	afx_msg void OnClickedTodoCheckbox4();
	afx_msg void OnClickedTodoCheckbox5();
	afx_msg void OnClickedTodoCheckbox6();
	afx_msg void OnClickedTodoCheckbox7();
	afx_msg void OnClickedTodoCheckbox8();
	CButton m_btnaddTodo;
	afx_msg void OnEnChangeAddTodoMemo();
	afx_msg void OnBnClickedAddTodoButton();
	CButton m_btnaddDday;
	afx_msg void OnClickedDdayAddButton();
	CDateTimeCtrl m_todoStart;
	CDateTimeCtrl m_todoEnd;
	bool m_bChecked[8];
	afx_msg void OnDtnDatetimechangeTodoStart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeTodoEnd(NMHDR *pNMHDR, LRESULT *pResult);
//	CEdit m_Todomemo;
};

#ifndef _DEBUG  // WeeklyPlannerView.cpp의 디버그 버전
inline CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const
   { return reinterpret_cast<CWeeklyPlannerDoc*>(m_pDocument); }
#endif

