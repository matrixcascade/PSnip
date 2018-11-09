#pragma  once
#include <vector>
#include "pe_File.h"


#define  CUBECFGFILE_GUID_LEN  38
#define  CUBECFGFILE_FILENAME_LEN 255
using namespace std;

template <typename ConfigStruct>
class pe_ConfigFile
{
public:
	pe_ConfigFile()
	{
		m_FileName[0]='\0';
		m_StructSize=sizeof(ConfigStruct);
		memset(m_GUID,0,sizeof m_GUID);
	}
	void SetGUID(const char *GUID)
	{
		memcpy(m_GUID,GUID,CUBECFGFILE_GUID_LEN);
	}
	BOOL LoadCfgFile(const char *FileName)
	{
		ConfigStruct st;
		Free();

		strcpy(m_FileName,FileName);

		if(!m_file.Open((char *)FileName))
		{
			return FALSE;
		}
		char GUID[CUBECFGFILE_GUID_LEN];

		m_file.Read(GUID,CUBECFGFILE_GUID_LEN);

		if (strcmp(GUID,m_GUID)!=0)
		{
			goto _ERROR;
		}

		unsigned int Count;
		if(m_file.Read(&Count,sizeof(unsigned int))!=sizeof(unsigned int))
		goto _ERROR;

		
		for (unsigned int i=0;i<Count;i++)
		{
			if (m_file.Read(&st,sizeof(ConfigStruct))!=sizeof(ConfigStruct))
			{
				goto _ERROR;
			}
			m_vStruct.push_back(st);
		}

		m_file.Close();
		m_file.Free();

		
		return TRUE;

		_ERROR:
		m_vStruct.clear();
		m_file.Close();
		m_file.Free();
		return FALSE;
	}
	ConfigStruct &GetConfigStruct(int Index)
	{
		return m_vStruct.at(Index);
	}
	int GetConfigStructCount()
	{
		return m_vStruct.size();
	}
	void Add(ConfigStruct T)
	{
		m_vStruct.push_back(T);
		Update();
	}
	void Remove(int Index)
	{
		if (Index>m_vStruct.size()-1)
		{
			return;
		}
		m_vStruct.erase(m_vStruct.begin()+Index);
		Update();
	}

	void Clear()
	{
		m_vStruct.clear();
	}

	BOOL Update()
	{
		if(!m_file.Open(m_FileName,"wb"))
			return FALSE;

		if(m_file.Write(m_GUID,CUBECFGFILE_GUID_LEN)!=CUBECFGFILE_GUID_LEN)
			goto _ERROR;

		int Count=m_vStruct.size();

		if(m_file.Write(&Count,sizeof(unsigned int))!=sizeof(unsigned int))
			goto _ERROR;

		for(int i=0;i<Count;i++)
		{
			if (m_file.Write(&m_vStruct.at(i),sizeof(ConfigStruct))!=sizeof(ConfigStruct))
			{
				goto _ERROR;
			}
		}
		m_file.Close();
		m_file.Free();

		return TRUE;
_ERROR:
		m_file.Close();
		m_file.Free();
		return FALSE;
	}
	void Free()
	{
		m_vStruct.clear();
		m_file.Free();
		m_StructSize=0;
		memset(m_GUID,0,sizeof m_GUID);
	}
protected:
private:
	pe_File m_file;
	int m_StructSize;
	vector<ConfigStruct> m_vStruct;
	char m_FileName[CUBECFGFILE_FILENAME_LEN];
	char m_GUID[CUBECFGFILE_GUID_LEN];
};



