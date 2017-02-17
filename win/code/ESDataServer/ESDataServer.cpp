#include "StdAfx.h"
#include "include.h"


CESDBServer::CESDBServer(CTSFile * source)
{
	InitESDBServer(source);
}
CESDBServer::~CESDBServer()
{
	ClearESDBServer();
}

bool CESDBServer::InitESDBServer(CTSFile * source)
{
	Client = NULL;
	DataBase = new CESDataBase;
	Engine = new CDemux(source,this);
	//Engine->Init(source,this);
	Engine->Run();
	//Engine->PostCommand(MSG_CMD_INIT_DEMUX_TS ,(WPARAM)source,(LPARAM)this)	;
	return true;
}

bool CESDBServer::ClearESDBServer()
{
	
	CClient * pClient ;
	while(pClient = Client)
	{
		Client = Client->pNext;
		delete pClient;
	}
	
	if(DataBase)
		delete DataBase;
	if(Engine)
		delete Engine;
	
	return true;
}

bool CESDBServer::ClientExist(u32 ClientID, u16 PID)
{
	CClient * pClient = Client;
	while(pClient )
	{
		if((pClient->ClientID == ClientID)&&
			(pClient->PID == PID))
			return true;
		pClient = pClient->pNext;
	}
	return false;
}

bool CESDBServer::AddClient(u32 ClientID , u16 PID)
{
	if(ClientExist(ClientID ,   PID) == false)
	{
		CClient * pClient = new CClient;
		pClient->ClientID = ClientID;
		pClient->PID = PID;
		pClient->pNext = Client;
		Client = pClient;
		
		if(DataBase->SearchES( PID) == NULL)
		{
			CESData * pNewES = new CESData(PID);
			DataBase->AddES(pNewES);
			Engine->AddDemuxFilter(pNewES);
		}
		return true;
	}
	return false;
}
/*
bool CESDBServer::RemoveClient(u32 ClientID , u16 PID)
{
	if(ClientExist(ClientID ,   PID) == true)
	{
		bool del = true;
		CClient * pClient = Client;
		CClient * pRemClient  = NULL;
		while(pClient )
		{
			if((pClient->ClientID != ClientID)&&	(pClient->PID == PID))
				del = false;
			
			if((pClient->ClientID == ClientID)&&	(pClient->PID == PID))
				pRemClient = pClient;
			
			pClient = pClient->pNext;
		}
		if(del)
			DataBase->RemoveES(PID);

		//if(pRemClient)
			//delete pRemClient;
		
		return true;
	}
	return false;
}
*/
CESData * CESDBServer::GetEs(u16 PID)
{
	return DataBase->SearchES( PID);
}

bool CESDBServer::SubscribeES(u32 ClientID , u16 PID)
{
	AddClient( ClientID ,  PID);

	CESData * pES = DataBase->SearchES( PID);

	if((pES )&&(pES->GetPacketCunt() ))
	{
		/* message to Client with this ES	*/
		PostThreadMessage(ClientID,MSG_BLD_ES_DATA_VALID ,(WPARAM)pES,0);
	}

	return true;
}

bool CESDBServer::UnSubscribeES(u32 ClientID , u16 PID)
{
	CClient * pClient_b = NULL, * pClient = Client;
	for(;pClient; pClient = pClient->pNext)
	{
		if((pClient->ClientID == ClientID)&&
			(pClient->PID == PID))
		{
			if(pClient_b)
				pClient_b->pNext = pClient->pNext;
			else
				Client = Client->pNext;

			delete pClient;
			
			break;
		}
		pClient_b = pClient;
	}
	for(pClient = Client;pClient; pClient = pClient->pNext)
	{
		if(pClient->PID == PID)
		{
			break;
		}
	}
	if(NULL == pClient)/* this ES is no need anymore */
	{
		DataBase->RemoveES(PID);
	}
	return true;
}


bool CESDBServer::UpdateES(u16 PID /*, u8 * data , u16 len*/)
{
	/*
	update
	*/
	CESData * pNewES = DataBase->SearchES(PID);
	Engine->AddRemuxFilter(pNewES);
	//ESUpdatedNotify(PID);
	return true;
}

bool CESDBServer::ESUpdatedNotify( u16 PID)
{
	
	CClient * pClient = Client;
	CESData * pES = DataBase->SearchES( PID);
	while(pClient && pES)
	{
		if(pClient->PID == PID)
		{
			PostThreadMessage(pClient->ClientID,MSG_BLD_ES_DATA_VALID ,(WPARAM)pES,0);
		}
		pClient = pClient->pNext;
	}

	return true;
}

void CESDBServer::SavePart(u32 PktCnt)
{
	Engine->PostCommand(MSG_DMX_SAVE_PART_TO_FILE, PktCnt);
}


