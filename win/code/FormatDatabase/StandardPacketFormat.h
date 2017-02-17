#pragma once

//#include "SectionFormatDefine.h"
#include "FormatParser.h"
//#include "DVBDefine.h"
//#define PACKETS_NUMBER_TO_TRY_PACKET_SIZE	20
//#define SYN_BYTE	0x47
//#define STEP_SIZE	100
#define PACKET_HEAD_SIZE	4

/*
struct SamePIDPackets
{
u16 Pid;
u32 PacketNum;
u32 PacketCapability;
u32 * PacketArray;
};
*/


class CStandardPacketFormat :public virtual CFormatParser 
{
public:
	CStandardPacketFormat(void);
	~CStandardPacketFormat(void);
	

	CContentTree *  	ParsePacketData(CESData * pES, u32 Indx);
	void 			ParseAdaptionData(CPacket * pPkt ,CContentTree *	 pParent);

};
