// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "typedefine.h"
#include "progstatusbar.h"
#include "Message.h"

class CMainFrame : public CFrameWnd, public virtual CMessageSpace
{
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	void CMainFrame::TransferMenu(bool Opened );

public:
	afx_msg void OnDestroy();

    	CComboBox m_ComboBox;

protected: 
	
	/*CStatusBar*/

	CProgStatusBar  	m_wndStatusBar;
	CToolBar    		m_wndToolBar;
	CSplitterWnd 		m_wndSplitterV,m_wndSplitterH1;//,m_wndSplitterH2;
	u8 				SuccessedThreadCount;
	//bool OwnTreeUpdate;
};


