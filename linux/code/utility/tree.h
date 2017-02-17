#ifndef TREE_H
#define TREE_H

#include "typedefine.h"



class CTreeNode
{	
public:
	CTreeNode(){};
	virtual ~CTreeNode(){};
	
	//virtual CTreeNode * Duplicate() = 0;
};




class CTree 
{
private:
	CTreeNode	* Node;
	
	CTree * Father;
	CTree * RightBrother;
	CTree * Children;
public:
	CTree(CTreeNode * Node_a , CTree * fa = NULL, 
		CTree * Bro = NULL, CTree *  Cld = NULL);
	
	virtual ~CTree(void);
	
	void 			ReleaseChildren(void);
	CTreeNode 	*	GetMyNode();
	CTree * 	GetChildren(void);
	CTree * 	GetRightBrother(void);
	CTree * 	GetFather(void);
	
	CTree * 	GetRoot(void);
	CTree * 	GetBigBrother(void);
      CTree * GetLeftBrother(void);

	CTree * 	GetLastSon(void);
	CTree * 	AddChild(CTree * ChildTree);
	CTree * 	AddBrother(CTree * BrotTree);
	CTree * 	FindOffSpring(CTree * TreeBranch);

	CTree * 	AddChild(CTreeNode * ChildLeafData);
	CTree * 	AddBrother(CTreeNode * BrotLeafData);

};


#endif
