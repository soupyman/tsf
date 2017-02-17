#include "StdAfx.h"

#include "Mpeg2Decoder.h"

 
extern "C"
{
extern vo_open_t vo_dxrgb_open;
extern vo_open_t vo_dx_open; 
}


CMpeg2Decoder::CMpeg2Decoder(void)
{
	output = NULL;
	mpeg2dec = NULL;
}

CMpeg2Decoder::~CMpeg2Decoder(void)
{
	FreeDatas();
}

void CMpeg2Decoder::InitDatas(void)
{
	FreeDatas();
      mpeg2dec = mpeg2_init ();
	output = vo_dxrgb_open();// vo_dx_open(); 
      if (output == NULL) 
	{
		TRACE ( "Can not open output\n");
      }
	new_pic_decode_out = 0;
	PacketIndex = 0;
	FramePeriod = 0;
	FrameCount = 0;
      //total_offset = 0;

}

void CMpeg2Decoder::FreeDatas(void)
{	
	if (output && (output->close))
		output->close (output);

	if(output)
		free(output);
	output = NULL;
	
	if(mpeg2dec)
	    mpeg2_close (mpeg2dec);
	mpeg2dec = NULL;
}

bool CMpeg2Decoder::DecodeESData (uint8_t * current, uint8_t * end)
{
    const mpeg2_info_t * info;
    mpeg2_state_t state;
    vo_setup_result_t setup_result;
 	static int sequence_count = 0;
    mpeg2_buffer (mpeg2dec, current, end);
    //total_offset += end - current;
    info = mpeg2_info (mpeg2dec);
    while (1) {
	state = mpeg2_parse (mpeg2dec);

	switch (state) {
	case STATE_BUFFER:
	    return 0;
	case STATE_SEQUENCE:
		FramePeriod = info->sequence->frame_period;
		sequence_count ++ ;
		TRACE ( " STATE_SEQUENCE times %d \n", sequence_count);
	    /* might set nb fbuf, convert format, stride */
	    /* might set fbufs */
	    if (output->setup (output, info->sequence->width,
			       info->sequence->height,
			       info->sequence->chroma_width,
			       info->sequence->chroma_height, &setup_result)) {
		TRACE ( "display setup failed\n");
		return (1);
	    }
	    if (setup_result.convert &&
		mpeg2_convert (mpeg2dec, setup_result.convert, NULL)) {
		TRACE ( "color conversion setup failed\n");
		return (1);
	    }
	    if (output->set_fbuf) {
		uint8_t * buf[3];
		void * id;

		mpeg2_custom_fbuf (mpeg2dec, 1);
		output->set_fbuf (output, buf, &id);
		mpeg2_set_buf (mpeg2dec, buf, id);
		output->set_fbuf (output, buf, &id);
		mpeg2_set_buf (mpeg2dec, buf, id);
	    } else if (output->setup_fbuf) {
		uint8_t * buf[3];
		void * id;

		output->setup_fbuf (output, buf, &id);
		mpeg2_set_buf (mpeg2dec, buf, id);
		output->setup_fbuf (output, buf, &id);
		mpeg2_set_buf (mpeg2dec, buf, id);
		output->setup_fbuf (output, buf, &id);
		mpeg2_set_buf (mpeg2dec, buf, id);
	    }
	    mpeg2_skip (mpeg2dec, (output->draw == NULL));
	    break;
	case STATE_PICTURE:
	    /* might skip */
	    /* might set fbuf 
	    if (output->set_fbuf) {
		uint8_t * buf[3];
		void * id;

		output->set_fbuf (output, buf, &id);
		mpeg2_set_buf (mpeg2dec, buf, id);
	    }
	    if (output->start_fbuf)
		output->start_fbuf (output, info->current_fbuf->buf,
				    info->current_fbuf->id);*/
	    break;
	case STATE_SLICE:
	case STATE_END:
	case STATE_INVALID_END:
	    /* draw current picture */
	    /* might free frame buffer */
		new_pic_decode_out = 1;
		FrameCount ++;
	    if (info->display_fbuf) {
		if (output->draw)
		{
		    	output->draw (output, info->display_fbuf->buf,  info->display_fbuf->id);
			//TRACE("draw a frame \n");;
		}
	    }
	    if (output->discard && info->discard_fbuf)
		output->discard (output, info->discard_fbuf->buf, info->discard_fbuf->id);
	    break;
	default:
	    break;
	}
    }
	return 1;
}

bool CMpeg2Decoder::DemuxPES(uint8_t * buf, uint8_t * end, int flags)
{
    static int mpeg1_skip_table[16] = {
	0, 0, 4, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

#define DEMUX_HEADER 0
#define DEMUX_DATA 1
#define DEMUX_SKIP 2
    static int state = DEMUX_SKIP;
    static int state_bytes = 0;
    static uint8_t head_buf[264];

    uint8_t * header;
    int bytes;
    int len;

#define NEEDBYTES(x)						\
    do {							\
	int missing;						\
								\
	missing = (x) - bytes;					\
	if (missing > 0) {					\
	    if (header == head_buf) {				\
		if (missing <= end - buf) {			\
		    memcpy (header + bytes, buf, missing);	\
		    buf += missing;				\
		    bytes = (x);				\
		} else {					\
		    memcpy (header + bytes, buf, end - buf);	\
		    state_bytes = bytes + end - buf;		\
		    return 0;					\
		}						\
	    } else {						\
		memcpy (head_buf, header, bytes);		\
		state = DEMUX_HEADER;				\
		state_bytes = bytes;				\
		return 0;					\
	    }							\
	}							\
    } while (0)

#define DONEBYTES(x)		\
    do {			\
	if (header != head_buf)	\
	    buf = header + (x);	\
    } while (0)

    if (flags & DEMUX_PAYLOAD_START)
	goto payload_start;
    switch (state) {
    case DEMUX_HEADER:
	if (state_bytes > 0) {
	    header = head_buf;
	    bytes = state_bytes;
	    goto continue_header;
	}
	break;
    case DEMUX_DATA:
	    DecodeESData (buf, end);
	    state_bytes -= end - buf;
    return 0;
    case DEMUX_SKIP:
	    state_bytes -= end - buf;
    return 0;
    default:
	    state = DEMUX_SKIP;
	    return 0;
    	
    }

    payload_start:
	header = buf;
	bytes = end - buf;
    continue_header:
	NEEDBYTES (4);
	if (header[0] || header[1] || (header[2] != 1)) { /* prefix not 001*/
		state = DEMUX_SKIP;
		return 0;
	}
	    if ((header[3] < 0xe0) || (header[3] > 0xef))
    	{
	    TRACE ("bad stream id %x\n", header[3]);
	    return 1;
    	}
		NEEDBYTES (7);
		if ((header[6] & 0xc0) == 0x80) {	/* mpeg2 */
		    NEEDBYTES (9);
		    len = 9 + header[8];
		    NEEDBYTES (len);
		    /* header points to the mpeg2 pes header */
		    if (header[7] & 0x80) {
			uint32_t pts, dts;

			pts = (((header[9] >> 1) << 30) |
			       (header[10] << 22) | ((header[11] >> 1) << 15) |
			       (header[12] << 7) | (header[13] >> 1));
			dts = (!(header[7] & 0x40) ? pts :
			       (uint32_t)(((header[14] >> 1) << 30) |
				(header[15] << 22) |
				((header[16] >> 1) << 15) |
				(header[17] << 7) | (header[18] >> 1)));
			mpeg2_tag_picture (mpeg2dec, pts, dts);
		    }
		} else {	/* mpeg1 */
		    int len_skip;
		    uint8_t * ptsbuf;

		    len = 7;
		    while (header[len - 1] == 0xff) {
			len++;
			NEEDBYTES (len);
			if (len > 23) {
			    TRACE ("too much stuffing\n");
			    break;
			}
		    }
		    if ((header[len - 1] & 0xc0) == 0x40) {
			len += 2;
			NEEDBYTES (len);
		    }
		    len_skip = len;
		    len += mpeg1_skip_table[header[len - 1] >> 4];
		    NEEDBYTES (len);
		    /* header points to the mpeg1 pes header */
		    ptsbuf = header + len_skip;
		    if ((ptsbuf[-1] & 0xe0) == 0x20) {
			uint32_t pts, dts;

			pts = (((ptsbuf[-1] >> 1) << 30) |
			       (ptsbuf[0] << 22) | ((ptsbuf[1] >> 1) << 15) |
			       (ptsbuf[2] << 7) | (ptsbuf[3] >> 1));
			dts = (((ptsbuf[-1] & 0xf0) != 0x30) ? pts :
			       (uint32_t)(((ptsbuf[4] >> 1) << 30) |
				(ptsbuf[5] << 22) | ((ptsbuf[6] >> 1) << 15) |
				(ptsbuf[7] << 7) | (ptsbuf[18] >> 1)));
			mpeg2_tag_picture (mpeg2dec, pts, dts);
		    }
		}
		DONEBYTES (len);
		bytes = 6 + (header[4] << 8) + header[5] - len;

		    DecodeESData (buf, end);
		    state = DEMUX_DATA;
		    state_bytes = bytes - (end - buf);
		    return 0;
}

/*
	return
	0: not finished;
	1: finished;
	2: scrambled.
*/
u8  CMpeg2Decoder::DecodeTsNextPic()
{
	u16  pPktID;

	while(PacketIndex < pTs->m_nTotalPacketNumber)
	{
		pPktID = pTs->GetPacketPID(PacketIndex);

		if(pPktID == video_pid)
		{
			pTs->ReadPacket(&(pkt),PacketIndex);
			if(pkt.GetScramblingControl())
			{
				return 2;
			}
			if(pkt.GetAdaptiationControl() & 0x1)
			{
				DemuxPES((pkt).GetPesAddress(), (u8*)((pkt).GetPktData()) + (pkt).GetPktSize(), (pkt).IsUnitStart());
				if(new_pic_decode_out)
				{
					new_pic_decode_out = 0;
					PacketIndex++;
					return 0;
				}
			}
		}
		PacketIndex++;
		
	}
	return 1;
}
