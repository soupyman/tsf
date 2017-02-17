#include "StdAfx.h"
#include "include.h"

CTSFile::CTSFile(LPCTSTR lpszPathName)
{
	m_FileWritable = true;
	m_FileState 	= TSFileUnknow;
	//m_FileError 	= TSFileOK;
	m_nPacketSize = 0;
	m_nTotalPacketNumber =0;
	Offset		=	0;
	m_FileName 	= lpszPathName;	
	//m_PgIx 		= 0;
	OpenAndAnalyse();
}

CTSFile::~CTSFile(void)
{
	//FreeESData();
	if(m_FileState != TSFileUnknow)
		Close( );
	//RollBack(TSFileClosed);
}

void CTSFile::OpenAndAnalyse( )
{

	if(!Open(m_FileName,CFile::modeReadWrite))
	{
		if(!Open(m_FileName,CFile::modeRead))
		{
			m_FileState = TSFileOpenFailed;
			return;
		}
		m_FileWritable = false;
	}
	m_FileSize = GetLength();
	
	if(false == Analyse())
		m_FileState = TSFileAnalyseFailed;
	else
		m_FileState = TSFileOK;
		
}


bool CTSFile::Analyse(void)
{
	//int 	TestTime;
	bool Ret	= false;
	Offset = 0;
	if( IsOtvMux())
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
	//Offset = 0;
	u32 os = Offset;
	u32	LastPart;
	for(int i =0; i <204 ; i++, os++)
	{
		if(GetByte(os) == SYN_BYTE)
		{
			/*	try 188 packet size	*/
			for(TestTime=1; TestTime<PACKETS_NUMBER_TO_TRY_PACKET_SIZE ; TestTime++)
			{
				if(GetByte(os+ 188*TestTime)!= SYN_BYTE)
					break;
			}
			if (TestTime == PACKETS_NUMBER_TO_TRY_PACKET_SIZE)
			{
				Ret = true;
				m_nPacketSize = 188;
				break;
			}
			/*	try 204 packet size	*/
			for(TestTime=1; TestTime<PACKETS_NUMBER_TO_TRY_PACKET_SIZE ; TestTime++)
			{
				if(GetByte(os+ 204*TestTime) != SYN_BYTE)
					break;
			}
			if (TestTime == PACKETS_NUMBER_TO_TRY_PACKET_SIZE)
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
		m_nTotalPacketNumber = (u32)((m_FileSize - os)/m_nPacketSize) ;
		LastPart = (u32)((m_FileSize - os) % m_nPacketSize );
		Offset = os;
	}
	return Ret;
}

u8 CTSFile::GetByte(u32 OffSet)
{
	//Seek returns the new byte offset from the beginning of the file.
	u8 data;
	Seek(OffSet, CFile::begin);
	Read(&data , 1);
	return data;
}

void CTSFile::ReadPacket(CPacket * pp,u32 PacketIndex)
{
	Seek(Offset+PacketIndex * m_nPacketSize, CFile::begin);
	Read(pp->GetPktData(), m_nPacketSize);
	pp->PacketSize = m_nPacketSize;
	pp->SetPktIdxInTs(PacketIndex);
}

bool CTSFile::HardWritePacket(CPacket * pp)
{
	if(m_FileWritable)
	{
		Seek(Offset+pp->GetPktIdxInTs()* m_nPacketSize, CFile::begin);
		Write(pp->GetPktData(), m_nPacketSize);
		return true;
	}
	else
		return false;
}

bool CTSFile::SoftWritePacket(CPacket * pp)
{
	if(m_FileWritable)
	{
		u16 pid = GetPacketPID(pp->GetPktIdxInTs());
		if((pp->GetPID() == pid)||(STUFF_PID ==  pid))
			HardWritePacket(pp);
		else
		{
			u16 OffsetIdx = 0;
			/* find next stuff packet */
			while((STUFF_PID   != GetPacketPID(pp->GetPktIdxInTs() + (++OffsetIdx)))&&(OffsetIdx < SOFTSIZE));
			
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

CString  CTSFile::GetErrString()
{
	switch(m_FileState)
	{
		default:
		case TSFileUnknow:
			m_FileErrorStr.AppendFormat("unknow error");
			break;
		case TSFileOpenFailed:
			m_FileErrorStr.AppendFormat("file open failed");
			break;
		case TSFileAnalyseFailed:
			m_FileErrorStr.AppendFormat("file analyse failed");
			break;
		case TSFileOK:
			m_FileErrorStr.AppendFormat("ts file ok");
			break;
	}
	return m_FileErrorStr;
}

u8 	CTSFile::GetPacketSize()
{
	return m_nPacketSize;
}
/*
	u8 syn_byte						:8 ;

	u8 PID_hi						:5 ;
	u8 transport_priority				:1 ;
	u8 payload_unit_start_indicator	:1 ;
	u8 transport_error_indicator		:1 ;
	
	u8 PID_low						:8;
*/
bool CTSFile::SetPacketPID(u32 PacketIndex, u16 NewPid)
{
	u8 data[2];
	if(m_FileWritable)
	{
		Seek(Offset+PacketIndex * m_nPacketSize+1, CFile::begin);
		Read(data , 2);

		data[0] = (data[0] & 0xE0) | (NewPid>>8);
		data[1] = NewPid & 0xFF;
		Seek(Offset+PacketIndex * m_nPacketSize+1, CFile::begin);
		Write(data , 2);
		return true;
	}
	else
		return false;
}

u16 CTSFile::GetPacketPID(u32 PacketIndex)
{
	u8 data[2];
	Seek(Offset+PacketIndex * m_nPacketSize+1, CFile::begin);
	Read(data , 2);
	return (((data[0]) & 0x1F)*256 + data[1]);
}

bool CTSFile::IsValid(void)
{
	if(m_FileState == TSFileOK)
	{
		/*if((m_nPacketSize == 0)||(m_nTotalPacketNumber == 0))
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
	if( (GetByte(0) == 'O') &&(GetByte(1) == 'T') &&(GetByte(2) == 'V') &&
		(GetByte(3) == ' ') &&
		(GetByte(4) == ':') &&(GetByte(5) == '-') &&(GetByte(6) == ')'))
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
	//Offset += 0xc7c;
	char cb;
	Offset = 0;
	u32 Offset2;
	while(1)
	{
		Offset2 = Offset;
		while( GetByte(Offset2++)  == 0);

		if((Offset2 - Offset) >10 )/*	more than 10 0	*/
		{
			Offset = Offset2-1;
			break;
		}
		
		cb = GetByte(Offset) ;
		
		Offset++;

		if(cb == 0 )
			m_FileErrorStr.AppendFormat("\r\n");
		else if(cb < 32 )/*		the charactor befor 32 is control code	*/
			m_FileErrorStr.AppendFormat("'%02x'", cb);
		else
			m_FileErrorStr.AppendFormat("%c", cb);
	}
	m_FileErrorStr.AppendFormat("\r\n");
	return true;
}
