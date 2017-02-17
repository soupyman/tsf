#ifndef TABLE_H
#define TABLE_H

#include "typedefine.h"
#include "dvbspace.h"
#include "section.h"
#include "file.h"
#include "cstring.h"

using namespace NDVBSpace;

class CContentTree;

class CTable
{
		//#define PACKET_INDEX_ARRAY_STEP		(10)

		/* max rate in 100ms	*/
#define		PAT_REPET_RATE
#define		CAT_REPET_RATE
#define		PMT_REPET_RATE
#define		TSDT_REPET_RATE
#define		NIT_REPET_RATE  500
#define		BAT_REPET_RATE  200
#define		SDT_A_REPET_RATE  500
#define		SDT_O_REPET_RATE  1000
#define		EIT_PF_A_REPET_RATE  500
#define		EIT_PF_O_REPET_RATE  2000
#define		EIT_SCH_REPET_RATE  10000
#define		TDT_TOT_REPET_RATE  300

#define            DEFAULT_REPET_RATE  200 /* 0.5 s*/

	public:
		CTable(u16 tableid = INVALID_TABLE_ID, u16 pid = INVALID_PID, u8 Ver = INVALID_VERSION);
		virtual ~CTable(void);
		void    SortSection(void);
		//void    ReleaseSection(void);

		bool 	AttachSection(CSection *pSec);
		bool 	DetachSection(CSection *pSec);
		bool 	IsSame(CTable *pTbl);
		CTable *Duplicate();
		//void 	CopyTable(CTable * pTbl);
		//Result ForceReBuild(bool OtherVer = false);
		//void ReplaceSectionData(u8 SecNum, u8 * pNewData, u32 DataLen);
		bool 	UpDateVersion();
		bool       UpDateIdsFromSection();
		bool     UpDateSectionCrc();

		//bool WriteBackToTS(void);
		bool 	DumpToString(CString  &Str);
		bool 	SavBinToTxtFile(CFile *pFile);
		bool 	SaveToBinFile(CFile *pFile);
            bool        SaveToBinFile(CString FileName);
		bool 	SaveToTxtFile(CString FileName);
		bool 	SaveToXmlFile(CString FileName);

		//bool LoadFromFile(CString * pFileName);
		u16 	*AnalyseSubTablePIDArray();
		bool 	FreeAllSection(void);
		//BOOL ReturnMessage(UINT msg, WPARAM wP, LPARAM lP);
		//u16 GetSectionLength(u8 SecNum);
		CSection *GetSectionByNumber(u16 SecNum);
		//CTable	* GetNextVer();
		//void SetNextVer(CTable	* nv);
		bool        IsIntegrity();
		bool 	IsValid(void);
		//bool AddToLast(CTable * last);
		//u16 CRCCheckReport(void);
		u16 		GetValidSectionCount();
		u32 		GetRepetionTime();

		u16 		GetTableID()
		{
			return TableID;
		};
		u16		GetPID()
		{
			return PID;
		};
		void		SetPID(u16	 NwPid)
		{
			PID = NwPid;
		};

		u16		GetSubId()
		{
			return SubTableID;
		};
		u16		GetSubIdMask()
		{
			return SubIDMask;
		};
		u16		GetVersion()
		{
			return Version;
		};
		//u16		GetDupliCount(){	return DupliCountInStream; };
		CSection	 *GetSection()
		{
			return pSection;
		}
		//s32	GetCurPktIdx(){	return CurPktIdx; };
		//void	SetCurPktIdx(s32 Idx){	CurPktIdx = Idx; };
		void        SetIds(u16 tableid , u16 pid,  u16	SubID, u16	SubMask);
		bool  LoadFromXmlFile(CString FileName);

		bool  LoadFromContentTree(CContentTree *pCt);

	protected:
		/*	attributes		*/
		u16			PID;
		u16			TableID;/* 	table 's identify	*/
		u16			SubTableID;
		u16			SubIDMask;
		u8			Version;
		//u16			DupliCountInStream;
		/*	section datas		*/
		CSection		*pSection;
		/*	record where other duplicate from	*/
	public:
		CString          SourceTsName;
		CString          ShortName;
		CContentTree *pContent;
		//bool             Edit;
		CTable		*pNextVer;/* 	table is a list diff version with same TableID	*/

};


#endif

