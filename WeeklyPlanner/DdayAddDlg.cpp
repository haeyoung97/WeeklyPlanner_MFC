// DdayAddDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "MainFrm.h"
#include "WeeklyPlanner.h"
#include "DdayAddDlg.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"

// CDdayAddDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDdayAddDlg, CDialogEx)

CDdayAddDlg::CDdayAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DDAY_ADD_DIALOG, pParent)
	, pView(NULL)
{

}

CDdayAddDlg::~CDdayAddDlg()
{
}

void CDdayAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnDdayOK);
	DDX_Control(pDX, IDC_EDIT_DDAY_TITLE, m_editNewDdayTitle);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_ctrlDdayCal);
}


BEGIN_MESSAGE_MAP(CDdayAddDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_DDAY_MEMO, &CDdayAddDlg::OnEnChangeEditDdayMemo)
	ON_BN_CLICKED(IDOK, &CDdayAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDdayAddDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDdayAddDlg 메시지 처리기입니다.




//BOOL CDdayAddDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
//	{
//		if ((GetDlgItem(IDC_EDIT_DDAY_MEMO) == GetFocus()))
//		{
//			CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_DDAY_MEMO);
//			int nLen = edit->GetWindowTextLength();
//			edit->SetSel(nLen, nLen);
//			edit->ReplaceSel(_T("\r\n"));
//		}
//	}
//
//
//	return CDdayAddDlg::PreTranslateMessage(pMsg);
//}


void CDdayAddDlg::OnEnChangeEditDdayMemo()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CDdayAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDOK)->SendMessage(WM_KILLFOCUS, NULL);
	CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
	pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	m_hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_DDAY));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_hBit);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDdayAddDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	//pView 통해 WeeklyPlannerView의 m_timeNewDday, m_strNewDdayTitle, m_strNewDdayMemo에  값 전달
	CTime tSelected, nowTime;
	CString strDdayTitle, strDday, strTmp, strforQ;
	CListCtrl* ctrlDdayList = &(pView->m_ctrlDdayList);

	int nCount =ctrlDdayList->GetItemCount()+1;
	int index;
	nowTime = CTime::GetCurrentTime();
	m_ctrlDdayCal.GetCurSel(tSelected);
	strDdayTitle.Empty();
	strDday.Empty();
	strTmp.Empty();

	
	CTimeSpan dDay = tSelected - nowTime;
	//*******************
	int m_dDay = dDay.GetTotalSeconds();
	if (dDay > 0)
		strDday.Format(_T("D-%d"), abs(m_dDay / (24 * 60 * 60) + 1));
	else if(abs(m_dDay / (24 * 60 * 60)) == 0)
		strDday.Format(_T("D-Day"));
	else
		strDday.Format(_T("D+%d"), abs(m_dDay / (24 * 60 * 60)));
	pView->m_strNewDdayDate = strDday;
	//********************

	strTmp = tSelected.Format(_T("%Y-%m-%d"));
	pView->m_timeNewDday = strTmp;

	m_editNewDdayTitle.GetWindowText(strDdayTitle);
	pView->m_strNewDdayTitle = strDdayTitle;
	
	index = ctrlDdayList->InsertItem(nCount, strTmp);
	//ctrlDdayList->SetItem(index, 0, LVIF_TEXT, strDday, 0, 0, 0, NULL);
	ctrlDdayList->SetItem(index, 1, LVIF_TEXT, strDdayTitle, 0, 0, 0, NULL);
	ctrlDdayList->SetItem(index, 2, LVIF_TEXT, strDday, 0, 0, 0, NULL);
	////////////////////////////////////////////////////////////

	strforQ.Format(L"INSERT INTO dday VALUES ('%s', '%s')", strTmp, strDdayTitle);

	SQLHSTMT h_statement_forDday;


	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, pView->m_odbc->GetMh_odbc(), &h_statement_forDday)) {
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		RETCODE ret = SQLExecDirect(h_statement_forDday, (SQLWCHAR*)(const wchar_t *)strforQ, SQL_NTS);

		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		}

		SQLEndTran(SQL_HANDLE_ENV, pView->m_odbc->GetMh_odbc(), SQL_COMMIT);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forDday);

	}

	CDialogEx::OnOK();


}


BOOL CDdayAddDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{

		if ((GetDlgItem(IDC_EDIT_DDAY_MEMO) == GetFocus()))
		{
			CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_DDAY_MEMO);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
			return true;
		}


	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDdayAddDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


//void CDdayAddDlg::OnDestroy()
//{
//	CDialogEx::OnDestroy();
//
//	pView->UpdateData(TRUE);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//}


void CDdayAddDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
					   // 스크린과 호환되는 DC생성.
	HDC hMemDC = CreateCompatibleDC(dc);
	// 호환DC에 비트맵을 선정.
	SelectObject(hMemDC, m_hBitmap);
	// 메모리 DC에서 스크린 DC로 이미지 복사
	BitBlt(dc, 0, 0, m_hBit.bmWidth, m_hBit.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// 메모리 DC삭제
	DeleteDC(hMemDC);

}


HBRUSH CDdayAddDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nCtlColor)
	{
	case (CTLCOLOR_STATIC):
	{
		pDC->SetBkColor(RGB(255, 255, 255));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	}


	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
