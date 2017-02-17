 #include "StdAfx.h"
#include "include.h"

UINT AssistantThread(LPVOID lParam)
{
	UINT BR;
	CThreadObject * pTo = (CThreadObject *)lParam;

	BR =  pTo->MainLoop();
	assert(0);
	return BR;
};


CThreadObject::CThreadObject(void)
{
	pThread = NULL;
}

CThreadObject::~CThreadObject(void)
{
	Exit();
}

CWinThread 	* CThreadObject::Run( )
{
/*  CWinThread* AfxBeginThread(
				   AFX_THREADPROC pfnThreadProc,
				   LPVOID pParam,
				   int nPriority = THREAD_PRIORITY_NORMAL,
				   UINT nStackSize = 0, // same as creater
				   DWORD dwCreateFlags = 0,//0   Start the thread immediately after creation
				   LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL // same as creater
				);

nPriority:
                        THREAD_PRIORITY_ABOVE_NORMAL
                        THREAD_PRIORITY_BELOW_NORMAL
                        THREAD_PRIORITY_HIGHEST
                        THREAD_PRIORITY_IDLE
                        THREAD_PRIORITY_LOWEST
                        THREAD_PRIORITY_NORMAL
                        THREAD_PRIORITY_TIME_CRITICAL
*/


	pThread = AfxBeginThread(AssistantThread ,this,THREAD_PRIORITY_NORMAL,100, 0 , NULL);


	return pThread;
}

void CThreadObject::Exit( )
{
	if(pThread)
	{
		TerminateThread(pThread->m_hThread  ,0);
		delete pThread;
		pThread = NULL;
	}
}

UINT CThreadObject::MainLoop()
{
	UINT ret = 0;
	assert(0);
	return ret;
}

BOOL CThreadObject::PostCommand( UINT msg,WPARAM wP, LPARAM lP)
{
	if(pThread)
		return PostThreadMessage(pThread->m_nThreadID,msg ,wP,lP);
	else
	{
		assert(0);
		return false;
	}
}


