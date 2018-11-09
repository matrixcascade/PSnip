#include "../inc/Cube_Thread.h"

void __Cube_ThreadProcessFunc(void *p)
{
	((Cube_Thread *)p)->run();
	((Cube_Thread *)p)->__ThreadFlagRemove();
};


Cube_Thread::Cube_Thread()
{
	m_ThreadHandle=NULL;
	m_ThreadID=0;
	m_IsRunning=false;
}

Cube_Thread::~Cube_Thread()
{
	terminate();
}

void Cube_Thread::start()
{
	SetRunningFlag();
	m_ThreadHandle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)__Cube_ThreadProcessFunc,this,0,&m_ThreadID);
}

void Cube_Thread::__ThreadFlagRemove()
{
	m_ThreadHandle=NULL;
	m_ThreadID=NULL;
	m_IsRunning=false;
}

void Cube_Thread::WaitForExit()
{
	WaitForSingleObject(m_ThreadHandle, INFINITE);
}

void Cube_Thread::terminate()
{
	if(m_ThreadHandle)
	TerminateThread(m_ThreadHandle,0);
}

bool Cube_Thread::IsRunning()
{
	return m_IsRunning;
}

void Cube_Thread::SetRunningFlag()
{
	m_IsRunning=true;
}



void __Cube_ThreadsProcessFunc(void *p)
{
	THREADS_PARAM *param=(THREADS_PARAM *)p;
	((Cube_Threads *)param->p)->run(param->nThread);
	((Cube_Threads *)param->p)->__ThreadFlagRemove(param->nThread);
};

bool Cube_Threads::start(int *nThread)
{
	static THREADS_PARAM param;
	int i;
	for (i=0;i<CUBE_MAX_THREADS_COUNT;i++)
	{
		if (m_IsRunning[i]==false)
		{
			param.nThread=i;
			param.p=this;
			break;
		}
	}
	if (i==CUBE_MAX_THREADS_COUNT)
	{
		return false;
	}
	SetRunningFlag(param.nThread);
	m_ThreadHandle[param.nThread]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)__Cube_ThreadsProcessFunc,&param,0,&m_ThreadID[param.nThread]);
	return true;
}

bool Cube_Threads::start(int nThread)
{
	if (m_IsRunning[nThread]==false)
	{
			m_Param[nThread].nThread=nThread;
			m_Param[nThread].p=this;
	}
	else
	{
		return false;
	}

	m_ThreadHandle[m_Param[nThread].nThread]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)__Cube_ThreadsProcessFunc,&m_Param[nThread],0,&m_ThreadID[m_Param[nThread].nThread]);
	return true;
}

void Cube_Threads::terminate(int nThread)
{
	if(m_ThreadHandle[nThread])
		TerminateThread(m_ThreadHandle[nThread],0);
}

bool Cube_Threads::IsRunning(int nThread)
{
	return m_IsRunning[nThread];
}

void Cube_Threads::SetRunningFlag(int nThread)
{
	m_IsRunning[nThread]=true;
}

void Cube_Threads::__ThreadFlagRemove(int nThread)
{
	m_IsRunning[nThread]=false;
}

void Cube_Threads::WaitForExit(int nThread)
{
	WaitForSingleObject(m_ThreadHandle[nThread], INFINITE);
}

void Cube_Threads::WaitForAllExit()
{
	WaitForMultipleObjects(CUBE_MAX_THREADS_COUNT,m_ThreadHandle,TRUE,INFINITE);
}
