#include "StdAfx.h"
#include "ObjectDB.h"

CTObject::CTObject()
{
	Identify = CTObjectDB::GetFreeIdentify();
	/*add to list*/
	pNext = CTObjectDB::pObjList;
	CTObjectDB::pObjList = this;
}

CTObject::~CTObject()
{
	/* delete from list */
	CTObject * pList;
	pList = CTObjectDB::pObjList;
	if(pList == this)
	{
		CTObjectDB::pObjList = pNext;
	}
	else
	{
		while(pList->pNext != this);
			pList = pList->pNext;
			
		pList->pNext = pNext;
	}
}


u32 CTObjectDB::IdentifyCount = 0;
CTObject * CTObjectDB::pObjList = NULL;

CTObjectDB::CTObjectDB()
{
}

CTObjectDB::~CTObjectDB()
{
}

u32 CTObjectDB::GetFreeIdentify()
{
	return IdentifyCount++;
}

CTObject * CTObjectDB::FindObjectById( u32 id)
{
	CTObject * pList;
	pList = pObjList;
	while(pList && (pList->Identify != id))
		pList = pList->pNext;


	return pList;
}

