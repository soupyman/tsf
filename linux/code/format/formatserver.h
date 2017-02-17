#ifndef FORMAT_SERVER_H
#define FORMAT_SERVER_H

#include "typedefine.h"

#include "tableformat.h"
#include "packetformat.h"


// interface class
class CFormatServer:public CStandardTableFormat,public CStandardPacketFormat
{

public:
	CFormatServer(void);
	~CFormatServer(void);
};

#endif
