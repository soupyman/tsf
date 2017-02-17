#include "StdAfx.h"
#include "include.h"

CPoolNode::CPoolNode(void)
{
	PoolAddr =  new CPacket[POOL_SIZE];
	AllocCount = 0;
	pNext = NULL;
}

CPoolNode::~CPoolNode(void)
{
		delete[] PoolAddr;
}


CPacketPool::CPacketPool(void)
{
	PoolCunt = 0;
	PacketCount = 0;
	pPoolHead = NULL;
	pPoolTail = NULL;
}

CPacketPool::~CPacketPool(void)
{
	FreeAllPackets();
}

void CPacketPool::FreeAllPackets(void)
{
	CPoolNode * 	ppb;
	while(pPoolHead  != NULL)
	{
		ppb = pPoolHead->pNext;
		//delete[] pPoolHead->PoolAddr;
		delete pPoolHead;
		pPoolHead = ppb;
	}
	PoolCunt = 0;
	PacketCount = 0;
	pPoolHead = NULL;
	pPoolTail = NULL;
}

void CPacketPool::CopyFromAnotherPool(CPacketPool & pAp)
{
	CPacket * pPkt;
	for(u32 i=0; i< pAp.GetPacketCunt(); i ++)
	{
		pPkt = NewPacket();
		memcpy(pPkt->GetPktData() , pAp.GetPacket(i)->GetPktData(),pPkt->PacketSize);
	}
}


CPoolNode * CPacketPool::NewPoolNode(void)
{
	CPoolNode * pcurPool;
	pcurPool = new CPoolNode;
	//pcurPool->PoolAddr =  new CPacket[POOL_SIZE];
	//pcurPool->AllocCount = 0;
	//pcurPool->pNext = NULL;
	PoolCunt ++;
	
	if(pPoolTail)
	{
		pPoolTail->pNext = pcurPool;
	}
	else/* first node */
	{
		pPoolHead = pcurPool;
	}
	pPoolTail = pcurPool;
	return pcurPool;
}
	
CPacket * CPacketPool::NewPacket(void)
{
	CPacket * pp;
	if((pPoolTail == NULL)||(pPoolTail->AllocCount >= POOL_SIZE))
		NewPoolNode();

	
	pp =  &(pPoolTail->PoolAddr[pPoolTail->AllocCount]);
	pPoolTail->AllocCount ++ ;
	//pp->SetPktIdx(PacketCount);
	PacketCount ++;
	
	return pp;
}


/*  
	PacketIndex from 0 - N 
*/
CPacket * CPacketPool::GetPacket(u32 PacketIndex)
{
	if(PacketIndex < PacketCount)
	{
		CPoolNode * pcpp = pPoolHead ;
		
		while(PacketIndex >= POOL_SIZE)
		{
			pcpp =  pcpp->pNext;
			PacketIndex -=  POOL_SIZE;
		}
		return  &(pcpp->PoolAddr[PacketIndex]);
	}
	else
		return NULL;
}

u32		CPacketPool::GetPacketCunt(void)
{
	return PacketCount;
}

