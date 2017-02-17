#include "StdAfx.h"
#include "include.h"

CPacket::CPacket(void * PktDat, u8	PktLen, u32 index)
{
	assert(PktLen);
	if(PktDat)
	{
		memcpy(Data, PktDat,PktLen);
	}
	PacketSize = PktLen;
	PktIdxInTs = index;
	//pNext = NULL;
	Edit = false;
}
	
CPacket::~CPacket(void)
{
}

void CPacket::SetPktIdxInTs(u32	NewIdx)
{
	PktIdxInTs = NewIdx;
}

void CPacket::SetPktData(void	* pPD)
{
	if((pPD)&&(memcmp(Data, pPD,PacketSize)))
	{
		memcpy(Data, pPD,PacketSize);
		Edit = true;
	}
}

void	* CPacket::GetPktData(void)
{
	return (void *)Data;
}

bool CPacket::IsValid()/*	this should be delete 	*/
{
	if(Data	== NULL)
		return false;
	else 
		return true;
}

bool CPacket::SameWith(CPacket & CmpPkt, u8 PktLen)
{
	if(IsValid()&& CmpPkt.IsValid())
	{
		if(0 == memcmp(Data , CmpPkt.GetPktData() ,PktLen))
			return true;
	}
	return false;
}

void CPacket::SetPID(u16 NPID)
{
	PacketData * pPacketData = (PacketData	*)Data;
	pPacketData->PID_low = NPID & 0xFF ;
	pPacketData->PID_hi = (NPID>>8) & 0x1F;
}

u16 CPacket::GetPID()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return (pPacketData->PID_hi * 256 + pPacketData->PID_low);
}

u32 CPacket::GetPktIdxInTs()
{
	return PktIdxInTs;
}

bool	CPacket::IsUnitStart()
{
	PacketData * pPacketData = (PacketData	*)Data;
	if((pPacketData->payload_unit_start_indicator)&&(GetAdaptiationControl() & 0x1))
		return true ;
	else 
		return false;
}

bool	CPacket::SetUnitStart(bool IsStart)
{
	PacketData * pPacketData = (PacketData	*)Data;
	
	pPacketData->payload_unit_start_indicator = IsStart;
	
	if(IsUnitStart() == IsStart)
		return true ;
	else 
		return false;
}

/*
*	if(adaptiation_field_control == 10 || adaptiation_field_control==11)
*		adaptation_field()
*	if(adaptiation_field_control == 01 )
*	{
*		for(i=0;i<N;i++)
*			data_byte		8 		bslbf
*	}
*/
u8	CPacket::GetAdaptiationControl()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->adaptiation_field_control ;
}

void CPacket::SetAdaptiationControl(u8 AFC)
{
	PacketData * pPacketData = (PacketData	*)Data;
	pPacketData->adaptiation_field_control = AFC ;
}

u8	CPacket::GetScramblingControl()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->transport_scrambling_control ;
}

u8	CPacket::GetDataStartPointer()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->DataStartPointer ;
}

void CPacket::SetDataStartPointer(u8 DSP)
{
	PacketData * pPacketData = (PacketData	*)Data;
	pPacketData->DataStartPointer = DSP;
}

u8	CPacket::GetPktHeadSize()
{
	return PACKET_HEAD_SIZE ;
}

u8 CPacket::GetDataStartOffset()
{
	PacketData * pPacketData = (PacketData	*)Data;
	if(IsUnitStart()||(GetAdaptiationControl() & 0x2))
		return ( PACKET_HEAD_SIZE + 1 + pPacketData->DataStartPointer);
	else
		return (PACKET_HEAD_SIZE );
}

u8 CPacket::GetAdaptationFieldLength()
{
	PacketData * pPacketData = (PacketData	*)Data;
	if((pPacketData->adaptiation_field_control) & 0x2)
		return pPacketData->DataStartPointer;
	
	return 0;
}

CDVBSpace::CommonSection * CPacket::GetSectionAddress()
{
	return (CommonSection * ) (Data + GetDataStartOffset());
}
u8 * CPacket::GetPesAddress()
{
	PacketData * pPacketData = (PacketData	*)Data;

	if(GetAdaptiationControl() & 0x2)
		return Data + PACKET_HEAD_SIZE + 1 + pPacketData->DataStartPointer;
	else
		return Data + PACKET_HEAD_SIZE;
}

u8 		CPacket::GetSynByte()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->syn_byte;
}
u8		CPacket::GetTransportPriority()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->transport_priority;
}
u8 		CPacket::GetPayLoadUnitStartIndicator()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->payload_unit_start_indicator;
}
u8 		CPacket::GetTransportErrorIndivator()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->transport_error_indicator;
}
u8 		CPacket::GetContinuityCounter()
{
	PacketData * pPacketData = (PacketData	*)Data;
	return pPacketData->continuity_counter;
}
void 		CPacket::SetContinuityCounter(u8 		CC)
{
	PacketData * pPacketData = (PacketData	*)Data;
	pPacketData->continuity_counter = CC;
}


/*
PCR_Base  定义了41个bit
PCR_ext定义了9个bit
但是PCR 是27兆时钟300分频的一个取样。
而PCR = PCR_Base*300+PCR_ext;
所以应该不会超过u32

*/

bool CPacket::GetPCR(u32 & PCR_Base, u32 &PCR_ext)
{
	if(( GetAdaptiationControl())&0x2)
	{
		if((Data[GetPktHeadSize()+1]>>4)& 0x1)//PCR_flag
		{
			u8 * pdata = Data + GetPktHeadSize()+2;
		
			PCR_Base = pdata[0]*256*256*256 + pdata[1]*256*256 + pdata[2]*256 + pdata[3];
			PCR_Base = PCR_Base << 1;/* dispose the first bit */ 
			PCR_Base = PCR_Base & (pdata[4] >> 7);
			
			PCR_ext = (pdata[4]& 0x1)*256 + pdata[5];

			return true;
		}
	}
	return false;
}


