#pragma once
#include <windows.h>
#pragma comment(lib,"winmm.lib")
#include "../../typedef/typedef.h"
#include "../../log/Pt_log.h"

pt_bool Pt_System_init(void *iost=NULL);
pt_bool Pt_System_Loop(void *iost=NULL);
pt_int Pt_System_Random(pt_int Min,pt_int Max);
pt_float Pt_System_RandomF(pt_float Min,pt_float Max);
void Pt_System_Free(void *iost=NULL);
void Pt_SystemLastError();

class pt_time
{
public:
	pt_time();
	pt_uint elpased();
private:
	pt_uint m_lastTime;
};