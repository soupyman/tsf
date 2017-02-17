
#pragma once

/*
                   CONCEPT  DEFINE: 
                    
formular is two son tree,  has 3 kind of tree node.
1.  operator (including + - * / =)
2.  parameter
3.  number (non zero unsign integer)

leaf node type can be paramter or number. operator node must have two sons.

!!!NOTE: 
    About expertion, all operator has same priority. if you want to change the priority, use ().
    zero is not support for zero may cause exception in calcuate.
example:
1.
    experssion:         2 
    formular tree:    (2)

2.
    experssion:         section_lenth 
    formular tree:    (section_lenth)

3.
    experssion:         A + 2 
    formular tree:    (+)
                        /     \
                      (A)      (2)

4.
    experssion:        3 * A  + (B - 2 ) + C
    formular tree:   
                                     (+)  
                                  /       \
                             (+)           (C)
                          /       \
                      (*)          (-)
                    /     \        /   \
                  (3)      (A) (B)    (2)

5.
    experssion:        3 + A  - B * 2  = 5
    formular tree:   
                                   (=)  
                                /       \
                            (*)          (5)
                          /      \
                       (-)        (2)
                     /     \       
                  (+)      (B) 
                /     \       
             (3)      (A) 

*/

#include "typedefine.h"
#include "cstring.h"

#include "tree.h"



class CFormulaNode:public CTreeNode
{
public:
	CFormulaNode(){};
	~CFormulaNode(){};

public:
	enum
	{
		OPERATOR,
		PARAMETER,
		NUMERAL
	}	type ;
};


class CFormulaOperatorNode:public CFormulaNode
{
public:
	CFormulaOperatorNode();
	~CFormulaOperatorNode(){};

public:
	
	enum
	{
	       INVALID_OPERATOR,
		PLUS,
		MINUS,
		MULTIPLY,
		DIVIDE,
		EQUAL,
	} op;

};

class CFormulaParameterNode:public CFormulaNode
{
public:
	CFormulaParameterNode();
	~CFormulaParameterNode(){};

public:
	CString param;
};

class CFormulaNumeralNode:public CFormulaNode
{
public:
	CFormulaNumeralNode();
	~CFormulaNumeralNode(){};

public:
	u32 num;
};

class CFormula: public CTree
{
public:
	CFormula(CFormulaNode * pNode);
	~CFormula(){};
public:

	//bool SearchParamValue(CContentTree * start, CString * pSLabel , u32 & Ret);
	//bool	Calculate(CContentTree * start , u64 & Ret);
	//bool	IsValid(CFormatTree * start );
	bool DumpToString(CString & TraceStr);

};
