
// WeeklyPlannerView.cpp : CWeeklyPlannerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WeeklyPlanner.h"
#endif

#include "WeeklyPlannerDoc.h"
#include "WeeklyPlannerView.h"

#include "DdayAddDlg.h"

//#include "ProfileModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWeeklyPlannerView

IMPLEMENT_DYNCREATE(CWeeklyPlannerView, CFormView)

BEGIN_MESSAGE_MAP(CWeeklyPlannerView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_MESSAGE_MODIFY_BUTTON, &CWeeklyPlannerView::OnClickedMessageModifyButton)
	ON_EN_CHANGE(IDC_MESSAGE, &CWeeklyPlannerView::OnEnChangeMessage)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX1, &CWeeklyPlannerView::OnClickedTodoCheckbox1)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX2, &CWeeklyPlannerView::OnClickedTodoCheckbox2)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX3, &CWeeklyPlannerView::OnClickedTodoCheckbox3)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX4, &CWeeklyPlannerView::OnClickedTodoCheckbox4)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX5, &CWeeklyPlannerView::OnClickedTodoCheckbox5)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX6, &CWeeklyPlannerView::OnClickedTodoCheckbox6)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX7, &CWeeklyPlannerView::OnClickedTodoCheckbox7)
	ON_BN_CLICKED(IDC_TODO_CHECKBOX8, &CWeeklyPlannerView::OnClickedTodoCheckbox8)
	ON_EN_CHANGE(IDC_ADD_TODO_MEMO, &CWeeklyPlannerView::OnEnChangeAddTodoMemo)
	ON_BN_CLICKED(IDC_ADD_TODO_BUTTON, &CWeeklyPlannerView::OnBnClickedAddTodoButton)
	ON_BN_CLICKED(IDC_DDAY_ADD_BUTTON, &CWeeklyPlannerView::OnClickedDdayAddButton)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TODO_START, &CWeeklyPlannerView::OnDtnDatetimechangeTodoStart)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TODO_END, &CWeeklyPlannerView::OnDtnDatetimechangeTodoEnd)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON1, &CWeeklyPlannerView::OnClickedTodoDeleteButton1)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON2, &CWeeklyPlannerView::OnClickedTodoDeleteButton2)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON3, &CWeeklyPlannerView::OnClickedTodoDeleteButton3)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON4, &CWeeklyPlannerView::OnClickedTodoDeleteButton4)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON5, &CWeeklyPlannerView::OnClickedTodoDeleteButton5)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON6, &CWeeklyPlannerView::OnClickedTodoDeleteButton6)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON7, &CWeeklyPlannerView::OnClickedTodoDeleteButton7)
	ON_BN_CLICKED(IDC_TODO_DELETE_BUTTON8, &CWeeklyPlannerView::OnClickedTodoDeleteButton8)
	ON_BN_CLICKED(IDC_BUTTON_PROFILE_OPEN, &CWeeklyPlannerView::OnBnClickedButtonProfileOpen)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PROFILE_DELETE, &CWeeklyPlannerView::OnClickedButtonProfileDelete)
END_MESSAGE_MAP()

// CWeeklyPlannerView 생성/소멸

CWeeklyPlannerView::CWeeklyPlannerView()
	: CFormView(IDD_WEEKLYPLANNER_FORM)
	, m_bModifyBtn(false)
	, m_nTodoDone(0)
	, m_todoEnd(0)
	, m_checkCnt(0)
	, m_strProfilePath(_T(""))
	, m_timeNewDday(0)
	, m_strNewDdayTitle(_T(""))
	, m_strNewDdayMemo(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.

	m_bChecked[8] = { true };
	m_arrayTodoCheck[0] = &m_TodoCheck1;
	m_arrayTodoCheck[1] = &m_TodoCheck2;
	m_arrayTodoCheck[2] = &m_TodoCheck3;
	m_arrayTodoCheck[3] = &m_TodoCheck4;
	m_arrayTodoCheck[4] = &m_TodoCheck5;
	m_arrayTodoCheck[5] = &m_TodoCheck6;
	m_arrayTodoCheck[6] = &m_TodoCheck7;
	m_arrayTodoCheck[7] = &m_TodoCheck8;
}

CWeeklyPlannerView::~CWeeklyPlannerView()
{
}

void CWeeklyPlannerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_MODIFY_BUTTON, m_btnMessagemodify);
	DDX_Control(pDX, IDC_MESSAGE, m_EditMessage);
	DDX_Control(pDX, IDC_PROFILE_PHOTO, m_ProfilePhoto);
	DDX_Control(pDX, IDC_TODO_CHECKBOX1, m_TodoCheck1);
	DDX_Control(pDX, IDC_TODO_ACHIVE_PROGRESS, m_TodoAchivePrgs);
	DDX_Control(pDX, IDC_TODO_CHECKBOX2, m_TodoCheck2);
	DDX_Control(pDX, IDC_TODO_CHECKBOX3, m_TodoCheck3);
	DDX_Control(pDX, IDC_TODO_CHECKBOX4, m_TodoCheck4);
	DDX_Control(pDX, IDC_TODO_CHECKBOX5, m_TodoCheck5);
	DDX_Control(pDX, IDC_TODO_CHECKBOX6, m_TodoCheck6);
	DDX_Control(pDX, IDC_TODO_CHECKBOX7, m_TodoCheck7);
	DDX_Control(pDX, IDC_TODO_CHECKBOX8, m_TodoCheck8);
	DDX_Control(pDX, IDC_ADD_TODO_BUTTON, m_btnaddTodo);
	//  DDX_Control(pDX, IDC_DDAY_ADD_BUTTON, m_btnaddDday);
	DDX_DateTimeCtrl(pDX, IDC_TODO_START, m_todoStart);
	DDX_DateTimeCtrl(pDX, IDC_TODO_END, m_todoEnd);
	DDX_Control(pDX, IDC_TODO_START, m_cTodoStart);
	DDX_Control(pDX, IDC_TODO_END, m_cTodoEnd);
	DDX_Control(pDX, IDC_DDAY_LIST_CNTL, m_ctrlDdayList);
	DDX_Control(pDX, IDC_DDAY_DELETE_BUTTON, m_btnDeleteDday);
	DDX_Control(pDX, IDC_DDAY_ADD_BUTTON, m_btnAddDday);
	DDX_Control(pDX, IDC_DDAY_MODIFY_BUTTON, m_btnModifyDday);
}

BOOL CWeeklyPlannerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CWeeklyPlannerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_TodoAchivePrgs.SetRange(0, 1000);
	m_cTodoStart.SetFormat(_T("tt  HH: mm"));
	m_cTodoEnd.SetFormat(_T("tt  HH: mm"));
	// set using the SYSTEMTIME
	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	sysTime.wYear = 1998;
	sysTime.wMonth = 4;
	sysTime.wDay = 3;
	VERIFY(m_cTodoStart.SetTime(&sysTime));
	VERIFY(m_cTodoEnd.SetTime(&sysTime));


	/*CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "주소값을 읽어올 수 없습니다.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 180, 180, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);*/


	//List Control 설정
	CRect list_rect;
	m_ctrlDdayList.GetWindowRect(&list_rect);
	m_ctrlDdayList.EnableScrollBar(FALSE);
	LPWSTR list[3] = { _T("D"), _T("제목"), _T("DAY") };
	double nWidth[3] = { list_rect.Width()*0.2, 0, list_rect.Width()*0.3 };
	nWidth[1] = list_rect.Width() - nWidth[0] - nWidth[2]-3;
	m_ctrlDdayList.GetHeaderCtrl()->EnableWindow(false);

	//List Column 설정
	for (int i = 0; i < 3; i++) {
		m_ctrlDdayList.InsertColumn(i, list[i], LVCFMT_CENTER, nWidth[i]);
	}
	m_ctrlDdayList.SetExtendedStyle(m_ctrlDdayList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	((CButton*)GetDlgItem(IDC_DDAY_DELETE_BUTTON))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_DDAY_MODIFY_BUTTON))->EnableWindow(FALSE);



}

void CWeeklyPlannerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWeeklyPlannerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWeeklyPlannerView 진단

#ifdef _DEBUG
void CWeeklyPlannerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWeeklyPlannerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeeklyPlannerDoc)));
	return (CWeeklyPlannerDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeeklyPlannerView 메시지 처리기


void CWeeklyPlannerView::OnClickedMessageModifyButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bModifyBtn) { //수정 일때
		m_EditMessage.EnableWindow(true);
		m_btnMessagemodify.SetWindowText(_T("적용"));
		m_bModifyBtn = true;

	}
	else {
		m_EditMessage.EnableWindow(false);
		m_btnMessagemodify.SetWindowText(_T("수정"));

		m_bModifyBtn = false;
	}

}


void CWeeklyPlannerView::OnEnChangeMessage()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CWeeklyPlannerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{

		if ((GetDlgItem(IDC_MESSAGE) == GetFocus()))
		{
			CEdit* edit = (CEdit*)GetDlgItem(IDC_MESSAGE);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
		}
		if ((GetDlgItem(IDC_ADD_TODO_MEMO) == GetFocus())) {
			CEdit* edit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));
		}

	}

	return CFormView::PreTranslateMessage(pMsg);
}




void CWeeklyPlannerView::OnClickedTodoCheckbox1()
{
	if (m_TodoCheck1.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck1.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck1, 0);
}



void CWeeklyPlannerView::OnClickedTodoCheckbox2()
{
	if (m_TodoCheck2.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck2.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck2, 1);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox3()
{
	if (m_TodoCheck3.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck3.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck3, 2);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox4()
{
	if (m_TodoCheck4.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck4.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck4, 3);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox5()
{
	if (m_TodoCheck5.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck5.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck5, 4);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox6()
{
	if (m_TodoCheck6.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck6.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck6, 5);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox7()
{
	if (m_TodoCheck7.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck7.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck7, 6);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox8()
{
	if (m_TodoCheck8.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck8.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck8, 7);
}

void CWeeklyPlannerView::UpdateTodoProgressBar(CButton* m_checkBtn, int index)
{
	int i = 8, j = 0, cnt = 0;
	while (i--) {
		if (!m_bChecked[j])
			cnt++;
		j++;
	}
	if (cnt != 0) {
		int percent = 1000 / cnt;
		int remainder = 1000 - percent*(cnt - 1);
		if (m_checkCnt-1 != 0) {
			m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.SetPos(0)*m_checkCnt + percent);
		}
		if (cnt == m_checkCnt) {
			if (m_checkBtn->GetCheck() == BST_CHECKED && !m_bChecked[index]) {
				m_nTodoDone++;
				m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.GetPos() + remainder);
			}
			else if (m_checkBtn->GetCheck() == BST_UNCHECKED && !m_bChecked[index]) {
				m_nTodoDone--;
				m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.GetPos() - remainder);
			}
		}
		else {
			if (m_checkBtn->GetCheck() == BST_CHECKED && !m_bChecked[index]) {
				m_nTodoDone++;
				m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.GetPos() + percent);
			}
			else if (m_checkBtn->GetCheck() == BST_UNCHECKED && !m_bChecked[index]) {
				m_nTodoDone--;
				m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.GetPos() - percent);
			}
		}
	}
	else {
		m_TodoAchivePrgs.SetPos(m_TodoAchivePrgs.SetPos(0));
	}
}





void CWeeklyPlannerView::OnEnChangeAddTodoMemo()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CWeeklyPlannerView::OnBnClickedAddTodoButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str, strStart, strEnd, m_strTodomemo;
	GetDlgItemText(IDC_ADD_TODO_MEMO, m_strTodomemo);
	if (m_todoStart.GetMinute() < 10)
		strStart.Format(_T("%d:0%d"), m_todoStart.GetHour(), m_todoStart.GetMinute());
	else
		strStart.Format(_T("%d:%d"), m_todoStart.GetHour(), m_todoStart.GetMinute());
	if (m_todoEnd.GetMinute() < 10)
		strEnd.Format(_T("%d:0%d"), m_todoEnd.GetHour(), m_todoEnd.GetMinute());
	else
		strEnd.Format(_T("%d:%d"), m_todoEnd.GetHour(), m_todoEnd.GetMinute());
	str += strStart; str += " ~ "; str += strEnd;
	str += "  :  ";
	str += m_strTodomemo;

	int i = 0;
	while (1) {
		if (m_bChecked[i]) {
			m_bChecked[i] = false;
			break;
		}
		if (i == 8) {
			break;
		}
		i++;
	}

	if (i <= 7) {
		m_arrayTodoCheck[i]->SetWindowText(str);
		m_arrayTodoCheck[i]->EnableWindow(true);
		UpdateTodoProgressBar(m_arrayTodoCheck[i], i);
	}
	else
		AfxMessageBox(_T("일정을 추가할 수 없습니다."));

	//버튼 클릭 후 공간 비우기
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
	pEdit->SetWindowText(_T(""));

	// DB에 올리기
}


void CWeeklyPlannerView::OnClickedDdayAddButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CDdayAddDlg m_ddayaddDlg;
	m_ddayaddDlg.DoModal();
}


void CWeeklyPlannerView::OnDtnDatetimechangeTodoStart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	*pResult = 0;
}


void CWeeklyPlannerView::OnDtnDatetimechangeTodoEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	*pResult = 0;
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[0] = true;
	MoveCheckboxStr(1);
	m_checkCnt--;
	m_arrayTodoCheck[7]->SetWindowText(_T("_______________________________"));
	m_bChecked[7] = true;
	m_arrayTodoCheck[7]->EnableWindow(false);
	UpdateTodoProgressBar(&m_TodoCheck1, 0);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[1] = true;
	MoveCheckboxStr(2);
	UpdateTodoProgressBar(&m_TodoCheck2, 1);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[2] = true;
	MoveCheckboxStr(3);
	UpdateTodoProgressBar(&m_TodoCheck3, 2);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[3] = true;
	MoveCheckboxStr(4);
	UpdateTodoProgressBar(&m_TodoCheck4, 3);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[4] = true;
	MoveCheckboxStr(5);
	UpdateTodoProgressBar(&m_TodoCheck5, 4);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[5] = true;
	MoveCheckboxStr(6);
	UpdateTodoProgressBar(&m_TodoCheck6, 5);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[6] = true;
	MoveCheckboxStr(7);
	UpdateTodoProgressBar(&m_TodoCheck7, 6);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bChecked[7] = true;
	m_checkCnt--;
	m_TodoCheck8.SetWindowText(_T("_______________________________"));
	m_arrayTodoCheck[7]->SetCheck(false);
	m_arrayTodoCheck[7]->EnableWindow(false);
	UpdateTodoProgressBar(&m_TodoCheck8, 7);
}

void CWeeklyPlannerView::MoveCheckboxStr(int i)
{
	while (i<8) {
		CString str;
		m_arrayTodoCheck[i]->GetWindowText(str);
		m_arrayTodoCheck[i - 1]->SetWindowText(str);
		m_bChecked[i - 1] = m_bChecked[i];
		if (m_arrayTodoCheck[i]->GetCheck() == BST_UNCHECKED)
			m_arrayTodoCheck[i - 1]->SetCheck(false);
		else if (m_arrayTodoCheck[i]->GetCheck() == BST_CHECKED)
			m_arrayTodoCheck[i - 1]->SetCheck(true);
		boolean isEnabled = m_arrayTodoCheck[i]->IsWindowEnabled();
		m_arrayTodoCheck[i - 1]->EnableWindow(isEnabled);
		i++;
	}
	m_checkCnt--;
	m_arrayTodoCheck[7]->SetWindowText(_T("_______________________________"));
	m_bChecked[7] = true;
	m_arrayTodoCheck[7]->EnableWindow(false);
}

void CWeeklyPlannerView::OnBnClickedButtonProfileOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//dimx, dimy : image size;
	int dimx = 150;
	int dimy = 160;
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.bmp, *.jpg, *.png)|*.bmp; *.jpg; *.png|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		m_strProfilePath = dlg.GetPathName();

		CImage profileImage;
		profileImage.Load(m_strProfilePath);

		CDC *screenDC = GetDC();
		CDC mDC;
		mDC.CreateCompatibleDC(screenDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(screenDC, dimx, dimy);
		CBitmap *bmp = mDC.SelectObject(&bitmap);
		mDC.SetStretchBltMode(HALFTONE);
		profileImage.StretchBlt(mDC.m_hDC, 0, 0, dimx, dimy, 0, 0, profileImage.GetWidth(), profileImage.GetHeight(), SRCCOPY);
		mDC.SelectObject(bmp);

		((CStatic*)GetDlgItem(IDC_PROFILE_PHOTO))->SetBitmap((HBITMAP)bitmap.Detach());
		ReleaseDC(screenDC);

		//CImage img;
		//img.Load(_T("res\\1.png"));
		//img.BItBlt(pDC, x, y);

	}
}

void CWeeklyPlannerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	//프로필 디폴트 등록
	CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "주소값을 읽어올 수 없습니다.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 140, 140, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
}



void CWeeklyPlannerView::OnClickedButtonProfileDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "주소값을 읽어올 수 없습니다.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 140, 140, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
}
