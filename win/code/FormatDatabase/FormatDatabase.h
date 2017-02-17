#pragma once

#include "TypeDefine.h"

#include "twmlist.h"

class CFormatDatabase
{
private:
	static CTwmList 	PrivateFormatList; /* format tree list*/

public:
	CFormatDatabase(void);
	~CFormatDatabase(void);
	static bool FreeFormatList(void);
	void        DumpToStr(CString & TraceStr);
	bool 	AddFormat(CFormatTree * pFT);
	bool		RemoveFormat(CFormatTree * pFT);
      bool		CheckFormatHead(CFormatTree * pFT , CString & ErrInfo);
protected:

	CTwmList * 	GetPrivateFormatList();
	CFormatTree * GetPrivateDescriptorFormatByTag(u8 tag);
	CFormatTree * GetPrivateTableFormatByID(u16 ID);

};
