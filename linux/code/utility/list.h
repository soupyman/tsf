#ifndef LIST_H
#define LIST_H

#include "typedefine.h"


class CListNode
{	
public:	
        CListNode * pNext;
        CListNode * pPrev;
public:
	CListNode(){pNext = NULL;pPrev = NULL;};
	virtual ~CListNode(){};

};




class CList
{
    
private:	
	CListNode * HeadNode;
       CListNode * TailNode;
public:
	CList(CListNode * Head = NULL , CListNode * Tail = NULL);
	virtual ~CList(void);
	void AddNodeToHead(CListNode * node );
	void AddNodeToTail(CListNode * node );
	bool RemoveNode(CListNode * node );

      CListNode * GetHeadNode();
      void  ClearNodes(void);

};


#endif
