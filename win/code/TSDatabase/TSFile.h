#pragma once

#include "TypeDefine.h"
#include "TwmList.h"
#include "Packet.h"
#include "DVBDefine.h"

//class CESData;

class CTSFile : public CFile, public CDVBSpace
{
public:
#define PACKETS_NUMBER_TO_TRY_PACKET_SIZE	20
#define INVALID_PACKET_INDEX                    0xFFFFFFFF

#define MAX_TS_PAGE_SIZE  (1000 * 1024 * 1024)  /* 1G */

	enum TSFileState
	{
		TSFileUnknow,
		TSFileOpenFailed,
		TSFileAnalyseFailed,
		TSFileOK,

	};

#define SOFTSIZE	2000
	CString 		m_FileName;

	u64			m_FileSize;
	
	
	TSFileState	m_FileState;
	//TSFileErrorCode	m_FileError;
	CString 		m_FileErrorStr;
	bool			m_FileWritable;
	
	u8 			m_nPacketSize;
	u32 			m_nTotalPacketNumber;
	u32 	 		Offset;

	//CTwmList 	ESList;
	//CPacket  		pPacket;
	
public:
	CTSFile(LPCTSTR lpszPathName);
	//CTSFile(u8	*	FileData, u32		FileLength);
	~CTSFile(void);
	void		OpenAndAnalyse();
	//bool 		RollBack(TSFileErrorCode reason);
	bool 		Analyse(void);
	bool 		AnalyseAttribute(void);
	u8 			GetByte(u32 OffSet);
	CString 		GetErrString();
	void 		ReadPacket(CPacket * pp,u32 PacketIndex);
	bool 		HardWritePacket(CPacket * pp);
	bool 		SoftWritePacket(CPacket * pp);
	u8 			GetPacketSize();
	u16 			GetPacketPID(u32 PacketIndex);
	bool 		SetPacketPID(u32 PacketIndex, u16 NewPid);
	bool 		IsValid(void);
	//bool 		OverWrite(u8 * addr, u32 lenth);
	bool 		IsOtvMux();
	bool 		AnalyseOtvHead();

};



