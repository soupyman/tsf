#pragma once
#include "typedefine.h"

#define 	POOL_SIZE	2000
class CPoolNode
{
public:
	CPoolNode(void);
	~CPoolNode(void);
public:
	CPacket * 	PoolAddr;
	u32			AllocCount;
	CPoolNode * pNext;
};


class CPacketPool
{
private:
	u32					PoolCunt;
	CPoolNode * 		pPoolHead;		
	CPoolNode * 		pPoolTail;	

protected:
	u32 				PacketCount;

public:
	CPacketPool(void);
	~CPacketPool(void);
	
	CPacket * 	NewPacket(void);

	CPacket * 	GetPacket(u32 PacketIndex);
	u32			GetPacketCunt(void);
	void 		FreeAllPackets(void);
    void    CopyFromAnotherPool(CPacketPool & pAp);

private:
	CPoolNode * NewPoolNode(void);

};
