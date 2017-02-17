

#include "tsfile.h"
#include "formatfile.h"

#include <string.h>

#include <iostream>
using namespace std;

using namespace NDVBSpace;
/*
#define SUPPORT_TABLE_COUNT     17
struct TableInfo
{
    s8 name[10];
    u16 tableId;
    u16 pid;
}TableArr[SUPPORT_TABLE_COUNT] =
{
    {
        "PAT",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "CAT",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "PMT",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "TSDT",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "NIT_A",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "NIT_O",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "SDT_A",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "SDT_O",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "BAT",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
     {
        "TDT",
            PROGRAM_ASSOCIATION_INFORMATION,
            PAT_PID
    },
                printf("[0. Back], [1. PAT], [2. CAT], [3. PMT], [4. TSDT], [5. NIT_A], [6. NIT_O], [7. SDT_A], [8. SDT_O]. \n");
            printf("[9. BAT], [10. TDT], [11. RSIT], [12. TOT], [13. DIT], [14. SIT], [15. ECM_EVEN], [16. ECM_ODD], [17. EMM]. \n");
};
*/
#if 0

u16 ReadEmmPid(CTSFile *pts)
{
	CTable tbl;
	u8 Operator;
	u16 PID;

	tbl.SetIds(CONDITIONAL_ACCESS_INFORMATION, CAT_PID, 0, 0);
	if(false == pts->BuildTable(tbl))
	{
		printf("build CAT failed! \n");
		printf("please manual input the PID of EMM. \n");
		scanf("%d", &PID);
		return PID;
	}
	else
	{
		u16 *SubArray =  tbl.AnalyseSubTablePIDArray();
		printf("select EMM pid.\n");
		for(u16 i = 1; i <= SubArray[0]; i++)
		{
			printf("[%d. EMM PID %d],", i, SubArray[i]);
		}
		printf("[ other number for manual input].\n");
		Operator = 0;
		scanf("%d", &Operator);
		if(Operator == 0 || Operator > SubArray[0])
		{
			printf("please manual input the PID of EMM. \n");
			scanf("%d", &PID);
		}
		else
			PID = SubArray[Operator];

		free(SubArray);
		return PID;
	}
}

u16 ReadEcmPid(CTSFile *pts)
{
	CTable tbl;
	u8 Operator;
	u16 PID;

	tbl.SetIds(PROGRAM_ASSOCIATION_INFORMATION, PAT_PID, 0, 0);
	if(false == pts->BuildTable(tbl))
	{
		printf("build PAT failed! \n");
		printf("please manual input the PID of ECM. \n");
		scanf("%d", &PID);
		return PID;
	}
	else
	{
		u16 *SubArray =  tbl.AnalyseSubTablePIDArray();
		printf("select PMT pid.\n");
		for(u16 i = 1; i <= SubArray[0]; i++)
		{
			printf("[%d. PMT PID %d],", i, SubArray[i]);
		}
		printf("[ other number for manual input].\n");
		Operator = 0;
		scanf("%d", &Operator);
		if(Operator == 0 || Operator > SubArray[0])
		{
			printf("please manual input the PID of PMT. \n");
			scanf("%d", &PID);
		}
		else
			PID = SubArray[Operator];

		free(SubArray);

		tbl.SetIds(PROGRAM_MAP_INFORMATION, PID, 0, 0);
		if(false == pts->BuildTable(tbl))
		{
			printf("build PMT(pid:%d) failed! \n", PID);
			printf("please manual input the PID of ECM. \n");
			scanf("%d", &PID);
			return PID;
		}
		else
		{
			SubArray =  tbl.AnalyseSubTablePIDArray();
			printf("select ECM pid.\n");
			for(u16 i = 1; i <= SubArray[0]; i++)
			{
				printf("[%d. ECM PID %d],", i, SubArray[i]);
			}
			printf("[ other number for manual input].\n");
			Operator = 0;
			scanf("%d", &Operator);
			if(Operator == 0 || Operator > SubArray[0])
			{
				printf("please manual input the PID of ECM. \n");
				scanf("%d", &PID);
			}
			else
				PID = SubArray[Operator];

			free(SubArray);

			return PID;
		}
	}
}

u16 ReadPmtPid(CTSFile *pts)
{
	CTable tbl;
	u8 Operator;
	u16 PID;

	tbl.SetIds(PROGRAM_ASSOCIATION_INFORMATION, PAT_PID, 0, 0);
	if(false == pts->BuildTable(tbl))
	{
		printf("build PAT failed! \n");
		printf("please manual input the PID of PMT. \n");
		scanf("%d", &PID);
		return PID;
	}
	else
	{
		u16 *SubArray =  tbl.AnalyseSubTablePIDArray();
		printf("select PMT pid.\n");
		for(u16 i = 1; i <= SubArray[0]; i++)
		{
			printf("[%d. PMT PID %d],", i, SubArray[i]);
		}
		printf("[ other number for manual input].\n");
		Operator = 0;
		scanf("%d", &Operator);
		if(Operator == 0 || Operator > SubArray[0])
		{
			printf("please manual input the PID of PMT. \n");
			scanf("%d", &PID);
		}
		else
		{
			PID = SubArray[Operator];
		}
		free(SubArray);
		return PID;
	}
}

void ExtractSiPsiToXml(CTSFile *pts)
{
	CTable tbl;
	u8 Operator;
	u16 PID;
	CString tblname, filename;
	CStreamFile *pFile ;
	CFormatServer fs;
	CContentTree *pCt;

	while(1)
	{
		Operator = 0;
		printf("\n                  what table u want to extract? \n\n");
		printf("[0. Back], [1. PAT], [2. CAT], [3. PMT], [4. TSDT], [5. NIT_A], [6. NIT_O], [7. SDT_A], [8. SDT_O], ");
		printf("[9. BAT], [10. TDT], [11. RSIT], [12. TOT], [13. DIT], [14. SIT], [15. ECM_EVEN], [16. ECM_ODD], [17. EMM]. \n");
		/*avoid eit in this phase.
		[10. EIT_PF_A], [11. EIT_PF_O], [12. EIT_SCH_A], [13. EIT_SCH_O],*/
		scanf("%d", &Operator);
		switch(Operator)
		{
			case 1:
				tbl.SetIds(PROGRAM_ASSOCIATION_INFORMATION, PAT_PID, 0, 0);
				break;
			case 2:
				tbl.SetIds(CONDITIONAL_ACCESS_INFORMATION, CAT_PID, 0, 0);
				break;
			case 3:
				tbl.SetIds(PROGRAM_MAP_INFORMATION, ReadPmtPid(pts), 0, 0);
				break;
			case 4:
				tbl.SetIds(TRANSPORT_STREAM_DESCRIPTION, TSDT_PID, 0, 0);
				break;
			case 5:
				tbl.SetIds(ACTUAL_NETWORK_INFORMATION, NIT_ST_PID, 0, 0);
				break;
			case 6:
				tbl.SetIds(OTHER_NETWORK_INFORMATION, NIT_ST_PID, 0, 0);
				break;
			case 7:
				tbl.SetIds(ACTUAL_TRANSPORT_SERVICE_DESCRIPTION, SDT_BAT_ST_PID, 0, 0);
				break;
			case 8:
				tbl.SetIds(OTHER_TRANSPORT_SERVICE_DESCRIPTION, SDT_BAT_ST_PID, 0, 0);
				break;
			case 9:
				tbl.SetIds(BOUQUET_ASSOCIATION_INFORMATION, SDT_BAT_ST_PID, 0, 0);
				break;
			case 10:
				tbl.SetIds(TIME_DATE_INFORMATION, TDT_TOT_ST_PID, 0, 0);
				break;
			case 11:
				tbl.SetIds(RUNING_STATUS_INFORMATION, RST_ST_PID, 0, 0);
				break;
			case 12:
				tbl.SetIds(TIME_OFFSET_INFORMATION, TDT_TOT_ST_PID, 0, 0);
				break;
			case 13:
				tbl.SetIds(DISCONTINUITY_INFORMATION, DIT_PID, 0, 0);
				break;
			case 14:
				tbl.SetIds(SELECTION_INFORMATION, SIT_PID, 0, 0);
				break;
			case 15:
				tbl.SetIds(ENTITLE_CONTROL_MESSAGE_EVEN,  ReadEcmPid(pts), 0, 0);
				break;
			case 16:
				tbl.SetIds(ENTITLE_CONTROL_MESSAGE_ODD,  ReadEcmPid(pts), 0, 0);
				break;
			case 17:
				tbl.SetIds(ENTITLE_MANAGE_MESSAGE,  ReadEmmPid(pts), 0, 0);
				break;
			case 0:
			default:
				return;
				break;
		}

		if(false == pts->BuildTable(tbl))
		{
			printf("build table(tableid:%d ; pid:%d) failed! \n", tbl.GetTableID(), tbl.GetPID());
			continue;
		}
		tblname = fs.GetShortName(&tbl);
		filename.Format("%s.txt", tblname.c_str());
		tbl.SaveToTxtFile(filename.c_str());
		printf("save %s to %s. \n", tblname.c_str(), filename.c_str());

		filename.Format("%s.xml", tblname.c_str());
		pFile = new CStreamFile(filename.c_str());
		pFile->Open(CFile::OPEN_TEXT_WRITE_ONLY);
		pFile->SeekTo(0);

		pCt = fs.ParseContent(&tbl);
		pCt->SaveToXmlFile(*pFile, 0);

		pFile->Close();
		delete pFile;
		delete pCt;
		printf("save %s to %s. \n\n", tblname.c_str(), filename.c_str());
	}

}

void RemuxSiPsiFromXml(CTSFile *pts)
{
	CTable table;
	s8 filename[256];
	CContentFile *pCntFile ;
	CString  TraceStr;

	printf("\nplease input the xml file u want load to ts . \n");
	scanf("%s", filename);
	pCntFile = new CContentFile(filename);
	if(false == pCntFile->Load())
	{
		ERR("Load file %s failed. ", filename);
		delete pCntFile;
		return;
	}
	if(false == pCntFile->pCTree->TransferToTable(table))
	{
		pCntFile->pCTree->DumpToString(TraceStr, 0);
		ERR(" %s \nTransferToTable() failed. ", TraceStr.c_str());
		delete pCntFile;
		return;
	}
	pts->RemuxTable(table);
	delete pCntFile;

}
void FormatDbOpr(CTSFile *pts)
{
	u8 Operator;
	CFmtDbInterface FmtDb;
	CString str;
	s8 filename[256];
	CFormatFile *pFF;

	while(1)
	{
		Operator = 0;
		printf("\n[0. Back]. \n");
		printf("[1. Dump Format DB]. \n");
		printf("[2. Load Format File]. \n");

		scanf("%d", &Operator);
		switch(Operator)
		{
			case 0:
				return;
			case 1:
				FmtDb.DumpToStr(str);
				printf("Format data base :\n %s \n", str.c_str());
				break;
			case 2:
				printf("please input the format file u want load to DB . \n");
				scanf("%s", filename);
				pFF = new CFormatFile(filename);
				if(true == pFF->Load())
				{
					INF("Load %s to DB success.\n", filename);
				}
				delete pFF;
				break;
			default:
				break;
		}
	}
}

void ExitTsf(CTSFile *pts)
{
	delete pts;
	exit(0);
}

void test2(CTSFile *pts)
{
	for(pktidx = 0 ; pktidx < pts->GetPacketCount(); pktidx++)
	{
		pts->ReadPacket(&pp, pktidx);
		if(0x500   == pp.GetPID())
		{
			//printf("pktidx %d , GetContinuityCounter( %d ). \n", pktidx, pp.GetContinuityCounter());
			if(pp.GetPayLoadUnitStartIndicator())
			{
				pCAM = (CAMessageSection *)pp.GetSectionAddress();
				if(table_id != pCAM->table_id)
				{
					table_id = pCAM->table_id;
					ecmcnt++;
					if(table_id == ENTITLE_CONTROL_MESSAGE_EVEN)
						printf("pktidx %d ,even ecm\n", pktidx);
					else if(table_id == ENTITLE_CONTROL_MESSAGE_ODD)
						printf("pktidx %d ,odd ecm\n", pktidx);
					else
						printf("pktidx %d ,unknow ecm\n", pktidx);
				}
			}
		}
	}
	// printf("ecmcnt %d .\n",ecmcnt);


}


void ToolsOpr(CTSFile *pts)
{
	u8 Operator;
	CFmtDbInterface FmtDb;
	CString str;
	CFormatFile *pFF;

	while(1)
	{
		Operator = 0;
		printf("\n[0. Back]. \n");
		printf("[1. Defrag TS]. \n");
		printf("[2. Change PID]. \n");
		printf("[3. Extract ES]. \n");
		printf("[4. Parse packet]. \n");

		scanf("%d", &Operator);
		switch(Operator)
		{
			case 0:
				return;
			case 1:/*Defrag TS*/
			{
				u32 PktCnt = 10000;
				CString filename;
				CPacket pp;
				CTSFile *pNewTs;

				printf("How many packets u want to save? \n");
				scanf("%d", &PktCnt);
				filename.Format("Pkt%d_", PktCnt);
				filename.Append(pts->GetFileName());
				if(PktCnt > pts->GetPacketCount())
					PktCnt = pts->GetPacketCount();

				pNewTs = new CTSFile((s8 *)(filename.c_str()));
				pNewTs->OpenForWrite();
				if(false == pNewTs->TraceTsfileState())
				{
					printf("open ts file failed! \n");
				}
				else
				{
					for(u32 pktidx = 0 ; pktidx < PktCnt; pktidx++)
					{
						pts->ReadPacket(pp, pktidx);
						pNewTs->AppendPacket(pp);
					}
					printf("save %d packets to ts %s.\n", PktCnt, filename.c_str());
				}
				delete pNewTs;
			}
			break;
			case 2:/*Change PID*/
			{
				u16 new_PID, old_PID, find_cnt;
				printf("please input the old PID and the new PID . \n");
				scanf("%d%d", &old_PID, &new_PID);
				find_cnt = 0;
				for(u32 pktidx = 0 ; pktidx < pts->GetPacketCount(); pktidx++)
				{
					if(pts->ReadPktPID(pktidx) == old_PID)
					{
						pts->WritePktPID(pktidx, new_PID);
						find_cnt++;
					}
				}
				printf("Change %d packet's PID from %d to %d ... \n", find_cnt, old_PID, new_PID);
			}
			break;
			case 3:/*Extract ES*/
			{
				u16 PID;
				CString filename;
				CPacket pp;
				CTSFile *pNewTs;
				u32 esidx = 0;
				printf("What's the ES pid u want extract? \n");
				scanf("%d", &PID);
				filename.Format("ES(%d)of_", PID);
				filename.Append(pts->GetFileName());
				pNewTs = new CTSFile((s8 *)(filename.c_str()));
				pNewTs->OpenForWrite();
				if(false == pNewTs->TraceTsfileState())
				{
					printf("open es file for save failed! \n");
				}
				else
				{
					for(u32 pktidx = 0 ; pktidx < pts->GetPacketCount(); pktidx++)
					{
						if(pts->ReadPktPID(pktidx) == PID)
						{
							pts->ReadPacket(pp, pktidx);
							pNewTs->AppendPacket(pp);
							esidx++;
						}
					}
					printf("save %d packets to %s.\n", esidx, filename.c_str());
				}
				delete pNewTs;
			}
			break;
			case 4:/*Parse pkt*/
			{
				u16 PID, index;
				CPacket pp;
				CFormatServer fs;
				CContentTree *pCtree;
				CString str;
				printf("please input the PID and the index u want to parse . \n");
				scanf("%d%d", &PID, &index);
				if(index >= pts->GetPacketCount())
				{
					printf("packet index can't more than max value %d . \n", pts->GetPacketCount());
					break;
				}
				u16	find_cnt = 0;
				for(u32 pktidx = 0 ; pktidx < pts->GetPacketCount(); pktidx++)
				{
					if(pts->ReadPktPID(pktidx) == PID)
					{
						find_cnt++;
						if(find_cnt >= (index + 1))
						{
							pts->ReadPacket(pp, pktidx);
							pCtree = fs.ParsePacket(&pp);
							if(pCtree)
							{
								pCtree->DumpToString(str, 0);
								printf("Packet(Pid:%d,index:%d) content: \n%s. \n", PID, index, str.c_str());
								delete pCtree;
							}
							else
								printf("Parse Packet(Pid:%d,index:%d) failed. \n", PID, index);

							break;
						}
					}
				}
				if(find_cnt < index)
					printf("find Packet(Pid:%d,index:%d) failed, index max %d. \n", PID, index, find_cnt);

			}
			break;
			default:
				break;
		}
	}
}
#endif

void ShowAboutInfo()
{
	printf("\ntsf version 1.1 \n");
	printf("<build data:%s; time:%s>\n ", __DATE__, __TIME__);
	printf("author: Soup. \n");
	printf("home page: http://code.google.com/p/tsfriend-on-linux/. \n\n");
}

void LoadDefaultFormat()
{
	CFile::SetWorkDir(".//format//");
	CFormatFile *pFF = new CFormatFile("format.fdf");
	pFF->Load();
	delete pFF;
	CFile::SetWorkDir(".//");
}

bool MenuLoop(void * pParam);

int main(int argc, char *argv[])
{
	CTSFile *pts;
	s8 filename[256];
	ShowAboutInfo();
	LoadDefaultFormat();

	if(1 >= argc)
	{
		printf("please input the ts file name. \n");
		scanf("%s", filename);
	}
	else
		strcpy(filename, argv[1]);

	pts = new CTSFile(filename);
	pts->OpenAndAnalyse();

	if(false == pts->TraceTsfileState())
	{
		printf("open ts file failed! \n");
		delete pts;
		return 0;
	}

	MenuLoop((void * )pts);


	delete pts;
	return 0;
}
