// TSDoc.h :  CTSDoc 类的接口
//


#pragma once
#include "typedefine.h"
#include "TwmList.h"
#include "Message.h"
#include "DVBDefine.h"
#include "FormatServer.h"

class CTSDoc : public CDocument , public CDVBSpace, public virtual CMessageSpace
{



protected: // 仅从序列化创建
	CTSDoc();
	DECLARE_DYNCREATE(CTSDoc)

// 属性
public:
	enum ViewType
	{
		TABLE_LIST_VIEW = 0x1,
		VERSION_LIST_VIEW = 0x2,
		TREE_VIEW = 0x4,
		DATA_VIEW = 0x8,
		SELECT_DATA_VIEW = 0x10,
		INFO_VIEW = 0x20,
		ALL_VIEW = 0xFF
	};
/*****************	view pointer 	***************/
	CView 	* pTableListView;
	CView 	* pVersionListView;
	CView 	* pTreeView;
	CView 	* pDataView;
	CView 	* pSltDataView;
	CView 	* pInfoView;
	/*****************	view data 	***************/
	/********** Table List View data source */
	//CTwmList 	TreeList;
	CTwmList 	TableList;
	//CTwmList 	ESList;
	/********** Version&Section List View data source */
	CTable 	*	pTable;
	/********** Tree View data source */
	CSection *	pSection;
	//CContentTree *	pDocTree;
	/********** Data Edit View data source */
	u8	 		DocData[DATAVIEWBUFFERLENTH]; 
	u32			DocDataLen;
	u32			DocDataFocusStart;
	u32			DocDataFocusLen;
	/********** Information View data source */
	CString 		DocInfo; 

	
	/*****************	Golobe data 	***************/
	CFormatServer				FmtSvr;
	
	CString        				m_sPathName;
	u32 						MainWinThreadID;
	CTSFile		*			Stream;
	CESDBServer	*			pESServer;
	CTableDBServer*			pTblServer;

	//twm: following parametre maybe useless
	//CTable   		* 			pCurTable;
	//u8 						SecNum;
	CESData 		* 			pCurES;
	//bool						CurEdit;


public:
	virtual void Serialize(CArchive& ar);
	virtual ~CTSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	//void TempSaveEdit();
	//void AddContentTree(CContentTree * pTree);
	void AddTable(CTable * pTbl);
	//void AddES(CESData * pES);

	//BOOL PostCommand( UINT msg,WPARAM wP = 0, LPARAM lP = 0);
    
	void TSFileClosed();
private:
	BOOL RequestTable(u16 TableID, u16 PID);
public://action of command on menu
        void OpenTSFile(CString FilePath);
        void OpenPrivateFormatFile(CString FilePath);
public:
	afx_msg void OnSystemLoadFormat();
	//bool LoadAllTDF(void);
	
/*POPUP "TS(&T)"
    BEGIN
        MENUITEM "Open",                			ID_TS_FILE_OPEN
        MENUITEM "Close",              	 		ID_TS_FILE_CLOSE
    END*/	
	afx_msg void OnOpenTSFile();
	afx_msg void OnCloseTSFile();
/* POPUP "ES(&E)"
    BEGIN
        MENUITEM "Parse",                        		ID_ES_PARSE
        MENUITEM "Change Pid",                        	ID_ES_CHANGE_PID
        MENUITEM "Save",                        		ID_ES_SAVE
        MENUITEM "Save As",                        		ID_ES_SAVE_AS
        MENUITEM "Load",                        		ID_ES_LOAD
        MENUITEM "Delete"					ID_ES_DELETE
    END*/	
	afx_msg void OnESParse();
	afx_msg void OnESChangePID();
	afx_msg void OnESSave();
	afx_msg void OnESSaveAs();
	afx_msg void OnESLoad();
	afx_msg void OnESDelete();
	
/* POPUP "SI/PSI(&P)"
    BEGIN
    	POPUP "Parse"
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
	        END
        MENUITEM "Save",                        	ID_SI_PSI_SAVE
        MENUITEM "Save As",                        	ID_SI_PSI_SAVE_AS
        MENUITEM "Load",                        	ID_SI_PSI_LOAD
        MENUITEM "Delete"				ID_SI_PSI_DELETE
    END*/
	afx_msg void OnParsePat();
	afx_msg void OnParseCat();
	afx_msg void OnParsePmt();
	//afx_msg void OnParseAllTables();
	afx_msg void OnParseSpecTable();
	afx_msg void OnParseEmm();
	afx_msg void OnParseEcmE();
	afx_msg void OnParseEcmO();
	afx_msg void OnParseTot();
	afx_msg void OnParseTdt();
	afx_msg void OnParseNitA();
	afx_msg void OnParseNitO();
	afx_msg void OnParseRst();
	afx_msg void OnParseTsdt();
	afx_msg void OnParseBat();
	afx_msg void OnParseSdtA();
	afx_msg void OnParseSdt0();
	afx_msg void OnParsePfeitA();
	afx_msg void OnParsePfeitO();
	afx_msg void OnParseSeitA();
	afx_msg void OnParseSeitO();
	afx_msg void OnParseDit();
	afx_msg	void OnParseSit();
	//afx_msg void OnSIPSISave();
	afx_msg void OnSIPSISaveAs();
	//afx_msg void OnSIPSILoad();
	//afx_msg void OnSIPSIDelete();
/*POPUP "Index(&I)"
    BEGIN
        MENUITEM "First",                       ID_INDEX_FIRST
        MENUITEM "PriDiff",                     ID_INDEX_PRIDIFF
        MENUITEM "Privious",                    ID_INDEX_PRIVIOUS
        MENUITEM "Next",                        ID_INDEX_NEXT
        MENUITEM "NextDiff",                    ID_INDEX_NEXTDIFF
        MENUITEM "Last",                        ID_INDEX_LAST
    END
	*/
	afx_msg void OnEditEdition();
	afx_msg void OnEditSave();
	afx_msg void OnEditDelete();
	afx_msg void OnEditCopyPart();
	afx_msg void OnEditSearch();
/*    POPUP "Play"
    BEGIN
        MENUITEM "Test",                        ID_PLAY_TEST
    END
*/

       afx_msg void OnPlayTest();

};


