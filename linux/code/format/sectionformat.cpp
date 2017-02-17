#include "sectionformat.h"


CStandardSectionFormat::CStandardSectionFormat(void)
{
}

CStandardSectionFormat::~CStandardSectionFormat(void)
{
}

CString CStandardSectionFormat::GetTitleBySection(CSection * pSec)
{
	CString Title;
	if (pSec->GetSectionSyntaxIndicator())
		Title.Format("Version %d; Section[%d / %d].",pSec->GetSectionVersion(),
			pSec->GetSectionNum(),pSec->GetLastSectionNum());
	else
		Title.Format("Data");

	return Title;
}

CContentTree * CStandardSectionFormat::ParseContentBySection(CSection * pSec)
{
	CContentTree * CurRoot = NULL;
	
	if(pSec->GetSectionData() == NULL)
		return NULL;

	CurRoot = PrivateSectionParse(pSec);
	if( CurRoot != NULL)
		return CurRoot;
	
	CurRoot = AddSectionBranch(NULL,GetTitleBySection(pSec) );
	ParseSectionContent(pSec->GetSectionData(), CurRoot);

	return CurRoot;
}

bool CStandardSectionFormat::ParseSectionContent(u8 * SectionData,CContentTree * CurRoot)
{
	bool		ret = false;
	CommonSection * pCS =(CommonSection * )SectionData;
	
	switch(pCS->table_id)
	{
		case PROGRAM_ASSOCIATION_INFORMATION:
			ret= ParsePATContent(pCS,CurRoot);
			break;
		case CONDITIONAL_ACCESS_INFORMATION:
			ret = ParseCATContent(pCS,CurRoot);
			break;
		case PROGRAM_MAP_INFORMATION:
			ret = ParsePMTContent(pCS,CurRoot);
			break;
		case TRANSPORT_STREAM_DESCRIPTION:
			ret = ParseTSDTContent(pCS,CurRoot);
			break;
		case ACTUAL_NETWORK_INFORMATION:
		case OTHER_NETWORK_INFORMATION:
			ret = ParseNITContent(pCS,CurRoot);
			break;
		case ACTUAL_TRANSPORT_SERVICE_DESCRIPTION:
		case OTHER_TRANSPORT_SERVICE_DESCRIPTION:
			ret = ParseSDTContent(pCS,CurRoot);
			break;
		case BOUQUET_ASSOCIATION_INFORMATION:
			ret = ParseBATContent(pCS,CurRoot);
			break;
		case ACTUAL_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION:
		case OTHER_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION:
			ret = ParseEITContent(pCS,CurRoot);
			break;
		case TIME_DATE_INFORMATION:
			ret = ParseTDTContent(pCS,CurRoot);
			break;
		case RUNING_STATUS_INFORMATION:
			ret = ParseRSTContent(pCS,CurRoot);
			break;
		case STUFFING_SECTION:
			ret = ParseSTContent(pCS,CurRoot);
			break;
		case TIME_OFFSET_INFORMATION:
			ret = ParseTOTContent(pCS,CurRoot);
			break;
		case DISCONTINUITY_INFORMATION:
			ret = ParseDITContent(pCS,CurRoot);
			break;
		case SELECTION_INFORMATION:
			ret = ParseSITContent(pCS,CurRoot);
			break;
		case ENTITLE_CONTROL_MESSAGE_EVEN:
		case ENTITLE_CONTROL_MESSAGE_ODD:
		case ENTITLE_MANAGE_MESSAGE:
			ret = ParsePrivateTableContent(pCS,CurRoot);
		break;
		default:
		{
			if(((pCS->table_id & 0xF0) == ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN)||
				((pCS->table_id & 0xF0) == OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN))
			{
				ret = ParseEITContent(pCS,CurRoot);
			}
			else
			{
				ret = ParsePrivateTableContent(pCS,CurRoot);
			}
		}
			break;
	}
	
	return (ret);
}


bool CStandardSectionFormat::ParsePATContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	PATSection * pPS= (PATSection *)pCS;

	int SectionLength = pPS->section_length_hi*256+pPS->section_length_low;
	int ProgramNumber,PMT_PID ,ProgramCount = (SectionLength -5 - CRC_32_LENGTH)/4;
	u8 * pU8 = (u8 *)pCS ;

	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pPS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pPS->section_syntax_indicator);

	AddReserveBranch(CurRt,9,3);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLength);

	StrBuff.Format("transport_stream_id");
	AddDefaultBranch(CurRt,StrBuff,24,16,pPS->transport_stream_id_hi*256+pPS->transport_stream_id_low);

	AddReserveBranch(CurRt,40,2);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pPS->version_number);

	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pPS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pPS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pPS->last_section_number);
	
	pU8 += 8;
	CContentTree * pTree;
	int i = 0;
	for(; i<ProgramCount; i++)
	{
		StrBuff.Format("Service");
		pTree = AddSubBodyBranch(CurRt,StrBuff,64+(i * 32) , 32);

		ProgramNumber = pU8[0]*256 + pU8[1];
		StrBuff.Format("program_number");
		AddDefaultBranch(pTree,StrBuff,64+(i * 32) , 16, ProgramNumber);

		AddReserveBranch(pTree,64 + 16 +(i * 32), 3);

		PMT_PID = pU8[2]&0x1F ;
		PMT_PID = PMT_PID	* 256 + pU8[3];
		if(ProgramNumber)
			StrBuff.Format("program_map_PID - PMT");
		else
			StrBuff.Format("network_id - NIT" );

		AddDefaultBranch(pTree,StrBuff,64 + 16 + 3  +(i * 32) , 13, PMT_PID);
		pU8 += 4;
	}
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	StrBuff.Format("CRC_32");
	AddDefaultBranch(CurRt,StrBuff,64 +(i * 32) , 32, CRC);
	
	return true;
}


bool CStandardSectionFormat::ParseCATContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	//CContentTree * pHTree;
	CATSection * pPS= (CATSection *)pCS;;
	int SectionLength = pPS->section_length_hi*256+pPS->section_length_low;
	int DescriptorCount=0;

	u8 *pU8 = (u8 *)pCS ;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pPS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pPS->section_syntax_indicator);
	AddReserveBranch(CurRt,9,3);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLength);
	AddReserveBranch(CurRt,24,18);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pPS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pPS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pPS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pPS->last_section_number);
	
	pU8 += 8;
	SectionLength -=5;
	ParseDescriptorLoop(CurRt, pU8 , SectionLength-4,64);
	pU8 += SectionLength-4;
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	StrBuff.Format("CRC_32");
	AddDefaultBranch(CurRt,StrBuff,64 + 8 * (SectionLength-4) ,32,CRC);
	return true;
}

bool CStandardSectionFormat::ParsePMTContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	CString HelpStr;
	CContentTree *  pHTree;
	PMTSection * pPS= (PMTSection *)pCS;
	int SectionLeft = pPS->section_length_hi*256+pPS->section_length_low;
	int temp;//,DescriptorCount=0;

	u8 *pU8 = (u8 *)pPS ;
	
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pPS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pPS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLeft);
	
	temp = pPS->program_number_hi * 256 + pPS->program_number_low;
	StrBuff.Format("program_number");
	AddDefaultBranch(CurRt,StrBuff,24,16,temp);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pPS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pPS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pPS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pPS->last_section_number);

	temp = pPS->PCR_PID_hi * 256 + pPS->PCR_PID_low;
	StrBuff.Format("PCR_PID");
	AddDefaultBranch(CurRt,StrBuff,67,13,temp);

	temp = pPS->program_info_length_hi * 256 + pPS->program_info_length_low;
	StrBuff.Format("program_info_loop_length");
	AddDefaultBranch(CurRt,StrBuff,84,12,temp);

	pU8 += 12;
	if((SectionLeft - (temp + (12 - 3)) ) < 4)
		return false;
	ParseDescriptorLoop(CurRt, pU8, temp,96);

	pU8 += temp;
	SectionLeft -= (temp + (12 - 3));
	u32 CurIdx = 96+8*temp;
	//u8 i=0;
	while(SectionLeft >= 5)
	{
		temp = (pU8[3]& 0x0F)*256 + pU8[4];
		
		StrBuff.Format("ES");
		//StrBuff.AppendFormat(DATAFORMAT,i++);
		pHTree=AddSubBodyBranch(CurRt,StrBuff,  CurIdx, (temp+5)*8 );

		HelpStr.Format(" (0->ITU-T|ISO/IEC Reserved)\r\n");
		HelpStr.AppendFormat(" (1->ISO/IEC 11172 Video)\r\n");
		HelpStr.AppendFormat(" (2->ITU-T Rec.H.262|ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video)\r\n");
		HelpStr.AppendFormat(" (3->ISO/IEC 11172 Audio)\r\n");
		HelpStr.AppendFormat(" (4->ISO/IEC 13818-3 Audio)\r\n");
		HelpStr.AppendFormat(" (5->ITU-T Rec. H.222.0|ISO/IEC 13818-1 private_sections)\r\n");
		HelpStr.AppendFormat(" (6->ITU-T Rec. H.222.0|ISO/IEC 13818-1 PES packets containing private data)\r\n");
		HelpStr.AppendFormat(" (7->ISO/IEC 13522 MHEG)\r\n");
		HelpStr.AppendFormat(" (8->ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM CC)\r\n");
		HelpStr.AppendFormat(" (9->ITU-T Rec. H.222.1)\r\n");
		HelpStr.AppendFormat(" (10->ISO/IEC 13818-6 type A)\r\n");
		HelpStr.AppendFormat(" (11->ISO/IEC 13818-6 type B)\r\n");
		HelpStr.AppendFormat(" (12->ISO/IEC 13818-6 type C)\r\n");
		HelpStr.AppendFormat(" (13->ISO/IEC 13818-6 type D)\r\n");
		HelpStr.AppendFormat(" (14->ISO/IEC 13818-1 auxiliary)\r\n");
		HelpStr.AppendFormat(" (15-0x80->ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved)");
		HelpStr.AppendFormat(" (other->User Private)");
		
		StrBuff.Format("stream_type");
		AddDefaultBranch(pHTree,StrBuff,CurIdx,8 ,pU8[0]);

		StrBuff.Format("elementary_PID");
		AddDefaultBranch(pHTree,StrBuff,CurIdx+11,13,(pU8[1]& 0x1F)*256 + pU8[2]);

		StrBuff.Format("ES_info_Descriptor_loop_length");
		AddDefaultBranch(pHTree,StrBuff,CurIdx+28,12,temp);

		pU8 += 5;
		ParseDescriptorLoop(pHTree,pU8, temp, CurIdx+40);
		pU8 += temp;
		SectionLeft -= (temp+5);
		CurIdx = CurIdx+(temp+5)*8;
	}
	
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	AddDefaultBranch(CurRt,StrBuff,CurIdx,32,CRC);
	return true;
}

bool CStandardSectionFormat::ParseTSDTContent(CommonSection * pCS , CContentTree * CurRt)
{
	ASSERT(0);
	return true;
}

bool CStandardSectionFormat::ParseBATContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	CContentTree * tempRt;
	BATSection * pPS= (BATSection *)pCS;
	int SectionLength = pPS->section_length_hi*256+pPS->section_length_low;
	int temp,DescriptorCount=0;

	u8 *pU8 = (u8 *)pCS ;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pPS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pPS->section_syntax_indicator);
	AddReserveBranch(CurRt,9,3);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLength);
	
	temp = pPS->bouquet_id_hi * 256 + pPS->bouquet_id_low;
	StrBuff.Format("bouquet_id");
	AddDefaultBranch(CurRt,StrBuff,24,16,temp);
	AddReserveBranch(CurRt,40,2);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pPS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pPS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pPS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pPS->last_section_number);
	AddReserveBranch(CurRt,64,4);

	temp = pPS->bouquet_descriptors_length_hi * 256 + pPS->bouquet_descriptors_length_low;
	StrBuff.Format("bouquet_descriptors_loop_length");
	AddDefaultBranch(CurRt,StrBuff,68,12,temp);
	pU8 += 10;
	ParseDescriptorLoop(CurRt, pU8, temp,80);
	AddReserveBranch(CurRt,(temp * 8 ) + 80,4);

	int CurIdx = (temp * 8 ) + 84;
	pU8 += temp;
	temp = (pU8[0] & 0xF)* 256 + pU8[1];
	StrBuff.Format("transport stream loop length");
	//tempRt=AddDefaultBranch(CurRt,StrBuff,CurIdx,12);
	AddDefaultBranch(CurRt,StrBuff,CurIdx,12,temp);
	CurIdx = CurIdx + 12;
	pU8+=2;
	u8 temp2;
	while(temp >= 6)
		{
		temp2 = (pU8[4] & 0xF) * 256 + pU8[5];
		StrBuff.Format("transport stream");
		tempRt = AddSubBodyBranch(CurRt,StrBuff, CurIdx, (temp2+6) * 8);

		StrBuff.Format("stream id");
		AddDefaultBranch(tempRt,StrBuff, CurIdx , 16,(pU8[0] * 256 + pU8[1]));
		CurIdx += 16;
		StrBuff.Format("original network id");
		AddDefaultBranch(tempRt,StrBuff,CurIdx , 16,(pU8[2] * 256 + pU8[3]));
		AddReserveBranch(tempRt,(CurIdx ) + 16,4);

		CurIdx += 20;
		temp2 = (pU8[4] & 0xF)* 256 + pU8[5];
		StrBuff.Format("transport descriptor loop length");
		AddDefaultBranch(tempRt,StrBuff,CurIdx,12, temp2);
		CurIdx += 12;
		ParseDescriptorLoop(tempRt, pU8+6, temp2,CurIdx);
		CurIdx += temp2 * 8;
		temp -= (temp2 + 6);
		pU8+=(temp2 + 6);
		}
	pU8 += temp;/* normally temp should be 0 this time */
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	StrBuff.Format("CRC_32");
	AddDefaultBranch(CurRt,StrBuff, CurIdx , 32,CRC);
	return true;
}

bool CStandardSectionFormat::ParseTDTContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;	
	u8 * pU8 = (u8 *)pCS;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,  0, 8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff, 12,12,pCS->section_length_hi*256+pCS->section_length_low);

	StrBuff.Format("UTC_Date");
	//AppendMJDTime(pU8[3]*256 + pU8[4] , StrBuff);
	AddFieldBranch(CurRt, StrBuff  ,24,16,FIELD_FMT_MJD,pU8[3]*256 + pU8[4] );

	StrBuff.Format("UTC_Time(2:2:2 BCD code)");
	//StrBuff.AppendFormat("%d%d:%d%d:%d%d",pU8[5]>>4, pU8[5] & 0xF,pU8[6]>>4, pU8[6] & 0xF,pU8[7]>>4, pU8[7] & 0xF);
	AddFieldBranch(CurRt,StrBuff, 40,24,
		FIELD_FMT_BCD,pU8[5]* 256 *256 + pU8[6]*256 + pU8[7]);

	/*pU8 += 8;
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	StrBuff.AppendFormat(DATAFORMAT,CRC);
	AddDefaultBranch(CurRt,StrBuff,64,32);*/
	return true;
}

bool CStandardSectionFormat::ParseRSTContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;	
	u8 * pU8 = (u8 *)pCS;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,  0, 8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	u8 SectLeft = pCS->section_length_hi*256+pCS->section_length_low;
	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff, 12,12,SectLeft);
	pU8 += 3;
	int CurIdx = 24;
	while(SectLeft >= 9)
		{
		StrBuff.Format("transport_stream_id");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,16, pU8[0] * 256 + pU8[1]);
		CurIdx += 16;
		StrBuff.Format("original_network_id");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,16, pU8[2] * 256 + pU8[3]);
		CurIdx += 16;
		StrBuff.Format("service_id");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,16, pU8[4] * 256 + pU8[5]);
		CurIdx += 16;
		StrBuff.Format("event_id");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,16, pU8[6] * 256 + pU8[7]);
		CurIdx += (16+5);
		StrBuff.Format("running_status");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,3, pU8[8] & 0x07);
		CurIdx += 3;
		pU8 += 9;
		SectLeft -= 9;
		}
	return true;
}

bool CStandardSectionFormat::ParseSTContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;	
	u8 * pU8 = (u8 *)pCS;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,  0, 8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	u8 SectLeft = pCS->section_length_hi*256+pCS->section_length_low;
	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff, 12,12,SectLeft);
	
	StrBuff.Format("data_byte_length");
	AddDefaultBranch(CurRt,StrBuff,24,SectLeft * 8, SectLeft);
		
	return true;
}

bool CStandardSectionFormat::ParseTOTContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;	
	u8 * pU8 = (u8 *)pCS;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff, 12 ,12,pCS->section_length_hi*256+pCS->section_length_low);


	StrBuff.Format("UTC_Date");
	AddFieldBranch(CurRt, StrBuff  ,24,16,FIELD_FMT_MJD,pU8[3]*256 + pU8[4] );

	StrBuff.Format("UTC_Time(2:2:2 BCD code)");
	AddFieldBranch(CurRt,StrBuff, 40,24,
		FIELD_FMT_BCD,pU8[5]* 256 *256 + pU8[6]*256 + pU8[7]);
	/*
	StrBuff.Format("UTC_Time");
	AppendMJDTime(pU8[3]*256 + pU8[4] , StrBuff);
	StrBuff.AppendFormat("%d%d:%d%d:%d%d",pU8[5]>>4, pU8[5] & 0xF,pU8[6]>>4, pU8[6] & 0xF,pU8[7]>>4, pU8[7] & 0xF);
	AddDefaultBranch(CurRt,StrBuff, 24,40);
	*/
	u8 DescLoopLenth = (pU8[8] & 0xF) * 256 + pU8[9];
	StrBuff.Format("descriptor_loop_length");
	AddDefaultBranch(CurRt,StrBuff,68,12,DescLoopLenth);
	
	pU8 += 10;
	ParseDescriptorLoop(CurRt , pU8 , DescLoopLenth, 80);
	pU8 += DescLoopLenth;
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	AddDefaultBranch(CurRt,StrBuff,80 + DescLoopLenth * 8, 32,CRC);
	return true;
}

bool CStandardSectionFormat::ParseDITContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;	
	u8 * pU8 = (u8 *)pCS;
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,  0, 8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	u8 SectLeft = pCS->section_length_hi*256+pCS->section_length_low;
	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff, 12,12,SectLeft);
	
	StrBuff.Format("transition_flag");
	AddDefaultBranch(CurRt,StrBuff,24,1, pU8[3] & 0x01);
		
	return true;
}

bool CStandardSectionFormat::ParseSITContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	//CContentTree *  pHTree;
	int SectionLeft = pCS->section_length_hi*256+pCS->section_length_low;
	int temp;//,DescriptorCount=0;

	u8 *pU8 = (u8 *)pCS ;
	
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLeft);
	
	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pCS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pCS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pCS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pCS->last_section_number);
	pU8 += 8;
	temp = (pU8[0] & 0xF)* 256 + pU8[1];
	StrBuff.Format("transmission_info_loop_length");/*12*/
	AddDefaultBranch(CurRt,StrBuff,68,12,temp);
	pU8 += 2;
	ParseDescriptorLoop(CurRt, pU8, temp,80);

	pU8 += temp;
	SectionLeft -= (temp + 7);
	u32 CurIdx = 80+8*temp;
	while(SectionLeft >= 8)
		{
		StrBuff.Format("service_id");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,16,(pU8[0]*256) + pU8[1] );
		CurIdx += (16+1);
		StrBuff.Format("running_status");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,3,((pU8[2]>>4) & 0x7));
		CurIdx += 3;
		temp = (pU8[2]& 0x0F)*256 + pU8[3];
		StrBuff.Format("service_loop_length");
		AddDefaultBranch(CurRt,StrBuff,CurIdx,12,temp);
		CurIdx += 12;
		pU8 += 4;
		ParseDescriptorLoop(CurRt,pU8, temp, CurIdx);
		pU8 += temp;
		SectionLeft -= (temp+4);
		CurIdx = CurIdx+temp*8;
		}
	
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	AddDefaultBranch(CurRt,StrBuff,CurIdx,32,CRC);
	return true;
}
	
bool CStandardSectionFormat::ParseSDTContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	CContentTree *  pHTree;
	int SectionLeft = pCS->section_length_hi*256+pCS->section_length_low;
	u32 temp;

	u8 *pU8 = (u8 *)pCS ;
	
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLeft);
	
	temp = pU8[3] * 256 + pU8[4];
	StrBuff.Format("transport stream id");
	AddDefaultBranch(CurRt,StrBuff,24,16,temp);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pCS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pCS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pCS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pCS->last_section_number);

	pU8 += 8;
	StrBuff.Format("original network id");
	AddDefaultBranch(CurRt,StrBuff,64,16 ,pU8[0] * 256 + pU8[1]);
	pU8 += 3;
	SectionLeft -= 8;
	u32 CurIdx = 88;
	while(SectionLeft >= 9)
	{
		temp = (pU8[3] & 0xF) * 256 + pU8[4];
		StrBuff.Format("service");
		pHTree = AddSubBodyBranch(CurRt,StrBuff, CurIdx, (temp+5) * 8);

		StrBuff.Format("service id");
		AddDefaultBranch(pHTree,StrBuff, CurIdx,16,pU8[0] * 256 + pU8[1]);
		CurIdx += 22;
		StrBuff.Format("EIT schedule flag");
		AddDefaultBranch(pHTree,StrBuff, CurIdx , 1,(pU8[2] & 0x2)>>1 );
		CurIdx += 1;
		StrBuff.Format("EIT present following flag");
		AddDefaultBranch(pHTree,StrBuff, CurIdx ,1 ,pU8[2] & 0x1);
		CurIdx += 1;
		StrBuff.Format("running status");
		AddDefaultBranch(pHTree,StrBuff, CurIdx , 3,pU8[3] >> 5 );
		CurIdx += 3;
		StrBuff.Format("free CA mode");
		AddDefaultBranch(pHTree,StrBuff,CurIdx ,1 ,(pU8[3] >> 4) & 0x1);
		CurIdx += 1;
		
		StrBuff.Format("descriptors loop length");
		AddDefaultBranch(pHTree,StrBuff, CurIdx, 12,temp);
		CurIdx += 12;
		pU8 += 5;
		ParseDescriptorLoop(pHTree , pU8 , temp,CurIdx);
		CurIdx += temp * 8;
		SectionLeft -= (temp + 5);
		pU8 += temp;
	}
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	AddDefaultBranch(CurRt,StrBuff,CurIdx , 32,CRC);
	return true;
}

bool CStandardSectionFormat::ParseNITContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	CContentTree *  pHTree;
	int SectionLeft = pCS->section_length_hi*256+pCS->section_length_low;
	int temp;

	u8 *pU8 = (u8 *)pCS ;
	
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLeft);
	
	temp = pU8[3] * 256 + pU8[4];
	StrBuff.Format("network id");
	AddDefaultBranch(CurRt,StrBuff,24,16,temp);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pCS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pCS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pCS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pCS->last_section_number);

	pU8 += 8;
	temp = (pU8[0] & 0xF) * 256 + pU8[1];
	StrBuff.Format("network descriptors loop length");
	AddDefaultBranch(CurRt,StrBuff, 68,12,temp);
	pU8 += 2;
	ParseDescriptorLoop(CurRt , pU8 , temp,80);
	pU8 += temp;
	u16 CurIdx = 84+temp*8;
	temp =  (pU8[0] & 0xF) * 256 + pU8[1];
	StrBuff.Format("transport stream loop length");
	AddDefaultBranch(CurRt,StrBuff, CurIdx,12,temp);
	CurIdx += 12;
	pU8 += 2;
	u8 temp2;
	while(temp >= 6)
	{
		temp2 = (pU8[4] & 0xF) * 256 + pU8[5];
		StrBuff.Format("transport stream");
		pHTree = AddSubBodyBranch(CurRt,StrBuff, CurIdx, (temp2+6) * 8);
		
		StrBuff.Format("stream id");
		AddDefaultBranch(pHTree,StrBuff, CurIdx, 16,pU8[0] * 256 + pU8[1]);
		CurIdx += 16;
		StrBuff.Format("original network id");
		AddDefaultBranch(pHTree,StrBuff, CurIdx , 16,pU8[2] * 256 + pU8[3]);
		CurIdx += (16 +4);
		StrBuff.Format("transport descriptors loop length");
		AddDefaultBranch(pHTree,StrBuff, CurIdx,12,temp2);
		CurIdx += 12;
		pU8 += 6;
		ParseDescriptorLoop(pHTree , pU8 , temp2,CurIdx);
		CurIdx += (temp2 * 8);
		temp -= (temp2 + 6);
		pU8 += temp2;
	}
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	AddDefaultBranch(CurRt,StrBuff,CurIdx ,32,CRC);
	return true;
}

bool CStandardSectionFormat::ParseEITContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	CContentTree *  pHTree;
	int SectionLeft = pCS->section_length_hi*256+pCS->section_length_low;
	int temp;

	u8 *pU8 = (u8 *)pCS ;
	
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLeft);
	
	temp = pU8[3] * 256 + pU8[4];
	StrBuff.Format("service id");
	AddDefaultBranch(CurRt,StrBuff,24,16,temp);

	StrBuff.Format("version_number");
	AddDefaultBranch(CurRt,StrBuff,42,5,pCS->version_number);
	
	StrBuff.Format("current_next_indicator");
	AddDefaultBranch(CurRt,StrBuff,47,1,pCS->current_next_indicator);

	StrBuff.Format("section_number");
	AddDefaultBranch(CurRt,StrBuff,48,8,pCS->section_number);

	StrBuff.Format("last_section_number");
	AddDefaultBranch(CurRt,StrBuff,56,8,pCS->last_section_number);
	
	StrBuff.Format("transport stream id");
	AddDefaultBranch(CurRt,StrBuff, 64,16,pU8[8] * 256 + pU8[9]);

	StrBuff.Format("original network id");
	AddDefaultBranch(CurRt,StrBuff, 80,16,pU8[10] * 256 + pU8[11]);

	StrBuff.Format("segment last section number");
	AddDefaultBranch(CurRt,StrBuff, 96,8,pU8[12]);

	StrBuff.Format("last table id");
	AddDefaultBranch(CurRt,StrBuff, 104,8,pU8[13]);

	pU8 += 14;
	SectionLeft -= 11;
	u16 CurIdx = 112;
	while(SectionLeft >= (12 + 4))
		{
		StrBuff.Format("event id");
		pHTree = AddDefaultBranch(CurRt,StrBuff, CurIdx , 16,pU8[0] * 256 + pU8[1]);
		CurIdx += 16;

		StrBuff.Format("start date");
		AddFieldBranch(pHTree, StrBuff  ,CurIdx,16,
			FIELD_FMT_MJD,pU8[2]*256 + pU8[3]);

		//AppendMJDTime(pU8[2]*256 + pU8[3] , StrBuff);
		StrBuff.Format("start time(2:2:2 BCD code)");
		//StrBuff.AppendFormat("%d%d:%d%d:%d%d",pU8[4]>>4, pU8[4] & 0xF,pU8[5]>>4, pU8[5] & 0xF,pU8[6]>>4, pU8[6] & 0xF);
		//AddDefaultBranch(pHTree,StrBuff, CurIdx , 40);
		temp = (pU8[4]<<16) + (pU8[5]<<8) + pU8[6];
		AddFieldBranch(pHTree, StrBuff ,CurIdx+16,24,FIELD_FMT_BCD, temp);

		CurIdx +=  40;
		StrBuff.Format("duration");
		AddDefaultBranch(pHTree,StrBuff, CurIdx,24 ,(pU8[7]<<16)  | (pU8[8] << 8) |  pU8[9]);
		CurIdx += 24;
		StrBuff.Format("running status");
		AddDefaultBranch(pHTree,StrBuff,   CurIdx ,3,pU8[10] >> 5);
		CurIdx += 3;
		StrBuff.Format("free CA mode");
		AddDefaultBranch(pHTree,StrBuff,   CurIdx , 1,(pU8[10] >> 4) & 0x1);
		CurIdx += 1;
		temp = (pU8[10] & 0xF) * 256 + pU8[11];
		StrBuff.Format("descriptors loop length");
		AddDefaultBranch(pHTree,StrBuff, CurIdx,12,temp);
		CurIdx += 12;
		pU8 += 12;
		ParseDescriptorLoop(pHTree , pU8 , temp, CurIdx);
		CurIdx += (temp * 8);
		SectionLeft -= (temp + 12);
		pU8 += temp;
		}
	StrBuff.Format("CRC_32");
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	AddDefaultBranch(CurRt,StrBuff,CurIdx,32,CRC);
	return true;
}

#if 0
bool CStandardSectionFormat::ParseCAMessageContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	CAMessageSection * pPS= (CAMessageSection *)pCS;
	int SectionLength = pPS->section_length_hi*256+pPS->section_length_low;

	u8 *pU8 = (u8 *)pPS ;
	StrBuff.Format("table_id : 0x%x	", pPS->table_id);
	AddDefaultBranch(CurRt,StrBuff,0,8);

	StrBuff.Format("section_syntax_indicator : 0x%x", pPS->section_syntax_indicator);
	AddDefaultBranch(CurRt,StrBuff,8,1);

	StrBuff.Format("section_length : 0x%x", SectionLength);
	AddDefaultBranch(CurRt,StrBuff,12,12);
	if(pPS->table_id == ENTITLE_MANAGE_MESSAGE)
		{
			u32 SCNum = pPS->card_number1 * 256 * 256 * 256 + pPS->card_number2 * 256 * 256 + pPS->card_number3 * 256 + pPS->card_number4;
			StrBuff.Format("SMART CARD Address : 0x%x", SCNum);
			AddDefaultBranch(CurRt,StrBuff,48,32);
		}

	pU8 += (SectionLength+3-4);
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	StrBuff.Format("CRC_32 : 0x%x",CRC);
	AddDefaultBranch(CurRt,StrBuff,(SectionLength+3-4)*8  ,32);
	return true;
}
#else
bool CStandardSectionFormat::ParsePrivateTableContent(CommonSection * pCS , CContentTree * CurRt)
{
	CString StrBuff;
	int SectionLength = pCS->section_length_hi*256+pCS->section_length_low;

	u8 *pU8 = (u8 *)pCS ;	
	StrBuff.Format("table_id");
	AddDefaultBranch(CurRt,StrBuff,0,8,pCS->table_id);

	StrBuff.Format("section_syntax_indicator");
	AddDefaultBranch(CurRt,StrBuff,8,1,pCS->section_syntax_indicator);

	StrBuff.Format("private_indicator");
	AddDefaultBranch(CurRt,StrBuff,9,1,pCS->private_indicator);
	
	StrBuff.Format("section_length");
	AddDefaultBranch(CurRt,StrBuff,12,12,SectionLength);

	if(pCS->section_syntax_indicator)
	{
		int sub_table_id = pCS->sub_table_id_hi*256 + pCS->sub_table_id_low;
		StrBuff.Format("table_id_extension");
		AddDefaultBranch(CurRt,StrBuff,24,16,sub_table_id);

		StrBuff.Format("version_number");
		AddDefaultBranch(CurRt,StrBuff,42,5,pCS->version_number);
		
		StrBuff.Format("current_next_indicator");
		AddDefaultBranch(CurRt,StrBuff,47,1,pCS->current_next_indicator);

		StrBuff.Format("section_number");
		AddDefaultBranch(CurRt,StrBuff,48,8,pCS->section_number);

		StrBuff.Format("last_section_number");
		AddDefaultBranch(CurRt,StrBuff,56,8,pCS->last_section_number);

		StrBuff.Format("private_data");
		pU8 += 8;
		if(SectionLength <= 5)
		{
		}
		else if((SectionLength /*- 4*/ - 5) <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < (SectionLength /*- 4*/ - 5) ; i ++)
			{
				value <<= 8;
				value |= *(pU8);
				pU8 ++;
			}
			AddDefaultBranch(CurRt,StrBuff,64,(SectionLength /*- 4*/ - 5)*8, value);
		}
		else
		{
			AddDefaultBranch(CurRt,StrBuff,64,(SectionLength /*- 4*/ - 5)*8, (u64)(pU8));
		}
	}
	else
	{
		StrBuff.Format("private_data");
		pU8 += 3;
		if(SectionLength == 0)
		{
		}
		else if((SectionLength /*- 4*/ ) <= VALUE_MAX_LENGTH_IN_BYTE)
		{
			u64 value = 0;
			for(u8 i = 0; i < (SectionLength /*- 4*/ ) ; i ++)
			{
				value <<= 8;
				value |= *(pU8);
				pU8 ++;
			}
			AddDefaultBranch(CurRt,StrBuff,24,(SectionLength /*- 4*/ )*8, value);
		}
		else
		{
			AddDefaultBranch(CurRt,StrBuff,24,(SectionLength /*- 4*/)*8, (u64)(pU8));
		}
	}
	/*
	pU8 += (SectionLength+3-4);
	u32 CRC = pU8[0] * 256 * 256 * 256 + pU8[1] * 256 * 256 + pU8[2] * 256 + pU8[3];
	StrBuff.Format("CRC_32");
	StrBuff.AppendFormat(DATAFORMAT,CRC);
	AddDefaultBranch(CurRt,StrBuff,(SectionLength+3-4)*8  ,32);
	*/
	return true;
}
#endif

bool CStandardSectionFormat::ParseDescriptorLoop(CContentTree * parent, u8 * pDescriptorData, u32 DescriptorDataLength,u32 BaseBitIdx)
{	
	u32 descriptor_lenth ;
	while(DescriptorDataLength	>=	2)
	{
		descriptor_lenth = ParseDescriptor(parent,pDescriptorData,BaseBitIdx);
		
		if((descriptor_lenth+2) > DescriptorDataLength)
			return false;
		DescriptorDataLength -= (descriptor_lenth + 2);
		pDescriptorData += (descriptor_lenth + 2);
		BaseBitIdx += (descriptor_lenth + 2)* 8;
	}
	return true;
}

u8 CStandardSectionFormat::ParseDescriptor(CContentTree * parent, u8 * pDescriptorData ,u32 BaseBitIdx)
{	
	u8 descriptor_lenth = 0;
	if(PrivateDescriptorParse( parent, pDescriptorData, BaseBitIdx))
	{
		descriptor_lenth = pDescriptorData[1];
	}
	else if(pDescriptorData[0] <= DESCRIPTOR_MPEG_TAG_BIGEST)
	{
		descriptor_lenth = (u8)ParseMPEG2DescriptorContent(parent,pDescriptorData,BaseBitIdx);
	}
	else
	{
		descriptor_lenth = (u8)ParseDVBDescriptorContent(parent,pDescriptorData,BaseBitIdx);
	}
	return descriptor_lenth;
}

