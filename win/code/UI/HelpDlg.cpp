// HelpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "HelpDlg.h"


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString CompileString;
	CompileString.Format("Current Version release Time: %s %s",__DATE__,__TIME__);
	GetDlgItem(IDC_EDIT_COMPILE_DATE_TIME)->SetWindowText(CompileString);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

