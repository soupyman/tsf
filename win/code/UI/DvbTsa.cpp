// DVB_TSA.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "MainFrm.h"

#include "TSDoc.h"
#include "TreeListView.h"
#include "VersionListView.h"
#include "DvbTsa.h"
#include "crc_calculater.h"
#include "helpdlg.h"
#include "SystemConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDvbTsaApp

BEGIN_MESSAGE_MAP(CDvbTsaApp, CWinApp)
/*    
POPUP "System(&S)"
    BEGIN
        MENUITEM "Config",                			ID_SYSTEM_CONFIG
        MENUITEM "Private format load",             ID_SYSTEM_LOAD_PRIVATE_FORMAT
        MENUITEM SEPARATOR
        MENUITEM "Exit(&X)",                    		ID_APP_EXIT
    END*/	
	ON_COMMAND(ID_SYSTEM_CONFIG, OnSystemConfig)
	//ON_COMMAND(ID_SYSTEM_LOAD_PRIVATE_FORMAT, OnSystemLoadPrivateFormat)
/*
POPUP "Tools(&T)"
    BEGIN
        MENUITEM "calculator",                  ID_TOOLS_CALCULATOR
        MENUITEM "notepad",                     ID_TOOLS_NOTEPAD
        MENUITEM "paint",                       ID_TOOLS_PAINT
        MENUITEM "CRC",                         ID_TOOLS_CRC
    END
*/
	ON_COMMAND(ID_TOOLS_CALCULATOR, OnToolsCalculator)
	ON_COMMAND(ID_TOOLS_NOTEPAD, OnToolsNotepad)
	ON_COMMAND(ID_TOOLS_PAINT, OnToolsPaint)
	ON_COMMAND(ID_TOOLS_CRC, OnToolsCrc)
/*    
POPUP "Help(&H)"
    BEGIN
        MENUITEM "About DVB_TSA(&A)...",        ID_APP_ABOUT
        MENUITEM "Help",                        ID_HELP_HELP
    END*/
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
END_MESSAGE_MAP()


// CDvbTsaApp 构造

CDvbTsaApp::CDvbTsaApp()
{

	cal_pi.hProcess = NULL;
	note_pi.hProcess = NULL;
	paint_pi.hProcess = NULL;
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDvbTsaApp 对象

CDvbTsaApp theApp;

// CDvbTsaApp 初始化

BOOL CDvbTsaApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTSDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CTableListView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}


#define SYSTEM
void CDvbTsaApp::OnSystemConfig()
{
	//AfxMessageBox("This feature has not been developed yet. :)");
	CSystemConfigDlg SysCfg;
	SysCfg.DoModal();
}
/*
void CDvbTsaApp::OnSystemLoadPrivateFormat()
{
	AfxMessageBox("OnSystemLoadPrivateFormat()");
}
*/
#define HELP
// 用于运行对话框的应用程序命令
void CDvbTsaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
/*
void CDvbTsaApp::OnHelpHelp()
{
	CHelpDlg helpDlg;
	helpDlg.DoModal();
}
*/

/* CDvbTsaApp 消息处理程序*/
#define TOOLS

void CDvbTsaApp::OnToolsCalculator()
{
	if(TerminateProcess(cal_pi.hProcess,0))
	{
		cal_pi.hProcess = NULL;
	}
	else
	{
		STARTUPINFO   si;   
		si.cb   =   sizeof(STARTUPINFO);   
		si.lpReserved   =   NULL;   
		si.lpDesktop   =   NULL;   
		si.dwFlags   =   0;   
		si.lpReserved2   =   NULL;   
		si.cbReserved2   =   0;   
		si.lpTitle   =   NULL;   
		BOOL   success   =   CreateProcess(NULL,   "calc",   
		NULL,    NULL,   FALSE,   0,   NULL,    NULL,    &si,     &cal_pi);   
		if   (!success)   
		{   
			AfxMessageBox("启动进程有误!!!");   
		}   
	}
}

void CDvbTsaApp::OnToolsNotepad()
{
	if(TerminateProcess(note_pi.hProcess,0))
	{
		note_pi.hProcess = NULL;
	}
	else
	{
		STARTUPINFO   si;   
		si.cb   =   sizeof(STARTUPINFO);   
		si.lpReserved   =   NULL;   
		si.lpDesktop   =   NULL;   
		si.dwFlags   =   0;   
		si.lpReserved2   =   NULL;   
		si.cbReserved2   =   0;   
		si.lpTitle   =   NULL;   
		BOOL   success   =   CreateProcess(NULL,   "notepad",   
		NULL,    NULL,   FALSE,   0,   NULL,    NULL,    &si,     &note_pi);   
		if   (!success)   
		{   
			AfxMessageBox("启动进程有误!!!");   
		}   
	}
}

void CDvbTsaApp::OnToolsPaint()
{
	if(TerminateProcess(paint_pi.hProcess,0))
	{
		paint_pi.hProcess =NULL;
	}
	else
	{
		STARTUPINFO   si;   
		si.cb   =   sizeof(STARTUPINFO);   
		si.lpReserved   =   NULL;   
		si.lpDesktop   =   NULL;   
		si.dwFlags   =   0;   
		si.lpReserved2   =   NULL;   
		si.cbReserved2   =   0;   
		si.lpTitle   =   NULL;   
		BOOL   success   =   CreateProcess(NULL,   "mspaint",   
		NULL,    NULL,   FALSE,   0,   NULL,    NULL,    &si,     &paint_pi);   
		if   (!success)   
		{   
			AfxMessageBox("启动进程有误!!!");   
		}   
	}
}


void CDvbTsaApp::OnToolsCrc()
{
	Ccrc_calculater CRCDialog;
	CRCDialog.DoModal();
}


