// UnitInfoView.cpp : 实现文件
//

#include "stdafx.h"
#include "DvbTsa.h"
#include "DataView.h"
#include "ContentTree.h"
#include "TSDoc.h"
#include "crc.h"
#include "builder.h"
#include "SelectDataView.h"
// CSelectDataView

IMPLEMENT_DYNCREATE(CSelectDataView, CEditView)

CSelectDataView::CSelectDataView()
{
	Modifyed = false;
}

CSelectDataView::~CSelectDataView()
{
}

BEGIN_MESSAGE_MAP(CSelectDataView, CEditView)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CSelectDataView 诊断

#ifdef _DEBUG
void CSelectDataView::AssertValid() const
{
	CEditView::AssertValid();
}

void CSelectDataView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// CSelectDataView 消息处理程序

void CSelectDataView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(lHint & CTSDoc::SELECT_DATA_VIEW)
	{
		CEdit & ThisEdit = this->GetEditCtrl();
		int FocusStart = GetDocument()->DocDataFocusStart ;
		int FocusEnd = GetDocument()->DocDataFocusStart + GetDocument()->DocDataFocusLen;
		//ThisEdit.Clear();
		Content.Empty();
		if(GetDocument()->DocDataLen)
		{
			u8 * pData = GetDocument()->DocData;
			u8  Data, Bit;
			u32 i=0;
			while(i< (GetDocument()->DocDataLen))
			{
				Data = *pData;
				for(int bs = 7; bs >= 0;bs --)
				{
					//if(((i*8) + (7 - bs)) == FocusStart)
					//	Content.AppendFormat(" ");
					Bit =Data;
					Bit = (Bit>>bs) & 0x1;
					Content.AppendFormat("%x", Bit);
					//if(((i*8) + (7 - bs)) == (FocusEnd -1))
					//	Content.AppendFormat(" ");
				}
				pData++;
				i++;
			}
		}
		ThisEdit.SetWindowText((LPCTSTR)Content);
// +1 for start space
		ThisEdit.SetSel(FocusStart,FocusEnd);

	}
}

CTSDoc * CSelectDataView::GetDocument(void)
{
	return (CTSDoc *)CView::GetDocument();;
}



void CSelectDataView::UpdataData()
{
	CEdit & ThisEdit = this->GetEditCtrl();
	if(Modifyed)
	{
		static u8	 	DataBuff[DATAVIEWBUFFERLENTH]; 
		Content.Empty();
		ThisEdit.GetWindowText(Content);
		int lenth = Content.GetLength();
		u32 DataLen = lenth/8;
		if(lenth%8 == 0)
		{
			u8 * pData = DataBuff;
			
			memset(pData ,0 , DataLen);
			int BitIdx = 0 ;
			while(lenth > BitIdx)
			{
				if(Content.Left(1) == "1")
				{
					pData[BitIdx/8] |= 0x1 << (7 - (BitIdx % 8));
				}
				else if (Content.Left(1) != "0")
				{
					MessageBox("Data input error,you should only input Binary data");
					GetDocument()->UpdateAllViews(NULL,CTSDoc::DATA_VIEW|CTSDoc::TREE_VIEW);
					return;
				}
				Content.Delete(0);
				BitIdx ++;
			}
			if((memcmp(pData, GetDocument()->DocData , DataLen ) != 0)||
				(DataLen != GetDocument()->DocDataLen))
			{// if updated
				CRC myCrc;
				u32 NewCrc = myCrc.CalCrc32(pData, (u16)(DataLen - 4));

				pData[DataLen-4] = (u8)(NewCrc >> 24 & 0xFF);
				pData[DataLen-3] = (u8)(NewCrc >> 16 & 0xFF);
				pData[DataLen-2] = (u8)(NewCrc >> 8 & 0xFF);
				pData[DataLen-1] = (u8)(NewCrc & 0xFF);
				memcpy(GetDocument()->DocData , pData , DataLen);
				GetDocument()->DocDataLen = DataLen;
				//GetDocument()->TempSaveEdit();
				//TraceOutData(GetDocument()->DocData, GetDocument()->DocDataLen);
			}
		}
		else
		{
			MessageBox("Section Data Length should be 8*x bit");
		}
		GetDocument()->UpdateAllViews(NULL,CTSDoc::ALL_VIEW);
		Modifyed = false;
	}
}

BOOL CSelectDataView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	DataFont.CreateFont(DATA_HEIGHT, DATA_WIDTH, 0,0,700, 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times New Roman");

	/*
	  ScalFont.CreateFont(10   ,   5   ,   0   ,   0   ,   FW_BLACK   ,      FALSE   ,   FALSE   ,   FALSE   ,   
		  GB2312_CHARSET   ,       OUT_DEFAULT_PRECIS   ,   CLIP_DEFAULT_PRECIS   ,     
		  DEFAULT_QUALITY   ,     FIXED_PITCH   |   FF_MODERN,     "宋体");   

*/
	BOOL Ret = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL); 
	return Ret;
}


void CSelectDataView::OnEnKillfocus()
{
	UpdataData();
}


void CSelectDataView::OnEnChange()
{	
	Modifyed = true;
	GetEditCtrl().SetModify(FALSE);   
}

LRESULT CSelectDataView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_UI_DATA_VIEW_EDIT:
		{
			CEdit & ThisEdit = this->GetEditCtrl();
			ThisEdit.GetWindowText(Content);
			
			u8 * NewData = (u8 * )wParam;
			CString NDS;// new data string
			u32 index = 0;
			for(; index < GetDocument()->DocDataFocusLen; index++)
				NDS.AppendChar('0');
			
			index = GetDocument()->DocDataFocusLen;
			while(NewData[0])
			{
				index--;
				
				if(NewData[0] & 0x1)
					NDS.SetAt(index,'1');
				
				NewData[0] >>= 1;
			}

			Content.Delete(GetDocument()->DocDataFocusStart,GetDocument()->DocDataFocusLen);
			Content.Insert(GetDocument()->DocDataFocusStart,NDS);
			
			ThisEdit.SetWindowText((LPCTSTR)Content);
			Modifyed = true;
			
			UpdataData();
		}
		break;
		default:
			break;
	}

	return CEditView::WindowProc(message, wParam, lParam);
}

void CSelectDataView::OnInitialUpdate()
{
	CEdit & ThisEdit = this->GetEditCtrl();
	CEditView::OnInitialUpdate();
	ThisEdit.SetFont(&DataFont);
	//ThisEdit.SetMargins(20,0);
	GetDocument()->pSltDataView = this;
	ThisEdit.SetReadOnly(TRUE);
}

void CSelectDataView::OnPaint() 
{
	/*CRect rect;
	GetClientRect(&rect);
	rect.left += SCALE_WIDTH+SCALE_GAP;
	rect.top += SCALE_WIDTH+SCALE_GAP;
	rect.bottom -= SCALE_WIDTH+SCALE_GAP;
	rect.right -= SCALE_WIDTH+SCALE_GAP;
	GetEditCtrl().SetRect(&rect);
	*/
	CEditView::OnPaint();
	
	//PaintIndx();
}

void CSelectDataView::PaintIndx()
{
/*
	CBrush brushb(RGB(220,220,220));
	
	CDC* hdc;
	hdc = GetWindowDC();
	CRect rect;
	GetClientRect(&rect);
	
	hdc->FillRect (CRect(rect.left ,rect.top ,rect.left  + SCALE_WIDTH ,rect.bottom ),&brushb);//画底色
	hdc->FillRect (CRect(rect.left+SCALE_WIDTH ,rect.top ,rect.right   ,rect.top  + SCALE_WIDTH),&brushb);//画底色
	
	brushb.DeleteObject ();

	CEdit& theEdit = GetEditCtrl ();
	int nFirstVisible = theEdit.GetFirstVisibleLine();	//得到当前显示的最上端的行号

  	CFont*   pOldFont=(CFont*)hdc->SelectObject(&ScalFont);   
  	hdc->SetBkMode(TRANSPARENT);   
  	
	CString Index;
	u16 ind16;
	for(int i = 0; (SCALE_WIDTH+SCALE_GAP+ (i+1)*(DATA_WIDTH+DATA_GAP) ) < rect.Width (); i ++)
	{
		ind16 = i/10 ;
		if(i%10 == 0)
			Index.Format("%d",ind16);
		else
			Index.Format("|");
		
		hdc->TextOut((i*(DATA_WIDTH+DATA_GAP) + SCALE_WIDTH+SCALE_GAP+3),0,Index);
	}
	
	for(int i = nFirstVisible; ((i - nFirstVisible)*DATA_HEIGHT + SCALE_WIDTH) < rect.Height (); i ++)
	{
		ind16 = i/10 ;
		if(i%10 == 0)
			Index.Format("%d",ind16);
		else
			Index.Format("-");
		
		hdc->TextOut(0,((i - nFirstVisible)*DATA_HEIGHT + SCALE_WIDTH+15),Index);
	}
	
	hdc->SelectObject (pOldFont);
	
	//OrigBrush.DeleteObject();
	*/
}

void CSelectDataView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int star, end;
	CEdit & ThisEdit = this->GetEditCtrl();
	ThisEdit.GetSel(star,end);
	AfxGetMainWnd()->PostMessage(MSG_UI_UPDATE_SELECT_INDEX, star ,end);
	
	CEditView::OnLButtonUp(nFlags, point);
}

