#include "StdAfx.h"

#include "Tree.h"



CCommonTree::CCommonTree(CTreeNode * Node_a ,CCommonTree * fa, CCommonTree * Bro, CCommonTree *  Cld)
{
	Node = Node_a;
	Father = fa;
	RightBrother = Bro;
	Children = Cld;
}

/*
*delete me and my children but not brothers
*/
CCommonTree::~CCommonTree(void)
{
	if(GetFather())/* not root , adjust brothers */
	{
		CCommonTree *Brother = GetFather()->GetChildren();
		if(Brother == this)
		{
			GetFather()->Children = RightBrother;
		}
		else
		{
			while(Brother && (Brother->RightBrother != this))
				Brother= Brother->RightBrother;
			if(Brother)
				Brother->RightBrother = RightBrother;
		}
	}
	/*	free node	*/
	if(Node)
		delete Node;
	
	/* free children */
	while(Children)
	{
		delete Children; /* nest delete */
	}
}

void CCommonTree::ReleaseChildren(void)
{
	CCommonTree * pc;
	while(pc = (CCommonTree *)Children)
	{
		pc->Father = NULL;
		Children = pc->RightBrother;
		pc->RightBrother = NULL;
	}
}

CCommonTree * CCommonTree::GetRoot(void)
{
	CCommonTree * root = this;
	while(root->GetFather())
		root = (root->GetFather());
	return root;
}

CCommonTree * CCommonTree::GetChildren(void)
{
	return (CCommonTree *)Children;
}

CCommonTree * CCommonTree::GetRightBrother(void)
{
	return (CCommonTree *)RightBrother;
}

CCommonTree * CCommonTree::GetBigBrother(void)
{
	if(Father)
		return Father->GetChildren();
	else
		return this;
}

CCommonTree * CCommonTree::GetLeftBrother(void)
{
	CCommonTree * LeftBro = GetBigBrother();
	while(LeftBro && (LeftBro->GetRightBrother() != this))
	{
		LeftBro = LeftBro->GetRightBrother();
	}
	
	return LeftBro; // this may be NULL
}

CCommonTree * CCommonTree::GetLastSon(void)
{
	CCommonTree * pChild = GetChildren();
	if(pChild)
	{
		while(pChild->GetRightBrother())
			pChild = pChild->GetRightBrother();
		
		return pChild;
	}
	else
		return NULL;
}

CCommonTree * CCommonTree::GetFather(void)
{
	return (CCommonTree *)Father;
}



CCommonTree * CCommonTree::AddChild(CCommonTree * ChildTree)
{
	if(Children)
		return Children->AddBrother(ChildTree);
	else
	{
		ChildTree->Father = this;
		return (Children = ChildTree);
	}
}

CCommonTree * CCommonTree::AddBrother(CCommonTree * BrotTree)
{
	if(RightBrother)
		return RightBrother->AddBrother(BrotTree);
	else
	{
		BrotTree->Father = Father;
		return (RightBrother = BrotTree);
	}
}


/*
*	OffSpring include small brothers
*/
CCommonTree * CCommonTree::FindOffSpring(CCommonTree * TreeBranch)
{
	CCommonTree * pRes = NULL;
	if(NULL == TreeBranch)
	{
		TRACE("paramate error :FindOffSpring(NULL)");
		return NULL;
	}
	if(this == TreeBranch)
	{
		return this;
	}
	if(NULL != Children)
	{
		pRes = Children->FindOffSpring(TreeBranch);
		if(NULL != pRes)
			return pRes;
	}
	if(NULL != RightBrother)
	{
		pRes = RightBrother->FindOffSpring(TreeBranch);
		if(NULL != pRes)
				return pRes;
	}
	return NULL;
			
}

CCommonTree * CCommonTree::AddChild(CTreeNode * ChildLeafData)
{
	if(Children)
		return Children->AddBrother(ChildLeafData);
	else
		return (Children = new CCommonTree(ChildLeafData, this , NULL, NULL));
}
CCommonTree * CCommonTree::AddBrother(CTreeNode * BrotLeafData)
{
	if(RightBrother)
		return RightBrother->AddBrother(BrotLeafData);
	else
		return (RightBrother = new CCommonTree(BrotLeafData,Father , NULL ,NULL));
}

CTreeNode 	*	CCommonTree::GetMyNode()
{
	return Node;
}
