#ifndef CONTENTTREE_H
#define CONTENTTREE_H
/*	portable include  */
#include "typedefine.h"
#include "cstring.h"
#include "file.h"

/*	utility include  */
#include "tree.h"
#include "formula.h"

/*	dvb include  */
#include "packet.h"
//#include "table.h"
#include "section.h"

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
      Format_e	Format;
    
	u64		    Value;
    /*normally it's value of this field,
	but if (Lenth>64) or Format == FIELD_FMT_STR
	Value should be pointer to a memory */

   // CString Help;
    
public:
	CContentFieldNode(){/*Help.clear();*/};
	~CContentFieldNode(void);
	CContentTreeNode * Duplicate();
	bool  GetFormatString(CString & str);
	bool  GetValueString(CString & str);

};

class CContentTableNode : public CContentTreeNode
{
public:
	//CTable * 		pTable;
	u16             PID;

public:
	CContentTableNode(){};
	~CContentTableNode(void){};
	CContentTreeNode * Duplicate();
};

class CContentSectionNode : public CContentTreeNode
{
public:
     // CSection *     pSection;
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
       u16                  PID;
	//CPacket * 		pPkt;
	//u8				PacketSize;
	//u32				PktIdxInTs;	/* index in total TS */

public:
	CContentPacketNode(){};
	~CContentPacketNode(void){};
	CContentTreeNode * Duplicate();
};


class CContentTree : public CTree
{
        
public:
	CContentTree(CContentTreeNode * Node_a , CContentTree * fa = NULL, 
		CContentTree * Bro = NULL, CContentTree *  Cld = NULL);
	
	~CContentTree(void);

public:
	CContentTree * Duplicate();
	CContentTreeNode::TreeNodeType GetNodeType();
	bool	CalculateFormula(CFormula * pFormula , u64 & Ret);

	//void * GetOrigin();
	
	bool SearchBackFieldValue( CString * pSLabel , u64 & Ret);
	CContentTree * SearchBranchByNode(CContentTreeNode * Node_a);

	bool DumpToShortString(CString &TraceStr, u8 NestDeep);
	bool DumpToString(CString & TraceStr,u8 NestDeep);
	bool SaveToTxtFile(CStreamFile *file);
	bool SaveToXmlFile(CStreamFile * file,u8 NestDeep);
      // bool  TransferToTable( CTable & ptb);
        bool  FillData( u8 * pData, u32& idx);

};

#endif
