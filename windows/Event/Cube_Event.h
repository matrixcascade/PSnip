#include <Windows.h>


class Cube_Event
{
public:
	Cube_Event()
	{
		m_EventHandle=INVALID_HANDLE_VALUE;
	}
	~Cube_Event()
	{
		Free();
	}
	BOOL Create();
	BOOL Wait(unsigned int ms);
	void Set();
	void Reset();
	void Free();
private:
	HANDLE m_EventHandle;
};

