// EditSection.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "include.h"
#include "EditSection.h"
#include "FormatServer.h"
#include "SystemConfig.h"


// CEditSection 对话框

IMPLEMENT_DYNAMIC(CEditSection, CDialog)
CEditSection::CEditSection(CWnd* pParent /*=NULL*/)
	: CDialog(CEditSection::IDD, pParent)
{
	//SectionContent = "";
	pCntTree = NULL;
	LastEnd = 0;
	NewSecLenth = 0;

}

CEditSection::~CEditSection()
{
	if(pCntTree)
		delete pCntTree;

}

void CEditSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SECTION, SectionContent);
	DDX_Control(pDX, IDC_EDIT_HINT, HintControl);
	DDX_Control(pDX, IDC_EDIT_SECTION, SectionContentControl);
}


BEGIN_MESSAGE_MAP(CEditSection, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRE_CHECK, OnBnClickedButtonPreCheck)
	ON_EN_KILLFOCUS(IDC_EDIT_SECTION, OnEnKillfocusEditSection)
END_MESSAGE_MAP()


// CEditSection 消息处理程序

BOOL CEditSection::OnInitDialog()
{
	CDialog::OnInitDialog();
	SectionContent.Format	("\t\t# How do edit section \r\n\r\n");
	SectionContent.AppendFormat("#{ means sub body start \r\n");
	SectionContent.AppendFormat("#} means sub body end\r\n");
	SectionContent.AppendFormat("# '#' means comment, like \\\\ in C++ \r\n");
	SectionContent.AppendFormat("# CRC is calcuate automaticly, no need to edit \r\n");
	SectionContent.AppendFormat("# field_name: field_lenth: field_value #content format\r\n");
	SectionContent.AppendFormat("# field_lenth is bits count in decimal; field_value is in hex.\r\n\r\n\r\n");

	AttachTree(pCntTree , SectionContent);
	
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CEditSection::OnBnClickedButtonPreCheck()
{
	// check if it's format ok for a integrated section
	UpdateData(true);
	if(SectionContent.PreCheckSectionContent())
	{
		MessageBox("Seems ok. :)");
	}
	else
	{
		MessageBox(SectionContent.ErrorInfo, "ERROR", MB_ICONWARNING);
	}
}

void CEditSection::OnOK()
{
	CSystemConfig SysCfg;

	UpdateData(true);
	NewSecLenth = 0;
	if((FALSE == SysCfg.ForcePrecheck)||(SectionContent.PreCheckSectionContent()))
	{//ok we read it
		SectionContent.ReadSectionDataTo(NewSecData, NewSecLenth);
	}
	else
	{
		MessageBox(SectionContent.ErrorInfo, "ERROR", MB_ICONWARNING);
	}

	CDialog::OnOK();
}

void CEditSection::OnEnKillfocusEditSection()
{
	int nSelStart = 0, nSelEnd = 0 , SelContentLenth;
	UpdateData(true);
	SectionContentControl.GetSel(nSelStart, nSelEnd);

	CString strSelString ;//= SectionContent.Mid(nSelStart, nSelEnd - nSelStart + 1);

	SelContentLenth = SectionContent.GetContentLengthInBit(nSelStart, nSelEnd);

	strSelString.Format("Select:%d bit or 0x%x byte + 0x%x bit",SelContentLenth,SelContentLenth/8,SelContentLenth%8);
	HintControl.SetWindowText(strSelString);
}

void CEditSection::AttachTree( CCommonTree *	pTT , CString & parent)
{
	CContentTreeNode * pNode = (CContentTreeNode *)(pTT->GetMyNode());
	CCommonTree *	pTTemp = pTT;
	CString CurLine;
	
	CurLine.Empty();
	while(pTTemp = pTTemp->GetFather())
		CurLine.AppendFormat("\t");

	switch(pNode->Type)
	{
		case CContentTreeNode::SUBBODY_NODE:
			{
				CContentSubBodyNode * pSBN = (CContentSubBodyNode *)pNode;
				if(LastEnd && (LastEnd < pSBN->Index))/*never first one , may be reserve befor sub body*/	
				{
					CurLine.AppendFormat("Reserve:%d: 0 \r\n", pSBN->Index - LastEnd);
					
					pTTemp = pTT;
					while(pTTemp = pTTemp->GetFather())
						CurLine.AppendFormat("\t");
				}
				LastEnd = pSBN->Index ;
				CurLine.AppendFormat("#");
				CurLine.Append(pNode->Title);
			}
			break;
		case CContentTreeNode::RESERVE_NODE:
			{
				CContentReserveNode * pCRN = (CContentReserveNode *)pNode;
				if(LastEnd && (LastEnd < pCRN->Index))/*never first one */	
				{
					CurLine.AppendFormat("Reserve:%d: 0 \r\n", pCRN->Index - LastEnd);
					
					pTTemp = pTT;
					while(pTTemp = pTTemp->GetFather())
						CurLine.AppendFormat("\t");
				}
				LastEnd = pCRN->Index + pCRN->Lenth;
				
				CurLine.AppendFormat("Reserve:%d: 0 ", pCRN->Lenth);
			}
			break;
		case CContentTreeNode::FIELD_NODE:
			{
				CContentFieldNode * pCFN = (CContentFieldNode *)pNode;
				if(LastEnd && (LastEnd < pCFN->Index))/*never first one */
				{
					CurLine.AppendFormat("Reserve:%d: 0\r\n", pCFN->Index - LastEnd);
					
					pTTemp = pTT;
					while(pTTemp = pTTemp->GetFather())
						CurLine.AppendFormat("\t");
				}
				LastEnd = pCFN->Index + pCFN->Lenth;
				
				pNode->Title.Replace(":","|");
				CurLine.Append(pNode->Title);

				CurLine.AppendFormat(":%d:", pCFN->Lenth);
				if((pCFN->Format != CContentFieldNode::FIELD_FMT_STR)&&
					(pCFN->Lenth <= (VALUE_MAX_LENGTH_IN_BYTE*8)))
				{
					CurLine.AppendFormat("%x",pCFN->Value);
				}
				else
				{
					u8 * pU8 = (u8 *)((u64)pCFN->Value);
					for(u32 i = 0 ; i < pCFN->Lenth;  i += 8)
					{
						CurLine.AppendFormat("%02x",*pU8++);
					}
				}
				if(0 <= pCFN->Help.GetLength())
				{
					//CurLine.AppendFormat("\r\n");
					//CurLine.Append(pCFN->Help);
				}
			}
			break;
		case CContentTreeNode::ROOT_NODE:
		case CContentTreeNode::TABLE_NODE:
		case CContentTreeNode::SECTION_NODE:
		case CContentTreeNode::PACKET_NODE:
			{
				CurLine.AppendFormat("#");
				LastEnd = 0;
				CurLine.Append(pNode->Title);
			}
			break;
		default:
			{
				assert(0);
				LastEnd = 0;
			}
			break;
	}
	CurLine.AppendFormat("\r\n");
	parent.Append(CurLine);
		
	if(pTT->GetChildren())
	{
		pTTemp = pTT;
		while(pTTemp = pTTemp->GetFather())
			parent.AppendFormat("\t");
		parent.AppendFormat("{\r\n");
		
		AttachTree(pTT->GetChildren() , parent);
		
		pTTemp = pTT;
		while(pTTemp = pTTemp->GetFather())
			parent.AppendFormat("\t");
		parent.AppendFormat("}\r\n");
	}
	if(pTT->GetRightBrother())
	{
		AttachTree(pTT->GetRightBrother() , parent);
	}	
}



