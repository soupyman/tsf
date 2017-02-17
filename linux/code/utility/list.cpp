
#include "list.h"


CList::CList(CListNode * Head  , CListNode * Tail )
{
	HeadNode = Head;
	TailNode = Tail;

}

/*
*delete me and my children but not brothers
*/
CList::~CList(void)
{
	ClearNodes();
}

void CList::AddNodeToHead(CListNode * node )
{
	if(node)
	{
		node->pNext = NULL;
		node->pPrev = NULL;
		if(HeadNode== NULL)
		{
			HeadNode = node;
			TailNode = node;
		}
		else
		{
			HeadNode->pPrev = node;
			node->pNext = HeadNode;
			HeadNode = node;
		}
	}
}

void CList::AddNodeToTail(CListNode * node )
{
	if(node)
	{
		node->pNext = NULL;
		node->pPrev = NULL;
		if(TailNode == NULL)
		{
			HeadNode = node;
			TailNode = node;
		}
		else
		{
			TailNode->pNext = node;
			node->pPrev = TailNode;
			TailNode = node;
		}
	}
}

CListNode * CList::GetHeadNode()
{
	return HeadNode;
}

bool CList::RemoveNode(CListNode * node )
{
	CListNode * tmp = HeadNode;
	while(NULL != tmp)
	{
		if(tmp == node)
		{
			if(tmp->pNext)
				tmp->pNext->pPrev = tmp->pPrev;
			if(tmp->pPrev)
				tmp->pPrev->pNext = tmp->pPrev;

			
			if(tmp == HeadNode)
			{
				HeadNode = tmp->pNext;
			}
			if(tmp == TailNode)
			{
				TailNode = tmp->pPrev;
			}
			
			delete tmp;
			
			return true;
		}
	}
	return false;
}

void CList::ClearNodes(void)
{
	CListNode * tmp;
	while(NULL != HeadNode)
	{
		tmp = HeadNode;
		HeadNode = HeadNode->pNext;
		tmp->pNext = NULL;
		
		if(HeadNode)
			HeadNode->pPrev = NULL;
		
		delete tmp;
	}
}

