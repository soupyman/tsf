#pragma once

#include "resource.h"
#include "typedefine.h"
// CDialogParamInput 对话框

class CDialogParamInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogParamInput)

public:
	CDialogParamInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogParamInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAM_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

// CDialogPIDIndexInput 对话框

class CDialogPIDIndexInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogPIDIndexInput)

public:
	CDialogPIDIndexInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPIDIndexInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_PID_INDEX_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int _Pid_i;
	int PacketIndx;
};

// CDialogPIDTableIDInput 对话框

class CDialogPIDTableIDInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogPIDTableIDInput)

public:
	CDialogPIDTableIDInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPIDTableIDInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_PID_TABLEID_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int _Pid_i;
	int _TableID_i;

};
