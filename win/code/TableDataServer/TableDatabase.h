#pragma once

#include "TypeDefine.h"


//class CTable;

class CTableDatabase :public CTwmList
{

public:
	CTableDatabase(void);
	~CTableDatabase(void);	
	void 	Free(void);
	CTable * SearchTable(u16	tableid,u16 	pid );
	CTable * SearchNextTable(POSITION & CurPos , u16	tableid);
	bool 	AddTable( CTable * pNewTable);
	CTable * 	UpdateTable(CTable * pNewTable);
	bool  	RemoveTable(u16	tableid,u16 	pid);
};


	
