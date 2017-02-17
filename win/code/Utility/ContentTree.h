/*


*/

#pragma once

#include "TypeDefine.h"
#include "Tree.h"

class CContentTreeNode : public CTreeNode
{
public:
	CString 				Title; /* name # comment */
	enum TreeNodeType
	{
		ROOT_NODE,
		TABLE_NODE,
		SECTION_NODE,
		PACKET_NODE,
		SUBBODY_NODE,
		FIELD_NODE,
		RESERVE_NODE
	}			Type;
	
public:
	CContentTreeNode(){};
	~CContentTreeNode(){};
    	bool AppendMJDTime(u64 MJD , CString & str);
      CString  GetTitleWithFmtVal();
	virtual CContentTreeNode * Duplicate();
	//virtual bool operator==(CTreeNode & OtherTreeNode) = 0;
};


class CContentRootNode : public CContentTreeNode
{
public:
	CContentRootNode(){};
	~CContentRootNode(void){};
	CContentTreeNode * Duplicate();
};

class CContentReserveNode : public CContentTreeNode
{
public:
      	u32		Index;
	u32		Lenth;	

public:
	CContentReserveNode(){};
	~CContentReserveNode(void){};
	CContentTreeNode * Duplicate();
};

class CContentFieldNode : public CContentTreeNode
{
public:
	u32		Index;
	u32		Lenth;	
    enum FieldFormat
	{
	    FIELD_FMT_BIN,
          FIELD_FMT_DEC,
          FIELD_FMT_HEX,
          FIELD_FMT_BCD,
          FIELD_FMT_MJD,
          FIELD_FMT_STR
	}			Format;
    
	u64		    Value;
    /*normally it's value of this field,
	but if (Lenth>64) or Format == FIELD_FMT_STR
	Value should be pointer to a memory */

    CString Help;
    
public:
	CContentFieldNode(){Help.Empty();};
	~CContentFieldNode(void);
	CContentTreeNode * Duplicate();
};

class CContentTableNode : public CContentTreeNode
{
public:
	CTable * 		pTable;

public:
	CContentTableNode(){};
	~CContentTableNode(void){};
	CContentTreeNode * Duplicate();
};

class CContentSectionNode : public CContentTreeNode
{
public:
      CSection *     pSection;
	//u32 			DataAddr;
	//u32			DataLenth;	

public:
	CContentSectionNode(){};
	~CContentSectionNode(void){};
	CContentTreeNode * Duplicate();
};

class CContentSubBodyNode : public CContentTreeNode
{
public:
	u32		Index;
	u32		Lenth;	

public:
	CContentSubBodyNode(){};
	~CContentSubBodyNode(void){};
	bool Verified();
	CContentTreeNode * Duplicate();
};

class CContentPacketNode : public CContentTreeNode
{
public:
	CESData * 		pES;
	u32 			PacketIdx;

public:
	CContentPacketNode(){};
	~CContentPacketNode(void){};
	CContentTreeNode * Duplicate();
};


class CContentTree : public CCommonTree
{
public:
	CContentTree(CContentTreeNode * Node_a , CContentTree * fa = NULL, 
		CContentTree * Bro = NULL, CContentTree *  Cld = NULL);
	
	~CContentTree(void);

public:
	CContentTree * Duplicate();
	CContentTreeNode::TreeNodeType GetNodeType();

	void * GetOrigin();
	
	bool SearchBackFieldValue( CString * pSLabel , u64 & Ret);
	CContentTree * SearchBranchByNode(CContentTreeNode * Node_a);
	//bool  DescetIdx( u16  DesCnt);
	//bool  AdjustRelativeIdxForIsolate();

};
