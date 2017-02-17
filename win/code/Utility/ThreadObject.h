#pragma once

#include "Message.h"

class CThreadObject : public CMessage
{
public:
	CThreadObject(void);
	virtual ~CThreadObject(void);

	CWinThread 	* Run();
	void Exit( );

	virtual UINT MainLoop() = 0;
	BOOL PostCommand( UINT msg,WPARAM wP = 0, LPARAM lP = 0);

public:
	CWinThread 	*		pThread;
};
