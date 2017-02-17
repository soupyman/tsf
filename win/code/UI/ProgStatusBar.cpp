// ProgStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "typedefine.h"
#include "ProgStatusBar.h"

#define ROUND_STEP	1

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器/*ID_INDICATOR_BASE*/
	ID_INDICATOR_START_INDEX,
	ID_INDICATOR_END_INDEX,
	ID_INDICATOR_ACTION_STATUS,
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};
// CProgStatusBar

IMPLEMENT_DYNAMIC(CProgStatusBar, CStatusBar)
CProgStatusBar::CProgStatusBar()
{
	//RunTxt = "0100110100010100100010111000";
}

CProgStatusBar::~CProgStatusBar()
{
}


BEGIN_MESSAGE_MAP(CProgStatusBar, CStatusBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CProgStatusBar 消息处理程序

int CProgStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
	
	m_ProgressBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 1);
	m_ProgressBar.SetRange(0,100);

	return 0;
}

/*
bool CProgStatusBar::ThreadRun(void)
{
	CString TmpTxt = RunTxt.Left(ROUND_STEP);
	RunTxt = RunTxt.Right(RunTxt.GetLength() - ROUND_STEP);
	RunTxt.Append(TmpTxt);
	SetPaneText(1,RunTxt);
	return false;
}
	*/


bool CProgStatusBar::ActionStatus(bool status)
{
	CString action;
 
	if(status)
		action.Format("Waiting");
	else
		action.Format("Finished");
	
	SetPaneText(ID_INDICATOR_ACTION_STATUS - ID_INDICATOR_BASE,action);
	return false;
}

void CProgStatusBar::SetSelectIndex(u32 start, u32 end)
{
	CString str;

	str.Format("%d",start);
	SetPaneText(ID_INDICATOR_START_INDEX - ID_INDICATOR_BASE,str);
	
	str.Format("%d",end);
	SetPaneText(ID_INDICATOR_END_INDEX - ID_INDICATOR_BASE,str);
}

void CProgStatusBar::SetProBarPos(u8 pos)
{
	m_ProgressBar.SetPos(pos);
}

void CProgStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CStatusBar::OnSize(nType, cx, cy);

	cx -= 520;
	if(cx < 220)
		cx = 220;
    m_ProgressBar.MoveWindow(240,3,cx,17);
}
