#include "StdAfx.h"
#include "include.h"

CStandardPacketFormat::CStandardPacketFormat(void)
{
}
	
CStandardPacketFormat::~CStandardPacketFormat(void)
{
}


CContentTree * CStandardPacketFormat::ParsePacketData(CESData * pES, u32 Indx)
{
	CPacket * pPkt = pES->GetPacket(Indx);
	if(false == pPkt->IsValid())
		return NULL;

	CString StrBuff;
	CContentTree *	 PktCnt ;
		
	StrBuff.Format("ES PID(0x%x)Packet Index(%x)",pPkt->GetPID(),Indx);
	PktCnt = AddPacketBranch(NULL,StrBuff,  (u64)pES, (u32)Indx);

	StrBuff.Format("Synchro Byte");
	AddDefaultBranch(PktCnt,StrBuff, 0, 8,pPkt->GetSynByte());
	
	StrBuff.Format("transport_error_indicator");
	AddDefaultBranch(PktCnt,StrBuff, 8, 1,pPkt->GetTransportErrorIndivator());

	StrBuff.Format("payload_unit_start_indicator");
	AddDefaultBranch(PktCnt,StrBuff, 9, 1, pPkt->GetPayLoadUnitStartIndicator());
	
	StrBuff.Format("transport_priority");
	AddDefaultBranch(PktCnt,StrBuff, 10, 1, pPkt->GetTransportPriority());

	StrBuff.Format("PID");
	AddDefaultBranch(PktCnt,StrBuff, 11, 13, pPkt->GetPID());
	
	StrBuff.Format("transport_scrambling_control");
	AddDefaultBranch(PktCnt,StrBuff, 24, 2, pPkt->GetScramblingControl());

	StrBuff.Format("adaptiation_field_control");
	AddDefaultBranch(PktCnt,StrBuff, 26, 2, pPkt->GetAdaptiationControl());
	
	StrBuff.Format("continuity_counter");
	AddDefaultBranch(PktCnt,StrBuff, 28, 4, pPkt->GetContinuityCounter());

	if(( pPkt->GetAdaptiationControl())&0x2)
	{
		ParseAdaptionData(pPkt,PktCnt);
	}
	if(( pPkt->GetAdaptiationControl())&0x1)
	{
		u8 PayLoadLen = pPkt->PacketSize -  pPkt->GetDataStartOffset();
		u8 * PayLoadData = ((u8 *) pPkt->GetPktData())+ pPkt->GetDataStartOffset();
		StrBuff.Format("PayLoad");
		//assert(PayLoadLen > VALUE_MAX_LENGTH_IN_BYTE);
		//AddDefaultBranch(PktCnt,StrBuff, 8*( pPkt->GetDataStartOffset()), PayLoadLen * 8, (u64)(PayLoadData));

		if(PayLoadLen == 0)
		{
		}
		else if(PayLoadLen <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < PayLoadLen ; i ++)
			{
				value <<= 8;
				value |= *(PayLoadData);
				PayLoadData ++;
			}
			AddDefaultBranch(PktCnt,StrBuff,8*( pPkt->GetDataStartOffset()),PayLoadLen*8, value);
		}
		else
		{
			AddDefaultBranch(PktCnt,StrBuff,8*( pPkt->GetDataStartOffset()),PayLoadLen*8, (u64)(PayLoadData));
		}
		
	}
	return PktCnt;
}

void CStandardPacketFormat::ParseAdaptionData(CPacket * pPkt ,CContentTree *	 pParent)
{
	if( pPkt->GetAdaptationFieldLength() > 0)
	{
		CString StrBuff;
		u16 BitIdx = 8*  pPkt->GetPktHeadSize();
		u8	discontinuity_indicator,random_access_indicator;
		u8	elementary_stream_priority_indicator,PCR_flag,OPCR_flag;
		u8 	splicing_point_flag,transport_private_data_flag,adaptation_field_extension_flag;
		
		u8 * pdata  = (u8 *)( pPkt->GetPktData()) +  pPkt->GetPktHeadSize();

		StrBuff.Format("AdaptationField");
		pParent = AddSubBodyBranch(pParent,StrBuff, BitIdx, 8*( pPkt->GetAdaptationFieldLength()+1));
		
		StrBuff.Format("adaptation_field_length");
		AddDefaultBranch(pParent,StrBuff, BitIdx, 8, pPkt->GetAdaptationFieldLength());
		
		pdata += 1;
		BitIdx += 8;
		
		discontinuity_indicator = (pdata[0]>>7) & 0x1;
		StrBuff.Format("discontinuity_indicator");
		AddDefaultBranch(pParent,StrBuff, BitIdx, 1,discontinuity_indicator);
	
		random_access_indicator = (pdata[0]>>6) & 0x1;
		StrBuff.Format("random_access_indicator");
		AddDefaultBranch(pParent,StrBuff, BitIdx+1, 1,random_access_indicator);
		
		elementary_stream_priority_indicator = (pdata[0]>>5) & 0x1;
		StrBuff.Format("elementary_stream_priority_indicator");
		AddDefaultBranch(pParent,StrBuff, BitIdx+2, 1,elementary_stream_priority_indicator);
		
		PCR_flag = (pdata[0]>>4) & 0x1;
		StrBuff.Format("PCR_flag");
		AddDefaultBranch(pParent,StrBuff, BitIdx+3, 1,PCR_flag);
		
		OPCR_flag = (pdata[0]>>3) & 0x1;
		StrBuff.Format("OPCR_flag");
		AddDefaultBranch(pParent,StrBuff, BitIdx+4, 1,OPCR_flag);
		
		splicing_point_flag = (pdata[0]>>2) & 0x1;
		StrBuff.Format("splicing_point_flag");
		AddDefaultBranch(pParent,StrBuff, BitIdx+5, 1,splicing_point_flag);
		
		transport_private_data_flag = (pdata[0]>>1) & 0x1;
		StrBuff.Format("transport_private_data_flag");
		AddDefaultBranch(pParent,StrBuff, BitIdx+6, 1,transport_private_data_flag);
		
		adaptation_field_extension_flag = pdata[0] & 0x1;
		StrBuff.Format("adaptation_field_extension_flag");
		AddDefaultBranch(pParent,StrBuff, BitIdx+7, 1,adaptation_field_extension_flag);
		pdata += 1;
		BitIdx += 8;
		
		if(PCR_flag == 1)
		{
			u64 value = (pdata[0]<<25) + (pdata[1]<<17) + (pdata[2]<<9) + (pdata[3]<<1) + (pdata[4]>>7);

			StrBuff.Format("program_clock_reference_base");
			//StrBuff.AppendFormat(LONGDATAFORMAT,MSD,LSD);
			AddDefaultBranch(pParent,StrBuff, BitIdx, 33,value);
			/*6 bit reserve */
			StrBuff.Format("program_clock_reference_extension");
			AddDefaultBranch(pParent,StrBuff, BitIdx+39, 9,(pdata[4]& 0x1)*256 + pdata[5]);
			pdata += 6;
			BitIdx += 48;
		}
		if(OPCR_flag == 1)
		{
			u64 value = (pdata[0]<<25) + (pdata[1]<<17) + (pdata[2]<<9) + (pdata[3]<<1) + (pdata[4]>>7);
			StrBuff.Format("original_program_clock_reference_base");
			/*u32 MSD = pdata[0]*256*256*256 + pdata[1]*256*256 + pdata[2]*256 + pdata[3];
			MSD >>= 7;
			u32 LSD = pdata[3]*256 + pdata[4];
			LSD >>= 7;
			LSD &= 0xFF;
			StrBuff.AppendFormat(LONGDATAFORMAT,MSD,LSD);*/
			AddDefaultBranch(pParent,StrBuff, BitIdx, 33,value);
			/*6 bit reserve */
			StrBuff.Format("original_program_clock_reference_extension");
			AddDefaultBranch(pParent,StrBuff, BitIdx+39, 9,(pdata[4]& 0x1)*256 + pdata[5]);
			pdata += 6;
			BitIdx += 48;
		}
		if(splicing_point_flag == 1)
		{
			StrBuff.Format("splice_countdown");
			AddDefaultBranch(pParent,StrBuff, BitIdx, 8, pdata[0]);
			pdata += 1;
			BitIdx += 8;
		}
		if(transport_private_data_flag == 1)
		{
			StrBuff.Format("transport_private_data_length");
			AddDefaultBranch(pParent,StrBuff, BitIdx, 8, pdata[0]);

			StrBuff.Format("private_data_byte(length)");
			AddDefaultBranch(pParent,StrBuff, BitIdx+8, pdata[0] * 8, pdata[0]);
			pdata += (1+pdata[0]);
			BitIdx += 8*(1+pdata[0]);
		}
		if(adaptation_field_extension_flag == 1)
		{
			u8 ltw_flag,piecewise_rate_flag,seamless_splice_flag;
			StrBuff.Format("adaptation_field_extension_length");
			AddDefaultBranch(pParent,StrBuff, BitIdx, 8, pdata[0]);
			
			ltw_flag = (pdata[1])&0x80;
			StrBuff.Format("ltw_flag");
			AddDefaultBranch(pParent,StrBuff, BitIdx+8, 1, ltw_flag);

			piecewise_rate_flag = (pdata[1])&0x40;
			StrBuff.Format("piecewise_rate_flag");
			AddDefaultBranch(pParent,StrBuff, BitIdx+9, 1, piecewise_rate_flag);

			seamless_splice_flag = (pdata[1])&0x20;
			StrBuff.Format("seamless_splice_flag");
			AddDefaultBranch(pParent,StrBuff, BitIdx+10, 1, seamless_splice_flag);
			// 5 bit reserved
			pdata += 2;
			BitIdx += 16;
			if(ltw_flag == 1)
			{
				StrBuff.Format("itw_valid_flag");
				AddDefaultBranch(pParent,StrBuff, BitIdx, 1, (pdata[0] >> 7));

				StrBuff.Format("itw_offset");
				AddDefaultBranch(pParent,StrBuff, BitIdx+1, 15, (pdata[0]&0x8F)*256 + pdata[1]);
				pdata += 2;
				BitIdx += 8*2;	
			}
			if(piecewise_rate_flag == 1)
			{
				// 2  bit reserved
				StrBuff.Format("piecewise_rate");
				AddDefaultBranch(pParent,StrBuff, BitIdx+2, 22, (pdata[0]&0x4F)*256*256 + pdata[1]*256 + pdata[2]);
				pdata += 3;
				BitIdx += 8*3;	
			}
			if(seamless_splice_flag == 1)
			{
				StrBuff.Format("splice_type");
				AddDefaultBranch(pParent,StrBuff, BitIdx, 4, (pdata[0] >> 4));

				StrBuff.Format("DTS_next_au[32-30]");
				AddDefaultBranch(pParent,StrBuff, BitIdx+4, 3, (pdata[0] >> 1)& 0x7);
				StrBuff.Format("marker_bit");
				AddDefaultBranch(pParent,StrBuff, BitIdx+7, 1, pdata[0] & 0x1);
				
				StrBuff.Format("DTS_next_au[29-15]");
				AddDefaultBranch(pParent,StrBuff, BitIdx+8, 15, (pdata[1] * 256 + pdata[2]) >> 1);
				StrBuff.Format("marker_bit");
				AddDefaultBranch(pParent,StrBuff, BitIdx+23, 1, pdata[2] & 0x1);

				StrBuff.Format("DTS_next_au[14-0]");
				AddDefaultBranch(pParent,StrBuff, BitIdx+24, 15, (pdata[3] * 256 + pdata[4]) >> 1);
				StrBuff.Format("marker_bit");
				AddDefaultBranch(pParent,StrBuff, BitIdx+39, 1, pdata[4] & 0x1);
				pdata += 5;
				BitIdx += 8*5;	
			}
			// other bytes is reserved
		}
		// other bytes is stuffing_byte
	}
}

