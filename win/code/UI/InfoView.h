#pragma once


// CInfoView ��ͼ
class CTSDoc;

class CInfoView : public CEditView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


