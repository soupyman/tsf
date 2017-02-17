#ifndef SECTION_H
#define SECTION_H

#include "typedefine.h"
#include "dvbspace.h"

using namespace NDVBSpace;

class CSection
{
	public:
#define MAX_SECTION_LENGTH           		(4096)
#define INVALID_SECTION_NUMBER    		(256) /* */

		CSection(void);
		~CSection(void);

		u8	 *GetSectionData()
		{
			return SectionData;
		};
		u16 	GetSectionLength();
		u16 	GetSectionNum();
		u16 GetLastSectionNum();

		u8 	GetSectionVersion();
		bool IsLastSection();
		u8	GetSectionSyntaxIndicator(void);
		u16   *AnalyseSubTablePID(u16 *PidArray);
		//bool Build(CESData * 	es ,u32 PktIdx);
		//bool WriteBack(CESData * pES,u32 PktIdx = NULL);
		bool IsSame(u8 *pNewData, u32 DataLen);
		bool IsSame(CSection *pSec);

		bool ReplaceData(u8 *pNewData, u32 DataLen);
		void AddDuplicateIdx(u32 Idx);
		bool ReCalCrc();

		CSection *Duplicate();
	protected:
	public:

		u8					*SectionData;

		u32					StartPktIdx; //start packet index in TS.

		CSection				*pNext;
};


#endif
