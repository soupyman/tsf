// DVB_TSA.h : TSReaderUI 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号


// CDvbTsaApp:
// 有关此类的实现，请参阅 DVB_TSA.cpp
//

class CDvbTsaApp : public CWinApp
{
public:
	CDvbTsaApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	DECLARE_MESSAGE_MAP()
/*    
POPUP "System(&S)"
    BEGIN
        MENUITEM "Config",                			ID_SYSTEM_CONFIG
        MENUITEM "Private format load",             ID_SYSTEM_LOAD_PRIVATE_FORMAT
        MENUITEM SEPARATOR
        MENUITEM "Exit(&X)",                    		ID_APP_EXIT
    END*/	
	afx_msg void OnSystemConfig();
	//afx_msg void OnSystemLoadPrivateFormat();
/*
POPUP "Tools(&T)"
    BEGIN
        MENUITEM "calculator",                  ID_TOOLS_CALCULATOR
        MENUITEM "notepad",                     ID_TOOLS_NOTEPAD
        MENUITEM "paint",                       ID_TOOLS_PAINT
        MENUITEM "CRC",                         ID_TOOLS_CRC
    END
*/
	afx_msg void OnToolsCalculator();
	afx_msg void OnToolsNotepad();
	afx_msg void OnToolsPaint();
	afx_msg void OnToolsCrc();
/*    
POPUP "Help(&H)"
    BEGIN
        MENUITEM "About DVB_TSA(&A)...",        ID_APP_ABOUT
        MENUITEM "Help",                        ID_HELP_HELP
    END*/
	afx_msg void OnAppAbout();
	//afx_msg void OnHelpHelp();
	
private:
	PROCESS_INFORMATION   cal_pi; 
	PROCESS_INFORMATION   note_pi; 
	PROCESS_INFORMATION   paint_pi; 
};

extern CDvbTsaApp theApp;
