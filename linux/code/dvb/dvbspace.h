#ifndef DVBSPACE_H
#define DVBSPACE_H

#include "typedefine.h"


namespace NDVBSpace
{

	#define SYN_BYTE			0x47
	#define CRC_32_LENGTH		4

	/*	described on page 70	*/
	enum PACKET_ID
	{
		PAT_PID			=0X0000,
		CAT_PID			=0X0001,
		TSDT_PID			=0X0002,
		NIT_ST_PID 			=0X0010,
		SDT_BAT_ST_PID	=0X0011,
		EIT_ST_PID			=0X0012,
		RST_ST_PID			=0X0013,
		TDT_TOT_ST_PID	=0X0014,
		NET_SYN_PID		=0X0015,/*网络同步化*/
		DIT_PID				=0X001E,
		SIT_PID				=0X001F,
		STUFF_PID				=0x1FFF,
		INVALID_PID		=0XFFFF/* pid is a 13 bit data, max is 0x1FFF.	*/
	};


	enum TableIdDefine
	{
		PROGRAM_ASSOCIATION_INFORMATION							=0X00,
		CONDITIONAL_ACCESS_INFORMATION								=0X01,
		PROGRAM_MAP_INFORMATION										=0X02,
		TRANSPORT_STREAM_DESCRIPTION								=0x03,

		ACTUAL_NETWORK_INFORMATION									=0x40,
		OTHER_NETWORK_INFORMATION									=0x41,

		ACTUAL_TRANSPORT_SERVICE_DESCRIPTION						=0x42,
		OTHER_TRANSPORT_SERVICE_DESCRIPTION							=0x46,

		BOUQUET_ASSOCIATION_INFORMATION								=0x4A,

		ACTUAL_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION 		=0X4E,
		OTHER_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION 		=0X4F,

		ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN				= 0X50,	/* 0X50 ~ 0X5F,*/
		ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MAX				= 0X5F,	/* 0X50 ~ 0X5F,*/

		OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN				= 0X60,	/* 0X60 ~ 0X6F,*/
		OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MAX				= 0X6F,	/* 0X60 ~ 0X6F,*/

		TIME_DATE_INFORMATION										=0X70,
		RUNING_STATUS_INFORMATION									=0X71,
		STUFFING_SECTION											=0X72,
		TIME_OFFSET_INFORMATION										=0X73,
		DISCONTINUITY_INFORMATION									=0X7E,
		SELECTION_INFORMATION										=0X7F,

		ENTITLE_CONTROL_MESSAGE_EVEN								=0X80,
		ENTITLE_CONTROL_MESSAGE_ODD									=0X81,
		ENTITLE_MANAGE_MESSAGE				=0X82,	/*	0X 82 ~ 0X 8F		*/

		RESERVED														= 0XFF,
		INVALID_TABLE_ID				= 0XFFFF/* Table ID  is a 8 bit data, max is 0xFF.	*/
	};
		
	/*	described on page 25 chinese SI	*/
	enum MPEG2DESCRIPTOR_TAG
	{
	/* iso/iec 13818-1  descripted in MPEG2_CHS p70 */
	/* 0x00, 0x01, 0x19-0x3f are reserved */
	        DESCRIPTOR_VIDEO_STREAM                       =       0x02,
	        DESCRIPTOR_AUDIO_STREAM                       =       0x03,
	        DESCRIPTOR_HIERARCHY                          =       0x04,
	        DESCRIPTOR_REGISTRATION                       =       0x05,
	        DESCRIPTOR_DATA_STREAM_ALIGNMENT              =       0x06,
	        DESCRIPTOR_TARGET_BACKGROUND_GRID             =       0x07,
	        DESCRIPTOR_VIDEO_WINDOW                       =       0x08,
	        DESCRIPTOR_CA                                 =       0x09,
	        DESCRIPTOR_ISO_639_LANGUAGE                   =       0x0A,
	        DESCRIPTOR_SYSTEM_CLOCK                       =       0x0B,
	        DESCRIPTOR_MULTIPLEX_BUFFER_UTILIZATION       =       0x0C,
	        DESCRIPTOR_COPYRIGHT                          =       0x0D,
	        DESCRIPTOR_MAXIMUM_BITRATE                    =       0x0E,
	        DESCRIPTOR_PRIVATE_DATA_INDICATOR             =       0x0F,
	        DESCRIPTOR_SMOOTHING_BUFFER                   =       0x10,
	        DESCRIPTOR_STD                                		=       0x11,
	        DESCRIPTOR_IBP                                		=       0x12,
		 DESCRIPTOR_MPEG_TAG_BIGEST				=		0x3F,
	        
	/*	0x10 ~ 0x 3F  ITU_T Rce.H.222.0 | ISO/IEC 13818 reserved */
	};	


	/* on page 27	*/
	/*
	高位在前
	*/
	struct PacketData
	{
		u8 syn_byte						:8 ;

		u8 PID_hi						:5 ;
		u8 transport_priority				:1 ;
		u8 payload_unit_start_indicator	:1 ;
		u8 transport_error_indicator		:1 ;
		
		u8 PID_low						:8;

		u8 continuity_counter			:4 ;
		u8 adaptiation_field_control		:2 ;
		u8 transport_scrambling_control	:2 ;

		/*	if ((payload_unit_start_indicator == 1) && (current packet is a SI Packet))	*/
		u8	DataStartPointer			:8; 
	/*
	*	if(adaptiation_field_control == 10 || adaptiation_field_control==11)
	*		DataStartPointer is adaptation_field_length
	*		adaptation_field()
	*	if(adaptiation_field_control == 01 )//00  is reserved
	*	{
	*		for(i=0;i<N;i++)
	*			data_byte		8 		bslbf
	*	}
	*/
	}	;


	struct  Descriptor{
		u8 	descriptor_tag				:8;
		u8	descriptor_length			:8;

		/*
		other data depend on descriptor_tag
		*/
	};

	/*	common Section struct define	8 Byte head */
	struct CommonSection{
	    	u8	table_id						:8;
		
		u8	section_length_hi				:4;
		u8	reserved1					:2;
		u8	private_indicator			:1;/* it's a flag of if it's a private table or not 	*/
		u8	section_syntax_indicator		:1;/* if 1 means this table is divided to sections ; 0 means not*/
			
		u8	section_length_low			:8;
		u8	sub_table_id_hi				:8;
		u8	sub_table_id_low				:8;
		
		u8 	current_next_indicator		:1;
		u8 	version_number				:5;
		u8	reserved4					:2;
		
		u8 	section_number				:8;
		u8 	last_section_number			:8;     
	/*
		for(i=0;i<N-7;i++)
			data_byte				:8
	*/
	};

	/* on page 57	*/
	struct PATSection
	{
		u8	table_id						:8;

		u8	section_length_hi				:4;
		u8	reserved1						:2;	
		u8	zero/* always == 0 */				:1;
		u8	section_syntax_indicator			:1;

		u8	section_length_low				:8;
		
		u8	transport_stream_id_hi			:8;
		u8 	transport_stream_id_low			:8;

		u8	current_next_indicator			:1;
		u8	version_number					:5;	
		u8	reserved2						:2;
		
		u8	section_number				:8;
		u8	last_section_number			:8;
	/*
		for(i=0;i<N;i++)
		{
			program_number					16
			reserved							3
			if(program_number == 0)
				network_id					13
			else
				program_map_PID				13
		}
		CRC_32							32			rpchop
	*/

	};

	/* on page 60	*/
	struct PMTSection
	{
		u8	table_id						:8;

		u8	section_length_hi			:4;
		u8	reserved1					:2;
		u8	zero /* always == 0*/			:1;	
		u8	section_syntax_indicator		:1;
		
		u8	section_length_low			:8;
		
		u8	program_number_hi			:8;

		u8	program_number_low		:8;

		u8 	current_next_indicator		:1;
		u8 	version_number				:5;	
		u8	reserved2					:2;
		
		u8 	section_number				:8;
		u8 	last_section_number			:8;

		u8	PCR_PID_hi					:5;	
		u8	reserved3					:3;

		u8	PCR_PID_low				:8;
		
		u8	program_info_length_hi		:4;	
		u8	reserved4					:4;

		u8	program_info_length_low	:8;
		
	/*
		for(i=0;i<N ;i++)
			descriptor()
		for(i=0;i<N;i++)
		{
			stream_type				8
			reserved					3
			elementary_PID			13
			reserved					4
			ES_info_length			12
			for(j=0;j<N;j++)
				descriptor()
		}
		CRC_32						32
	*/
		

	};


	/* on page 79 */
	struct BATSection
	{
		u8 	table_id					:8;

		u8	section_length_hi		:4;
		u8							:2;
		u8							:1;	
		u8	section_syntax_indicator	:1;

		u8	section_length_low		:8;

		u8 	bouquet_id_hi			:8;
		u8	bouquet_id_low			:8;
		
		u8	current_next_indicator	:1;	
		u8	version_number			:5;
		u8							:2;

		u8 	section_number			:8;
		u8	last_section_number		:8;

		u8	bouquet_descriptors_length_hi :4;
		u8							:4;

		u8 	bouquet_descriptors_length_low	:8;
		/*
		for(j=0;j<N;j++)
			descriptor()
		u8	...
		*/
			
	};
	/* on page 63	*/
	struct CATSection
	{
		u8	table_id					:8;

		u8	section_length_hi			:4;
		u8	reserved1				:2;
		u8	zero /* always == 0*/		:1;	
		u8	section_syntax_indicator	:1;

		u8	section_length_low			:8;
		u8	reserved2				:8;
		u8	reserved3				:8;

		u8 	current_next_indicator		:1;
		u8 	version_number			:5;	
		u8	reserved4				:2;
		
		u8 	section_number			:8;
		u8 	last_section_number		:8;
	/*
		for(i=0;i<N;i++)
			CA_descriptor()

		CRC_32								32
	*/
	};

	/* on page 193	*/
	struct CAMessageSection
	{
		u8	table_id					:8;

		u8	section_length_hi		:4;
		u8	ISO_reserved			:2;
		u8	DVB_reserved			:1;	
		u8	section_syntax_indicator	:1;

		u8	section_length_low		:8;

		u8								:8;
		u8								:8;
		u8								:8;
		/*	only when it's EMM	will contain card number	*/
		u8  	card_number1				:8;
		u8  	card_number2				:8;
		u8  	card_number3				:8;
		u8  	card_number4				:8;
		
	/*
		for(i=0;i<N-7;i++)
			CA_data_byte				:8
	*/
	};

};


#endif

