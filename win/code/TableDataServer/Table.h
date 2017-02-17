#pragma once

#include "typedefine.h"
#include "DVBDefine.h"

class CTable : public CDVBSpace
{
#define PACKET_INDEX_ARRAY_STEP		(10)

public:
	CTable(u16 tableid =INVALID_TABLE_ID,u16 pid = INVALID_PID,u8 Ver = INVALID_VERSION);
	virtual ~CTable(void);
	void 	AttachSection(CSection * pSec);
	bool 	DetachSection(CSection * pSec);
	bool 	IsSame(CTable * pTbl);
	CTable * Duplicate();
	//void 	CopyTable(CTable * pTbl);
	//Result ForceReBuild(bool OtherVer = false);
	//void ReplaceSectionData(u8 SecNum, u8 * pNewData, u32 DataLen);
	bool 	UpDateVersion();
	//bool WriteBackToTS(void);
	bool 	SaveToFile(CStdioFile * pFile);
	//bool LoadFromFile(CString * pFileName);
	u16 * 	AnalyseSubTablePIDArray();
	bool 	FreeAllSection(void);
	//BOOL ReturnMessage(UINT msg, WPARAM wP, LPARAM lP);
	//u16 GetSectionLength(u8 SecNum);
	CSection * GetSectionByNumber(u16 SecNum);
	//CTable	* GetNextVer();
	//void SetNextVer(CTable	* nv);
	//u8 GetLastSectionNumber(void);
	bool 	IsValid(void);
	//bool AddToLast(CTable * last);
	u16 		GetValidSectionCount(void);
	//u16 CRCCheckReport(void);
	u16 		GetTableID(){	return TableID;};
	u16		GetPID(){	return PID; };
	u16		GetVersion(){	return Version; };
	u16		GetDupliCount(){	return DupliCountInStream; };
	CSection	* GetSection(){ return pSection;}
	//s32	GetCurPktIdx(){	return CurPktIdx; };
	//void	SetCurPktIdx(s32 Idx){	CurPktIdx = Idx; };
	void		DuplicateRecord(CTable * Duplicate);
	void		AddStartPktIdx(u32 StarPktIdx);
       
protected:
	/*	attributes		*/
	u16			PID;
	u16			TableID;/* 	table 's identify	*/
	u8			Version;
	u16			DupliCountInStream;
	/*	section datas		*/
	CSection	*	pSection;
	/*	record where other duplicate from	*/
    
	u32			StarPktIdxArySize;/* useless now*/
	u32 		* 	pStarPktIdxAry;/* useless now*/
	u32			StarPktIdxCount;/* useless now*/

public:	
       bool             Edit;
	CTable	*	pNextVer;/* 	table is a list diff version with same TableID	*/
	
};

