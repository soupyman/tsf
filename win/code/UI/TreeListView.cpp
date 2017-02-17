// UnitTreeView.cpp : 实现文件

#include "stdafx.h"
#include "DvbTsa.h"
#include "TreeListView.h"
#include "TSDoc.h"
#include "include.h"


IMPLEMENT_DYNCREATE(CTableListView, CListView)

CTableListView::CTableListView()
{
}

CTableListView::~CTableListView()
{
}

BEGIN_MESSAGE_MAP(CTableListView, CListView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()


// CTableListView 诊断

#ifdef _DEBUG
void CTableListView::AssertValid() const
{
	CListView::AssertValid();
}

void CTableListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

CTSDoc * CTableListView::GetDocument(void)
{
	return (CTSDoc *)CView::GetDocument();
}

// CTableListView 消息处理程序
void CTableListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(lHint & CTSDoc::TABLE_LIST_VIEW)
	{
		CListCtrl * pListCtrl = &(this->GetListCtrl());
		pListCtrl->DeleteAllItems();

		CTwmList *	pList = &(GetDocument()->TableList);
		u8 ItemIdx = 0;
		POSITION pos = pList->GetHeadPosition();
		CTable * pT;
		CString Name;
		while(pos)
		{
			pT = (CTable *)(pList->GetNext(pos));
			Name = GetDocument()->FmtSvr.GetNameByTable(pT);
			pListCtrl->InsertItem(ItemIdx,(LPCTSTR)  Name );
			pListCtrl->SetItemData(ItemIdx,(DWORD_PTR)pT);

			ItemIdx++;
		}
	}
}

BOOL CTableListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_ALIGNLEFT|LVS_REPORT |LVS_SHOWSELALWAYS  ;
	return CListView::PreCreateWindow(cs);
}

void CTableListView::OnInitialUpdate()
{
	CFont pf;
	GetDocument()->pTableListView = this;
	
	CListView::OnInitialUpdate();
	CListCtrl * pListCtrl = &(this->GetListCtrl());
	pListCtrl->InsertColumn(0,"Table List");
	pListCtrl->SetColumnWidth(0,350);

	pf.CreateFont(20 ,15, 0,0,700, 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times New Roman");

	pListCtrl->SetFont(&pf);
}

void CTableListView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl * pListCtrl = &(this->GetListCtrl());
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	int HotIdx = pListCtrl->GetNextSelectedItem(pos);

	if((HotIdx >= 0)&&(HotIdx < pListCtrl->GetItemCount()))
	{
		if(GetDocument()->pTable != ((CTable *)(pListCtrl->GetItemData(HotIdx))))
		{
			GetDocument()->pTable = ((CTable *)(pListCtrl->GetItemData(HotIdx)));
			GetDocument()->pVersionListView->PostMessage(MSG_UI_SELECT_CHANGE,
				(WPARAM)GetDocument()->pTable->GetSection());
		}
	}
	*pResult = 0;
}


void CTableListView::UpdateSelection(CTable * pCurSelTbl)
{
	if(GetDocument()->pTable != pCurSelTbl)
	{
		GetDocument()->pTable = pCurSelTbl;
		GetDocument()->pVersionListView->PostMessage(MSG_UI_SELECT_CHANGE,(WPARAM)pCurSelTbl->GetSection());
	}
}

LRESULT CTableListView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_UI_SELECT_CHANGE:
		{
			UpdateSelection((CTable * )wParam);	
		}
		break;
		default:
			break;
	}

	return CListView::WindowProc(message, wParam, lParam);
}

