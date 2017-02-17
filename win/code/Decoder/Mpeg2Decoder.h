#pragma once

extern "C"
{
#include "mpeg2.h"
#include "VideoOut.h"
}
#include "typedefine.h"
#include "TSFile.h"
#include "Packet.h"
class CMpeg2Decoder 
{
public:
//#define BUFF_MAX    1024

#define DEMUX_PAYLOAD_START 1

	CMpeg2Decoder(void);
	~CMpeg2Decoder(void);
    void InitDatas(void);
    void FreeDatas(void);

    bool DemuxPES(uint8_t * buf, uint8_t * end, int flags);
    bool DecodeESData (uint8_t * current, uint8_t * end);
    u8 DecodeTsNextPic();

public:
        mpeg2dec_t * mpeg2dec; 
        vo_instance_t * output;


        CTSFile  * pTs;
        u16 video_pid;
        u8 new_pic_decode_out;
        CPacket pkt;
	  u32 PacketIndex ;
        u32 FramePeriod;
        u32 FrameCount;
        //u8 Buff[BUFF_MAX];
        //s32 total_offset ;
};


	
