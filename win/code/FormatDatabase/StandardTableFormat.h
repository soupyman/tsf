#pragma once

#include "StandardSectionFormat.h"

//class CTSFile;

class CStandardTableFormat : public CStandardSectionFormat
{
public:
	CStandardTableFormat(void);
	virtual ~CStandardTableFormat(void);

	CContentTree * ParseContent(CTable * pTbl);
	CContentTree *  ParseContentRoot(CTable * pTbl);
	CString GetNameByTable(CTable * pTbl);

};


