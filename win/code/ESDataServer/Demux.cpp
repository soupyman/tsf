#include "StdAfx.h"
#include "include.h"
//#include "Demux.h"

//CDemux   * CDemux::CDemux_at = NULL;

CDemux::CDemux(CTSFile 	* src , CESDBServer   *cli)
{
	assert(src && cli);
	SourceTS = src;
	ClientAgent = cli;

	pDmxFilter = NULL;
	pRmxFilter = NULL;
	//CDemux_at = this;

	//m_event = new CEvent(false,true);
	
	//m_event->ResetEvent();
}

CDemux::~CDemux(void)
{
	//m_event->ResetEvent();
	//delete m_event;
	CDemuxFilter * pFtb;
	while(pDmxFilter)
	{
		pFtb = pDmxFilter;
		pDmxFilter = pDmxFilter->pNext;
		delete pFtb;
	}
	CRemuxFilter * pDftb;
	while(pRmxFilter)
	{
		pDftb = pRmxFilter;
		pRmxFilter = pRmxFilter->pNext;
		delete pDftb;
	}
	//CDemux_at = NULL;
	SourceTS = NULL;
	ClientAgent = NULL;
}

bool CDemux::AddDemuxFilter(CESData * pES)
{
	CDemuxFilter * pFtb = pDmxFilter;
	if(pES == NULL)
	{
		InformationToUI("AddDemuxFilter(NULL) fail.\r\n");
		return false;
	}
	/*if(pES->Integrity)
	{
		InformationToUI("you should not filter a ES again when it's build finished.\r\n");
		return false;
	}*/
	while(pFtb)
	{
		if(pFtb->pES == pES)
		{
			InformationToUI("you should not add a filter which is already in the filter chain.\r\n");
			return false;
		}
		
		pFtb = pFtb->pNext;
	}
	pFtb = new CDemuxFilter;
	pFtb->PID = pES->PID;
	pFtb->pES = pES;
	pFtb->DmxPktStart = -1;
	pFtb->LastNotifyCount = 0;
	pFtb->pNext = pDmxFilter;
	
	pDmxFilter = pFtb;

	//m_event->SetEvent() ;
	//if(! m_syn.IsLocked1())
		//m_syn.Lock1();
	PostCommand(MSG_DMX_ADD_DEMUX_FILTER);
	return true;
}
/*
return next filter
*/
bool CDemux::DeleteDmxFilterForFinish(CDemuxFilter * pFt)
{
	CDemuxFilter * pFtf , * pFtb ; //, * pFtbb;
	pFtb = pFt;
	//pFtbb = pFtb->pNext;
	
	//pFt->pES->Integrity = true;
	ClientAgent->ESUpdatedNotify(pFt->PID);
	if(pFt == pDmxFilter)
		pDmxFilter = pDmxFilter->pNext;
	else
	{
		pFtf = pDmxFilter;
		while((pFtf)&&(pFtf->pNext != pFt))
			pFtf = pFtf->pNext;
		if(pFtf)
			pFtf->pNext = pFt->pNext;
	}
	delete pFtb;
	return true;
}
bool CDemux::AddRemuxFilter(CESData * pES )
{
	CRemuxFilter * pDftb = pRmxFilter;
	if(pES == NULL)
	{
		InformationToUI("AddRemuxFilter(NULL) fail.\r\n");
		return false;
	}
	/*if(pES->Integrity == false)
	{
		InformationToUI("you can not remux a ES when it's not finished filter.\r\n");
		return false;
	}*/
	while(pDftb)
	{
		if(pDftb->pES == pES)
		{
			InformationToUI("you should not add a defilter which is already in the defilter chain.\r\n");
			return false;
		}
		pDftb = pDftb->pNext;
	}
	pDftb = new CRemuxFilter;
	pDftb->PID = pES->PID;
	pDftb->pES = pES;
	//pDftb->dlt = dlt;
	//pDftb->RemuxPktStart = -1;
	pDftb->pNext = pRmxFilter;
	pRmxFilter = pDftb;
	//m_event->SetEvent() ;
	//m_syn.Lock1();
	PostCommand(MSG_DMX_ADD_REMUX_FILTER);
	return true;
}
/*
return next filter
*/
bool CDemux::DeleteRmxFilterForFinish(CRemuxFilter * pDft)
{
	CRemuxFilter * pDftf;

	if(pDft == pRmxFilter)
		pRmxFilter = pRmxFilter->pNext;
	else
	{
		pDftf = pRmxFilter;
		while((pDftf)&&(pDftf->pNext != pDft))
			pDftf = pDftf->pNext;
		if(pDftf)
			pDftf->pNext = pDft->pNext;
	}
	delete pDft;
	return true;
}
/*
bool	CDemux::Init(CTSFile 	* src , CESDBServer   *cli)
{
	
	assert(src && cli);
	SourceTS = src;
	ClientAgent = cli;
	return true;
}
*/
UINT CDemux::MainLoop()
{
	//UINT ret = 0;
	MessageToUI(MSG_UI_DEMUX_THREAD_CREATE_SUCCESS);

	 while(GetMessage(&msg,   NULL,   0,   0))   
	{
		MessageToUI(MSG_UI_CURRENT_ACTION_PROCESS,0);
		//WaitForSingleObject(m_event->m_hObject, INFINITE);//return WAIT_TIMEOUT or WAIT_OBJECT_0
		switch(msg.message)
		{
			case MSG_DMX_ADD_DEMUX_FILTER:
			{
				RunDemux();
			}
			break;
			case MSG_DMX_ADD_REMUX_FILTER:
			{
				RunRemux();
			}
				break;
			case MSG_DMX_SAVE_PART_TO_FILE:
			{
				SavePart((u32)msg.wParam);
			}
			break;
			default:
				assert(0);
				break;
		//m_event->ResetEvent();
		}
		MessageToUI(MSG_UI_CURRENT_ACTION_PROCESS,100);
	}
	 return 0;
}
void CDemux::RunDemux(void)
{
	u32 PacketIndex = 0;
	u32 StuffPacketCount = 0;
	u16  pPktID;
	u32	Pctg = 101;
	CDemuxFilter	* pFt , *pFtTemp;
	bool	ReadOut ;
	while(pDmxFilter)
	{
		pPktID = SourceTS->GetPacketPID(PacketIndex);

		if(pPktID == STUFF_PID)
			StuffPacketCount++;
		
		pFt = pDmxFilter;
		ReadOut = false;
		while((pFt)&&(ReadOut == false))
		{
			if(pPktID == pFt->PID)
			{//add this packet to pDmxFilter->pES
				SourceTS->ReadPacket(pFt->pES->NewPacket(),PacketIndex);
				///if((pFt->LastNotifyCount == 0) || (pFt->pES->GetPacketCunt() == pFt->LastNotifyCount * 10 ))
				{
					//notify clients
					//ClientAgent->ESUpdatedNotify(pFt->PID);
					//pFt->LastNotifyCount  = pFt->pES->GetPacketCunt() ;
				}
				ReadOut = true;
			}
			
			if(pFt->DmxPktStart == PacketIndex)
			{// if finish , delete this filter
				MessageToUI(MSG_UI_ES_ATTRIBUTE,pFt->pES->PID,pFt->pES->GetPacketCunt());
				pFtTemp = pFt;
				pFt = pFt->pNext;
				DeleteDmxFilterForFinish(pFtTemp);
			}
			else
			{
				if(pFt->DmxPktStart < 0)
					pFt->DmxPktStart = PacketIndex;
				
				pFt = pFt->pNext;
			}
		}
		
		PacketIndex++;
		
		if((PacketIndex * 100)/SourceTS->m_nTotalPacketNumber  !=  Pctg)
		{
			Pctg = (PacketIndex * 100)/SourceTS->m_nTotalPacketNumber;
			MessageToUI(MSG_UI_CURRENT_ACTION_PROCESS,  Pctg);
		}
		if(PacketIndex >= SourceTS->m_nTotalPacketNumber)
		{
			InformationToUI("There are %d(%d%%) stuff packets in the TS file.", StuffPacketCount,
				(StuffPacketCount*100)/SourceTS->m_nTotalPacketNumber);
			StuffPacketCount = 0;
			PacketIndex = 0;
		}
	}
	
}
void CDemux::RunRemux(void)
{
	u32 PacketIndex ;
	u32	Pctg ;
	u32	EsPktCnt;
	CRemuxFilter	* pDft;
	CPacket * pPkt;
	while(pRmxFilter)
	{
		Pctg = 101;
		pDft = pRmxFilter;
		PacketIndex = 0;
		EsPktCnt = (pDft->pES->OldPktIdxCnt);
		/*delete old packet to stuff */
		while(EsPktCnt--)
		{
			SourceTS->SetPacketPID(pDft->pES->OldPktIdxInTsArry[EsPktCnt], STUFF_PID);
		}
		free(pDft->pES->OldPktIdxInTsArry);
		pDft->pES->OldPktIdxInTsArry = NULL;
		pDft->pES->OldPktIdxCnt = 0;
		EsPktCnt = (pDft->pES->GetPacketCunt());
		/*insert new packets */
		while(PacketIndex < EsPktCnt)
		{
			pPkt = pDft->pES->GetPacket(PacketIndex);
			/*init packet index*/
			pPkt->PktIdxInTs = PacketIndex *( (SourceTS->m_nTotalPacketNumber)/EsPktCnt) ;
			SourceTS->SoftWritePacket(pPkt);

			PacketIndex++;
			
			if((PacketIndex * 100)/EsPktCnt  !=  Pctg)
			{
				Pctg = (PacketIndex * 100)/EsPktCnt;
				MessageToUI(MSG_UI_CURRENT_ACTION_PROCESS,Pctg);
			}
		}
		
		InformationToUI("Save ES(PID = 0x%d)to TS file successed.",pDft->PID);
		DeleteRmxFilterForFinish(pDft);
	}
	
}


void CDemux::SavePart(u32 Count)
{
	 u32 PacketIndex = 0;
	 u32 Pctg = 101;
	 CPacket PktBuff;
	 CFile PartFile;
	 CString NewName,Addtion;
	 
	 if(Count > SourceTS->m_nTotalPacketNumber)
		  Count = SourceTS->m_nTotalPacketNumber;
	 
	 NewName = SourceTS->m_FileName;
	 Addtion.Format("_%d_packets",Count);
	 NewName.Insert( NewName.ReverseFind('.'),Addtion);
	 
	 PartFile.Open(NewName,CFile::modeReadWrite|CFile::modeCreate);
	 
	 while(PacketIndex < Count)
	 {
		  SourceTS->ReadPacket(&PktBuff,PacketIndex);
		  PartFile.Write(PktBuff.GetPktData(), PktBuff.GetPktSize());
		  if((PacketIndex * 100)/Count  !=  Pctg)
		  {
			   Pctg = (PacketIndex * 100)/Count;
			   MessageToUI(MSG_UI_CURRENT_ACTION_PROCESS,  Pctg);
		  }
		  PacketIndex++;
	 }
	 PartFile.Close();
	 InformationToUI("Save %d packets to file %s.",Count,NewName);
}


