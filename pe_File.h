#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned long ULONG;
typedef int BOOL;
typedef unsigned long DWORD;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_PATH 260
#define PE_FILE_IOBUFSIZE 1024
class pe_File
{
public:
	BOOL  Open(char *FileName,const char *mode="rb");
	ULONG Read(void *pDestMemory,ULONG ReadSize);
	bool  ReadToken(void *pDestMemory, ULONG ReadSize);
	ULONG Write(void *pResBuffer,ULONG WriteSize);
	bool  WriteToken(void *pResBuffer, ULONG WriteSize);
	void  Close();

	void MoveToStart();
	void MoveToEnd();
	void MoveFromStart(DWORD Seek);
	void Free();
	
	ULONG GetFileSize();
	pe_File(void)
	{
		m_fp=NULL;
		m_FileSize=0;
	}
	~pe_File(void)
	{
		Free();
	}
private:
	char m_FileName[MAX_PATH];
	ULONG m_FileSize;
	FILE *m_fp;

};

extern ULONG stdGetFileSize(char *FileName);