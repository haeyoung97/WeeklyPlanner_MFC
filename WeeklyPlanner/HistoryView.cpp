// HistoryView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WeeklyPlanner.h"
#include "HistoryView.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"


// CHistoryView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistoryView, CDialogEx)

CHistoryView::CHistoryView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HISTORY, pParent)
	, m_vHistory(COleDateTime::GetCurrentTime())
{
		
}

CHistoryView::~CHistoryView()
{
}

void CHistoryView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_HISTORY, m_ctrlHistory);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HISTORY, m_vHistory);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_HISTORY, m_ctrlHistory);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_HISTORY, m_vHistory);
	DDX_Control(pDX, IDC_LIST_HISTORY, m_listHistory);
	DDX_Control(pDX, IDC_PROGRESS_HISTORY, m_progressHistory);
}


BEGIN_MESSAGE_MAP(CHistoryView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_OK, &CHistoryView::OnBnClickedButtonHistoryOk)
END_MESSAGE_MAP()


// CHistoryView 메시지 처리기입니다.


void CHistoryView::OnBnClickedButtonHistoryOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 이렇게 하면 선택한 날짜를 int형으로 받아옴. 
	UpdateData();
	int Year = m_vHistory.GetYear();
	int Month = m_vHistory.GetMonth();
	int day = m_vHistory.GetDay();

	// CString으로 변환인데 필요 없으면 지워도 됨.
	CString date;
	date.Format(_T("%d-%d-%d"), Year, Month, day);

	// 여기에서 디비 불러온 걸 리스트 박스에 집어넣으면 됩니다.
	// 달성도 부분에서 프로그레스바는 그 체크 했는지 안했는지 부분 보고 계산?



	//m_listHistory.AddString(date);
	UpdateData(FALSE);

}
