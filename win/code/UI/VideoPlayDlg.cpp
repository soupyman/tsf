// VideoPlayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "VideoPlayDlg.h"
#include "Section.h"
#include "Table.h"

// CVideoPlayDlg 对话框

IMPLEMENT_DYNAMIC(CVideoPlayDlg, CDialog)
CVideoPlayDlg::CVideoPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoPlayDlg::IDD, pParent)
{
	TimePast = 0;
	FirstTimeStamp = 0;
	state = STATE_STOP;
}

CVideoPlayDlg::~CVideoPlayDlg()
{
}

void CVideoPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROGRAMS, ProgramListCtrl);
}


BEGIN_MESSAGE_MAP(CVideoPlayDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CVideoPlayDlg 消息处理程序


LRESULT CVideoPlayDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
		{
	    case WM_WINDOWPOSCHANGED:
			{
			    vo_instance_t * instance;

				instance = (vo_instance_t *) GetWindowLong (GetSafeHwnd(), GWL_USERDATA);
				if(instance)
					 instance->win_pos_chg( instance, (int)(GetSafeHwnd()));

				return 0;
	    	}
		default:
			break;
	    }

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CVideoPlayDlg::OnBnClickedButtonPlay()
{
	POSITION pos = ProgramListCtrl.GetFirstSelectedItemPosition();
	int HotIdx = ProgramListCtrl.GetNextSelectedItem(pos);
	u16 SelectItemData = 0; 
	if((HotIdx >= 0)&&(HotIdx < ProgramListCtrl.GetItemCount()))
	{
		SelectItemData = ProgramListCtrl.GetItemData(HotIdx);
	}
	else
	{
		SelectItemData = ProgramListCtrl.GetItemData(0);
	}
	TRACE("Select program list data is %d \n", SelectItemData);
	if(state == STATE_STOP)
	{
		pM2D.InitDatas();
		pM2D.output->set_win(pM2D.output, (int)(GetSafeHwnd()));
		pM2D.output->win_pos_chg( pM2D.output, (int)(GetSafeHwnd()));
		pM2D.pTs = pTs;
		pM2D.video_pid = SelectItemData; 
		FirstTimeStamp = 0;
		LastPauseFrameCount = 0;
		m_nTimer = SetTimer(VIDEO_TIMER_EVENT_ID, 50 , 0); /* play after 50 millisecond */
		state = STATE_PLAYING; 
		SetDlgItemText(IDC_BUTTON_PLAY, "pause");
		SetWindowText("playing.");
	}
	else if (state == STATE_PAUSED)
	{
		SetDlgItemText(IDC_BUTTON_PLAY, "pause");
		FirstTimeStamp = 0; /* reset first time stamp for synchro to new play time */
		m_nTimer = SetTimer(VIDEO_TIMER_EVENT_ID, 50 , 0); /* play after 50 millisecond */
		state = STATE_PLAYING; 
		SetWindowText("playing.");
	}
	else /*STATE_PLAYING */
	{
		SetDlgItemText(IDC_BUTTON_PLAY, "play");
		KillTimer(m_nTimer);
		LastPauseFrameCount = pM2D.FrameCount;
		state = STATE_PAUSED;
		SetWindowText("paused.");
	}
}


void CVideoPlayDlg::OnBnClickedButtonStop()
{
	/* kill timer if it have */
	SetWindowText("stoped.");
	SetDlgItemText(IDC_BUTTON_PLAY, "play");
	KillTimer(m_nTimer);
	pM2D.FreeDatas();
	SetWindowLong (GetSafeHwnd(), GWL_USERDATA,  NULL);
	state = STATE_STOP;
	Invalidate();
}

void CVideoPlayDlg::OnTimer(UINT nIDEvent)
{
	if(VIDEO_TIMER_EVENT_ID == nIDEvent)
	{
		u32 time_gap;
		u8 DecodeState;
		KillTimer(m_nTimer);

		while(((DecodeState = pM2D.DecodeTsNextPic()) == 0)&&(STATE_PLAYING == state ))
		{
			time_gap = (pM2D.FrameCount - LastPauseFrameCount) * (pM2D.FramePeriod)/27000; //  should time gap in milliseconds
			if(0 == FirstTimeStamp)
				FirstTimeStamp = GetTickCount();;
			
			TimePast = GetTickCount() - FirstTimeStamp;
			//TRACE("ts decode a pic with should time %d ; actual time %d .\n",time_gap, TimePast);
			if(time_gap >  (TimePast+ 1))// need wait
			{
				time_gap -= (TimePast+ 1);
				m_nTimer = SetTimer(VIDEO_TIMER_EVENT_ID, time_gap , 0); 
				return;
			}			
		}

		if(1 == DecodeState)
		{
			state = STATE_STOP;
			SetWindowText("play finished.");
			SetDlgItemText(IDC_BUTTON_PLAY, "play");
		}
		else if(2 == DecodeState)
		{
			state = STATE_STOP;
			SetWindowText("!!! This program scrambled.");
			SetDlgItemText(IDC_BUTTON_PLAY, "play");

		}
		return;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CVideoPlayDlg::UpdateProgramList()
{
	POSITION  pos = pTblList->GetHeadPosition();
	CTable  * pT , * pSdt = NULL;
	PMTSection * pPmtS;
	u16 ProgramNumber, VideoPid;
	CString ServiceName;
	CListCtrl * pListCtrl ;
	u8 ProgramIndex = 0;
	while(pos)
	{
		pT = (CTable *)(pTblList->GetNext(pos));
		
		if(pT->GetTableID()== ACTUAL_TRANSPORT_SERVICE_DESCRIPTION)
		{
			pSdt = pT;
		}
	}
	 pos = pTblList->GetHeadPosition();
	ProgramListCtrl.DeleteAllItems();
	while(pos)
	{
		pT = (CTable *)(pTblList->GetNext(pos));
		if(pT->GetTableID()== PROGRAM_MAP_INFORMATION)
		{
			pPmtS = (PMTSection * )(pT->GetSection()->GetSectionData());
			ProgramNumber = pPmtS->program_number_hi*256 + pPmtS->program_number_low;
			if((NULL == pSdt)||(0 == get_service_name(pSdt, ProgramNumber, ServiceName)))
			{
				ServiceName.Format("Program %d", ProgramNumber);;
			}
			VideoPid = get_video_pid(pT);
			ServiceName.AppendFormat(" vid : %d ",VideoPid);
			if(VideoPid  !=  INVALID_PID )
			{
				//GetDlgItem(IDC_LIST_OF_PROGRAMS);
				ProgramListCtrl.InsertItem(ProgramIndex,(LPCTSTR) ServiceName);
				ProgramListCtrl.SetItemData(ProgramIndex,(DWORD_PTR)VideoPid);
				
				ProgramIndex++;
			}
		}
	}
}

 /*
 param :
 sdt_l, ProgramNumber for input;
 name for output.

 */
 u8  CVideoPlayDlg::get_service_name (CTable * Sdt_l , u16 ProgramNumber , CString & name)
{
    CSection * psec =  Sdt_l->GetSection();
	static u8  service_name[257];
	static u8  service_privoder_name[257];

    u8 * pu8;
    int sec_lenth, des_list_lenth ;
    while(psec)
    {
        pu8 = psec->SectionData;
        sec_lenth = ((pu8[1] & 0xF)*256) + pu8[2];
        pu8 += 11;
        sec_lenth -= 8;
        while(sec_lenth >= 9)
        {
            des_list_lenth = (pu8[3]& 0x0F)*256 + pu8[4] ;
            if((pu8[0] * 256 + pu8[1]) == ProgramNumber)
            {
                pu8 += 5;
                /* go into descriptors for find service descriptor  0x48 */
                while(des_list_lenth > 0)
                {
                    if(pu8[0] == 0x48)
                    {
                        memcpy(service_privoder_name, pu8+4 , pu8[3]);
                        service_privoder_name[pu8[3]] = 0;
                        memcpy(service_name, pu8+4+pu8[3]+1 , pu8[4+pu8[3]]);
                        service_name[ pu8[4+pu8[3]] ] = 0;
				if(pu8[3])
				{
	                    	name.Format("%s from %s", service_name , service_privoder_name );
				}
				else
				{
					name.Format("%s", service_name  );
				}
                        return 1;
                    }
                    des_list_lenth -= (pu8[1] + 2);
                    pu8 += (pu8[1] + 2);
                }
            
                return 0;
            }
            sec_lenth -= (des_list_lenth + 5);
            pu8 += (des_list_lenth + 5);
        }
        psec = psec->pNext;
    }
    return 0;
}

 u16  CVideoPlayDlg::get_video_pid ( CTable * pmt_l)
{
    CSection * psec = pmt_l->GetSection();
    u8 * pu8;
    u16 sec_lenth ;
    while(psec)
    {
        pu8 = psec->SectionData;
        sec_lenth = ((pu8[1] & 0xF)*256) + pu8[2];
        pu8 += (12 + ((pu8[10] & 0xF)*256) + pu8[11]);
        sec_lenth -= (9 + ((pu8[10] & 0xF)*256) + pu8[11]);
        while(sec_lenth > 5)
        {
            if(pu8[0] == 2)
            {
                return (u16) ((pu8[1]& 0x1F)*256 + pu8[2]);
            }
            sec_lenth -= ((pu8[3]& 0x0F)*256 + pu8[4] + 5);
            pu8 += ((pu8[3]& 0x0F)*256 + pu8[4] + 5);
        }
        psec = psec->pNext;
    }
    return INVALID_PID;
}


BOOL CVideoPlayDlg::OnInitDialog()
{
	  __super::OnInitDialog();

	UpdateProgramList();
	return TRUE;  // return TRUE unless you set the focus to a control

}
