#pragma once
#include "TypeDefine.h"


// CSystemConfig 对话框

class CSystemConfig :  public CFile
{

#define SAVE_PACKET_COUNT_DEFAULT		(1000*1000)    
#define CONFIG_FILE_PATH                                "C:\\DVB_TSA_Config.dat"
#define MAX_CFG_DATA_LENTH                          256
#define SYS_CFG_ID_LENTH_IN_FILE               4
#define SYS_CFG_LENTH_LENTH_IN_FILE             4

public:
	CSystemConfig();   // 标准构造函数
	virtual ~CSystemConfig();

// 对话框数据
       enum{SYS_CFG_ID_PKT_CNT,
            SYS_CFG_ID_FORCE_PRECHECK,
            SYS_CFG_ID_AUTO_OPEN_PPF,
            SYS_CFG_ID_AUTO_OPEN_TS
            }ConfigDataID;
    

public:
    void   SetCfgToDefault();
    BOOL SaveCfgToFile();
    BOOL ReadCfgOutFile();

public:
	u32 PacketCount;
	BOOL ForcePrecheck;
	CString PPFAuto;
	CString TSAuto;
	
};

class CSystemConfigDlg : public CDialog, public CSystemConfig
{
	DECLARE_DYNAMIC(CSystemConfigDlg)

public:
	CSystemConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemConfigDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SYS_CONFIG };    

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
 	afx_msg void OnBnClickedButtonPPF();
 	afx_msg void OnBnClickedButtonTS();

 	afx_msg void OnBnClickedButtonApply();
 	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditPktCount();
    
	virtual BOOL OnInitDialog();
};

