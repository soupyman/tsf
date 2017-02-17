#include "StdAfx.h"
#include "include.h"

CTableDatabase::CTableDatabase(void)
{
}

CTableDatabase::~CTableDatabase(void)
{
	Free();
}

CTable * CTableDatabase::SearchTable(u16	tableid,u16 	pid )
{
	POSITION pos = GetHeadPosition();
	CTable * pTable;
	while(pos)
	{
		pTable = (CTable *)(GetNext(pos));
		if((pTable->GetTableID() == tableid)&&(pTable->GetPID() == pid))
		{
			return pTable;
		}
	}
	return NULL;
}

CTable * CTableDatabase::SearchNextTable(POSITION & CurPos , u16	tableid)
{
	CTable * pTable;
	while(CurPos)
	{
		pTable = (CTable *)(GetNext(CurPos));
		if(pTable->GetTableID() == tableid)
		{
			return pTable;
		}
	}
	return NULL;
}
bool CTableDatabase::AddTable( CTable * pNewTable)
{
	if(SearchTable(pNewTable->GetTableID(), pNewTable->GetPID()) == NULL)
	{
		AddTail(pNewTable);
	}
	else
	{
		RemoveTable(pNewTable->GetTableID(), pNewTable->GetPID());
		if(!AddTable(pNewTable))
		{
			assert(0);//free old and add new
			return false;
		}
	}
	return true;
	
}

CTable *  CTableDatabase::UpdateTable(CTable * pNewTable)
{
	CTable * pOldTable = SearchTable(pNewTable->GetTableID(), pNewTable->GetPID());
	if(pOldTable )
	{
		/* update here	*/
		pNewTable = pNewTable->Duplicate();
		
		RemoveTable(pOldTable->GetTableID(), pOldTable->GetPID());
		AddTable(pNewTable);
		return pNewTable;
	}
	else
	{
		assert(0);
		return NULL;
	}
}

bool  CTableDatabase::RemoveTable(u16	tableid,u16 	pid)
{
	POSITION pp, pos = GetHeadPosition();
	CTable * pTable;
	while(pos)
	{
		pp = pos;
		pTable = (CTable *)(GetNext(pos));
		if((pTable->GetTableID() == tableid)&&(pTable->GetPID() == pid))
		{
			delete pTable;
			RemoveAt(pp);
			return true;
		}
	}
	return false;
}
/* twm_dbg lots of pb here 
void CTableDatabase::AddTable(WPARAM PID, LPARAM TableID)
{
	if(PID == INVALID_PID)
	{
		switch(TableID)
		{
			case PROGRAM_MAP_INFORMATION:
			{
				CTable * pPAT;
				pPAT = AddTable(PROGRAM_ASSOCIATION_INFORMATION,PAT_PID);
				BuildTable(PAT_PID,PROGRAM_ASSOCIATION_INFORMATION,true);
			}
				break;
			case ENTITLE_MANAGE_MESSAGE:
			{
				CTable * pCAT;
				pCAT = AddTable(CONDITIONAL_ACCESS_INFORMATION,CAT_PID);
				BuildTable(CAT_PID,CONDITIONAL_ACCESS_INFORMATION,true);
			}
				break;
			case ENTITLE_CONTROL_MESSAGE_ODD:
			case ENTITLE_CONTROL_MESSAGE_EVEN:
			{
				CTable * pT;
				pT = AddTable(PROGRAM_ASSOCIATION_INFORMATION,PAT_PID);
				BuildTable(PAT_PID,PROGRAM_ASSOCIATION_INFORMATION,true);
				BuildTable(INVALID_PID,PROGRAM_MAP_INFORMATION,true);
			}
				break;
			default:
				assert(0);
				break;
		}
	}
	else
	{
		AddTable(TableID,PID);
	}
}
*/
void CTableDatabase::Free(void)
{
	POSITION pos = GetHeadPosition();
	CTable * pTable;
	while(pos)
	{
		pTable = (CTable *)(GetNext(pos));
		delete pTable;
	}
	RemoveAll();
}
