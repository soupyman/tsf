#include "StdAfx.h"
#include "include.h"

CBuilder::CBuilder(CTableDBServer * 	agent)
{
	pReq = NULL;
	ClientAgent = agent;
}

CBuilder::~CBuilder(void)
{	
	CRequest		* pRequest;
	while(pRequest = pReq)
	{
		pReq = pReq->pNext;
		delete pRequest;
	}
}

UINT CBuilder::MainLoop()
{
	MessageToUI(MSG_UI_BUILDER_THREAD_CREATE_SUCCESS);
	//InformationToUI("Builder thread runing.\r\n");
	  //MSG   msg;   
	 /*
	  The GetMessage function retrieves a message from the calling thread's message queue. 
	  The function dispatches incoming sent messages until a posted message is available for retrieval. 
	  Unlike GetMessage, the PeekMessage function does not wait for a message to be posted before returning.
	  BOOL GetMessage(          LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax
);
	 */
	  while(GetMessage(&msg,   NULL,   0,   0))   
	  {   
	  	//u32 cmd = (msg.message - MSG_BASE);
	  	  //InformationToUI("Get Command (0x%x).", cmd);
		  MessageToUI(MSG_UI_CURRENT_ACTION_START);
	          switch(msg.message)   
	          {   
			  case MSG_BLD_ES_DATA_VALID:
			  	{
					BuildTable((CESData * )msg.wParam);
			  	}
				break;
			  case MSG_BLD_SAVE_TABLE:
		  		{
					SaveTable((CTable * )msg.wParam,(CESData * )msg.lParam);
			  	}
			  	break;
			 /* case MSG_CMD_SAVE_TABLE_AS_FILE:
		  		{
				  	CTable * pTable = ClientAgent->DataBase->SearchTable((u16)msg.lParam,(u16)msg.wParam);
					if(pTable)
					{
						//if(pTable->SaveToFile())
							InformationToUI("Save Table(TableID = 0x%x;PID = 0x%x) successed;\r\n", pTable->GetTableID(),pTable->GetPID());
						//else
							InformationToUI("Save Table(TableID = 0x%x;PID = 0x%x) fail for open ES file fail;\r\n", pTable->GetTableID(),pTable->GetPID());
					}
			  	}
			  	break;
			  	*/
	          	default:   
			  	{
			          	InformationToUI("Unrecogenise command message\r\n");
			          	assert(0);
	          		}
				break;   
          	}   
		MessageToUI(MSG_UI_CURRENT_ACTION_FINISHED);
	}    
	return 0;//ANALYSE_NO_ERROR;
}



void CBuilder::AddRequest(u16 PID, u16 TableID)
{
	CRequest		* pRequest = pReq;
	while(pRequest)
	{
		if((pRequest->PID == PID)&&(pRequest->TableID == TableID))
			return;

		pRequest = pRequest->pNext;
	}
	pRequest = new CRequest;
	pRequest->PID = PID;
	pRequest->TableID = TableID;
	pRequest->pNext = pReq;
	pReq = pRequest;
}

void CBuilder::BuildTable(CESData *  pES)
{
	CRequest			* pRequest = pReq;
	CTable * pCTable;
	while(pRequest)
	{
		if(pES->PID == pRequest->PID)
		{
			if(pCTable = pES->BuildTable(pRequest->TableID))
			{
				ClientAgent->DataBase->AddTable(pCTable);

				ClientAgent->UpdatedNotify(pRequest->TableID, pRequest->PID);
			}
		}
		pRequest = pRequest->pNext;
	}
	//pES->PrintHeadInfo();
}
void CBuilder::SaveTable(CTable * pTable, CESData * pES)
{
	if(pTable && pES)
	{
		pES->SaveIndexInTS();
		if(pES->BuildFromTable(pTable))
		{
			ClientAgent->Source->UpdateES(pTable->GetPID());
			InformationToUI("Save Table(TableID = 0x%x;PID = 0x%x) to ES success ;", 
			pTable->GetTableID(),pTable->GetPID());
		}
		else
			InformationToUI("Save Table(TableID = 0x%x;PID = 0x%x) to ES fail ;",
			pTable->GetTableID(),pTable->GetPID());
	}

}

/*
析得出PCR_Base 和 PCR_ext。
把PCR_Base*300+PCR_ext。就可以得到一个PCR的值。
那么再找紧跟其后的一个PCR的值，方法同上。
当得到2个PCR的值 姑且定义为 PCR_ONE,PCR_TWO.
根据协议中的公式 I-2-5计算出rate；
rate = （2个PCR相隔的包的个数*188*8 *27000000）/（PCR_TWO - PCR_ONE）
*/

void CBuilder::CalcBitRate(CESData * 	pEs)
{
	//CESData * 	pEs = pTSData->FindESDataByPID((u16)PCR_PID);
	u32 CurPktIdx , CurPktIdxBk = pEs->GetCurPktIdx();
	CPacket * pPkt;
	u32 		PCRBase , PCRExten, PCRBase_b , PCRExten_b;
	u8		PCRCount = 0;
	u32		PktIdxInTs1, BitRate =0;
		
	for(CurPktIdx = 0;PCRCount < 2 && CurPktIdx < pEs->GetPacketCunt(); CurPktIdx++)
	{
		pEs->SetCurPktIdx(CurPktIdx);
		pPkt = pEs->GetCurPacket();
		if(pPkt->GetPCR(PCRBase, PCRExten))
		{
			if(PCRCount == 0)
			{
				PCRBase_b = PCRBase;
				PCRExten_b = PCRExten;
				PktIdxInTs1 = pPkt->PktIdxInTs;
			}
			else
			{
				BitRate = (pPkt->PktIdxInTs -PktIdxInTs1)*188*8*27000000/( (PCRBase - PCRBase_b)*300 + (PCRExten_b - PCRExten));
			}
			PCRCount++;
		}
	}
	if(BitRate != 0)
		InformationToUI("The BitRate of this TS is 0x%x .",BitRate);
	else
		InformationToUI("Can't calcuate BitRate of this TS by ES(0x%x).", pEs->PID);
}

/*
void CBuilder::WriteBackOverSelf(WriteBackParam wbp, LPARAM lpData)
{
	switch(wbp)
	{
		case Table:
		{
			CTable * pTP = (CTable *)lpData;
			pTP->WriteBackToTS();
		}
			break;
		case Packet:
		{
			CESData * pES = (CESData *)lpData;
			//pES->SaveParsedPkt(false);
		}
			break;
		case AllData:
		{
			POSITION pos = pTableList->GetHeadPosition();
			CTable * pTP;
			while(pos)
			{
				pTP = (CTable *)(pTableList->GetNext(pos));
				pTP->WriteBackToTS();
			}
			//pTSData->SaveES(false);
		}
			break;
	}
}

void CBuilder::WriteBackOverAll(WriteBackParam wbp, LPARAM lpData)
{
	switch(wbp)
	{
		case Table:
		{
			CTable * pTP = (CTable *)lpData;
			pTP->WriteBackToTS();
		}
			break;
		case Packet:
		{
			CESData * pES = (CESData *)lpData;
			//pES->SaveParsedPkt(true);
		}
			break;
		case AllData:
		{
			POSITION pos = pTableList->GetHeadPosition();
			CTable * pTP;
			while(pos)
			{
				pTP = (CTable *)(pTableList->GetNext(pos));
				pTP->WriteBackToTS();
			}
			//pTSData->SaveES(true);
		}
			break;
	}
}
*/
