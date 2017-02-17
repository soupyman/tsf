#pragma once


// CProgStatusBar

class CProgStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CProgStatusBar)
public:
	CProgressCtrl m_ProgressBar;
private:
	//CString RunTxt;

public:
	CProgStatusBar();
	virtual ~CProgStatusBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	//bool ThreadRun(void);
	bool ActionStatus(bool status);
	void SetSelectIndex(u32 start, u32 end);
	void SetProBarPos(u8 pos);
};


