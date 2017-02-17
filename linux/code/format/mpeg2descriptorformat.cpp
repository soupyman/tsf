
#include "mpeg2descriptorformat.h"



CMPEG2Descriptor::CMPEG2Descriptor(void)
{

}

CMPEG2Descriptor::~CMPEG2Descriptor(void)
{
}


/*DESCRIPTOR_VIDEO_STREAM*/
void CMPEG2Descriptor::ParseDescriptorVideoStream(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u8 private_data_lenth;
	u32 PrivateBaseIdx;
	u8 * private_data;
	u16	data_index = 2;

	pSLabel.Format("DESCRIPTOR_VIDEO_STREAM" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("multiple_frame_rate_flag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]>>7);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,1,pDescriptorData[data_index]>>7);

	pSLabel.Format("frame_rate_code");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[data_index]>>3)& 0xF);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+17,4,(pDescriptorData[data_index]>>3)& 0xF);
	u8 m2f = (pDescriptorData[data_index]>>2)& 0x1;
	pSLabel.Format("MPEG_1_only_flag");
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+21,1,m2f);

	pSLabel.Format("constrained_parameter_flag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[data_index]>>1)& 0x1);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+22,1,(pDescriptorData[data_index]>>1)& 0x1);

	pSLabel.Format("still_pic_flag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]& 0x1);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+23,1,pDescriptorData[data_index]& 0x1);

	if(m2f == 1)
	{
		pSLabel.Format("profile_and_level_indicater");
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+1]);

		pSLabel.Format("chroma_format");
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+32,2,pDescriptorData[data_index+2]>>6);

		pSLabel.Format("frame_rate_externsion_flag");
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+34,1,(pDescriptorData[data_index+2]>>5)&0x1);

		AddReserveBranch(CurRot  ,BaseBitIdx+35,5);
		private_data_lenth = descriptor_lenth - 3;
		private_data = pDescriptorData + 5;
		PrivateBaseIdx = BaseBitIdx+40;
	}	
	else
	{
		private_data_lenth = descriptor_lenth - 1;
		private_data = pDescriptorData + 3;
		PrivateBaseIdx = BaseBitIdx+24;
	}

		/* private data	*/
	if(private_data_lenth)
	{
		pSLabel.Format("private_data");
		pDescriptorData += 6;
		if(private_data_lenth <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < private_data_lenth ; i ++)
			{
				value <<= 8;
				value |= *private_data;
				private_data ++;
			}
			AddDefaultBranch(CurRot,pSLabel,PrivateBaseIdx,private_data_lenth*8, value);
		}
		else
		{
			AddDefaultBranch(CurRot,pSLabel,PrivateBaseIdx,private_data_lenth*8, (u64)private_data);
		}
	}

}
/*DESCRIPTOR_AUDIO_STREAM*/
void CMPEG2Descriptor::ParseDescriptorAudioStream(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

	pSLabel.Format("DESCRIPTOR_AUDIO_STREAM" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("free_format_flag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]>>7);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,1,pDescriptorData[data_index]>>7);

	pSLabel.Format("ID");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[data_index]>>6)&0x1);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+17,1,(pDescriptorData[data_index]>>6)&0x1);

	pSLabel.Format("layer");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[data_index]>>4)&0x3);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+18,2,(pDescriptorData[data_index]>>4)&0x3);

	pSLabel.Format("variable_rate_audio_indicator");
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+20,1,(pDescriptorData[data_index]>>3)&0x1);

	AddReserveBranch(CurRot  ,BaseBitIdx+21,3);

}
/*DESCRIPTOR_HIERARCHY*/
void CMPEG2Descriptor::ParseDescriptorHierarchy(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

	pSLabel.Format("DESCRIPTOR_HIERARCHY" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("hierarchy_type");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]& 0xF);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+20,4,pDescriptorData[data_index]& 0xF);

	pSLabel.Format("hierarchy_layer_index");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+1]&0x3F);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+26,6,pDescriptorData[data_index+1]&0x3F);

	pSLabel.Format("hierarchy_embedded_layer");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+2]&0x3F);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+34,6,pDescriptorData[data_index+2]&0x3F);

	pSLabel.Format("hierarchy_priority");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+3]&0x3F);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+42,6,pDescriptorData[data_index+3]&0x3F);
}
/*DESCRIPTOR_REGISTRATION*/
void CMPEG2Descriptor::ParseDescriptorRegistration(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

	pSLabel.Format("DESCRIPTOR_REGISTRATION" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	u32 fier = pDescriptorData[data_index]*256*256*256+pDescriptorData[data_index+1]*256*256
		+pDescriptorData[data_index+2]*256+pDescriptorData[data_index+3];
	pSLabel.Format("format_identifier");
	//PSLabeL.AppeNdFormat(DATAFORMAT,fier);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,32,fier);
	if(descriptor_lenth > 4)
	{
		pSLabel.Format("addtional_identification_info");
		//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth-4);
		//AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+48,8*(descriptor_lenth-4),descriptor_lenth-4);
		
		pDescriptorData += (data_index +4);
		if((descriptor_lenth - 4) <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < (descriptor_lenth - 4) ; i ++)
			{
				value <<= 8;
				value |= *(pDescriptorData);
				pDescriptorData ++;
			}
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+48,(descriptor_lenth - 4)*8, value);
		}
		else
		{
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+48,(descriptor_lenth - 4)*8, (u64)(pDescriptorData));
		}
	}
}
/*DESCRIPTOR_DATA_STREAM_ALIGNMENT*/
void CMPEG2Descriptor::ParseDescriptorDataStreamAlignment(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

	pSLabel.Format("DESCRIPTOR_DATA_STREAM_ALIGNMENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("alignment_type");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,8,pDescriptorData[data_index]);
}
/*DESCRIPTOR_CA*/
void CMPEG2Descriptor::ParseDescriptorCA(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
	
	pSLabel.Format("DESCRIPTOR_CA" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	
	pSLabel.Format("CA_system_ID");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[2]*256 + pDescriptorData[3] ));
	AddDefaultBranch(CurRot, pSLabel,BaseBitIdx+16,16 ,(pDescriptorData[2]*256 + pDescriptorData[3] ));
	
	pSLabel.Format("CA_PID");
	//PSLabeL.AppeNdFormat(DATAFORMAT,((pDescriptorData[4] & 0x1F)*256 + pDescriptorData[5] ));
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+35,13,((pDescriptorData[4] & 0x1F)*256 + pDescriptorData[5] ));
	/* private data	*/
	if(descriptor_lenth > 4)
	{
		pSLabel.Format("private_data");
		pDescriptorData += 6;
		if((descriptor_lenth - 4) <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < (descriptor_lenth - 4) ; i ++)
			{
				value <<= 8;
				value |= *(pDescriptorData);
				pDescriptorData ++;
			}
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+48,(descriptor_lenth - 4)*8, value);
		}
		else
		{
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+48,(descriptor_lenth - 4)*8, (u64)(pDescriptorData));
		}
		
		//AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+48,8*(descriptor_lenth-4),(u32)(pDescriptorData+6));
	}
}
/*DESCRIPTOR_ISO_639_LANGUAGE*/
void CMPEG2Descriptor::ParseDescriptorISO639Language(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
		
	pSLabel.Format("DESCRIPTOR_ISO_639_LANGUAGE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	u32 I6lc;
	for(int i =0 ; i < (descriptor_lenth-1); i+= 3)
	{
		I6lc = pDescriptorData[2+i]*256*256 + pDescriptorData[2+i+1]*256
			+ pDescriptorData[2+i+2];
		pSLabel.Format("ISO_639_language_code");
		//PSLabeL.AppeNdFormat(DATAFORMAT,I6lc);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,24,I6lc);
		BaseBitIdx += 24;
	}

	pSLabel.Format("audio_type");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[descriptor_lenth+2-1]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx , 8,pDescriptorData[descriptor_lenth+2-1]);
}

	/*DESCRIPTOR_TARGET_BACKGROUND_GRID*/
void CMPEG2Descriptor::ParseDescriptorTargetBackgroundGrid(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
			
	pSLabel.Format("DESCRIPTOR_TARGET_BACKGROUND_GRID" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("horizontal_size");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]*64+(pDescriptorData[data_index+1]>>2));
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16 , 14,pDescriptorData[data_index]*64+(pDescriptorData[data_index+1]>>2));

	pSLabel.Format("vertical_size");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[data_index+1]&0x3)<<12 +(pDescriptorData[data_index+2]<<4)+ (pDescriptorData[data_index+3]>>4));
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+30 , 14,
		((pDescriptorData[data_index+1]&0x3)<<12) +(pDescriptorData[data_index+2]<<4)+ (pDescriptorData[data_index+3]>>4));

	pSLabel.Format("pel_aspect_ratio");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+3]&0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+44 , 4,pDescriptorData[data_index+3]&0xF);
}
/*DESCRIPTOR_VIDEO_WINDOW*/
void CMPEG2Descriptor::ParseDescriptorVideoWindow(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
	pSLabel.Format("DESCRIPTOR_VIDEO_WINDOW" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("horizontal_offset");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]*64+(pDescriptorData[data_index+1]>>2));
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16 , 14,pDescriptorData[data_index]*64+(pDescriptorData[data_index+1]>>2));

	pSLabel.Format("vertical_offset");
	//PSLabeL.AppeNdFormat(DATAFORMAT,(pDescriptorData[data_index+1]&0x3)<<12 +(pDescriptorData[data_index+2]<<4)+ pDescriptorData[data_index+3]>>4);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+30 , 14,
		((pDescriptorData[data_index+1]&0x3)<<12) +(pDescriptorData[data_index+2]<<4)+ (pDescriptorData[data_index+3]>>4));

	pSLabel.Format("window_priority");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+3]&0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+44 , 4,pDescriptorData[data_index+3]&0xF);
}
/*DESCRIPTOR_SYSTEM_CLOCK*/
void CMPEG2Descriptor::ParseDescriptorSystemClock(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
	pSLabel.Format("DESCRIPTOR_SYSTEM_CLOCK" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("external_clock_reference_indicator");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]>>7);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16 , 1,pDescriptorData[data_index]>>7);

	pSLabel.Format("clock_accuracy_integer");
	//PSLabeL.AppeNdFormat(DATAFORMAT, pDescriptorData[data_index]&0x3F);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+18 , 6, pDescriptorData[data_index]&0x3F);

	pSLabel.Format("clock_accuracy_exponent");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+1]>>5);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+24 , 3,pDescriptorData[data_index+1]>>5);

	AddReserveBranch(CurRot, BaseBitIdx+27 , 5);
}
/*DESCRIPTOR_MULTIPLEX_BUFFER_UTILIZATION*/
void CMPEG2Descriptor::ParseDescriptorMultiplexBufferUtilization(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;

	pSLabel.Format("DESCRIPTOR_MULTIPLEX_BUFFER_UTILIZATION" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("mdv_valid_flag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index]>>7);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16 , 1,pDescriptorData[data_index]>>7);

	pSLabel.Format("multiplex_delay_variation");
	//PSLabeL.AppeNdFormat(DATAFORMAT, (pDescriptorData[data_index]&0x7F)*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+17 , 15,(pDescriptorData[data_index]&0x7F)*256 + pDescriptorData[data_index+1]);

	pSLabel.Format("multiplex_strategy");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pDescriptorData[data_index+2] >> 5);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+32 , 3,pDescriptorData[data_index+2] >> 5);
}
/*DESCRIPTOR_COPYRIGHT*/
void CMPEG2Descriptor::ParseDescriptorCopyright(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;

	pSLabel.Format("DESCRIPTOR_COPYRIGHT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	u32 idtfer = pDescriptorData[data_index]*256*256*256 + pDescriptorData[data_index+1]*256*256
		+ pDescriptorData[data_index+2] * 256 + pDescriptorData[data_index+3];
	pSLabel.Format("copyright_identifier");
	//PSLabeL.AppeNdFormat(DATAFORMAT,idtfer);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16 , 32,idtfer);

	pSLabel.Format("addtional_copyright_info");

	pDescriptorData += (data_index+4);
	if(descriptor_lenth <= 4)
	{
	}
	else if((descriptor_lenth -4) <= VALUE_MAX_LENGTH_IN_BYTE)
	{
		u64 value = 0;
		for(u8 i = 0; i < (descriptor_lenth -4) ; i ++)
		{
			value <<= 8;
			value |= *(pDescriptorData);
			pDescriptorData ++;
		}
		AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+48,(descriptor_lenth -4)*8, value);
	}
	else
	{
		AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+48,(descriptor_lenth -4)*8, (u64)(pDescriptorData));
	}
}
/*DESCRIPTOR_MAXIMUM_BITRATE*/
void CMPEG2Descriptor::ParseDescriptorMaximumBitrate(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
	
	pSLabel.Format("DESCRIPTOR_MAXIMUM_BITRATE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	u32 mbrt = (pDescriptorData[data_index]& 0x3F)*256*256 + pDescriptorData[data_index+1]*256
		+ pDescriptorData[data_index+2];
	pSLabel.Format("maximum_bitrate");
	//PSLabeL.AppeNdFormat(DATAFORMAT,mbrt);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+18 , 22,mbrt);
}
/*DESCRIPTOR_PRIVATE_DATA_INDICATOR*/
void CMPEG2Descriptor::ParseDescriptorPricateDataIndicator(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	//CContentTree * SubRot;
	pSLabel.Format("DESCRIPTOR_PRIVATE_DATA_INDICATOR" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	u32 pdi = pDescriptorData[data_index]*256*256*256 + pDescriptorData[data_index+1]*256*256
		+ pDescriptorData[data_index+2] * 256 + pDescriptorData[data_index+3];
	pSLabel.Format("private_data_indicator");
	//PSLabeL.AppeNdFormat(DATAFORMAT,pdi);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16 , 32,pdi);
}

/*DESCRIPTOR_DEFAULT*/
void CMPEG2Descriptor::ParseDescriptorDefault(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];

	pSLabel.Format("DESCRIPTOR_UNKOWN" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//PSLabeL.AppeNdFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	//for(int i = 0; i < descriptor_lenth ; i ++)
	pSLabel.Format("descriptor_data");
		//AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,descriptor_lenth * 8, (u32)(pDescriptorData+2));
	pDescriptorData += 2;
	if(descriptor_lenth == 0)
	{
	}
	else if((descriptor_lenth ) <= VALUE_MAX_LENGTH_IN_BYTE)
	{
		u64 value = 0;
		for(u8 i = 0; i < (descriptor_lenth ) ; i ++)
		{
			value <<= 8;
			value |= *(pDescriptorData);
			pDescriptorData ++;
		}
		AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+16,(descriptor_lenth )*8, value);
	}
	else
	{
		AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+16,(descriptor_lenth )*8, (u64)(pDescriptorData));
	}
}
u32 CMPEG2Descriptor::ParseMPEG2DescriptorContent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{		/*	descriptor is descripted in MPEG2_CHS p71		*/
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];

	switch(descriptor_tag)
	{
		case DESCRIPTOR_VIDEO_STREAM:
			ParseDescriptorVideoStream(parent, pDescriptorData,BaseBitIdx);		
			break;
		case DESCRIPTOR_AUDIO_STREAM:
			ParseDescriptorAudioStream(parent, pDescriptorData,BaseBitIdx);	
			break;
		case DESCRIPTOR_HIERARCHY:
			ParseDescriptorHierarchy(parent, pDescriptorData,BaseBitIdx);	
			break;
		case DESCRIPTOR_REGISTRATION:
			ParseDescriptorRegistration(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_DATA_STREAM_ALIGNMENT:
			ParseDescriptorDataStreamAlignment(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TARGET_BACKGROUND_GRID:
			ParseDescriptorTargetBackgroundGrid(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_VIDEO_WINDOW:
			ParseDescriptorVideoWindow(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_CA:
			ParseDescriptorCA(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_ISO_639_LANGUAGE:
			ParseDescriptorISO639Language(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SYSTEM_CLOCK:
			ParseDescriptorSystemClock(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_MULTIPLEX_BUFFER_UTILIZATION:
			ParseDescriptorMultiplexBufferUtilization(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_COPYRIGHT:
			ParseDescriptorCopyright(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_MAXIMUM_BITRATE:
			ParseDescriptorMaximumBitrate(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_PRIVATE_DATA_INDICATOR:
			ParseDescriptorPricateDataIndicator(parent, pDescriptorData,BaseBitIdx);
			break;			
		default:
			ParseDescriptorDefault(parent, pDescriptorData,BaseBitIdx);
		break;	
	}
	return descriptor_lenth;
}

