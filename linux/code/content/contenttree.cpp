
#include "contenttree.h"
#include "stdlib.h"
#include "string.h"


bool CContentTreeNode::AppendMJDTime(u64 MJD , CString &str)
{
	u32 Y, M, D, temp1, temp2;
	Y = (u32)((MJD - 15078.2) / 365.25) ;
	temp1 = (u32)(Y * 365.25);
	M = (u32)((MJD - 14956.1 - temp1) / 30.6001);
	temp2 = (u32)(M * 30.6001);
	D = (u32)(MJD - 14945 - temp1 - temp2 - 11); /* -11 for actual adjust */
	if((M == 14) || (M == 15))
	{
		Y ++;
		M -= 13;
	}
	else
	{
		M--;
	}
	str.AppendFormat(" : mjd : %d / %d / %d ", Y , M , D);
	return true;
}

CString 	CContentTreeNode::GetTitleWithFmtVal()
{
	CString FmtTitle;
	switch(Type)
	{
		case FIELD_NODE:
		{
			CContentFieldNode *pFN = (CContentFieldNode *)this;
			FmtTitle = Title;
			switch(pFN->Format)
			{
				case FIELD_FMT_BIN:
					FmtTitle.AppendFormat(" : 0b ");

					for(int i = pFN->Lenth - 1  ; i >= 0; i --)
						FmtTitle.AppendFormat("%d", (pFN->Value >> (i)) & 0x01) ;

					break;
				case FIELD_FMT_DEC:
					FmtTitle.AppendFormat(" : 0d %d", pFN->Value);
					break;
				case FIELD_FMT_HEX:
					FmtTitle.AppendFormat(" : 0x %x", pFN->Value);
					break;

				case FIELD_FMT_BCD:
				{
					//assume that: lenth can't be longer then 32 and must be *4
					FmtTitle.AppendFormat(" : bcd ");

					for(int i = pFN->Lenth - 4 ; i >= 0; i -= 4)
						FmtTitle.AppendFormat("%d", (pFN->Value >> (i)) & 0x0F) ;
				}
				break;
				case FIELD_FMT_MJD:
					AppendMJDTime(pFN->Value, FmtTitle);
					break;
				case FIELD_FMT_STR:
				{
					u8 *pC = (u8 *) ((u32)(pFN->Value));
					FmtTitle.AppendFormat(" : 0s ");
					for(u32 i = 0 ; i < pFN->Lenth / 8; i ++)
						FmtTitle.AppendFormat("%c", *(pC + i));
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

CContentTreeNode *CContentTreeNode::Duplicate()
{
	CContentTreeNode *pCtn = NULL;
	ASSERT(0);
	return pCtn;
}

CContentTreeNode *CContentRootNode::Duplicate()
{
	CContentRootNode *pCtn  = new CContentRootNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	return (CContentTreeNode *)pCtn;
}

CContentTreeNode *CContentReserveNode::Duplicate()
{
	CContentReserveNode *pCtn = new CContentReserveNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->Index = Index;
	pCtn->Lenth = Lenth;
	return (CContentTreeNode *)pCtn;
}

CContentTreeNode *CContentFieldNode::Duplicate()
{
	CContentFieldNode *pCtn = new CContentFieldNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->Index = Index;
	pCtn->Lenth = Lenth;
	pCtn->Value = Value;
	pCtn->Format = Format;

	return (CContentTreeNode *)pCtn;
}

bool  CContentFieldNode::GetFormatString(CString &str)
{
	switch(Format)
	{
		case FIELD_FMT_BIN:
			str = "bin";
			break;
		case FIELD_FMT_DEC:
			str = "dec";
			break;
		case FIELD_FMT_HEX:
			str = "hex";
			break;
		case FIELD_FMT_BCD:
			str = "bcd";
			break;
		case FIELD_FMT_MJD:
			str = "mjd";
			break;
		case FIELD_FMT_STR:
			str = "str";
			break;
		default:
			str = "unknow";
			break;
	}
	return true;
}

bool  CContentFieldNode::GetValueString(CString &str)
{
	str.Empty();
	switch(Format)
	{
		case FIELD_FMT_BIN:
			if(Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
			{
				u8 *pData = (u8 *) ((u32)Value);

				for(s8 i = Lenth - 1 ; i >= 0 ; i--)
				{
					str.AppendFormat("%d", (pData[ i] >> (i % 8)) & 0x1);
				}
			}
			else
			{
				for(s8 i = Lenth - 1 ; i >= 0 ; i--)
				{
					str.AppendFormat("%d", (Value >> i) & 0x1);
				}
			}
			break;
		case FIELD_FMT_DEC:
			if(Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
			{
				/*u8 * pData = (u8 *) Value;
				for(s8 i = (Lenth-1)/8 ; i >= 0 ; i--)
				{
					str.AppendFormat("%d", pData[  i]);
				}*/
				printf("u should not DEC to present bigdata more than 64 bit.");
				ASSERT(0);
			}
			else
			{
				str.Format(" %d ", Value);
			}
			break;
		case FIELD_FMT_MJD:
		case FIELD_FMT_BCD:
		case FIELD_FMT_HEX:
			if(Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
			{
				u8 *pData = (u8 *) ((u32)Value);
				u32 max = (Lenth - 1) / 8;
				u32 i = 0;

				while((i < max) && (0 == pData[ i]))
				{
					DBG("ignor a byte for it's 0 in head.");
					i++;
				}
				/* first should not begin with 0 */
				str.AppendFormat("%x", pData[ i]);
				i++;
				/* others should not ignore 0 */
				for(; (i <= max) ; i++)
				{
					str.AppendFormat("%02x", pData[ i]);
					//DBG("%d",i);
				}
			}
			else
			{
				str.Format(" %x ", Value);
			}
			break;
		case FIELD_FMT_STR:
		{
			s8 *pstr = (s8 *)((u32)Value);
			str.Format(" %s ", pstr);
		}
			break;
		default:
			str = " ~@!@~ ";
			break;
	}
	return true;
}

CContentFieldNode::~CContentFieldNode(void)
{
	if((Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8)) || (Format == FIELD_FMT_STR))
	{
		if(0 != Value)
		{
			delete((u8 *)((u32)Value));
		}
	}
}

CContentTreeNode *CContentTableNode::Duplicate()
{
	CContentTableNode *pCtn = new CContentTableNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->PID = PID;
	return (CContentTreeNode *)pCtn;
}
CContentTreeNode *CContentSectionNode::Duplicate()
{
	CContentSectionNode *pCtn = new CContentSectionNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	//pCtn->pSection = pSection;
	//pCtn->DataLenth = DataLenth;

	return (CContentTreeNode *)pCtn;
}
CContentTreeNode *CContentSubBodyNode::Duplicate()
{
	CContentSubBodyNode *pCtn = new CContentSubBodyNode;
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

CContentTreeNode *CContentPacketNode::Duplicate()
{
	CContentPacketNode *pCtn = new CContentPacketNode;
	pCtn->Title = Title;
	pCtn->Type = Type;
	pCtn->PID = PID;

	return (CContentTreeNode *)pCtn;
}

CContentTree::CContentTree(CContentTreeNode *Node_a, CContentTree *fa, CContentTree *Bro, CContentTree *Cld)
	: CTree(Node_a , fa ,  Bro ,  Cld)
{

}


CContentTree::~CContentTree(void)
{
}


CContentTree *CContentTree::Duplicate()
{
	CContentTreeNode *pNN = NULL, * pON;
	pON = (CContentTreeNode *)(GetMyNode());
	if(pON)
		pNN =  pON->Duplicate();

	CContentTree *pNewTree = new CContentTree(pNN);

	/*  */
	CContentTree *pCCT = (CContentTree *)GetChildren();
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


bool	CContentTree::CalculateFormula(CFormula *pFormula , u64 &Ret)
{

	CFormulaNode *ptn = (CFormulaNode *)pFormula->GetMyNode();
	switch(ptn->type)
	{
		case CFormulaNode::OPERATOR:
		{
			u64 Value1 = 0, Value2 = 0;
			CFormula *pChildFml1 = (CFormula *)pFormula->GetChildren();
			CFormula *pChildFml2 = (CFormula *)(pChildFml1->GetRightBrother());
			CFormulaOperatorNode *pFON  = (CFormulaOperatorNode *)ptn;

			if(false == CalculateFormula(pChildFml1, Value1))
				return false;

			if(false == CalculateFormula(pChildFml2, Value2))
				return false;

			/*  formula is a two son tree. */
			switch(pFON->op)
			{
				case CFormulaOperatorNode::PLUS:
					Ret = Value1 + Value2;
					break;
				case CFormulaOperatorNode::MINUS:
					ASSERT(Value1 >= Value2);
					Ret = Value1 - Value2;
					break;
				case CFormulaOperatorNode::MULTIPLY:
					Ret = Value1 * Value2;
					break;
				case CFormulaOperatorNode::DIVIDE:
					Ret = Value1 / Value2;
					break;
				case CFormulaOperatorNode::EQUAL:
					Ret = (Value1 == Value2);
					break;
				default:
					ASSERT(0);
					break;
			}
		}
		break;
		case CFormulaNode::PARAMETER:
		{
			CFormulaParameterNode *pFPN = (CFormulaParameterNode *)ptn;
			if(false == (SearchBackFieldValue(&(pFPN->param) , Ret)))
				return false;
		}
		break;
		case CFormulaNode::NUMERAL:
		{
			CFormulaNumeralNode *pFNN = (CFormulaNumeralNode *)ptn;
			Ret = pFNN->num;
		}
		break;
		default:
			return false;
	}
	return true;
}

/*
void * CContentTree::GetOrigin()
{
	if(GetNodeType() == CContentTreeNode::TABLE_NODE)
		return (void *)(((CContentTableNode *)GetMyNode())->pTable);
	if(GetNodeType() == CContentTreeNode::PACKET_NODE)
		return (void *)(((CContentPacketNode *)GetMyNode())->pPkt);

	CContentTree * pct = (CContentTree *)GetFather() ;
	if(pct)
		return pct->GetOrigin();

	ASSERT(0);
	return NULL;
}
*/
/*
function : search back from this to root
return for same string found
*/
bool CContentTree::SearchBackFieldValue(CString *pSLabel , u64 &Ret)
{
	CContentTree *NextSearchTar = this;

	while(NextSearchTar)
	{
		CContentTreeNode *pNode = (CContentTreeNode *)NextSearchTar->GetMyNode();

		if((pNode->Type == CContentTreeNode::FIELD_NODE) && (pNode->Title.Find(*pSLabel)))
		{
			CContentFieldNode *pTnf = (CContentFieldNode *)pNode;
			Ret = pTnf->Value;
			return true;
		}
		NextSearchTar = (CContentTree *)NextSearchTar->GetLeftBrother();
	}
	NextSearchTar = (CContentTree *)GetFather();

	if((NextSearchTar) && NextSearchTar->SearchBackFieldValue(pSLabel  , Ret))
		return true;

	ASSERT(0);
	return false;
}


CContentTree   *CContentTree::SearchBranchByNode(CContentTreeNode *Node_a)
{
	/*  */
	CContentTree *pCCT ;

	if((CContentTreeNode *)(GetMyNode()) == Node_a)
	{
		return this;
	}

	if((pCCT = (CContentTree *)GetChildren()) && (pCCT = pCCT->SearchBranchByNode(Node_a)))
	{
		return pCCT;
	}

	if((pCCT = (CContentTree *)GetRightBrother()) && (pCCT = pCCT->SearchBranchByNode(Node_a)))
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

bool CContentTree::DumpToShortString(CString &TraceStr, u8 NestDeep)
{
	for(u8 Ti = 0 ; Ti < (NestDeep); Ti ++)
		TraceStr.AppendFormat("\t");

	CContentTreeNode *ptn = (CContentTreeNode *)GetMyNode();
	switch(ptn->Type)
	{
		case CContentTreeNode::ROOT_NODE:
			TraceStr.AppendFormat("\r\n");
		break;
		case CContentTreeNode::TABLE_NODE:
		{
			CContentTableNode *pdn = (CContentTableNode *)ptn  ;
			TraceStr.AppendFormat("<TABLE ( %s ) :pid( %d )> \r\n" , pdn->Title.c_str() , pdn->PID);
		}
		break;
		case CContentTreeNode::SECTION_NODE:
		{
			CContentSectionNode *pSn = (CContentSectionNode *)ptn  ;
			TraceStr.AppendFormat("<SECTION	: %s > \r\n" , pSn->Title.c_str());
		}
		break;
		case CContentTreeNode::FIELD_NODE:
		{
			CContentFieldNode *pFn = (CContentFieldNode *)ptn  ;
			CString str;
			/*if(FIELD_FMT_STR  == pFn->Format)
			{
				u8 *pData = (u8 *)((u32)(pFn->Value));
				TraceStr.AppendFormat("<%s : %s> \r\n" , pFn->Title.c_str() ,   pData);
			}
			else if(pFn->Lenth <= 64)
			{
				TraceStr.AppendFormat("<%s :  %x> \r\n" , pFn->Title.c_str() ,  pFn->Value);
				//may be add zero append better .
			}
			else
			{
				u8 *pData = (u8 *)((u32)(pFn->Value));
				u32 DataLen = (pFn->Lenth) / 8 + ((pFn->Lenth) % 8 ? 1 : 0);
				TraceStr.AppendFormat("<%s : " , pFn->Title.c_str() );
				for(u16 idx = 0 ; idx < DataLen; idx++)
					TraceStr.AppendFormat("%02x", pData[idx]);
*/
				TraceStr.AppendFormat("<%s : " , pFn->Title.c_str() );
				pFn->GetValueString(str);
				TraceStr.Append(str);
			
				TraceStr.AppendFormat("> \r\n");
			//}
		}
		break;
		case CContentTreeNode::PACKET_NODE:
		{
			CContentPacketNode *pPn = (CContentPacketNode *)ptn  ;
			TraceStr.AppendFormat("<PACKET_NODE: %s  > \r\n" , pPn->Title.c_str());
		}
		break;
		case CContentTreeNode::SUBBODY_NODE:
			TraceStr.AppendFormat("\r\n");
		break;
		case CContentTreeNode::RESERVE_NODE:
		{
			CContentReserveNode *pRn = (CContentReserveNode *)ptn  ;
			TraceStr.AppendFormat("< : > \r\n" );
		}
		break;
		default:
			TraceStr.AppendFormat("\r\n<Unkow node> \r\n");
			break;
	}
	/*	dump children node	*/
	if(GetChildren())
	{
		CContentTree *child = (CContentTree *)GetChildren();

		for(u8 Ti = 0 ; Ti < NestDeep; Ti ++)
			TraceStr.AppendFormat("\t");
		TraceStr.AppendFormat("{ \r\n");

		while(child)
		{
			child->DumpToShortString(TraceStr, NestDeep + 1);
			child = (CContentTree *)(child->GetRightBrother());
		}

		for(u8 Ti = 0 ; Ti < NestDeep; Ti ++)
			TraceStr.AppendFormat("\t");
		TraceStr.AppendFormat("} \r\n");
	}

	return true;
}

bool CContentTree::DumpToString(CString &TraceStr, u8 NestDeep)
{

	/*	dump current node	*/
	//CFormatTree * SubEnd = NULL;

	for(u8 Ti = 0 ; Ti < (NestDeep); Ti ++)
		TraceStr.AppendFormat("\t");

	CContentTreeNode *ptn = (CContentTreeNode *)GetMyNode();
	switch(ptn->Type)
	{
		case CContentTreeNode::ROOT_NODE:
		{
			CContentRootNode *pdn = (CContentRootNode *)ptn  ;
			TraceStr.AppendFormat("\r\n<!-- Copyright %s --> \r\n", pdn->Title.c_str());
		}
		break;
		case CContentTreeNode::TABLE_NODE:
		{
			CContentTableNode *pdn = (CContentTableNode *)ptn  ;
			TraceStr.AppendFormat("<TABLE ( %s ) :pid( %d )> \r\n" , pdn->Title.c_str() , pdn->PID);
		}
		break;
		case CContentTreeNode::SECTION_NODE:
		{
			CContentSectionNode *pSn = (CContentSectionNode *)ptn  ;
			//CSection * 		pSec = pSn->pSection ;
			TraceStr.AppendFormat("<SECTION	: %s > \r\n" , pSn->Title.c_str());
		}
		break;
		case CContentTreeNode::FIELD_NODE:
		{
			CContentFieldNode *pFn = (CContentFieldNode *)ptn  ;
			if(FIELD_FMT_STR  == pFn->Format)
			{
				u8 *pData = (u8 *)((u32)(pFn->Value));
				TraceStr.AppendFormat("<FIELD_NODE:%s,idx:%d,len:%d,value:%s> \r\n" , pFn->Title.c_str() , pFn->Index, pFn->Lenth, pData);
			}
			else if(pFn->Lenth <= 64)
			{
				TraceStr.AppendFormat("<FIELD_NODE:%s,idx:%d,len:%d,value:%x> \r\n" , pFn->Title.c_str() , pFn->Index, pFn->Lenth, pFn->Value);
			}
			else
			{
				u8 *pData = (u8 *)((u32)(pFn->Value));
				u32 DataLen = (pFn->Lenth) / 8 + ((pFn->Lenth) % 8 ? 1 : 0);
				TraceStr.AppendFormat("<FIELD_NODE:%s,idx:%d,len:%d,value:" , pFn->Title.c_str() , pFn->Index, pFn->Lenth);
				for(u16 idx = 0 ; idx < DataLen; idx++)
					TraceStr.AppendFormat("%02x", pData[idx]);
				TraceStr.AppendFormat("> \r\n");
			}
		}
		break;
		case CContentTreeNode::PACKET_NODE:
		{
			CContentPacketNode *pPn = (CContentPacketNode *)ptn  ;
			TraceStr.AppendFormat("<PACKET_NODE: %s  > \r\n" , pPn->Title.c_str());
		}
		break;
		case CContentTreeNode::SUBBODY_NODE:
		{
			CContentSubBodyNode *pSBn = (CContentSubBodyNode *)ptn  ;

			TraceStr.AppendFormat("<SUBBODY_NODE: %s : %d :%d> \r\n" , pSBn->Title.c_str() , pSBn->Index, pSBn->Lenth);
		}
		break;
		case CContentTreeNode::RESERVE_NODE:
		{
			CContentReserveNode *pRn = (CContentReserveNode *)ptn  ;

			TraceStr.AppendFormat("<RESERVE_NODE: %d :%d> \r\n" , pRn->Index, pRn->Lenth);
		}
		break;
		default:
			TraceStr.AppendFormat("\r\n<Unkow node> \r\n");
			break;
	}
	/*	dump children node	*/
	if(GetChildren())
	{
		CContentTree *child = (CContentTree *)GetChildren();

		for(u8 Ti = 0 ; Ti < NestDeep; Ti ++)
			TraceStr.AppendFormat("\t");
		TraceStr.AppendFormat("{ \r\n");

		while(child)
		{
			child->DumpToString(TraceStr, NestDeep + 1);
			child = (CContentTree *)(child->GetRightBrother());
		}

		for(u8 Ti = 0 ; Ti < NestDeep; Ti ++)
			TraceStr.AppendFormat("\t");
		TraceStr.AppendFormat("} \r\n");
	}

	return true;
}

bool CContentTree::SaveToTxtFile(CStreamFile *file)
{
		CString ContStr;

		DumpToShortString(ContStr, 0);
		file->WriteString(ContStr);
		
		return true;
}

bool CContentTree::SaveToXmlFile(CStreamFile *file, u8 NestDeep)
{

	CString str;
	/*	save <node> first */
	for(u8 Ti = 0 ; Ti < (NestDeep); Ti ++)
		str.AppendFormat("\t");

	CContentTreeNode *ptn = (CContentTreeNode *)GetMyNode();
	switch(ptn->Type)
	{
		case CContentTreeNode::ROOT_NODE:
		{
			CContentRootNode *pdn = (CContentRootNode *)ptn  ;
			str.AppendFormat("<!-- Copyright %s --> \r\n", pdn->Title.c_str());
		}
		break;
		case CContentTreeNode::TABLE_NODE:
		{
			CContentTableNode *pdn = (CContentTableNode *)ptn  ;
			str.AppendFormat("<TABLE name='%s' pid='%d' > \r\n" , pdn->Title.c_str() , pdn->PID);
		}
		break;
		case CContentTreeNode::SECTION_NODE:
		{
			CContentSectionNode *pSn = (CContentSectionNode *)ptn  ;
			str.AppendFormat("<SECTION name='%s' > \r\n" , pSn->Title.c_str());
		}
		break;
		case CContentTreeNode::FIELD_NODE:
		{
			CContentFieldNode *pFn = (CContentFieldNode *)ptn  ;
			CString FmtStr;
			pFn->GetFormatString(FmtStr);
			str.AppendFormat("<FIELD name='%s' lenth='%d' format='%s'> 	" , pFn->Title.c_str() , pFn->Lenth, FmtStr.c_str());
		}
		break;
		case CContentTreeNode::PACKET_NODE:
		{
			CContentPacketNode *pPn = (CContentPacketNode *)ptn  ;
			str.AppendFormat("\r\n<PACKET name='%s' > \r\n" , pPn->Title.c_str());
		}
		break;
		case CContentTreeNode::SUBBODY_NODE:
		{
			CContentSubBodyNode *pSBn = (CContentSubBodyNode *)ptn  ;

			str.AppendFormat("<SUBBODY name='%s' lenth='%d'> \r\n" , pSBn->Title.c_str() , pSBn->Lenth);
		}
		break;
		case CContentTreeNode::RESERVE_NODE:
		{
			CContentReserveNode *pRn = (CContentReserveNode *)ptn  ;

			str.AppendFormat("<RESERVE lenth='%d' /> \r\n" , pRn->Lenth);
		}
		break;
		default:
			str.AppendFormat("<UNKNOW NODE>");
			break;
	}
	file->WriteString(str);
	/*	save value second */
	switch(ptn->Type)
	{
		case CContentTreeNode::FIELD_NODE:
		{
			CContentFieldNode *pFn = (CContentFieldNode *)ptn  ;
			pFn->GetValueString(str);
			file->WriteString(str);
		}
		break;
		default:
		{
			CContentTree *child = (CContentTree *)GetChildren();
			while(child)
			{
				if(GetNodeType() == CContentTreeNode::ROOT_NODE)
					child->SaveToXmlFile(file, NestDeep);
				else
					child->SaveToXmlFile(file, NestDeep + 1);

				child = (CContentTree *)(child->GetRightBrother());
			}
		}
		break;
	}
	/*	save </node> third */
	str.Empty();
	if(CContentTreeNode::RESERVE_NODE != ptn->Type )
	{
		for(u8 Ti = 0 ; Ti < (NestDeep); Ti ++)
			str.AppendFormat("\t");
		switch(ptn->Type)
		{
			case CContentTreeNode::ROOT_NODE:
				break;
			case CContentTreeNode::TABLE_NODE:
				str.AppendFormat("</TABLE> \r\n");
				break;
			case CContentTreeNode::SECTION_NODE:
				str.AppendFormat("</SECTION> \r\n");
				break;
			case CContentTreeNode::FIELD_NODE:
				str.AppendFormat("</FIELD> \r\n");
				break;
			case CContentTreeNode::PACKET_NODE:
				str.AppendFormat("</PACKET> \r\n");
				break;
			case CContentTreeNode::SUBBODY_NODE:
				str.AppendFormat("</SUBBODY> \r\n");
				break;
			case CContentTreeNode::RESERVE_NODE:
				//str.AppendFormat("</RESERVE> \r\n");
				break;
			default:
				str.AppendFormat("</UNKNOW NODE>");
				break;
		}
		file->WriteString(str);
	}

	return true;
}

/*

bool  CContentTree::TransferToTable(CTable &ptb)
{
	if(GetNodeType() == CContentTreeNode::ROOT_NODE)
	{
		if(GetChildren())
		{
			CContentTree *child = (CContentTree *)GetChildren();

			while(child)
			{
				if(true == child->TransferToTable(ptb))
					return true;

				child = (CContentTree *)(child->GetRightBrother());
			}
		}
	}
	if(GetNodeType() == CContentTreeNode::TABLE_NODE)
	{
		CSection *pSec;
		CContentTableNode *pCTN = (CContentTableNode *)GetMyNode();
		ptb.SetPID(pCTN->PID);
		if(GetChildren())
		{
			CContentTree *child = (CContentTree *)GetChildren();
			u32 index;
			while((child) && (child->GetNodeType() == CContentTreeNode::SECTION_NODE))
			{
				pSec = new CSection;
				index = 0;
				pSec->SectionData = (u8 *)malloc(4096);   // malloc MAX_SECTION_LENGTH
				memset(pSec->SectionData, 0 , 4096);
				child->FillData(pSec->SectionData , index);
				ptb.AttachSection(pSec);
				child = (CContentTree *)(child->GetRightBrother());
			}
			if(ptb.IsIntegrity())
			{
				ptb.UpDateSectionCrc();
				ptb.UpDateVersion();
				ptb.UpDateIdsFromSection();
				ptb.SortSection();
				return true;
			}
		}

	}
	return false;
}
*/
bool  CContentTree::FillData(u8 *pData, u32 &idx)
{
	if(GetNodeType() == CContentTreeNode::FIELD_NODE)
	{
		CContentFieldNode *pCFN = (CContentFieldNode *) GetMyNode();

		for(s32 i = (pCFN->Lenth - 1) ; i >= 0 ; i--)
		{
			u8 LastBit = 0;
			if((pCFN->Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
					|| (pCFN->Format == FIELD_FMT_STR))
			{
				u8 *pD = (u8 *)((u32)(pCFN->Value));
				LastBit = (pD[((pCFN->Lenth - 1) - i) / 8] >> (i%8)) & 0x1;
			}
			else
			{
				LastBit = ((pCFN->Value) >> i) &  0x1 ;
			}
			pData[ idx / 8 ] |= LastBit << (7 - (idx % 8));
			idx ++;
		}
		pCFN = NULL;
	}
	else if(GetNodeType() == CContentTreeNode::RESERVE_NODE)
	{
		CContentReserveNode *pCRn = (CContentReserveNode *)GetMyNode()  ;
		idx += pCRn->Lenth;
	}

	if(GetChildren())
	{
		CContentTree *child = (CContentTree *)GetChildren();

		while(child)
		{
			child->FillData(pData, idx);
			child = (CContentTree *)(child->GetRightBrother());
		}
	}

	return true;
}

