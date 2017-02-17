// crc_calculater.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "crc_calculater.h"

#include <assert.h>

// Ccrc_calculater 对话框

IMPLEMENT_DYNAMIC(Ccrc_calculater, CDialog)
Ccrc_calculater::Ccrc_calculater(CWnd* pParent /*=NULL*/)
	: CDialog(Ccrc_calculater::IDD, pParent)
	, RawData(_T(""))
	, CrcResult(0)
{
}

Ccrc_calculater::~Ccrc_calculater()
{
}

void Ccrc_calculater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RAW_DATA, RawData);
	DDX_Text(pDX, IDC_EDIT_CRC_RESULT, CrcResult);
}


BEGIN_MESSAGE_MAP(Ccrc_calculater, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CALC, OnBnClickedButtonCalc)
END_MESSAGE_MAP()


// Ccrc_calculater 消息处理程序


void Ccrc_calculater::OnBnClickedButtonCalc()
{
	UpdateData(TRUE);
	u8 Data[1000];
	CString DataStr = RawData;
	CString CurByte;
	u16 lenth,index = 0;
	lenth = DataStr.GetLength();
	if(lenth % 2 != 0)
	{
		MessageBox("data lenth % 2 != 0");
		return;
	}
	lenth = (lenth / 2);
	assert(lenth < 1000);
	int CurB;
	while(index < lenth)
	{
		CurByte = DataStr.Left(2);
		CurB = StrToHex(CurByte);
		assert(CurB != -1);
		Data[index] = CurB;
		index ++;
		DataStr.Delete(0,2);
	}
	 
	CrcResult = CrcInstance.CalCrc32(Data, lenth);
	UpdateData(FALSE);
}

int Ccrc_calculater::StrToHex(CString str)
{
	if(str.GetLength() != 2)
		return -1;
	int index = 1, hex = 0;
	CString temp;
	while(index <= 2)
	{
		hex *= 16;
		temp = str.Left(1);
		if(temp.CompareNoCase("0") == 0)
		{
			hex += 0;
		}
		else if(temp.CompareNoCase("1") == 0)
		{
			hex += 1;
		}
		else if(temp.CompareNoCase("2") == 0)
		{
			hex += 2;
		}
		else if(temp.CompareNoCase("3") == 0)
		{
			hex += 3;
		}
		else if(temp.CompareNoCase("4") == 0)
		{
			hex += 4;
		}
		else if(temp.CompareNoCase("5") == 0)
		{
			hex += 5;
		}
		else if(temp.CompareNoCase("6") == 0)
		{
			hex += 6;
		}
		else if(temp.CompareNoCase("7") == 0)
		{
			hex += 7;
		}
		else if(temp.CompareNoCase("8") == 0)
		{
			hex += 8;
		}
		else if(temp.CompareNoCase("9") == 0)
		{
			hex += 9;
		}
		else if(temp.CompareNoCase("a") == 0)
		{
			hex += 10;
		}
		else if(temp.CompareNoCase("b") == 0)
		{
			hex += 11;
		}
		else if(temp.CompareNoCase("c") == 0)
		{
			hex += 12;
		}
		else if(temp.CompareNoCase("d") == 0)
		{
			hex += 13;
		}
		else if(temp.CompareNoCase("e") == 0)
		{
			hex += 14;
		}
		else if(temp.CompareNoCase("f") == 0)
		{
			hex += 15;
		}
		else
		{
			return -1;
		}
		str.Delete(0,1);
		index ++;
	}
	return hex;
}
