#pragma once

#include "formatparser.h"


class CStandardPacketFormat : public virtual CFormatParser
{
	public:
		CStandardPacketFormat(void);
		~CStandardPacketFormat(void);

		CContentTree *ParsePacket(CPacket *pPkt);
		CContentTree *ParsePacketRoot(CPacket *pPkt);
		CContentTree *ParsePacketHead(CPacket *pPkt, CContentTree 		*root);
		void ParsePacketData(CPacket *pPkt, CContentTree 	 *PktHed);
		void ParseAdaptionData(CPacket *pPkt , CContentTree 	 *pParent);

};
