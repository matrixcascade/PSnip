#include "Pt_System.h"
#include <stdio.h>

pt_bool Pt_System_init(void *iost/*=NULL*/)
{
	srand(timeGetTime());
	return PT_TRUE;
}

pt_bool Pt_System_Loop(void *iost)
{
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));

	if(Msg.message != WM_QUIT) 
	{
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		} 
		return true;
	}
	return false;
}

pt_int Pt_System_Random(pt_int Min,pt_int Max)
{
	return Min+(rand()<<15|rand())%(Max-Min);
}

pt_float Pt_System_RandomF(pt_float Min,pt_float Max)
{
	return rand()*1.0f/RAND_MAX*(Max-Min)+Min;
}

void Pt_System_Free(void *iost)
{
	CoUninitialize();
}

void Pt_SystemLastError()
{
		// Retrieve the system error message for the last-error code
	char *text;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR)&text, 0, NULL);
	PT_ERROR(text);  //½á¹û
	LocalFree(text);

}

pt_time::pt_time()
{
	this->m_lastTime=timeGetTime();
}

pt_uint pt_time::elpased()
{
	pt_uint time=timeGetTime();
	pt_uint timeElpased=time-m_lastTime;
	m_lastTime=time;
	return timeElpased;
}
