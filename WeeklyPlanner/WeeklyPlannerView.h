
// WeeklyPlannerView.h : CWeeklyPlannerView 클래스의 인터페이스
//

#pragma once

#include "afxwin.h"
#include "atlimage.h"
#include "afxcmn.h"
#include "atltime.h"
#include "afxdtctl.h"
#include "WeeklyPlannerDoc.h"
#include "SoundPlayer.h"
#include "atltypes.h"
#include "TW_ODBC.h"
#include "HistoryView.h"


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
	TW_ODBC *m_odbc;

	CButton m_btnMessagemodify;
	afx_msg void OnClickedMessageModifyButton();
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
	afx_msg void OnClickedDdayAddButton();
	bool m_bChecked[8];
	afx_msg void OnDtnDatetimechangeTodoStart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeTodoEnd(NMHDR *pNMHDR, LRESULT *pResult);
	CTime m_todoStart;
	CTime m_todoEnd;
	afx_msg void OnClickedTodoDeleteButton1();
	afx_msg void OnClickedTodoDeleteButton2();
	afx_msg void OnClickedTodoDeleteButton3();
	afx_msg void OnClickedTodoDeleteButton4();
	afx_msg void OnClickedTodoDeleteButton5();
	afx_msg void OnClickedTodoDeleteButton6();
	afx_msg void OnClickedTodoDeleteButton7();
	afx_msg void OnClickedTodoDeleteButton8();
	CButton* m_arrayTodoCheck[8];
	CButton* m_arrayTodoBtn[8];
	int m_checkCnt;
	afx_msg void OnBnClickedButtonProfileOpen();
	CString m_strProfilePath;
	afx_msg void OnPaint();
	void MoveCheckboxStr(int i);
	CDateTimeCtrl m_cTodoStart;
	CDateTimeCtrl m_cTodoEnd;
	afx_msg void OnClickedButtonProfileDelete();
	CString m_timeNewDday;
	CString m_strNewDdayTitle;
	CString m_strNewDdayDate;
	CListCtrl m_ctrlDdayList;
	CButton m_btnDeleteDday;
	CButton m_btnAddDday;
	afx_msg void OnLvnItemchangedDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInsertitemDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDdayDeleteButton();
	bool m_bModifyBtn;
	int m_nDdayListSelectedItem;
	afx_msg void OnClickDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult);
	int percent;
	CButton m_TodoBtn1;
	CButton m_TodoBtn2;
	CButton m_TodoBtn3;
	CButton m_TodoBtn4;
	CButton m_TodoBtn5;
	CButton m_TodoBtn6;
	CButton m_TodoBtn7;
	CButton m_TodoBtn8;
	afx_msg void OnClickedPauseSong();
	afx_msg void OnClickedPrevSong();
	afx_msg void OnClickedNextSong();
	int m_nPlayPause;
	int m_nPlayIndex;
	CButton m_btnPlayPause;
	CSoundPlayer m_soundSP;
	CListCtrl m_soundPlayList;
	CStatic m_strSongName;
	afx_msg void OnBnClickedSongs();
	afx_msg void OnBnClickedButtonSoundDelete();
	CRect winRect;
	int m_nSizeProfileX;
	int m_nSizeProfileY;
	// todo에 올라간 일정 수
	int m_nTodoCnt;
	CWeeklyPlannerView* pView;
	CString strToday;
	CString strTomorrow;
	CStatic* m_pDefaultPicture;
	CString m_strDefaultImagePath;
	afx_msg void OnHistoryView();
	HACCEL m_hAccelTable;
	CHistoryView view;
	CString m_strOldPath;
	CString m_strProfileMessage;
	afx_msg void OnDestroy();
//	CFont m_titleFont;
	CFont m_titleFont;
	CString SoundPath[50];
	CString SoundName[50];
};

#ifndef _DEBUG  // WeeklyPlannerView.cpp의 디버그 버전
inline CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const
   { return reinterpret_cast<CWeeklyPlannerDoc*>(m_pDocument); }
#endif

