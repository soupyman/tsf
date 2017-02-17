#pragma once


class CTableDBServer : public CMessageSpace, public CDVBSpace
{
//#define MAX_CLIENT 10
	class CClient
	{
	//#define CLIENT_USED 0x1
	public:
		/* client : thread's ID*/
		u32		ClientID;
		
		/* data */
		u16		PID;
		u16		TableID;
		//u8			Version;

		CClient * pNext;
	private:	
		/*flag*/
		//u8			flag;

	public:
		CClient(void){};
		~CClient(void){};
		//bool IsUsed();
	};


public:
	//static CTableDBServer  * Self;
	CTableDatabase	* DataBase;
	CClient  			* Client;
	CBuilder			* Engine;
	CESDBServer 		* Source;
	//u32				EngineID;
public:
	CTableDBServer(CESDBServer * src);
	~CTableDBServer(void);
	//CTableDBServer * GetInstance(void);

	bool SubscribeTable(u32 ClientID , u16 TableID, u16 PID);
	bool	UnSubscribeTable(u32 ClientID , u16 TableID, u16 PID);
	//void TestCall();
	bool	RemoveTable(CTable * TblToRmv);
	bool	UpdateTable(CTable * TblToUdt);
	bool UpdatedNotify( u16 TableID , u16 PID);
      //bool	SaveTableToFile( u16 TableID , u16 PID);

private:
		
	bool InitTDBServer();
	bool ClearTDBServer();
	bool ClientExist(u32 ClientID , u16 TableID, u16 PID);
	bool AddClient(u32 ClientID , u16 TableID, u16 PID);
	
};





