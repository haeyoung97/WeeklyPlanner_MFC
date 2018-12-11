// HistoryView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WeeklyPlanner.h"
#include "HistoryView.h"
#include "afxdialogex.h"
#include "WeeklyPlannerView.h"
#include "MainFrm.h"


// CHistoryView ��ȭ �����Դϴ�.

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
END_MESSAGE_MAP()


// CHistoryView �޽��� ó�����Դϴ�.


void CHistoryView::OnBnClickedButtonHistoryOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// �̷��� �ϸ� ������ ��¥�� int������ �޾ƿ�. 
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
	AfxMessageBox(strToday);

	// ���⿡�� ��� �ҷ��� �� ����Ʈ �ڽ��� ��������� �˴ϴ�.
	// �޼��� �κп��� ���α׷����ٴ� �� üũ �ߴ��� ���ߴ��� �κ� ���� ���?

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	pView->m_odbc->LoadHistoryTodolist(strToday, strTomorrow);
	
}