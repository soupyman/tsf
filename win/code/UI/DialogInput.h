#pragma once

#include "resource.h"
#include "typedefine.h"
// CDialogParamInput �Ի���

class CDialogParamInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogParamInput)

public:
	CDialogParamInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogParamInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARAM_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	u32 		_data_i;
	int 		LenInBin;
	CString 	Title;
	bool	 	consumed;

	afx_msg void OnEnChangeEditParamInputDec();
	afx_msg void OnEnChangeEditParamInputBin();
	afx_msg void OnEnChangeEditParamInputHex();
private:
	u32 HexStrToInt(CString & pstr);

};

// CDialogPIDIndexInput �Ի���

class CDialogPIDIndexInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogPIDIndexInput)

public:
	CDialogPIDIndexInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPIDIndexInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_PID_INDEX_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int _Pid_i;
	int PacketIndx;
};

// CDialogPIDTableIDInput �Ի���

class CDialogPIDTableIDInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogPIDTableIDInput)

public:
	CDialogPIDTableIDInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPIDTableIDInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_PID_TABLEID_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int _Pid_i;
	int _TableID_i;

};
