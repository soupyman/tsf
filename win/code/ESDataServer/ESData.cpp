#include "StdAfx.h"
#include "include.h"

CESData::CESData(u16 pid)
{
	//pParentTS = pTS;
	PID = pid;
	CurPktIdx = 0;
	//PktTree = NULL;
	Integrity = false;
	//Deleted = false;
	OldPktIdxInTsArry = NULL;
	OldPktIdxCnt = 0;
	//CDemux::GetInstance()->AddDemuxFilter( this);
}

CESData::~CESData(void)
{
	if(OldPktIdxInTsArry)
		free(OldPktIdxInTsArry);

}
/*
bool CESData::IsAvailable()
{
	u32 PktIdx =0 ;
	u32	CheckMax = pParentTS->m_nTotalPacketNumber;
	
	if(CheckMax > CHECK_AVAILABLE_MAX_PACKET_NUMBER)
		CheckMax = CHECK_AVAILABLE_MAX_PACKET_NUMBER;
	
	while(PktIdx< CheckMax)
	{
		if(PID == pParentTS->GetPacketPID(PktIdx))
		{
			return true;
		}
		PktIdx ++;
	}
	return false;
}
*/

BOOL CESData::ReturnMessage(WPARAM wP, LPARAM lP)
{
	return AfxGetMainWnd()->PostMessage(MSG_UI_INFORMATION,wP,lP);
}


/*
always attach to last
*/

CPacket * CESData::GetCurPacket(void)
{
	return GetPacket(CurPktIdx);
}
/*
return true: reach es end. false hasn't reach es end
*/
bool CESData::IfEsBuildFinish( )
{
	return Integrity;
}
/*
CurIdx is the start packet of the section

bool CESData::FindBackwardSection(s32 & CurIdx ,u16	tableid,u8 SecNmb)
{
	CPacket  * pPkt ;
	CommonSection * pPS; 
	while(NULL != (pPkt = GetPacket(--CurIdx)))
	{
		//pPkt = GetPacket(--CurIdx);
		if(pPkt->IsUnitStart())
		{
			pPS = pPkt->GetSectionAddress();
			if((pPS->table_id == tableid)&&
				(pPS->section_syntax_indicator ? 
				(pPS->section_number == SecNmb):1))
			{
				return true;
			}
		}
		if(CurIdx<0)
			break;
	}
	CurIdx ++;
	return false;
}
*/
bool CESData::FindForwardSection(s32 & CurIdx ,u16	tableid,u8 SecNmb)
{
	CPacket  * pPkt ;
	CommonSection * pPS; 
	while(NULL != (pPkt = GetPacket(++CurIdx)))
	{
		if(pPkt->IsUnitStart())
		{
			pPS = pPkt->GetSectionAddress();
			if((pPS->table_id == tableid)&&
				(pPS->section_syntax_indicator ? 
				(pPS->section_number == SecNmb):1))
			{
				return true;
			}
		}
	}
	CurIdx--;
	return false;
}

u32 CESData::FindOtherSectionCount(u16	tableid)
{
	CPacket  * pPkt ;
	CommonSection * pPS; 
	s32  CurIdx = -1;
	u32 OtherSectionCount = 0;
	while(NULL != (pPkt = GetPacket(++CurIdx)))
	{
		if(pPkt->IsUnitStart())
		{
			pPS = pPkt->GetSectionAddress();
			if(pPS->table_id != tableid)
			{
				OtherSectionCount++;
			}
		}
	}

	return OtherSectionCount;
}


CSection * CESData::BuildSection(s32 & CurIdx ,u16	tableid,u8 SecNmb)
{
	if(false == FindForwardSection(CurIdx ,	tableid, SecNmb))
		return NULL;
	
	u32 NeedCopyCount ;//  , SecBuffLen;
	CPacket  * pPkt = GetPacket(CurIdx);
	u8	PktSize = pPkt->GetPktSize();
	CommonSection * pPS = pPkt->GetSectionAddress();
	
	NeedCopyCount = (pPS->section_length_hi*256+pPS->section_length_low) + 3 ;	
	/*	section length only	indicat data lenth after this field, so +3 */
	//return NULL;
	assert(NeedCopyCount < 4096);
	CSection * pSec = new CSection;

	if(!pSec || ((pSec->SectionData =  (u8 *)malloc(NeedCopyCount)) == NULL))
	{
		TRACE("Malloc(%d) section data fail\r\n",NeedCopyCount);
		delete pSec;
		assert(0);
		return NULL;
	}
	pSec->StartPktIdx = CurIdx;
		
	//NeedCopyCount = SecBuffLen;
	
	if(pPkt->GetDataStartOffset() + NeedCopyCount < (PktSize))
	{/* this section in one Packet	*/
		memcpy(pSec->SectionData , pPS ,NeedCopyCount);
	}
	else
	{/*	this section transmited in more than one Packet	*/
		u8 * BuffAddr = pSec->SectionData;
		u8 CurCpy;
		memcpy(BuffAddr , pPS , (PktSize - pPkt->GetDataStartOffset()));
		NeedCopyCount -= (PktSize - pPkt->GetDataStartOffset());
		BuffAddr += (PktSize - pPkt->GetDataStartOffset());
		CurIdx ++;
		while((NeedCopyCount > 0)&&(pPkt = GetPacket(CurIdx)))
		{
			if((NeedCopyCount + PACKET_HEAD_SIZE) > (PktSize ))
				CurCpy = (PktSize - PACKET_HEAD_SIZE);
			else
				CurCpy = (u8)NeedCopyCount;
			memcpy(BuffAddr , ((u8 *)(pPkt->GetPktData()))+ PACKET_HEAD_SIZE ,CurCpy);
			BuffAddr += CurCpy;
			NeedCopyCount -= CurCpy;/* may be copy more than we want */
			CurIdx ++;
		}
		CurIdx --;
		if(NeedCopyCount > 0)
		{
			TRACE("section data unintegreted on pkt %d \r\n",CurIdx);
			delete pSec;
			return NULL;
		}
	}
	return pSec;
}

CTable* CESData::BuildTable(u16 TableID)
{
	bool BltRst = false;
	CSection * pSec;
	CTable*	pTblHead = NULL, * pTblTail = NULL;
	CTable * pTblCur= NULL;
	s32		PktIdx = -1;
	
	while((PktIdx+1) < (s32)PacketCount)// current index is not last index
	{
		u8 SectionNumber = 0;
		pTblCur = new CTable(TableID , PID);
		while(1/*SectionNumber <= GetLastSectionNumber()*/)
		{
			if(pSec = BuildSection(PktIdx,TableID, SectionNumber))
			{
				pTblCur->AttachSection(pSec);
				if(pSec->IsLastSection())
				{
					pTblCur->UpDateVersion();
					BltRst = true;
					break;
				}
			}
			else
			{
				BltRst = false;
				delete pTblCur;
				break;
			}
			SectionNumber++;
		}
		if(BltRst)
		{
			if(pTblHead)
			{
				if(pTblTail->IsSame(pTblCur))//in standand stream, table can only be duplicate with last one.
				{
					pTblTail->DuplicateRecord(pTblCur);

					delete pTblCur;
				}
				else
				{
					pTblTail->pNextVer = pTblCur;
					pTblTail = pTblTail->pNextVer;
				}
			}
			else
			{
				pTblHead = pTblTail = pTblCur;
			}
		}
	}
	return pTblHead; /* build successed out	*/
}
/*
	if old packet is not enough than insert one packet.
	if old packet is too much for new section, then stuff unnecessery packets.
*/
bool CESData::AppendSectionToPool(CSection * pSec, CPacketPool & PP )
{
	u32 NeedCopyCount  ;
	CPacket  * pPkt ;
	u8 * PktData ;
	u8 * SecData ;
	u8 CurCpy;
	bool FirstPkt = true;
	CommonSection * pCSS = (CommonSection *)(pSec->GetSectionData());
	

	 SecData = pSec->SectionData;
	 NeedCopyCount  = pSec->GetSectionLength() + 3;

	while((NeedCopyCount > 0)&&(pPkt = PP.NewPacket()))
	{
		/* should init packet data*/
		memcpy(pPkt->GetPktData(), GetPacket(0)->GetPktData(), GetPacket(0)->GetPktHeadSize());
		pPkt->SetAdaptiationControl(0x01);
		pPkt->SetDataStartPointer(0);
		
		if(FirstPkt )
		{
			pPkt->SetUnitStart(true);
		}
		if(FirstPkt == false)
		{
			pPkt->SetUnitStart(false);
		}
		FirstPkt = false;
		
		PktData = (u8 *)(pPkt->GetPktData()) + pPkt->GetDataStartOffset();
		if((NeedCopyCount + pPkt->GetDataStartOffset()) > (pPkt->GetPktSize() ))
			CurCpy = (pPkt->GetPktSize() - pPkt->GetDataStartOffset());
		else
			CurCpy = (u8)NeedCopyCount;
		
		memcpy(PktData , SecData ,CurCpy);
		SecData += CurCpy;
		NeedCopyCount -= CurCpy;/* may be copy more than we want */
	}
	return true;
}


bool CESData::BuildFromTable(CTable * pTbl)
{
/*
	build insert mapping
	
	calcuate Insert count array in every postion.
	
1. calcuate OtherSectionCount, so postion = OtherSectionCount+1;
2. malloc postion array for calcuate how many table insert in every position.
3. calcuate Current table count.
4. init postion array with (Current table count)/postions
5. add first if CurTblLft--;
6. add last if CurTblLft--.
7. add left in middle if CurTblLft
*/
	CTable * pTblTmp = pTbl;
	u32 OtherSecCnt = FindOtherSectionCount(pTbl->GetTableID());
	u32 PostionCnt = OtherSecCnt+1;
	u8 * pPosIstCnt = (u8 *)malloc(PostionCnt);
	u32 CurTblCnt = 0;
	u32	CurTblLft ;
	while(pTblTmp)
	{
		CurTblCnt += pTblTmp->GetDupliCount();
		pTblTmp = pTblTmp->pNextVer;
	}
	memset(pPosIstCnt,CurTblCnt/PostionCnt ,PostionCnt);
	CurTblLft = CurTblCnt%(PostionCnt);
	if(CurTblLft)
	{
		pPosIstCnt[0]++;
		CurTblLft--;
	}
	if(CurTblLft)
	{
		pPosIstCnt[PostionCnt -1 ]++;
		CurTblLft--;
	}
	u32 OldIndex = 0, NewIndex;
	
	while(CurTblLft)
	{
		for(NewIndex = (OldIndex+1); NewIndex < PostionCnt; NewIndex++)
		{
			if(pPosIstCnt[NewIndex] == (CurTblCnt/PostionCnt)+1)
				break;
		}
		if(pPosIstCnt[(OldIndex+NewIndex)/2] == (CurTblCnt/PostionCnt))
		{
			pPosIstCnt[(OldIndex+NewIndex)/2]++;
			CurTblLft--;
		}
		OldIndex = NewIndex;
		
		if(OldIndex >= (PostionCnt - 1))
		{
			OldIndex = 0;
		}
	}
/*
build packet pool accroding to insert mapping for every postion
*/
	CSection * pSec;
	CPacketPool pNewPool;
	u32 CurDupLft = pTbl->GetDupliCount();
	CPacket  * pPkt ;
	CommonSection * pPS; 
	s32  CurIdx = -1;
	u32 OtherSectionCount = 0;
	CPacket * pNewPkt;
	u32 NeedCopyCount ;
	u32 Index;
	for(Index = 0; Index < PostionCnt ; Index++)
	{
		//insert pPosIstCnt[Index] tables, may be cross versions
		while( pPosIstCnt[Index])
		{
			if(CurDupLft--)
			{
				pSec = pTbl->GetSection();
				while(pSec)
				{			
					if(AppendSectionToPool( pSec,  pNewPool ))
					{
					}
					else
					{
						assert(0);
						return false;
					}
					pSec = pSec->pNext;
				}
				pPosIstCnt[Index]--;
			}
			else//this table is not enough
			{
				pTbl = pTbl->pNextVer;
				CurDupLft = pTbl->GetDupliCount();
			}
		}
		
		

		//insert old sections if have, last postion should not have old section
		while(NULL != (pPkt = GetPacket(++CurIdx)))
		{
			if(pPkt->IsUnitStart())
			{
				pPS = pPkt->GetSectionAddress();
				if(pPS->table_id != pTbl->GetTableID())//other table section
				{
					OtherSectionCount++;
					if(OtherSectionCount == (Index+1))
					{
						//find section we want then copy it.
						NeedCopyCount = (pPS->section_length_hi*256+pPS->section_length_low) + 3 ;	
						while(NeedCopyCount)
						{
							pNewPkt = pNewPool.NewPacket();

							if((NeedCopyCount + pPkt->GetDataStartOffset()) > (pPkt->GetPktSize() ))
								NeedCopyCount -= (pPkt->GetPktSize() - pPkt->GetDataStartOffset());
							else
								NeedCopyCount  = 0;
							memcpy(pNewPkt->GetPktData() , pPkt->GetPktData(),pPkt->PacketSize);
						}
						break;
					}
				}
			}
		}

	}
	free(pPosIstCnt);
//	and adjust ContinuityCounter and copy back
	for(u32 i = 0; i < pNewPool.GetPacketCunt() ; i ++)
	{
		pPkt = pNewPool.GetPacket(i);
		pPkt->SetContinuityCounter((u8)(i%16));
	}
	FreeAllPackets();
	CopyFromAnotherPool(pNewPool);
	
	return true; /* build successed out	*/
}

void CESData::SaveIndexInTS()
{
	if(OldPktIdxInTsArry)
		free(OldPktIdxInTsArry);
	
	OldPktIdxInTsArry = (u32 *)malloc(GetPacketCunt() * sizeof(u32));
	for(u32 i = 0 ; i < GetPacketCunt(); i ++)
	{
		OldPktIdxInTsArry[i] = GetPacket(i)->GetPktIdxInTs();
	}
	OldPktIdxCnt = GetPacketCunt();
}

bool CESData::SaveToFile(CString * pFileName)
{
	CString FileName;
	CFile pFile;
	if(pFileName == NULL)
	{
		FileName.Format(".\\ES(%d).es",PID);
	}
	else
	{
		FileName = *pFileName;
	}
	
	if(! IfEsBuildFinish())
	{
		CString * pInfo = new CString;
		pInfo->Format("Save ES(PID = 0x%x) fail for ES is not build finish;\r\n", PID);
		ReturnMessage((WPARAM)pInfo);
		return false;
	}
	if(! pFile.Open((LPCTSTR)FileName,CFile::modeCreate|CFile::modeWrite ))
	{
		CString * pInfo = new CString;
		pInfo->Format("Save ES(PID = 0x%x) fail for open ES file fail;\r\n", PID);
		ReturnMessage((WPARAM)pInfo);
		return false;
	}
	pFile.SeekToBegin();
	for(u32 index = 0;index < PacketCount;index++)
	{
		pFile.Write(GetPacket(index),sizeof(CPacket));
	}
	pFile.Close();
	CString * pInfo = new CString;
	pInfo->Format("Save ES(PID = 0x%x) successed;\r\n", PID);
	ReturnMessage((WPARAM)pInfo);
	return true;
}
bool CESData::LoadFromFile(CString * pFileName)
{
	if(PacketCount)
		FreeAllPackets();
	
	CFile pFile;
	if(! pFile.Open((LPCTSTR)(*pFileName),CFile::modeRead ))
	{
		CString * pInfo = new CString;
		pInfo->Format("Load ES Fail for open file(%s) fail;\r\n",(*pFileName));
		ReturnMessage((WPARAM)pInfo);
		return false;
	}
	u64 FileLenth = pFile.GetLength();
	CPacket * pp;
	while(FileLenth)
	{
		pp = NewPacket();
		pFile.Read(pp,sizeof(CPacket));

		if(PID == INVALID_PID)
			PID = pp->GetPID();
		
		FileLenth -= sizeof(CPacket);
	}
	pFile.Close();
	Integrity = true;

	
	CString * pInfo = new CString;
	pInfo->Format("Load ES(PID = 0x%x) from file(%s) successed;\r\n", PID,(*pFileName));
	ReturnMessage((WPARAM)pInfo);
	return true;
}

/*
this function for debug
*/
void CESData::PrintHeadInfo()
{
	CString * pInfo = new CString;
	pInfo->Format("Trace out ContinuityCounter:\r\n", PID);
	for(u32 index = 0;index < PacketCount;index++)
	{
		pInfo->AppendFormat("Pkt %d(%d) syn byte %x, TransPriority %x, strartIndicator %x,TransErrorIndivator %x,ScramCont %x,AdaptionCont %x,ContinuityCounter is %d \r\n",index, GetPacket(index)->PktIdxInTs,
			GetPacket(index)->GetSynByte(), GetPacket(index)->GetTransportPriority(),
			GetPacket(index)->GetPayLoadUnitStartIndicator(),GetPacket(index)->GetTransportErrorIndivator(),
			GetPacket(index)->GetScramblingControl(),GetPacket(index)->GetAdaptiationControl(),GetPacket(index)->GetContinuityCounter());
	}
	ReturnMessage((WPARAM)pInfo);

}
/*

void CESData::SetParsedPkt(void	* pPktData,u32 PktIdx)
{
	if(ParsedPktData == NULL)
		ParsedPktData = (u8 *)malloc(GetPacketSize());
	
	memcpy(ParsedPktData,pPktData,GetPacketSize());
	ParsedPktIdx = PktIdx;
	ParsedPktOutDataed = false;
}

void CESData::SetParsedPktOutDated()
{
	ParsedPktOutDataed = true;
}

bool CESData::SavePacketToTS(u32	PktIdx)
{
	if(PacketCount > PktIdx)
	{
		return pParentTS->HardWritePacket(GetPacket(PktIdx));
	}
	return false;
}

CContentTree	* CESData::ParsedCurPkt(void)
{
	//if(PktTree)
		//delete PktTree;
	return GetCurPacket()->ParsePacketData(this,CurPktIdx);
}
*/
void 		CESData::ChangePID(u16 NewPid)
{
	if(IfEsBuildFinish())
	{
		for(u32 PktIdx = 0 ; PktIdx < GetPacketCunt(); PktIdx ++)
		{
			GetPacket(PktIdx)->SetPID(NewPid);
		}
		PID = NewPid;
	}
	else
	{
		CString * pInfo = new CString;
		pInfo->Format("ChangePID 0x%x to 0x%x fail for it's not finished filte;", PID,NewPid);
		ReturnMessage((WPARAM)pInfo);
	}
}


