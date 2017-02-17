#include "StdAfx.h"

#include "include.h"



CShareResource::CShareResource(u8 IntSem )
{
/*
CSemaphore(
   LONG lInitialCount = 1,
   LONG lMaxCount = 1,
   LPCTSTR pstrName = NULL,
   LPSECURITY_ATTRIBUTES lpsaAttributes = NULL 
);
*/
	ptrSemaphore=new CSemaphore(IntSem,1);

/*
explicit CSingleLock(
   CSyncObject* pObject,
   BOOL bInitialLock = FALSE 
);
*/
	pSignalLock1 = new CSingleLock(ptrSemaphore);
	pSignalLock2 = new CSingleLock(ptrSemaphore);
}

CShareResource::~CShareResource()
{
	delete pSignalLock1;
	delete pSignalLock2;
	delete ptrSemaphore;
}
/*
Calls the Win32 function EnterCriticalSection, which waits until the thread can take ownership of the critical section object contained in the m_sec data member.
Returns S_OK on success, E_OUTOFMEMORY or E_FAIL on failure.
*/
BOOL CShareResource::Lock1()
{
	return pSignalLock1->Lock();
}

BOOL CShareResource::UnLock1()
{
	return pSignalLock1->Unlock();
}

BOOL CShareResource::IsLocked1()
{
	return pSignalLock1->IsLocked();
}

BOOL CShareResource::Lock2()
{
	return pSignalLock2->Lock();
}

BOOL CShareResource::UnLock2()
{
	return pSignalLock2->Unlock();
}

BOOL CShareResource::IsLocked2()
{
	return pSignalLock2->IsLocked();
}

