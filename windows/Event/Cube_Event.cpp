#include "Cube_Event.h"

BOOL Cube_Event::Create()
{
	if (m_EventHandle!=INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	return (m_EventHandle=CreateEvent(NULL,TRUE,FALSE,NULL))!=INVALID_HANDLE_VALUE;
}

BOOL Cube_Event::Wait(unsigned int ms)
{
	if (m_EventHandle==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	return WaitForSingleObject(m_EventHandle,ms)==WAIT_OBJECT_0;
}

void Cube_Event::Set()
{
	if(m_EventHandle!=INVALID_HANDLE_VALUE)
	SetEvent(m_EventHandle);
}

void Cube_Event::Reset()
{
	if(m_EventHandle!=INVALID_HANDLE_VALUE)
		ResetEvent(m_EventHandle);
}

void Cube_Event::Free()
{
	if(m_EventHandle!=INVALID_HANDLE_VALUE)
		CloseHandle(m_EventHandle);
	m_EventHandle=INVALID_HANDLE_VALUE;
}
