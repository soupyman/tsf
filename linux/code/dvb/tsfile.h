#ifndef TSFILE_H
#define TSFILE_H

#include "table.h"
#include "section.h"
#include "packet.h"
#include "file.h"

using namespace NDVBSpace;
 
class CTSFile :public CFile
{
public:
#define PACKETS_NUMBER_TO_TRY_PACKET_SIZE	20
#define PITRATE_DEFAULT                    (6875000 * 4)

//#define MAX_TS_PAGE_SIZE  (1000 * 1024 * 1024)  /* 1G */

#define INVALID_PKT_IDX     0XFFFFFFFF

	enum TSFileState
	{
		TSFileUnknow,
		TSFileOpenFailed,
		TSFileAnalyseFailed,
		TSFileOK,

	};

//#define SOFTSIZE	2000

	TSFileState	m_FileState;

	u32			m_FileSize;
	u8 			m_nPacketSize;
	u32 			m_PacketCount;
	u32 	 		m_Offset;
	u64		       m_BitRate ;

    
public:
	CTSFile(s8 * pFileName);
	//CTSFile(u8	*	FileData, u32		FileLength);
	~CTSFile(void);
	void		    OpenAndAnalyse();
	//bool 		RollBack(TSFileErrorCode reason);
	bool 		Analyse(void);
	bool 		AnalyseAttribute(void);
        void            OpenForWrite();


	bool 		TraceTsfileState();
	void 		ReadPacket(CPacket & pp,u32 PacketIndex);
       bool             WritePacket(CPacket & pp, u32 PacketIndex);
       bool             AppendPacket(CPacket &pp);

	//bool 		SoftWritePacket(CPacket * pp);
	u8 			GetPacketSize();
    	u32 			GetPacketCount();

	u16 			ReadPktPID(u32 PacketIndex);
	bool 		WritePktPID(u32 PacketIndex, u16 NewPid);
	bool 		IsValid(void);
	//bool 		OverWrite(u8 * addr, u32 lenth);
	bool 		IsOtvMux();
	bool 		AnalyseOtvHead();
        bool        FindNextPkt(u32 &CurIdx, CTable	 & Tbl);
        bool            FindForwardSection(u32 & CurIdx, CTable	 & Tbl);
       CSection *  BuildNextSection(u32 & CurIdx,  CTable	 &Tbl);
	 bool            BuildTable(CTable	& pTbl );
        bool          RemuxTable(CTable	& pTbl );
        bool        RemuxPkt(CPacket   * * pkt, u32 PktCnt ,  u32 Gap);
        bool        EraseTable(CTable	 &pTbl);
        u64          CalcBitRate();
	u32 		GetRepetionGap(CTable	 &Tbl);


};


#endif
