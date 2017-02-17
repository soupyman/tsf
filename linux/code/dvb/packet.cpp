
#include "packet.h"
#include "file.h"

#include <stdlib.h>
#include <string.h>

CPacket::CPacket(void *PktDat, u8	PktLen, u32 index)
{
	ASSERT(PktLen);
	if(PktDat)
	{
		memcpy(Data, PktDat, PktLen);
	}
	else
	{
		PacketData *pPacketData = (PacketData *)Data;
		memset(Data, 0 , 204);
		pPacketData->syn_byte = SYN_BYTE;
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

void CPacket::SetPktData(void	 *pPD)
{
	if((pPD) && (memcmp(Data, pPD, PacketSize)))
	{
		memcpy(Data, pPD, PacketSize);
		Edit = true;
	}
}

void	 *CPacket::GetPktData(void)
{
	return (void *)Data;
}


bool CPacket::IsValid()
{
	/*	this should be updated 	*/
		return true;
}

bool CPacket::SameWith(CPacket &CmpPkt, u8 PktLen)
{
	if(IsValid() && CmpPkt.IsValid())
	{
		if(0 == memcmp(Data , CmpPkt.GetPktData() , PktLen))
			return true;
	}
	return false;
}

void CPacket::SetPID(u16 NPID)
{
	PacketData *pPacketData = (PacketData *)Data;
	pPacketData->PID_low = NPID & 0xFF ;
	pPacketData->PID_hi = (NPID >> 8) & 0x1F;
}

u16 CPacket::GetPID()
{
	PacketData *pPacketData = (PacketData *)Data;
	return (pPacketData->PID_hi * 256 + pPacketData->PID_low);
}

u32 CPacket::GetPktIdxInTs()
{
	return PktIdxInTs;
}

bool	CPacket::IsUnitStart()
{
	PacketData *pPacketData = (PacketData *)Data;
	if((pPacketData->payload_unit_start_indicator) && (GetAdaptiationControl() & 0x1))
		return true ;
	else
		return false;
}

bool	CPacket::SetUnitStart(bool IsStart)
{
	PacketData *pPacketData = (PacketData *)Data;

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
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->adaptiation_field_control ;
}

void CPacket::SetAdaptiationControl(u8 AFC)
{
	PacketData *pPacketData = (PacketData *)Data;
	pPacketData->adaptiation_field_control = AFC ;
}

u8	CPacket::GetScramblingControl()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->transport_scrambling_control ;
}

u8	CPacket::GetDataStartPointer()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->DataStartPointer ;
}

void CPacket::SetDataStartPointer(u8 DSP)
{
	PacketData *pPacketData = (PacketData *)Data;
	pPacketData->DataStartPointer = DSP;
}

u8	CPacket::GetPktHeadSize()
{
	return PACKET_HEAD_SIZE ;
}

u8 CPacket::GetDataStartOffset()
{
	PacketData *pPacketData = (PacketData *)Data;
	if(IsUnitStart() || (GetAdaptiationControl() & 0x2))
		return (PACKET_HEAD_SIZE + 1 + pPacketData->DataStartPointer);
	else
		return (PACKET_HEAD_SIZE);
}

u8 CPacket::GetAdaptationFieldLength()
{
	PacketData *pPacketData = (PacketData *)Data;
	if((pPacketData->adaptiation_field_control) & 0x2)
		return pPacketData->DataStartPointer;

	return 0;
}

CommonSection *CPacket::GetSectionAddress()
{
	return (CommonSection *)(Data + GetDataStartOffset());
}
u8 *CPacket::GetPesAddress()
{
	PacketData *pPacketData = (PacketData *)Data;

	if(GetAdaptiationControl() & 0x2)
		return Data + PACKET_HEAD_SIZE + 1 + pPacketData->DataStartPointer;
	else
		return Data + PACKET_HEAD_SIZE;
}

u8 		CPacket::GetSynByte()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->syn_byte;
}
u8		CPacket::GetTransportPriority()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->transport_priority;
}
u8 		CPacket::GetPayLoadUnitStartIndicator()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->payload_unit_start_indicator;
}
u8 		CPacket::GetTransportErrorIndivator()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->transport_error_indicator;
}
u8 		CPacket::GetContinuityCounter()
{
	PacketData *pPacketData = (PacketData *)Data;
	return pPacketData->continuity_counter;
}
void 		CPacket::SetContinuityCounter(u8 		CC)
{
	PacketData *pPacketData = (PacketData *)Data;
	pPacketData->continuity_counter = CC;
}


/*
PCR_Base  定义了33个bit
PCR_ext定义了9个bit
但是PCR 是27兆时钟300分频的一个取样。
而PCR = PCR_Base*300+PCR_ext;
adaptation_field() {
	adaptation_field_length						8
	if(adaptation_field_length >0) {
		discontinuity_indicator					1
		random_access_indicator				1
		elementary_stream_priority_indicator		1
		PCR_flag								1
		OPCR_flag							1
		splicing_point_flag						1
		transport_private_data_flag				1
		adaptation_field_extension_flag			1
		if(PCR_flag == '1') {
			program_clock_reference_base		33
			reserved							6
			program_clock_reference_extension	9
		}
.
.
.

*/

bool CPacket::GetPCR(u64 &PCR)
{
	u32 PCR_ext;
	if((GetAdaptiationControl()) & 0x2)
	{
		if((Data[PACKET_HEAD_SIZE + 1] >> 4) & 0x1) //PCR_flag
		{

			u8 *pdata = Data + PACKET_HEAD_SIZE + 2;

			PCR = (pdata[0] << 25) | (pdata[1] << 17) | (pdata[2] << 9) | (pdata[3] << 1) | (pdata[4] >> 7);
			PCR_ext = (pdata[4] & 0x1) * 256 + pdata[5];
			PCR = (PCR * 300) + PCR_ext;
			//INF("PCR data %d %d %d %d %d %d . ", pdata[0], pdata[1], pdata[2], pdata[3], pdata[4], pdata[5]);

			//INF("PCR %d . ", PCR);

			return true;
		}
	}
	return false;
}

bool CPacket::PCRAvalible()
{
	if((GetAdaptiationControl()) & 0x2)
	{
		if((Data[PACKET_HEAD_SIZE + 1] >> 4) & 0x1) //PCR_flag
		{
			return true;
		}
	}
	return false;
}

bool CPacket::DumpToFile(const s8 *filename)
{
	CFile *pFile = new CFile(filename);
	char  StrLine[256];

	if(NULL == pFile)
	{
		printf("new CFile(%s) failed.\n", filename);
		return false;
	}
	pFile->Open(CFile::OPEN_TEXT_WRITE_ONLY);
	pFile->SeekTo(0);

	for(int i = 0; i < PacketSize; i++)
	{
		if(i % 8 == 0)
		{
			sprintf(StrLine, "\n");
			pFile->Write((u8 *)StrLine, strlen(StrLine));
		}
		sprintf(StrLine, "0x%02x ", Data[i]);
		pFile->Write((u8 *)StrLine, strlen(StrLine));
	}
	sprintf(StrLine, "\n\n");
	pFile->Write((u8 *)StrLine, strlen(StrLine));

	pFile->Close();
	delete pFile;
	return true;
}


