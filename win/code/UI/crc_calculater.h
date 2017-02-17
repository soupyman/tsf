#pragma once


// Ccrc_calculater 对话框
#include "CRC.h"

class Ccrc_calculater : public CDialog
{
	DECLARE_DYNAMIC(Ccrc_calculater)

public:
	Ccrc_calculater(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ccrc_calculater();

// 对话框数据
	enum { IDD = IDD_TOOLS_CRC };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCalc();
	int StrToHex(CString str);

public:
	UINT CrcResult;	
	CRC CrcInstance;
	CString RawData;

};
