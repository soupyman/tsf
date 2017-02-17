#pragma once
#include "typedefine.h"
//#include "Table.h"
#include "Message.h"

class CTSDoc;

class CTableListView : public CListView ,public CMessageSpace
{
	DECLARE_DYNCREATE(CTableListView)

protected:
	CTableListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTableListView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	
public:
	CTSDoc * GetDocument(void);
	void 	UpdateSelection(CTable * pCurSelTbl);

};


