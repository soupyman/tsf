#include "StdAfx.h"
#include "include.h"

CESDataBase::CESDataBase(void)
{
}

CESDataBase::~CESDataBase(void)
{
	Free();
}

CESData * CESDataBase::SearchES(u16 	pid )
{
	POSITION pos = GetHeadPosition();
	CESData * pES;
	while(pos)
	{
		pES = (CESData *)(GetNext(pos));
		if(pES->PID == pid)
		{
			return pES;
		}
	}
	return NULL;
}

bool CESDataBase::AddES( CESData * pNewES)
{
	if(SearchES( pNewES->PID) == NULL)
	{
		AddTail(pNewES);
		return true;
	}
	else
		return false;
	
}

bool  CESDataBase::RemoveES(u16 	pid)
{
	POSITION pp, pos = GetHeadPosition();
	CESData * pES;
	while(pos)
	{
		pp = pos;
		pES = (CESData *)(GetNext(pos));
		if(pES->PID == pid)
		{
			delete pES;
			RemoveAt(pp);
			return true;
		}
	}
	return false;
}

void CESDataBase::Free(void)
{
	POSITION pos = GetHeadPosition();
	CESData * pES;
	while(pos)
	{
		pES = (CESData *)(GetNext(pos));
		delete pES;
	}
	RemoveAll();
}
