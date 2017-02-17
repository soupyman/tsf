#pragma once
#include "Mpeg2Decoder.h"
#include "afxwin.h"
#include "afxcmn.h"

class CTSFile;
// CVideoPlayDlg 对话框

class CVideoPlayDlg : public CDialog , public CDVBSpace
{
	DECLARE_DYNAMIC(CVideoPlayDlg)
#define VIDEO_TIMER_EVENT_ID        123
public:
	CVideoPlayDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoPlayDlg();

// 对话框数据
    enum { IDD = IDD_DIALOG_PLAY };
    enum PlayState
    {
        STATE_STOP  ,
        STATE_PLAYING,
        STATE_PAUSED
    };    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
        void UpdateProgramList();
         u8  get_service_name (CTable * Sdt_l , u16 ProgramNumber , CString & name);
       u16  get_video_pid ( CTable * pmt_l);

public:
	CListCtrl ProgramListCtrl;
	CTSFile * pTs;
    	CMpeg2Decoder pM2D;
        u32    TimePast;
        u32     FirstTimeStamp;
        UINT_PTR m_nTimer;
        PlayState      state;
        u32 LastPauseFrameCount;


	CTwmList 	* pTblList;

};
