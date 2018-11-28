// ProfileModifyDlg.cpp : 구현 파일입니다.


#include "stdafx.h"
#include "WeeklyPlanner.h"
#include "ProfileModifyDlg.h"
#include "afxdialogex.h"


// CProfileModifyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProfileModifyDlg, CDialogEx)

CProfileModifyDlg::CProfileModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROFILE_MODIFY, pParent)
	, m_strFilePath(_T(""))
{

}

CProfileModifyDlg::~CProfileModifyDlg()
{
}

void CProfileModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MODIFY_CANCLE, m_btnProfileCancel);
	DDX_Control(pDX, IDC_BUTTON_MODIFY_APPLY, m_btnProfileApply);
	DDX_Control(pDX, IDC_BUTTON_IMAGEFILE_OPEN, m_btnProfileOpen);
	DDX_Control(pDX, IDC_PROFILE_MODAL, m_profileModalImg);
}


BEGIN_MESSAGE_MAP(CProfileModifyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_CANCLE, &CProfileModifyDlg::OnBnClickedButtonModifyCancle)
	ON_BN_CLICKED(IDC_BUTTON_IMAGEFILE_OPEN, &CProfileModifyDlg::OnBnClickedButtonImagefileOpen)
END_MESSAGE_MAP()


// CProfileModifyDlg 메시지 처리기입니다.


void CProfileModifyDlg::OnBnClickedButtonModifyCancle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();

}

void CProfileModifyDlg::OnBnClickedButtonImagefileOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.PNG, *.JPG)|*.BMP, *.PNG, *.JPG, *.png, *.bmp, *.jpg|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		m_strFilePath = dlg.GetFileName();

		CImage profileImg;
		CBitmap imageBmp;
		CDC bitmapDC;
		CBitmap *pOldBmp;
		BITMAP bi;

		CDC *pDC = m_profileModalImg.GetWindowDC();
		
		int width, height;

		profileImg.Destroy();
		profileImg.Load(m_strFilePath);

		imageBmp.Attach(profileImg.Detach());

		width = profileImg.GetWidth();
		height = profileImg.GetHeight();

		bitmapDC.CreateCompatibleDC(pDC);

		//HBITMAP hBmp = (HBITMAP)::LoadImage()
		
		pOldBmp = bitmapDC.SelectObject(&imageBmp);
		imageBmp.GetBitmap(&bi);
		pDC->BitBlt(0, 0, bi.bmWidth, bi.bmHeight, &bitmapDC, 0, 0, SRCCOPY);
		bitmapDC.SelectObject(pOldBmp);
	}
}


CString CProfileModifyDlg::ReturnFilePath()
{
	return m_strFilePath;
}
