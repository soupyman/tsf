#pragma once
#include "typedefine.h"
#include "Message.h"

class CTSDoc;

class CVersionListView : public CListView,public CMessageSpace
{
	DECLARE_DYNCREATE(CVersionListView)

protected:
	CVersionListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CVersionListView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	
public:
	CTSDoc * GetDocument(void);
	void 	UpdateSelection(CSection * pCurSelSec);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual void OnInitialUpdate();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


