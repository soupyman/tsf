#include "StdAfx.h"

#include "include.h"
	
/*bool CClient::IsUsed()
{

	return flag&CLIENT_USED;
}
*/
//CTableDBServer::Self = NULL;

CTableDBServer::CTableDBServer(CESDBServer * src)
{
	Source = src;
	InitTDBServer();
	//Self = this;
	//TestCall();
}
CTableDBServer::~CTableDBServer()
{
	ClearTDBServer();
}
/*
CTableDBServer * CTableDBServer::GetInstance(void)
{
	if(Self == NULL)
		new CTableDBServer;

	assert(Self);
	return Self;
}
*/	
bool CTableDBServer::ClearTDBServer()
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

bool CTableDBServer::InitTDBServer()
{
	Client = NULL;
	DataBase = new CTableDatabase;
	Engine = new CBuilder(this);
	Engine->Run();
	return true;
}

bool CTableDBServer::ClientExist(u32 ClientID , u16 TableID, u16 PID)
{
	CClient * pClient = Client;
	while(pClient )
	{
		if((pClient->ClientID == ClientID)&&
			(pClient->TableID == TableID)&&
			(pClient->PID == PID))
			return true;
		pClient = pClient->pNext;
	}
	return false;
}

bool CTableDBServer::AddClient(u32 ClientID , u16 TableID, u16 PID)
{
	if(ClientExist(ClientID ,  TableID,  PID) == false)
	{
		CClient * pClient = new CClient;
		pClient->ClientID = ClientID;
		pClient->TableID = TableID;
		pClient->PID = PID;
		pClient->pNext = Client;
		Client = pClient;
		
		Engine->AddRequest(PID, TableID);
		
		return true;
	}
	return false;
}

bool CTableDBServer::SubscribeTable(u32 ClientID , u16 TableID, u16 PID)
{
	CTable * pTable = NULL;
	u16 * SubTblAry = NULL;

	if(PID == INVALID_PID)
	{
		switch(TableID)
		{
			case ENTITLE_CONTROL_MESSAGE_ODD:
			case ENTITLE_CONTROL_MESSAGE_EVEN:
			{
				pTable = DataBase->SearchTable(PROGRAM_MAP_INFORMATION, INVALID_PID);
			}
				break;
			case PROGRAM_MAP_INFORMATION:
			{
				pTable = DataBase->SearchTable(PROGRAM_ASSOCIATION_INFORMATION, PAT_PID);
			}
				break;
			case ENTITLE_MANAGE_MESSAGE:
			{
				pTable = DataBase->SearchTable(CONDITIONAL_ACCESS_INFORMATION, CAT_PID);
			}
				break;
			default:
				assert(0);
				break;
		}
		if(pTable && (SubTblAry = pTable->AnalyseSubTablePIDArray()) && SubTblAry[0])
		{
			for(int i = 1; i <= SubTblAry[0]; i++)
				SubscribeTable(ClientID, TableID, SubTblAry[i]);
		}
		if(SubTblAry)
			free( SubTblAry);
		
		return true;
	}
	if(AddClient( ClientID ,  TableID,  PID))
	{
		pTable = DataBase->SearchTable(TableID, PID);
		if(pTable)
		{
			/* message to Client with this table	*/
			PostThreadMessage(ClientID,MSG_UI_TABLE ,(WPARAM)pTable->Duplicate(),0);
		}
		else
		{
			/* subscribe ES from ES database */

			Source->SubscribeES(Engine->pThread->m_nThreadID , PID);
		}
	}
	return true;
}

bool CTableDBServer::UnSubscribeTable(u32 ClientID , u16 TableID, u16 PID)
{
	CClient * pClient_b = NULL, * pClient = Client;
	for(;pClient; pClient = pClient->pNext)
	{
		if((pClient->ClientID == ClientID)&&
			(pClient->TableID == TableID)&&
			(pClient->PID == PID))
		{
			if(pClient_b)
				pClient_b->pNext = pClient->pNext;
			else
				Client = Client->pNext;

			delete pClient;
			//delete request from Engine
			break;
		}
		pClient_b = pClient;
	}
	/*for(pClient = Client;pClient; pClient = pClient->pNext)
	{
		if((pClient->TableID == TableID)&&
			(pClient->PID == PID))
		{
			break;
		}
	}
	if(NULL == pClient)//this table is no need any more
	{
		DataBase->RemoveTable(TableID, PID);
	}*/
	
	return true;
}


bool	CTableDBServer::RemoveTable(CTable * TblToRmv)
{
	DataBase->RemoveTable(TblToRmv->GetTableID(), TblToRmv->GetPID());
	
	//Engine->PostCommand(UINT msg, WPARAM wP, LPARAM lP);
	return true;
}

bool	CTableDBServer::UpdateTable(CTable * TblToUdt)
{
	if(TblToUdt->GetValidSectionCount())
		TblToUdt = DataBase->UpdateTable(TblToUdt);
	else
		DataBase->RemoveTable(TblToUdt->GetTableID(), TblToUdt->GetPID());
	
	Engine->PostCommand(MSG_BLD_SAVE_TABLE, (WPARAM)TblToUdt,
		(LPARAM)(Source->GetEs(TblToUdt->GetPID())));
	return true;
}

bool CTableDBServer::UpdatedNotify( u16 TableID , u16 PID)
{
	
	CClient * pClient = Client;
	CTable * pTbl = DataBase->SearchTable(TableID, PID);
	while(pClient && pTbl)
	{
		if((pClient->PID == PID)&&(pClient->TableID == TableID))
		{
			if(pClient->ClientID == UI_CLIENT_ID)
				Engine->MessageToUI(MSG_UI_TABLE ,(WPARAM)pTbl->Duplicate(),0);
			else
				PostThreadMessage(pClient->ClientID,MSG_UI_TABLE ,(WPARAM)pTbl->Duplicate(),0);
		}
		pClient = pClient->pNext;
	}

	return true;
}
/*
bool	CTableDBServer::SaveTableToFile( u16 TableID , u16 PID)
{
	Engine->PostCommand(MSG_CMD_SAVE_TABLE_AS_FILE, (WPARAM)PID,(LPARAM)TableID);
	return true;
}
*/

