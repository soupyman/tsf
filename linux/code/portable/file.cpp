
#include "file.h"
#include <string.h>

CString CFile::s_WorkDir ;

CFile::CFile(const s8 *lpszPathName)
{
	m_FileWritable = true;
	m_FileName = lpszPathName;
	pFile = NULL;

}


CFile::~CFile(void)
{
	Close();
}

void  CFile::SetWorkDir(const s8 * Path)
{
	s_WorkDir = Path;
}
	
bool CFile::Close(void)
{
	if(pFile != NULL)
	{
		fclose(pFile);
		pFile = NULL;
		return true;
	}
	return false;
}
/*
��־ ����
r, rb ��ֻ����ʽ��
w, wb ��ֻд��ʽ�򿪡�����ļ������ڣ��򴴽����ļ��������ļ����ض�
a, ab ��׷�ӷ�ʽ�򿪡�����ļ������ڣ��򴴽����ļ�
r+, r+b, rb+ �Զ�д��ʽ��
w+, w+b, wh+ �Զ�д��ʽ�򿪡�����ļ�������ʱ���������ļ��������ļ����ض�
a+, a+b, ab+ �Զ���׷�ӷ�ʽ�򿪡�����ļ������ڣ��������ļ�
*/
bool CFile::Open(FileOpenFlag flag)
{
	CString PathName;
	if(m_FileName.at(0) == '/')
		{
		PathName = m_FileName;
		}
	else
		{
		PathName = s_WorkDir;
		PathName.Append(m_FileName);
		}
	
	if(NULL != pFile)
	{
		Close();
	}
	switch(flag)
	{
		case OPEN_TEXT_READ_ONLY:
			pFile = fopen((const char *)PathName.c_str(), "r");
			m_FileWritable = false;
			break;
		case OPEN_TEXT_WRITE_ONLY:
			pFile = fopen((const char *)PathName.c_str(), "w");
			m_FileWritable = true;
			break;
		case OPEN_TEXT_READ_AND_WRITE:
			pFile = fopen((const char *)PathName.c_str(), "r+");
			m_FileWritable = true;
			break;
		case OPEN_BIN_READ_ONLY:
			pFile = fopen((const char *)PathName.c_str(), "rb");
			m_FileWritable = false;
			break;
		case OPEN_BIN_WRITE_ONLY:
			pFile = fopen((const char *)PathName.c_str(), "wb");
			m_FileWritable = true;
			break;
		case OPEN_BIN_READ_AND_WRITE:
			pFile = fopen((const char *)PathName.c_str(), "rb+");
			m_FileWritable = true;
			break;
		default:
			printf("open flag unknow.");
			break;
	}

	if(NULL == pFile)
	{
		return false;
	}

	return true;
}


bool CFile::GetLenth(u32 &lenth)
{
	if(NULL == pFile)
	{
		return false;
	}

	fseek(pFile, 0L, SEEK_END);
	lenth = ftell(pFile);
	return true;
}


bool CFile::GetByte(u32 OffSet, u8 &data)
{
	if(NULL == pFile)
	{
		return false;
	}
	fseek(pFile, OffSet, 0);
	data =  fgetc(pFile);

	return true;
}

bool CFile::PutByte(u32 OffSet, u8 data)
{
	if((NULL == pFile) || (false == m_FileWritable))
	{
		return false;
	}

	fseek(pFile, OffSet, 0);
	fputc(data, pFile);

	return true;
}

bool CFile::SeekTo(u32 OffSet)
{
	if(NULL == pFile)
	{
		return false;
	}

	fseek(pFile, OffSet, 0);

	return true;
}

bool CFile::ReachEnd()
{
	if(NULL == pFile)
	{
		return false;
	}
	return feof(pFile);
}

u32 CFile::Read(u8 *pBuff, u32 lenth, s32 OffSet)
{
	u32 readsize;

	if(NULL == pFile)
	{
		return false;
	}
	if(OffSet >= 0)
	{
		fseek(pFile, OffSet, 0);
	}
	readsize =  fread(pBuff, 1,  lenth,  pFile);

	return readsize;
}

bool CFile::Write(u8 *pBuff, u32 lenth, s32 OffSet)
{
	u32 writesize;

	if((NULL == pFile) || (false == m_FileWritable))
	{
		return false;
	}
	if(OffSet >= 0)
	{
		fseek(pFile, OffSet, 0);
	}
	writesize = fwrite(pBuff, 1, lenth, pFile);

	return true;
}

CStreamFile::CStreamFile(const s8 *lpszPathName)
	: CFile(lpszPathName)
{
}


bool  CStreamFile::WriteString(CString &str, s32 OffSet)
{
	return Write((u8 *)str.c_str(), str.GetLength(), OffSet);
}

bool  CStreamFile::ReadString(CString &str, s32 OffSet)
{

	return true;
}


