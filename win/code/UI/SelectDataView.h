#pragma once

#include "TypeDefine.h"
#include "Message.h"

#define DATA_WIDTH	11
#define DATA_HEIGHT	20
#define DATA_GAP		2

#define SCALE_WIDTH	10
#define SCALE_GAP	10

// CSelectDataView ��ͼ
class CTSDoc;


class CSelectDataView : public CEditView , public virtual CMessageSpace
{
	DECLARE_DYNCREATE(CSelectDataView)

protected:
	CSelectDataView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSelectDataView();

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
	//void 		TraceOutData(u8 * pData, u32 Lenth);
	void 		UpdataData();
	void 		PaintIndx();
private:
	CString 		Content;
	CFont 		DataFont;
	CFont 		ScalFont;
	BOOL		Modifyed;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//public:
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnChange();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
};


