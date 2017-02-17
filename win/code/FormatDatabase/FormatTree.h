#pragma once

#include "TypeDefine.h"

#include "Tree.h"


class CTokenNode: public CTreeNode
{
public:
	enum
	{
		KEY_WORD_DESCRIPTOR,
		KEY_WORD_LOOP,
		KEY_WORD_IF,
		KEY_WORD_ELSE,
		DATA_NODE,
		RESERVE_NODE,
		TABLE_HEAD_NODE,
		DESCRIPTOR_HEAD_NODE
	} 	NodeType;

public:
	CTokenNode(){};
	~CTokenNode(){};
	//virtual u32 GetLength(){ return 0;};
};


class CTableHeadNode:public CTokenNode
{
public:
	CString  		Name;
	u8				ID;		/*  tableID  */
public:
	CTableHeadNode();
	~CTableHeadNode();
};

class CDescriptorHeadNode:public CTokenNode
{
public:
	CString  		Name;
	u8				ID;		/* tag */
public:
	CDescriptorHeadNode();
	~CDescriptorHeadNode();
};

class CReserveNode:public CTokenNode
{
public:	
	//u8			Valuelength;
      CFormula * pLengthFml; /*calcuate length */
public:
	CReserveNode();
	~CReserveNode();
	//u32 GetLength(){return Valuelength;};
};

class CDataNode:public CTokenNode
{
public:
	enum {
      	    FIELD_FMT_BIN,
          FIELD_FMT_DEC,
          FIELD_FMT_HEX,
          FIELD_FMT_BCD,
          FIELD_FMT_MJD,
          FIELD_FMT_STR
	}  format;
	
	CString 		NameString;
	//u8			Valuelength;
	CFormula * pLengthFml;
public:
	CDataNode();
	~CDataNode();
	//u32 GetLength(){return Valuelength;};
};

class CKeyLoopNode:public CTokenNode
{
public:
	CFormula * pFml;
	CString 	   Name; 
public:
	CKeyLoopNode();
	~CKeyLoopNode();
};

class CKeyIfNode :public CTokenNode
{
public:
	CFormula * pFml;
	//CFormatTree *	NoContext;
    /*if( formula)
        {
            run children;
        }
        else
        {
            run NoContext;
        }*/
public:
	CKeyIfNode();
	~CKeyIfNode();
};

class CKeyElseNode :public CTokenNode
{/* this node can only follow CKeyIfNode */
public:
	CKeyElseNode();
	~CKeyElseNode();
};

class CKeyDescriptorNode:public CTokenNode
{
public:
	CKeyDescriptorNode();
	~CKeyDescriptorNode();
};

class CFormatTree : public CCommonTree
{
public:
	CFormatTree(CTokenNode * Node_a , CFormatTree * fa = NULL, 
		CFormatTree * Bro = NULL, CFormatTree *  Cld = NULL);
	
	~CFormatTree(void);

	//u32 CalculateFormatLenth();
	bool SearchBackField( CString * pSLabel);

	bool DumpToString(CString & TraceStr,u8 NestDeep);

};

