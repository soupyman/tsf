#pragma once

#include "TypeDefine.h"


class CMessageSpace 
{
public:
	CMessageSpace(void);
	~CMessageSpace(void);

public:
	
#define	MSG_BASE					WM_USER
#define 	MSG_BUILDER_BASE			MSG_BASE
#define 	MSG_DEMUX_BASE			MSG_BASE+0x20
#define 	MSG_UI_BASE				MSG_BASE+0x40


	enum MSG_FOR_UI
	{
		MSG_UI_BUILDER_THREAD_CREATE_SUCCESS		=	MSG_UI_BASE,
		MSG_UI_DEMUX_THREAD_CREATE_SUCCESS,
		MSG_UI_UPDATE_HIDE_MENU					,
		MSG_UI_UPDATE_SELECT_INDEX					,
		//MSG_UI_TS_FILE_CLOSED				,
		MSG_UI_TS_ATTRIBUTE					,
		MSG_UI_TABLE						,
		MSG_UI_PACKET						,
		MSG_UI_CURRENT_ACTION_FINISHED		,
 		MSG_UI_CURRENT_ACTION_PROCESS		,
		MSG_UI_CURRENT_ACTION_START		,
		//MSG_UI_TABLE_WRITE_BACK_SUCCESSED	,
		//MSG_UI_TABLE_WRITE_BACK_FAILURE		,
		MSG_UI_ES_ATTRIBUTE					,
		MSG_UI_INFORMATION				,

		MSG_UI_SELECT_CHANGE		,
		MSG_UI_TREE_VIEW_SELECT_DELETE		,
		MSG_UI_TREE_VIEW_EDIT_SAVE			,
		MSG_UI_TREE_VIEW_SEARCH                 ,
		MSG_UI_DATA_VIEW_EDIT                       ,
		MSG_UI_PPF_AND_TS_AUTO_OPEN         ,

		MSG_UI_INVALID
	};


	enum MSG_FOR_BUILDER
	{
		MSG_BLD_ANALYZE_TS_ATTRIBUTE	= MSG_BUILDER_BASE,
		MSG_BLD_CHANGE_PID				,
		MSG_BLD_SAVE_ES						,
		MSG_BLD_SAVE_ES_AS_FILE					,
		MSG_BLD_DELETE_ES					,
		MSG_BLD_LOAD_ES					,
		//MSG_BLD_DMX_MAKE_FRIENDS				,
		//MSG_CMD_FILTER_ES					,
		//MSG_CMD_BUILD_ONE_TABLE				,
		//MSG_CMD_BUILD_ALL_TABLE					,
		//MSG_CMD_WRITE_BACK_PACKET				,
		//MSG_CMD_SAVE_TABLE_AS_FILE						,
		//MSG_CMD_DELETE_TABLE					,
		//MSG_CMD_LOAD_TABLE					,
		//MSG_CMD_WRITE_BACK_OVER_SELF			,
		//MSG_CMD_WRITE_BACK_OVER_ALL				,
		//MSG_CMD_QUIT_PROCESS					,
		//MSG_CMD_TABLE_INDEX_CHANGE				,
		//MSG_CMD_PACKET_INDEX_CHANGE			,
		MSG_BLD_SAVE_TABLE						,
    		MSG_BLD_ES_DATA_VALID						    ,

		MSG_BLD_INVALID
	};
    
	enum MSG_FOR_DEMUX
	{
		MSG_DMX_INIT_DEMUX_TS	= MSG_DEMUX_BASE,
		//MSG_DMX_BLD_MAKE_FRIENDS				,
		MSG_DMX_ADD_DEMUX_FILTER			,
		MSG_DMX_ADD_REMUX_FILTER			,			
		MSG_DMX_SAVE_PART_TO_FILE			,
		MSG_DMX_ES_DELETE_OK                        ,

		MSG_DMX_INVALID

	};

	
};

class CMessage : public CMessageSpace
{
public:
	CMessage(void);
	~CMessage(void);
	BOOL MessageToUI(UINT msg, WPARAM wP = 0, LPARAM lP = 0);
	BOOL InformationToUI(LPCSTR pszFormat, ...);

public:
	/*
	typedef struct tagMSG {     // msg  
	   HWND hwnd;
	   UINT message;
	   WPARAM wParam;
	   LPARAM lParam;
	   DWORD time;
	   POINT pt;
	} MSG;
	*/
	MSG   msg;
	
};
