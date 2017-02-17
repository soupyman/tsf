// InfoView.cpp : 实现文件
//

#include "stdafx.h"
#include "TSDoc.h"
#include "InfoView.h"


// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CEditView)

CInfoView::CInfoView()
{
	InfoLen = 0;
	if(! InfoFile.Open("Infomation.txt",CFile::modeCreate|CFile::modeWrite ))
	{
			GetDocument()->DocInfo.AppendFormat("!!! Open Infomation.txt failed \r\n");
	}
}

CInfoView::~CInfoView()
{
	InfoFile.Close();
}

BEGIN_MESSAGE_MAP(CInfoView, CEditView)
END_MESSAGE_MAP()


// CInfoView 诊断

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CEditView::AssertValid();
}

void CInfoView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

CTSDoc * CInfoView::GetDocument(void)
{
	return (CTSDoc *)CView::GetDocument();;
}


void CInfoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(lHint & CTSDoc::INFO_VIEW)
	{
		CEdit & ThisEdit = this->GetEditCtrl();
		ThisEdit.Clear();
		
		InfoFile.WriteString(GetDocument()->DocInfo.Right(GetDocument()->DocInfo.GetLength() - InfoLen));
		if(GetDocument()->DocInfo.GetLength() > 10000)
		{
			GetDocument()->DocInfo.Delete(0,1000);
		}
		InfoLen = GetDocument()->DocInfo.GetLength();
		ThisEdit.SetWindowText((LPCTSTR)(GetDocument()->DocInfo));
		ThisEdit.LineScroll(ThisEdit.GetLineCount() -1 );	
	}
}

// CInfoView 消息处理程序

BOOL CInfoView::PreCreateWindow(CREATESTRUCT& cs)
{

	BOOL ret = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping
	return ret;
}

void CInfoView::OnInitialUpdate()
{
//#define TEST_DATA_LENGTH
#ifdef TEST_DATA_LENGTH
	u8 u8t;
	u16 u16t;
	u32 u32t;
	s32 s32t;
	u64 u64t;
	s64 s64t;
	GetDocument()->DocInfo.AppendFormat(
		"Size test: u8 : %d; u16 : %d; u32 : %d; s32 : %d; u64 : %d; s64 : %d. \r\n double :%d", 
		sizeof(u8t),sizeof(u16t),sizeof(u32t),sizeof(s32t),sizeof(u64t),sizeof(s64t),sizeof(double));
#endif

	GetDocument()->pInfoView = this;	
	
	CEdit & ThisEdit = this->GetEditCtrl();
	ThisEdit.SetReadOnly();	
		
	CEditView::OnInitialUpdate();
}

