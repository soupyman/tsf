
#include "StdAfx.h"
#include "include.h"


CTwmList    CFormatDatabase::PrivateFormatList ;


CFormatDatabase::CFormatDatabase(void)
{
}

CFormatDatabase::~CFormatDatabase(void)
{
}

bool CFormatDatabase::FreeFormatList()
{
	POSITION pos = PrivateFormatList.GetHeadPosition();
	CCommonTree * ptn;
	while(pos)
	{
		ptn = (CCommonTree *)(PrivateFormatList.GetNext(pos));
		delete ptn;
	}
	PrivateFormatList.RemoveAll();
	return true;
}

bool 	CFormatDatabase::AddFormat(CFormatTree * pFT)
{
	/*check if same exist first */
	PrivateFormatList.AddTail(pFT);
	return true;
}

bool	CFormatDatabase::RemoveFormat(CFormatTree * pFT)
{
	POSITION pos_b, pos = PrivateFormatList.GetHeadPosition();
	CFormatTree * ptn;
	while(pos)
	{
		pos_b = pos;
		ptn = (CFormatTree *)(PrivateFormatList.GetNext(pos));
		if(ptn == pFT)
		{
			PrivateFormatList.RemoveAt(pos_b);
			delete ptn;
		}
	}
	return true;
}

bool	CFormatDatabase::CheckFormatHead(CFormatTree * pFT , CString & ErrInfo)
{
	CTokenNode * ptn;
	ptn = (CTokenNode *)pFT->GetMyNode();
	
	switch( ptn->NodeType)
	{
		case CTokenNode::TABLE_HEAD_NODE:	
		{
			CTableHeadNode * pTHN = (CTableHeadNode *)ptn;
			if(GetPrivateTableFormatByID(pTHN->ID))
			{
				ErrInfo.AppendFormat("Duplicate Table format found: <table : %s : %d>", pTHN->Name ,pTHN->ID);
				ErrInfo.AppendFormat("\r\n this normally happend by redefine or reinclude. \r\n");
				return false;
			}
		}
			break;
		case CTokenNode::DESCRIPTOR_HEAD_NODE:
		{
			CDescriptorHeadNode * pDHN = (CDescriptorHeadNode *)ptn;
			if(GetPrivateDescriptorFormatByTag(pDHN->ID))
			{
				ErrInfo.AppendFormat("Duplicate Descriptor format found: <descriptor : %s : %d>", pDHN->Name ,pDHN->ID);
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

CTwmList * CFormatDatabase::GetPrivateFormatList()
{
	return &(PrivateFormatList);
}

CFormatTree * CFormatDatabase::GetPrivateDescriptorFormatByTag(u8 tag)
{
	CTwmList *	pfl = GetPrivateFormatList();
	
	POSITION pos = pfl->GetHeadPosition();
	while(pos)
	{
		CFormatTree * pft = (CFormatTree*)(pfl->GetNext(pos));
		CTokenNode * ptn = (CTokenNode *)(pft->GetMyNode());
		if(ptn->NodeType == CTokenNode::DESCRIPTOR_HEAD_NODE)
		{
			CDescriptorHeadNode * pHd = (CDescriptorHeadNode *)(ptn);
			if(pHd->ID == tag)
				return pft;
		}
	}
	return NULL;
}

CFormatTree * CFormatDatabase::GetPrivateTableFormatByID(u16 ID)
{
	CTwmList *	pfl = GetPrivateFormatList(); //CTokenNode list
	
	POSITION pos = pfl->GetHeadPosition();
	while(pos)
	{
		CFormatTree * pft = (CFormatTree*)(pfl->GetNext(pos));
		CTokenNode * ptn = (CTokenNode *)pft->GetMyNode();
		
		if(ptn->NodeType == CTokenNode::TABLE_HEAD_NODE)
		{
			CTableHeadNode * pHd = (CTableHeadNode *)(ptn);
			if(pHd->ID == ID)
				return pft;
		}
	}
	return NULL;
}




void CFormatDatabase::DumpToStr(CString & TraceStr)
{	
	POSITION pos;
	CFormatTree * pft;
	CTokenNode * ptn;
	pos = PrivateFormatList.GetHeadPosition();
	while(pos)
	{
		pft = (CFormatTree * )PrivateFormatList.GetNext(pos);
		ptn = (CTokenNode *)pft->GetMyNode();
		
		switch( ptn->NodeType)
		{
			case CTokenNode::TABLE_HEAD_NODE:	
			case CTokenNode::DESCRIPTOR_HEAD_NODE:
			{
				pft->DumpToString(TraceStr,0);
				break;
			}
			default:
			{
				TraceStr.AppendFormat("\r\n <unexpect Format Tree(%d) found >\r\n",ptn->NodeType);
				break;
			}
		}
	}
}

