
// WeeklyPlannerView.cpp : CWeeklyPlannerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.

#include "WeeklyPlannerDoc.h"
#include "WeeklyPlannerView.h"
#include "SoundPlayer.h"
#include "DdayAddDlg.h"

#ifndef SHARED_HANDLERS
#include "WeeklyPlanner.h"
#endif

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
ON_NOTIFY(LVN_ITEMCHANGED, IDC_DDAY_LIST_CNTL, &CWeeklyPlannerView::OnLvnItemchangedDdayListCntl)
ON_NOTIFY(LVN_INSERTITEM, IDC_DDAY_LIST_CNTL, &CWeeklyPlannerView::OnInsertitemDdayListCntl)
ON_BN_CLICKED(IDC_DDAY_DELETE_BUTTON, &CWeeklyPlannerView::OnBnClickedDdayDeleteButton)
ON_NOTIFY(NM_CLICK, IDC_DDAY_LIST_CNTL, &CWeeklyPlannerView::OnClickDdayListCntl)
ON_BN_CLICKED(IDC_PAUSE_SONG, &CWeeklyPlannerView::OnClickedPauseSong)
ON_BN_CLICKED(IDC_PREV_SONG, &CWeeklyPlannerView::OnClickedPrevSong)
ON_BN_CLICKED(IDC_NEXT_SONG, &CWeeklyPlannerView::OnClickedNextSong)
ON_BN_CLICKED(IDC_SONGS, &CWeeklyPlannerView::OnBnClickedSongs)
ON_BN_CLICKED(IDC_BUTTON_SOUND_DELETE, &CWeeklyPlannerView::OnBnClickedButtonSoundDelete)
ON_COMMAND(ID_HISTORY_VIEW, &CWeeklyPlannerView::OnHistoryView)
ON_WM_DESTROY()
END_MESSAGE_MAP()

// CWeeklyPlannerView 생성/소멸

CWeeklyPlannerView::CWeeklyPlannerView()
	: CFormView(IDD_WEEKLYPLANNER_FORM)
	, m_nTodoDone(0)
	, m_todoEnd(0)
	, m_checkCnt(0)
	, m_strProfilePath(_T(""))
	, m_timeNewDday(_T(""))
	, m_strNewDdayTitle(_T(""))
	, m_strNewDdayDate(_T(""))
	, m_bModifyBtn(false)
	, m_nDdayListSelectedItem(0)
	, percent(1000)
	, m_nPlayPause(0)
	, m_nPlayIndex(0)
	, m_nSizeProfileX(0)
	, m_nSizeProfileY(0)
	, m_nTodoCnt(0)
	, pView(NULL)
	, strToday(_T(""))
	, strTomorrow(_T(""))
	, view(NULL)
	, m_strOldPath(_T(""))
	, m_strProfileMessage(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_odbc = new TW_ODBC();
	//
	m_bChecked[8] = { true };
	//
	m_arrayTodoCheck[0] = &m_TodoCheck1;
	m_arrayTodoCheck[1] = &m_TodoCheck2;
	m_arrayTodoCheck[2] = &m_TodoCheck3;
	m_arrayTodoCheck[3] = &m_TodoCheck4;
	m_arrayTodoCheck[4] = &m_TodoCheck5;
	m_arrayTodoCheck[5] = &m_TodoCheck6;
	m_arrayTodoCheck[6] = &m_TodoCheck7;
	m_arrayTodoCheck[7] = &m_TodoCheck8;
	//
	m_arrayTodoBtn[0] = &m_TodoBtn1;
	m_arrayTodoBtn[1] = &m_TodoBtn2;
	m_arrayTodoBtn[2] = &m_TodoBtn3;
	m_arrayTodoBtn[3] = &m_TodoBtn4;
	m_arrayTodoBtn[4] = &m_TodoBtn5;
	m_arrayTodoBtn[5] = &m_TodoBtn6;
	m_arrayTodoBtn[6] = &m_TodoBtn7;
	m_arrayTodoBtn[7] = &m_TodoBtn8;
	
	m_titleFont.CreateFont(23, 15, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("함초롬바탕"));
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
	DDX_DateTimeCtrl(pDX, IDC_TODO_START, m_todoStart);
	DDX_DateTimeCtrl(pDX, IDC_TODO_END, m_todoEnd);
	DDX_Control(pDX, IDC_TODO_START, m_cTodoStart);
	DDX_Control(pDX, IDC_TODO_END, m_cTodoEnd);
	DDX_Control(pDX, IDC_DDAY_LIST_CNTL, m_ctrlDdayList);
	DDX_Control(pDX, IDC_DDAY_DELETE_BUTTON, m_btnDeleteDday);
	DDX_Control(pDX, IDC_DDAY_ADD_BUTTON, m_btnAddDday);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON1, m_TodoBtn1);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON2, m_TodoBtn2);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON3, m_TodoBtn3);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON4, m_TodoBtn4);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON5, m_TodoBtn5);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON6, m_TodoBtn6);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON7, m_TodoBtn7);
	DDX_Control(pDX, IDC_TODO_DELETE_BUTTON8, m_TodoBtn8);
	DDX_Control(pDX, IDC_PAUSE_SONG, m_btnPlayPause);
	DDX_Control(pDX, IDC_PLAYLIST, m_soundPlayList);
	DDX_Control(pDX, IDC_MUSIC_INFO, m_strSongName);
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
	GetDlgItem(IDC_DDAY_TITLE)->SetFont(&m_titleFont);
	GetDlgItem(IDC_TODO_TITLE)->SetFont(&m_titleFont);



	m_TodoAchivePrgs.SetRange(0, 1000);
	m_cTodoStart.SetFormat(_T("tt  HH: mm"));
	m_cTodoEnd.SetFormat(_T("tt  HH: mm"));
	// set using the SYSTEMTIME
	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	// DB에서 오늘 날짜 불러와서 변수에 저장할 수 있었으면 한다.
	// 현재 날짜 얻어오기
	CTime ctime = CTime::GetCurrentTime();
	if (ctime.GetDay() < 10) {
		if (ctime.GetMonth() < 10) {
			strToday.Format(L"%d-0%d-0%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay());
			if (ctime.GetDay() == 9)
				strTomorrow.Format(L"%d-0%d-%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() + 1);
			else
				strTomorrow.Format(L"%d-0%d-0%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() + 1);
		}
		else {
			strToday.Format(L"%d-%d-0%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay());
			if (ctime.GetDay() == 9)
				strTomorrow.Format(L"%d-%d-%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() + 1);
			else
				strTomorrow.Format(L"%d-%d-0%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() + 1);
		}
	}
	else {
		strToday.Format(L"%d-%d-%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay());
		strTomorrow.Format(L"%d-%d-%d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay() + 1);
	}
	

	sysTime.wYear = ctime.GetYear();
	sysTime.wMonth = ctime.GetMonth();
	sysTime.wDay = ctime.GetDay();
	VERIFY(m_cTodoStart.SetTime(&sysTime));
	VERIFY(m_cTodoEnd.SetTime(&sysTime));

	UpdateData(TRUE);
	/*CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "주소값을 읽어올 수 없습니다.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 180, 180, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);*/


	//List Control 설정
	CRect list_rect;
	m_ctrlDdayList.GetWindowRect(&list_rect);
	m_ctrlDdayList.EnableScrollBar(FALSE);
	LPWSTR list[3] = { _T("마감 일자"), _T("제목"),_T("남은 날짜") };
	double nWidth[3] = { list_rect.Width()*0.3, 0.0 ,list_rect.Width()*0.25};
	nWidth[1] = list_rect.Width() - nWidth[0] - nWidth[2]-3;
	m_ctrlDdayList.GetHeaderCtrl()->EnableWindow(FALSE);

	//List Column 설정
	for (int i = 0; i < 3; i++) {
		m_ctrlDdayList.InsertColumn(i, list[i], LVCFMT_CENTER, nWidth[i]);
	}
	m_ctrlDdayList.SetExtendedStyle(m_ctrlDdayList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	((CButton*)GetDlgItem(IDC_DDAY_DELETE_BUTTON))->EnableWindow(FALSE);
	//((CButton*)GetDlgItem(IDC_DDAY_MODIFY_BUTTON))->EnableWindow(FALSE);

	// 뷰 크기 얻기
	
	this->GetClientRect(winRect);
	m_nSizeProfileX = (int)winRect.right * 0.155;
	m_nSizeProfileY = (int)winRect.bottom * 0.35;

	// Ctrl+R acceleratror 등록?
	//m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_HISTORY_VIEW));

	// 프로필 이미지 DC
	m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	m_strDefaultImagePath = L"res\\default_Image.bmp";

	//SoundList 설정
	SoundPath[0] = _T("res\\MFC_SOUND\\Barroom_Ballet_Silent_Film_Light.wav");
	SoundPath[1] = _T("res\\MFC_SOUND\\Greedy.wav");
	SoundPath[2] = _T("res\\MFC_SOUND\\Merry_Go_Slower.wav");
	SoundPath[3] = _T("res\\MFC_SOUND\\Old_MacDonald.wav");
	SoundPath[4] = _T("res\\MFC_SOUND\\Ponies_and_Balloons");
	SoundPath[5] = _T("res\\MFC_SOUND\\Rainy_Day_Games");
	SoundPath[6] = _T("res\\MFC_SOUND\\Summer_Smile.wav");
	SoundPath[7] = _T("res\\MFC_SOUND\\Toy_Piano.wav");
	SoundPath[8] = _T("res\\MFC_SOUND\\Washington_Post.wav");

	SoundName[0] = _T("Barroom_Ballet_Silent_Film_Light");
	SoundName[1] = _T("Greedy");
	SoundName[2] = _T("Merry_Go_Slower");
	SoundName[3] = _T("Old_MacDonald");
	SoundName[4] = _T("Ponies_and_Balloons");
	SoundName[5] = _T("Rainy_Day_Games");
	SoundName[6] = _T("Summer_Smile");
	SoundName[7] = _T("Toy_Piano");
	SoundName[8] = _T("Washington_Post");

	// DB에서 데이터 가져오기
	m_nTodoCnt = m_odbc->ImportData(strToday, strTomorrow);


	//sound list 설정
	m_soundPlayList.InsertColumn(0, L"제목", LVCFMT_CENTER, 600);
	for (int i = 0; i < m_soundSP.m_nSoundIndex; i++)
	{
		m_soundPlayList.InsertItem(i, m_soundSP.m_strSoundName[i]);
	}

	

	UpdateTodoProgressBar(NULL);

	m_EditMessage.SendMessage(WM_KILLFOCUS, NULL);
	Invalidate();


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
		m_EditMessage.SetReadOnly(false);
		m_btnMessagemodify.SetWindowText(_T("적용"));
		m_EditMessage.SendMessage(WM_KILLFOCUS, NULL);
		m_bModifyBtn = true;

	}
	else {
		m_EditMessage.SetReadOnly(true);
		m_btnMessagemodify.SetWindowText(_T("수정"));
		//AfxMessageBox(m_strProfileMessage);
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
	//if (m_hAccelTable != NULL)
	//{
	//	if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
	//		return true;
	//}

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
	UpdateTodoProgressBar(&m_TodoCheck1);
}



void CWeeklyPlannerView::OnClickedTodoCheckbox2()
{
	if (m_TodoCheck2.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck2.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck2);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox3()
{
	if (m_TodoCheck3.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck3.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck3);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox4()
{
	if (m_TodoCheck4.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck4.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck4);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox5()
{
	if (m_TodoCheck5.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck5.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck5);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox6()
{
	if (m_TodoCheck6.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck6.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck6);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox7()
{
	if (m_TodoCheck7.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck7.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck7);
}


void CWeeklyPlannerView::OnClickedTodoCheckbox8()
{
	if (m_TodoCheck8.GetCheck() == BST_CHECKED) {
		m_checkCnt++;
	}
	else if (m_TodoCheck8.GetCheck() == BST_UNCHECKED) {
		m_checkCnt--;
	}
	UpdateTodoProgressBar(&m_TodoCheck8);
}

void CWeeklyPlannerView::UpdateTodoProgressBar(CButton* m_checkBtn)
{
	int i = 8, j = 0, cnt = 0;
	while (i--) {
		if (!m_bChecked[j])
			cnt++;
		j++;
	}
	
	if (cnt == 0 || m_checkCnt == 0) {
		m_TodoAchivePrgs.SetPos(0);
	}
	else {
		if (cnt == m_checkCnt) {
			m_TodoAchivePrgs.SetPos(1000);

		}
		else {
			percent = 1000 / cnt;
			m_TodoAchivePrgs.SetPos(m_checkCnt*percent);
		}
	}
	m_odbc->DataRemoveTodolist(strToday, strTomorrow);
	m_odbc->DataSaveTodolist();
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

	if (m_nTodoCnt <= 7) {
		m_nTodoCnt++;
		m_arrayTodoCheck[m_nTodoCnt-1]->SetWindowText(str);
		m_arrayTodoCheck[m_nTodoCnt-1]->EnableWindow(true);
		m_arrayTodoCheck[m_nTodoCnt-1]->SetCheck(0);
		m_arrayTodoBtn[m_nTodoCnt-1]->EnableWindow(true);
		m_bChecked[m_nTodoCnt - 1] = false;
		
		UpdateTodoProgressBar(m_arrayTodoCheck[m_nTodoCnt]);
	}
	else
		AfxMessageBox(_T("일정을 추가할 수 없습니다."));

	//버튼 클릭 후 공간 비우기
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
	pEdit->SetWindowText(_T(""));

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
	m_nTodoCnt--;
	m_bChecked[0] = true;
	MoveCheckboxStr(1);
	m_arrayTodoCheck[7]->SetWindowText(_T("_______________________________"));
	m_bChecked[7] = true;
	m_arrayTodoCheck[7]->EnableWindow(false);
	UpdateTodoProgressBar(&m_TodoCheck1);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[1] = true;
	MoveCheckboxStr(2);
	UpdateTodoProgressBar(&m_TodoCheck2);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[2] = true;
	MoveCheckboxStr(3);
	UpdateTodoProgressBar(&m_TodoCheck3);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[3] = true;
	MoveCheckboxStr(4);
	UpdateTodoProgressBar(&m_TodoCheck4);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[4] = true;
	MoveCheckboxStr(5);
	UpdateTodoProgressBar(&m_TodoCheck5);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[5] = true;
	MoveCheckboxStr(6);
	UpdateTodoProgressBar(&m_TodoCheck6);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[6] = true;
	MoveCheckboxStr(7);
	UpdateTodoProgressBar(&m_TodoCheck7);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTodoCnt--;
	m_bChecked[7] = true;
	m_TodoCheck8.SetWindowText(_T("_______________________________"));
	m_arrayTodoCheck[7]->SetCheck(false);
	m_arrayTodoCheck[7]->EnableWindow(false);
	m_arrayTodoBtn[7]->EnableWindow(false);
	UpdateTodoProgressBar(&m_TodoCheck8);
}

void CWeeklyPlannerView::MoveCheckboxStr(int i)
{
	if (m_arrayTodoCheck[i-1]->GetCheck() == BST_CHECKED)
		m_checkCnt--;
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
		isEnabled = m_arrayTodoBtn[i]->IsWindowEnabled();
		m_arrayTodoBtn[i - 1]->EnableWindow(isEnabled);
		i++;
	}
	m_arrayTodoCheck[7]->SetWindowText(_T("_______________________________"));
	m_bChecked[7] = true;
	m_arrayTodoCheck[7]->EnableWindow(false);
	m_arrayTodoBtn[7]->EnableWindow(false);
}

void CWeeklyPlannerView::OnBnClickedButtonProfileOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//dimx, dimy : image size;
	
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.bmp, *.jpg, *.png)|*.bmp; *.jpg; *.png|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		m_strProfilePath = dlg.GetPathName();

		CImage profileImage;
		profileImage.Load(m_strProfilePath);
		m_strOldPath = m_strDefaultImagePath;
		m_strDefaultImagePath = m_strProfilePath;

		CDC *screenDC = GetDC();
		CDC mDC;
		mDC.CreateCompatibleDC(screenDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(screenDC, m_nSizeProfileX, m_nSizeProfileY);
		CBitmap *bmp = mDC.SelectObject(&bitmap);
		mDC.SetStretchBltMode(HALFTONE);
		profileImage.StretchBlt(mDC.m_hDC, 0, 0, m_nSizeProfileX, m_nSizeProfileY, 0, 0, profileImage.GetWidth(), profileImage.GetHeight(), SRCCOPY);
		mDC.SelectObject(bmp);

		m_pDefaultPicture->SetBitmap((HBITMAP)bitmap.Detach());
		ReleaseDC(screenDC);
	}
}

void CWeeklyPlannerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	//프로필 디폴트 등록
	//CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	//assert(m_pDefaultPicture && "주소값을 읽어올 수 없습니다.");

	//HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, m_nSizeProfileX, m_nSizeProfileY, LR_LOADMAP3DCOLORS);
	//m_pDefaultPicture->SetBitmap(hbmp);
	//AfxMessageBox(_T("호잇호잇"));

	CImage profileImage;
	profileImage.Load(m_strDefaultImagePath);


	CDC *screenDC = GetDC();
	CDC mDC;
	mDC.CreateCompatibleDC(screenDC);
	CBitmap bitmap;

	bitmap.CreateCompatibleBitmap(screenDC, m_nSizeProfileX, m_nSizeProfileY);
	CBitmap *bmp = mDC.SelectObject(&bitmap);
	mDC.SetStretchBltMode(HALFTONE);
	profileImage.StretchBlt(mDC.m_hDC, 0, 0, m_nSizeProfileX, m_nSizeProfileY, 0, 0, profileImage.GetWidth(), profileImage.GetHeight(), SRCCOPY);
	
	mDC.SelectObject(bmp);

	m_pDefaultPicture->SetBitmap((HBITMAP)bitmap.Detach());
	ReleaseDC(screenDC);
}



void CWeeklyPlannerView::OnClickedButtonProfileDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "주소값을 읽어올 수 없습니다.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, m_nSizeProfileX, m_nSizeProfileY, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
	m_strDefaultImagePath = L"res\\default_Image.bmp";
	
}


void CWeeklyPlannerView::OnLvnItemchangedDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CWeeklyPlannerView::OnInsertitemDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool m_bListValid = m_ctrlDdayList.GetItemCount();
	m_btnDeleteDday.EnableWindow(m_bListValid);
	UpdateData(TRUE);

	*pResult = 0;
}


void CWeeklyPlannerView::OnBnClickedDdayDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strforQ, strTitle;

	if (m_nDdayListSelectedItem >= 0) {
		strTitle = m_ctrlDdayList.GetItemText(m_nDdayListSelectedItem, 1);

		m_ctrlDdayList.DeleteItem(m_nDdayListSelectedItem);
		m_nDdayListSelectedItem = -1;
	}
	if (m_ctrlDdayList.GetItemCount() == 0) {
		m_btnDeleteDday.EnableWindow(FALSE);
	}

	strforQ.Format(L"DELETE FROM dday WHERE Title = '%s'", strTitle);

	SQLHSTMT h_statement_forDday;


	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, m_odbc->GetMh_odbc(), &h_statement_forDday)) {
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		RETCODE ret = SQLExecDirect(h_statement_forDday, (SQLWCHAR*)(const wchar_t *)strforQ, SQL_NTS);

		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		}

		SQLEndTran(SQL_HANDLE_ENV, m_odbc->GetMh_odbc(), SQL_COMMIT);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forDday);

	}
		UpdateData(TRUE);
}

void CWeeklyPlannerView::OnClickDdayListCntl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nDdayListSelectedItem = pNMItemActivate->iItem;

	*pResult = 0;
}


void CWeeklyPlannerView::OnClickedPauseSong()
{
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nPlayPause == 0)
	{
		//AfxMessageBox(_T("음악음악"));
		m_soundSP.SoundPlay(m_nPlayIndex);
		m_nPlayPause = 1;
		m_btnPlayPause.SetWindowText(_T("■"));
		m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);

	}
	else
	{
		//AfxMessageBox(_T("음악종료종료"));
		m_soundSP.SoundStop();
		m_nPlayPause = 0;
		m_btnPlayPause.SetWindowText(_T("▶"));
		m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);
	}

}


void CWeeklyPlannerView::OnClickedPrevSong()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	m_soundSP.SoundStop();
	
	if (m_nPlayIndex <= 0) {
		m_nPlayIndex = m_soundSP.m_nSoundIndex - 1;
	}
	else
	{
		m_nPlayIndex -= 1;
	}
	m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);
	if (m_nPlayPause == 1) {
		m_soundSP.SoundPlay(m_nPlayIndex);
	}

}


void CWeeklyPlannerView::OnClickedNextSong()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
		m_soundSP.SoundStop();
	

		if (m_nPlayIndex >= m_soundSP.m_nSoundIndex - 1) {

			m_nPlayIndex = 0;
		}
		else {
			m_nPlayIndex += 1;
		}
		m_strSongName.SetWindowText(m_soundSP.m_strSoundName[m_nPlayIndex]);
		if (m_nPlayPause == 1) {
			m_soundSP.SoundPlay(m_nPlayIndex);
		}

}


void CWeeklyPlannerView::OnBnClickedSongs()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("wav 파일(*.wav)|*.wav|모든파일(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, szFilter);

	CString SoundPath;
	CString fileName;
	
	POSITION Pos;

	CFileFind finder;
	int m_nIndex = m_soundSP.m_nSoundIndex;

	if (fileDlg.DoModal() == IDOK)
	{
		Pos = fileDlg.GetStartPosition();
		while (Pos)
		{
			SoundPath = fileDlg.GetNextPathName(Pos);
			
			m_soundSP.m_strSoundPath[m_nIndex] = SoundPath;

			finder.FindFile(SoundPath);
			finder.FindNextFile();
			m_soundSP.m_strSoundName[m_nIndex] = finder.GetFileTitle();
			m_soundPlayList.InsertItem(m_soundPlayList.GetItemCount(), finder.GetFileTitle());
			m_nIndex += 1;

			finder.Close();
		}
		m_soundSP.m_nSoundIndex = m_nIndex;
	}

}


void CWeeklyPlannerView::OnBnClickedButtonSoundDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos = m_soundPlayList.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int nitem = m_soundPlayList.GetNextSelectedItem(pos);
		
		m_soundSP.m_strSoundPath[nitem] = "";
		m_soundSP.m_strSoundName[nitem] = "";
		for (int i = nitem; i < m_soundSP.m_nSoundIndex; i++)
		{
			if (i + 1 == m_soundSP.m_nSoundIndex)
				break;
			m_soundSP.m_strSoundPath[i] = m_soundSP.m_strSoundPath[i + 1];
			m_soundSP.m_strSoundPath[i + 1] = "";
			m_soundSP.m_strSoundName[i] = m_soundSP.m_strSoundName[i + 1];
			m_soundSP.m_strSoundName[i + 1] = "";
		}
		m_soundSP.m_nSoundIndex -= 1;
		m_soundPlayList.DeleteItem(nitem);
		pos = m_soundPlayList.GetFirstSelectedItemPosition();
		
	
	}
}


void CWeeklyPlannerView::OnHistoryView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//view = new CHistoryView;
	view.DoModal();
}


void CWeeklyPlannerView::OnDestroy()
{
	GetDlgItemText(IDC_MESSAGE, m_strProfileMessage);
	m_odbc->DeleteProfilePath(m_strDefaultImagePath, m_strOldPath);
	m_odbc->SaveProfilePath(m_strDefaultImagePath, m_strOldPath);

	m_odbc->DeleteSound();
	m_odbc->DataSaveSound(m_soundSP.m_strSoundPath, m_soundSP.m_strSoundName);

	for (int i = 0; i < m_soundSP.m_nSoundIndex; i++) {
		m_soundSP.m_strSoundName[i] = "";
		m_soundSP.m_strSoundPath[i] = "";
		i++;
	}
	m_soundSP.m_nSoundIndex = 0;

	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
