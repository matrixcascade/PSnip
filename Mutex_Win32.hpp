#pragma once

#ifdef _MSC_VER
#include <windows.h>
bool IsAppRunning() {
	HANDLE hMutex = CreateMutex(NULL, TRUE, L"458C3A00-3550-4410-A142-62D74720A8FA");
	if (hMutex != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
	}
	return false;
}
#endif // _MSC_VER
