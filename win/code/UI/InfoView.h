#pragma once


// CInfoView 视图
class CTSDoc;

class CInfoView : public CEditView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CTSDoc * GetDocument(void);
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)	;
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
public:
	CStdioFile	InfoFile;
	u32			InfoLen;

};


