//#define _NOINFOM

#include "formatdb.h"


CFormatDbNode::~CFormatDbNode(void)
{
	if(pFmtTree )
		delete pFmtTree;
}




void CFmtDbInterface::DumpToStr(CString & TraceStr)
{	
	CFormatDbNode * node;
	CFormatTree * pft;
	CTokenNode * ptn;
	node = (CFormatDbNode *)s_FmtDb.GetHeadNode();
	while(node)
	{
		pft = node->GetFmtTree();
		ptn = (CTokenNode *)pft->GetMyNode();
		
		switch( ptn->NodeType)
		{
			case CTokenNode::TABLE_HEAD_NODE:	
			case CTokenNode::DESCRIPTOR_HEAD_NODE:
			{
				pft->DumpToClearString(TraceStr,0);
				break;
			}
			default:
			{
				TraceStr.AppendFormat("\r\n <unexpect Format Tree(%d) found >\r\n",ptn->NodeType);
				break;
			}
		}
		node = (CFormatDbNode *)(node->pNext);
	}
}



bool CFmtDbInterface::AddFormat(CFormatTree * pFT)
{
	//INF(" %s ", pFT->GetFmtTreeName().c_str());

	CFormatDbNode * pNode = new CFormatDbNode(pFT);
	s_FmtDb.AddNodeToHead(pNode );
	return true;
}

bool	CFmtDbInterface::RemoveFormat(CFormatTree * pFT)
{
	CFormatDbNode * node = (CFormatDbNode * )s_FmtDb.GetHeadNode();
	while(node)
	{
		if(node->GetFmtTree() == pFT)
		{
			s_FmtDb.RemoveNode(node);
		}
		node = (CFormatDbNode * )(node->pNext);
	}
	return true;
}

CFormatTree * CFmtDbInterface::GetDescriptorFormat(u8 DescTag)
{
	CFormatDbNode * node = (CFormatDbNode * )s_FmtDb.GetHeadNode();
	CTokenNode * ptn ;

	while(node)
	{
		ptn = (CTokenNode * )(node->GetFmtTree()->GetMyNode());
		if(ptn->NodeType == CTokenNode::DESCRIPTOR_HEAD_NODE)
		{
			CDescriptorHeadNode * pHd = (CDescriptorHeadNode *)(ptn);
			if(pHd->ID == DescTag)
			{
				return node->GetFmtTree();
			}
		}
		node = (CFormatDbNode * )(node->pNext);
	}
	return NULL;
}

CFormatTree * CFmtDbInterface::GetTableFormat(u16 TableId)
{
	CFormatDbNode * node = (CFormatDbNode * )s_FmtDb.GetHeadNode();
	CTokenNode * ptn ;

	while(node)
	{
		ptn = (CTokenNode * )(node->GetFmtTree()->GetMyNode());
		if(ptn->NodeType == CTokenNode::TABLE_HEAD_NODE)
		{
			CTableHeadNode * pHd = (CTableHeadNode *)(ptn);
			if(pHd->ID == TableId)
			{
				return node->GetFmtTree();
			}
		}
		node = (CFormatDbNode * )(node->pNext);
	}
	return NULL;
}



bool CFmtDbInterface::FreeAllFormat()
{
	s_FmtDb.ClearNodes();
	return true;
}

bool	CFmtDbInterface::CheckFormatHead(CFormatTree * pFT , CString & ErrInfo)
{
	CTokenNode * ptn;
	ptn = (CTokenNode *)pFT->GetMyNode();
	
	switch( ptn->NodeType)
	{
		case CTokenNode::TABLE_HEAD_NODE:	
		{
			CTableHeadNode * pTHN = (CTableHeadNode *)ptn;
			if(GetTableFormat(pTHN->ID))
			{
				ErrInfo.AppendFormat("Duplicate Table format found: <table : %s : %d>", pTHN->Name.c_str() ,pTHN->ID);
				ErrInfo.AppendFormat("\r\n this normally happend by redefine or reinclude. \r\n");
				return false;
			}
		}
			break;
		case CTokenNode::DESCRIPTOR_HEAD_NODE:
		{
			CDescriptorHeadNode * pDHN = (CDescriptorHeadNode *)ptn;
			if(GetDescriptorFormat(pDHN->ID))
			{
				ErrInfo.AppendFormat("Duplicate Descriptor format found: <descriptor : %s : %d>", pDHN->Name.c_str() ,pDHN->ID);
				ErrInfo.AppendFormat("\r\n this normally happend by redefine or reinclude. \r\n");
				return false;
			}
		}
			break;
		default:
		{
			ErrInfo.AppendFormat("Format type(%d) not format head. \r\n", ptn->NodeType);
			return false;
		}
			break;
	}
	return true;
}

CFormatDb * CFmtDbInterface::GetFormatDb()
{
	return &(s_FmtDb);
}



CFormatDb    CFmtDbInterface::s_FmtDb ;




