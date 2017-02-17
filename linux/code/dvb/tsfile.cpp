
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "formatserver.h"

#include "tsfile.h"
#include "crc.h"

using namespace std;

CTSFile::CTSFile(s8 *pFileName): CFile(pFileName)
{
	m_FileState 	= TSFileUnknow;

	m_FileSize 	= 0;
	m_nPacketSize = 0;
	m_PacketCount = 0;
	m_Offset		=	0;
	m_BitRate = 0;

	//OpenAndAnalyse();
}

CTSFile::~CTSFile(void)
{
}

void CTSFile::OpenAndAnalyse()
{
	if(false == Open(CFile::OPEN_BIN_READ_AND_WRITE))
	{
		if(false == Open(CFile::OPEN_BIN_READ_ONLY))
		{
			m_FileState = TSFileOpenFailed;
			return;
		}
	}

	GetLenth(m_FileSize);

	if(false == Analyse())
		m_FileState = TSFileAnalyseFailed;
	else
		m_FileState = TSFileOK;

}

void CTSFile::OpenForWrite()
{
	if(false == Open(CFile::OPEN_BIN_WRITE_ONLY))
	{
		m_FileState = TSFileOpenFailed;
	}
	else
		m_FileState = TSFileOK;
}

bool CTSFile::Analyse(void)
{
	//int 	TestTime;
	bool Ret	= false;
	m_Offset = 0;
	if(IsOtvMux())
	{
		AnalyseOtvHead();
	}

	return AnalyseAttribute();
}
/*
* name : AnalyseTSDataAttribute
* param : pFileData ,nFileSize
* return : if analyse successed retrun ture or false.
* note : analyse out packet size and offset
*/
bool CTSFile::AnalyseAttribute(void)
{
	int 	TestTime;
	bool Ret	= false;
	//m_Offset = 0;
	u8 data;
	u32 os = m_Offset;
	u32	LastPart;
	for(int i = 0; i < 204 ; i++, os++)
	{
		GetByte(os, data);
		if(data == SYN_BYTE)
		{
			/*	try 188 packet size	*/
			for(TestTime = 1; TestTime < PACKETS_NUMBER_TO_TRY_PACKET_SIZE ; TestTime++)
			{
				GetByte(os + 188 * TestTime, data);
				if(data != SYN_BYTE)
					break;
			}
			if(TestTime == PACKETS_NUMBER_TO_TRY_PACKET_SIZE)
			{
				Ret = true;
				m_nPacketSize = 188;
				break;
			}
			/*	try 204 packet size	*/
			for(TestTime = 1; TestTime < PACKETS_NUMBER_TO_TRY_PACKET_SIZE ; TestTime++)
			{
				GetByte(os + 204 * TestTime, data);
				if(data != SYN_BYTE)
					break;
			}
			if(TestTime == PACKETS_NUMBER_TO_TRY_PACKET_SIZE)
			{
				Ret = true;
				m_nPacketSize = 204;
				break;
			}
		}
	}
	if(Ret)
	{
		//pPacket.SetPktLenth(m_nPacketSize);
		m_PacketCount = (u32)((m_FileSize - os) / m_nPacketSize) ;
		LastPart = (u32)((m_FileSize - os) % m_nPacketSize);
		m_Offset = os;
	}
	return Ret;
}

void CTSFile::ReadPacket(CPacket &pp, u32 PacketIndex)
{
	ASSERT(PacketIndex < m_PacketCount); /* should be 0 ~ m_PacketCount-1*/
	Read((u8 *)(pp.GetPktData()), m_nPacketSize , m_Offset + PacketIndex * m_nPacketSize);

	pp.PacketSize = m_nPacketSize;
	pp.SetPktIdxInTs(PacketIndex);
}

bool CTSFile::WritePacket(CPacket &pp, u32 PacketIndex)
{
	ASSERT(PacketIndex < m_PacketCount); /* should be 0 ~ m_PacketCount-1*/

	if(m_FileWritable)
	{
		//CPacket NewPkt;
		Write((u8 *)(pp.GetPktData()), m_nPacketSize, m_Offset + (PacketIndex * m_nPacketSize));
		//ReadPacket(NewPkt, IndexInTs);
		//INF("WritePacket success? %d \n", pp.SameWith(NewPkt, m_nPacketSize));
		return true;
	}
	else
		return false;
}

bool CTSFile::AppendPacket(CPacket &pp)
{
	if(m_FileWritable)
	{
		Write((u8 *)(pp.GetPktData()), pp.GetPktSize());
		return true;
	}
	else
		return false;
}

/*
bool CTSFile::SoftWritePacket(CPacket * pp)
{
	if(m_FileWritable)
	{
		u16 pid = ReadPktPID(pp->GetPktIdxInTs());
		if((pp->GetPID() == pid)||(STUFF_PID ==  pid))
			HardWritePacket(pp);
		else
		{
			u16 OffsetIdx = 0;

			while((STUFF_PID   != ReadPktPID(pp->GetPktIdxInTs() + (++OffsetIdx)))&&(OffsetIdx < SOFTSIZE));

			if(OffsetIdx < SOFTSIZE)
			{
				Seek(Offset+(pp->GetPktIdxInTs() + OffsetIdx)* m_nPacketSize, CFile::begin);
				Write(pp->GetPktData(), m_nPacketSize);
				pp->PktIdxInTs += OffsetIdx;
			}
			else
			{
				TRACE("Find next stuff packet failed !!!!!");
				HardWritePacket(pp);
			}
		}
		return true;
	}
	else
		return false;
}

*/
bool CTSFile::TraceTsfileState()
{
	//printf("file:%s . lenth: %d. \n", m_FileName.c_str(), m_FileSize);
	cout << endl << "File : " << m_FileName.c_str() << endl;
	cout << "Lenth:" << m_FileSize << endl;
	cout << "Packet size:" << (int)m_nPacketSize << endl;
	cout << "Packet count:" << m_PacketCount << endl;

	switch(m_FileState)
	{
		default:
		case TSFileUnknow:
			cout << "unknow error" << endl;
			break;
		case TSFileOpenFailed:
			cout <<  "file open failed" << endl;
			break;
		case TSFileAnalyseFailed:
			cout <<  "file analyse failed" << endl;
			break;
		case TSFileOK:
			cout <<  "ts file ok \n" << endl;
			return true;
	}
	return false	;
}

u8 	CTSFile::GetPacketSize()
{
	return m_nPacketSize;
}
u32 	CTSFile::GetPacketCount()
{
	return m_PacketCount;
}


/*
	u8 syn_byte						:8 ;

	u8 PID_hi						:5 ;
	u8 transport_priority				:1 ;
	u8 payload_unit_start_indicator	:1 ;
	u8 transport_error_indicator		:1 ;

	u8 PID_low						:8;
*/
bool CTSFile::WritePktPID(u32 PacketIndex, u16 NewPid)
{
	u8 data;
	ASSERT(PacketIndex < m_PacketCount); /* should be 0 ~ m_PacketCount-1*/

	if(m_FileWritable)
	{
		//INF("1ReadPktPID %d ", ReadPktPID(PacketIndex));
		GetByte(m_Offset + PacketIndex * m_nPacketSize + 1, data);

		data = (data & 0xE0) ;
		data |= ((NewPid >> 8) & 0x1F);
		PutByte(m_Offset + PacketIndex * m_nPacketSize + 1, data);
		data = (NewPid & 0xFF);
		PutByte(m_Offset + PacketIndex * m_nPacketSize + 2, data);

		//INF("WritePktPID %d new pid %d. ", PacketIndex, NewPid);
		//INF("2ReadPktPID %d ", ReadPktPID(PacketIndex));
		return true;
	}
	else
		return false;
}

u16 CTSFile::ReadPktPID(u32 PacketIndex)
{
	u8 data1, data2;
	ASSERT(PacketIndex < m_PacketCount); /* should be 0 ~ m_PacketCount-1*/

	GetByte(m_Offset + (PacketIndex * m_nPacketSize) + 1, data1);
	GetByte(m_Offset + (PacketIndex * m_nPacketSize) + 2, data2);

	return (((data1) & 0x1F) * 256 + data2);
}

bool CTSFile::IsValid(void)
{
	if(m_FileState == TSFileOK)
	{
		/*if((m_nPacketSize == 0)||(m_PacketCount == 0))
		{
			if(AnalyseAttribute())
			{
				m_FileState = TSFileReady;
				return true;
			}
			else
			{
				m_FileState = TSFileError;
				return false;
			}
		}*/
		return true;
	}
	else
		return false;
}

bool CTSFile::IsOtvMux()
{
	u8 data[7];
	Read(data, 7, 0);

	if((data[0] == 'O') && (data[1] == 'T') && (data[2] == 'V') &&
			(data[3] == ' ') &&
			(data[4] == ':') && (data[5] == '-') && (data[6] == ')'))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool CTSFile::AnalyseOtvHead()
{
	//m_Offset += 0xc7c;
	u8 byte;
	u8 otvhd[4096];
	m_Offset = 0;
	u32 ZeroCnt = 0;
	while(GetByte(m_Offset, byte))
	{
		if(0 == byte)
			ZeroCnt++;
		else
			ZeroCnt = 0;

		if(ZeroCnt >= 10) //	more than 10 0
		{
			break;
		}
		m_Offset++;
	}
	if(4096 > m_Offset)
	{
		memset(otvhd, 0 , 4096);
		Read(otvhd, m_Offset, 0);
		for(u32 i = 1; i < m_Offset; i++)
		{
			if(0 == otvhd[i])
			{
				if('\n' == otvhd[i - 1])
					otvhd[i - 1] = ' ';

				otvhd[i] = '\n'
						   ;
			}
		}
		//INF("%s", otvhd);
	}
	else
	{
		DBG("OTV head big! size is %d. ", m_Offset);
	}
	return true;
}

bool CTSFile::FindNextPkt(u32 &CurIdx, CTable  &Tbl)
{
	//u32 LastIdx = CurIdx;
	while((INVALID_PKT_IDX == CurIdx) || (CurIdx < (m_PacketCount - 1)))
	{
		if(INVALID_PKT_IDX == CurIdx)
			CurIdx = 0 ;
		else
			CurIdx ++;

		if(ReadPktPID(CurIdx) == Tbl.GetPID())
		{
			//DBG("two packet gap is %d, on PID %d.", CurIdx-LastIdx, Tbl.GetPID());
			return true;
		}
	}
	return false;
}


bool CTSFile::FindForwardSection(u32 &CurIdx, CTable  &Tbl)
{
	//u16 pid , u16	tableid
	CPacket  pPkt ;
	CommonSection *pPS;

	while((INVALID_PKT_IDX == CurIdx) || (CurIdx < (m_PacketCount - 1)))
	{
		if(INVALID_PKT_IDX == CurIdx)
			CurIdx = 0 ;
		else
			CurIdx ++;

		if(ReadPktPID(CurIdx) == Tbl.GetPID())
		{
			ReadPacket(pPkt, CurIdx);
			if(pPkt.IsUnitStart())
			{
				pPS = pPkt.GetSectionAddress();
				if(pPS->table_id == Tbl.GetTableID())
				{
					if(0 == Tbl.GetSubIdMask())
						return true;
					else if((Tbl.GetSubIdMask() & (pPS->sub_table_id_hi * 256 + pPS->sub_table_id_low))
							== (Tbl.GetSubIdMask() & Tbl.GetSubId()))
						return true;
				}
			}
		}
	}
	return false;
}


CSection *CTSFile::BuildNextSection(u32 &CurIdx, CTable	 &Tbl)
{
	u32 NeedCopyCount ;
	CPacket  Pkt ;
	u8 PayLoadLen;
	if(false == FindForwardSection(CurIdx, Tbl))
		return NULL;

	ReadPacket(Pkt, CurIdx);
	u8	PktSize = Pkt.GetPktSize();
	CommonSection *pPS = Pkt.GetSectionAddress();

	NeedCopyCount = (pPS->section_length_hi * 256 + pPS->section_length_low) + 3 ;
	/*	section length only	indicat data lenth after this field, so +3 */

	ASSERT(NeedCopyCount < 4096);
	CSection *pSec = new CSection;

	if(!pSec || ((pSec->SectionData = (u8 *)malloc(NeedCopyCount)) == NULL))
	{
		ERR("Malloc(%d) section data fail\r\n", NeedCopyCount);
		delete pSec;
		ASSERT(0);
		return NULL;
	}
	pSec->StartPktIdx = CurIdx;
	PayLoadLen = PktSize - Pkt.GetDataStartOffset();
	if(NeedCopyCount < PayLoadLen)
	{
		/* this section in one Packet	*/
		memcpy(pSec->SectionData , pPS , NeedCopyCount);
	}
	else
	{
		/*	this section transmited in more than one Packet	*/
		u8 *BuffAddr = pSec->SectionData;
		u8 CurCpy;
		memcpy(BuffAddr , pPS , PayLoadLen);
		NeedCopyCount -= PayLoadLen;
		BuffAddr += PayLoadLen;
		while(NeedCopyCount > 0)
		{
			if(false == FindNextPkt(CurIdx, Tbl))
			{
				break;
			}
			ReadPacket(Pkt, CurIdx);

			PayLoadLen = PktSize - Pkt.GetDataStartOffset();

			if((NeedCopyCount) > PayLoadLen)
				CurCpy = PayLoadLen;
			else
				CurCpy = (u8)NeedCopyCount;
			memcpy(BuffAddr , ((u8 *)(Pkt.GetSectionAddress())) , CurCpy);
			BuffAddr += CurCpy;
			NeedCopyCount -= CurCpy;/* may be copy more than we want */
		}

		if(NeedCopyCount > 0)
		{
			ERR("section data unintegreted on pkt %d \r\n", CurIdx);
			delete pSec;
			return NULL;
		}
	}
	return pSec;
}


bool CTSFile::BuildTable(CTable	 &pTbl)
{
	bool BltRst = false;
	CSection *pSec;
	CFormatServer fs;
	CContentTree *pCt;

	u32		CurIdx = INVALID_PKT_IDX;
	CRC 		crc;

	pTbl.FreeAllSection();

	while((INVALID_PKT_IDX == CurIdx) || (CurIdx < (m_PacketCount - 1)))
	{
		pSec = BuildNextSection(CurIdx, pTbl);
		if(pSec)
		{
			//DBG("cur %d, total %d, percentage %d %% .\r\n",
			//PktIdx, m_PacketCount, PktIdx * 100 / m_PacketCount);
			if(true ==  fs.IsCrcNeed(&pTbl))
			{
				if(false == crc.CheckCrc32(pSec->SectionData, pSec->GetSectionLength() + 3))
				{
					ERR("CRC error on section %d ,size %d .\r\n",
						pSec->GetSectionNum(), pSec->GetSectionLength() + 3);
					delete pSec;
					continue;
				}
				else
				{
					DBG("CRC correct on section %d ,size %d .\r\n",
						pSec->GetSectionNum(), pSec->GetSectionLength() + 3);
				}
			}
			pTbl.AttachSection(pSec);
			if(pTbl.IsIntegrity())
			{
				pTbl.UpDateVersion();
				pTbl.SortSection();
				pTbl.SourceTsName = GetFileName();
				pTbl.ShortName = fs.GetShortName(&pTbl);
				pTbl.pContent = fs.ParseContent(&pTbl);

				if(NULL == pTbl.pContent)
				{
					ERR("parse table(tableid:%d, sub tableid:%d; pid:%d) failed! \n",
						pTbl.GetTableID(), pTbl.GetSubId(), pTbl.GetPID());
				}
				else
				{
					BltRst = true;
				}
				break;
			}
		}
		else
		{
			break;
		}
	}
	return BltRst; /* build successed out	*/
}


bool CTSFile::RemuxTable(CTable	 &pTbl)
{
	bool BltRst = false;
	CPacket   *pkt[256];
	CSection *pSec ;
	u32		PktCnt = 0;
	u32		PktIdx = 0;
	u32 		CpySize;
	s32		SecSize;
	u8 	*SecData ;
	bool 	FirstPkt = true;
	s32		GapCnt;
	if(0 == m_BitRate)
	{
		CalcBitRate();
		if(0 == m_BitRate)
		{
			DBG("bit rate error!");
			return false;
		}
	}
	GapCnt = 0;//GetRepetionGap(pTbl);

	/* 1. 	new enough packets */
	pSec = pTbl.GetSection();
	while(pSec)
	{
		SecSize = (pSec->GetSectionLength() + 3) + 1 /*1 for start point*/;
		while(SecSize > 0)
		{
			/*	we ignor adoption here, for normarlly it's not used in si packet .*/
			PktCnt ++;
			SecSize -= (m_nPacketSize - PACKET_HEAD_SIZE);
		}
		pSec = pSec->pNext;
	}
	for(u16 i = 0 ; i < PktCnt; i++)
		pkt[i] = new CPacket;

	INF("step 1 done: new %d Packets. ", PktCnt);
	/* 2. 	build packets by section data */
	pSec = pTbl.GetSection();
	while(pSec)
	{
		SecSize = (pSec->GetSectionLength() + 3) + 1 /*1 for start point*/;
		SecData = pSec->SectionData;
		FirstPkt = true;
		while(SecSize > 0)
		{
			if(FirstPkt)
			{
				CpySize = (m_nPacketSize - PACKET_HEAD_SIZE - 1);
				if(CpySize > SecSize)
					CpySize = SecSize;

				memcpy(((u8 *)(pkt[PktIdx]->GetPktData())) + PACKET_HEAD_SIZE + 1, SecData, CpySize);
				pkt[PktIdx]->SetDataStartPointer(0);
				pkt[PktIdx]->SetUnitStart(true);
				FirstPkt = false;
			}
			else
			{
				CpySize = (m_nPacketSize - PACKET_HEAD_SIZE);
				if(CpySize > SecSize)
					CpySize = SecSize;

				memcpy(((u8 *)(pkt[PktIdx]->GetPktData())) + PACKET_HEAD_SIZE , SecData, CpySize);
				pkt[PktIdx]->SetUnitStart(false);
			}
			SecData += CpySize;
			SecSize -= CpySize;
			pkt[PktIdx]->SetAdaptiationControl(0x01);
			pkt[PktIdx]->SetContinuityCounter(PktIdx);
			pkt[PktIdx]->SetPID(pTbl.GetPID());
			PktIdx ++;
		}
		pSec = pSec->pNext;
	}
	INF("step 2 done: build Packets by section. ");
	/* 3. 	erase old table in tsfile */
	EraseTable(pTbl);
	INF("step 3 done: Erase old table in tsfile. ");

	/* 4. 	write table packets to tsfile */
	RemuxPkt(pkt, PktCnt, GetRepetionGap(pTbl));
	INF("step 4 done: write packets to tsfile. ");

	/* 5. 	delete packets memory */
	for(u16 i = 0 ; i < PktCnt; i++)
		delete pkt[i];

	INF("step 5 done: free packet memory. ");

	return BltRst; /* build successed out	*/
}

bool CTSFile::RemuxPkt(CPacket   * * pkt, u32 PktCnt, u32 Gap)
{
	u32		WriteCnt = 0;
	u32		PktIdxInTs = 0;
	u16		PktPid, NewPid;
	s32		GapCnt;
	u32		idx, PktIdx = 0;
	u32		IdxArry[100];
	if(PktCnt > 100)
	{
		ERR(" PktCnt: %d.", PktCnt);
		return false;
	}
	NewPid = pkt[0]->GetPID();
	
	if(0 == m_BitRate)
	{
		CalcBitRate();
		if(0 == m_BitRate)
		{
			DBG("bit rate error!");
			return false;
		}
	}
	GapCnt = 0;//GetRepetionGap(pTbl);

	//INF("write new table: 00%%. ");
	while(PktIdxInTs < (m_PacketCount - 1))
	{
		PktPid = ReadPktPID(PktIdxInTs);
		if((STUFF_PID == PktPid) && (0 >= GapCnt))
		{
			/*	writeable and gap reached */
			IdxArry[PktIdx] = PktIdxInTs;
			//WritePacket(*(pkt[PktIdx]), PktIdxInTs);
			PktIdx ++;
			if(PktIdx >= PktCnt)
			{
				/* find a valid space */
				for(PktIdx = 0 ; PktIdx < PktCnt; PktIdx ++)
				{
					WritePacket(*(pkt[PktIdx]), IdxArry[PktIdx]);
					WriteCnt++;
				}
				/*table repetition write finished.*/
				PktIdx = 0;
				GapCnt = Gap;
			}
		}
		else if(NewPid ==  PktPid)
		{
			PktIdx = 0;
		}

		if(GapCnt > 0)
			GapCnt--;

		PktIdxInTs ++;
		//INF("\10bwrite new table: %d%%. ", PktIdxInTs * 100 / m_PacketCount);
		/* \10b means back 10 char. */
	}
	INF(" write %d packets in tsfile. ", WriteCnt);
	return true;
}

bool CTSFile::EraseTable(CTable	 &pTbl)
{
	u32		PktIdxInTs = 0;
	u32		EraseCnt = 0;

	CPacket  Pkt ;
	CommonSection *pPS;
	u16 SectionLenth;

	//INF("erase old table: 00%%. ");

	while(FindForwardSection(PktIdxInTs, pTbl))
	{
		ReadPacket(Pkt, PktIdxInTs);
		pPS = Pkt.GetSectionAddress();
		SectionLenth = (pPS->section_length_hi * 256 + pPS->section_length_low) + 3 ;

		while(SectionLenth)
		{
			SectionLenth -= (Pkt.GetPktSize() - Pkt.GetDataStartOffset());

			WritePktPID(PktIdxInTs, STUFF_PID);
			EraseCnt++;
			if(SectionLenth)
			{
				if(false == FindNextPkt(PktIdxInTs, pTbl))
				{
					break;
				}
				ReadPacket(Pkt, PktIdxInTs);
			}
			//INF("\10berase old table: %d%%. ", PktIdxInTs * 100 / m_PacketCount);
		}
	}
	INF("\10berase %d packets of old table finished.", EraseCnt);
	return true;
}


/*
析得出PCR_Base 和 PCR_ext。
把PCR_Base*300+PCR_ext。就可以得到一个PCR的值。
那么再找紧跟其后的一个PCR的值。
当得到2个PCR的值 姑且定义为 PCR_ONE,PCR_TWO.
根据协议中的公式 I-2-5计算出rate；
rate = （2个PCR相隔的包的个数*188*8 *27000000）/（PCR_TWO - PCR_ONE）
*/


u64 CTSFile::CalcBitRate()
{
	CPacket  CurPkt ;
	u64 		PCR1 = 0, PCR2 = 0;
	u32 		CurPktIdx ;
	u32		PktIdxInTs_b;
	u16		PID;
	for(CurPktIdx = 0;  CurPktIdx < (m_PacketCount - 1); CurPktIdx++)
	{
		if(STUFF_PID != (ReadPktPID(CurPktIdx)))
		{
			ReadPacket(CurPkt, CurPktIdx);
			if(CurPkt.PCRAvalible())
			{
				CurPkt.GetPCR(PCR1);
				PID = CurPkt.GetPID();
				PktIdxInTs_b = CurPktIdx;
				break;
			}
		}
	}
	if(CurPktIdx >= m_PacketCount)
	{
		ERR("search PCR failed for ts file.");
		return 0 ;
	}

	for(CurPktIdx++; CurPktIdx < (m_PacketCount - 1); CurPktIdx++)
	{
		if(ReadPktPID(CurPktIdx) == PID)
		{
			ReadPacket(CurPkt, CurPktIdx);
			if(CurPkt.PCRAvalible())
			{
				CurPkt.GetPCR(PCR2);
				if(PCR2 > PCR1)
				{
					/* ok */
					u64 PCRDelta = (PCR2 - PCR1);
					u64 BitCnt = (CurPktIdx - PktIdxInTs_b);
					BitCnt = BitCnt * 188 * 8 * 27000000;
					m_BitRate = BitCnt / PCRDelta;
					//m_BitRate = ((CurPktIdx - PktIdxInTs_b) * 188 * 8 * 27000000) / (PCR2 - PCR1);
					//INF("PCR1 %x. ", PCR1);
					//INF("PCR2 %x. ", PCR2);
					INF("the bitrate of the stream is 0x%x . ", (u32)(m_BitRate));
					break;
				}
				else
				{
					PCR1 = PCR2;
					PktIdxInTs_b = CurPktIdx;
				}
			}
		}
	}
	return m_BitRate;
}

u32 CTSFile::GetRepetionGap(CTable	 &Tbl)
{
	u32 gap = m_BitRate / (188 * 8); //packet rate
	gap = gap * Tbl.GetRepetionTime() / 1000;
	return gap;
}

