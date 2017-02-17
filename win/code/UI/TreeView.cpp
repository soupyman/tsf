// UnitTreeView.cpp : 实现文件

#include "stdafx.h"
#include "include.h"
#include "TreeView.h"
#include "DialogInput.h"
#include "DvbTsa.h"
#include "TSDoc.h"
#include "MainFrm.h"

// CUnitTreeView

IMPLEMENT_DYNCREATE(CUnitTreeView, CTreeView)

CUnitTreeView::CUnitTreeView()
{
	pCntTree = NULL;
	m_SlctTItem = NULL;
}

CUnitTreeView::~CUnitTreeView()
{
	if(pCntTree)
		delete pCntTree;
}

BEGIN_MESSAGE_MAP(CUnitTreeView, CTreeView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CUnitTreeView 诊断

#ifdef _DEBUG
void CUnitTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CUnitTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

void CUnitTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CTreeCtrl * theTree = &(this->GetTreeCtrl());
	//theTree->SetBkColor(RGB(230,230,230));
	
	m_imgTree.Create(IDB_TREE_ICON, 16, 1, RGB(192, 192, 192));
	theTree->SetImageList(&m_imgTree, TVSIL_NORMAL);
	
	GetDocument()->pTreeView = this;
}

BOOL CUnitTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES   |    TVS_HASBUTTONS | TVS_LINESATROOT |TVS_SHOWSELALWAYS ;
	return CTreeView::PreCreateWindow(cs);
}

void CUnitTreeView::UpdateSelection()
{
	CTreeCtrl& theTree = this->GetTreeCtrl();
	
	HTREEITEM TItem = theTree.GetSelectedItem();
	if(TItem != NULL)
	{
		if(m_SlctTItem != TItem)
		{
			DWORD_PTR ItemData = theTree.GetItemData(TItem);
			if(ItemData)
			{
				m_SlctTItem = TItem;
				CContentTreeNode * pNode = (CContentTreeNode *)ItemData;
				switch(pNode->Type)
				{
					case CContentTreeNode::SECTION_NODE:
					{
						//GetDocument()->DocDataFocusStart = 0;
						//GetDocument()->DocDataFocusLen = ((CContentSectionNode * )pNode)->DataLenth * 8;
					}				
					break;
					case CContentTreeNode::SUBBODY_NODE:
					{
						GetDocument()->DocDataFocusStart = ((CContentSubBodyNode   * )pNode)->Index;
						GetDocument()->DocDataFocusLen =  ((CContentSubBodyNode   * )pNode)->Lenth;
					}
					break;
					case CContentTreeNode::FIELD_NODE:
					{
						GetDocument()->DocDataFocusStart = ((CContentFieldNode   * )pNode)->Index;
						GetDocument()->DocDataFocusLen =  ((CContentFieldNode   * )pNode)->Lenth;
					}
					break;
					//case CContentTreeNode::KEY_NODE:
						//break;
					case CContentTreeNode::PACKET_NODE:
					{
						assert(0);
						/*assert(((pNode->Virtual.packet.PacketLenth) < DATAVIEWBUFFERLENTH)&&((pNode->Virtual.packet.PacketLenth) > 0 ));
						CESData * pES = ((CContentPacketNode   * )pNode)->pES;
						CPacket * pPkt = pES->GetPacket(((CContentPacketNode   * )pNode)->PacketIdx);
						memcpy(GetDocument()->DocData,(u8 *)(pPkt->GetPktData()) , pPkt->GetPktSize());
						GetDocument()->DocDataLen = pPkt->GetPktSize();

						SetSltPkt((CESData *)(((CContentPacketNode   * )pNode)->pES));
						GetDocument()->DocDataFocusStart = 0;
						GetDocument()->DocDataFocusLen = GetDocument()->DocDataLen * 8 ;*/
					}
					break;
					case CContentTreeNode::ROOT_NODE:
					case CContentTreeNode::TABLE_NODE:
					default:
						assert(0);
						break;
				}
				AfxGetMainWnd()->PostMessage(MSG_UI_UPDATE_SELECT_INDEX,
				GetDocument()->DocDataFocusStart,GetDocument()->DocDataFocusStart+GetDocument()->DocDataFocusLen );
				
				GetDocument()->UpdateAllViews(NULL,CTSDoc::SELECT_DATA_VIEW|CTSDoc::DATA_VIEW);
			}
		}
	}
}
/*
void CUnitTreeView::TraceCurEdit()
{
	return;
	if(GetDocument()->pCurTable)
	{
		GetDocument()->DocInfo.AppendFormat("Currently edit tableID(%d) PID(%d) sectin(%d) version(%d)\r\n",\
			GetDocument()->pCurTable->GetTableID(),GetDocument()->pCurTable->GetPID()//,\
			//GetDocument()->SecNum,GetDocument()->pCurTable->GetVersion()
			);
	}
	else if(GetDocument()->pCurES)
	{
		GetDocument()->DocInfo.AppendFormat("Currently edit Packet PID(%d) \r\n",GetDocument()->pCurES->PID);
	}
	else
	{
		GetDocument()->DocInfo.AppendFormat("Currently edit invalid \r\n");
	}
}
*/
void CUnitTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
/*
	CTreeCtrl& theTree = this->GetTreeCtrl();
	HTREEITEM TItem = theTree.GetSelectedItem();
	CString ParamName;
	CDialogParamInput pDPIDI;
	ParamName = theTree.GetItemText(TItem);
	ParamName.Delete(ParamName.Find(':')+1, ParamName.GetLength() - ParamName.Find(':'));
	pDPIDI.Title.Format("%s (%d Bit ):",ParamName,GetDocument()->DocDataFocusLen);
	pDPIDI.LenInBin = GetDocument()->DocDataFocusLen;
	if(pDPIDI.DoModal() == IDOK)
	{
		GetDocument()->pEditView->PostMessage(MSG_UI_DATA_VIEW_EDIT,pDPIDI._data_i);
	}	
	*/
	*pResult = 0;
}

void CUnitTreeView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	PostMessage(MSG_UI_SELECT_CHANGE);
	*pResult = 0;
}
/*
void CUnitTreeView::SetSltPkt(CESData	* pES)
{
	GetDocument()->pCurES = pES;
	GetDocument()->SecNum = 0;
	GetDocument()->pCurTable = NULL;
}

void CUnitTreeView::SetSltTbl(CTable * pTable,	u8 SecNum)
{
	GetDocument()->pCurES = NULL;
	GetDocument()->SecNum = SecNum;
	GetDocument()->pCurTable = pTable;
}
*/
CTSDoc * CUnitTreeView::GetDocument(void)
{
	return (CTSDoc *)CView::GetDocument();
}


/*
search the tree item after current select with string part.
*/
BOOL CUnitTreeView::SearchTreeField(HTREEITEM From ,CString * Part , bool & FindCur)
{
	HTREEITEM Next;
	CString ItemTxt;
	if(NULL == From)
	{
		From = this->GetTreeCtrl().GetRootItem();
	}
	
	if(FindCur && ((ItemTxt =  this->GetTreeCtrl().GetItemText(From))&&(-1 != ItemTxt.Find(*Part))))
	{
		this->GetTreeCtrl().SelectItem(From);
		return true;
	}
	if(From == this->GetTreeCtrl().GetSelectedItem())
	{
		FindCur =  true;
	}
	
	if(Next = this->GetTreeCtrl().GetChildItem(From))
	{
		if(SearchTreeField(Next, Part,FindCur))
			return true;
	}
	if(Next = this->GetTreeCtrl().GetNextItem(From,TVGN_NEXT))
	{
		if(SearchTreeField(Next, Part,FindCur))
			return true;
	}
	return false;
}

void CUnitTreeView::AttachTree( CCommonTree *	pTT , HTREEITEM parent)
{
	HTREEITEM Par;
	assert(pTT);
	//assert(parent);
	CContentTreeNode * pNode = (CContentTreeNode *)(pTT->GetMyNode());
	CString item;
	if(pNode->Type != CContentTreeNode::RESERVE_NODE)
	{
		item = pNode->GetTitleWithFmtVal();
		
		if(parent)
			Par =  this->GetTreeCtrl().InsertItem((LPCTSTR)item,parent);
		else
			Par =  this->GetTreeCtrl().InsertItem((LPCTSTR)item);
		
		this->GetTreeCtrl().SetItemData(Par , (DWORD_PTR)pNode);
		this->GetTreeCtrl().SetItemImage(Par,pNode->Type,pNode->Type);
		if(pTT->GetChildren())
			AttachTree(pTT->GetChildren() , Par);
	}
	if(pTT->GetRightBrother())
		AttachTree(pTT->GetRightBrother() , parent);
	
	this->GetTreeCtrl().Expand(parent ,TVE_EXPAND);
}

// CUnitTreeView 消息处理程序
void CUnitTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

	if(lHint & CTSDoc::TREE_VIEW)
	{
		this->GetTreeCtrl().DeleteAllItems();
		if(pCntTree)
			delete pCntTree;
		if(GetDocument()->pSection)
			pCntTree = GetDocument()->FmtSvr.ParseContentBySection(GetDocument()->pSection);
		else
			pCntTree = NULL;
		if(pCntTree != NULL)
		{
			AttachTree(pCntTree, NULL);
			m_SlctTItem = NULL;
		}
		this->GetTreeCtrl().SelectItem(this->GetTreeCtrl().GetFirstVisibleItem());
		UpdateSelection();	
	}
	
}
/*
void CUnitTreeView::DeleteSelection()
{
	if(pCntTree)
	{
		CTreeCtrl& theTree = this->GetTreeCtrl();
		HTREEITEM TItem ;
		CContentTreeNode * SelNode ;
		CContentTree * pBranch ;
		
		if((theTree)&&
			(TItem = theTree.GetSelectedItem())&&
			(SelNode = (CContentTreeNode *)(theTree.GetItemData(TItem)))&&
			(pBranch = pCntTree->SearchBranchByNode(SelNode)))
		{
			if(pBranch->GetNodeType() == CContentTreeNode::SUBBODY_NODE)
			{
				CContentSubBodyNode * pNode =(CContentSubBodyNode *)( pBranch->GetMyNode());
				u32 DataLenth = GetDocument()->DocDataLen;
				DataLenth -= (pNode->Lenth / 8);
				GetDocument()->DocDataLen = DataLenth;
				for(int i = (pNode->Index /8); i <  GetDocument()->DocDataLen ; i++)
			 		GetDocument()->DocData[i] = GetDocument()->DocData[i+(pNode->Lenth / 8)] ;

				pBranch->AdjustRelativeIdxForIsolate();
				delete pBranch;
				theTree.DeleteAllItems();
				AttachTree(pCntTree, NULL);
				GetDocument()->UpdateAllViews(NULL,CTSDoc::SELECT_DATA_VIEW);
			}
			else if(pBranch->GetNodeType() == CContentTreeNode::FIELD_NODE)
			{
			}
			else if(pBranch->GetNodeType() == CContentTreeNode::SECTION_NODE)
			{
				GetDocument()->pTable->DetachSection(GetDocument()->pSection);
				GetDocument()->pSection = NULL;
				GetDocument()->DocDataLen = 0;
				GetDocument()->UpdateAllViews(NULL,
					CTSDoc::DATA_VIEW|CTSDoc::VERSION_LIST_VIEW|CTSDoc::TREE_VIEW);
			}

		}
	}
}
*/
LRESULT CUnitTreeView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_UI_SELECT_CHANGE:
		{
			UpdateSelection();	
		}
		break;
		case MSG_UI_TREE_VIEW_SELECT_DELETE:
		{
			//DeleteSelection();
		}
		break;
		case MSG_UI_TREE_VIEW_SEARCH:
		{
			CString * pStr = (CString *)wParam;
			bool 	FindCur = false;
			if(SearchTreeField(NULL,pStr,FindCur))
			{
				SetFocus();
			}
			delete pStr;
		}
		break;
		case WM_KEYDOWN ://wParam  : Specifies the virtual-key code of the nonsystem key. 
		{
			if(wParam == 46)//46 is delete key
			{
				PostMessage(MSG_UI_TREE_VIEW_SELECT_DELETE);
			}
		}
		break;
		default:
			break;
	}

	return CTreeView::WindowProc(message, wParam, lParam);
}


