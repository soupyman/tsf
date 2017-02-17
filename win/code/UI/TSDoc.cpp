// CTSDoc.cpp :  CTSDoc 类的实现
//

#include "stdafx.h"
#include "include.h"
#include "DvbTsa.h"
#include "TSDoc.h"
#include "DialogInput.h"
#include "SystemConfig.h"
#include "EditSection.h"
#include "VideoPlayDlg.h"

#include "MainFrm.h"

#ifdef _DEBUG

#define new DEBUG_NEW

#endif

// CTSDoc

IMPLEMENT_DYNCREATE(CTSDoc, CDocument)

BEGIN_MESSAGE_MAP(CTSDoc, CDocument)

/*    
POPUP "System(&S)"
    BEGIN
        MENUITEM "Config",                			ID_SYSTEM_CONFIG
        MENUITEM "User define format load",             ID_SYSTEM_LOAD_USER_DEFINE_FORMAT
        MENUITEM SEPARATOR
        MENUITEM "Exit(&X)",                    		ID_APP_EXIT
    END*/	
	//ON_COMMAND(ID_SYSTEM_CONFIG, OnSystemConfig)
	ON_COMMAND(ID_SYSTEM_LOAD_USER_DEFINE_FORMAT, OnSystemLoadFormat)

/*POPUP "TS(&T)"
    BEGIN
        MENUITEM "Open",                			ID_TS_FILE_OPEN
        MENUITEM "Close",              	 		ID_TS_FILE_CLOSE
    END*/	
	ON_COMMAND(ID_TS_FILE_OPEN, OnOpenTSFile)
	ON_COMMAND(ID_TS_FILE_CLOSE, OnCloseTSFile)
/* POPUP "ES(&E)"
    BEGIN
        MENUITEM "Parse",                        		ID_ES_PARSE
        MENUITEM "Change Pid",                        	ID_ES_CHANGE_PID
        MENUITEM "Save as new PID",        		ID_ES_SAVE_NEW_PID
        MENUITEM "Save As",                        		ID_ES_SAVE_AS
        MENUITEM "Load",                        		ID_ES_LOAD
        MENUITEM "Delete"					ID_ES_DELETE
    END*/	
	ON_COMMAND(ID_ES_PARSE, OnESParse)
	ON_COMMAND(ID_ES_CHANGE_PID, OnESChangePID)
	ON_COMMAND(ID_ES_SAVE, OnESSave)
	ON_COMMAND(ID_ES_SAVE_AS, OnESSaveAs)
	ON_COMMAND(ID_ES_LOAD, OnESLoad)
	ON_COMMAND(ID_ES_DELETE, OnESDelete)

/* POPUP "SI/PSI(&P)"
    BEGIN

        MENUITEM "PAT",                         ID_PARSE_PAT
        MENUITEM "CAT",                         ID_PARSE_CAT
        MENUITEM "PMT",                         ID_PARSE_PMT
        MENUITEM "TSDT",                        ID_PARSE_TSDT
        MENUITEM "NIT_A",                       ID_PARSE_NIT_A
        MENUITEM "NIT_O",                       ID_PARSE_NIT_O
        MENUITEM "SDT_A",                       ID_PARSE_SDT_A
        MENUITEM "SDT_O",                       ID_PARSE_SDT_O
        MENUITEM "BAT",                         ID_PARSE_BAT
        MENUITEM "PFEIT_A",                     ID_PARSE_PFEIT_A
        MENUITEM "PFEIT_O",                     ID_PARSE_PFEIT_O
        MENUITEM "SEIT_A",                      ID_PARSE_SEIT_A
        MENUITEM "SEIT_O",                      ID_PARSE_SEIT_O
        MENUITEM "TDT",                         ID_PARSE_TDT
        MENUITEM "RST",                         ID_PARSE_RST
        MENUITEM "TOT",                         ID_PARSE_TOT
        MENUITEM "DIT",                         ID_PARSE_DIT
        MENUITEM "SIT",                         ID_PARSE_SIT
        MENUITEM "ECM_E",                       ID_PARSE_ECM_E
        MENUITEM "ECM_O",                       ID_PARSE_ECM_O
        MENUITEM "EMM",                         ID_PARSE_EMM
        MENUITEM "Private",                 ID_PARSE_SPEC_SI_PSI
        MENUITEM "ALL",                  ID_PARSE_ALL_TABLES
        MENUITEM "Save",                        	ID_SI_PSI_SAVE
        MENUITEM "Save As",                        	ID_SI_PSI_SAVE_AS
        MENUITEM "Load",                        	ID_SI_PSI_LOAD
        MENUITEM "Delete"				ID_SI_PSI_DELETE
    END*/
	ON_COMMAND(ID_PARSE_PAT, 	OnParsePat)
	ON_COMMAND(ID_PARSE_CAT, 	OnParseCat)
	ON_COMMAND(ID_PARSE_PMT, 	OnParsePmt)
	ON_COMMAND(ID_PARSE_TSDT, 	OnParseTsdt)
	ON_COMMAND(ID_PARSE_NIT_A, 	OnParseNitA)
	ON_COMMAND(ID_PARSE_NIT_O, 	OnParseNitO)
	ON_COMMAND(ID_PARSE_SDT_A, 	OnParseSdtA)
	ON_COMMAND(ID_PARSE_SDT_O, 	OnParseSdt0)
	ON_COMMAND(ID_PARSE_BAT, 	OnParseBat)
	ON_COMMAND(ID_PARSE_PFEIT_A, OnParsePfeitA)
	ON_COMMAND(ID_PARSE_PFEIT_O, OnParsePfeitO)
	ON_COMMAND(ID_PARSE_SEIT_A, OnParseSeitA)
	ON_COMMAND(ID_PARSE_SEIT_O, OnParseSeitO)
	ON_COMMAND(ID_PARSE_TDT, OnParseTdt)
	ON_COMMAND(ID_PARSE_RST, OnParseRst)
	ON_COMMAND(ID_PARSE_TOT, OnParseTot)
	ON_COMMAND(ID_PARSE_DIT, OnParseDit)
	ON_COMMAND(ID_PARSE_SIT, OnParseSit)
	ON_COMMAND(ID_PARSE_EMM, OnParseEmm)
	ON_COMMAND(ID_PARSE_ECM_E, OnParseEcmE)
	ON_COMMAND(ID_PARSE_ECM_O, OnParseEcmO)
	ON_COMMAND(ID_PARSE_SPEC_SI_PSI, OnParseSpecTable)
	//ON_COMMAND(ID_PARSE_ALL_TABLES, OnParseAllTables)
	//ON_COMMAND(ID_SI_PSI_SAVE, OnSIPSISave)
	ON_COMMAND(ID_SI_PSI_SAVE_AS, OnSIPSISaveAs)
	//ON_COMMAND(ID_SI_PSI_LOAD, OnSIPSILoad)
	//ON_COMMAND(ID_SI_PSI_DELETE, OnSIPSIDelete)

/*POPUP "Index(&I)"
    BEGIN
    BEGIN
        MENUITEM "Save",                        ID_INDEX_FIRST
        MENUITEM "Undo",                        ID_INDEX_PRIDIFF
        MENUITEM "Add",                         ID_INDEX_PRIVIOUS
        MENUITEM "Delete",                      ID_INDEX_NEXT
    END
*/
	ON_COMMAND(ID_EDIT_EDIT, 		OnEditEdition)
	//ON_COMMAND(ID_EDIT_EDIT, 	OnEditUndo)
	ON_COMMAND(ID_EDIT_SAVE, 	OnEditSave)
	ON_COMMAND(ID_EDIT_DELETE, 		OnEditDelete)
	ON_COMMAND(ID_EDIT_COPY_PART, 	OnEditCopyPart)
	ON_COMMAND(ID_EDIT_SEARCH, 		OnEditSearch)
/*    POPUP "Play"
    BEGIN
        MENUITEM "Test",                        ID_PLAY_TEST
    END
*/
	ON_COMMAND(ID_PLAY_TEST, 		OnPlayTest)
END_MESSAGE_MAP()

// CTSDoc 构造/析构

CTSDoc::CTSDoc()
{
	pTable  = NULL;
	pSection = NULL;
	DocDataLen = 0;
	DocDataFocusStart = 0;
	DocDataFocusLen = 0;
		
	DocInfo =	"Information View \r\n";
	Stream = NULL;
	pESServer = NULL;
	pTblServer = NULL;
	//pBuilderThread = NULL;
	MainWinThreadID = UI_CLIENT_ID;
	m_sPathName.Empty();
	//pCurTable	= NULL;
	pCurES		= NULL;
	//SecNum		= 0;/* may be cause error */
	//pPkt = new Packet;
   	//TableCount = 0;
   	//CurEdit = false;
}

CTSDoc::~CTSDoc()
{
	TSFileClosed();
}

// CTSDoc 序列化

void CTSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
// CTSDoc 诊断

#ifdef _DEBUG
void CTSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL CTSDoc::RequestTable(u16 TableID, u16 PID)
{
	if(MainWinThreadID)
		return pTblServer->SubscribeTable( MainWinThreadID, TableID, PID);
	else
		return false;
}

/*
void CTSDoc::TempSaveEdit()
{
	assert(0);
	if(pCurTable)
	{
		//pCurTable->ReplaceSectionData(SecNum, DocData, DocDataLen);
		//pCurTable->ParseContent();	
		//pDocTree = pCurTable->ParseContent();
		CurEdit = true;
	}
	if(pCurES)
	{
		//pCurES->GetCurPacket()->SetPktData((void *)DocData);
		//pCurES->ParsedCurPkt();
		
		//pDocTree = pCurES->ParsedCurPkt();
		CurEdit = true;
	}
}
void CTSDoc::AddContentTree(CContentTree * pTree)
{
	if(pDocTree && (pDocTree->GetOrigin() == pTree->GetOrigin()))
	{
		pDocTree = pTree;
	}

	POSITION pb , pos = TreeList.GetHeadPosition();
	CContentTree * pT;
	while(pos)
	{
		pb = pos;
		pT = (CContentTree *)(TreeList.GetNext(pos));
		
		if(pT->GetOrigin() == pTree->GetOrigin())
		{
			TreeList.InsertBefore(pb ,pTree);
			delete pT;
			TreeList.RemoveAt(pb);
			return;
		}
	}
	TreeList.AddTail(pTree);
	
}
*/
void CTSDoc::AddTable(CTable * pTbl)
{
	/*	current table update */
	if((pTable == NULL)||((pTable->GetTableID()== pTbl->GetTableID())&&
			(pTable->GetPID()== pTbl->GetPID())))
	{
		pTable = NULL;
		pSection = NULL;
		pTableListView->PostMessage(MSG_UI_SELECT_CHANGE,(WPARAM)pTbl);
	}
	POSITION pb , pos = TableList.GetHeadPosition();
	CTable * pT;
	while(pos)
	{
		pb = pos;
		pT = (CTable *)(TableList.GetNext(pos));
		
		if((pT->GetTableID()== pTbl->GetTableID())&&
			(pT->GetPID()== pTbl->GetPID()))
		{
			TableList.InsertBefore(pb ,pTbl);
			delete pT;
			TableList.RemoveAt(pb);
			return;
		}
	}
	TableList.AddTail(pTbl);
	
}

/*
void CTSDoc::AddES(CESData * pES)
{
	if(pCurES == pES)
	{
		pDocTree = pES->GetPktTree();
	}
	
	if((pES != NULL)&&(ESList.Find((void *)pES) == NULL))
	{
		ESList.AddTail((void *)pES);
	}
}
*/
// CTSDoc 命令
#define TS
void CTSDoc::OnOpenTSFile()
{
	CFileDialog TsFile(true,NULL,NULL,0,"TS file(*.ts, *.trp or *.mux)|*.ts;*.trp;*.mux");
	if(TsFile.DoModal() == IDOK)
	{
		//LPDWORD       lpdwProcessId =0;
		//MainWinThreadID =  GetWindowThreadProcessId( AfxGetMainWnd()->GetSafeHwnd(),lpdwProcessId);   
		OpenTSFile(TsFile.GetPathName());
	}
}

void CTSDoc::OnCloseTSFile()
{
	AfxGetMainWnd()->PostMessage(MSG_UI_UPDATE_HIDE_MENU);
	TSFileClosed();
	this->SetTitle("DVB_TSA");
}

void CTSDoc::OpenTSFile(CString FilePath)
{
		Stream = new CTSFile(FilePath);
				
		if(Stream->IsValid())
		{
			m_sPathName.Format("%s", FilePath);
			this->SetTitle((LPCTSTR)m_sPathName);
			if(Stream->m_FileWritable)
				DocInfo.AppendFormat("File(%s) Opened writeable;\r\n",(LPCTSTR)m_sPathName);
			else
				DocInfo.AppendFormat("File(%s) Opened read only;\r\n",(LPCTSTR)m_sPathName);

			DocInfo.AppendFormat("Packet Size : %d \r\n",Stream->m_nPacketSize);
			DocInfo.AppendFormat("Packet Number : %d \r\n", Stream->m_nTotalPacketNumber);
			
			if(Stream->m_FileSize > /*CTSFile::*/MAX_TS_PAGE_SIZE)
				DocInfo.AppendFormat("File is very big, be patience when parse \r\n");

			pESServer = new CESDBServer(Stream);
			pTblServer = new CTableDBServer(pESServer);
			//pBuilderThread = AfxBeginThread(BuilderThread ,Stream);
			DocInfo.AppendFormat("%s;\r\n", Stream->GetErrString());

		}
		else
		{
			DocInfo.AppendFormat("File(%s) %s ;\r\n",FilePath, Stream->GetErrString());
			delete Stream;
			Stream = NULL;
		}
		
		UpdateAllViews(NULL,ALL_VIEW);
}

void CTSDoc::TSFileClosed()
{
	m_sPathName.Empty();

	/*	free stream, ESs and tables	*/
	if(pTblServer)
	{
		delete pTblServer;
		pTblServer = NULL;
	}
	if(pESServer)
	{
		delete pESServer;
		pESServer = NULL;
	}
	if(Stream)
	{
		delete Stream;
		Stream = NULL;
	}
	
	/*	free table list	*/
	
	POSITION pos = TableList.GetHeadPosition();
	CTable * pT;
	while(pos)
	{
		pT = (CTable *)(TableList.GetNext(pos));
		delete pT;
	}
	TableList.RemoveAll();
	pTable  = NULL;
	pSection = NULL;	
	DocDataLen = 0;
	DocInfo.AppendFormat("Current File closed;\r\n");

	UpdateAllViews(NULL,ALL_VIEW);
}

#define ES
void CTSDoc::OnESParse()
{
	AfxMessageBox("ES opratore not allowed now.");
	return;

	CDialogPIDIndexInput pDPIDI;
	if(pDPIDI.DoModal() == IDOK)
	{
		assert(0);
		//PostCommand(MSG_CMD_FILTER_ES,pDPIDI._Pid_i,pDPIDI.PacketIndx);
	}
}
void CTSDoc::OnESSave()
{
	AfxMessageBox("ES opratore not allowed now.");
	return;

	assert(0);
	/*
	if(pCurES)
		PostCommand(MSG_CMD_SAVE_ES,pCurES->PID);
	else
		AfxMessageBox("You should select a ES first.");
		*/
}
void CTSDoc::OnESChangePID()
{
	AfxMessageBox("ES opratore not allowed now.");
	return;

	assert(0);
	if(pCurES)
	{
		CDialogParamInput pDPIDI;
		pDPIDI.Title.Format("Please input new PID:");
		pDPIDI.LenInBin = 13;
		//if(pDPIDI.DoModal() == IDOK)
			//PostCommand(MSG_CMD_CHANGE_PID , pCurES->PID , pDPIDI._data_i );
	}
	else
		AfxMessageBox("You should select a ES first.");
}
void CTSDoc::OnESSaveAs()
{
	AfxMessageBox("ES opratore not allowed now.");
	return;

	assert(0);
	if(pCurES)
	{
		//PostCommand(MSG_CMD_SAVE_ES_AS_FILE,pCurES->PID);
	}
	else
		AfxMessageBox("You should select a ES first.");
}
void CTSDoc::OnESLoad()
{
/*
explicit CFileDialog(
   BOOL bOpenFileDialog,
   LPCTSTR lpszDefExt = NULL,
   LPCTSTR lpszFileName = NULL,
   DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
   LPCTSTR lpszFilter = NULL,
   CWnd* pParentWnd = NULL,
   DWORD dwSize = 0
);
*/
	AfxMessageBox("ES opratore not allowed now.");
	return;

	assert(0);
	CFileDialog cfd(true,NULL,NULL,0,"ES file(*.es)|*.es");
	CString * pfn;
	if(cfd.DoModal() == IDOK)
	{
		pfn = new CString;
		*pfn = cfd.GetPathName();
		//PostCommand(MSG_CMD_LOAD_ES, WPARAM(pfn));
	}
}
void CTSDoc::OnESDelete()
{
	AfxMessageBox("ES opratore not allowed now.");
	return;
	assert(0);
	if(pCurES)
	{
		//PostCommand(MSG_CMD_DELETE_ES,pCurES->PID);
	}
	else
		AfxMessageBox("You should select a ES first.");
}
#define SI_PSI
void CTSDoc::OnParsePat()
{
	RequestTable( PROGRAM_ASSOCIATION_INFORMATION, PAT_PID);
}

void CTSDoc::OnParsePmt()
{
	RequestTable( PROGRAM_MAP_INFORMATION, INVALID_PID);
}

void CTSDoc::OnParseCat()
{
	RequestTable(CONDITIONAL_ACCESS_INFORMATION, CAT_PID);
}

void CTSDoc::OnParseEmm()
{
	RequestTable( ENTITLE_MANAGE_MESSAGE,INVALID_PID);
}

void CTSDoc::OnParseEcmE()
{
	RequestTable( ENTITLE_CONTROL_MESSAGE_EVEN, INVALID_PID);
}

void CTSDoc::OnParseEcmO()
{
	RequestTable( ENTITLE_CONTROL_MESSAGE_ODD ,INVALID_PID);
}

void CTSDoc::OnParseTot()
{
	RequestTable(TIME_OFFSET_INFORMATION,TDT_TOT_ST_PID);
}

void CTSDoc::OnParseTdt()
{
	RequestTable(TIME_DATE_INFORMATION,TDT_TOT_ST_PID);
}

void CTSDoc::OnParseNitA()
{
	RequestTable(ACTUAL_NETWORK_INFORMATION,NIT_ST_PID);
}

void CTSDoc::OnParseNitO()
{
	RequestTable(OTHER_NETWORK_INFORMATION,NIT_ST_PID);
}

void CTSDoc::OnParseRst()
{
	RequestTable( RUNING_STATUS_INFORMATION,RST_ST_PID);
}

void CTSDoc::OnParseTsdt()
{	
	RequestTable(TRANSPORT_STREAM_DESCRIPTION,TSDT_PID);
}

void CTSDoc::OnParseBat()
{
	RequestTable(BOUQUET_ASSOCIATION_INFORMATION,SDT_BAT_ST_PID);
}

void CTSDoc::OnParseSdtA()
{
	RequestTable( ACTUAL_TRANSPORT_SERVICE_DESCRIPTION,SDT_BAT_ST_PID);
}
void CTSDoc::OnParseSdt0()
{
	RequestTable( OTHER_TRANSPORT_SERVICE_DESCRIPTION,SDT_BAT_ST_PID);
}

void CTSDoc::OnParsePfeitA()
{
	RequestTable( ACTUAL_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION,EIT_ST_PID);
}

void CTSDoc::OnParsePfeitO()
{
	RequestTable( OTHER_TRANSPORT_PRESENT_FOLLOWING_EVENT_INFORMATION,EIT_ST_PID);
}
void CTSDoc::OnParseSeitA()
{
	for(u8 tid = ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN;
	tid < ACTUAL_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MAX; tid ++)
		{
		RequestTable(tid,EIT_ST_PID);
		}
}

void CTSDoc::OnParseSeitO()
{
	for(u8 tid = OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MIN;
	tid < OTHER_TRANSPORT_SCHEDULE_EVENT_INFORMATION_MAX; tid ++)
		{
		RequestTable(tid,EIT_ST_PID);
		}

}

void CTSDoc::OnParseDit()
{
	RequestTable( DISCONTINUITY_INFORMATION,DIT_PID);
}

void CTSDoc::OnParseSit()
{
	RequestTable(SELECTION_INFORMATION,SIT_PID);
}

void CTSDoc::OnParseSpecTable()
{
	CDialogPIDTableIDInput pDPIDI;
	if(pDPIDI.DoModal() == IDOK)
	{
		RequestTable(pDPIDI._TableID_i,pDPIDI._Pid_i);
	}
}

void CTSDoc::OnSIPSISaveAs()
{
	if(pTable)
	{
		CString DefaultName = FmtSvr.GetNameByTable(pTable);;
		CFileDialog TblFile(false,"tbl",DefaultName,0,"Table file(*.tbl)|*.tbl");
		if(TblFile.DoModal() == IDOK)
		{
			CStdioFile pFile;

			if(! pFile.Open((LPCTSTR)TblFile.GetFileName(),CFile::modeCreate|CFile::modeWrite ))
			{
				AfxMessageBox("Open file for save failed!!!");
				return;
			}
			pFile.SeekToBegin();

			pTable->SaveToFile(&pFile);

			pFile.Close();
		}
	}
	else
		AfxMessageBox("You should select a Table first.");
}
/*
void CTSDoc::OnSIPSILoad()
{

explicit CFileDialog(
   BOOL bOpenFileDialog,
   LPCTSTR lpszDefExt = NULL,
   LPCTSTR lpszFileName = NULL,
   DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
   LPCTSTR lpszFilter = NULL,
   CWnd* pParentWnd = NULL,
   DWORD dwSize = 0
);

	CFileDialog cfd(true,NULL,NULL,0,"Table file(*.tbl)|*.tbl");
	CString * pfn;
	if(cfd.DoModal() == IDOK)
	{
		pfn = new CString;
		*pfn = cfd.GetFileName();
		PostCommand(MSG_CMD_LOAD_TABLE, WPARAM(pfn));
	}
}

void CTSDoc::OnSIPSIDelete()
{
	if(pCurTable)
		PostCommand(MSG_CMD_DELETE_TABLE,pCurTable->GetPID());
	else
		AfxMessageBox("You should select a Table first.");
}
*/
#define EDIT
void CTSDoc::OnEditEdition() 
{
	if(pSection)
	{
		CEditSection pES;
		
		pES.pCntTree = FmtSvr.ParseContentBySection(pSection);;
		pES.NewSecData = DocData;
		
		if((pES.DoModal() == IDOK) && (pES.NewSecLenth))
		{
			DocDataLen = pES.NewSecLenth;

			if(pSection)
				pSection->ReplaceData(DocData, DocDataLen);

			if(pTable)
				pTable->Edit = true;
			
			UpdateAllViews(NULL,CTSDoc::ALL_VIEW);
		}
	}
}

void CTSDoc::OnEditSave()//ADD
{
	if((pTable)&&(pTable->Edit))
	{
		CString TableName = FmtSvr.GetNameByTable(pTable);
		CString MsgCnt;
		MsgCnt.Format("Back you file before save, do you really want to save %s to file?",TableName );
		if(AfxMessageBox(MsgCnt ,MB_YESNO ) == IDYES)
		{
			pTblServer->UpdateTable(pTable);
			pTable->Edit = false;
		}
	}
	else if(pTable)
	{
		AfxMessageBox("This table is not edit, nothing need to be save. :)");
	}
	else
	{
		AfxMessageBox("No table selected. :)");
	}
}

void CTSDoc::OnEditDelete()//delete
{
	if(pTable && pSection)
	{
		pTable->DetachSection(pSection);
		pTable->Edit = true;
		pSection = NULL;
		UpdateAllViews(NULL,CTSDoc::ALL_VIEW);
	}
	//pTreeView->PostMessage(MSG_UI_TREE_VIEW_SELECT_DELETE);
}

void CTSDoc::OnEditCopyPart()//copy a part to a new file
{
	CSystemConfig SysCfg;

	pESServer->SavePart(SysCfg.PacketCount);
}

void CTSDoc::OnEditSearch()
{
	CString * text = new CString;
	CMainFrame * pMF = (CMainFrame *)AfxGetMainWnd();
	pMF->m_ComboBox.GetWindowText(*text);
	if(pMF->m_ComboBox.FindString(0,*text) == CB_ERR )
	{
		pMF->m_ComboBox.AddString(*text);

	}
	
	pTreeView->PostMessage(MSG_UI_TREE_VIEW_SEARCH, WPARAM (text));

}

#define PLAY
void CTSDoc::OnPlayTest()
{
	CVideoPlayDlg VPD;
	VPD.pTs = Stream;
	VPD.pTblList = &TableList;
	VPD.DoModal();
}

#define SYSTEM
void CTSDoc::OnSystemLoadFormat()
{
	CFileDialog cfd(true,NULL,NULL,0,"Format define file(*.pff)|*.pff");
	if(cfd.DoModal() == IDOK)
	{
		OpenPrivateFormatFile(cfd.GetPathName() );
	}
}
/* return total offset in bit */
void CTSDoc::OpenPrivateFormatFile(CString FilePath)
{
	CFormatDefineFile  myfile;
	CFormatDatabase FDB;
	if(myfile.Open( FilePath , CFile::modeRead))
	{
		FDB.FreeFormatList();
		if(myfile.UpdateFormatToDB(FDB))
		{
			DocInfo.AppendFormat("Load format successed: \r\n");
			FDB.DumpToStr(DocInfo);
		}
		else
		{
			DocInfo.AppendFormat("Load format error info: \r\n\r\n");
			DocInfo.Append(myfile.ErrorInfo);
			DocInfo.AppendFormat("\r\n");
		}
		myfile.Close();
	}
	else
	{
		DocInfo.AppendFormat("Open format file (%s) error: \r\n", FilePath);
	}
	UpdateAllViews(NULL,INFO_VIEW);
}

#if 0
void CTSDoc::OnFileSavesectioncontent()
{
	CFileDialog cfd(false);
	if(cfd.DoModal() == IDOK)
	{
		CFile file(cfd.GetFileName(),CFile::modeCreate|CFile::modeWrite);
		CString str;
		str.Format("Table ID :%x , PID : %x , Version : %x , Section Number : %x . \r\n\r\n Section Data:  \r\n\r\n",\
			pCurTable->GetTableID(),pCurTable->GetPID(),pCurTable->GetVersion(),SecNum);
		for(u8 i=0; i < DocDataLen ; i++)
		{
			if(DocData[i] > 0xF )
			{
				str.AppendFormat("%x",DocData[i]);
			}
			else
			{
				str.AppendFormat("0%x",DocData[i]);			
			}
		}
		file.Write(str , str.GetLength());
		file.Close();
	}
}

void CTSDoc::OnFileSaveinformation()
{
	CFileDialog cfd(false);
	if(cfd.DoModal() == IDOK)
	{
		CFile file(cfd.GetFileName(),CFile::modeCreate|CFile::modeWrite);
		file.Write(DocInfo , DocInfo.GetLength());
		file.Close();
	}
}
void CTSDoc::OnFileSaveESByPid()
{
	CDialogParamInput pDPIDI;
	pDPIDI.Label.Format("PID:");
	pDPIDI.Title.Format("Please Input the PID");
	if(pDPIDI.DoModal() == IDOK)
	{
		WPARAM wP = pDPIDI._data_i;
		PostCommand(MSG_CMD_SAVE_ES,wP);
	}
}

void CTSDoc::OnWritebackCurrentSelf()
{
	if(pCurTable)
		PostCommand(MSG_CMD_WRITE_BACK_OVER_SELF ,Table, (LPARAM)pCurTable);
	else if(pCurES)
		PostCommand(MSG_CMD_WRITE_BACK_OVER_SELF ,Packet, (LPARAM)pCurES);
}

void CTSDoc::OnWritebackCurrentAll()
{
	if(pCurTable)
		PostCommand(MSG_CMD_WRITE_BACK_OVER_ALL ,Table, (LPARAM)pCurTable);
	else if(pCurES)
		PostCommand(MSG_CMD_WRITE_BACK_OVER_ALL ,Packet, (LPARAM)pCurES);
}

void CTSDoc::OnWritebackAllSelf()
{
	PostCommand(MSG_CMD_WRITE_BACK_OVER_SELF ,AllData);
}

void CTSDoc::OnWritebackAllAll()
{
	PostCommand(MSG_CMD_WRITE_BACK_OVER_ALL ,AllData);
}
	
void CTSDoc::OnEditDeletealltree()
{
	TableList.RemoveAll();
	ESList.RemoveAll();
	pCurTable = NULL;
	pCurES = NULL;
	pDocTree = NULL;
	UpdateAllViews(NULL,ALL_VIEW);
}

void CTSDoc::OnEditDeletecurrenttree()
{
	POSITION pos = TableList.Find((void *)pCurTable);
	if(pos)
	{
		TableList.RemoveAt(pos);
		pCurTable = NULL;
	}
	pos = ESList.Find((void *)pCurES);
	if(pos)
	{
		ESList.RemoveAt(pos);
		pCurES = NULL;
	}
	pDocTree = NULL;
	UpdateAllViews(NULL,ALL_VIEW);
}

void CTSDoc::OnEditDeletetreeexpectcurrent()
{
	TableList.RemoveAll();
	if(pCurTable)
		TableList.AddTail((void *)pCurTable);
	ESList.RemoveAll();
	if(pCurES)
		ESList.AddTail((void *)pCurES);
	UpdateAllViews(NULL,ALL_VIEW);
}

/*void CTSDoc::OnWritebackSelected()
{
	PostCommand(MSG_CMD_WRITE_BACK_TABLE ,pCurTable->GetPID(), pCurTable->GetTableID());
}
*/
#endif

