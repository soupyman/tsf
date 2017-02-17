// DialogInput.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "DialogInput.h"


// CDialogParamInput 对话框

IMPLEMENT_DYNAMIC(CDialogParamInput, CDialog)

BEGIN_MESSAGE_MAP(CDialogParamInput, CDialog)
	ON_EN_CHANGE(IDC_EDIT_PARAM_INPUT_DEC, OnEnChangeEditParamInputDec)
	ON_EN_CHANGE(IDC_EDIT_PARAM_INPUT_BIN,  OnEnChangeEditParamInputBin)
	ON_EN_CHANGE(IDC_EDIT_PARAM_INPUT_HEX, OnEnChangeEditParamInputHex)
END_MESSAGE_MAP()

CDialogParamInput::CDialogParamInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogParamInput::IDD, pParent)
{
	_data_i = -1;
	LenInBin = 0;
	consumed = false;
}

CDialogParamInput::~CDialogParamInput()
{
}

void CDialogParamInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	CString dec ;

	SetWindowText(Title);
	//GetDlgItem(IDC_EDIT_LABEL)->SetWindowText(Label);
	
	DDX_Text(pDX, IDC_EDIT_PARAM_INPUT_DEC, dec);
	DDV_MaxChars(pDX, dec, 4);

	_data_i = atoi( LPCTSTR(dec)); 	
	CString 	BinData;
	for(int i  = (LenInBin-1) ; i >= 0; i--)
	{
		if((0x1<<i)&_data_i)
			BinData.AppendChar('1');
		else
			BinData.AppendChar('0');
	}
	GetDlgItem(IDC_EDIT_PARAM_INPUT_BIN)->SetWindowText(BinData);	
}

void CDialogParamInput::OnEnChangeEditParamInputDec()
{
	if(consumed == false)
	{
		consumed = true;
		CString		StrData;
		GetDlgItem(IDC_EDIT_PARAM_INPUT_DEC)->GetWindowText(StrData);

		_data_i = atoi( LPCTSTR(StrData)); 

		u32 mask = 0xFFFFFFFF;
		mask = ~(mask << LenInBin);
		_data_i &= mask;
		StrData.Format("%d",_data_i);
		GetDlgItem(IDC_EDIT_PARAM_INPUT_DEC)->SetWindowText(StrData);	
		StrData.Format("%x",_data_i);
		GetDlgItem(IDC_EDIT_PARAM_INPUT_HEX)->SetWindowText(StrData);
		StrData.Empty();
		for(int i  = (LenInBin-1) ; i >= 0; i--)
		{
			if((0x1<<i)&_data_i)
				StrData.AppendChar('1');
			else
				StrData.AppendChar('0');
		}
		GetDlgItem(IDC_EDIT_PARAM_INPUT_BIN)->SetWindowText(StrData);	
		consumed = false;
	}
}
void CDialogParamInput::OnEnChangeEditParamInputBin()
{
	if(consumed == false)
	{
		consumed = true;
		CString		StrData;
		
		GetDlgItem(IDC_EDIT_PARAM_INPUT_BIN)->GetWindowText(StrData);
		_data_i = 0;
		for(int i  = (LenInBin-1) ; i >= 0; i--)
		{
			if(StrData.Mid(i , 1)  == "1")
				_data_i = _data_i<<1 | 0x1;
			else
				_data_i = _data_i<<1 | 0x0;
		}
		u32 mask = 0xFFFFFFFF;
		mask = ~(mask << LenInBin);
		_data_i &= mask;
		
		StrData.Format("%d",_data_i);
		GetDlgItem(IDC_EDIT_PARAM_INPUT_DEC)->SetWindowText(StrData);	
		StrData.Format("%x",_data_i);
		GetDlgItem(IDC_EDIT_PARAM_INPUT_HEX)->SetWindowText(StrData);
		StrData.Empty();
		for(int i  = (LenInBin-1) ; i >= 0; i--)
		{
			if((0x1<<i)&_data_i)
				StrData.AppendChar('1');
			else
				StrData.AppendChar('0');
		}
		GetDlgItem(IDC_EDIT_PARAM_INPUT_BIN)->SetWindowText(StrData);	
		consumed = false;
	}
}
void CDialogParamInput::OnEnChangeEditParamInputHex()
{
	if(consumed == false)
	{
		consumed = true;
		CString		StrData;
		GetDlgItem(IDC_EDIT_PARAM_INPUT_HEX)->GetWindowText(StrData);
		_data_i = HexStrToInt(StrData); 

		u32 mask = 0xFFFFFFFF;
		mask = ~(mask << LenInBin);
		_data_i &= mask;
		
		StrData.Format("%d",_data_i);
		GetDlgItem(IDC_EDIT_PARAM_INPUT_DEC)->SetWindowText(StrData);	
		StrData.Format("%x",_data_i);
		GetDlgItem(IDC_EDIT_PARAM_INPUT_HEX)->SetWindowText(StrData);
		StrData.Empty();
		for(int i  = (LenInBin-1) ; i >= 0; i--)
		{
			if((0x1<<i)&_data_i)
				StrData.AppendChar('1');
			else
				StrData.AppendChar('0');
		}
		GetDlgItem(IDC_EDIT_PARAM_INPUT_BIN)->SetWindowText(StrData);	
		consumed = false;
	}
}

u32 CDialogParamInput::HexStrToInt(CString & pstr)
{
	u32 ret = 0;
	CString cur;
	for(int i=0; i < pstr.GetLength(); i++ )
	{
		ret = ret<<4;
		cur = pstr.Mid(i , 1);
		cur.MakeUpper();
		if(cur == "F")
				ret +=  0xF;
		if(cur == "E")
				ret +=  0xE;
		if(cur == "D")
				ret +=  0xD;
		if(cur == "C")
				ret +=  0xC;
		if(cur == "B")
				ret +=  0xB;
		if(cur == "A")
				ret +=  0xA;
		else
				ret +=  atoi( LPCTSTR(cur ));
	}
	return ret;
}

// CDialogParamInput 消息处理程序

// CDialogPIDIndexInput 对话框

IMPLEMENT_DYNAMIC(CDialogPIDIndexInput, CDialog)
CDialogPIDIndexInput::CDialogPIDIndexInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPIDIndexInput::IDD, pParent)
{
	_Pid_i = -1;
	PacketIndx = 0;
}

CDialogPIDIndexInput::~CDialogPIDIndexInput()
{
}

void CDialogPIDIndexInput::DoDataExchange(CDataExchange* pDX)
{
	CString PID ,StrIdx;
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PID_INDEX_INPUT_PID, PID);
	DDX_Text(pDX, IDC_EDIT_PID_INDEX_INPUT_START_INDEX, StrIdx);
	DDV_MaxChars(pDX, PID, 4);
	DDV_MaxChars(pDX, StrIdx, 6);

	_Pid_i = atoi( LPCTSTR(PID)); 
	PacketIndx = atoi( LPCTSTR(StrIdx)); 
}


BEGIN_MESSAGE_MAP(CDialogPIDIndexInput, CDialog)
END_MESSAGE_MAP()


// CDialogPIDIndexInput 消息处理程序

// CDialogPIDTableIDInput 对话框

IMPLEMENT_DYNAMIC(CDialogPIDTableIDInput, CDialog)
CDialogPIDTableIDInput::CDialogPIDTableIDInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPIDTableIDInput::IDD, pParent)
{
	_Pid_i = -1;
	_TableID_i = -1;
}

CDialogPIDTableIDInput::~CDialogPIDTableIDInput()
{
}

void CDialogPIDTableIDInput::DoDataExchange(CDataExchange* pDX)
{
	CString PID ,TableID;
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PID_TABLE_INPUT_PID, PID);
	DDX_Text(pDX, IDC_EDIT_PID_TABLE_INPUT_TABLEID, TableID);
	DDV_MaxChars(pDX, PID, 4);
	DDV_MaxChars(pDX, TableID, 4);

	_Pid_i = atoi( LPCTSTR(PID)); 
	_TableID_i = atoi( LPCTSTR(TableID)); 
}


BEGIN_MESSAGE_MAP(CDialogPIDTableIDInput, CDialog)
END_MESSAGE_MAP()


