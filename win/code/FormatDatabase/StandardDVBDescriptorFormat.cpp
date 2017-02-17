#include "StdAfx.h"
#include <assert.h>
#include "StandardDVBDescriptorFormat.h"



CDVBDescriptor::CDVBDescriptor(void)
{

}

CDVBDescriptor::~CDVBDescriptor(void)
{

}

void CDVBDescriptor::ParseDescriptorAnciliaryData(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	//u16	data_index = 2;
	
	pSLabel.Format("DESCRIPTOR_ANCILLARY_DATA" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,  descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel , BaseBitIdx+8,8,descriptor_lenth);
	
	pSLabel.Format("anciliary_data_identifier");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[2]);
	AddDefaultBranch(CurRot, pSLabel , BaseBitIdx+16,8,pDescriptorData[2] );
}

void CDVBDescriptor::ParseDescriptorAnnouncementSupport(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

	pSLabel.Format("DESCRIPTOR_ANNOUNCEMENT_SUPPORT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,  descriptor_lenth);

	pSLabel.Format("announcement_support_indicator");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[2]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16,16,   pDescriptorData[2]);

	BaseBitIdx += 32;
	data_index = 4;
	while((descriptor_lenth+2) > data_index)
	{
		pSLabel.Format("announcement_type");
		//pSLabel.AppendFormat(DATAFORMAT,(pDescriptorData[data_index] & 0xF0));
		SubRot = AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,4 ,
		(pDescriptorData[data_index] & 0xF0));

		u8 ref_type = pDescriptorData[data_index] & 0x7;
		pSLabel.Format("reference_type");
		//pSLabel.AppendFormat(DATAFORMAT,ref_type);
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+5,3,ref_type);
		data_index += 1;
		BaseBitIdx += 8;
		if((ref_type == 1)||(ref_type == 2)||(ref_type == 3))
		{
			pSLabel.Format("original_network_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1] );
			AddDefaultBranch(SubRot,pSLabel ,BaseBitIdx , 16,
				pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1] );
			
			pSLabel.Format("transport_stream_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2] * 256 + pDescriptorData[data_index+3]);
			AddDefaultBranch(SubRot,pSLabel ,BaseBitIdx+16, 16,
				pDescriptorData[data_index+2] * 256 + pDescriptorData[data_index+3]);

			pSLabel.Format("transport_descriptors_length");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+4] * 256 + pDescriptorData[data_index+5]);
			AddDefaultBranch(SubRot,pSLabel ,BaseBitIdx+32, 16,
				pDescriptorData[data_index+4] * 256 + pDescriptorData[data_index+5]);
			
			pSLabel.Format("component_tag");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+6]);
			AddDefaultBranch(SubRot,pSLabel ,BaseBitIdx+48, 8,
			pDescriptorData[data_index+6]);

			data_index += 7;
			BaseBitIdx += 56;
		}
	}
}

void CDVBDescriptor::ParseDescriptorBouquetName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	//u16	data_index = 2;
	
	pSLabel.Format("DESCRIPTOR_BOUQUET_NAME" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,descriptor_tag);

	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("bouquet_name");
	//while(data_index < (descriptor_lenth+2))
	{
		//pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
	}
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+16,descriptor_lenth * 8,
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+2));
}

void CDVBDescriptor::ParseDescriptorCAIdentifier(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
		
	pSLabel.Format("DESCRIPTOR_CA_IDENTIFIER" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("CA_system_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,
			pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		BaseBitIdx += 16;
		data_index += 2;
	}
}
/*DESCRIPTOR_CELL_FREQUENCY_LINK*/
void CDVBDescriptor::ParseDescriptorCellFrequencyLine(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
		
	pSLabel.Format("DESCRIPTOR_CELL_FREQUENCY_LINK" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	u32 CellFreq;
	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("cell_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		SubRot = AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,
			pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);

		
		CellFreq = pDescriptorData[data_index+2] * 256 * 256 * 256 + pDescriptorData[data_index+3] * 256 * 256 ;
		CellFreq += pDescriptorData[data_index+4] * 256 + pDescriptorData[data_index+5];
		pSLabel.Format("frequency");
		//pSLabel.AppendFormat(DATAFORMAT,CellFreq);
		AddDefaultBranch(SubRot, pSLabel  ,BaseBitIdx+16,32,CellFreq);

		pSLabel.Format("subcell_info_loop_lenth");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+6]);
		AddDefaultBranch(SubRot, pSLabel  ,BaseBitIdx+48,8,
			pDescriptorData[data_index+6]);
		
		for(int i = 0 ; i < pDescriptorData[data_index+6]; i += 5)
		{
			pSLabel.Format("cell_id_extension");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+6+1+i]);
			AddDefaultBranch(SubRot, pSLabel  ,BaseBitIdx+56+i*8,8,
				pDescriptorData[data_index+6+1+i]);
			
			CellFreq = pDescriptorData[data_index+6+2+i] * 256 * 256 * 256 + pDescriptorData[data_index+6+3+i] * 256 * 256 ;
			CellFreq += pDescriptorData[data_index+6+4+i] * 256 + pDescriptorData[data_index+6+5+i];
			pSLabel.Format("transposer_frequency");
			//pSLabel.AppendFormat(DATAFORMAT,CellFreq);
			AddDefaultBranch(SubRot, pSLabel  ,BaseBitIdx+64+i*8,32,
				CellFreq);
		}
		
		BaseBitIdx += 56 + 8*pDescriptorData[data_index+6];
		data_index += 7 + pDescriptorData[data_index+6];
	}
}

/*DESCRIPTOR_CELL_LIST*/
void CDVBDescriptor::ParseDescriptorCellList(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_CELL_LIST" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("cell_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		SubRot = AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,16,
			pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		data_index += 2;
		pSLabel.Format("cell_latitude");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+16,16,
			pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		data_index += 2;
		pSLabel.Format("cell_longitude");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+32,16,
			pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
		data_index += 2;
		pSLabel.Format("cell_extend_of_latitude");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + (pDescriptorData[data_index+1]>>4));
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+48,12,
			pDescriptorData[data_index] * 256 + (pDescriptorData[data_index+1]>>4));

		pSLabel.Format("cell_extend_of_longitude");
		//pSLabel.AppendFormat(DATAFORMAT,(pDescriptorData[data_index+1] & 0xF) * 256 + pDescriptorData[data_index+2]);
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+60,12,
			(pDescriptorData[data_index+1] & 0xF) * 256 + pDescriptorData[data_index+2]);
		data_index += 3;
		pSLabel.Format("subcell_info_loop_length");
		u8 LoopLen = pDescriptorData[data_index];
		//pSLabel.AppendFormat(DATAFORMAT,LoopLen);
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+72,8,LoopLen);
		data_index += 1;
		BaseBitIdx += 80;
		for(int i = 0 ; i < LoopLen; i += 8)
		{
			pSLabel.Format("cell_id_extension");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
			SubRot = AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,pDescriptorData[data_index]);
			data_index += 1;
			pSLabel.Format("subcell_latitude");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
			AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+8,16,
				pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
			data_index += 2;
			pSLabel.Format("subcell_longitude");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
			AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+24,16,
				pDescriptorData[data_index] * 256 + pDescriptorData[data_index+1]);
			data_index += 2;
			pSLabel.Format("subcell_extend_of_latitude");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] * 256 + (pDescriptorData[data_index+1]>>4));
			AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+40,12,
				pDescriptorData[data_index] * 256 + (pDescriptorData[data_index+1]>>4));

			pSLabel.Format("subcell_extend_of_longitude");
			//pSLabel.AppendFormat(DATAFORMAT,(pDescriptorData[data_index+1] & 0xF) * 256 + pDescriptorData[data_index+2]);
			AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx+52,12,
				(pDescriptorData[data_index+1] & 0xF) * 256 + pDescriptorData[data_index+2]);
			data_index += 3;
			BaseBitIdx += 64;
		}
	}
}

/*DESCRIPTOR_COMPONENT*/
void CDVBDescriptor::ParseDescriptorComponent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_COMPONENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	
	pSLabel.Format("stream_content");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]& 0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+20,4,pDescriptorData[data_index]& 0xF);

	pSLabel.Format("component_type");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+24,8,pDescriptorData[data_index+1]);

	pSLabel.Format("component_tag");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+32,8,pDescriptorData[data_index+2]);

	pSLabel.Format("ISO 639-2_language_code");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+40,24
		,pDescriptorData[data_index+3]*256*256 + pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);

	if(descriptor_lenth > 6)
	{
		data_index += 6;
		pSLabel.Format("text_char");
		AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+64,(descriptor_lenth - 6)*8,
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));
	}
}

/*DESCRIPTOR_CONTENT*/
void CDVBDescriptor::ParseDescriptorContent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_CONTENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("content_nibble_level_1");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]>>4);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,4,pDescriptorData[data_index]>>4);
		pSLabel.Format("content_nibble_level_2");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]&0xF);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+4,4,pDescriptorData[data_index]&0xF);
		
		pSLabel.Format("user_nibble");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]>>4);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,4,pDescriptorData[data_index+1]>>4);
		pSLabel.Format("user_nibble");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]&0xF);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+12,4,pDescriptorData[data_index+1]&0xF);
		
		BaseBitIdx += 16;
		data_index += 2;
	}
}

/*DESCRIPTOR_COUNTRY_AVAILABILITY*/
void CDVBDescriptor::ParseDescriptorCountryAvailability(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

				
	pSLabel.Format("DESCRIPTOR_COUNTRY_AVAILABILITY" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("country_availability_flag");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]>>7);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16,1,pDescriptorData[data_index]>>7);
	BaseBitIdx += 24;
	data_index ++;
	u32 country_code;
	while(data_index < (descriptor_lenth+2))
	{
		country_code = pDescriptorData[data_index]*256*256+\
			pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2];
		pSLabel.Format("country_code");
		//pSLabel.AppendFormat(DATAFORMAT,country_code);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,24,country_code);
		BaseBitIdx += 24;
		data_index += 3;
	}
	
}
/*DESCRIPTOR_DATA_BROADCAST*/
void CDVBDescriptor::ParseDescriptorDataBroadcast(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_DATA_BROADCAST" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("data_broadcast_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);

	pSLabel.Format("component_tag");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+32,8,pDescriptorData[data_index+2]);

	u8 Lenth = pDescriptorData[data_index+3];
	pSLabel.Format("selector_length");
	//pSLabel.AppendFormat(DATAFORMAT,Lenth);
	SubRot = AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+40,8,Lenth);
	BaseBitIdx += 48;
	data_index += 4;
	for(int i = 0 ; i < Lenth ; i++)
	{
		pSLabel.Format("Selector_type");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
		AddDefaultBranch(SubRot, pSLabel ,BaseBitIdx,8,pDescriptorData[data_index]);
		BaseBitIdx += 8;
		data_index ++;
	}
	u32 LanCode = pDescriptorData[data_index]*256*256\
		+ pDescriptorData[data_index+1]*256 +pDescriptorData[data_index +2];
	pSLabel.Format("ISO 639-2_language_code");
	//pSLabel.AppendFormat(DATAFORMAT,LanCode);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,24,LanCode);

	Lenth = pDescriptorData[data_index+3];
	pSLabel.Format("text_length");
	//pSLabel.AppendFormat(DATAFORMAT,Lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+24,8,Lenth);
	BaseBitIdx += 32;
	data_index += 4;

	pSLabel.Format("text_char ");
	//for(int i = 0 ; i < Lenth ; i ++)
	{
		//pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
	}
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx,8*Lenth,CContentFieldNode::FIELD_FMT_STR,
		(u64)(pDescriptorData + data_index ));
}

/*DESCRIPTOR_DATA_BROADCAST_ID*/
void CDVBDescriptor::ParseDescriptorDataBroadcastID(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;

				
	pSLabel.Format("DESCRIPTOR_DATA_BROADCAST_ID" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("data_broadcast_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	BaseBitIdx += 32;
	data_index += 2;
	
	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("id_selector_byte");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index++]);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,pDescriptorData[data_index++]);
		BaseBitIdx += 8;
	}
}
/*DESCRIPTOR_CABLE_DELIVERY_SYSTEM*/
void CDVBDescriptor::ParseDescriptorCableDeliverySystem(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u32 temp;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_CABLE_DELIVERY_SYSTEM" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	
	pSLabel.Format("frequency(4.4 BCD code)");
	temp = (pDescriptorData[data_index]<<24) + (pDescriptorData[data_index+1]<<16 )
		 + (pDescriptorData[data_index+2]<<8) + (pDescriptorData[data_index+3]);
	//pSLabel.AppendFormat("%d%d%d%d.%d%d%d%d MHZ",pDescriptorData[data_index]>>4,pDescriptorData[data_index]&0xF,
		//pDescriptorData[data_index+1]>>4,pDescriptorData[data_index+1]&0xF,pDescriptorData[data_index+2]>>4,
		//pDescriptorData[data_index+2]&0xF,pDescriptorData[data_index+3]>>4,pDescriptorData[data_index+3]&0xF);
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+16,32, CContentFieldNode::FIELD_FMT_BCD , temp);

	pSLabel.Format("FEC_outer");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +5]& 0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+60,4,pDescriptorData[data_index +5]& 0xF);

	pSLabel.Format("modulation(1:16 QAM)(2:32 QAM)(3:64 QAM)(4:128 QAM)(5:256 QAM)");
	/*pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +6]);
	switch(pDescriptorData[data_index +6])
	{
		case 0x1:
			pSLabel.AppendFormat("(16 QAM)");
		break;
		case 0x2:
			pSLabel.AppendFormat("(32 QAM)");
		break;
		case 0x3:
			pSLabel.AppendFormat("(64 QAM)");
		break;
		case 0x4:
			pSLabel.AppendFormat("(128 QAM)");
		break;
		case 0x5:
			pSLabel.AppendFormat("(256 QAM)");
		break;
		default:
			pSLabel.AppendFormat("(unknown QAM)");
		break;
	}*/
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+64,8,pDescriptorData[data_index +6]);

	pSLabel.Format("symbol_rate(3.4 BCD code)");
	temp = (pDescriptorData[data_index+7]<<20) + (pDescriptorData[data_index+8]<<12)
		+ (pDescriptorData[data_index+9]<<4) + (pDescriptorData[data_index+10]>>4);
	//pSLabel.AppendFormat("%d%d%d.%d%d%d%d",pDescriptorData[data_index+7]>>4,pDescriptorData[data_index+7]&0xF, \
		//pDescriptorData[data_index+8]>>4,pDescriptorData[data_index+8]&0xF,pDescriptorData[data_index+9]>>4,\
		//pDescriptorData[data_index+9]&0xF,pDescriptorData[data_index+10]>>4);
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+72,28, CContentFieldNode::FIELD_FMT_BCD , temp);

	
	pSLabel.Format("FEC_inner");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +10]& 0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+100,4,pDescriptorData[data_index +10]& 0xF);
}
/*DESCRIPTOR_SATELLITE_DELIVERY_SYSTEM*/
void CDVBDescriptor::ParseDescriptorSatelliteDeliverySystem(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	u32 temp;
				
	pSLabel.Format("DESCRIPTOR_SATELLITE_DELIVERY_SYSTEM" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);
	
	pSLabel.Format("frequency(3.5 BCD code)");
	temp = (pDescriptorData[data_index]<<24) + (pDescriptorData[data_index+1]<<16) 
		 + (pDescriptorData[data_index+2]<<8) + (pDescriptorData[data_index+3]);

	//pSLabel.AppendFormat("%d%d%d.%d%d%d%d%d GHZ",pDescriptorData[data_index]>>4,pDescriptorData[data_index]&0xF,\
		//pDescriptorData[data_index+1]>>4,pDescriptorData[data_index+1]&0xF,pDescriptorData[data_index+2]>>4,\
		//pDescriptorData[data_index+2]&0xF,pDescriptorData[data_index+3]>>4,pDescriptorData[data_index+3]&0xF);
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+16,32,CContentFieldNode::FIELD_FMT_BCD  , temp);

	pSLabel.Format("orbital_position");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +4]*256 + pDescriptorData[data_index +5]);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+48,16,pDescriptorData[data_index +4]*256 + pDescriptorData[data_index +5]);

	pSLabel.Format("west_east_flag");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +6]>>7);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+64,1,pDescriptorData[data_index +6]>>7);

	pSLabel.Format("polarization");
	//pSLabel.AppendFormat(DATAFORMAT,(pDescriptorData[data_index +6]>>5)& 0x3);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+65,2,(pDescriptorData[data_index +6]>>5)& 0x3);
	
	pSLabel.Format("modulation(1:QPSK)");
	/*pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +6] & 0x1F);
	switch(pDescriptorData[data_index +6] & 0x1F)
	{
		case 0x1:
			pSLabel.AppendFormat("(QPSK)");
		break;
		default:
			pSLabel.AppendFormat("(unknown QAM)");
		break;
	}*/
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+67,5,pDescriptorData[data_index +6] & 0x1F);

	pSLabel.Format("symbol_rate(3.4 BCD code)");
	temp = (pDescriptorData[data_index+7]<<20) + (pDescriptorData[data_index+8]<<12)
		+ (pDescriptorData[data_index+9]<<4) + (pDescriptorData[data_index+10]>>4);

	//pSLabel.AppendFormat("%d%d%d.%d%d%d%d",pDescriptorData[data_index+7]>>4,pDescriptorData[data_index+7]&0xF,\
		//pDescriptorData[data_index+8]>>4,pDescriptorData[data_index+8]&0xF,pDescriptorData[data_index+9]>>4,\
		//pDescriptorData[data_index+9]&0xF,pDescriptorData[data_index+10]>>4);
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+72,28,CContentFieldNode::FIELD_FMT_BCD, temp);

	
	pSLabel.Format("FEC_inner");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index +10]& 0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+100,4,pDescriptorData[data_index +10]& 0xF);
}
/*DESCRIPTOR_TERRESTRIAL_DELIVERY_SYSTEM*/
void CDVBDescriptor::ParseDescriptorTerrestrialDeliverySystem(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_TERRESTRIAL_DELIVERY_SYSTEM" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag");
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+8,8,descriptor_lenth);

	u32 CenFreq = pDescriptorData[data_index]* 256*256*256 + pDescriptorData[data_index+1]*256*256 
		+pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3];
	pSLabel.Format("centre_frequency(*10HZ)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16,32,CenFreq);

	pSLabel.Format("bandwidth(0-8 MHZ)(1-7 MHZ)(2-6 MHZ)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+48,3,pDescriptorData[data_index+4]>>5);

	pSLabel.Format("constellation(0-QPSK)(1-16 QAM)(2-64 QAM)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+56,2,pDescriptorData[data_index+5]>>6);

	pSLabel.Format("hierarchy_information");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+58,3,(pDescriptorData[data_index+5]>>3)&0x7);

	pSLabel.Format("code_rate-HP_stream(0-1/2)(1-2/3)(2-3/4)(3-5/6)(4-7/8)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+61,3 , pDescriptorData[data_index+5]&0x7);

	pSLabel.Format("code_rate-LP_stream(0-1/2)(1-2/3)(2-3/4)(3-5/6)(4-7/8)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+64,3,pDescriptorData[data_index+6]>>5);

	pSLabel.Format("guard_interval(0-1/32)(1-1/16)(2-1/8)(3-1/4)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+67,2,(pDescriptorData[data_index+6]>>3) & 0x3);

	pSLabel.Format("transmission_mode(0-2K)(1-8k)");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+69,2,(pDescriptorData[data_index+6]>>1) & 0x3);

	pSLabel.Format("other_frequency_flag");
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+71,1,pDescriptorData[data_index+6] & 0x1);

	AddReserveBranch(CurRot ,BaseBitIdx+72,32);
}

/*DESCRIPTOR_DSNG*/
void CDVBDescriptor::ParseDescriptorDSNG(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_DSNG" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("byte");
	/*while(data_index < (descriptor_lenth+2))
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
	}*/
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+16,descriptor_lenth*8,
		CContentFieldNode::FIELD_FMT_STR ,(u64)(pDescriptorData+data_index) );

}
/*DESCRIPTOR_EXTENDED_EVENT*/
void CDVBDescriptor::ParseDescriptorExtendedEvent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_EXTENDED_EVENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("descriptor_number");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]>>4);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+16,4,pDescriptorData[data_index]>>4);

	pSLabel.Format("last_descriptor_number");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] & 0xF);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+20,4,pDescriptorData[data_index] & 0xF);

	u32 LanCode = pDescriptorData[data_index+1]*256*256+pDescriptorData[data_index+2]*256\
		+pDescriptorData[data_index+3];
	pSLabel.Format("ISO 639-2_language_code");
	//pSLabel.AppendFormat(DATAFORMAT,LanCode);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+24,24,LanCode);

	u8 lenOfItems = pDescriptorData[data_index+4];
	pSLabel.Format("length_of_items");
	//pSLabel.AppendFormat(DATAFORMAT,lenOfItems);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx+48,8,lenOfItems);
	data_index += 5;
	BaseBitIdx += 56;
	u8 lenTmp;
	while(lenOfItems >0)
	{
		lenTmp = pDescriptorData[data_index];
		pSLabel.Format("item_description_length");
		//pSLabel.AppendFormat(DATAFORMAT,lenTmp);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,lenTmp);
		data_index++;
		pSLabel.Format("Item_description_char");
		/*for(int i=0;i < lenTmp; i++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+8,lenTmp*8,
			CContentFieldNode::FIELD_FMT_STR ,(u64)(pDescriptorData+data_index));
		data_index += lenTmp;
		BaseBitIdx += lenTmp*8 + 8;
		lenOfItems -= (lenTmp+1);
		
		lenTmp = pDescriptorData[data_index];
		pSLabel.Format("item_length");
		//pSLabel.AppendFormat(DATAFORMAT,lenTmp);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,lenTmp);
		data_index++;
		pSLabel.Format("Item_char");
		/*for(int i=0;i < lenTmp; i++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+8,lenTmp*8,
			CContentFieldNode::FIELD_FMT_STR ,(u64)(pDescriptorData+data_index));
		data_index += lenTmp;
		BaseBitIdx += lenTmp*8 + 8;
		lenOfItems -= (lenTmp+1);
	}
	lenTmp = pDescriptorData[data_index];
	pSLabel.Format("text_length");
	//pSLabel.AppendFormat(DATAFORMAT,lenTmp);
	AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,lenTmp);
	data_index++;
	pSLabel.Format("text_char");
	/*for(int i=0;i < lenTmp; i++)
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
	}*/
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+8,lenTmp*8,
		CContentFieldNode::FIELD_FMT_STR ,(u64)(pDescriptorData+data_index));
}

/*DESCRIPTOR_FREQUENCY_LIST*/
void CDVBDescriptor::ParseDescriptorFrequencyList(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_FREQUENCY_LIST" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("coding_type");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]& 0x3);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+22,2,pDescriptorData[data_index]& 0x3);
	data_index++;
	BaseBitIdx += 24;
	u32 Freq ;
	while(data_index < (descriptor_lenth+2))
	{
		Freq = pDescriptorData[data_index]*256*256*256+pDescriptorData[data_index+1]*256*256\
			+pDescriptorData[data_index+2]*256 +pDescriptorData[data_index+3];
		pSLabel.Format("centre_frequency");
		//pSLabel.AppendFormat(DATAFORMAT,Freq);
		AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,32,Freq);
		BaseBitIdx += 32;
		data_index += 4;
	}
}

/*DESCRIPTOR_LINKAGE*/
void CDVBDescriptor::ParseDescriptorLinkage(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_LINKAGE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	pSLabel.Format("transport_stream_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	data_index += 2;
	BaseBitIdx += 16;
	pSLabel.Format("original_network_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	data_index += 2;
	BaseBitIdx += 16;
	pSLabel.Format("service_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	data_index += 2;
	BaseBitIdx += 16;
	pSLabel.Format("linkage_type");
	u8 LinkgType = pDescriptorData[data_index];
	//pSLabel.AppendFormat(DATAFORMAT,LinkgType);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,LinkgType);
	data_index += 1;
	BaseBitIdx += 8;
	if(LinkgType == 8)
	{
		u8 HandOverType = pDescriptorData[data_index]>>4 ;
		u8 OrgType = pDescriptorData[data_index] & 0x1 ;
		pSLabel.Format("hand-over_type");
		//pSLabel.AppendFormat(DATAFORMAT,HandOverType);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,4,HandOverType);
		pSLabel.Format("origin_type");
		//pSLabel.AppendFormat(DATAFORMAT,OrgType);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+7,1,OrgType);
		data_index += 1;
		BaseBitIdx += 8;
		if((HandOverType == 0x1)||(HandOverType == 0x2)||(HandOverType == 0x3))
		{
			pSLabel.Format("network_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
			data_index += 2;
			BaseBitIdx += 16;
		}
		if(OrgType == 0x00)
		{
			pSLabel.Format("initial_service_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
			data_index += 2;
			BaseBitIdx += 16;
		}
		pSLabel.Format("private_data");
		//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth+2-data_index);
		//AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8*(descriptor_lenth+2-data_index),(u64)(pDescriptorData+data_index));
		pDescriptorData += data_index;
		if((descriptor_lenth+2) <= data_index )
		{
		}
		else if((descriptor_lenth+2-data_index) <= VALUE_MAX_LENGTH_IN_BYTE )
		{
			u64 value = 0;
			for(u8 i = 0; i < (descriptor_lenth+2-data_index) ; i ++)
			{
				value <<= 8;
				value |= *(pDescriptorData);
				pDescriptorData ++;
			}
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx,(descriptor_lenth+2-data_index)*8, value);
		}
		else
		{
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx,(descriptor_lenth+2-data_index)*8, (u64)(pDescriptorData));
		}
	}
	else
	{
		pSLabel.Format("private_data");
		//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth+2-data_index);
		//AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8*(descriptor_lenth+2-data_index),(u64)(pDescriptorData+data_index));
		pDescriptorData += data_index;
		if((descriptor_lenth+2) <= data_index )
		{
		}
		else if((descriptor_lenth+2-data_index) <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < (descriptor_lenth+2-data_index) ; i ++)
			{
				value <<= 8;
				value |= *(pDescriptorData);
				pDescriptorData ++;
			}
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx,(descriptor_lenth+2-data_index)*8, value);
		}
		else
		{
			AddDefaultBranch(CurRot,pSLabel,BaseBitIdx,(descriptor_lenth+2-data_index)*8, (u64)(pDescriptorData));
		}
	}
	
}
/*DESCRIPTOR_LOCAL_TIME_OFFSET*/
void CDVBDescriptor::ParseDescriptorLocalTimeOffset(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	u32 temp;
			
	pSLabel.Format("DESCRIPTOR_LOCAL_TIME_OFFSET" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	while(data_index < (descriptor_lenth+2))
	{
		u32 U32Temp = pDescriptorData[data_index]*256*256+\
			pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2];
		pSLabel.Format("country_code");
		//pSLabel.AppendFormat(DATAFORMAT,U32Temp);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,U32Temp);

		pSLabel.Format("country_region_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]>>2);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,6,pDescriptorData[data_index+3]>>2);

		pSLabel.Format("local_time_offset_polarity");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3] & 0x1);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+31,1,pDescriptorData[data_index+3] & 0x1);

		pSLabel.Format("local_time_offset(2:2 BCD code)");
		temp = (pDescriptorData[data_index+4]<<8) + pDescriptorData[data_index+5];
		
		//pSLabel.AppendFormat(" : %d%d:%d%d",pDescriptorData[data_index+4]>>4,pDescriptorData[data_index+4]&0xF,
		//	pDescriptorData[data_index+5]>>4 ,pDescriptorData[data_index+5] & 0xF );
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+32,16,CContentFieldNode::FIELD_FMT_BCD, temp);

		pSLabel.Format("date_of_change(MJD code)");
		//AppendMJDTime(pDescriptorData[data_index+6]*256 + pDescriptorData[data_index+7] , pSLabel);
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+48,16,
			CContentFieldNode::FIELD_FMT_MJD,pDescriptorData[data_index+6]*256 + pDescriptorData[data_index+7] );

		
		pSLabel.Format("time_of_change(2:2:2 BCD code)");
		temp = (pDescriptorData[data_index+8]<<16) + (pDescriptorData[data_index+9]<<8) + pDescriptorData[data_index+10];

		//pSLabel.AppendFormat("%d%d:%d%d:%d%d",pDescriptorData[8]>>4, pDescriptorData[8] & 0xF,\
			//pDescriptorData[9]>>4, pDescriptorData[9] & 0xF,pDescriptorData[10]>>4, pDescriptorData[10] & 0xF);
		AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+48+16,24,CContentFieldNode::FIELD_FMT_BCD, temp);

		pSLabel.Format("next_time_offset(2:2 BCD code)");
		temp = (pDescriptorData[data_index+11]<<8) + (pDescriptorData[data_index+12]);

		//pSLabel.AppendFormat(" : %d%d:%d%d",pDescriptorData[data_index+11]>>4,pDescriptorData[data_index+11]&0xF,
			//pDescriptorData[data_index+12]>>4 ,pDescriptorData[data_index+12] & 0xF );
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+88,16,CContentFieldNode::FIELD_FMT_BCD, temp);	

		BaseBitIdx += 104;
		data_index += 13;
	}
}

/*DESCRIPTOR_MOSAIC*/
void CDVBDescriptor::ParseDescriptorMosaic(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
	u8 cell_linkage_info;
	pSLabel.Format("DESCRIPTOR_MOSAIC" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	
	pSLabel.Format("mosaic_entry_point");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]>>7);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,1,pDescriptorData[data_index]>>7);

	pSLabel.Format("number_of_horizontal_elementary_cells");
	//pSLabel.AppendFormat(DATAFORMAT,(pDescriptorData[data_index]>>4)&0x7);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+17,3,(pDescriptorData[data_index]>>4)&0x7);
	pSLabel.Format("number_of_vertical_elementary_cells");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] & 0x7);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+21,3,pDescriptorData[data_index] & 0x7);
	data_index ++;
	BaseBitIdx += 24;
	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("logical_cell_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]>>2);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,6,pDescriptorData[data_index]>>2);
		
		pSLabel.Format("logical_cell_presentation_info");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1] & 0x7);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+13,3,pDescriptorData[data_index+1] & 0x7);

		pSLabel.Format("elementary_cell_field_length");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,8,pDescriptorData[data_index+2]);

		for(int i = 0 ; i < pDescriptorData[data_index+2] ; i++)
		{
			pSLabel.Format("elementary_cell_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3+i]&0x3F);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24+i*8+2 ,6,pDescriptorData[data_index+3+i]&0x3F);
		}
		BaseBitIdx += 24+pDescriptorData[data_index+2]*8;
		data_index += 3+pDescriptorData[data_index+2];
		
		pSLabel.Format("cell_linkage_info");
		cell_linkage_info = pDescriptorData[data_index];
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,cell_linkage_info);

		if(cell_linkage_info == 0x1)
		{
			pSLabel.Format("bouquet_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,16,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			BaseBitIdx += 24;
			data_index += 3;
		}
		else if(cell_linkage_info == 0x2)
		{
			pSLabel.Format("original_network_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,16,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			pSLabel.Format("transport_stream_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]*256+pDescriptorData[data_index+4]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,16,pDescriptorData[data_index+3]*256+pDescriptorData[data_index+4]);
			pSLabel.Format("service_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+5]*256+pDescriptorData[data_index+6]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+40,16,pDescriptorData[data_index+5]*256+pDescriptorData[data_index+6]);
			BaseBitIdx += 56;
			data_index += 7;

		}
		else if(cell_linkage_info == 0x3)
		{
			pSLabel.Format("original_network_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,16,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			pSLabel.Format("transport_stream_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]*256+pDescriptorData[data_index+4]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,16,pDescriptorData[data_index+3]*256+pDescriptorData[data_index+4]);
			pSLabel.Format("service_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+5]*256+pDescriptorData[data_index+6]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+40,16,pDescriptorData[data_index+5]*256+pDescriptorData[data_index+6]);
			BaseBitIdx += 56;
			data_index += 7;
		}
		else if(cell_linkage_info == 0x4)
		{
			pSLabel.Format("original_network_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,16,pDescriptorData[data_index+1]*256+pDescriptorData[data_index+2]);
			pSLabel.Format("transport_stream_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]*256+pDescriptorData[data_index+4]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,16,pDescriptorData[data_index+3]*256+pDescriptorData[data_index+4]);
			pSLabel.Format("service_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+5]*256+pDescriptorData[data_index+6]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+40,16,pDescriptorData[data_index+5]*256+pDescriptorData[data_index+6]);

			pSLabel.Format("event_id");
			//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+7]*256+pDescriptorData[data_index+8]);
			AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+56,16,pDescriptorData[data_index+7]*256+pDescriptorData[data_index+8]);
			BaseBitIdx += 72;
			data_index +=  9;
		}
		else
		{
			/*don't know format how to calculater length? so consider all left data as undefined private data */
			if((data_index+1) < (descriptor_lenth+2))
			{
				pSLabel.Format("unknow cell_linkage_info(0x%x)",cell_linkage_info);
				CurRot = AddSubBodyBranch(CurRot, pSLabel  ,BaseBitIdx+8, ((descriptor_lenth+2) -(data_index+1))  );

				pSLabel.Format("private_data");
				pDescriptorData += (data_index+1);
				if((descriptor_lenth+2-(data_index+1)) <= VALUE_MAX_LENGTH_IN_BYTE)
				{
					u64 value = 0;
					for(u8 i = 0; i < (descriptor_lenth+2-(data_index+1)) ; i ++)
					{
						value <<= 8;
						value |= *(pDescriptorData);
						pDescriptorData ++;
					}
					AddDefaultBranch(CurRot,pSLabel,BaseBitIdx,(descriptor_lenth+2-(data_index+1))*8, value);
				}
				else
				{
					AddDefaultBranch(CurRot,pSLabel,BaseBitIdx,(descriptor_lenth+2-(data_index+1))*8, (u64)(pDescriptorData));
				}
			}
			break;
		}
	}
}
/*DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME*/
void CDVBDescriptor::ParseDescriptorMultilingualBouquetName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	u32 LanCode ;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256\
			+ pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639-2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);

		pSLabel.Format("bouquent_name_length");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);

		pSLabel.Format("bouquent_name");
		/*for(int i=0 ; i < pDescriptorData[data_index+3] ; i ++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index+4+i]);
		}*/
		AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+32,8*pDescriptorData[data_index+3],
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+4));
		BaseBitIdx += 32 + 8*pDescriptorData[data_index+3];
		data_index += 4 + pDescriptorData[data_index+3];
	}
	
}
/*DESCRIPTOR_MULTILINGUAL_COMPONENT*/
void CDVBDescriptor::ParseDescriptorMultilingualComponent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_MULTILINGUAL_COMPONENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("Component_tag");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,8,pDescriptorData[data_index]);
	data_index ++;
	BaseBitIdx += 24;

	u32 LanCode ;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256\
			+ pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639-2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);

		pSLabel.Format("text_description_length");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);

		pSLabel.Format("text_char");
		/*
		for(int i=0 ; i < pDescriptorData[data_index+3] ; i ++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index+4+i]);
		}*/
		AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+32,8*pDescriptorData[data_index+3],
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+4));
		BaseBitIdx += 32 + 8*pDescriptorData[data_index+3];
		data_index += 4 + pDescriptorData[data_index+3];
	}

}

/*DESCRIPTOR_MULTILINGUAL_NETWORK_NAME*/
void CDVBDescriptor::ParseDescriptorMultilingualNetworkName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_MULTILINGUAL_NETWORK_NAME" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	u32 LanCode ;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256 + pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639-2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);

		pSLabel.Format("network_name_length");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);

		pSLabel.Format("network_name");/*
		for(int i=0 ; i < pDescriptorData[data_index+3] ; i ++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index+4+i]);
		}*/
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+32,8*pDescriptorData[data_index+3],
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+4));
		BaseBitIdx += 32 + 8*pDescriptorData[data_index+3];
		data_index += 4 + pDescriptorData[data_index+3];
	}
}
/*DESCRIPTOR_MULTILINGUAL_SERVICE_NAME*/
void CDVBDescriptor::ParseDescriptorMultilingualServiceName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_MULTILINGUAL_SERVICE_NAME" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	u32 LanCode ;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256\
			+ pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639-2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);

		pSLabel.Format("service_provider_name_length");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);

		pSLabel.Format("service_provider_name");
		/*for(int i=0 ; i < pDescriptorData[data_index+3] ; i ++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index+4+i]);
		}*/
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+32,8*pDescriptorData[data_index+3],
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+4));

		BaseBitIdx += 32 + 8*pDescriptorData[data_index+3];
		data_index += 4 + pDescriptorData[data_index+3];

		pSLabel.Format("service_name_length");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,pDescriptorData[data_index]);

		pSLabel.Format("service_name");
		/*for(int i=0 ; i < pDescriptorData[data_index] ; i ++)
		{
			pSLabel.AppendFormat("%c",pDescriptorData[data_index+1+i]);
		}*/
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+8,8*pDescriptorData[data_index],
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+1));

		BaseBitIdx += 8 + 8 * pDescriptorData[data_index];
		data_index += 1 + pDescriptorData[data_index];
	}
}
/*DESCRIPTOR_NVOD_REFERENCE*/
void CDVBDescriptor::ParseDescriptorNVODReference(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_NVOD_REFERENCE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("Transport_stream_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);

		pSLabel.Format("original_network_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,16,pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3]);

		pSLabel.Format("service_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+32,16,pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);
		
		BaseBitIdx += 48;
		data_index += 6;
	}
}

/*DESCRIPTOR_NETWORK_NAME*/
void CDVBDescriptor::ParseDescriptorNetworkName(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_NETWORK_NAME" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("name");/*
	for(int i=0 ; i < descriptor_lenth ; i ++)
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index+i]);
	}*/
	AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+16,8*descriptor_lenth,
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));
}
/*DESCRIPTOR_PARENTAL_RATING*/
void CDVBDescriptor::ParseDescriptorParentalRating(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_PARENTAL_RATING" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("country_code");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256*256\
			//+ pDescriptorData[data_index+1]*256 + pDescriptorData[data_index+2] );
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,
			pDescriptorData[data_index]*256*256+ pDescriptorData[data_index+1]*256 + pDescriptorData[data_index+2]);

		pSLabel.Format("Rating");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);
		
		BaseBitIdx += 32;
		data_index += 4;
	}
}
/*DESCRIPTOR_PDC*/
void CDVBDescriptor::ParseDescriptorPDC(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_PDC" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("programme_identification_label");
	u32 pil = (pDescriptorData[data_index]& 0xF)*256*256 + pDescriptorData[data_index+1]*256\
		+pDescriptorData[data_index+2];
	//pSLabel.AppendFormat(DATAFORMAT,pil);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+20,20,pil);
}
/*DESCRIPTOR_PRIVATE_DATA_SPECIFIER*/
void CDVBDescriptor::ParseDescriptorPrivateDataSpecifier(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_PRIVATE_DATA_SPECIFIER" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("private_data_specifier");
	u32 pds = pDescriptorData[data_index]*256*256*256 + pDescriptorData[data_index+1]*256*256\
		+ pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3];
	//pSLabel.AppendFormat(DATAFORMAT,pds);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,32,pds);
}
/*DESCRIPTOR_SHORT_SMOOTHING_BUFFER*/
void CDVBDescriptor::ParseDescriptorShortSmoothingBuffer(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_SHORT_SMOOTHING_BUFFER" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("sb_size");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]>>6);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,2,pDescriptorData[data_index]>>6);
	
	pSLabel.Format("sb_leak_rate");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index] & 0x3F);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+18,6,pDescriptorData[data_index] & 0x3F);
}
/*DESCRIPTOR_SERVICE*/
void CDVBDescriptor::ParseDescriptorService(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_SERVICE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("service_type");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,8,pDescriptorData[data_index]);
	
	pSLabel.Format("service_provider_name_length");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+1]);

	pSLabel.Format("service_provider_name");
	/*for(int i=0 ; i < pDescriptorData[data_index+1] ; i ++)
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index+2+i]);
	}*/
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+32,8*pDescriptorData[data_index+1],
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+2));

	BaseBitIdx += 32 + 8*pDescriptorData[data_index+1];
	data_index += 2 + pDescriptorData[data_index+1];

	pSLabel.Format("service_name_length");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,pDescriptorData[data_index]);

	pSLabel.Format("service_name");
	/*for(int i=0 ; i < pDescriptorData[data_index] ; i ++)
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index+1+i]);
	}*/
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+8,8*pDescriptorData[data_index],
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+1));
}
/*DESCRIPTOR_SERVICE_LIST*/
void CDVBDescriptor::ParseDescriptorServiceList(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
//	CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_SERVICE_LIST" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("service_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256	+ pDescriptorData[data_index+1]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256	+ pDescriptorData[data_index+1]);

		pSLabel.Format("service_type");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,8,pDescriptorData[data_index+2]);
		
		BaseBitIdx += 24;
		data_index += 3;
	}
}
/*DESCRIPTOR_SERVICE_MOVE*/
void CDVBDescriptor::ParseDescriptorServiceMove(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_SERVICE_MOVE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	pSLabel.Format("new_original_network_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);

	pSLabel.Format("new_transport_stream_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,16,pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3]);

	pSLabel.Format("new_service_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+32,16,pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);
}
/*DESCRIPTOR_SHORT_EVENT*/
void CDVBDescriptor::ParseDescriptorShortEvent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_SHORT_EVENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;
	
	u32 LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256 \
		+ pDescriptorData[data_index+2];
	pSLabel.Format("ISO 639_2_language_code");
	//pSLabel.AppendFormat(DATAFORMAT,LanCode);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);

	pSLabel.Format("event_name_length");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);

	pSLabel.Format("event_name");
	/*for(int i=0 ; i < pDescriptorData[data_index+3] ; i ++)
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index+4+i]);
	}*/
	AddFieldBranch(CurRot, pSLabel ,BaseBitIdx+32,8*pDescriptorData[data_index+3],
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+4));

	BaseBitIdx += 32 + 8*pDescriptorData[data_index+3];
	data_index += 4 + pDescriptorData[data_index+3];

	pSLabel.Format("text_length");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,pDescriptorData[data_index]);

	if(pDescriptorData[data_index])
	{
		pSLabel.Format("text_char");
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx+8,8*pDescriptorData[data_index],
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index+1));
	}
}
/*DESCRIPTOR_STREAM_IDENTIFIER*/
void CDVBDescriptor::ParseDescriptorStreamIdentifier(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_STREAM_IDENTIFIER" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("component_tag");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,8,pDescriptorData[data_index]);
}
/*DESCRIPTOR_STUFFING*/
void CDVBDescriptor::ParseDescriptorStuffing(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
			
	pSLabel.Format("DESCRIPTOR_STUFFING" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
}
/*DESCRIPTOR_SUBTITLING*/
void CDVBDescriptor::ParseDescriptorSubtitling(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_SUBTITLING" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	u32 LanCode;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256 \
			+pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639_2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);
		
		pSLabel.Format("substitling_type");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,8,pDescriptorData[data_index+3]);

		pSLabel.Format("composition_page_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+32,16,pDescriptorData[data_index+4]*256 + pDescriptorData[data_index+5]);

		pSLabel.Format("ancillary_page_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+6]*256 + pDescriptorData[data_index+7]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+48,16,pDescriptorData[data_index+6]*256 + pDescriptorData[data_index+7]);
		
		BaseBitIdx += 64;
		data_index += 8;
	}
}
/*DESCRIPTOR_TELETEXT*/
void CDVBDescriptor::ParseDescriptorTeletext(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
				
	pSLabel.Format("DESCRIPTOR_TELETEXT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx +=  16;
	u32 LanCode;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256 \
			+ pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639_2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);
		
		pSLabel.Format("teletext_type");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]>>3);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,5,pDescriptorData[data_index+3]>>3);

		pSLabel.Format("teletext_magazine_number");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3] & 0x7);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+29,3,pDescriptorData[data_index+3] & 0x7);

		pSLabel.Format("teletext_page_number");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+4]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+32,8,pDescriptorData[data_index+4]);
		
		BaseBitIdx += 40;
		data_index += 5;
	}
}
/*DESCRIPTOR_TIME_SHIFTED_EVENT*/
void CDVBDescriptor::ParseDescriptorTimeShiftedEvent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_TIME_SHIFTED_EVENT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	pSLabel.Format("reference_service_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);

	pSLabel.Format("reference_event_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,16,pDescriptorData[data_index+2]*256 + pDescriptorData[data_index+3]);
}
/*DESCRIPTOR_TIME_SHIFTED_SERVICE*/
void CDVBDescriptor::ParseDescriptorTimeShiftedService(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_TIME_SHIFTED_SERVICE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	pSLabel.Format("reference_service_id");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,16,pDescriptorData[data_index]*256 + pDescriptorData[data_index+1]);
}
/*DESCRIPTOR_TRANSPORT_STREAM*/
void CDVBDescriptor::ParseDescriptorTransportStream(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_TRANSPORT_STREAM" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	pSLabel.Format("byte");
	/*while(data_index < (descriptor_lenth+2))
	{
		pSLabel.AppendFormat("%c",pDescriptorData[data_index++]);
	}*/
	AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx,8 * descriptor_lenth,
		CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));

}
/*DESCRIPTOR_VBI_DATA*/
void CDVBDescriptor::ParseDescriptorVBIData(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_VBI_DATA" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	while(data_index < (descriptor_lenth+2))
	{
		pSLabel.Format("data_service_id");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,8,pDescriptorData[data_index]);

		u8 DesLen = pDescriptorData[data_index+1];
		pSLabel.Format("data_service_descriptor_length");
		//pSLabel.AppendFormat(DATAFORMAT,DesLen);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,DesLen);

		if((pDescriptorData[data_index] == 0x1)||(pDescriptorData[data_index] == 0x2)||
			(pDescriptorData[data_index] == 0x4)||(pDescriptorData[data_index] == 0x5)||
			(pDescriptorData[data_index] == 0x6)||(pDescriptorData[data_index] == 0x7))
		{
			BaseBitIdx += 16;
			for(int i = 0; i < DesLen;  i ++)
			{
				pSLabel.Format("field_parity");
				//pSLabel.AppendFormat(DATAFORMAT, (pDescriptorData[data_index+i]>>5)&0x1);
				AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+2,1, (pDescriptorData[data_index+i]>>5)&0x1);

				pSLabel.Format("line_offset");
				//pSLabel.AppendFormat(DATAFORMAT, pDescriptorData[data_index+i]&0x1F);
				AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+3,5, pDescriptorData[data_index+i]&0x1F);
				BaseBitIdx += 8;
			}
		}
		BaseBitIdx += DesLen*8;
		data_index += DesLen;
	}
}
/*DESCRIPTOR_VBI_TELETEXT*/
void CDVBDescriptor::ParseDescriptorVBITeletext(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_VBI_TELETEXT" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);
	BaseBitIdx += 16;

	u32 LanCode;
	while(data_index < (descriptor_lenth+2))
	{
		LanCode = pDescriptorData[data_index]*256*256 + pDescriptorData[data_index+1]*256 \
			+ pDescriptorData[data_index+2];
		pSLabel.Format("ISO 639_2_language_code");
		//pSLabel.AppendFormat(DATAFORMAT,LanCode);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx,24,LanCode);
		
		pSLabel.Format("teletext_type");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3]>>3);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+24,5,pDescriptorData[data_index+3]>>3);

		pSLabel.Format("teletext_magazine_number");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+3] & 0x7);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+29,3,pDescriptorData[data_index+3] & 0x7);

		pSLabel.Format("teletext_page_number");
		//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index+4]);
		AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+32,8,pDescriptorData[data_index+4]);
		
		BaseBitIdx += 40;
		data_index += 5;
	}
}
/*DESCRIPTOR_TELEPHONE*/
void CDVBDescriptor::ParseDescriptorTelephone(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	//CContentTree * SubRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
	u16	data_index = 2;
			
	pSLabel.Format("DESCRIPTOR_TELEPHONE" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_tag);
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	//pSLabel.AppendFormat(DATAFORMAT,descriptor_lenth);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("foreign_availability");
	//pSLabel.AppendFormat(DATAFORMAT,(pDescriptorData[data_index]>>5) & 0x1);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+18,1,(pDescriptorData[data_index]>>5) & 0x1);

	pSLabel.Format("connection_type");
	//pSLabel.AppendFormat(DATAFORMAT,pDescriptorData[data_index]& 0x1F);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+19,5,pDescriptorData[data_index]& 0x1F);

	u8 cpl,iacl,ocl,nacl,cnl;
	cpl = (pDescriptorData[data_index+1]>>5)& 0x3;
	pSLabel.Format("country_prefix_length");
	//pSLabel.AppendFormat(DATAFORMAT,cpl);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+25,2,cpl);

	iacl = (pDescriptorData[data_index+1]>>2)& 0x7;
	pSLabel.Format("international_area_code_length");
	//pSLabel.AppendFormat(DATAFORMAT,iacl);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+27,3,iacl);

	ocl = pDescriptorData[data_index+1]& 0x3;
	pSLabel.Format("operator_code_length");
	//pSLabel.AppendFormat(DATAFORMAT,ocl );
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+30,2,ocl);
	
	nacl = (pDescriptorData[data_index+2]>>4)& 0x7;
	pSLabel.Format("national_area_code_length");
	//pSLabel.AppendFormat(DATAFORMAT,nacl);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+33,3,nacl);

	cnl = pDescriptorData[data_index+2]& 0xF;
	pSLabel.Format("core_number_length");
	//pSLabel.AppendFormat(DATAFORMAT, cnl);
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+36,4, cnl);	
	data_index += 3;
	BaseBitIdx += 40;
	
	if(cpl)
	{
		pSLabel.Format("country_prefix");
		/*for(int i =0; i < cpl ; i++)
		{
			pSLabel.AppendFormat("%c", pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel   ,BaseBitIdx,8*cpl,
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));

		BaseBitIdx += 8*cpl;
		data_index += cpl;
	}

	if(iacl)
	{
		pSLabel.Format("international_area_code");
		/*for(int i =0; i < iacl ; i++)
		{
			pSLabel.AppendFormat("%c", pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel   ,BaseBitIdx,8*iacl,
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));
		BaseBitIdx += 8*iacl;
		data_index += iacl;
	}

	if(ocl)
	{
		pSLabel.Format("operator_code");
		/*for(int i =0; i < ocl ; i++)
		{
			pSLabel.AppendFormat("%c", pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx,8*ocl,
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));
		BaseBitIdx += 8*ocl;
		data_index += ocl;
	}

	if(nacl)
	{
		pSLabel.Format("national_area_code");
		/*for(int i =0; i < nacl ; i++)
		{
			pSLabel.AppendFormat("%c", pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel  ,BaseBitIdx,8*nacl,
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));
		BaseBitIdx += 8*nacl;
		data_index +=  nacl;
	}

	if(cnl)
	{
		pSLabel.Format("core_number_char");
		/*for(int i =0; i < cnl ; i++)
		{
			pSLabel.AppendFormat("%c", pDescriptorData[data_index++]);
		}*/
		AddFieldBranch(CurRot, pSLabel  , BaseBitIdx,8*cnl,
			CContentFieldNode::FIELD_FMT_STR,(u64)(pDescriptorData+data_index));
		data_index +=  cnl;
		BaseBitIdx += 8*cnl;
	}
}
/*DESCRIPTOR_DEFAULT*/
void CDVBDescriptor::ParseDescriptorDefault(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{
	CString pSLabel;
	CContentTree * CurRot;
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];
			
	pSLabel.Format("DESCRIPTOR_UNKNOW" );
	CurRot = AddSubBodyBranch(parent, pSLabel ,BaseBitIdx,(descriptor_lenth+2)*8);

	pSLabel.Format("tag" );
	 AddDefaultBranch(CurRot, pSLabel ,BaseBitIdx,8,descriptor_tag);
	
	pSLabel.Format("length");
	AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+8,8,descriptor_lenth);

	pSLabel.Format("descriptor_data");
	pDescriptorData += 2;
	if(descriptor_lenth == 0)
	{
	}
	else if(descriptor_lenth <= VALUE_MAX_LENGTH_IN_BYTE )
	{
		u64 value = 0;
		for(u8 i = 0; i < descriptor_lenth ; i ++)
		{
			value <<= 8;
			value |= *(pDescriptorData);
			pDescriptorData ++;
		}
		AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+16,descriptor_lenth*8, value);
	}
	else
	{
		AddDefaultBranch(CurRot,pSLabel,BaseBitIdx+16,descriptor_lenth*8, (u64)(pDescriptorData));
	}
	//for(int i = 0; i < descriptor_lenth; i++)
//{
	//AddDefaultBranch(CurRot, pSLabel  ,BaseBitIdx+16,descriptor_lenth * 8, *(pDescriptorData+2+i));
//}
}

u32 CDVBDescriptor::ParseDVBDescriptorContent(CContentTree * parent, u8 * pDescriptorData,u32 BaseBitIdx)
{		
	u8 descriptor_tag = pDescriptorData[0];
	u8 descriptor_lenth = pDescriptorData[1];

	switch(descriptor_tag)
	{
		case DESCRIPTOR_ANCILLARY_DATA:
			ParseDescriptorAnciliaryData(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_ANNOUNCEMENT_SUPPORT:
			ParseDescriptorAnnouncementSupport(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_BOUQUET_NAME:
			ParseDescriptorBouquetName(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_CA_IDENTIFIER:
			ParseDescriptorCAIdentifier(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_CELL_FREQUENCY_LINK:
			ParseDescriptorCellFrequencyLine(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_CELL_LIST:
			ParseDescriptorCellList(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_COMPONENT:
			ParseDescriptorComponent(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_CONTENT:
			ParseDescriptorContent(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_COUNTRY_AVAILABILITY:
			ParseDescriptorCountryAvailability(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_DATA_BROADCAST:
			ParseDescriptorDataBroadcast(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_DATA_BROADCAST_ID:
			ParseDescriptorDataBroadcastID(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_CABLE_DELIVERY_SYSTEM:
			ParseDescriptorCableDeliverySystem(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SATELLITE_DELIVERY_SYSTEM:
			ParseDescriptorSatelliteDeliverySystem(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TERRESTRIAL_DELIVERY_SYSTEM:
			ParseDescriptorTerrestrialDeliverySystem(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_DSNG:
			ParseDescriptorDSNG(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_EXTENDED_EVENT:
			ParseDescriptorExtendedEvent(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_FREQUENCY_LIST:
			ParseDescriptorFrequencyList(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_LINKAGE:
			ParseDescriptorLinkage(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_LOCAL_TIME_OFFSET:
			ParseDescriptorLocalTimeOffset(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_MOSAIC:
			ParseDescriptorMosaic(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME:
			ParseDescriptorMultilingualBouquetName(parent, pDescriptorData,BaseBitIdx);	
			break;
		case DESCRIPTOR_MULTILINGUAL_COMPONENT:
			ParseDescriptorMultilingualComponent(parent, pDescriptorData,BaseBitIdx);	
			break;
		case DESCRIPTOR_MULTILINGUAL_NETWORK_NAME:
			ParseDescriptorMultilingualNetworkName(parent, pDescriptorData,BaseBitIdx);		
			break;
		case DESCRIPTOR_MULTILINGUAL_SERVICE_NAME:
			ParseDescriptorMultilingualServiceName(parent, pDescriptorData,BaseBitIdx);	
			break;
		case DESCRIPTOR_NVOD_REFERENCE:
			ParseDescriptorNVODReference(parent, pDescriptorData,BaseBitIdx);	
			break;
		case DESCRIPTOR_NETWORK_NAME:
			ParseDescriptorNetworkName(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_PARENTAL_RATING:
			ParseDescriptorParentalRating(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_PDC:
			ParseDescriptorPDC(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_PRIVATE_DATA_SPECIFIER:
			ParseDescriptorPrivateDataSpecifier(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SHORT_SMOOTHING_BUFFER:
			ParseDescriptorShortSmoothingBuffer(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SERVICE:
			ParseDescriptorService(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SERVICE_LIST:
			ParseDescriptorServiceList(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SERVICE_MOVE:
			ParseDescriptorServiceMove(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SHORT_EVENT:
			ParseDescriptorShortEvent(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_STREAM_IDENTIFIER:
			ParseDescriptorStreamIdentifier(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_STUFFING:
			ParseDescriptorStuffing(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_SUBTITLING:
			ParseDescriptorSubtitling(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TELETEXT:
			ParseDescriptorTeletext(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TIME_SHIFTED_EVENT:
			ParseDescriptorTimeShiftedEvent(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TIME_SHIFTED_SERVICE:
			ParseDescriptorTimeShiftedService(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TRANSPORT_STREAM:
			ParseDescriptorTransportStream(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_VBI_DATA:
			ParseDescriptorVBIData(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_VBI_TELETEXT:
			ParseDescriptorVBITeletext(parent, pDescriptorData,BaseBitIdx);
			break;
		case DESCRIPTOR_TELEPHONE:
			ParseDescriptorTelephone(parent, pDescriptorData,BaseBitIdx);
			break;
		default:
			ParseDescriptorDefault(parent, pDescriptorData,BaseBitIdx);		
			break;	
	}
	return descriptor_lenth;
}

