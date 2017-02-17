#include "StdAfx.h"
#include "include.h"

CTable::CTable(u16	tableid,u16 	pid,u8 Ver)
{
	PID 		= pid;
	TableID 	= tableid;
	Version	=	Ver;
	DupliCountInStream = 1;
	//pNext	=	NULL;
	pNextVer = NULL;
	pSection = NULL;
	//Content = NULL;
	pStarPktIdxAry = NULL;
	StarPktIdxArySize = 0;
	StarPktIdxCount = 0;
	Edit = false;
}

CTable::~CTable(void)
{
	FreeAllSection();
	
	if(pNextVer)
		delete pNextVer;

	if(pStarPktIdxAry)
		free( pStarPktIdxAry);
}


// for build : attach to last
void CTable::AttachSection(CSection * pSec)
{
	if(pSection == NULL)
	{
		pSection = pSec;
	}
	else
	{
		CSection * pSecTmp;
		pSecTmp = pSection;
		while(pSecTmp->pNext)
			pSecTmp = pSecTmp->pNext;
		pSecTmp->pNext = pSec;
	}
	while(pSec)
	{
		AddStartPktIdx(pSec->StartPktIdx);
		pSec = pSec->pNext;
	}

}
// for edit: delete this section
bool CTable::DetachSection(CSection * pSec)
{
	CSection * pSB = NULL, * pS = pSection;
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


bool CTable::UpDateVersion()
{
	if((Version == INVALID_VERSION)&&(pSection))
	{
		Version = pSection->GetSectionVersion();
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

/*
return value only indicate current version table build successful or not

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
bool CTable::IsSame(CTable * pTbl)
{
	if((PID == pTbl->PID)&&(TableID == pTbl->TableID)&&(Version == pTbl->Version)
		&& GetValidSectionCount() == pTbl->GetValidSectionCount() )
	{
		CSection * pSecS = pSection;
		CSection * pSecO = pTbl->pSection;
		while(pSecS)
		{
			if((pSecS->GetSectionLength()  !=  pSecO->GetSectionLength())||
				(memcmp(pSecS->GetSectionData() , pSecO->GetSectionData(),(pSecS->GetSectionLength()+3))!= 0))
				return false;
			pSecS = pSecS->pNext;
			pSecO = pSecO->pNext;
		}
		return true;
	}
	return false;
}
/*	no duplicate it's Duplicate list */
CTable * CTable::Duplicate()
{
	CTable * pTbl = new CTable;
	//pTbl->DataSource = DataSource;
	pTbl->PID 		= PID;
	pTbl->TableID 	= TableID;
	pTbl->Version 	= Version;
	pTbl->DupliCountInStream = DupliCountInStream;

	pTbl->StarPktIdxCount = StarPktIdxCount;
	pTbl->StarPktIdxArySize = StarPktIdxArySize;
	pTbl->pStarPktIdxAry = (u32 *)malloc(StarPktIdxArySize * sizeof(u32));
	memcpy(pTbl->pStarPktIdxAry, pStarPktIdxAry,StarPktIdxArySize * sizeof(u32));
	
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
bool CTable::SaveToFile(CStdioFile * pFile)
{
	CString FileName, StrLine;

	assert(pFile);
	/*	save table head	*/
	StrLine.Format("TableID(0x%x),Version(0x%x),PID(0x%x):\n",	TableID,Version,PID);
	pFile->WriteString(StrLine);
	CSection * pSc = pSection;
	int SecIdx =0;
	u8 * SecData;
	/*	save sections		*/
	while(pSc)
	{
		StrLine.Format("Section:0x%x, start packet index in es 0x%x",SecIdx ,pSc->StartPktIdx);
		pFile->WriteString(StrLine);
		StrLine.Empty();
		SecData = pSc->GetSectionData();
		for(int i = 0; i < (pSc->GetSectionLength()+3); i++)
		{
			if(i%16 == 0)
			{
				StrLine.AppendFormat("\n");
			}
			StrLine.AppendFormat("0x%02x ",SecData[i]);
		}
		StrLine.AppendFormat("\n\n");
		pFile->WriteString(StrLine);
		StrLine.Empty();
		SecIdx++;
		pSc = pSc->pNext;
	}
	if(pNextVer)
	{
		pNextVer->SaveToFile(pFile);
	}
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
u16 *  CTable::AnalyseSubTablePIDArray( )
{
	CSection * pSec = pSection;
	u16 * PidArray = NULL;
	while(pSec)
	{
		PidArray = pSec->AnalyseSubTablePID(PidArray);
		pSec = pSec->pNext;
	}
	return PidArray;
}

bool CTable::FreeAllSection(void)
{
	CSection * pSecB,* pSec = pSection;
	while(pSec)
	{
		pSecB = pSec;
		pSec = pSec->pNext;
		delete pSecB;
	}
	pSection = NULL;
	return 0;
}
/*
u16 CTable::GetSectionLength(u8 SecNum)
{
	return GetSectionByNumber(SecNum)->GetSectionLength();
}
*/
CSection * CTable::GetSectionByNumber(u16 SecNum)
{
	CSection * pSec = pSection;
	while(pSec )
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
	if((PID == INVALID_PID)||(TableID == INVALID_TABLE_ID))
		{
		return false;
		}
	return true;
}

u16 CTable::GetValidSectionCount(void)
{
	u16	ValidCount = 0;
	CSection * pSec = pSection;
	while(pSec )
	{
		ValidCount++;
		pSec = pSec->pNext;
	}
	return ValidCount;
}

void	CTable::DuplicateRecord(CTable * Duplicate)
{	
	CSection * pSec = Duplicate->pSection;
	while(pSec)
	{
		AddStartPktIdx(pSec->StartPktIdx);
		pSec = pSec->pNext;
	}
	DupliCountInStream++; 
	
};

void	CTable::AddStartPktIdx(u32 StarPktIdx)
{	
	if(StarPktIdxCount > StarPktIdxArySize)
		assert(0);
	
	if(StarPktIdxCount == StarPktIdxArySize)
	{
		StarPktIdxArySize += PACKET_INDEX_ARRAY_STEP;
		pStarPktIdxAry = (u32 *) realloc(pStarPktIdxAry, StarPktIdxArySize * sizeof(u32));
	}
	pStarPktIdxAry[StarPktIdxCount] = StarPktIdx;
	StarPktIdxCount++;
	
};
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
