

#include "section.h"

#include "crc.h"

#include <stdlib.h>
#include <string.h>


CSection::CSection(void)
{
	SectionData = NULL;
	StartPktIdx = 0xFFFFFFFF;
	pNext = NULL;
}
/*
CSection::CSection(u16 SectinSize)
{
	ASSERT(SectionSize > 0);

	SectionData = (u8 *)malloc(SectinSize);
	SectionSize = SectinSize;
	SectionContent = NULL;
	//pNext = NULL;
}
*/
CSection::~CSection(void)
{
	if(SectionData)
	{
		free(SectionData);
		SectionData = NULL;
	}
}


u16   *CSection::AnalyseSubTablePID(u16 *PidArray)
{
	CommonSection *pCS = (CommonSection *)SectionData;
	u16 		Found_PID;
	u8		*pU8 = SectionData;

	if(pCS == NULL)
		return PidArray;

	if(PidArray == NULL)
	{
		PidArray = (u16 *)malloc(sizeof(u16)); /*first for pid count */
		PidArray[0] = 0;
	}
	switch(pCS->table_id)
	{
		case PROGRAM_ASSOCIATION_INFORMATION:
		{
			u16 ProgramCount = (GetSectionLength() - 5 - CRC_32_LENGTH) / 4;
			u16 ProgramNumber;

			pU8 += 8;
			for(int i = 0; i < ProgramCount; i++)
			{
				ProgramNumber = pU8[0] * 256 + pU8[1];
				Found_PID = (pU8[2] & 0x1F)	* 256 + pU8[3];
				if(ProgramNumber)// service -> PMT
				{
					PidArray[0] ++;
					PidArray = (u16 *)realloc(PidArray, (PidArray[0] + 1) * sizeof(u16));
					ASSERT(PidArray);
					PidArray[PidArray[0] ] =  Found_PID;
				}
				else	// NIT
				{
					//pList.AddTable(ACTUAL_NETWORK_INFORMATION, Found_PID);
				}
				pU8 += 4;
			}
		}
		break;
		case CONDITIONAL_ACCESS_INFORMATION:
		{
			int SectionLength = GetSectionLength();

			pU8 += 8;
			SectionLength -= 5;
			while(SectionLength >= 10)
			{
				Found_PID = (pU8[4] & 0x1F) * 256 + pU8[5];
				//pList.AddTable(ENTITLE_MANAGE_MESSAGE, Found_PID);
				PidArray[0] ++;
				PidArray = (u16 *)realloc(PidArray, (PidArray[0] + 1) * sizeof(u16));
				ASSERT(PidArray);
				PidArray[PidArray[0] ] =  Found_PID;

				SectionLength -= (pU8[1] + 2);
				pU8 += (pU8[1] + 2);
			}
		}
		break;
		case PROGRAM_MAP_INFORMATION:
		{
			PMTSection *pPMTS = (PMTSection *)SectionData;
			int LeftLength = pPMTS->program_info_length_hi * 256 + pPMTS->program_info_length_low;

			pU8 += 12 ;
			while(LeftLength > 5)
			{
				if((pU8[0]) == DESCRIPTOR_CA)
				{
					Found_PID = (pU8[4] & 0x1F) * 256 + pU8[5];
					PidArray[0] ++;
					PidArray = (u16 *)realloc(PidArray, (PidArray[0] + 1) * sizeof(u16));
					ASSERT(PidArray);
					PidArray[PidArray[0] ] =  Found_PID;

					//pList.AddTable(ENTITLE_CONTROL_MESSAGE_EVEN, Found_PID);
					//pList.AddTable(ENTITLE_CONTROL_MESSAGE_ODD, Found_PID);
				}
				LeftLength -= (pU8[1] + 2);
				pU8 += (pU8[1] + 2);
			}
			pU8 += LeftLength;
			LeftLength = GetSectionLength() - 9 - (pPMTS->program_info_length_hi * 256 + pPMTS->program_info_length_low);
			while(LeftLength >= (5 + 6 + 4))
			{
				u16 es_info_length = (pU8[3] & 0x0F) * 256 + pU8[4];
				LeftLength -= (5 + es_info_length);
				pU8 += 5;

				while(es_info_length >= 6)
				{
					if((pU8[0]) == DESCRIPTOR_CA)
					{
						Found_PID = (pU8[4] & 0x1F) * 256 + pU8[5];

						PidArray[0] ++;
						PidArray = (u16 *)realloc(PidArray, (PidArray[0] + 1) * sizeof(u16));
						ASSERT(PidArray);
						PidArray[PidArray[0] ] =  Found_PID;

						//pList.AddTable(ENTITLE_CONTROL_MESSAGE_EVEN, Found_PID);
						//pList.AddTable(ENTITLE_CONTROL_MESSAGE_ODD, Found_PID);
					}
					es_info_length -= (pU8[1] + 2);
					pU8 += (pU8[1] + 2);
				}
				pU8 += es_info_length;
			}
		}
		break;
		default:
			ASSERT(0);
			break;
	}
	return PidArray;

}

/*
bool CSection::SectionDataValid()
{
	CommonSection * pCS =(CommonSection * )SectionData;
	return ((SectionSize -3) == (pCS->section_length_hi*256+pCS->section_length_low));
}
*/
u16 CSection::GetSectionNum()
{
	if(SectionData)
	{
		CommonSection *pCS = (CommonSection *)SectionData;
		return (pCS->section_number);
	}
	else
		return INVALID_SECTION_NUMBER;
}

u16 CSection::GetLastSectionNum()
{
	if(SectionData)
	{
		CommonSection *pCS = (CommonSection *)SectionData;
		if(pCS->section_syntax_indicator)
			return (pCS->last_section_number);
		else
			return 0;
	}
	else
		return INVALID_SECTION_NUMBER;
}

u16 CSection::GetSectionLength()
{
	if(SectionData)
	{
		CommonSection *pCS = (CommonSection *)SectionData;
		return (pCS->section_length_hi * 256 + pCS->section_length_low);
	}
	else
		return 0;
}

u8 CSection::GetSectionVersion()
{
	if(SectionData)
	{
		CommonSection *pCS = (CommonSection *)SectionData;
		return (pCS->version_number);
	}
	else
		return INVALID_VERSION;
}

bool CSection::IsLastSection()
{
	CommonSection *pPS = (CommonSection *)SectionData;
	return (pPS->section_syntax_indicator ? (pPS->last_section_number <= pPS->section_number) : 1);
}

u8	CSection::GetSectionSyntaxIndicator(void)
{
	CommonSection *pPS = (CommonSection *)SectionData;
	return (pPS->section_syntax_indicator);
}

/*

return true:
table finished;
false : table building not finished


bool CSection::Build(CESData * 	es ,u32 PktIdx)
{
	u32 NeedCopyCount  , SecBuffLen;

	CPacket  * pPkt = es->GetPacket(PktIdx);
	u8	PktSize = pPkt->GetPktSize();
	CommonSection * pPS = pPkt->GetSectionAddress();

	SecBuffLen = (pPS->section_length_hi*256+pPS->section_length_low) + 3 ;
	//	section length only	indicat data lenth after this field, so +3

	ASSERT(SecBuffLen < MAX_SECTION_LENGTH);
	if(SectionData)
		free(SectionData);
	if((SectionData =  (u8 *)malloc(SecBuffLen)) == NULL)
	{
		TRACE("Malloc(%d) section data fail\r\n",SecBuffLen);
		return false;
	}
	StartPktIdx = PktIdx;

	NeedCopyCount = SecBuffLen;

	if(pPkt->GetDataStartOffset() + NeedCopyCount < (PktSize))
	{// this section in one Packet
		memcpy(SectionData , pPS ,NeedCopyCount);
	}
	else
	{//	this section transmited in more than one Packet
		u8 * BuffAddr = SectionData;
		u8 CurCpy;
		memcpy(BuffAddr , pPS , (PktSize - pPkt->GetDataStartOffset()));
		NeedCopyCount -= (PktSize - pPkt->GetDataStartOffset());
		BuffAddr += (PktSize - pPkt->GetDataStartOffset());
		PktIdx ++;
		while(NeedCopyCount > 0)
		{
			if((NeedCopyCount + PACKET_HEAD_SIZE) > (PktSize ))
				CurCpy = (PktSize - PACKET_HEAD_SIZE);
			else
				CurCpy = (u8)NeedCopyCount;
			memcpy(BuffAddr , ((u8 *)((es->GetPacket(PktIdx))->GetPktData()))+ PACKET_HEAD_SIZE ,CurCpy);
			BuffAddr += CurCpy;
			NeedCopyCount -= CurCpy;// may be copy more than we want
			PktIdx ++;
		}
	}
	return true;
}
*/
/*
bool CSection::WriteBack(CESData * pES,u32 PktIdx)
{
	//if(PktIdx == NULL)
	//	PktIdx = StartPktIdx;//write back to myself by default
	bool ret =  false ;
	CPacket  * pPkt = pES->GetPacket(PktIdx);
	CommonSection * pPS = pPkt->GetSectionAddress();
	s32 NeedCopyCount = GetSectionLength() + 3;
	if(NeedCopyCount < (pES->GetPacketSize()- pPkt->GetDataStartOffset()))//in one packet
	{
		memcpy((u8 *)pPS, SectionData, NeedCopyCount);
		//ret = pES->SavePacketToTS(PktIdx);
		ASSERT(0);
	}
	else
	{//	this section in more than one Packet
		u8 * BuffAddr = SectionData;
		u8 CurCpy;
		memcpy(pPS , BuffAddr , (pES->GetPacketSize() - pPkt->GetDataStartOffset()));
		ret = pES->SavePacketToTS(PktIdx);
		NeedCopyCount -= (pES->GetPacketSize() - pPkt->GetDataStartOffset());
		BuffAddr += (pES->GetPacketSize() - pPkt->GetDataStartOffset());
		while(NeedCopyCount > 0)
		{
			PktIdx++;
			pPkt = pES->GetPacket(PktIdx);
			if(NeedCopyCount > (pES->GetPacketSize() - PACKET_HEAD_SIZE))
				CurCpy = (pES->GetPacketSize() - PACKET_HEAD_SIZE);
			else
				CurCpy = (u8)NeedCopyCount;
			memcpy(((u8 *)(pPkt->GetPktData()))+ PACKET_HEAD_SIZE ,BuffAddr , CurCpy);
			ret = pES->SavePacketToTS(PktIdx);
			BuffAddr += CurCpy;
			NeedCopyCount -= CurCpy;// may be copy more than we want
		}
	}
	return ret;
}
*/
bool CSection::IsSame(u8 *pNewData, u32 DataLen)
{
	return (memcmp(SectionData, pNewData, DataLen) == 0);
}

bool CSection::IsSame(CSection *pSec)
{
	if((GetSectionLength()  !=  pSec->GetSectionLength()) ||
			(memcmp(GetSectionData() , pSec->GetSectionData(), (GetSectionLength() + 3)) != 0))
		return false;
	else
		return true;
}

bool CSection::ReplaceData(u8 *pNewData, u32 DataLen)
{
	if(IsSame(pNewData, DataLen))
		return false;

	if(SectionData)
	{
		free(SectionData);
		SectionData = NULL;
	}
	ASSERT(DataLen <= 0xFFFF);
	SectionData = (u8 *)malloc(DataLen);
	memcpy(SectionData, pNewData , DataLen);
	{
		// if updated
		CRC myCrc;
		u32 NewCrc = myCrc.CalCrc32(SectionData, (u16)(DataLen - 4));

		SectionData[DataLen - 4] = (u8)(NewCrc >> 24 & 0xFF);
		SectionData[DataLen - 3] = (u8)(NewCrc >> 16 & 0xFF);
		SectionData[DataLen - 2] = (u8)(NewCrc >> 8 & 0xFF);
		SectionData[DataLen - 1] = (u8)(NewCrc & 0xFF);
	}


	return true;
}

bool CSection::ReCalCrc()
{
	CRC myCrc;
	u16 lenth = 	GetSectionLength() + 3;
	u32 NewCrc = myCrc.CalCrc32(SectionData, (lenth - 4));

	SectionData[lenth - 4] = (u8)(NewCrc >> 24 & 0xFF);
	SectionData[lenth - 3] = (u8)(NewCrc >> 16 & 0xFF);
	SectionData[lenth - 2] = (u8)(NewCrc >> 8 & 0xFF);
	SectionData[lenth - 1] = (u8)(NewCrc & 0xFF);

	return true;
}

CSection *CSection::Duplicate()
{
	CSection *pSecO = new CSection;
	pSecO->SectionData = (u8 *)malloc(GetSectionLength() + 3);
	memcpy(pSecO->SectionData, SectionData, GetSectionLength() + 3);
	pSecO->StartPktIdx = StartPktIdx;

	if(pNext)
		pSecO->pNext = pNext->Duplicate();
	else
		pSecO->pNext = NULL;

	return pSecO;
}


