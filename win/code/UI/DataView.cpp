// UnitInfoView.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "DataView.h"
#include "ContentTree.h"
#include "TSDoc.h"
#include "crc.h"
#include "builder.h"
#include "dataview.h"
// CDataView

IMPLEMENT_DYNCREATE(CDataView, CEditView)

CDataView::CDataView()
{
	Modifyed = false;
}

CDataView::~CDataView()
{
}

BEGIN_MESSAGE_MAP(CDataView, CEditView)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
END_MESSAGE_MAP()


// CDataView 诊断

#ifdef _DEBUG
void CDataView::AssertValid() const
{
	CEditView::AssertValid();
}

void CDataView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// CDataView 消息处理程序

void CDataView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(lHint & CTSDoc::DATA_VIEW)
	{
		CEdit & ThisEdit = this->GetEditCtrl();
		int FocusIdx = GetDocument()->DocDataFocusStart +GetDocument()->DocDataFocusLen-1 ;/*last first*/
		int FocusLen =  GetDocument()->DocDataFocusLen;

		Content.Empty();
		if((GetDocument()->DocDataLen)&&(GetDocument()->DocDataFocusLen))
		{
			u8 * pData = (GetDocument()->DocData + (FocusIdx/8));
			u8  Data, Bit;
			u8 CurData = 0;
			u8 CurDataIdx = 0;
			CString Cur;
			FocusIdx = (FocusIdx%8);
			while(FocusLen)
			{
				Data = *pData;
				for(; (FocusIdx >= 0) && (FocusLen) ;FocusIdx --)
				{
					Bit =Data;
					Bit = (Bit>>(7-FocusIdx)) & 0x1;
					CurData |=  Bit<<CurDataIdx;
					CurDataIdx ++;
					if(CurDataIdx >= 8)
					{
						Cur.Format("%02x", CurData);
						Content.Insert(0,Cur);
						CurData = 0;
						CurDataIdx = 0;
					}
					FocusLen --;
				}
				FocusIdx = 7;
				pData--;
			}
			if(CurDataIdx)
			{
				Cur.Format("%02x", CurData);
				Content.Insert(0,Cur);
			}
			//Content.Insert(0,"0x");
		}
		ThisEdit.SetWindowText((LPCTSTR)Content);

	}
}

CTSDoc * CDataView::GetDocument(void)
{
	return (CTSDoc *)CView::GetDocument();;
}

void CDataView::TraceOutData(u8 * pData, u32 Lenth)
{
	GetDocument()->DocInfo.AppendFormat("new Field Data:");
	u32 Idx =0 ;
	while(Lenth > Idx)
	{
		GetDocument()->DocInfo.AppendFormat(" %2x ",pData[Idx++]);
	}
	GetDocument()->DocInfo.AppendFormat("\r\n");
}

void CDataView::UpdataData()
{
	CEdit & ThisEdit = this->GetEditCtrl();
	//if(Modifyed)
	{
		static u8	 	DataBuff[DATAVIEWBUFFERLENTH]; 
		Content.Empty();
		ThisEdit.GetWindowText(Content);
		Content.MakeUpper();
		//int lenth = Content.GetLength();
		//u32 DataLen = lenth;

		u8 * pData = DataBuff;
			
		memset(pData ,0 , DATAVIEWBUFFERLENTH);
		int BitIdx = 0 ;
		while(Content.GetLength())
		{
			if((Content.Right(1) == "0"))
			{
				DataBuff[BitIdx/2]  |=  0<<(4- 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "1")
			{
				DataBuff[BitIdx/2]  |=  1<<(4- 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "2")
			{
				DataBuff[BitIdx/2]  |=  2<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "3")
			{
				DataBuff[BitIdx/2]  |=  3<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "4")
			{
				DataBuff[BitIdx/2]  |=  4<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "5")
			{
				DataBuff[BitIdx/2]  |=  5<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "6")
			{
				DataBuff[BitIdx/2]  |=  6<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "7")
			{
				DataBuff[BitIdx/2]  |=  7<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "8")
			{
				DataBuff[BitIdx/2]  |=  8<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "9")
			{
				DataBuff[BitIdx/2]  |=  9<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "A")
			{
				DataBuff[BitIdx/2]  |=  0xA<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "B")
			{
				DataBuff[BitIdx/2]  |=  0xB<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "C")
			{
				DataBuff[BitIdx/2]  |=  0xC<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "D")
			{
				DataBuff[BitIdx/2]  |=  0xD<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "E")
			{
				DataBuff[BitIdx/2]  |=  0xE<<(4 - 4*(BitIdx%2));
			}
			else if(Content.Right(1) == "F")
			{
				DataBuff[BitIdx/2]  |=  0xF<<(4 - 4*(BitIdx%2));
			}
			else
			{
				MessageBox("Data input error,you should only input Binary data");
				//GetDocument()->UpdateAllViews(NULL,CTSDoc::DATA_VIEW|CTSDoc::TREE_VIEW);
				return;
			}
			Content.Delete(Content.GetLength()-1);
			
			BitIdx ++;
		}
		{
			//GetDocument()->DocData[]
		}
		//GetDocument()->pSltDataView->PostMessage(MSG_UI_DATA_VIEW_EDIT,(WPARAM)DataBuff);

		Modifyed = false;
	}
}

void CDataView::OnEnKillfocus()
{
	UpdataData();
}

void CDataView::OnEnChange()
{	
	Modifyed = true;
	GetEditCtrl().SetModify(FALSE);   
}

void CDataView::OnInitialUpdate()
{
	CEdit & ThisEdit = this->GetEditCtrl();
	CEditView::OnInitialUpdate();
	GetDocument()->pDataView = this;
}

