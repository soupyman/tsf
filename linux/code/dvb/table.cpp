
#include "table.h"
#include "contentfile.h"
#include "contenttree.h"
#include "formatserver.h"

#include <string.h>
#include <stdlib.h>

CTable::CTable(u16	tableid, u16 	pid, u8 Ver)
{
	PID 		= pid;
	TableID 	= tableid;
	Version	=	Ver;
	SubTableID = 0;
	SubIDMask = 0;
	pNextVer = NULL;
	pSection = NULL;
	pContent  = NULL;
}

CTable::~CTable(void)
{
	FreeAllSection();

	if(pContent)
		delete pContent;

	if(pNextVer)
		delete pNextVer;

}

void        CTable::SetIds(u16 tableid , u16 pid,  u16	SubID, u16	SubMask)
{
	PID 		= pid;
	TableID 	= tableid;
	SubTableID =  SubID;
	SubIDMask =  SubMask;
}

bool CTable:: IsIntegrity()
{
	u16 SecCnt, LastSecNum;
	CSection *pSecTmp;

	if(pSection == NULL)
	{
		return false;
	}
	LastSecNum = pSection->GetLastSectionNum();
	SecCnt = 0;
	pSecTmp = pSection;
	while(pSecTmp)
	{
		SecCnt++;
		pSecTmp = pSecTmp->pNext;
	}

	if(SecCnt == (LastSecNum + 1))
	{
		return true;
	}

	return false;
}

void CTable::SortSection(void)
{
	CSection *pSecList , *pSecCur , * pSecPosPrev , * pSecPosNext ;

	if((NULL == pSection) || (NULL == pSection->pNext))
		return;

	pSecList = pSection->pNext;
	pSection->pNext = NULL;

	while(pSecList)
	{
		pSecCur = pSecList;
		pSecList = pSecList->pNext;

		pSecPosPrev = NULL;
		pSecPosNext = pSection;
		while((NULL != pSecPosNext) &&
				(pSecPosNext->GetSectionNum() < pSecCur->GetSectionNum()))
		{
			pSecPosPrev = pSecPosNext;
			pSecPosNext = pSecPosNext->pNext;
		}

		pSecCur->pNext = pSecPosNext;
		if(NULL == pSecPosPrev)
		{
			pSection = pSecCur;
		}
		else
		{
			pSecPosPrev->pNext = pSecCur;
		}

	}


}

bool CTable::FreeAllSection(void)
{
	CSection *pSecTmp;

	while(pSection)
	{
		pSecTmp = pSection;
		pSection = pSecTmp->pNext;
		delete pSecTmp;
	}
	return true;
}

// for build : attach to last
bool CTable::AttachSection(CSection *pSec)
{
	if(pSection == NULL)
	{
		pSection = pSec;
	}
	else
	{
		CSection *pSecTmp;
		pSecTmp = pSection;
		while(pSecTmp)
		{
			if(pSecTmp->IsSame(pSec))
			{
				delete pSec;
				return false;
			}
			pSecTmp = pSecTmp->pNext;
		}
		pSec->pNext = pSection;
		pSection = pSec;
	}
	return true;
}
// for edit: delete this section
bool CTable::DetachSection(CSection *pSec)
{
	CSection *pSB = NULL, * pS = pSection;
	while(pS)
	{
		if(pS == pSec)
		{
			if(pS == pSection)
			{
				pSection = pSection->pNext;
			}
			else
			{
				pSB->pNext = pS->pNext;
			}
			delete pS;
			return true ;
		}
		else
		{
			pSB = pS;
			pS = pSB->pNext;
		}
	}
	return false;
}

bool CTable::UpDateSectionCrc()
{
	CSection   *pS = pSection;
	while(pS)
	{
		pS->ReCalCrc();
		pS = pS->pNext;
	}
	return true;
}

bool CTable::UpDateVersion()
{
	if(pSection)
	{
		Version = pSection->GetSectionVersion();
		return true;
	}
	return false;
}

bool CTable::UpDateIdsFromSection()
{
	if(pSection)
	{
		CommonSection *pCS = (CommonSection *)pSection->GetSectionData();

		TableID = pCS->table_id;
		SubTableID = pCS->sub_table_id_hi * 256 + pCS->sub_table_id_low;

		return true;
	}
	return false;
}


/*
post condition : CurPktIdx is first packet index of the first section.

Result CTable::BuildPrivious()
{
	Result ret = UNVALID;
	s32 CurPktIdxBk;
	CSection * pSec;
	if((IsValid())&&(CurPktIdx>0))
	{
		u8 SectionNumber = 0;
		CESData * pES = DataSource->FindESDataByPID(PID);
		if((pES)&&(pES->FindBackwardSection(CurPktIdx,TableID, SectionNumber )))
		{
			FreeAllSection();
			CurPktIdxBk = CurPktIdx;
			while(1)
			{
				pSec = new CSection;
				if(pSec->Build(pES,CurPktIdxBk))
				{
					AttachSection(pSec);
					if(pSec->IsLastSection())
					{
						ret = SUCCESSED;
						break;
					}
				}
				SectionNumber++;
				if(false == pES->FindForwardSection(CurPktIdxBk,TableID, SectionNumber))
				{
					ret = DATAERROR;
					break;
				}
			}
		}
		else if(pES == NULL)
		{
			ret = DATAERROR;
		}
	}
	if(ret == DATAERROR)
		FreeAllSection();

	return ret;
}


//return value only indicate current version table build successful or not

Result CTable::BuildNext()
{
	Result ret = UNVALID;
	CSection * pSec;
	if(!IsValid())
		ret =  DATAERROR;
	if(ret != DATAERROR)
	{
		u8 SectionNumber = 0;
		CESData * pES = DataSource->FindESDataByPID(PID);
		if(pES->GetPacketCunt() == 0)
		{
			Sleep(100);
		}
		if(NULL != pES)
		{
			FreeAllSection();
			while(1)
			{
				if(false == pES->FindForwardSection(CurPktIdx,TableID, SectionNumber))
				{
					ret = DATAERROR;
					break;
				}
				pSec = new CSection;
				if(pSec->Build(pES,CurPktIdx))
				{
					AttachSection(pSec);
					if(pSec->IsLastSection())
					{
						ret = SUCCESSED;
						break;
					}
				}
				else
				{
					delete pSec;
					break;
				}
				SectionNumber++;
			}
		}
		else
		{
			ret = DATAERROR;
		}
	}
	if(ret == DATAERROR)
		FreeAllSection();
	return ret;
}
*/
bool CTable::IsSame(CTable *pTbl)
{
	if((PID == pTbl->PID) && (TableID == pTbl->TableID) && (Version == pTbl->Version)
			&& GetValidSectionCount() == pTbl->GetValidSectionCount())
	{
		CSection *pSecS = pSection;
		CSection *pSecO = pTbl->pSection;
		while(pSecS)
		{
			if(false == pSecS->IsSame(pSecO))
				return false;
			pSecS = pSecS->pNext;
			pSecO = pSecO->pNext;
		}
		return true;
	}
	return false;
}
/*	no duplicate it's Duplicate list */
CTable *CTable::Duplicate()
{
	CTable *pTbl = new CTable;
	//pTbl->DataSource = DataSource;
	pTbl->PID 		= PID;
	pTbl->TableID 	= TableID;
	pTbl->Version 	= Version;
	//pTbl->DupliCountInStream = DupliCountInStream;

	if(pSection)
		pTbl->pSection = pSection->Duplicate();
	else
		pTbl->pSection = NULL;

	if(pNextVer)
		pTbl->pNextVer = pNextVer->Duplicate();
	else
		pTbl->pNextVer = NULL;

	return pTbl;
}

/*
void CTable::CopyTable(CTable * pTbl)
{

	PID 		= pTbl->PID;
	TableID 	= pTbl->TableID;
	Version 	= pTbl->Version;
	FreeAllSection();
	if(pTbl->pSection)
	{
		AttachSection(pTbl->pSection->Duplicate());
	}
}
void CTable::ReplaceSectionData(u8 SecNum, u8 * pNewData, u32 DataLen)
{
	CSection * pSec = GetSectionByNumber(SecNum);

	if((pSec )&&(false == pSec->IsSame(pNewData,DataLen)))//updated
	{
		if(pBack == NULL)
			pBack = Duplicate();
		//OutDated = true;
		//if(pSec->SectionData)
		//	free(pSec->SectionData);
		//pSec->SectionData = (u8 *)malloc(DataLen);
		//memcpy(pSec->SectionData, pNewData , DataLen);
		pSec->ReplaceData(pNewData,DataLen);
	}
}
*/



bool CTable::DumpToString(CString  &Str)
{
	CSection *pSc = pSection;
	int SecIdx = 0;
	u8 *SecData;

	/*	 table head	*/
	Str.AppendFormat("TableID(0x%x),PID(0x%x),Version(0x%x),SubId(0x%x),SubMask(0x%x):\n",
					 TableID, PID, Version, SubTableID, SubIDMask);

	/*	 sections		*/
	while(pSc)
	{
		Str.AppendFormat("Section:0x%x, start packet index in TS 0x%x \n", SecIdx , pSc->StartPktIdx);

		SecData = pSc->GetSectionData();
		for(int i = 0; i < (pSc->GetSectionLength() + 3); i++)
		{
			if(i % 8 == 0)
			{
				Str.AppendFormat("\n");
			}
			Str.AppendFormat("0x%02x ", SecData[i]);
		}
		Str.AppendFormat("\n\n");

		SecIdx++;
		pSc = pSc->pNext;
	}
	if(pNextVer)
	{
		pNextVer->DumpToString(Str);
	}

	return true;
}

bool CTable::SaveToBinFile(CFile *pFile)
{
	CTable *pTbl;
	CSection *pSc;
	if(NULL == pFile)
	{
		ERR("Param ERROR.\n");
		return false;
	}
	pSc = pSection;
	/*	save sections		*/
	while(pSc)
	{
		pFile->Write(pSc->GetSectionData(), (pSc->GetSectionLength() + 3));
		pSc = pSc->pNext;
	}
	pTbl = 	pNextVer;
	while(pTbl)
	{
		pSc = pTbl->pSection;
		while(pSc)
		{
			/*	save sections		*/
			pFile->Write(pSc->GetSectionData(), (pSc->GetSectionLength() + 3));
			pSc = pSc->pNext;
		}
		pTbl = pTbl->pNextVer ;
	}

	return true;
}

bool CTable::SaveToBinFile(CString FileName)
{
	CFile *pFile;
	bool ret = false;
	pFile = new CStreamFile(FileName.c_str());

	pFile->Open(CFile::OPEN_BIN_WRITE_ONLY);
	pFile->SeekTo(0);

	if(pContent)
	{
		SaveToBinFile(pFile);

		ret = true;
	}

	pFile->Close();
	delete pFile;

	return ret;
}

bool CTable::SaveToTxtFile(CString FileName)
{
	CStreamFile *pFile;
	bool ret = false;
	pFile = new CStreamFile(FileName.c_str());

	pFile->Open(CFile::OPEN_TEXT_WRITE_ONLY);
	pFile->SeekTo(0);

	if(pContent)
	{
		pContent->SaveToTxtFile(pFile);

		ret = true;
	}

	pFile->Close();
	delete pFile;

	return ret;
}

bool CTable::SaveToXmlFile(CString FileName)
{
	CStreamFile *pFile;
	bool ret = false;
	pFile = new CStreamFile(FileName.c_str());

	pFile->Open(CFile::OPEN_TEXT_WRITE_ONLY);
	pFile->SeekTo(0);

	if(pContent)
	{
		pContent->SaveToXmlFile(pFile, 0);

		ret = true;
	}

	pFile->Close();
	delete pFile;

	return ret;
}

bool CTable::SavBinToTxtFile(CFile *pFile)
{
	char  StrLine[256];
	//CFile *pFile = new CFile(filename);
	CTable *pTbl;

	if(NULL == pFile)
	{
		ERR("Param ERROR.\n");
		return false;
	}
	//pFile->Open(CFile::OPEN_TEXT_WRITE_ONLY);
	//pFile->SeekTo(0);
	/*	save table head	*/
	sprintf(StrLine, "TableID(0x%x),PID(0x%x),Version(0x%x),SubId(0x%x),SubMask(0x%x):\n",
			TableID, PID, Version, SubTableID, SubIDMask);
	pFile->Write((u8 *)StrLine, strlen(StrLine));
	CSection *pSc = pSection;
	int SecIdx = 0;
	u8 *SecData;
	/*	save sections		*/
	while(pSc)
	{
		sprintf(StrLine, "Section:0x%x, start packet index in TS 0x%x \n", SecIdx , pSc->StartPktIdx);
		pFile->Write((u8 *)StrLine, strlen(StrLine));

		SecData = pSc->GetSectionData();
		for(int i = 0; i < (pSc->GetSectionLength() + 3); i++)
		{
			if(i % 8 == 0)
			{
				sprintf(StrLine, "\n");
				pFile->Write((u8 *)StrLine, strlen(StrLine));
			}
			sprintf(StrLine, "0x%02x ", SecData[i]);
			pFile->Write((u8 *)StrLine, strlen(StrLine));
		}
		sprintf(StrLine, "\n\n");
		pFile->Write((u8 *)StrLine, strlen(StrLine));

		SecIdx++;
		pSc = pSc->pNext;
	}
	pTbl = 	pNextVer;
	while(pTbl)
	{
		/*	save table head	*/
		sprintf(StrLine, "\nTableID(0x%x),PID(0x%x),Version(0x%x),SubId(0x%x),SubMask(0x%x):\n",
				pTbl->TableID, pTbl->PID, pTbl->Version, pTbl->SubTableID, pTbl->SubIDMask);
		pFile->Write((u8 *)StrLine, strlen(StrLine));
		CSection *pSc = pTbl->pSection;
		int SecIdx = 0;
		u8 *SecData;
		/*	save sections		*/
		while(pSc)
		{
			sprintf(StrLine, "Section:0x%x, start packet index in TS 0x%x \n", SecIdx , pSc->StartPktIdx);
			pFile->Write((u8 *)StrLine, strlen(StrLine));

			SecData = pSc->GetSectionData();
			for(int i = 0; i < (pSc->GetSectionLength() + 3); i++)
			{
				if(i % 8 == 0)
				{
					sprintf(StrLine, "\n");
					pFile->Write((u8 *)StrLine, strlen(StrLine));
				}
				sprintf(StrLine, "0x%02x ", SecData[i]);
				pFile->Write((u8 *)StrLine, strlen(StrLine));
			}
			sprintf(StrLine, "\n\n");
			pFile->Write((u8 *)StrLine, strlen(StrLine));

			SecIdx++;
			pSc = pSc->pNext;
		}

		pTbl = pTbl->pNextVer ;
	}

	//pFile->Close();
	//delete pFile;
	return true;
}

/*
bool CTable::LoadFromFile(CString * pFileName)
{
	CString FileName;
	CFile pFile;
	if(pFileName == NULL)
		FileName.Format(".\\table(PID 0x%x,TableID 0x%x).tbl",PID,TableID);
	else
		FileName = *pFileName;

	if(! pFile.Open((LPCTSTR)FileName,CFile::modeRead ))
	{
		CString * pInfo = new CString;
		pInfo->Format("Load Table(TableID = 0x%x;PID = 0x%x) fail for open ES file fail;\r\n", TableID,PID);
		MessageToUI(MSG_UI_INFORMATION,	(WPARAM)pInfo);
		return false;
	}
	pFile.SeekToBegin();
	pFile.Read(this,sizeof(CTable));
	CSection * pSc = pSection;// should new first
	while(pSc)
	{
		pFile.Read(pSc->SectionData,pSc->GetSectionLength()+3);
		pSc = pSc->pNext;
	}
	pFile.Close();
	CString * pInfo = new CString;
	pInfo->Format("Save ES(PID = 0x%x) successed;\r\n", PID);
	MessageToUI(MSG_UI_INFORMATION,	(WPARAM)pInfo);
	return true;
}


bool CTable::WriteBackToTS(void)
{
	bool  ret = false;
	CESData * pES = DataSource->FindESDataByPID(PID);
	if((!IsValid())||(pBack == NULL)||(pSection == NULL)||(pES == NULL))
	{
		return false;
	}

	CTable * pCTable = new CTable;
	pCTable->Config( DataSource,  TableID, 	PID);
	pCTable->SetCurPktIdx(CurPktIdx);
	while(1)//find to the first
	{
		if(pCTable->BuildPrivious() <  FAILSUCCGAP)
		{
			pCTable->CurPktIdx -- ;
			break;
		}
		if(false == pCTable->IsSame(pBack))
		{
			break;
		}
	}
	while(( pCTable->BuildNext() > FAILSUCCGAP)&&(pCTable->IsSame(pBack)))//over write one by one
	{
		CSection * pSec = pSection;
		CSection * pCurSec = pCTable->pSection;
		while(pSec)
		{
			ret = pSec->WriteBack(pES, pCurSec->StartPktIdx);
			pSec = pSec->pNext;
			pCurSec = pCurSec->pNext;
		}
	}
	delete pCTable;
	delete pBack;
	pBack = NULL;
	//OutDated = false;
	return ret; // write back successed out
}
*/
/*	return subtable Pid array, with first word is lenth	*/
u16   *CTable::AnalyseSubTablePIDArray()
{
	CSection *pSec = pSection;
	u16 *PidArray ;
	PidArray = (u16 *)malloc(sizeof(u16)); /*first for pid count */
	PidArray[0] = 0;

	while(pSec)
	{
		PidArray = pSec->AnalyseSubTablePID(PidArray);
		pSec = pSec->pNext;
	}
	return PidArray;
}

/*
u16 CTable::GetSectionLength(u8 SecNum)
{
	return GetSectionByNumber(SecNum)->GetSectionLength();
}
*/
CSection *CTable::GetSectionByNumber(u16 SecNum)
{
	CSection *pSec = pSection;
	while(pSec)
	{
		if(pSec->GetSectionNum() == SecNum)
		{
			return pSec;
		}
		pSec = pSec->pNext;
	}
	return NULL;
}

/*
u8 CTable::GetLastSectionNumber(void)
{
	CommonSection * pCS = (CommonSection *)Sections[0];
	if(pCS)
		return (pCS->section_syntax_indicator ? (pCS->last_section_number):0);

	return 0;
}

u8	CTable::GetSectionSyntaxIndicator(void)
{
	if(pSection)
		return (pSection->GetSectionSyntaxIndicator());

	return 0xFF;
}
*/
bool CTable::IsValid(void)
{
	//TRACE("PID = %x\t; TableID = %x \n",PID,TableID);
	if((PID == INVALID_PID) || (TableID == INVALID_TABLE_ID))
	{
		return false;
	}
	return true;
}

u16 CTable::GetValidSectionCount()
{
	u16	ValidCount = 0;
	CSection *pSec = pSection;
	while(pSec)
	{
		ValidCount++;
		pSec = pSec->pNext;
	}
	return ValidCount;
}
/*
	return repetion time in millseconds
*/
u32 CTable::GetRepetionTime()
{
	switch(TableID)
	{
		case ACTUAL_NETWORK_INFORMATION:
			return NIT_REPET_RATE;
		case BOUQUET_ASSOCIATION_INFORMATION:
			return BAT_REPET_RATE;
		default:
			return DEFAULT_REPET_RATE;
	}
	return DEFAULT_REPET_RATE;
}

bool  CTable::LoadFromContentTree(CContentTree *pCt)
{
	CFormatServer fs;

	if(pCt->GetNodeType() == CContentTreeNode::ROOT_NODE)
	{
		if(pCt->GetChildren())
		{
			CContentTree *child = (CContentTree *)pCt->GetChildren();

			while(child)
			{
				if(true == LoadFromContentTree(child))
					return true;

				child = (CContentTree *)(child->GetRightBrother());
			}
		}
	}
	if(pCt->GetNodeType() == CContentTreeNode::TABLE_NODE)
	{
		CSection *pSec;
		CContentTableNode *pCTN = (CContentTableNode *)pCt->GetMyNode();
		SetPID(pCTN->PID);
		if(pCt->GetChildren())
		{
			CContentTree *child = (CContentTree *)pCt->GetChildren();
			u32 index;
			while((child) && (child->GetNodeType() == CContentTreeNode::SECTION_NODE))
			{
				pSec = new CSection;
				index = 0;
				pSec->SectionData = (u8 *)malloc(4096);   /* malloc MAX_SECTION_LENGTH*/
				memset(pSec->SectionData, 0 , 4096);
				child->FillData(pSec->SectionData , index);
				if(((index / 8) - 3) != pSec->GetSectionLength())
				{
					ERR("section lenth %x error, should be %x .", 
						pSec->GetSectionLength(), (index / 8) - 3);
				}
				AttachSection(pSec);
				child = (CContentTree *)(child->GetRightBrother());
			}
			if(IsIntegrity())
			{
				UpDateVersion();
				UpDateIdsFromSection();
				SortSection();
				if(true ==  fs.IsCrcNeed(this))
				{
					UpDateSectionCrc();
				}
				return true;
			}
		}

	}
	return false;
}
bool  CTable::LoadFromXmlFile(CString FileName)
{
	CContentFile *pCntFile ;
	CString  TraceStr;

	pCntFile = new CContentFile(FileName.c_str());
	if(false == pCntFile->Load())
	{
		ERR("Load xml file %s failed. ", FileName.c_str());
		delete pCntFile;
		return false;
	}

	FreeAllSection();

	if(false == LoadFromContentTree(pCntFile->pCTree))
	{
		pCntFile->pCTree->DumpToString(TraceStr, 0);
		ERR(" %s \nTransferToTable() failed. ", TraceStr.c_str());
		delete pCntFile;
		return false;
	}

	if(pContent)
	{
		delete pContent;
	}
	pContent = pCntFile->pCTree;
	pCntFile->pCTree = NULL;

	delete pCntFile;

	return true;
}
/*

u16 CTable::CRCCheckReport(void)
{
	CRC CrcInstan;
	u16	CorrectCount = 0;
	for(int i=0;i<MAX_SECTION_NUMBER;i++)
	{
		if(Sections[i] != NULL)
		{
			if(CrcInstan.CheckCrc32(Sections[i], (GetSectionLength(i)+3)))
			{
				CorrectCount ++;
			}
		}
	}
	return CorrectCount;
}


bool CTable::AttachToLast(CTable * NewTbl)
{
	if((PID != NewTbl->PID)&&(TableID != NewTbl->TableID))
		return false;
	CTable	*	pTbl = this;

	while(pTbl->pNextVer)
		pTbl = pTbl->pNextVer;

	pTbl->pNextVer = NewTbl;
	return true;
}
*/
