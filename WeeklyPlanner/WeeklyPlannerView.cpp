
// WeeklyPlannerView.cpp : CWeeklyPlannerView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

// CWeeklyPlannerView ����/�Ҹ�

CWeeklyPlannerView::CWeeklyPlannerView()
	: CFormView(IDD_WEEKLYPLANNER_FORM)
	, m_bModifyBtn(false)
	, m_nTodoDone(0)
	, m_todoEnd(0)
	, m_checkCnt(0)
	, m_strProfilePath(_T(""))
	, m_timeNewDday(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	DDX_Control(pDX, IDC_DDAY_ADD_BUTTON, m_btnaddDday);
	DDX_DateTimeCtrl(pDX, IDC_TODO_START, m_todoStart);
	DDX_DateTimeCtrl(pDX, IDC_TODO_END, m_todoEnd);
	DDX_Control(pDX, IDC_TODO_START, m_cTodoStart);
	DDX_Control(pDX, IDC_TODO_END, m_cTodoEnd);
}

BOOL CWeeklyPlannerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

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
	assert(m_pDefaultPicture && "�ּҰ��� �о�� �� �����ϴ�.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 180, 180, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);*/
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


// CWeeklyPlannerView ����

#ifdef _DEBUG
void CWeeklyPlannerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWeeklyPlannerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWeeklyPlannerDoc* CWeeklyPlannerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeeklyPlannerDoc)));
	return (CWeeklyPlannerDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeeklyPlannerView �޽��� ó����


void CWeeklyPlannerView::OnClickedMessageModifyButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!m_bModifyBtn) { //���� �϶�
		m_EditMessage.EnableWindow(true);
		m_btnMessagemodify.SetWindowText(_T("����"));
		m_bModifyBtn = true;

	}
	else {
		m_EditMessage.EnableWindow(false);
		m_btnMessagemodify.SetWindowText(_T("����"));

		m_bModifyBtn = false;
	}

}


void CWeeklyPlannerView::OnEnChangeMessage()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CWeeklyPlannerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CWeeklyPlannerView::OnBnClickedAddTodoButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		AfxMessageBox(_T("������ �߰��� �� �����ϴ�."));

	//��ư Ŭ�� �� ���� ����
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ADD_TODO_MEMO);
	pEdit->SetWindowText(_T(""));

	// DB�� �ø���
}


void CWeeklyPlannerView::OnClickedDdayAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CDdayAddDlg m_ddayaddDlg;
	m_ddayaddDlg.DoModal();
}


void CWeeklyPlannerView::OnDtnDatetimechangeTodoStart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	*pResult = 0;
}


void CWeeklyPlannerView::OnDtnDatetimechangeTodoEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	*pResult = 0;
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[1] = true;
	MoveCheckboxStr(2);
	UpdateTodoProgressBar(&m_TodoCheck2, 1);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[2] = true;
	MoveCheckboxStr(3);
	UpdateTodoProgressBar(&m_TodoCheck3, 2);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[3] = true;
	MoveCheckboxStr(4);
	UpdateTodoProgressBar(&m_TodoCheck4, 3);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[4] = true;
	MoveCheckboxStr(5);
	UpdateTodoProgressBar(&m_TodoCheck5, 4);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[5] = true;
	MoveCheckboxStr(6);
	UpdateTodoProgressBar(&m_TodoCheck6, 5);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bChecked[6] = true;
	MoveCheckboxStr(7);
	UpdateTodoProgressBar(&m_TodoCheck7, 6);
}


void CWeeklyPlannerView::OnClickedTodoDeleteButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//dimx, dimy : image size;
	int dimx = 150;
	int dimy = 160;
	static TCHAR BASED_CODE szFilter[] = _T("�̹��� ����(*.bmp, *.jpg, *.png)|*.bmp; *.jpg; *.png|�������(*.*)|*.*||");
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
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CFormView::OnPaint()��(��) ȣ������ ���ʽÿ�.

	//������ ����Ʈ ���
	CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "�ּҰ��� �о�� �� �����ϴ�.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 140, 140, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
}



void CWeeklyPlannerView::OnClickedButtonProfileDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CStatic* m_pDefaultPicture = (CStatic*)GetDlgItem(IDC_PROFILE_PHOTO);
	assert(m_pDefaultPicture && "�ּҰ��� �о�� �� �����ϴ�.");

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_BITMAP_PROFILE_DEFAULT), IMAGE_BITMAP, 140, 140, LR_LOADMAP3DCOLORS);
	m_pDefaultPicture->SetBitmap(hbmp);
}
