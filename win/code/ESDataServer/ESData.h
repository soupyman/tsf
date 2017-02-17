#pragma once
#include "TypeDefine.h"
#include "PacketPool.h"

//#define STEP_SIZE	10
//#define 	CHECK_AVAILABLE_MAX_PACKET_NUMBER	(1024*1024/2)	/* 1 M packets */



/*twm ES update need protect 	*/
class CESData :public CPacketPool ,public CDVBSpace, public virtual CMessageSpace
{
public:
	u16 				PID;
	bool				Integrity;
	//bool				Deleted;

    /* save old index in ts when es is edit */
	 u32				*OldPktIdxInTsArry;
        u32                     OldPktIdxCnt;
private:
	u32				CurPktIdx;
	
public:
	CESData(u16 PID);
	~CESData(void);
	
	//bool 	IsAvailable();
	CPacket * GetCurPacket(void);
	bool 		IfEsBuildFinish( );
	
	void 	SetCurPktIdx(u32 PacketIndex)
	{
		if(PacketIndex>= 0)
			CurPktIdx = PacketIndex;
	};
	u32 		GetCurPktIdx(void){return CurPktIdx;};
	bool 	FindForwardSection(s32 & CurIdx ,u16	tableid,u8 SecNmb);
	u32     FindOtherSectionCount(u16	tableid);

	CSection * BuildSection(s32 & CurIdx ,u16	tableid,u8 SecNmb);//(u32 PktIdx);
	CTable* 	BuildTable(u16 TableID);
	bool 	AppendSectionToPool(CSection * pSec, CPacketPool & PP );
	bool 	BuildFromTable(CTable * pTbl);
      void       PrintHeadInfo();
    void        SaveIndexInTS();


	bool 	SaveToFile(CString *pFileName = NULL);
	bool 	LoadFromFile(CString * pFileName);

	BOOL 	ReturnMessage( WPARAM wP = 0, LPARAM lP = 0);
	
	void 		ChangePID(u16 NewPid);

};

