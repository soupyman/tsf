// TFDFile.cpp : 实现文件
//

#include "stdafx.h"
#include "include.h"
//#include "DvbTsa.h"

//#include "PrivateFormatFile.h"

//IMPLEMENT_DYNAMIC(CFormatDefineFile, CStdioFile)
CFormatDefineFile::CFormatDefineFile()
{
	LastLine.Empty();
	ErrorInfo.Empty();
}

CFormatDefineFile::~CFormatDefineFile()
{

}


bool CFormatDefineFile::ReadNextLine(CString & pNl)
{
	int pos;
	pNl.Empty();
	while(ReadString( pNl))
	{
		LastLine = pNl;/*back up */
		pos = pNl.Find('#');
		if(-1 != pos)
			pNl.Delete(pos,pNl.GetLength() - pos);

		pos = pNl.Find('@'); /*@ is for include or means comments, both is useless here .*/
		if(-1 != pos)
			pNl.Delete(pos,pNl.GetLength() - pos);
		
		pNl.Trim();
		
		if(pNl.GetLength() != 0)
			break;
	}
	if(pNl.GetLength() != 0)	
		return true;
	else
		return false;
}

bool CFormatDefineFile::ReadIncludeFile(CString & pFileName)
{
	int pos;
	pFileName.Empty();
	while(ReadString( pFileName))
	{
		if(-1 != (pos = pFileName.Find('#')))
			pFileName.Delete(pos,pFileName.GetLength() - pos);
		
		
		if(-1 != (pos = pFileName.Find("@include")))
		{
			pFileName.Delete(0, pos + 8);
			pFileName.Trim();
			/* if same as this files name then ignore it .*/
			if((pFileName.GetLength())&&(0 != GetFileName().CompareNoCase( pFileName)))
				break;
		}
	}
	if(pFileName.GetLength() != 0)	
		return true;
	else
		return false;
}


/*
pSub point to sub list
return Head List
*/
CFormatTree *  CFormatDefineFile::ParseHead(CString & pHdl)
{
	int PosStart, PosEnd,PosColon1,PosColon2;
	PosStart = pHdl.Find('<');
	PosEnd =  pHdl.Find('>');
	PosColon1 = pHdl.Find(':');
	PosColon2 = pHdl.ReverseFind(':');
	if((PosEnd > PosColon2)&&(PosColon2> PosColon1)&&(PosColon1> PosStart)&&(PosStart >= 0))
	{/*if format valid */
		CString Type, Name, Identify;
		CFormatTree 	* pft;
		
		/*get type string*/
		pHdl.Delete(0,PosStart+1);
		Type =  pHdl.Left(PosColon1 - (PosStart+1));
		Type.Trim();
		/*get name string*/
		pHdl.Delete(0,PosColon1 - PosStart);
		Name =  pHdl.Left(PosColon2 - (PosColon1+1));
		pHdl.Delete(0,PosColon2 - PosColon1);
		Name.Trim();
		/*get id string*/
		Identify = pHdl.Left(PosEnd - (PosColon2+1));
		Identify.Trim();
		if((Type.GetLength() == 0) || (0 == Name.GetLength()) || (0 == Identify.GetLength()))
		{
			ErrorInfo.AppendFormat("Find bad head: < %s : %s : %s > \r\n",Type,Name,Identify);
		}
		else if(Type.CompareNoCase("table") == 0)
		{
			CTableHeadNode * ptn;
			ptn = new CTableHeadNode;
			
			ptn->Name = Name;
			ptn->ID = atoi(Identify);
			pft = new CFormatTree(ptn);
			
			return pft;
		}
		else if(Type.CompareNoCase("descriptor") == 0)
		{
			CDescriptorHeadNode * ptn;
			ptn = new CDescriptorHeadNode;
			
			ptn->Name = Name;
			ptn->ID = atoi(Identify);
			pft = new CFormatTree(ptn);
			
			return pft;
		}
		else
		{
			ErrorInfo.AppendFormat("Find unknow head type in head: < %s : %s : %s > \r\n",Type,Name,Identify);
		}
	}
	return NULL;
}

bool CFormatDefineFile::ParseDataNode(CString & CurLine, CFormatTree * Father)
{
	int pos;
	CFormatTree * pft;
	CTokenNode * pTn = NULL;
	CString name, lenth , format;
	CFormatTree * FmlCheckBase;
	FmlCheckBase = (CFormatTree *)(Father->GetLastSon());
	if(NULL == FmlCheckBase)/* no son found */
	{
		FmlCheckBase = (CFormatTree *)Father;
	}

	pos = CurLine.Find(':');
	name = CurLine.Left(pos);
	CurLine.Delete(0,pos+1);
	pos = CurLine.Find(':');
	if(-1 != pos)
	{
		lenth = CurLine.Left(pos);
		format = CurLine.Right(CurLine.GetLength() - pos - 1);
	}
	else
	{
		lenth = CurLine;
	}
	name.Trim();
	lenth.Trim();
	format.Trim();

	if((name.GetLength() == 0)||(0 == name.CompareNoCase("Reserve")))
	{
		CReserveNode * pRn;
		pRn = new CReserveNode;
		if(NULL == (pRn->pLengthFml = ParseFormula(lenth)))
		{
			ErrorInfo.AppendFormat("pase formula failed for data length: %s \r\n",lenth);
			delete pRn;
			return false;
		}
		else if(false == pRn->pLengthFml->IsValid(FmlCheckBase))
		{
			ErrorInfo.AppendFormat("check formula failed for data length:\r\n");
			pRn->pLengthFml->DumpToString(ErrorInfo);
			ErrorInfo.AppendFormat("\r\n");
			delete pRn;
			return false;
		}

		pTn = pRn;
	}
	else
	{
		CDataNode * pDn;
		pDn = new CDataNode;
		pDn->NameString = name;
		//pDn->Valuelength = atoi(lenth);
		if(NULL == (pDn->pLengthFml = ParseFormula(lenth)))
		{
			ErrorInfo.AppendFormat("pase formula failed for data length:%s \r\n",lenth);
			delete pDn;
			return false;
		}
		else if(false == pDn->pLengthFml->IsValid(FmlCheckBase))
		{
			ErrorInfo.AppendFormat("check formula failed for data length:\r\n");
			pDn->pLengthFml->DumpToString(ErrorInfo);
			ErrorInfo.AppendFormat("\r\n");
			delete pDn;
			return false;
		}


		if(-1 != format.Find('b'))
			pDn->format = CDataNode::FIELD_FMT_BIN;
		else if(-1 != format.Find('d'))
			pDn->format = CDataNode::FIELD_FMT_DEC;
		else if(-1 != format.Find('x'))
			pDn->format = CDataNode::FIELD_FMT_HEX;
		else if(-1 != format.Find('c'))
			pDn->format = CDataNode::FIELD_FMT_BCD;
		else if(-1 != format.Find('m'))
			pDn->format = CDataNode::FIELD_FMT_MJD;
		else if(-1 != format.Find('s'))
			pDn->format = CDataNode::FIELD_FMT_STR;
		else
		{//default
			pDn->format = CDataNode::FIELD_FMT_HEX;
		}
		
		pTn = pDn;
	}

	pft = new CFormatTree(pTn);
	Father->AddChild(pft);
	return true;
}

bool CFormatDefineFile::ParseKeyDescriptorNode( CFormatTree * Father)
{
	CKeyDescriptorNode * ptn;	
	CFormatTree * pft;
	ptn = new CKeyDescriptorNode;
	pft = new CFormatTree(ptn);
	Father->AddChild(pft);
	return true;
}

/*
funtion:	find last sonFmlStr inside () of FatherFmlStr 
return:	sonFmlStr start postion;
example: 3 * ((A  + (B - 2 )) + C)

int CFormatDefineFile::LastFmlStrInsideBracket(CString & FatherFmlStr ,CString & SonFmlStr )
{
	s32 LeftFirstPos,LeftLastPos = 0;
	s32 RightLastPos = 0;
	CString temp = FatherFmlStr;
	LeftFirstPos = temp.ReverseFind(')');
	while((-1 != (LeftLastPos = temp.ReverseFind('(')))&&
		(-1 != (RightLastPos = temp.ReverseFind(')'))))
	{
		temp = FatherFmlStr.Mid(LeftLastPos+1, RightLastPos-LeftLastPos-1);
		 if ( -1 == temp.Find('('))
	 	{
		 	SonFmlStr = FatherFmlStr.Mid(LeftFirstPos+1, RightLastPos-LeftFirstPos-1);
		 	return RightLastPos;
	 	}
		 LeftLastPos++;
		 RightLastPos++;
	}

	return -1;
}
*/

/*
funtion:	find first sonFmlStr inside () of FatherFmlStr 
return:	sonFmlStr end postion;
*/

bool CFormatDefineFile::CheckAndTrimFmlStr(CString & FmlStr )
{
	s32 LeftLastPos = 0;
	s32 RightLastPos = 0;
	u8 LeftCnt = 0;
	u8 RightCnt = 0;
	while(-1 != (LeftLastPos = FmlStr.Find('(', LeftLastPos)))
	{
		LeftLastPos++;
		LeftCnt++;
	}
	while(-1 != (RightLastPos = FmlStr.Find(')', RightLastPos)))
	{
		RightLastPos++;
		RightCnt++;
	}
	if(RightCnt != LeftCnt)
		return false;

	if(-1 != (LeftLastPos = FmlStr.FindOneOf("+-*/=")))
	{
		if((0 == LeftLastPos ) ||(FmlStr.GetLength() == (LeftLastPos+1) ))
			return false;
	}
	FmlStr.Trim();
	while((FmlStr.Left(1) == "(") &&(FmlStr.Right(1) == ")"))
	{
		FmlStr.Delete(0 ,1);
		FmlStr.Delete(FmlStr.GetLength() -1 , 1);
		FmlStr.Trim();
	}
	FmlStr.Trim();
	
	return true;
}

/*
funtion:	find last operator in top level 
return:	sonFmlStr end postion;

example: 3 * ((A  + (B - 2 )) + C)  ;
		should return postion of *;
*/

s32 CFormatDefineFile::FindLastOpInTopLevel(CString  FmlStr )
{
	s32 SPPos, SMiPos, SMuPos, SDPos, SEPos;
	s32 Max;

	if(FmlStr.Right(1) == ")")
	{/* delete last sub inside () */
		u8 left = 0, right = 1;
		FmlStr.Delete(FmlStr.GetLength()-1);
		
		while((left != right)&&(FmlStr.GetLength()))
		{
			if(FmlStr.Right(1) == ")")
				right ++;
			if(FmlStr.Right(1) == "(")
				left ++;
			FmlStr.Delete(FmlStr.GetLength()-1);
		}
	}
	
	SPPos = FmlStr.ReverseFind('+');
	SMiPos = FmlStr.ReverseFind('-');
	SMuPos = FmlStr.ReverseFind('*');
	SDPos = FmlStr.ReverseFind('/');
	SEPos = FmlStr.ReverseFind('=');

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

/* 
1. this function only parse formula, will check if it's valid by caller.

2. as the left part has priority by default.
	parse serial: we need parse from left to right;
	parse nest: we need parse from right to left.

	example: 3 * D - 1 + ((A  + (B - 2 )) + C) / E
	step 1:  							(/)	
		3 * D - 1 + ((A  + (B - 2 )) + C) 		 E
	step 2: 							(/)	
					 (+) 					 E
		3 * D - 1             ((A  + (B - 2 )) + C) 
	step 3:
	

*/
CFormula * CFormatDefineFile::ParseFormula(CString  FmlStr )
{
	int  PosSign;
	CFormula * pFmlRet ;

	if(false == CheckAndTrimFmlStr(FmlStr))
		return NULL;
	
	if(-1 != (PosSign = FindLastOpInTopLevel(FmlStr)))  /*find last sign as last sign is botton priority */
	{
		CFormula * 	pFmlBeforOp , * pFmlAfterOp ;
		CString 		FmlStrBeforOp, FmlStrAfterOp;
		CString 		OpStr;
		CFormulaOperatorNode * pON;
		FmlStrBeforOp = FmlStr.Left(PosSign);
		FmlStrAfterOp = FmlStr.Right(FmlStr.GetLength() - PosSign - 1);

		OpStr = FmlStr.Mid(PosSign,1);
		
		if((FmlStrBeforOp.GetLength() == 0) || (FmlStrAfterOp.GetLength() == 0))
			return NULL;

		pFmlBeforOp = ParseFormula(FmlStrBeforOp);
		pFmlAfterOp = ParseFormula(FmlStrAfterOp);

		if((NULL == pFmlBeforOp)|| (NULL == pFmlAfterOp))
		{
			if(NULL != pFmlBeforOp)
				delete pFmlBeforOp;
			if(NULL != pFmlAfterOp)
				delete pFmlAfterOp;
			return NULL;
		}
		
		pON = new CFormulaOperatorNode;
		pFmlRet = new CFormula(pON);
		if(OpStr  == "+")
		{
			pON->op = CFormulaOperatorNode::PLUS;
		}
		else if(OpStr  == "-")
		{
			pON->op = CFormulaOperatorNode::MINUS;
		}
		else if(OpStr  == "*")
		{
			pON->op = CFormulaOperatorNode::MULTIPLY;
		}
		else if(OpStr  == "/")
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
		CString up, low;
		if(0 == FmlStr.GetLength()) /* make leaf full fill last pFml */
		{
			return NULL;
		}
		up = low = FmlStr;
		up.MakeUpper();
		low.MakeLower();
		if(up.Compare(low) == 0)// up  case same as low case, should be number
		{
			CFormulaNumeralNode * pNN = new CFormulaNumeralNode;
			pNN->num = atoi(FmlStr);
			pFmlRet = new CFormula(pNN);
		}
		else
		{
			CFormulaParameterNode * pPN = new CFormulaParameterNode;
			pPN->param = FmlStr;
			pFmlRet = new CFormula(pPN);
		}
	}

	return pFmlRet;
}


bool CFormatDefineFile::ParseKeyLoopNode(CString & LengthStr, CFormatTree * Father, CString name)
{
	CFormula * pFml ;
	CKeyLoopNode * ptn;
	CFormatTree * pft;
	CFormatTree * pBT = Father;
	if(pBT->GetChildren())
		pBT = (CFormatTree *)pBT->GetLastSon();
	
	if(NULL == ( pFml = ParseFormula(LengthStr)))
	{
		ErrorInfo.AppendFormat("parse formula fail for Loop \r\n");
		return false;
	}	
	else if(false == pFml->IsValid(pBT))
	{
		ErrorInfo.AppendFormat("check formula failed for Loop:\r\n");
		pFml->DumpToString(ErrorInfo);
		ErrorInfo.AppendFormat("\r\n");
		delete pFml;
		pFml = NULL;
		return false;
	}

	ptn = new CKeyLoopNode;
	ptn->pFml = pFml;
	ptn->Name = name;
	
	pft = new CFormatTree(ptn);
	Father->AddChild(pft);
	
	if(false == ParseSubBody(pft))
		return false;
	
	return true;
}

bool CFormatDefineFile::ParseKeyIfElseNode(CString & CurLine, CFormatTree * Father)
{
	CFormatTree * pBT = Father;
	if(pBT->GetChildren())
		pBT = (CFormatTree *)pBT->GetLastSon();
		
	CFormula * pFml = ParseFormula(CurLine);
	CKeyIfNode * ptn;
	CFormatTree * pft;
	int PosStart, PosEnd ;
	
	if(NULL == pFml)
	{
		ErrorInfo.AppendFormat("parse formula fail for if \r\n");
		return false;
	}
	else if(false == pFml->IsValid(pBT))
	{
		ErrorInfo.AppendFormat("check formula failed for if:\r\n");
		pFml->DumpToString(ErrorInfo);
		ErrorInfo.AppendFormat("\r\n");
		delete pFml;
		pFml = NULL;
		return false;
	}

	ptn = new CKeyIfNode;
	ptn->pFml = pFml;
	pft = new CFormatTree(ptn);
	Father->AddChild(pft);
	
	if(false == ParseSubBody(pft))
		return false;

	ULONGLONG dwPosition = GetPosition();

	ReadNextLine( CurLine);
	PosStart = CurLine.Find("[");
	PosEnd = CurLine.Find("]");
	if((0 <= PosStart)&&(PosStart < CurLine.Find("else"))&&(CurLine.Find("else") < PosEnd))
	{		
		//assert(0);
		//ptn = new CKeyElseNode;
		pft = new CFormatTree(new CKeyElseNode);
		Father->AddChild(pft);

		if(false == ParseSubBody(pft))
			return false;
	}
	else
	{//seekback
		Seek(dwPosition , CFile::begin);
	}
	return true;
}

bool CFormatDefineFile::ParseKeyNode(CString & CurLine, CFormatTree * Father)
{
	CString CurToken, Name;
	int PosStart, PosEnd;
	PosStart = CurLine.Find("[");
	PosEnd = CurLine.Find("]");
	if((0 <= PosStart)&&(PosStart < PosEnd))
	{
		CurToken = CurLine.Mid(PosStart+1,PosEnd- PosStart-1);
		CurLine.Delete(0, PosEnd+1); /*only length string left now */
		PosEnd = CurToken.Find(":");
		Name.Empty();
		if(0 <= PosEnd)/*if there is name string after : */
		{
			Name = CurToken.Right(CurToken.GetLength() - (PosEnd+1));
			Name.Trim();
			
			CurToken = CurToken.Left(PosEnd);
		}
		CurToken.Trim();
		if(0 == CurToken.CompareNoCase("loop"))/* new loop node */
		{
			return ParseKeyLoopNode(CurLine , Father, Name);
		}
		else if(0  == CurToken.CompareNoCase("if"))/* new if node */
		{
			return ParseKeyIfElseNode(CurLine , Father);
		}
		else if(0  == CurToken.CompareNoCase("descriptor"))/* new if node */
		{
			return ParseKeyDescriptorNode( Father);
		}
	}
	else
	{
		ErrorInfo.AppendFormat("Invalid line: \r\n %s \r\n",CurLine);
	}
	return false;
}

bool CFormatDefineFile::ParseSubBody(CFormatTree * pFather )
{
	SUB_PASE_PHASE pp = SEARCH_START;
	CString CurLine ;
	bool AcceptElse = false;
	bool empty = true;
	while(ReadNextLine( CurLine))
	{
		switch(pp)
		{
			case SEARCH_START:
				if(-1 != CurLine.Find('{'))
				{
					pp = SEARCH_END;
				}
				break;	
			case SEARCH_END:
				if(-1 != CurLine.Find('}'))
				{
					if(empty)
					{
						ErrorInfo.AppendFormat("empty sub body found. \r\n");
						return false;
					}
					else
						return true;		/* legal exit	*/
				}	
				if((-1 == CurLine.FindOneOf("[]")) &&(-1 != CurLine.Find(':')))/* new data node */
				{
					if(false == ParseDataNode(CurLine, pFather))
						return false;
				}
 				else if(false == ParseKeyNode(CurLine, pFather))
				{
					return false;
				}
				empty = false;
				break;
		}
	}
	
	if(pp == SEARCH_START)
	{
		ErrorInfo.AppendFormat("search '{' failed in parse sub body \r\n");
	}
	else if(pp == SEARCH_END)
	{
		ErrorInfo.AppendFormat("search '}' failed in parse sub body \r\n");
	}
	return false;	/* illegal exit	; report error*/
}

bool CFormatDefineFile::ParseInclude(CFormatDatabase & pFDB)
{
	CFormatDefineFile  IncludFmtfile;
	CString InclueFileName,FilePath, FathName;
	SeekToBegin();

	FilePath = GetFilePath();
	FilePath = FilePath.Left(FilePath.ReverseFind('\\')+1);
	
	while(ReadIncludeFile( InclueFileName))
	{
		/*for current path is not clear, so InclueFileName should append this file name's path*/
		FathName = 	FilePath;
		FathName.Append(InclueFileName);
		if(IncludFmtfile.Open( FathName , CFile::modeRead))
		{
			if(!IncludFmtfile.UpdateFormatToDB(pFDB))
			{ 
				ErrorInfo.Append(IncludFmtfile.ErrorInfo);
				ErrorInfo.AppendFormat("Load included format file %s in file %s fail. \r\n", InclueFileName, GetFileName());
				IncludFmtfile.Close();
				return false;
			}
			IncludFmtfile.Close();
		}
		else
		{
			ErrorInfo.AppendFormat("Open included format file (%s) in file %s fail. \r\n", InclueFileName, GetFileName());
			return false;
		}
	}
	return true;
}

bool CFormatDefineFile::UpdateFormatToDB(CFormatDatabase & pFDB)
{
	CFormatTree * pft;
	CString CurLine ;

	ErrorInfo.Empty();
	if(false == ParseInclude(pFDB))
	{
		return false;
	}
	SeekToBegin();
	while(ReadNextLine( CurLine))
	{
		if(pft = ParseHead(CurLine))		
		{
			if(false == pFDB.CheckFormatHead(pft , ErrorInfo))
			{/* check if it's already exist */
				delete pft;
				pFDB.FreeFormatList();
				return false;
			}
			if(false == ParseSubBody(pft))
			{
				delete pft;
				pFDB.FreeFormatList();
				ErrorInfo.AppendFormat("UpdateFormatToDB Sub Body failed:\r\n on line: %s \r\n",LastLine);
				return false;
			}
			pFDB.AddFormat(pft);
		}
	}
	return true;
}



