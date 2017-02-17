

#include "formattree.h"

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
		:CTree(Node_a , fa ,  Bro ,  Cld )
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
bool CFormatTree::SearchBackField( CString & pSLabel)
{
	CFormatTree * pFather ;
	CFormatTree * BigBrother =(CFormatTree *)GetBigBrother();

	while(BigBrother)
	{
		CTokenNode * pNode = (CTokenNode *)BigBrother->GetMyNode();
		
		if((pNode->NodeType == CTokenNode::DATA_NODE )&&(false != ((CDataNode *)pNode)->NameString.Find(pSLabel)))
		{
			return true;
		}

		if(BigBrother == this)
		{
			break;
		}
		BigBrother = (CFormatTree *)BigBrother->GetRightBrother();
	}
	pFather = (CFormatTree *)GetFather();
	
	if((pFather)&& pFather->SearchBackField( pSLabel ))
		return true;

	return false;
}

bool	CFormatTree::CheckFormulaValid(CFormula * pFml )
{
	CFormulaNode * ptn = (CFormulaNode *)(pFml->GetMyNode());
	switch(ptn->type)
	{
		case CFormulaNode::OPERATOR:
		{
			CFormula * pChildFml1 = (CFormula *)pFml->GetChildren();	
			CFormula * pChildFml2 = (CFormula *)(pChildFml1->GetRightBrother());	
			if(pChildFml2->GetRightBrother())
				return false; /* more than 2 sons*/ 
			if(false == CheckFormulaValid( pChildFml1))
				return false;
			if(false == CheckFormulaValid( pChildFml2))
				return false;
		}
		break;
		case CFormulaNode::PARAMETER:
		{
			CFormulaParameterNode * pFPN = (CFormulaParameterNode *)ptn;
			if(false == (SearchBackField(pFPN->param)))
				return false;
		}
		break;
		case CFormulaNode::NUMERAL:
		{
			CFormulaNumeralNode * pFNN = (CFormulaNumeralNode *)ptn;
			if(pFNN->num == 0)
			{
				//DGB("caution: found a '0' in formular!");
				/* number 0 maybe meanless .*/
			}
		}
		break;
		default:
			return false;
	}
	return true;
}

bool CFormatTree::DumpToClearString(CString & TraceStr,u8 NestDeep)
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
			TraceStr.AppendFormat("\r\n<descriptor : %s : %d > \r\n" ,pdn->Name.c_str() , pdn->ID);
		}
		break;
		case CTokenNode::TABLE_HEAD_NODE:
		{
			CTableHeadNode * pdn = (CTableHeadNode *)ptn  ;
			TraceStr.AppendFormat("\r\n<table	: %s : %d : crc check = %s> \r\n" ,
				pdn->Name.c_str() , pdn->ID, pdn->CrcCheck?"true":"fasle");
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
			TraceStr.AppendFormat("%s :  ",pdn->NameString.c_str() );
			pdn->pLengthFml->DumpToString(TraceStr);
			switch(pdn->format)
			{
				case FIELD_FMT_BIN:
					TraceStr.AppendFormat(" : b \r\n");
					break;
				case FIELD_FMT_DEC:
					TraceStr.AppendFormat(" : d \r\n");
					break;
				case FIELD_FMT_HEX:
					TraceStr.AppendFormat(" : x \r\n");
					break;
				case FIELD_FMT_BCD:
					TraceStr.AppendFormat(" : c \r\n");
					break;
				case FIELD_FMT_MJD:
					TraceStr.AppendFormat(" : m \r\n");
					break;
				case FIELD_FMT_STR:
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
				TraceStr.AppendFormat("[loop : %s]( ", pdn->Name.c_str());
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
		child->DumpToClearString(TraceStr,NestDeep+1);
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

bool CFormatTree::DumpToXmlString(CString & TraceStr,u8 NestDeep)
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
			TraceStr.AppendFormat("\r\n<DESCRIPTOR name='%s' id='%d' > \r\n" ,pdn->Name.c_str() , pdn->ID);
		}
		break;
		case CTokenNode::TABLE_HEAD_NODE:
		{
			CTableHeadNode * pdn = (CTableHeadNode *)ptn  ;
			TraceStr.AppendFormat("\r\n<TABLE	name='%s'  id='%d' > \r\n" ,pdn->Name.c_str() , pdn->ID);
		}				
			break;
		case CTokenNode::RESERVE_NODE:
		{
			CReserveNode * pRn = (CReserveNode *)ptn  ;
			TraceStr.AppendFormat("<RESERVE lenth='" );
			pRn->pLengthFml->DumpToString(TraceStr);
			TraceStr.AppendFormat("'>\r\n");
		}
			break;
		case CTokenNode::DATA_NODE:
		{
			CDataNode * pdn = (CDataNode *)ptn  ;
			TraceStr.AppendFormat("<FIELD name='%s'  lenth='",pdn->NameString.c_str() );
			pdn->pLengthFml->DumpToString(TraceStr);
			switch(pdn->format)
			{
				case FIELD_FMT_BIN:
					TraceStr.AppendFormat(" : b \r\n");
					break;
				case FIELD_FMT_DEC:
					TraceStr.AppendFormat(" : d \r\n");
					break;
				case FIELD_FMT_HEX:
					TraceStr.AppendFormat("' format='hex' >\r\n");
					break;
				case FIELD_FMT_BCD:
					TraceStr.AppendFormat(" : c \r\n");
					break;
				case FIELD_FMT_MJD:
					TraceStr.AppendFormat(" : m \r\n");
					break;
				case FIELD_FMT_STR:
					TraceStr.AppendFormat("' format='str' >\r\n");
					break;
				default:
					TraceStr.AppendFormat(" : @ \r\n");
					break;
			}
		}
			break;
		case CTokenNode::KEY_WORD_DESCRIPTOR:
		{
			TraceStr.AppendFormat("<DESCRIPTOR >\r\n");
		}
			break;
		case CTokenNode::KEY_WORD_LOOP:
		{
			CKeyLoopNode * pdn = (CKeyLoopNode *)ptn  ;
			if( pdn->Name.IsEmpty())
			{
				TraceStr.AppendFormat("<LOOP lenth='");
			}
			else
			{
				TraceStr.AppendFormat("<LOOP name='%s' lenth='", pdn->Name.c_str());
			}
			pdn->pFml->DumpToString(TraceStr);
			TraceStr.AppendFormat(" >\r\n");
		}
			break;
		case CTokenNode::KEY_WORD_IF:
		{
			CKeyIfNode * pdn = (CKeyIfNode *)ptn  ;
			//SubEnd = pdn->NoContext;
			TraceStr.AppendFormat("<IF condition='");
			pdn->pFml->DumpToString(TraceStr);
			TraceStr.AppendFormat("'>\r\n");
		}
			break;
		case CTokenNode::KEY_WORD_ELSE:
		{
			TraceStr.AppendFormat("<ELSE> \r\n");
		}
			break;
	}
/*	dump children node	*/
	CFormatTree * child = (CFormatTree *)GetChildren();

	while(child )
	{
		child->DumpToXmlString(TraceStr,NestDeep+1);
		child = (CFormatTree *)(child->GetRightBrother());
	}	
	return true;
}

CString CFormatTree::GetFmtTreeName(void)
{
	CTokenNode * ptn = (CTokenNode *)GetMyNode();
	CString ret;
	switch(ptn->NodeType)
	{
		case CTokenNode::DESCRIPTOR_HEAD_NODE:
		{
			CDescriptorHeadNode * pdn = (CDescriptorHeadNode *)ptn  ;
			ret.Format("<DESCRIPTOR format tree name='%s' id='%d' >" ,pdn->Name.c_str() , pdn->ID);
		}
		break;
		case CTokenNode::TABLE_HEAD_NODE:
		{
			CTableHeadNode * pdn = (CTableHeadNode *)ptn  ;
			ret.Format("<TABLE format tree name='%s'  id='%d' >" ,pdn->Name.c_str() , pdn->ID);
		}
		default:
		break;				
	}
	return ret;
}
