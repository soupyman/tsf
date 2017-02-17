#pragma once

#include "TypeDefine.h"

#include "StandardTableFormat.h"
#include "StandardPacketFormat.h"


// interface class
class CFormatServer:public CStandardTableFormat//,public CStandardPacketFormat
{

public:
	CFormatServer(void);
	~CFormatServer(void);
};
