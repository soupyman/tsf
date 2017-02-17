

#include "packet.h"
#include "tsfile.h"
#include "cstring.h"
#include "formatserver.h"
#include "contenttree.h"
#include "contentfile.h"
#include "formatfile.h"

#include "menu.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

using namespace NDVBSpace;

static   Menu_s *pMenu = NULL;


u16 ReadEmmPid(CTSFile *pts)
{
	CTable tbl;
	u32 Operator;
	u32 PID;

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
	u32 Operator;
	u32 PID;

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
	u32 Operator;
	u32 PID;

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

bool ExtractTable(CTSFile *pts, CTable &tbl)
{
	CString txtfilename, xmlfilename, binfilename;

	//printf("\n\n\n!!!!!!!!!!!!!!!!!size of CFormatServer is %d, I suppose it's 1M  ! \n\n",
	//        sizeof(CFormatServer));

	if(false == pts->BuildTable(tbl))
	{
		printf("build table(tableid:%d, sub tableid:%d; pid:%d) failed! \n",
			   tbl.GetTableID(), tbl.GetSubId(), tbl.GetPID());
		return false;
	}

	txtfilename.Format("%s(%d,%d).txt", tbl.ShortName.c_str(), tbl.GetPID(), tbl.GetTableID());
	xmlfilename.Format("%s(%d,%d).xml", tbl.ShortName.c_str(), tbl.GetPID(), tbl.GetTableID());
	binfilename.Format("%s(%d,%d).bin", tbl.ShortName.c_str(), tbl.GetPID(), tbl.GetTableID());

	tbl.SaveToTxtFile(txtfilename);
	printf("save %s to %s. \n", tbl.ShortName.c_str(), txtfilename.c_str());

	tbl.SaveToXmlFile(xmlfilename);
	printf("save %s to %s. \n\n", tbl.ShortName.c_str(), xmlfilename.c_str());

	tbl.SaveToBinFile(binfilename);
	printf("save %s to %s. \n\n", tbl.ShortName.c_str(), binfilename.c_str());

	return true;
}

bool ExtractPat(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(PROGRAM_ASSOCIATION_INFORMATION, PAT_PID, 0, 0);

	return ExtractTable(pts, tbl);
}

bool ExtractCat(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(CONDITIONAL_ACCESS_INFORMATION, CAT_PID, 0, 0);

	return ExtractTable(pts, tbl);
}

bool ExtractPmt(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(PROGRAM_MAP_INFORMATION, ReadPmtPid(pts), 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractTsdt(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(TRANSPORT_STREAM_DESCRIPTION, TSDT_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractNit_a(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(ACTUAL_NETWORK_INFORMATION, NIT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractNit_o(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(OTHER_NETWORK_INFORMATION, NIT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractSdt_a(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(ACTUAL_TRANSPORT_SERVICE_DESCRIPTION, SDT_BAT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractSdt_o(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(OTHER_TRANSPORT_SERVICE_DESCRIPTION, SDT_BAT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractBat(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(BOUQUET_ASSOCIATION_INFORMATION, SDT_BAT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}

bool ExtractEIT_pf_a(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;
	u32 ServiceID;
	printf("please input the service ID of the EIT_pf_a. \n");
	scanf("%d", &ServiceID);

	tbl.SetIds(ACTUAL_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION, EIT_ST_PID, ServiceID, 0xFFFF);

	return ExtractTable(pts, tbl);
}

bool ExtractEIT_pf_o(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;
	u32 ServiceID;
	printf("please input the service ID of the EIT_pf_o. \n");
	scanf("%d", &ServiceID);

	tbl.SetIds(OTHER_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION, EIT_ST_PID, ServiceID, 0xFFFF);

	return ExtractTable(pts, tbl);
}

bool ExtractTdt(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(TIME_DATE_INFORMATION, TDT_TOT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractRsit(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(RUNING_STATUS_INFORMATION, RST_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractTot(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(TIME_OFFSET_INFORMATION, TDT_TOT_ST_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractDit(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(DISCONTINUITY_INFORMATION, DIT_PID, 0, 0);

	return ExtractTable(pts, tbl);
}

bool ExtractSit(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(SELECTION_INFORMATION, SIT_PID, 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractEcm_e(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(ENTITLE_CONTROL_MESSAGE_EVEN,  ReadEcmPid(pts), 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractEcm_o(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(ENTITLE_CONTROL_MESSAGE_ODD,  ReadEcmPid(pts), 0, 0);

	return ExtractTable(pts, tbl);
}
bool ExtractEmm(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;

	tbl.SetIds(ENTITLE_MANAGE_MESSAGE,  ReadEmmPid(pts), 0, 0);

	return ExtractTable(pts, tbl);
}

bool ExtractOtherTable(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CTable tbl;
	u32 PID,    TableID,	SubID, 	SubMask, yes;
	PID = 0;
	TableID = 0;
	SubID = 0;
	SubMask = 0;
	yes = 0;

	printf("please input the PID of the TABLE. \n");
	scanf("%d", &PID);
	printf("please input the Table ID of the TABLE. \n");
	scanf("%d", &TableID);
	printf("please input the sub ID of the TABLE. \n");
	scanf("%d", &SubID);
	printf("please input the sub mask of the TABLE. \n");
	scanf("%d", &SubMask);

	printf("do u realy want to extract table(tableID :%d, PID : %d, SubID : %d, SubMask : %d) 1:yes,0:no. \n", TableID, PID, SubID, SubMask);
	scanf("%d", &yes);
	if(yes)
	{
		tbl.SetIds(TableID,  PID, SubID, SubMask);

		return ExtractTable(pts, tbl);
	}
	return false;
}

bool FormatDbDumpOpr(void *pParam)
{
	CFmtDbInterface FmtDb;
	CString str;

	FmtDb.DumpToStr(str);
	printf("Format data base :\n %s \n", str.c_str());
	return true;
}

bool FormatDbLoadOpr(void *pParam)
{
	s8 filename[256];
	CFormatFile *pFF;

	printf("please input the format file u want load to DB . \n");
	scanf("%s", filename);
	pFF = new CFormatFile(filename);
	if(true == pFF->Load())
	{
		INF("Load %s to DB success.\n", filename);
	}
	delete pFF;
	return true;
}

bool ToolsCalBitrateOpr(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	pts->CalcBitRate();
	return true;
}

bool ToolsParsePktOpr(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	u32 PID, index;
	CPacket pp;
	CFormatServer fs;
	CContentTree *pCtree;
	CString str;
	printf("please input the PID and the index u want to parse . \n");
	scanf("%d %d", &PID, &index);
	printf("[u input PID %d ; index %d].\n", PID, index);
	if(index >= pts->GetPacketCount())
	{
		printf("packet index %d can't more than max value %d . \n", index, pts->GetPacketCount());
		return false;
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

	return true;

}
bool ToolsExtractEsOpr(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;

	u32 PID;
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
	return true;

}

bool ToolsChangePidOpr(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;


	u32 new_PID, old_PID, find_cnt;
	printf("please input the old PID and the new PID . \n");
	scanf("%d %d", &old_PID, &new_PID);
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
	return true;


}
bool ToolsDefragTsOpr(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
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
	return true;

}

bool ToolsSplitFileOpr(void *pParam)
{
	u32 step , ReadLen;
	CString Newfilename;
	s8 OldFileName[256];
	CFile *pOldFile, * pNewFile;
	u8 *pData;
	u32 FileIdx = 0;

	printf("While file u want split? \n");
	scanf("%s", OldFileName);

	step	= 1000000;
	while((1024 * 4) < step)
	{
		printf("How many bytes u want to set as split step? \n");
		scanf("%d", &step);
	}
	pData = (u8 *)malloc(step + 1);
	pOldFile = new CFile(OldFileName);
	
	if(true ==  pOldFile->Open(CFile::OPEN_TEXT_READ_ONLY))
	{
		while((ReadLen = pOldFile->Read((u8 *)pData, step)))
		{
			Newfilename.Format("%d_", FileIdx++);
			Newfilename.Append(pOldFile->GetFileName());

			pNewFile = new CFile((s8 *)(Newfilename.c_str()));
			pNewFile->Open(CFile::OPEN_TEXT_WRITE_ONLY);
			pNewFile->Write(pData, ReadLen);
			pNewFile->Close();
			delete pNewFile;
		}

		pOldFile->Close();
	}
	else
		DBG("Open file failed.");
	
	delete pOldFile;
	return true;

}
//#define REMUX_DBG

bool RemuxSiPsiFromXml(void *pParam)
{
	CTSFile *pts = (CTSFile *)pParam;
	CString filenameStr;
	CTable table;
	s8 filename[256];

	printf("\nplease input the xml file u want load to ts . \n");
	scanf("%s", filename);
	filenameStr = filename;

	if(true == table.LoadFromXmlFile(filenameStr))
	{

#ifdef REMUX_DBG
		CString  xmlfilename, binfilename;
		xmlfilename.Format("test1.xml");
		binfilename.Format("test1.bin");

		table.SaveToXmlFile(xmlfilename);
		table.SaveToBinFile(binfilename);
#endif

		pts->RemuxTable(table);
	}
	return true;
}

bool FormatDbOpr(void *pParam)
{
	pMenu = &FormatMenu;
	return true;
}

bool ToolsOpr(void *pParam)
{
	pMenu = &ToolsMenu;
	return true;
}
bool ExtractSiPsiToXml(void *pParam)
{
	pMenu = &ExtractSipsiMenu;
	return true;
}

bool MenuLoop(void *pParam)
{
	u32  Operator;
	u32  ItemCnt ;

	pMenu = &MainMenu;
	while(pMenu)
	{
		Operator = 0;
		printf("\n       %s \n\n", pMenu->Title);

		for(ItemCnt = 0 ; NULL != pMenu->Item[ItemCnt].oper ; ItemCnt++)
		{
			printf("       [%d. %s  ].\n", ItemCnt + 1 , pMenu->Item[ItemCnt].name);
		}
		printf("       [other. Back  ].\n\n");

		printf("       u select : ");
		scanf("%d", &Operator);
		printf("\n");

		if((Operator > ItemCnt) || (0 == Operator))
		{
			pMenu = pMenu->father;
		}
		else
		{
			pMenu->Item[Operator - 1].oper(pParam);
		}
	}

	return true;
}
