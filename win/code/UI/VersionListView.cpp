// UnitTreeView.cpp : 实现文件

#include "stdafx.h"
#include <assert.h>
#include "DvbTsa.h"
#include "VersionListView.h"
#include "TSDoc.h"
#include "include.h"

// CVersionListView

IMPLEMENT_DYNCREATE(CVersionListView, CListView)

CVersionListView::CVersionListView()
{
}

CVersionListView::~CVersionListView()
{
}

BEGIN_MESSAGE_MAP(CVersionListView, CListView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()


// CVersionListView 诊断

#ifdef _DEBUG
void CVersionListView::AssertValid() const
{
	CListView::AssertValid();
}

void CVersionListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

CTSDoc * CVersionListView::GetDocument(void)
{
	return (CTSDoc *)CView::GetDocument();
}

// CVersionListView 消息处理程序
void CVersionListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

	if(lHint & CTSDoc::VERSION_LIST_VIEW)
	{
		CSection * pSec;
		u8 ItemIdx = 0;
		CTable *	pTbl = GetDocument()->pTable;
		
		CListCtrl * pListCtrl = &(this->GetListCtrl());
		pListCtrl->DeleteAllItems();
		pListCtrl->DeleteColumn(0);
		if(pTbl)
			pListCtrl->InsertColumn(0,GetDocument()->FmtSvr.GetNameByTable(pTbl));
		else
			pListCtrl->InsertColumn(0,"Version List");
		
		pListCtrl->SetColumnWidth(0,350);
	

		while(pTbl)
		{
			pSec = pTbl->GetSection();
			while(pSec)
			{
				pListCtrl->InsertItem(ItemIdx,(LPCTSTR)(GetDocument()->FmtSvr.GetTitleBySection(pSec)));
				pListCtrl->SetItemData(ItemIdx,(DWORD_PTR)pSec);
				ItemIdx++;
				pSec = pSec->pNext;
			}
			GetDocument()->DocInfo.AppendFormat("OnUpdate(VERSION_LIST_VIEW) TableID %d, PID %d ,Version %d ,%d Duplicate in stream\r\n",	
				pTbl->GetTableID(),pTbl->GetPID(),pTbl->GetVersion(),pTbl->GetDupliCount());
			pTbl = pTbl->pNextVer;
		}
		GetDocument()->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
	}

}
BOOL CVersionListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_ALIGNLEFT|LVS_REPORT |LVS_SHOWSELALWAYS  ;
	return CListView::PreCreateWindow(cs);
}

void CVersionListView::OnInitialUpdate()
{
	CFont pf;
	GetDocument()->pVersionListView = this;
	
	CListView::OnInitialUpdate();
	CListCtrl * pListCtrl = &(this->GetListCtrl());
	pListCtrl->InsertColumn(0,"Version List");
	pListCtrl->SetColumnWidth(0,350);
	//pListCtrl->DeleteColumn(0);
	pf.CreateFont(20 ,15, 0,0,700, 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times New Roman");

	pListCtrl->SetFont(&pf);

	//pListCtrl->SetBkColor(RGB(240,230,220));
	//pListCtrl->SetTextBkColor(RGB(240,230,220));
}
void CVersionListView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{

	CListCtrl * pListCtrl = &(this->GetListCtrl());
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	int HotIdx = pListCtrl->GetNextSelectedItem(pos);

	if((HotIdx >= 0)&&(HotIdx < pListCtrl->GetItemCount()))
	{
		if(GetDocument()->pSection != ( (CSection *)(pListCtrl->GetItemData(HotIdx))))
		{
			UpdateSelection( (CSection *)(pListCtrl->GetItemData(HotIdx) ));
		}
	}
	//TRACE("HotIdx (%d) \n",HotIdx);
	*pResult = 0;
}

void CVersionListView::UpdateSelection(CSection * pCurSelSec)
{
	GetDocument()->pSection = pCurSelSec;

	if(pCurSelSec)
	{
		GetDocument()->DocDataLen = GetDocument()->pSection->GetSectionLength() + 3;
		assert((GetDocument()->DocDataLen < DATAVIEWBUFFERLENTH)&&(GetDocument()->DocDataLen > 0 ));
		memcpy(GetDocument()->DocData,GetDocument()->pSection->GetSectionData() , GetDocument()->DocDataLen);
	}
	else
	{
		GetDocument()->DocDataLen = 0;
	}
	GetDocument()->DocDataFocusStart = 0;
	GetDocument()->DocDataFocusLen =  0;

	GetDocument()->UpdateAllViews(NULL,CTSDoc::TREE_VIEW|CTSDoc::DATA_VIEW| CTSDoc::SELECT_DATA_VIEW);

}

LRESULT CVersionListView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_UI_SELECT_CHANGE:
		{
			UpdateSelection((CSection * )wParam);	
			GetDocument()->UpdateAllViews(NULL,CTSDoc::VERSION_LIST_VIEW);
		}
		break;
		default:
			break;
	}

	return CListView::WindowProc(message, wParam, lParam);
}

