#pragma once

#include "TypeDefine.h"


//class CESData;

class CESDataBase :public CTwmList
{

public:
	CESDataBase(void);
	~CESDataBase(void);	
	void 	Free(void);
	bool 	AddES( CESData * pNewES);
	CESData * SearchES(u16 	pid );
	//CTable * SearchNextTable(POSITION & CurPos , u16	tableid);
	bool  	RemoveES(u16 	pid);
	//bool		UpdateTable();
};


	
