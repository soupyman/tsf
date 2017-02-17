#pragma once


#include "typedefine.h"

/*
CObject is used by system , so I use CTObject. :)
now this class is not used.
*/


class CTObject
{
public:
	CTObject();
	~CTObject();


public:
       u32             Identify;
      CTObject *     pNext;
};




class CTObjectDB
{
public:
    CTObjectDB();
    ~CTObjectDB();
    static u32 GetFreeIdentify();
    static CTObject * FindObjectById( u32 id);

    
private:
    
        static u32 IdentifyCount;

public:
    
        static CTObject * pObjList;
};
	
