#define _NOINFOM

#include "formatfile.h"
#include "dvbspace.h"
#include <string.h>
#include <stdlib.h>

using namespace NDVBSpace;
extern "C"
{
extern int stricmp(const char *dst, const char *src);
}


CFormatFile::CFormatFile(const s8 *pFileName)
	: CXmlFile(pFileName)
{
	pFTree = NULL;
	pCurTree = NULL;
	lenth = NULL ;
	condition = NULL;
	IncludeFlag = false;
}

CFormatFile::~CFormatFile(void)
{
	ClearFmtTree();
}


void CFormatFile::ClearFmtTree()
{	
	if(pFTree)
	{
		delete pFTree;
		pFTree = NULL;
		pCurTree = NULL;
	}
}

void CFormatFile::StoreFmtTree()
{
	if(pFTree)
	{
		AddFormat(pFTree);
		pFTree = NULL;
		pCurTree = NULL;
	}
}

void CFormatFile::ClearAttr()
{
	name = "" ;
	 format = FIELD_FMT_HEX ;
	CrcCheck = true;
	 id = INVALID_TABLE_ID;
	if(lenth)
	{
		delete lenth;
		lenth = NULL;
	}
	if(condition)
	{
		delete condition;
		condition = NULL;
	}
	
}

void CFormatFile::ReadAttr(const s8 **attr)
{
	ClearAttr();

	for(u8 idx = 0; attr[idx] ; idx += 2)
	{
		if(0 == stricmp(attr[idx], STRING_ID))
		{
			id = atoi(attr[idx + 1]);
		}
		else if(0 == stricmp(attr[idx], STRING_NAME))
		{
			name = attr[idx + 1];
		}
		else if(0 == stricmp(attr[idx], STRING_LENTH))
		{
			CString formulastr;
			formulastr = attr[idx + 1];
			lenth =  ParseFormula(formulastr);
			if(NULL == pCurTree)
			{
				CString tmp;
				lenth->DumpToString(tmp);
				ERR("formula don't have base in file %s. \n %s", GetFileName().c_str(), tmp.c_str());
			}
			else
			{
				CFormatTree *schtree;
				if(pCurTree->GetLastSon())
					schtree = (CFormatTree *)(pCurTree->GetLastSon());
				else
					schtree = pCurTree;

				if(false == schtree->CheckFormulaValid(lenth))
				{
					CString tmp;
					lenth->DumpToString(tmp);
					ERR("formula check error in file %s. \n %s", GetFileName().c_str(), tmp.c_str());
				}
			}
		}
		else if(0 == stricmp(attr[idx], STRING_CONDITION))
		{
			CString formulastr;
			formulastr = attr[idx + 1];
			condition = ParseFormula(formulastr);
			if(NULL == pCurTree)
			{
				CString tmp;
				condition->DumpToString(tmp);
				ERR("formula don't have base in file %s. \n %s", GetFileName().c_str(), tmp.c_str());
			}
			else
			{
				CFormatTree *schtree;
				if(pCurTree->GetLastSon())
					schtree = (CFormatTree *)(pCurTree->GetLastSon());
				else
					schtree = pCurTree;

				if(false == schtree->CheckFormulaValid(condition))
				{
					CString tmp;
					condition->DumpToString(tmp);
					ERR("formula check error in file %s. \n %s", GetFileName().c_str(), tmp.c_str());
				}
			}

		}
		else if(0 == stricmp(attr[idx], STRING_FORMAT))
		{
			if(0 == stricmp(attr[idx + 1], STRING_HEX))
				format = FIELD_FMT_HEX;
			else if(0 == stricmp(attr[idx + 1], STRING_STR))
				format = FIELD_FMT_STR;
			else
			{
				ERR("unkow format %s \n", attr[idx + 1]);
			}
		}
		else if(0 == stricmp(attr[idx], STRING_CRC))
		{
			if(0 == stricmp(attr[idx + 1], STRING_TRUE))
				CrcCheck = true;
			else if(0 == stricmp(attr[idx + 1], STRING_FALSE))
				CrcCheck = false;
			else
			{
				ERR("unkow bool for crc. %s \n", attr[idx + 1]);
			}
		}
		else
		{
			DBG("not used attr %s, \n", attr[idx]);
		}
	}
}

void CFormatFile::SetValue(const s8 *el,const s8 *vl)
{
	if((IncludeFlag) && (0 == stricmp(el, STRING_FILE)))
	{
		CFormatFile *pFF = new CFormatFile(vl);
		pFF->Load();
		delete pFF;

		INF("	 format file :<<%s>> included .\n", vl);
	}
	else
	{
		ERR("unused format tree node data <%s>.",vl);
	}
}

void CFormatFile::UserEndValue(const s8 *el)
{
	if(0 < CurValueLenth)
	{
		CurValuebuf[CurValueLenth] = 0;
		INF("data is:[%s] len = %d .\n", CurValuebuf, CurValueLenth);
		SetValue(el, CurValuebuf);
	}
	
	if(0 == stricmp(el, STRING_INCLUDE))
	{
		IncludeFlag = false;
	}
	else if(0 == stricmp(el, STRING_FILE))
	{

	}
	else
	{
		ASSERT(pCurTree);
		pCurTree = (CFormatTree *)(pCurTree->GetFather());
		if(0 == Depth)
		{
			StoreFmtTree();
		}
	}
}

void CFormatFile::UserStartValue(const s8 *el, const s8 **attr)
{
	//INF("FmtTreeNodeStart(%s) on deep %d, \n", el, Depth);
	ReadAttr(attr);
	if(0 == stricmp(el, STRING_INCLUDE))
	{
		IncludeFlag = true;
	}
	else if(0 == stricmp(el, STRING_FILE))
	{

	}
	else if(0 == stricmp(el, STRING_TABLE))
	{
		if(1 == Depth)
		{
			CTableHeadNode *ptn;
			ptn = new CTableHeadNode;

			ptn->Name = name;
			ptn->ID = id;
			ptn->CrcCheck = CrcCheck;
			pFTree = new CFormatTree(ptn);
			pCurTree = pFTree;
		}
		else
			ERR("deep error for <TABLE name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_DESCRIPTOR))
	{
		if(1 == Depth)
		{
			/*head*/
			CDescriptorHeadNode *ptn;
			ptn = new CDescriptorHeadNode;

			ptn->Name = name;
			ptn->ID = id;
			pFTree = new CFormatTree(ptn);
			pCurTree = pFTree;
		}
		else/* content of table */
		{
			CKeyDescriptorNode *ptn;
			CFormatTree   *pFT;
			ptn = new CKeyDescriptorNode;
			pFT = new CFormatTree(ptn);
			pCurTree = (CFormatTree *)(pCurTree->AddChild(pFT));
		}
	}
	else if(0 == stricmp(el, STRING_FIELD))
	{
		if(1 < Depth)
		{
			CDataNode *ptn;
			CFormatTree   *pFT;
			ptn = new CDataNode;
			ptn->NameString = name;
			ptn->format = format;
			ptn->pLengthFml = lenth;
			lenth = NULL;
			pFT = new CFormatTree(ptn);
			pCurTree = (CFormatTree *)(pCurTree->AddChild(pFT));
		}
		else
			ERR("deep error for <FIELD '%s'> node in file %s", name.c_str(), GetFileName().c_str());
	}
	else if(0 == stricmp(el, STRING_RESERVE))
	{
		if(1 < Depth)
		{
			CReserveNode *ptn;
			CFormatTree   *pFT;
			ptn = new CReserveNode;
			ptn->pLengthFml = lenth;
			lenth = NULL;
			pFT = new CFormatTree(ptn);
			pCurTree = (CFormatTree *)(pCurTree->AddChild(pFT));
		}
		else
			ERR("deep error for <RESERVE> node in file %s", GetFileName().c_str());
	}
	else if(0 == stricmp(el, STRING_LOOP))
	{
		if(1 < Depth)
		{
			CKeyLoopNode *ptn;
			CFormatTree   *pFT;
			ptn = new CKeyLoopNode;
			ptn->pFml = lenth;
			lenth = NULL;
			ptn->Name = name;

			pFT = new CFormatTree(ptn);
			pCurTree = (CFormatTree *)(pCurTree->AddChild(pFT));
		}
		else
			ERR("deep error for <LOOP> node in file %s", GetFileName().c_str());
	}
	else if(0 == stricmp(el, STRING_IF))
	{
		if(1 < Depth)
		{
			CKeyIfNode *ptn;
			CFormatTree   *pFT;
			ptn = new CKeyIfNode;
			ptn->pFml = condition;
			condition = NULL;
			pFT = new CFormatTree(ptn);
			pCurTree = (CFormatTree *)(pCurTree->AddChild(pFT));
		}
		else
			ERR("deep error for <IF> node in file %s", GetFileName().c_str());
	}
	else if(0 == stricmp(el, STRING_ELSE))
	{
		if(1 < Depth)
		{
			/*need judge if last son is IF NODE tree*/
			if(CTokenNode::KEY_WORD_IF == ((CTokenNode *)(pCurTree->GetLastSon()->GetMyNode()))->NodeType)
			{
				CKeyElseNode *ptn;
				CFormatTree   *pFT;
				ptn = new CKeyElseNode;
				pFT = new CFormatTree(ptn);
				pCurTree = (CFormatTree *)(pCurTree->AddChild(pFT));
			}
			else
				ERR("<ELSE> not follow <IF> in file %s", GetFileName().c_str());
		}
		else
			ERR("deep error for <ELSE> node in file %s", GetFileName().c_str());
	}
	else
	{
		ERR("unrecognized node <%s>  in file %s \n", el, GetFileName().c_str());
	}


}
/*

*/


/*
funtion:	find first sonFmlStr inside () of FatherFmlStr
return:	sonFmlStr end postion;
*/

bool CFormatFile::CheckAndTrimFmlStr(CString &FmlStr)
{
	s32 LeftLastPos = 0;
	s32 RightLastPos = 0;
	u8 LeftCnt = 0;
	u8 RightCnt = 0;
	const s8 *pd = FmlStr.c_str();
	for(u16 i = 0; i < FmlStr.GetLength() ; i ++)
	{
		if(pd[i] == '(')
		{
			LeftLastPos++;
			LeftCnt++;
		}
		else if(pd[i] == ')')
		{
			RightLastPos++;
			RightCnt++;
		}
	}
	if(RightCnt != LeftCnt)
		return false;/*op ( r not same as op )*/

	if(-1 != (LeftLastPos = FmlStr.find_first_of("+-*/=")))
	{
		if((0 == LeftLastPos) || (FmlStr.GetLength() == (LeftLastPos + 1)))
			return false;/*op on begin or last*/
	}
	FmlStr.Trim();
	while((FmlStr.Left(1) == "(") && (FmlStr.Right(1) == ")"))
	{
		FmlStr.Delete(0 , 1);
		FmlStr.Delete(FmlStr.GetLength() - 1 , 1);
		FmlStr.Trim();
	}

	if(0 == FmlStr.GetLength())
		return false;/* no valid content*/

	return true;
}

/*
funtion:	find last operator in top level
return:	sonFmlStr end postion;

example: 3 * ((A  + (B - 2 )) + C)  ;
		should return postion of *;
*/

s32 CFormatFile::FindLastOpInTopLevel(CString  FmlStr)
{
	s32 SPPos, SMiPos, SMuPos, SDPos, SEPos;
	s32 Max;

	if(FmlStr.Right(1) == ")")
	{
		/* delete last sub inside () */
		u8 left = 0, right = 1;
		FmlStr.Delete(FmlStr.GetLength() - 1);

		while((left != right) && (FmlStr.GetLength()))
		{
			if(FmlStr.Right(1) == ")")
				right ++;
			if(FmlStr.Right(1) == "(")
				left ++;
			FmlStr.Delete(FmlStr.GetLength() - 1);
		}
	}

	SPPos = FmlStr.rfind('+');
	SMiPos = FmlStr.rfind('-');
	SMuPos = FmlStr.rfind('*');
	SDPos = FmlStr.rfind('/');
	SEPos = FmlStr.rfind('=');

	Max = SPPos;
	if(SMiPos > Max)
		Max = SMiPos;
	if(SMuPos > Max)
		Max = SMuPos;
	if(SDPos > Max)
		Max = SDPos;
	if(SEPos > Max)
		Max = SEPos;

	return Max;
}

CFormula *CFormatFile::ParseFormula(CString  FmlStr)
{
	int  PosSign;
	CFormula *pFmlRet ;

	if(false == CheckAndTrimFmlStr(FmlStr))
		return NULL;

	if(-1 != (PosSign = FindLastOpInTopLevel(FmlStr)))  /*find last sign as last sign is botton priority */
	{
		CFormula 	*pFmlBeforOp , * pFmlAfterOp ;
		CString 		FmlStrBeforOp, FmlStrAfterOp;
		s8 		Op;
		CFormulaOperatorNode *pON;
		FmlStrBeforOp = FmlStr.Left(PosSign);
		FmlStrAfterOp = FmlStr.Right(FmlStr.GetLength() - PosSign - 1);

		Op = FmlStr.at(PosSign);

		if((FmlStrBeforOp.GetLength() == 0) || (FmlStrAfterOp.GetLength() == 0))
			return NULL;

		pFmlBeforOp = ParseFormula(FmlStrBeforOp);
		pFmlAfterOp = ParseFormula(FmlStrAfterOp);

		if((NULL == pFmlBeforOp) || (NULL == pFmlAfterOp))
		{
			if(NULL != pFmlBeforOp)
				delete pFmlBeforOp;
			if(NULL != pFmlAfterOp)
				delete pFmlAfterOp;
			return NULL;
		}

		pON = new CFormulaOperatorNode;
		pFmlRet = new CFormula(pON);
		if(Op  == '+')
		{
			pON->op = CFormulaOperatorNode::PLUS;
		}
		else if(Op  == '-')
		{
			pON->op = CFormulaOperatorNode::MINUS;
		}
		else if(Op  == '*')
		{
			pON->op = CFormulaOperatorNode::MULTIPLY;
		}
		else if(Op  == '/')
		{
			pON->op = CFormulaOperatorNode::DIVIDE;
		}
		else/* if(OpStr  == "=") only = possible */
		{
			pON->op = CFormulaOperatorNode::EQUAL;
		}

		pFmlRet ->AddChild(pFmlBeforOp);
		pFmlRet->AddChild(pFmlAfterOp);
	}
	else/* one leaf formular */
	{
		if(0 == FmlStr.GetLength()) /* make leaf full fill last pFml */
		{
			return NULL;
		}
		if(FmlStr.IsNumberStr())
		{
			CFormulaNumeralNode *pNN = new CFormulaNumeralNode;
			pNN->num = atoi(FmlStr.c_str());
			pFmlRet = new CFormula(pNN);
		}
		else
		{
			CFormulaParameterNode *pPN = new CFormulaParameterNode;
			pPN->param = FmlStr;
			pFmlRet = new CFormula(pPN);
		}
	}

	return pFmlRet;
}



