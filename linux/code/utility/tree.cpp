
#include "tree.h"



CTree::CTree(CTreeNode * Node_a ,CTree * fa, CTree * Bro, CTree *  Cld)
{
	Node = Node_a;
	Father = fa;
	RightBrother = Bro;
	Children = Cld;
}

/*
*delete me and my children but not brothers
*/
CTree::~CTree(void)
{
	if(GetFather())/* not root , adjust brothers */
	{
		CTree *Brother = GetFather()->GetChildren();
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

void CTree::ReleaseChildren(void)
{
	CTree * pc;
	while((pc = (CTree *)Children))
	{
		pc->Father = NULL;
		Children = pc->RightBrother;
		pc->RightBrother = NULL;
	}
}

CTree * CTree::GetRoot(void)
{
	CTree * root = this;
	while(root->GetFather())
		root = (root->GetFather());
	return root;
}

CTree * CTree::GetChildren(void)
{
	return (CTree *)Children;
}

CTree * CTree::GetRightBrother(void)
{
	return (CTree *)RightBrother;
}

CTree * CTree::GetBigBrother(void)
{
	if(Father)
		return Father->GetChildren();
	else
		return this;
}

CTree * CTree::GetLeftBrother(void)
{
	CTree * LeftBro = GetBigBrother();
	while(LeftBro && (LeftBro->GetRightBrother() != this))
	{
		LeftBro = LeftBro->GetRightBrother();
	}
	
	return LeftBro; // this may be NULL
}

CTree * CTree::GetLastSon(void)
{
	CTree * pChild = GetChildren();
	if(pChild)
	{
		while(pChild->GetRightBrother())
			pChild = pChild->GetRightBrother();
		
		return pChild;
	}
	else
		return NULL;
}

CTree * CTree::GetFather(void)
{
	return (CTree *)Father;
}

CTree * CTree::AddChild(CTree * ChildTree)
{
	if(Children)
		return Children->AddBrother(ChildTree);
	else
	{
		ChildTree->Father = this;
		return (Children = ChildTree);
	}
}

CTree * CTree::AddBrother(CTree * BrotTree)
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
CTree * CTree::FindOffSpring(CTree * TreeBranch)
{
	CTree * pRes = NULL;
	if(NULL == TreeBranch)
	{
		printf("paramate error :FindOffSpring(NULL)");
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

CTree * CTree::AddChild(CTreeNode * ChildLeafData)
{
	if(Children)
		return Children->AddBrother(ChildLeafData);
	else
		return (Children = new CTree(ChildLeafData, this , NULL, NULL));
}
CTree * CTree::AddBrother(CTreeNode * BrotLeafData)
{
	if(RightBrother)
		return RightBrother->AddBrother(BrotLeafData);
	else
		return (RightBrother = new CTree(BrotLeafData,Father , NULL ,NULL));
}

CTreeNode 	*	CTree::GetMyNode()
{
	return Node;
}
