#pragma once


// Ccrc_calculater �Ի���
#include "CRC.h"

class Ccrc_calculater : public CDialog
{
	DECLARE_DYNAMIC(Ccrc_calculater)

public:
	Ccrc_calculater(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ccrc_calculater();

// �Ի�������
	enum { IDD = IDD_TOOLS_CRC };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCalc();
	int StrToHex(CString str);

public:
	UINT CrcResult;	
	CRC CrcInstance;
	CString RawData;

};
