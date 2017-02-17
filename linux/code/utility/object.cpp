
#include "object.h"


s32    Object::ObjCnt = 0;

Object::Object()
{
	ObjCnt++;
}

/*
*delete me and my children but not brothers
*/
Object::~Object()
{
	ObjCnt--;
}


