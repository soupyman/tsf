#ifndef FORMATTREE_H
#define FORMATTREE_H


#include "typedefine.h"

#include "tree.h"
#include "formula.h"


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
	bool			CrcCheck;
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
	Format_e  format;
	
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

class CFormatTree : public CTree
{
public:
	CFormatTree(CTokenNode * Node_a , CFormatTree * fa = NULL, 
		CFormatTree * Bro = NULL, CFormatTree *  Cld = NULL);
	
	~CFormatTree(void);

	//u32 CalculateFormatLenth();
	bool SearchBackField( CString & pSLabel);
	bool	CheckFormulaValid(CFormula * pFml );

	bool DumpToClearString(CString & TraceStr,u8 NestDeep);
	bool DumpToXmlString(CString & TraceStr,u8 NestDeep);
	CString GetFmtTreeName(void);

};

#endif

