
#ifndef FORMAT_DB_H
#define FORMAT_DB_H

#include "typedefine.h"
#include "cstring.h"
#include "formattree.h"
#include "list.h"


class CFormatDbNode: public CListNode
{
private:
	CFormatTree * pFmtTree;
public:
	CFormatDbNode(CFormatTree * pFm ){pFmtTree = pFm;};
	~CFormatDbNode();
	CFormatTree * GetFmtTree(){return pFmtTree;};

};



class CFormatDb:public CList
{

public:
	CFormatDb(void){};
	~CFormatDb(void){};
};





class CFmtDbInterface
{
private:
	static CFormatDb 	s_FmtDb; /* format tree list*/

public:
	CFmtDbInterface(void){};
	~CFmtDbInterface(void){};
	void        DumpToStr(CString & TraceStr);
	bool 	AddFormat(CFormatTree * pFT);
	bool		RemoveFormat(CFormatTree * pFT);
protected:

	CFormatTree * GetDescriptorFormat(u8 DescTag);
	CFormatTree * GetTableFormat(u16 TableId);
	static bool FreeAllFormat(void);
      bool		CheckFormatHead(CFormatTree * pFT , CString & ErrInfo);
	CFormatDb * 	GetFormatDb();

};

#endif 


