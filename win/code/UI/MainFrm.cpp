// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "DvbTsa.h"

#include "TreeView.h"
#include "InfoView.h"
#include "DataView.h"
#include "SelectDataView.h"
#include "TreeListView.h"
#include "VersionListView.h"
#include "TSDoc.h"
#include "MainFrm.h"
#include "SystemConfig.h"
#include "include.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	 m_bAutoMenuEnable = false; 
	 SuccessedThreadCount = 0;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0), AFX_IDW_TOOLBAR+1) ||
		!m_wndToolBar.LoadToolBar(IDT_OPEN_TOOLBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	TransferMenu(false);
	PostMessage(MSG_UI_PPF_AND_TS_AUTO_OPEN);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	cs.x = 0;
	cs.y = 0;
	cs.cx   =   GetSystemMetrics(   SM_CXSCREEN   );   
  	cs.cy   =   GetSystemMetrics(   SM_CYSCREEN   ) - 30;   
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	m_wndSplitterH1.CreateStatic(this, 2, 1);
	m_wndSplitterH1.CreateView(0, 0, RUNTIME_CLASS(CInfoView), CSize(1100, 200), pContext);
	
	m_wndSplitterV.CreateStatic(&m_wndSplitterH1, 1, 4,WS_CHILD|WS_VISIBLE,m_wndSplitterH1.IdFromRowCol(1, 0));
	m_wndSplitterV.CreateView(0, 0, RUNTIME_CLASS(CTableListView), CSize(200, 600), pContext);
	m_wndSplitterV.CreateView(0, 1, RUNTIME_CLASS(CVersionListView), CSize(200, 600), pContext);
	m_wndSplitterV.CreateView(0, 2, RUNTIME_CLASS(CUnitTreeView), CSize(500, 600), pContext);
	m_wndSplitterV.CreateView(0, 3, RUNTIME_CLASS(CSelectDataView), CSize(400, 600), pContext);

	/* m_wndSplitterH2.CreateStatic(&m_wndSplitterV, 2, 1,WS_CHILD|WS_VISIBLE,m_wndSplitterV.IdFromRowCol(0, 3));
	m_wndSplitterH2.CreateView(0, 0, RUNTIME_CLASS(CSelectDataView ), CSize(200, 400), pContext);
	m_wndSplitterH2.CreateView(1, 0, RUNTIME_CLASS(CDataView), CSize(200, 300), pContext);
	CSize(hight, width)
	
	CSystemConfig SC;
	CTSDoc * pActiveDoc = ((CTSDoc *)GetActiveDocument());
	if(! SC.PPFAuto.IsEmpty())
	{
		pActiveDoc->OpenPrivateFormatFile(SC.PPFAuto);
	}
	if(! SC.TSAuto.IsEmpty())
	{
		pActiveDoc->OpenTSFile(SC.TSAuto);
	}*/
	return TRUE; //CFrameWnd::OnCreateClient(lpcs, pContext);
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CString t;
	CTSDoc * pActiveDoc = ((CTSDoc *)GetActiveDocument());
	
	switch(message)
	{
	case MSG_UI_BUILDER_THREAD_CREATE_SUCCESS:
		{
			// update ui needed 	
			pActiveDoc->DocInfo.AppendFormat("Builder thread created \r\n");
			SuccessedThreadCount ++;
			if(SuccessedThreadCount >= 2)
			{// start ts analyse
				TransferMenu(true);
			}
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
		}
		break;
	case MSG_UI_DEMUX_THREAD_CREATE_SUCCESS:
		{
			// update ui needed 	
			pActiveDoc->DocInfo.AppendFormat("Demux thread created \r\n");
			SuccessedThreadCount ++;
			if(SuccessedThreadCount >= 2)
			{// start ts analyse
				TransferMenu(true);
			}
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
		}
		break;
	case MSG_UI_UPDATE_HIDE_MENU:
		{
			// update ui needed 	
			SuccessedThreadCount = 0;
			TransferMenu(false);
			m_wndStatusBar.SetProBarPos(0);
			//pActiveDoc->TSFileClosed();
		}
		break;
	case MSG_UI_UPDATE_SELECT_INDEX:
		{
			m_wndStatusBar.SetSelectIndex((u32)wParam, (u32)lParam);
		}
		break;
	case MSG_UI_TS_ATTRIBUTE:
		{
			//	wParam : packet size ;  lParam : total backet number	
			CTSFile *pStream = pActiveDoc->Stream;
			
			pActiveDoc->DocInfo.AppendFormat("Packet size : %d \r\n",pStream->m_nPacketSize);
			pActiveDoc->DocInfo.AppendFormat("Packet total count : %d \r\n", pStream->m_nTotalPacketNumber);
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
		}
		break;
	case MSG_UI_PACKET:
		{
			assert(wParam);
			/*CContentTree * pTblPsr = (CContentTree *)wParam;
			//CESData * 	pEs =  (CESData *)wParam;
			//CTwmTree *	PktCntx = (CTwmTree *)wParam;
			if(pTblPsr->GetNodeType() == CContentTreeNode::PACKET_NODE)
			{
				CESData * 	pEs = ((CContentPacketNode * )(pTblPsr->GetMyNode()))->pES;
				pActiveDoc->AddContentTree(pTblPsr);	
				//pActiveDoc->AddES(pEs);
				pActiveDoc->DocInfo.AppendFormat("Receive tree from packet(%d);\r\n",pEs->PID);
				pActiveDoc->UpdateAllViews(NULL,CTSDoc::ALL_VIEW);
			}
			else
				assert(0);*/
		}
		break;
	case MSG_UI_TABLE: // CContentTree here
		{
			assert(wParam);
			CTable  * pTbl = (CTable *)wParam;
			//delete pTbl;
			//break;
			pActiveDoc->AddTable(pTbl);	
				
			pActiveDoc->DocInfo.AppendFormat("Receive tree from tableID(%d) PID(%d);\r\n", 
				pTbl->GetTableID(),pTbl->GetPID());
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::ALL_VIEW);
			
/*
*/
			
		}
		break;
	case MSG_UI_INFORMATION:
		{
			CString * pInfo = (CString *)wParam;
			if(pInfo)
			{
				pActiveDoc->DocInfo.Append(*pInfo);
				pActiveDoc->DocInfo.Append("\r\n");
				delete pInfo;
				pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
			}
		}
	break;
/*	case MSG_UI_TABLE_WRITE_BACK_SUCCESSED:
		{
			assert(wParam);
			CTable * pTblPsr = (CTable *)wParam;
			pActiveDoc->DocInfo.AppendFormat("TableID %x PID %x write back succesed \r\n",
				pTblPsr->GetTableID(),pTblPsr->GetPID());
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
		}
		break;
	case MSG_UI_TABLE_WRITE_BACK_FAILURE:
		{
			assert(wParam);
			CTable * pTblPsr = (CTable *)wParam;
			pActiveDoc->DocInfo.AppendFormat("TableID %x PID %x write back failed \r\n",
				pTblPsr->GetTableID(),pTblPsr->GetPID());
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
		}
		break;
		*/
	case MSG_UI_ES_ATTRIBUTE:
		{
			pActiveDoc->DocInfo.AppendFormat("Find ES(0x%x) %d packets\r\n",wParam,lParam);
			pActiveDoc->UpdateAllViews(NULL,CTSDoc::INFO_VIEW);
		}
		break;
	case MSG_UI_CURRENT_ACTION_START:
		{
			m_wndStatusBar.ActionStatus(true);
		}
		break;
	case MSG_UI_CURRENT_ACTION_PROCESS:
		{
			m_wndStatusBar.SetProBarPos((u8)wParam);
		}
		break;
	case MSG_UI_CURRENT_ACTION_FINISHED:
		{
			m_wndStatusBar.ActionStatus(false);
			MessageBeep(MB_ICONASTERISK);   // Beep
		}
		break;
	case MSG_UI_PPF_AND_TS_AUTO_OPEN:
		{
			CSystemConfig SC;
			if(! SC.PPFAuto.IsEmpty())
			{
				pActiveDoc->OpenPrivateFormatFile(SC.PPFAuto);
			}
			if(! SC.TSAuto.IsEmpty())
			{
				pActiveDoc->OpenTSFile(SC.TSAuto);
			}
		}
		break;
	default:
		break;
	}

	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}
/*
void CMainFrame::OnTimer(UINT nIDEvent)
{
	if(TaskThreatRun)
		m_wndStatusBar.ThreadRun();
	CFrameWnd::OnTimer(nIDEvent);
}
*/
void CMainFrame::TransferMenu(bool Opened )
{
	CMenu* m_menu = GetMenu();
	CMenu* submenu = m_menu->GetSubMenu(1); 
	//int pos;
	if(Opened)
	{
		m_wndToolBar.LoadToolBar(IDT_OPEN_TOOLBAR);
		DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP    ,CRect(0,0,0,0));

		submenu->EnableMenuItem(ID_TS_FILE_OPEN, MF_BYCOMMAND | MF_GRAYED);
		submenu->EnableMenuItem(ID_TS_FILE_CLOSE, MF_BYCOMMAND | MF_ENABLED);
		m_menu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		m_menu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
#ifdef SYSTEM_WITH_ES_OPERATORION
		m_menu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
#endif
		CRect rect;
		int index = 0;
		while(m_wndToolBar.GetItemID(index) != ID_EDIT_COMBO_BOX) 
			index++;
		m_wndToolBar.SetButtonInfo(index,  ID_EDIT_COMBO_BOX, TBBS_SEPARATOR, 100);
	   	m_wndToolBar.GetItemRect(index, &rect);
		rect.bottom += 200;
		m_ComboBox.Create(CBS_DROPDOWN    ,rect, &m_wndToolBar, ID_EDIT_COMBO_BOX);
		m_ComboBox.ShowWindow(	SW_SHOW);

	}
	else
	{
		m_ComboBox.DestroyWindow();
		m_wndToolBar.LoadToolBar(IDT_CLOSE_TOOLBAR);	
		DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP   ,CRect(0,0,0,0));

		submenu->EnableMenuItem(ID_TS_FILE_OPEN, MF_BYCOMMAND | MF_ENABLED);
		submenu->EnableMenuItem(ID_TS_FILE_CLOSE, MF_BYCOMMAND | MF_GRAYED);
		m_menu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);
		m_menu->EnableMenuItem(3, MF_BYPOSITION | MF_GRAYED);
#ifdef SYSTEM_WITH_ES_OPERATORION
		m_menu->EnableMenuItem(4, MF_BYPOSITION | MF_GRAYED);
#endif
	}
	DrawMenuBar();
}

void CMainFrame::OnDestroy()
{
	CTSDoc * pActiveDoc = ((CTSDoc *)GetActiveDocument());

	CFormatDatabase::FreeFormatList();

	pActiveDoc->TSFileClosed();

	CFrameWnd::OnDestroy();
}
