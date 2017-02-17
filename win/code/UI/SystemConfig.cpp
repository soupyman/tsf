// SystemConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "SystemConfig.h"
#include "assert.h"


CSystemConfig::CSystemConfig()
{
	//init cfg
	SetCfgToDefault();
	/*over write if has save before*/
	ReadCfgOutFile();
}

CSystemConfig::~CSystemConfig()
{
}

void   CSystemConfig::SetCfgToDefault()
{
	ForcePrecheck = TRUE;
	PacketCount = SAVE_PACKET_COUNT_DEFAULT;
	PPFAuto.Empty();
	TSAuto.Empty();
}

BOOL CSystemConfig::SaveCfgToFile()
{
	//Remove(CONFIG_FILE_PATH);
	if(Open(CONFIG_FILE_PATH,CFile::modeCreate|CFile::modeReadWrite))
	{
		u32 ID = SYS_CFG_ID_PKT_CNT , Lenth = 4;
		Write((void *)&ID,SYS_CFG_ID_LENTH_IN_FILE);
		Write((void *)&Lenth,SYS_CFG_LENTH_LENTH_IN_FILE);
		Write((void *)&PacketCount,Lenth);
		
		ID = SYS_CFG_ID_FORCE_PRECHECK , Lenth = 1;
		Write((void *)&ID,SYS_CFG_ID_LENTH_IN_FILE);
		Write((void *)&Lenth,SYS_CFG_LENTH_LENTH_IN_FILE);
		Write((void *)&ForcePrecheck,Lenth);
		
		ID = SYS_CFG_ID_AUTO_OPEN_PPF , Lenth = PPFAuto.GetLength();
		Write((void *)&ID,SYS_CFG_ID_LENTH_IN_FILE);
		Write((void *)&Lenth,SYS_CFG_LENTH_LENTH_IN_FILE);
		Write((void *) (LPCTSTR)PPFAuto ,Lenth);
		
		ID = SYS_CFG_ID_AUTO_OPEN_TS , Lenth = TSAuto.GetLength();
		Write((void *)&ID,SYS_CFG_ID_LENTH_IN_FILE);
		Write((void *)&Lenth,SYS_CFG_LENTH_LENTH_IN_FILE);
		Write((void *) (LPCTSTR)TSAuto,Lenth);

		Close();
		
		/*CFileStatus fs;
		GetStatus(fs);
		fs.m_attribute = CFile::hidden;
		CFile::SetStatus(CONFIG_FILE_PATH,fs); */
		return true;
	}
	return false;
}

BOOL CSystemConfig::ReadCfgOutFile()
{
	if(Open(CONFIG_FILE_PATH, CFile::modeRead))
	{
		u32 ID , Lenth;
		u8 data[MAX_CFG_DATA_LENTH];
		while(Read((void *)&ID,SYS_CFG_ID_LENTH_IN_FILE)\
			&&Read((void *)&Lenth,SYS_CFG_LENTH_LENTH_IN_FILE)&&(Lenth <= MAX_CFG_DATA_LENTH)\
			&&(Lenth == Read((void *)data,Lenth)))
		{
			if(0 != Lenth)
			{
				switch(ID)
				{
					case SYS_CFG_ID_PKT_CNT:
						PacketCount = data[3]*256*256*256 + data[2]*256*256 + data[1]*256 + data[0];
						break;
					case SYS_CFG_ID_FORCE_PRECHECK:
						ForcePrecheck = data[0];
						break;
					case SYS_CFG_ID_AUTO_OPEN_PPF:
						data[Lenth] = 0;
						PPFAuto = data;
						break;
					case SYS_CFG_ID_AUTO_OPEN_TS:
						data[Lenth] = 0;
						TSAuto = data;
						break;
					default:
						/*just ignore it for compatible in different versions */

						break;
				}
			}
		}

		Close();
		assert(Lenth <= MAX_CFG_DATA_LENTH);
		
		return true;
	}
	return false;
}

// CSystemConfig 对话框

IMPLEMENT_DYNAMIC(CSystemConfigDlg, CDialog)

BEGIN_MESSAGE_MAP(CSystemConfigDlg, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_PPF, OnBnClickedButtonPPF)
	ON_BN_CLICKED(IDC_BUTTON_TS, OnBnClickedButtonTS)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_PKT_COUNT, OnEnChangeEditPktCount)
END_MESSAGE_MAP()


CSystemConfigDlg::CSystemConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemConfigDlg::IDD, pParent)
{
}

CSystemConfigDlg::~CSystemConfigDlg()
{
}


void CSystemConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CString temp;
	temp.Format("%d",PacketCount);
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PKT_COUNT, temp);
	PacketCount = atoi( LPCTSTR(temp));
	DDX_Check(pDX, IDC_CHECK_FORCE_PRECHECK, ForcePrecheck);
}

void CSystemConfigDlg::OnBnClickedButtonPPF()
{
	CFileDialog TsFile(true,NULL,NULL,0,"Private format file(*.pff)|*.pff");
	TsFile.DoModal() ;
	{/*if(TsFile.DoModal()  != OK), TsFile.GetPathName() will == NULL, that's also what we want .*/
		PPFAuto = TsFile.GetPathName();
		GetDlgItem(IDC_EDIT_AUTO_LOAD_PPF)->SetWindowText(PPFAuto);
	}
}
void CSystemConfigDlg::OnBnClickedButtonTS()
{
	CFileDialog TsFile(true,NULL,NULL,0," file(*.ts, *.trp or *.mux)|*.ts;*.trp;*.mux");
	TsFile.DoModal();
	{
		TSAuto = TsFile.GetPathName();
		GetDlgItem(IDC_EDIT_AUTO_LOAD_TS)->SetWindowText(TSAuto);
	}
}

void CSystemConfigDlg::OnBnClickedButtonApply()
{
	UpdateData(true);
	SaveCfgToFile();
}

void CSystemConfigDlg::OnBnClickedOk()
{
	UpdateData(true);
	SaveCfgToFile();
	OnOK();
}

void CSystemConfigDlg::OnEnChangeEditPktCount()
{
	CString temp1, temp2;
	GetDlgItem(IDC_EDIT_PKT_COUNT)->GetWindowText(temp1);
	PacketCount = atoi( LPCTSTR(temp1));
	temp2.Format("%d",PacketCount);
	if(temp1.CompareNoCase(temp2))
	{
		GetDlgItem(IDC_EDIT_PKT_COUNT)->SetWindowText(temp2);
	}
}

BOOL CSystemConfigDlg::OnInitDialog()
{
	__super::OnInitDialog();

	GetDlgItem(IDC_EDIT_AUTO_LOAD_PPF)->SetWindowText(PPFAuto);
	GetDlgItem(IDC_EDIT_AUTO_LOAD_TS)->SetWindowText(TSAuto);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
