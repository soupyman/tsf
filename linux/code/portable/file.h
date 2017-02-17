#ifndef FILE_H
#define FILE_H

#include "typedefine.h"
#include "cstring.h"

#include<stdio.h>

class CFile
{

public:
	static CString   s_WorkDir;
	
      enum FileOpenFlag
        {
            OPEN_TEXT_READ_ONLY,
            OPEN_TEXT_WRITE_ONLY,
            OPEN_TEXT_READ_AND_WRITE,
            OPEN_BIN_READ_ONLY,
            OPEN_BIN_WRITE_ONLY,
            OPEN_BIN_READ_AND_WRITE    
        };
        
        FILE     *       pFile;
	CString 		  m_FileName;
	
	bool			m_FileWritable;
	
public:
	CFile(const s8 * lpszPathName);
	~CFile(void);
	
	static void 	SetWorkDir(const s8 * Path);
	bool 		Close(void);
	bool 		Open(FileOpenFlag flag );

	bool 		GetLenth(u32 & lenth);
	CString 		GetFileName(){return m_FileName;};    
	bool 		GetByte(u32 OffSet, u8 & data);
	bool 		PutByte(u32 OffSet, u8 data);
        bool             SeekTo( u32 OffSet);
        bool            ReachEnd();
	u32  		Read( u8 * pBuff, u32 lenth, s32 OffSet = -1);
	bool 		Write( u8 * pBuff, u32 lenth, s32 OffSet = -1);

};

class CStreamFile :public CFile
{
public:
	CStreamFile(const s8 * lpszPathName);
	~CStreamFile(void){};

	bool 		WriteString( CString & str, s32 OffSet = -1);
	bool 		ReadString( CString & str, s32 OffSet = -1);

};

#endif
