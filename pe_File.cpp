#include "pe_File.h"

BOOL pe_File::Open( char *FileName,const char *mode)
{
	BOOL unNeedSize=FALSE;
	if (m_fp!=NULL)
	{
		fclose(m_fp);
		m_fp=NULL;
	}

	m_FileSize=stdGetFileSize(FileName);

	m_fp=fopen(FileName,mode);

	memset(m_FileName,0,sizeof m_FileName);

	strcpy(m_FileName,FileName);

	if (m_fp!=NULL)
	{
		return true;
	}
	return false;
}

void pe_File::Free()
{
if (m_fp!=NULL)
{
	fclose(m_fp);
}
m_fp=NULL;

}



ULONG pe_File::Read( void *PDestMemory,unsigned long ReadSize )
{
	if (m_fp==NULL)
	{
		return false;
	}
	if (PDestMemory==NULL)
	{
		return false;
	}
	return fread(PDestMemory,1,ReadSize,m_fp);
}

bool pe_File::ReadToken(void *pDestMemory, ULONG ReadSize)
{
	if (m_fp == NULL)
	{
		return false;
	}
	if (pDestMemory == NULL)
	{
		return false;
	}
	int offset = 0;
	int rsize;
	while (ReadSize)
	{
		if (ReadSize > PE_FILE_IOBUFSIZE)
		{
			if ((rsize = fread((char*)pDestMemory + offset, 1, PE_FILE_IOBUFSIZE, m_fp)) == 0)
				return false;
		}
		else
		{
			if ((rsize = fread((char*)pDestMemory + offset, 1, ReadSize, m_fp)) == 0)
				return false;
		}
		ReadSize -= rsize;
		offset += rsize;
	}
	return true;
}

void pe_File::Close()
{
	if (m_fp!=NULL)
	{
		fclose(m_fp);
		m_fp=NULL;
	}
	m_FileSize = 0;
}

void pe_File::MoveToStart()
{
	if (m_fp==NULL)
	{
		return;
	}
	fseek(m_fp, 0, SEEK_SET);
}

void pe_File::MoveToEnd()
{
	if (m_fp==NULL)
	{
		return;
	}
	fseek(m_fp,0,SEEK_END);

}

void pe_File::MoveFromStart( DWORD Pos )
{
	if (m_fp)
	{
		return;
	}
	fseek(m_fp,Pos,SEEK_SET);
}

unsigned long pe_File::GetFileSize()
{
	return m_FileSize;
}


ULONG pe_File::Write( void *pResBuffer,ULONG WriteSize )
{
	if (m_fp==NULL)
	{
		return false;
	}
	if (pResBuffer==NULL)
	{
		return false;
	}
	return fwrite(pResBuffer,1,WriteSize,m_fp);
}

bool pe_File::WriteToken(void *pResBuffer, ULONG WriteSize)
{
	if (m_fp == NULL)
	{
		return false;
	}
	if (pResBuffer == NULL)
	{
		return false;
	}
	int offset = 0;
	int wsize;
	while (WriteSize)
	{
		if (WriteSize > PE_FILE_IOBUFSIZE)
		{
			if ((wsize=fwrite((char*)pResBuffer + offset, 1, PE_FILE_IOBUFSIZE, m_fp)) == 0)
				return false;
		}
		else
		{
			if ((wsize = fwrite((char*)pResBuffer + offset, 1, WriteSize, m_fp)) == 0)
				return false;
		}
		WriteSize -= wsize;
		offset += wsize;
	} 
	return true;
	
}

ULONG stdGetFileSize( char *FileName )
{
	FILE*fp;
	fp=fopen(FileName,"rb");// localfileÎÄ¼þÃû

	if (fp==NULL)
	{
		return 0;
	}


	fseek(fp,0,SEEK_SET);
	fseek(fp,0,SEEK_END);
	ULONG size=ftell(fp);
	fclose(fp);
	return size;
}

BOOL Save( char *FileName,void *DestData,ULONG size)
{
	FILE *fp=NULL;
	if (DestData!=NULL)
	{
		fopen_s(&fp,FileName,"wb");
		if (fp!=NULL)
		{
			fwrite(DestData,1,size,fp);
			fclose(fp);
			return true;
		}
	}
	return false;
}

