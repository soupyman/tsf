#include "StdAfx.h"

#include "ContentTree.h"
#include <assert.h>

bool CContentTreeNode::AppendMJDTime(u64 MJD , CString & str)
{
	u32 Y,M,D, temp1,temp2;
	Y = (u32)((MJD - 15078.2)/365.25) ;
	temp1 = (u32)(Y * 365.25);
	M = (u32)((MJD - 14956.1 - temp1)/30.6001);
	temp2 = (u32)(M * 30.6001);
	D = (u32)(MJD - 14945 - temp1 - temp2 - 11); /* -11 for actual adjust */
	if((M == 14)||(M == 15))
	{
		Y ++;
		M -= 13;
	}
	else
	{
		M--;
	}
	str.AppendFormat(" : mjd : %d / %d / %d ",Y ,M ,D);
	return true;
}

CString 	CContentTreeNode::GetTitleWithFmtVal()
{
	CString FmtTitle;
	switch(Type)
	{
		case FIELD_NODE:
		{
			CContentFieldNode * pFN = (CContentFieldNode * )this;
			FmtTitle = Title;
			switch(pFN->Format)
			{
				case CContentFieldNode::FIELD_FMT_BIN:
					FmtTitle.AppendFormat(" : 0b ");
					
					for(int i = pFN->Lenth-1  ; i >= 0; i --)
						FmtTitle.AppendFormat("%d", (pFN->Value >> (i))&0x01) ;
					
					break;
				case CContentFieldNode::FIELD_FMT_DEC:
					FmtTitle.AppendFormat(" : 0d %d", pFN->Value);
					break;
				case CContentFieldNode::FIELD_FMT_HEX:
					FmtTitle.AppendFormat(" : 0x %x", pFN->Value);
					break;
				case CContentFieldNode::FIELD_FMT_BCD:
				{
					//assume that: lenth can't be longer then 32 and must be *4
					FmtTitle.AppendFormat(" : bcd ");
					
					for(int i = pFN->Lenth - 4 ; i >= 0; i -= 4)
						FmtTitle.AppendFormat("%d", (pFN->Value >> (i))&0x0F) ;
				}
					break;
				case CContentFieldNode::FIELD_FMT_MJD:
					AppendMJDTime( pFN->Value, FmtTitle);
					break;
				case CContentFieldNode::FIELD_FMT_STR:
				{
					u8 * pC = (u8 *) pFN->Value;
					FmtTitle.AppendFormat(" : 0s ");
					for(u32 i = 0 ; i < pFN->Lenth/8; i ++)
						FmtTitle.AppendFormat("%c", *(pC+i) );
				}
					break;
			}
		}
			break;
		case RESERVE_NODE:
			FmtTitle = "Reserve";
			break;
		default:
			FmtTitle = Title;
			break;
	}
	return FmtTitle;
}

CContentTreeNode * CContentTreeNode::Duplicate()
{
	CContentTreeNode * pCtn = NULL;
	assert(0);
	return pCtn;
}

CContentTreeNode * CContentRootNode::Duplicate()
{
	CContentRootNode * pCtn  = new CContentRootNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	return (CContentTreeNode *)pCtn;
}

CContentTreeNode * CContentReserveNode::Duplicate()
{
	CContentReserveNode * pCtn = new CContentReserveNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->Index = Index;
	pCtn->Lenth = Lenth;		
	return (CContentTreeNode *)pCtn;
}

CContentTreeNode * CContentFieldNode::Duplicate()
{
	CContentFieldNode * pCtn = new CContentFieldNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->Index = Index;
	pCtn->Lenth = Lenth;
	pCtn->Value = Value;
	pCtn->Format = Format;
		
	return (CContentTreeNode *)pCtn;
}
CContentFieldNode::~CContentFieldNode(void)
{
	if((Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))||(Format == CContentFieldNode::FIELD_FMT_STR))
	{
		free( (void * )Value);
	}
}

CContentTreeNode * CContentTableNode::Duplicate()
{
	CContentTableNode * pCtn = new CContentTableNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->pTable = pTable;
	return (CContentTreeNode *)pCtn;
}
CContentTreeNode * CContentSectionNode::Duplicate()
{
	CContentSectionNode * pCtn = new CContentSectionNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->pSection = pSection;
	//pCtn->DataLenth = DataLenth;
	
	return (CContentTreeNode *)pCtn;
}
CContentTreeNode * CContentSubBodyNode::Duplicate()
{
	CContentSubBodyNode * pCtn = new CContentSubBodyNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->Index = Index;
	pCtn->Lenth = Lenth;
	return (CContentTreeNode *)pCtn;
}
bool CContentSubBodyNode::Verified()
{
	if(Type == SUBBODY_NODE)
		return true;
	else
		return false;
}

CContentTreeNode * CContentPacketNode::Duplicate()
{
	CContentPacketNode * pCtn = new CContentPacketNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->pES = pES;
	pCtn->PacketIdx = PacketIdx;
	return (CContentTreeNode *)pCtn;
}

CContentTree::CContentTree(CContentTreeNode * Node_a,CContentTree * fa,CContentTree * Bro,CContentTree * Cld)
		:CCommonTree(Node_a , fa ,  Bro ,  Cld )
{

}


CContentTree::~CContentTree(void)
{
}


CContentTree * CContentTree::Duplicate()
{
	CContentTreeNode * pNN = NULL, * pON;
	pON = (CContentTreeNode *)(GetMyNode());
	if(pON)
		pNN =  pON->Duplicate();

	CContentTree * pNewTree = new CContentTree(pNN);
	
	/*  */
	CContentTree * pCCT = (CContentTree *)GetChildren();
#if 0
	if(pCCT)
		pNewTree->AddChild(pCCT->Duplicate());
	
	pCCT = (CContentTree *)RightBrother;

	if(pCCT)
		pNewTree->AddBrother(pCCT->Duplicate());
#else
	while(pCCT)
	{
		pNewTree->AddChild(pCCT->Duplicate());
		pCCT = (CContentTree *)(pCCT->GetRightBrother());
	}
#endif
	return pNewTree;
}

CContentTreeNode::TreeNodeType CContentTree::GetNodeType()
{
	return ((CContentTreeNode *)GetMyNode())->Type ; 
}

void * CContentTree::GetOrigin()
{
	if(GetNodeType() == CContentTreeNode::TABLE_NODE)
		return (void *)(((CContentTableNode *)GetMyNode())->pTable);
	if(GetNodeType() == CContentTreeNode::PACKET_NODE)
		return (void *)(((CContentPacketNode *)GetMyNode())->pES);

	CContentTree * pct = (CContentTree *)GetFather() ;
	if(pct)
		return pct->GetOrigin();

	assert(0);
	return NULL;
}
/*
function : search back from this to root 
return for same string found
*/
bool CContentTree::SearchBackFieldValue( CString * pSLabel , u64 & Ret)
{
	CContentTree * NextSearchTar = this;

	while(NextSearchTar)
	{
		CContentTreeNode * pNode = (CContentTreeNode *)NextSearchTar->GetMyNode();
		
		if((pNode->Type == CContentTreeNode::FIELD_NODE )&&(-1 != pNode->Title.Find(*pSLabel)))
		{
			CContentFieldNode * pTnf = (CContentFieldNode *)pNode;
			Ret = pTnf->Value;
			return true;
		}
		NextSearchTar = (CContentTree *)NextSearchTar->GetLeftBrother();
	}
	NextSearchTar = (CContentTree *)GetFather();
	
	if((NextSearchTar)&& NextSearchTar->SearchBackFieldValue( pSLabel  , Ret))
		return true;

	assert(0);
	return false;
}


CContentTree *  CContentTree::SearchBranchByNode(CContentTreeNode * Node_a)
{
	/*  */
	CContentTree * pCCT ;
	
	if((CContentTreeNode *)(GetMyNode()) == Node_a)
	{
		return this;
	}
	
	if((pCCT = (CContentTree *)GetChildren())&&(pCCT = pCCT->SearchBranchByNode(Node_a) ))
	{
		return pCCT;
	}
	
	if((pCCT = (CContentTree *)GetRightBrother())&&(pCCT = pCCT->SearchBranchByNode(Node_a) ))
	{
		return pCCT;
	}
	return NULL;
}
/*
bool  CContentTree::DescetIdx( u16  DesCnt)
{

	CContentTree * pCTC = (CContentTree *)GetChildren();
	//descent self
	if(GetNodeType() == CContentTreeNode::SUBBODY_NODE)
	{
		 ((CContentSubBodyNode * )GetMyNode())->Index -= DesCnt;
		while(pCTC)
		{
			pCTC->DescetIdx(DesCnt);
			pCTC = (CContentTree *)pCTC->GetRightBrother();
		}
	}
	else if(GetNodeType() == CContentTreeNode::FIELD_NODE)
	{
		 ((CContentFieldNode * )GetMyNode())->Index -= DesCnt;
	}
	else
		return false;
	
	
	return true;
}


bool  CContentTree::AdjustRelativeIdxForIsolate()
{

	u32  DesCnt;
	CContentTree * pCCF = (CContentTree *)GetFather();
	CContentTree * pCCT = (CContentTree *)GetRightBrother();
	//	judge what we are		
	if(GetNodeType() == CContentTreeNode::SUBBODY_NODE)
	{
		DesCnt = ((CContentSubBodyNode * )GetMyNode())->Lenth;
	}
	else if(GetNodeType() == CContentTreeNode::FIELD_NODE)
	{
		DesCnt = ((CContentFieldNode * )GetMyNode())->Lenth;
	}
	else
		return false;
	
	while(pCCT)
	{
		pCCT->DescetIdx(DesCnt);
		pCCT = (CContentTree *)pCCT->GetRightBrother();
	}

	while(pCCF )
	{
		if(pCCF->GetNodeType() == CContentTreeNode::SUBBODY_NODE)
		{
			 ((CContentSubBodyNode * )pCCF->GetMyNode())->Lenth -= DesCnt;

			pCCT = (CContentTree *)pCCF->GetRightBrother();
			while(pCCT)
			{
				pCCT->DescetIdx(DesCnt);
				pCCT = (CContentTree *)pCCT->GetRightBrother();
			}
		}
		else if(pCCF->GetNodeType() == CContentTreeNode::SECTION_NODE)
		{
			((CContentSectionNode * )pCCF->GetMyNode())->DataLenth -= (DesCnt/8);
			break;
		}
		pCCF = (CContentTree *)pCCF->GetFather();
	}
	return true;
}
*/
