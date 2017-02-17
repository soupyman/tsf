#ifndef SECTION_FORMAT_H
#define SECTION_FORMAT_H

//#include "DVBDefine.h"
#include "mpeg2descriptorformat.h"
#include "dvbdescriptorformat.h"

class CStandardSectionFormat : public CMPEG2Descriptor,public CDVBDescriptor
{
//#define CRC_32_LENGTH					4

public:	
	CStandardSectionFormat(void);
	~CStandardSectionFormat(void);
	CContentTree * ParseContentBySection(CSection * pSec);
	CString GetTitleBySection(CSection * pSec);
protected:
	bool ParseSectionContent(u8 * SectionData,CContentTree * CurRoot);
	bool ParsePATContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseCATContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParsePMTContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseTSDTContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseBATContent			(CommonSection * pCS , CContentTree * CurRt);	
	bool ParseTDTContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseRSTContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseSTContent				(CommonSection * pCS , CContentTree * CurRt);
	bool ParseTOTContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseDITContent				(CommonSection * pCS , CContentTree * CurRt);
	bool ParseSITContent				(CommonSection * pCS , CContentTree * CurRt);
	bool ParseSDTContent			(CommonSection * pCS , CContentTree * CurRt);
	bool ParseNITContent				(CommonSection * pCS , CContentTree * CurRt);
	bool ParseEITContent				(CommonSection * pCS , CContentTree * CurRt);
	bool ParsePrivateTableContent	(CommonSection * pCS , CContentTree * CurRt);
	bool ParseDescriptorLoop		(CContentTree * parent, u8 * pDescriptorData, u32 DescriptorDataLength,u32 BaseBitIdx = 0);
	u8 	ParseDescriptor				(CContentTree * parent, u8 * pDescriptorData ,u32 BaseBitIdx);

};


#endif
