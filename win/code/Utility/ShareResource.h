/*
as a tree , root shouldn't have brothers

*/

#pragma once

#include "TypeDefine.h"
#include "afxmt.h"/* for semaphore */


class CShareResource
{
public:
	CShareResource(u8 IntSem = 1);
	~CShareResource();
	
	BOOL Lock1();
	BOOL UnLock1();
	BOOL IsLocked1();
	
	BOOL Lock2();
	BOOL UnLock2();
	BOOL IsLocked2();	
	
	private:
		CSemaphore 	* ptrSemaphore;
		CSingleLock	* pSignalLock1;
		CSingleLock	* pSignalLock2;
};



