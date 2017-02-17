/*
as a tree , root shouldn't have brothers

*/

#pragma once

#include "DVBDefine.h"
#include "FormatDatabase.h"
#include "ContentTree.h"


class CFormatParser : public CFormatDatabase , public CDVBSpace
{
public:
	CFormatParser(void){};
	~CFormatParser(void){};

protected:
	
	//void RemoveBranch(TreeNodeType nt , const char * pSLabel);
	
    
	CContentTree * 	AddDefaultBranch(CContentTree * parent, const char * pSLabel , 
	         u32 index  ,u32 length , u64  value );
    
    CContentTree * 	AddFieldBranch(CContentTree * parent, const char * pSLabel , 
	u32 index  ,u32 lenth , u32  format  ,u64 value ,const char  * pSHelp = NULL);
      CContentTree * 	AddSubBodyBranch(CContentTree * parent,const char * pSLabel ,  u32 Index  ,u32 Length);
    CContentTree * 	AddReserveBranch(CContentTree * parent, u32 Index  ,u32 Length);
    CContentTree * 	AddSectionBranch(CContentTree * parent,const char * pSLabel ,  u64 Section);
    CContentTree * 	AddPacketBranch(CContentTree * parent,const char * pSLabel ,  u64 Packet, u32 PktIdx);
    CContentTree * 	AddTableBranch(CContentTree * parent,const char * pSLabel ,  u64 Table);

	bool 			PrivateDescriptorParse(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx);
	CContentTree * 	PrivateSectionParse(CSection * pSec );
	//CContentTree *  	PrivateTableRootParse(u16	TableID);
	bool GetPrivateTableTitle(u16	TableID , CString & pTitle );

private:
#define DATA_BUFF_MAX_SIZE_FOR_EXTARCT     1000
	//bool SearchBackFieldBranch(CContentTree * start, CString * pSLabel , u32 & Ret);
	CContentTree * 	AddComBranch(CContentTree * parent, const char * pSLabel , 
                CContentTreeNode::TreeNodeType nt ,u64 Param64 =0  ,u32 Param2 =0, 
                u32  Param3 =0, u32  Param4 =0, const char * pSHelp = NULL);
	u64 ExtractData(u8 * & pDescriptorData,u8 & OffSet, u8 Lenth, u32 Format);
	u32 ParseWithFormatTree(CFormatTree * pFt, CContentTree * &parent, u8 * & pDescriptorData,u32 & BaseBitIdx, u8 &	OffSet);

	virtual u8 ParseDescriptor(CContentTree * parent, u8 * pDescriptorData ,u32 BaseBitIdx);
	u8 Buff[DATA_BUFF_MAX_SIZE_FOR_EXTARCT];

};



