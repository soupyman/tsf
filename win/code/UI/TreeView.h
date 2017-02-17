#pragma once
#include "typedefine.h"
#include "Table.h"
#include "Message.h"
#include "DVBDefine.h"


class CUnitTreeView : public CTreeView , public CDVBSpace, public virtual CMessageSpace
{
	DECLARE_DYNCREATE(CUnitTreeView)

protected:
	CUnitTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CUnitTreeView();

	//HTREEITEM pCurSectionBranch;
	HTREEITEM m_SlctTItem;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void AttachTree( CCommonTree *	pTT , HTREEITEM parent);
	void UpdateSelection();
	//void DeleteSelection();

	BOOL SearchTreeField(HTREEITEM From ,CString * Part , bool & FindCur);
	CTSDoc * GetDocument(void);
#if 0
	HTREEITEM FindDirectAheadItem(HTREEITEM item, CString strText);
	u32  ParseSubBody(u8 * pData, u8 OffsetInBit,CTwmList  & StructList ,HTREEITEM parent);
	bool ParseSectionContext(HTREEITEM parent, u8 * pSectionData = NULL);
	HTREEITEM AddSectionTitle(HTREEITEM  parent, CommonSection * pSectionData);
	HTREEITEM AddTreeItem(HTREEITEM parent, CString & pSLabel , u32 value);
#endif
	
		CImageList   m_imgTree;
		CContentTree *	pCntTree;
};


