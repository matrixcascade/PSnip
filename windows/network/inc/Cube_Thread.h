#pragma  once

#include <Windows.h>


//////////////////////////////////////////////////////////////////////////
/*     CriticalSection operator*/
//////////////////////////////////////////////////////////////////////////
#define  CubeInitializeCriticalSection(x)	InitializeCriticalSection(x)
#define  CubeEnterCriticalSection(x)		EnterCriticalSection(x)
#define  CubeLeaveCriticalSection(x)		LeaveCriticalSection(x)
typedef  CRITICAL_SECTION            CubeCriticalSection; 

class Cube_Thread
{
public:
	Cube_Thread();
	virtual ~Cube_Thread();

	void start();
	void terminate();
	bool IsRunning();
	void SetRunningFlag();
	virtual void run(){};
	virtual void stop(){};
	virtual void free(){};

	void  __ThreadFlagRemove();

	void  WaitForExit();
private:
	bool   m_IsRunning;
	HANDLE m_ThreadHandle;
	DWORD  m_ThreadID;
};

#define  CUBE_MAX_THREADS_COUNT 64
struct THREADS_PARAM
{
	void *p;
	int nThread;
};
class Cube_Threads
{
public:
	Cube_Threads(){};
	virtual ~Cube_Threads(){};

	bool start(int nThread);
	bool start(int *nThread);
	void terminate(int nThread);
	bool IsRunning(int nThread);
	void SetRunningFlag(int nThread);
	virtual void run(int nThread){};
	virtual void stop(int nThread){};
	virtual void free(int nThread){};

	void  __ThreadFlagRemove(int nThread);

	void  WaitForExit(int nThread);
	void  WaitForAllExit();
private:
	THREADS_PARAM m_Param[CUBE_MAX_THREADS_COUNT];
	bool   m_IsRunning[CUBE_MAX_THREADS_COUNT];
	HANDLE m_ThreadHandle[CUBE_MAX_THREADS_COUNT];
	DWORD  m_ThreadID[CUBE_MAX_THREADS_COUNT];
};

