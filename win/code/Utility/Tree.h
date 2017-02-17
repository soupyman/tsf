/*


*/

#pragma once

#include "TypeDefine.h"



class CTreeNode
{	
public:
	CTreeNode(){};
	virtual ~CTreeNode(){};
	
	//virtual CTreeNode * Duplicate() = 0;
};




class CCommonTree 
{
private:
	CTreeNode	* Node;
	
	CCommonTree * Father;
	CCommonTree * RightBrother;
	CCommonTree * Children;
public:
	CCommonTree(CTreeNode * Node_a , CCommonTree * fa = NULL, 
		CCommonTree * Bro = NULL, CCommonTree *  Cld = NULL);
	
	virtual ~CCommonTree(void);
	
	void 			ReleaseChildren(void);
	CTreeNode 	*	GetMyNode();
	CCommonTree * 	GetChildren(void);
	CCommonTree * 	GetRightBrother(void);
	CCommonTree * 	GetFather(void);
	
	CCommonTree * 	GetRoot(void);
	CCommonTree * 	GetBigBrother(void);
      CCommonTree * GetLeftBrother(void);

	CCommonTree * 	GetLastSon(void);
	CCommonTree * 	AddChild(CCommonTree * ChildTree);
	CCommonTree * 	AddBrother(CCommonTree * BrotTree);
	CCommonTree * 	FindOffSpring(CCommonTree * TreeBranch);

	CCommonTree * 	AddChild(CTreeNode * ChildLeafData);
	CCommonTree * 	AddBrother(CTreeNode * BrotLeafData);

};

