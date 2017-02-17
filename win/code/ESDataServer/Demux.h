#pragma once
#include "typedefine.h"
#include "ThreadObject.h"
#include "DVBDefine.h"


class CDemux : public CThreadObject , public CDVBSpace
{
public:

	class CDemuxFilter
	{
	public:
		u16 			PID;
		CESData	* 	pES;
		s32 			DmxPktStart;
		u32			LastNotifyCount;
		CDemuxFilter * 		pNext;
	};

	class CRemuxFilter
	{
	public:
		u16 			PID;
		CESData	* 	pES;
		s32 			RmxPktStart;
		//bool			dlt;
		CRemuxFilter * 		pNext;
	};
	
private:
	CTSFile 			* SourceTS;
	CESDBServer 		* ClientAgent;
	
	CDemuxFilter		* pDmxFilter;
	CRemuxFilter		* pRmxFilter;
	//static CDemux * CDemux_at;
	//CEvent		*  m_event;
	//CShareResource m_syn;
	//CTSFile 		* pBase;
public:
	CDemux(CTSFile 	* src , CESDBServer   *cli);
	~CDemux(void);
	
	//static CDemux * GetInstance();
	bool 		AddDemuxFilter(CESData * pES);
	bool 		AddRemuxFilter(CESData * pES );
	
	bool			DeleteDmxFilterForFinish(CDemuxFilter * pFt);
	bool			DeleteRmxFilterForFinish(CRemuxFilter * pDft);

	//bool			Init(CTSFile 	* src , CESDBServer   *cli);
	UINT 		MainLoop();
	void 		RunDemux(void);
	void 		RunRemux(void);
	void 		SavePart(u32 Count);

};


