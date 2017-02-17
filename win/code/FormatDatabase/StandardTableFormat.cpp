#include "StdAfx.h"
#include "include.h"


CStandardTableFormat::CStandardTableFormat(void)
{
}

CStandardTableFormat::~CStandardTableFormat(void)
{
}

CContentTree * CStandardTableFormat::ParseContent(CTable * pTbl)
{
	CContentTree *		Content;

	Content = ParseContentRoot(pTbl);	
	
	CSection * pSec = pTbl->GetSection();
	while(pSec)
	{
		Content->AddChild(ParseContentBySection(pSec));
		pSec = pSec->pNext;
	}

	return Content;
}
CContentTree * CStandardTableFormat::ParseContentRoot(CTable * pTbl)
{	
	CString Title;
	Title = GetNameByTable(pTbl);
	//Title.AppendFormat(DATAFORMAT,PID);

	return AddTableBranch(NULL,Title, (u64)pTbl );
}

CString CStandardTableFormat::GetNameByTable(CTable * pTbl)
{	
	CString  Name;
	/*Get private name first, only if failed then get standard name */
	if(GetPrivateTableTitle(pTbl->GetTableID(), Name))
		return Name;
	
	switch(pTbl->GetTableID())
	{
		case PROGRAM_ASSOCIATION_INFORMATION:
			Name.Format("PAT (PROGRAM_ASSOCIATION_INFORMATION)");
			break;
		case CONDITIONAL_ACCESS_INFORMATION:
			Name.Format("CAT (CONDITIONAL_ACCESS_INFORMATION)");
			break;
		case PROGRAM_MAP_INFORMATION:
			Name.Format("PMT (PROGRAM_MAP_INFORMATION)");
			break;
		case TRANSPORT_STREAM_DESCRIPTION:
			Name.Format("TSDT (TRANSPORT_STREAM_DESCRIPTION)");
			break;
		case ACTUAL_NETWORK_INFORMATION:
			Name.Format("NIT_A (ACTUAL_NETWORK_INFORMATION)");
			break;
		case OTHER_NETWORK_INFORMATION:
			Name.Format("NIT_O (OTHER_NETWORK_INFORMATION)");
			break;
		case ACTUAL_TRANSPORT_SERVICE_DESCRIPTION:
			Name.Format("SDT_A (ACTUAL_TRANSPORT_SERVICE_DESCRIPTION)");
			break;
		case OTHER_TRANSPORT_SERVICE_DESCRIPTION:
			Name.Format("SDT_O (OTHER_TRANSPORT_SERVICE_DESCRIPTION)");
			break;
		case BOUQUET_ASSOCIATION_INFORMATION:
			Name.Format("BAT (BOUQUET_ASSOCIATION_INFORMATION)");
			break;
		case ACTUAL_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION:
			Name.Format("EIT_A_PF (ACTUAL_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION)");
			break;
		case OTHER_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION:
			Name.Format("EIT_O_PF (OTHER_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION)");
			break;
		case TIME_DATE_INFORMATION:
			Name.Format("TDT (TIME_DATE_INFORMATION)");
			break;
		case RUNING_STATUS_INFORMATION:
			Name.Format("RST (RUNING_STATUS_INFORMATION)");
			break;
		case STUFFING_SECTION:
			Name.Format("SST (STUFFING_SECTION)");
			break;
		case TIME_OFFSET_INFORMATION:
			Name.Format("TOT (TIME_OFFSET_INFORMATION)");
			break;
		case DISCONTINUITY_INFORMATION:
			Name.Format("DIT (DISCONTINUITY_INFORMATION)");
			break;
		case SELECTION_INFORMATION:
			Name.Format("SIT (SELECTION_INFORMATION)");
			break;
		case ENTITLE_CONTROL_MESSAGE_EVEN:
			Name.Format("ECM_E (ENTITLE_CONTROL_MESSAGE_EVEN)");
			break;
		case ENTITLE_CONTROL_MESSAGE_ODD:
			Name.Format("ECM_O (ENTITLE_CONTROL_MESSAGE_ODD)");
			break;
		case ENTITLE_MANAGE_MESSAGE:
			Name.Format("EMM (ENTITLE_MANAGE_MESSAGE)");
			break;
		default:
		{
			if((pTbl->GetTableID() & 0xF0) == ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN)
				Name.Format("EIT_A_SC (ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION)");
			else if((pTbl->GetTableID() & 0xF0) == OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN)
				Name.Format("EIT_O_SC (OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION)");
			else
				Name.Format("UNRECOGNIZED_TABLE(tableID = %x )",pTbl->GetTableID());
		}
			break;
	}

	return Name;
}

