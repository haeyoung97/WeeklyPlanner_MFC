// HistoryView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WeeklyPlanner.h"
#include "HistoryView.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"
#include "MainFrm.h"


// CHistoryView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistoryView, CDialogEx)

CHistoryView::CHistoryView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HISTORY, pParent)
	, m_vHistory(COleDateTime::GetCurrentTime())
{
	m_arrayHistoryCheck[0] = &m_historyCheck1;
	m_arrayHistoryCheck[1] = &m_historyCheck2;
	m_arrayHistoryCheck[2] = &m_historyCheck3;
	m_arrayHistoryCheck[3] = &m_historyCheck4;
	m_arrayHistoryCheck[4] = &m_historyCheck5;
	m_arrayHistoryCheck[5] = &m_historyCheck6;
	m_arrayHistoryCheck[6] = &m_historyCheck7;
	m_arrayHistoryCheck[7] = &m_historyCheck8;
	
}

CHistoryView::~CHistoryView()
{
}

void CHistoryView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HISTORY, m_vHistory);
	DDX_Control(pDX, IDC_PROGRESS_HISTORY, m_progressHistory);
	DDX_Control(pDX, IDC_STATIC_CHECK1, m_historyCheck1);
	DDX_Control(pDX, IDC_STATIC_CHECK2, m_historyCheck2);
	DDX_Control(pDX, IDC_STATIC_CHECK3, m_historyCheck3);
	DDX_Control(pDX, IDC_STATIC_CHECK4, m_historyCheck4);
	DDX_Control(pDX, IDC_STATIC_CHECK5, m_historyCheck5);
	DDX_Control(pDX, IDC_STATIC_CHECK6, m_historyCheck6);
	DDX_Control(pDX, IDC_STATIC_CHECK7, m_historyCheck7);
	DDX_Control(pDX, IDC_STATIC_CHECK8, m_historyCheck8);
	DDX_Control(pDX, IDC_STATIC_PERECENT, m_historyPercent);
}


BEGIN_MESSAGE_MAP(CHistoryView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_OK, &CHistoryView::OnBnClickedButtonHistoryOk)
	ON_WM_PAINT()
//	ON_WM_DWMCOLORIZATIONCOLORCHANGED()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CHistoryView 메시지 처리기입니다.


void CHistoryView::OnBnClickedButtonHistoryOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 이렇게 하면 선택한 날짜를 int형으로 받아옴. 
	UpdateData();
	m_progressHistory.SetRange(0, 1000);
	int Year = m_vHistory.GetYear();
	int Month = m_vHistory.GetMonth();
	int day = m_vHistory.GetDay();
	CString strToday, strTomorrow;
	

	if (m_vHistory.GetDay() < 10) {
		if (m_vHistory.GetMonth() < 10) {
			strToday.Format(L"%d-0%d-0%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay());
			if (m_vHistory.GetDay() == 9)
				strTomorrow.Format(L"%d-0%d-%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay() + 1);
			else
				strTomorrow.Format(L"%d-0%d-0%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay() + 1);
		}
		else {
			strToday.Format(L"%d-%d-0%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay());
			if (m_vHistory.GetDay() == 9)
				strTomorrow.Format(L"%d-%d-%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay() + 1);
			else
				strTomorrow.Format(L"%d-%d-0%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay() + 1);
		}
	}
	else {
		strToday.Format(L"%d-%d-%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay());
		strTomorrow.Format(L"%d-%d-%d", m_vHistory.GetYear(), m_vHistory.GetMonth(), m_vHistory.GetDay() + 1);
	}

	// 여기에서 디비 불러온 걸 리스트 박스에 집어넣으면 됩니다.
	// 달성도 부분에서 프로그레스바는 그 체크 했는지 안했는지 부분 보고 계산?

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	pView->m_odbc->LoadHistoryTodolist(strToday, strTomorrow);
	
	Invalidate();
}


BOOL CHistoryView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_HISTORY));

	GetObject(m_hBitmap, sizeof(BITMAP), &m_hBit);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHistoryView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	HDC hMemDC = CreateCompatibleDC(dc);
	// 호환DC에 비트맵을 선정.
	SelectObject(hMemDC, m_hBitmap);
	// 메모리 DC에서 스크린 DC로 이미지 복사
	BitBlt(dc, 0, 0, m_hBit.bmWidth, m_hBit.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// 메모리 DC삭제
	DeleteDC(hMemDC);

}


//void CHistoryView::OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity)
//{
//	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
//	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CDialogEx::OnColorizationColorChanged(dwColorizationColor, bOpacity);
//}


HBRUSH CHistoryView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
