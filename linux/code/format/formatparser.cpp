
#include "formatparser.h"
#include <stdlib.h>
#include <string.h>


/*
this is used for add root node
*/
CContentTree * 	CFormatParser::AddRootBranch(const CString &  pSLabel )
{
	return  AddComBranch(NULL,   pSLabel ,CContentTreeNode::ROOT_NODE,	0 ,0, 0);
}

/*
this is used for add reserve node branch
*/
CContentTree * 	CFormatParser::AddReserveBranch(CContentTree * parent, u32 Index  ,u32 Length)
{
	CString tmp;
	InsertReserveBranchIfNeed(parent , Index);

	return  AddComBranch(parent,   tmp ,CContentTreeNode::RESERVE_NODE,
		0 ,Index, Length);
}

/*
this is used for add sub body node branch
*/
CContentTree * 	CFormatParser::AddSubBodyBranch(CContentTree * parent,const CString & pSLabel ,  u32 Index  ,u32 Length)
{
	InsertReserveBranchIfNeed(parent , Index);
	return  AddComBranch(parent,   pSLabel ,CContentTreeNode::SUBBODY_NODE,
		0 ,Index, Length);
}

/*
this is used for add section node branch
*/
CContentTree * 	CFormatParser::AddSectionBranch(CContentTree * parent,const CString & pSLabel )
{
	return  AddComBranch(parent,   pSLabel ,CContentTreeNode::SECTION_NODE);
}

/*
this is used for add section node branch
*/
CContentTree * 	CFormatParser::AddTableBranch(CContentTree * parent,const CString & pSLabel ,  u64 PID)
{
	return  AddComBranch(parent,   pSLabel ,CContentTreeNode::TABLE_NODE,	PID);
}

/*
this is used for add section node branch
*/
CContentTree * 	CFormatParser::AddPacketBranch(CContentTree * parent,const CString & pSLabel ,u64 PID)
{
	return  AddComBranch(parent,   pSLabel ,CContentTreeNode::PACKET_NODE, PID);
}

/*
this is used for add default branch
Param1: index start
Param2: lenth
Param3: value

*/

CContentTree * 	CFormatParser::AddFieldBranch(CContentTree * parent, const CString & pSLabel , 
	u32 index  ,u32 lenth , u32  format  ,u64 value )
{
	InsertReserveBranchIfNeed(parent , index);
	CContentTree * pBran =  AddComBranch(parent,   pSLabel ,CContentTreeNode::FIELD_NODE, 
		value ,index, lenth,format);

	return pBran;
}

/*
this is used for add default branch
Param1: index start
Param2: lenth
Param3: value

*/
CContentTree * 	CFormatParser::AddDefaultBranch(CContentTree * parent, const CString & pSLabel , 
	u32 index  ,u32 length , u64  value  )
{
	CContentTree * pBran = NULL;
	if(length)
	{
		 pBran =  AddComBranch(parent,   pSLabel ,CContentTreeNode::FIELD_NODE, 
			value ,index, length,FIELD_FMT_HEX);
	}
	return pBran;
}

/*
	Param64: value or a point
	Param2: index
	Param3: length
	Param4: format
*/
CContentTree * CFormatParser::AddComBranch(CContentTree * parent, const CString & pSLabel ,
	CContentTreeNode::TreeNodeType nt,  u64 Param64 ,u32 Param2 ,u32 Param3,u32  Param4  )
{
	CContentTreeNode * pCtn;
	switch(nt)
	{
		case CContentTreeNode::ROOT_NODE:
			{
				CContentRootNode * pNode = new CContentRootNode;
				pNode->Title = 	pSLabel;
				pNode->Type = 	nt;		
				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		case CContentTreeNode::TABLE_NODE:
			{
				CContentTableNode * pNode = new CContentTableNode;
				pNode->Title = 	pSLabel;
				pNode->Type = 	nt;	
				pNode->PID = (u16 )Param64;
				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		case CContentTreeNode::SECTION_NODE:
			{
				CContentSectionNode * pNode = new CContentSectionNode;
				pNode->Title = 	pSLabel;
				pNode->Type = 	nt;	
				//pNode->pSection = (CSection *)Param64;
				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		case CContentTreeNode::PACKET_NODE:
			{
				CContentPacketNode * pNode = new CContentPacketNode;
				pNode->Title = 	pSLabel;
				pNode->Type = 	nt;	
				pNode->PID = (u16 )Param64 ;
				//pNode->PacketIdx = Param2;
				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		case CContentTreeNode::FIELD_NODE:
			{
				CContentFieldNode * pNode = new CContentFieldNode;
				pNode->Title = 	pSLabel;
				pNode->Type = 	nt;			
				pNode->Index = 	Param2;
				pNode->Lenth = 	Param3;
				pNode->Format = 	(Format_e)(Param4);
				if((Param3 <= 64)&&(Param4 != FIELD_FMT_STR))
				{
					pNode->Value = Param64;
				}
				else
				{
					u32 DataLen = Param3/8 + (Param3%8?1:0);
					pNode->Value = (u64)malloc(DataLen + 1);
					//DBG("malloc node data lenth %d, value %d. ", DataLen + 1, Param64 );
					memset((void *)pNode->Value, 0 , DataLen + 1);
					if(0 != Param64)
					{/* may be value it late */
						memcpy((void *)pNode->Value, (void *)Param64 , DataLen);
					}
				}		
				/*if(pSHelp)
				{
					pNode->Help = pSHelp;
				}*/

				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		case CContentTreeNode::SUBBODY_NODE:
			{
				CContentSubBodyNode * pNode = new CContentSubBodyNode;
				pNode->Title = 	pSLabel;
				pNode->Type = nt;		
				pNode->Index = Param2;
				pNode->Lenth = Param3;
				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		case CContentTreeNode::RESERVE_NODE:
			{
				CContentReserveNode * pNode = new CContentReserveNode;
				//pNode->Title = 	pSLabel;
				pNode->Type = nt;	
				pNode->Index = Param2;
				pNode->Lenth = Param3;
				pCtn = (CContentTreeNode * )pNode;
			}
			break;
		default:
			pCtn = NULL;
			ASSERT(0);
			break;
	}

	if(parent)
		return (CContentTree *)parent->AddChild(pCtn);
	else
		return new CContentTree(pCtn);
}


void	CFormatParser::InsertReserveBranchIfNeed(CContentTree * parent , u32 index   )
{
	return;
	if(parent)
	{
		CContentTree * pLastBro = (CContentTree *)(parent->GetLastSon());
		if(pLastBro->GetNodeType() == CContentTreeNode::FIELD_NODE)
		{
			CContentFieldNode * pCFN = (CContentFieldNode *)(pLastBro->GetMyNode());
			if(pCFN->Index + pCFN->Lenth < index)
			{/*avoid some thing.*/
				AddReserveBranch(parent, pCFN->Index + pCFN->Lenth, (index - (pCFN->Index + pCFN->Lenth)));
			}
		}
		else if(pLastBro->GetNodeType() == CContentTreeNode::RESERVE_NODE)
		{
			CContentReserveNode * pCRN = (CContentReserveNode *)(pLastBro->GetMyNode());
			if(pCRN->Index + pCRN->Lenth < index)
			{/*avoid some thing.*/
				pCRN->Lenth = (index - pCRN->Index);
			}
		}
		else if(pLastBro->GetNodeType() == CContentTreeNode::SUBBODY_NODE)
		{
			CContentSubBodyNode * pCSN = (CContentSubBodyNode *)(pLastBro->GetMyNode());
			if(pCSN->Index + pCSN->Lenth < index)
			{/*avoid some thing.*/
				AddReserveBranch(parent, pCSN->Index + pCSN->Lenth, (index - (pCSN->Index + pCSN->Lenth)));
			}
		}
	}

}
u64 CFormatParser::ExtractData(u8 * & pDescriptorData,u8 & OffSet, u32 Lenth, u32 Format)
{
	u64 value = 0;
	ASSERT((Lenth < (DATA_BUFF_MAX_SIZE_FOR_EXTARCT*8))&&(OffSet < 8));
	if(( Lenth<=64 )&& (Format != FIELD_FMT_STR))
	{
		while(Lenth > 0)
		{
			value = (value<<1 ) + ((pDescriptorData[0] >>(7 - OffSet)) & 0x1) ;
			OffSet++;
			Lenth--;
			if(OffSet >=  8)
			{
				OffSet -= 8;
				pDescriptorData++;
			}
		}
	}
	else
	{
		u8 * pu8 = Buff;
		u8 DataOffset = 0;
		memset(pu8, 0 , DATA_BUFF_MAX_SIZE_FOR_EXTARCT);
		while(Lenth > 0)
		{
			pu8[0] = (pu8[0] <<1 ) + ((pDescriptorData[0] >>(7 - OffSet)) & 0x1) ;
			OffSet++;
			Lenth--;
			if(OffSet >=  8)
			{
				OffSet -= 8;
				pDescriptorData++;
			}
			DataOffset ++;
			if(DataOffset >= 8)
			{
				DataOffset = 0;
				pu8 ++;
			}
		}
		value = (u64)Buff;
	}
	return value;
}

//return lenth in bit
u32 CFormatParser::ParseWithFormatTree(CFormatTree * pFt, CContentTree * &parent, u8 * & pDescriptorData,u32 & BaseBitIdx, u8 &	OffSet)
{
	CString pSLabel;
	CContentTree * CurRot = parent;
	CContentTree * FmlBase;
	u32	FormatLenth = 0; // lenth in bit
	if(CurRot->GetChildren())
		FmlBase = (CContentTree *)CurRot->GetLastSon();
	else
		FmlBase = CurRot;
	
	CTokenNode * ptn = (CTokenNode *)(pFt->GetMyNode());
	CFormatTree * child = (CFormatTree *)pFt->GetChildren();
	switch(ptn->NodeType)
	{
		case CTokenNode::KEY_WORD_LOOP:
		{
			s32 value ;
			u64 BaseValue = 0;
			CKeyLoopNode * pKLN = (CKeyLoopNode *)ptn;
			
			if(FmlBase->CalculateFormula(pKLN->pFml, BaseValue))
			{
				value = (s32)BaseValue;// + value;//loop bytes
				FormatLenth += (8*value);
				u8 LoopIdx = 0;
				while(value > 0)
				{
					if( pKLN->Name.IsEmpty())
					{
						CurRot = parent;
					}
					else
					{
						pSLabel = pKLN->Name;
						pSLabel.AppendFormat("  %d" , LoopIdx++);
						CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,0/*length need update later*/);
					}
					u32 looplenth = 0;
					CFormatTree * ChildTemp = child;
					while(ChildTemp)
					{
						looplenth += ParseWithFormatTree(ChildTemp,CurRot,
							pDescriptorData,BaseBitIdx,OffSet);
						ChildTemp = (CFormatTree *)ChildTemp->GetRightBrother();
					}
					value -= looplenth/8;
				}
			}
		}
			break;
		case CTokenNode::KEY_WORD_IF:
		{
			CKeyIfNode * pKIN = (CKeyIfNode *)ptn;
			u64 value = 0;

			if(FmlBase->CalculateFormula(pKIN->pFml, value))
			{
				if(value)
				{
					while(child )
					{
						FormatLenth +=  ParseWithFormatTree(child,
							parent,pDescriptorData,BaseBitIdx,OffSet);
						child = (CFormatTree *)child->GetRightBrother();
					}
				}
				else
				{
					pFt = (CFormatTree *)(pFt->GetRightBrother());
					if(pFt && (CTokenNode::KEY_WORD_ELSE == (((CTokenNode *)(pFt->GetMyNode()))->NodeType )))
					{
						child = (CFormatTree *)pFt->GetChildren();

						while(child)
						{
							FormatLenth +=  ParseWithFormatTree(child,
								parent,pDescriptorData,BaseBitIdx,OffSet);
							child = (CFormatTree *)child->GetRightBrother();
						}
					}
				}
			}
		}
			break;
		case CTokenNode::KEY_WORD_ELSE:
		{/* what need done has been done in KEY_WORD_IF node, 
			even FormatLenth has been updated by KEY_WORD_IF node */
		}
			break;
		case CTokenNode::DATA_NODE:
		{
			CDataNode * pDN = (CDataNode *)ptn;
			u64	Value ,Len64/*normally u8 is enough */;
			u32 len32;
			if(FmlBase->CalculateFormula(pDN->pLengthFml, Len64))
			{
				len32 = (u32)Len64;
				ASSERT(len32 == Len64);
				Value= ExtractData(pDescriptorData,OffSet,len32,pDN->format);
				//it will update pDescriptorData and OffSet, so can not be in side if{}

				pSLabel = pDN->NameString;
				AddFieldBranch(parent, pSLabel , BaseBitIdx,len32 ,pDN->format,Value);
				FormatLenth += len32;
				BaseBitIdx += len32;
			}
			else
				ASSERT(0);
		}
			break;
		case CTokenNode::RESERVE_NODE:
		{
				CReserveNode * pRN = (CReserveNode *)ptn;
			u64	Value , Len64 ;
			u32 len32;
			if(FmlBase->CalculateFormula(pRN->pLengthFml, Len64))
			{
				len32 = (u32)Len64;
				ASSERT(len32 == Len64);
				/*value is useless, call this for synchro OffSet and pDescriptorData*/
			 	Value = ExtractData(pDescriptorData,OffSet,len32,FIELD_FMT_HEX/*dosen't matter if it's not string*/);
				AddReserveBranch(parent ,BaseBitIdx,len32);
				FormatLenth += len32;
				BaseBitIdx += len32;
			}
			else
				ASSERT(0);
		}
			break;
		case CTokenNode::KEY_WORD_DESCRIPTOR:
		{
			u8 descriptor_lenth ;// lenth in byte.  so no need to update OffSet. must be same as before
			
			descriptor_lenth = ParseDescriptor( parent,  pDescriptorData, BaseBitIdx);
			pDescriptorData += (descriptor_lenth + 2);
			BaseBitIdx += (descriptor_lenth + 2)* 8;

			FormatLenth += (descriptor_lenth + 2)*8;
		}
			break;
		case CTokenNode::DESCRIPTOR_HEAD_NODE:
		{
			u8 descriptor_lenth = pDescriptorData[1];
			CDescriptorHeadNode * pDhn = (CDescriptorHeadNode *)ptn;
			pSLabel = pDhn->Name;
			CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,8 * (descriptor_lenth+2));
			
			while(child)
			{
				FormatLenth += ParseWithFormatTree(child,CurRot,pDescriptorData,BaseBitIdx,OffSet);
				child = (CFormatTree *)child->GetRightBrother();
			}
		}			
			break;
		case CTokenNode::TABLE_HEAD_NODE:
		{
			ASSERT(0); 
			/*
			error :table format inside other format.
			as we need  set CSection * to node , 
			so we can't build this head here.
			*/
			
		}
			break;
	}
		
	return (FormatLenth);
}

bool CFormatParser::PrivateDescriptorParse(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	u8 descriptor_tag = pDescriptorData[0];
	u8	OffSet = 0;
	CFormatTree * pFt = GetDescriptorFormat(descriptor_tag);
	
	if(pFt)
	{		
		ParseWithFormatTree(pFt, parent, pDescriptorData, BaseBitIdx,OffSet);
		return true;
	}
	return false;
}

CContentTree * CFormatParser::PrivateSectionParse(CSection * pSec)
{
	CString pSLabel;
	CContentTree * CurRot = NULL;
	CommonSection * pCS =(CommonSection * )pSec->GetSectionData();
	CFormatTree * pFt = GetTableFormat(pCS->table_id);
	
	if(pFt)
	{
		u32 BaseBitIdx = 0;
		u8	OffSet = 0;
		CFormatTree * child = (CFormatTree *)pFt->GetChildren();
		u8 * pU8 = (u8 *)pCS;
		
		if (pCS->section_syntax_indicator)
			pSLabel.Format("Version %d; Section[%d / %d].",pSec->GetSectionVersion(),
				pSec->GetSectionNum(),pSec->GetLastSectionNum());
		else
			pSLabel.Format("Data");
		
		CurRot = AddSectionBranch(NULL, pSLabel );
		
		while(child)
		{
			ParseWithFormatTree(child,CurRot,pU8,BaseBitIdx,OffSet);
			child = (CFormatTree *)child->GetRightBrother();
		}

		return CurRot;
	}
	return NULL;
}

bool CFormatParser::GetPrivateTableTitle(u16	TableID , CString & pTitle )
{
	CFormatTree * pFt = GetTableFormat(TableID);
	if(pFt)
	{
		CTableHeadNode * pHN = (CTableHeadNode *)pFt->GetMyNode();
		pTitle = pHN->Name;
		return  true;
	}
	return false;
}

bool CFormatParser::GetPrivateTableCrcCheck(u16	TableID , bool & CrcCk )
{
	CFormatTree * pFt = GetTableFormat(TableID);
	if(pFt)
	{
		CTableHeadNode * pHN = (CTableHeadNode *)pFt->GetMyNode();
		CrcCk = pHN->CrcCheck;
		return  true;
	}
	return false;
}

u8 	CFormatParser::ParseDescriptor(CContentTree * parent, u8 * pDescriptorData ,u32 BaseBitIdx)
{			
	u8 descriptor_lenth = pDescriptorData[1];
	//PrivateDescriptorParse( parent, pDescriptorData, BaseBitIdx);
	ASSERT(0);
	return descriptor_lenth;
}
