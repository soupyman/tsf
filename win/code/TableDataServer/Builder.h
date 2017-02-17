#pragma once

#include "typedefine.h"
#include "DVBDefine.h"
#include "ThreadObject.h"

class CBuilder : public CThreadObject, public CDVBSpace
{
	class CRequest
	{
	public:
		/* data */
		u16			PID;
		u16			TableID;
		CRequest * 	pNext;
	public:
		CRequest(void){};
		~CRequest(void){};
	};

public:
	CRequest			* pReq;
	
	CTableDBServer 		* ClientAgent;
	
public:
	CBuilder(CTableDBServer * 	agent);
	~CBuilder(void);
	//bool Initialize(void);
	//void ConfigIndependenceTable(void);
	UINT MainLoop(void);
	//UINT MessagePump(void);

	//bool BuildAllTables(bool	BuildRelated = false);
	void BuildTable(CESData *  pES);
	void SaveTable(CTable * pTable, CESData * pES);

	//void BuildPacket(WPARAM PID,LPARAM Index);
	//void TableIndexChange(IndexSubCmd isc,CTable * pCurTbl);
	//void PacketIndexChange(IndexSubCmd isc,CESData * pEs);
	void CalcBitRate(CESData * 	pEs/* normally use video es, as they may have pcr*/);
	void AddRequest(u16 PID, u16 TableID);
	//void WriteBackOverSelf(WriteBackParam wbp, LPARAM lpData);
	//void WriteBackOverAll(WriteBackParam wbp, LPARAM lpData);
};

