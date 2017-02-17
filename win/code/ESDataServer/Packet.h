#pragma once

//#include "SectionFormatDefine.h"
#include "DVBDefine.h"
//#include "DVBDefine.h"
//#define PACKETS_NUMBER_TO_TRY_PACKET_SIZE	20
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


class CPacket :public CDVBSpace 
{
public:

	u8				Data[204];/* as much as enough memery */
	//PacketData	* pPacketData;
	
	u8				PacketSize;
	bool				Edit;
	u32				PktIdxInTs;	/* index in total TS */
	
	//CPacket *	pNext;
public:
	CPacket(void * PktDat =NULL, u8	PktLen = 188, u32 index = 0);
	~CPacket(void);
	u8		GetPktSize(){return PacketSize;};
	void 	SetPktData(void 	* pPD);
	void	* 	GetPktData(void);
	void 	SetPktIdxInTs(u32	NewIdx);

	bool 	IsValid(void);
	bool 	SameWith(CPacket & CmpPkt, u8 PktLen);
	bool		IsUnitStart(void);
	bool		SetUnitStart(bool IsStart);

	u8 		GetSynByte();
	u8		GetTransportPriority();
	u8 		GetPayLoadUnitStartIndicator();
	u8 		GetTransportErrorIndivator();
	u8 		GetContinuityCounter();
    void 		SetContinuityCounter(u8 		CC);

	u8		GetAdaptiationControl(void);
        void    SetAdaptiationControl(u8 AFC);

	u8		GetScramblingControl();
	u32 		GetPktIdxInTs();
	void 	SetPID(u16 NPID);
	u16 		GetPID(void);
	u8		GetDataStartPointer();
    void SetDataStartPointer(u8 DSP);

	u8 		GetDataStartOffset(void);
	u8		GetPktHeadSize();
	u8 		GetAdaptationFieldLength();

	CommonSection *	GetSectionAddress(void);
        u8 *    GetPesAddress(void);
	bool 	GetPCR(u32 & PCR_Base, u32 &PCR_ext);

};
