#pragma once
#include "typedefine.h"
#include "DVBDefine.h"

class CSection :public CDVBSpace
{
public:
#define MAX_SECTION_LENGTH           		(4096)
#define INVALID_SECTION_NUMBER    		(256) /* */
	
	CSection(void);
	~CSection(void);

	u8	* GetSectionData() { return SectionData;};
	u16 	GetSectionLength();
	u16 	GetSectionNum();
      u16 GetLastSectionNum();

	u8 	GetSectionVersion();
	bool IsLastSection();
	u8	GetSectionSyntaxIndicator(void);
	u16 *  AnalyseSubTablePID(u16 * PidArray);
	//bool Build(CESData * 	es ,u32 PktIdx);
	//bool WriteBack(CESData * pES,u32 PktIdx = NULL);
	bool IsSame(u8 * pNewData, u32 DataLen);
	bool ReplaceData(u8 * pNewData, u32 DataLen);
	void AddDuplicateIdx(u32 Idx);

	CSection * Duplicate();
protected:	
public:

	u8			*		SectionData;

	u32					StartPktIdx; //start packet index in ES.
	
	CSection		*		pNext;
};

