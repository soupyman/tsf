
#include "StdAfx.h"
#include "FormatTree.h"
#include "Formula.h"

CTableHeadNode::CTableHeadNode()
{
	NodeType = TABLE_HEAD_NODE;
}

CTableHeadNode::~CTableHeadNode()
{
}

CDescriptorHeadNode::CDescriptorHeadNode()
{
	NodeType = DESCRIPTOR_HEAD_NODE;
}

CDescriptorHeadNode::~CDescriptorHeadNode()
{
}

CReserveNode::CReserveNode()
{
	NodeType = RESERVE_NODE;
}
CReserveNode::~CReserveNode()
{
	if(pLengthFml)
		delete pLengthFml;
}

CDataNode::CDataNode()
{
	NodeType = DATA_NODE;
	format = FIELD_FMT_HEX ; // default value of the field
}

CDataNode::~CDataNode()
{
	if(pLengthFml)
		delete pLengthFml;
}

CKeyLoopNode::CKeyLoopNode()
{
	NodeType = KEY_WORD_LOOP;
	Name.Empty();
}

CKeyLoopNode::~CKeyLoopNode()
{
	if(pFml)
		delete pFml;
}

CKeyIfNode::CKeyIfNode()
{
	NodeType = KEY_WORD_IF;
}

CKeyIfNode::~CKeyIfNode()
{
	if(pFml)
		delete pFml;
}

CKeyElseNode::CKeyElseNode()
{
	NodeType = KEY_WORD_ELSE;
}

CKeyElseNode::~CKeyElseNode()
{
}

CKeyDescriptorNode::CKeyDescriptorNode()
{
	NodeType = KEY_WORD_DESCRIPTOR;
}

CKeyDescriptorNode::~CKeyDescriptorNode()
{
}

CFormatTree::CFormatTree(CTokenNode * Node_a,CFormatTree * fa,CFormatTree * Bro,CFormatTree * Cld)
		:CCommonTree(Node_a , fa ,  Bro ,  Cld )
{

}


CFormatTree::~CFormatTree(void)
{
}

/*
u32 CFormatTree::CalculateFormatLenth()
{
	u32 lenth = 0;
	CTokenNode * pTN = (CTokenNode *)GetMyNode();
	lenth += pTN->GetLength();
	
	CFormatTree * child = (CFormatTree *)GetChildren();
	while(child)
	{
		lenth += child->CalculateFormatLenth();
		child = (CFormatTree *)child->GetRightBrother();
	}
	return lenth;
}
*/
/*
this search back 
only check if there is a field name same;
will not care if latest or not.
*/
bool CFormatTree::SearchBackField( CString * pSLabel)
{
	CFormatTree * BigBrother =(CFormatTree *)GetBigBrother();

	while(BigBrother)
	{
		CTokenNode * pNode = (CTokenNode *)BigBrother->GetMyNode();
		
		if((pNode->NodeType == CTokenNode::DATA_NODE )&&(-1 != ((CDataNode *)pNode)->NameString.Find(*pSLabel)))
		{
			return true;
		}

		if(BigBrother == this)
		{
			break;
		}
		BigBrother = (CFormatTree *)BigBrother->GetRightBrother();
	}
	BigBrother = (CFormatTree *)GetFather();
	
	if((BigBrother)&& BigBrother->SearchBackField( pSLabel ))
		return true;

	return false;
}

bool CFormatTree::DumpToString(CString & TraceStr,u8 NestDeep)
{
	
/*	dump current node	*/
	//CFormatTree * SubEnd = NULL;

	for(u8 Ti =0 ; Ti < (NestDeep); Ti ++)
		TraceStr.AppendFormat("\t");
	
	CTokenNode * ptn = (CTokenNode *)GetMyNode();
	switch(ptn->NodeType)
	{
		case CTokenNode::DESCRIPTOR_HEAD_NODE:
		{
			CDescriptorHeadNode * pdn = (CDescriptorHeadNode *)ptn  ;
			TraceStr.AppendFormat("\r\n<descriptor : %s : %d > \r\n" ,pdn->Name , pdn->ID);
		}
		break;
		case CTokenNode::TABLE_HEAD_NODE:
		{
			CTableHeadNode * pdn = (CTableHeadNode *)ptn  ;
			TraceStr.AppendFormat("\r\n<table	: %s : %d > \r\n" ,pdn->Name , pdn->ID);
		}				
			break;
		case CTokenNode::RESERVE_NODE:
		{
			CReserveNode * pRn = (CReserveNode *)ptn  ;
			TraceStr.AppendFormat("Reserve : " );
			pRn->pLengthFml->DumpToString(TraceStr);
			TraceStr.AppendFormat("\r\n");
		}
			break;
		case CTokenNode::DATA_NODE:
		{
			CDataNode * pdn = (CDataNode *)ptn  ;
			TraceStr.AppendFormat("%s :  ",pdn->NameString );
			pdn->pLengthFml->DumpToString(TraceStr);
			switch(pdn->format)
			{
				case CDataNode::FIELD_FMT_BIN:
					TraceStr.AppendFormat(" : b \r\n");
					break;
				case CDataNode::FIELD_FMT_DEC:
					TraceStr.AppendFormat(" : d \r\n");
					break;
				case CDataNode::FIELD_FMT_HEX:
					TraceStr.AppendFormat(" : x \r\n");
					break;
				case CDataNode::FIELD_FMT_BCD:
					TraceStr.AppendFormat(" : c \r\n");
					break;
				case CDataNode::FIELD_FMT_MJD:
					TraceStr.AppendFormat(" : m \r\n");
					break;
				case CDataNode::FIELD_FMT_STR:
					TraceStr.AppendFormat(" : s \r\n");
					break;
				default:
					TraceStr.AppendFormat(" : @ \r\n");
					break;
			}
		}
			break;
		case CTokenNode::KEY_WORD_DESCRIPTOR:
		{
			TraceStr.AppendFormat("[descriptor] \r\n");
		}
			break;
		case CTokenNode::KEY_WORD_LOOP:
		{
			CKeyLoopNode * pdn = (CKeyLoopNode *)ptn  ;
			if( pdn->Name.IsEmpty())
			{
				TraceStr.AppendFormat("[loop]( ");
			}
			else
			{
				TraceStr.AppendFormat("[loop : %s]( ", pdn->Name);
			}
			pdn->pFml->DumpToString(TraceStr);
			TraceStr.AppendFormat(" )\r\n");
		}
			break;
		case CTokenNode::KEY_WORD_IF:
		{
			CKeyIfNode * pdn = (CKeyIfNode *)ptn  ;
			//SubEnd = pdn->NoContext;
			TraceStr.AppendFormat("[if]( ");
			pdn->pFml->DumpToString(TraceStr);
			TraceStr.AppendFormat(" )\r\n");
		}
			break;
		case CTokenNode::KEY_WORD_ELSE:
		{
			TraceStr.AppendFormat("[else] \r\n");
		}
			break;
	}
/*	dump children node	*/
	if(GetChildren())
	{
		for(u8 Ti =0 ; Ti < NestDeep; Ti ++)
			TraceStr.AppendFormat("\t");
		TraceStr.AppendFormat("{ \r\n");
	}
	CFormatTree * child = (CFormatTree *)GetChildren();

	while(child )
	{
		child->DumpToString(TraceStr,NestDeep+1);
		child = (CFormatTree *)(child->GetRightBrother());
	}
	if(GetChildren())
	{
		for(u8 Ti =0 ; Ti < NestDeep; Ti ++)
			TraceStr.AppendFormat("\t");
		TraceStr.AppendFormat("} \r\n");
	}	
	
	return true;
}
