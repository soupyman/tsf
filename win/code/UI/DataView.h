#pragma once

#include "TypeDefine.h"
#include "Message.h"

#define DATA_WIDTH	11
#define DATA_HEIGHT	20
#define DATA_GAP		2

#define SCALE_WIDTH	10
#define SCALE_GAP	10

// CDataView 视图
class CTSDoc;


class CDataView : public CEditView , public virtual CMessageSpace
{
	DECLARE_DYNCREATE(CDataView)

protected:
	CDataView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDataView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
public:
	CTSDoc * GetDocument(void);
	void 		TraceOutData(u8 * pData, u32 Lenth);
	void 		UpdataData();
private:
	CString 		Content;
	BOOL		Modifyed;
protected:
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnChange();
	
public:
	virtual void OnInitialUpdate();
};


