#pragma once




class CESDBServer : public CMessageSpace
{
	class CClient
	{
	//#define CLIENT_USED 0x1
	public:
		/* client : thread's ID*/
		u32		ClientID;
		
		/* data */
		u16		PID;
		CClient * pNext;

	public:
		CClient(void){};
		~CClient(void){};
	};
//#define MAX_CLIENT 10

public:
	CESDataBase		* DataBase;
	CClient  			* Client;
	CDemux			* Engine;
	//u32				EngineID;
public:
	CESDBServer(CTSFile * source);
	~CESDBServer(void);

	bool InitESDBServer(CTSFile * source);
	bool ClearESDBServer();
	
	bool ClientExist(u32 ClientID , u16 PID);
	bool AddClient(u32 ClientID , u16 PID);
	CESData * GetEs(u16 PID);
	bool SubscribeES(u32 ClientID , u16 PID);
	bool	UnSubscribeES(u32 ClientID , u16 PID);
	
	//bool	RemoveTable(CTable * TblToRmv);
	bool	UpdateES(u16 PID);//(u16 PID, CTable * TblToUdt);
	bool ESUpdatedNotify( u16 PID);
	void SavePart(u32 PktCnt);

};





