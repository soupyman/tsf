// DVB_TSA.cpp : ����Ӧ�ó��������Ϊ��
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


// CDvbTsaApp ����

CDvbTsaApp::CDvbTsaApp()
{

	cal_pi.hProcess = NULL;
	note_pi.hProcess = NULL;
	paint_pi.hProcess = NULL;
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDvbTsaApp ����

CDvbTsaApp theApp;

// CDvbTsaApp ��ʼ��

BOOL CDvbTsaApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTSDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CTableListView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
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
// �������жԻ����Ӧ�ó�������
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

/* CDvbTsaApp ��Ϣ�������*/
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
			AfxMessageBox("������������!!!");   
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
			AfxMessageBox("������������!!!");   
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
			AfxMessageBox("������������!!!");   
		}   
	}
}


void CDvbTsaApp::OnToolsCrc()
{
	Ccrc_calculater CRCDialog;
	CRCDialog.DoModal();
}


