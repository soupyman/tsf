#ifndef TABLE_FORMAT_H
#define TABLE_FORMAT_H

#include "sectionformat.h"
#include "table.h"

//class CTSFile;

class CStandardTableFormat : public CStandardSectionFormat
{
public:
	CStandardTableFormat(void);
	virtual ~CStandardTableFormat(void);

	CContentTree * ParseContent(CTable * pTbl);
	CContentTree *  ParseContentRoot(CTable * pTbl , CContentTree *	root);
	CString GetNameByTable(CTable * pTbl);
        CString GetShortName(CTable * pTbl);
	bool IsCrcNeed(CTable * pTbl);

};

#endif
