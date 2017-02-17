/*
as a tree , root shouldn't have brothers

*/

#ifndef FORMAT_PARSER_H
#define FORMAT_PARSER_H

//#include "dvbspace.h"
#include "formatdb.h"
#include "contenttree.h"


class CFormatParser : public CFmtDbInterface 
{
public:
	CFormatParser(void){};
	~CFormatParser(void){};

protected:
	//void RemoveBranch(TreeNodeType nt , const char * pSLabel);
	
	CContentTree * 	AddDefaultBranch(CContentTree * parent, const CString &  pSLabel , 
	         u32 index  ,u32 length , u64  value );
    
    CContentTree * 	AddFieldBranch(CContentTree * parent, const CString &  pSLabel , 
	u32 index  ,u32 lenth , u32  format  ,u64 value );
      CContentTree * 	AddSubBodyBranch(CContentTree * parent,const CString & pSLabel ,  u32 Index  ,u32 Length);
    CContentTree * 	AddReserveBranch(CContentTree * parent, u32 Index  ,u32 Length);
    CContentTree * 	AddSectionBranch(CContentTree * parent,const CString &  pSLabel );
    CContentTree * 	AddPacketBranch(CContentTree * parent,const CString & pSLabel ,u64 PID);
    CContentTree * 	AddTableBranch(CContentTree * parent,const CString &  pSLabel ,  u64 PID);
    CContentTree * 	AddRootBranch(const CString &  pSLabel );

	bool 			PrivateDescriptorParse(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	CContentTree * 	PrivateSectionParse(CSection * pSec );

	//CContentTree *  	PrivateTableRootParse(u16	TableID);
	bool                GetPrivateTableTitle(u16	TableID , CString & pTitle );
	bool 		GetPrivateTableCrcCheck(u16	TableID , bool & CrcCk );

private:
#define DATA_BUFF_MAX_SIZE_FOR_EXTARCT     (1024 * 1024) 
/* section lenth is 12 bit lenth,so the max lenth is 4096 bytes. */

	//bool SearchBackFieldBranch(CContentTree * start, CString * pSLabel , u32 & Ret);
        void	        InsertReserveBranchIfNeed(CContentTree * parent , u32 index   );
	CContentTree * 	AddComBranch(CContentTree * parent, const CString & pSLabel , 
                CContentTreeNode::TreeNodeType nt ,u64 Param64 =0  ,u32 Param2 =0, 
                u32  Param3 =0, u32  Param4 =0);
	u64 ExtractData(u8 * & pDescriptorData,u8 & OffSet, u32 Lenth, u32 Format);
	u32 ParseWithFormatTree(CFormatTree * pFt, CContentTree * &parent, u8 * & pDescriptorData,u32 & BaseBitIdx, u8 &	OffSet);

	virtual u8 ParseDescriptor(CContentTree * parent, u8 * pDescriptorData ,u32 BaseBitIdx);
	u8 Buff[DATA_BUFF_MAX_SIZE_FOR_EXTARCT];

};


#endif
