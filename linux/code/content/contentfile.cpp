#define _NOINFOM

#include "contentfile.h"
#include <string.h>
#include <stdlib.h>

extern "C"
{
	extern	int stricmp(const char *dst, const char *src);
}

CContentFile::CContentFile(const s8 *pFileName)
	: CXmlFile(pFileName)
{
	pCTree = NULL;
	pCurTree = NULL;
	bFileNode = false;
}

CContentFile::~CContentFile(void)
{
	ClearCntTree();
}

void CContentFile::ClearCntTree()
{
	if(pCTree)
	{
		delete pCTree;
		pCTree = NULL;
		pCurTree = NULL;
	}
}

void CContentFile::InitCntTree()
{
	if(pCTree)
	{
		ClearCntTree();
	}
	pCTree = AddRootBranch(m_FileName);
	pCurTree = pCTree;
}

void CContentFile::AddCntTreeBranch(const char *el, const char **attr)
{
	CString name ;
	u32 lenth;
	u32  format;
	u16 pid;
	for(u8 idx = 0; attr[idx] ; idx += 2)
	{
		if(0 == stricmp(attr[idx], STRING_PID))
		{
			pid = atoi(attr[idx + 1]);
		}
		else if(0 == stricmp(attr[idx], STRING_NAME))
		{
			name = attr[idx + 1];
		}
		else if(0 == stricmp(attr[idx], STRING_LENTH))
		{
			lenth = atoi(attr[idx + 1]);
		}
		else if(0 == stricmp(attr[idx], STRING_FORMAT))
		{
			if(0 == stricmp(attr[idx + 1], STRING_BIN))
				format = FIELD_FMT_BIN;
			else if(0 == stricmp(attr[idx + 1], STRING_DEC))
				format = FIELD_FMT_DEC;
			else if(0 == stricmp(attr[idx + 1], STRING_HEX))
				format = FIELD_FMT_HEX;
			else if(0 == stricmp(attr[idx + 1], STRING_BCD))
				format = FIELD_FMT_BCD;
			else if(0 == stricmp(attr[idx + 1], STRING_MJD))
				format = FIELD_FMT_MJD;
			else if(0 == stricmp(attr[idx + 1], STRING_STR))
			{
				format = FIELD_FMT_STR;
			}
			else
			{
				format = FIELD_FMT_HEX;
				DBG("unkow format %s \n", attr[idx + 1]);
			}
		}
		else
		{
			DBG("not used param %s, \n", attr[idx]);
		}
	}
	//DBG("(%s) on deep %d, \n", el, Depth);
	bFileNode = false;

	if(0 == stricmp(el, STRING_TABLE))
	{
		if(1 == Depth)
			pCurTree = AddTableBranch(pCurTree, name  , pid);
		else
			ERR("deep error for <TABLE name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_SECTION))
	{
		if(2 == Depth)
			pCurTree = AddSectionBranch(pCurTree, name);
		else
			ERR("deep error for <SECTION name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_PACKET))
	{
		if(1 == Depth)
			pCurTree = AddPacketBranch(pCurTree, name  , 0/*should be PID*/);
		else
			ERR("deep error for <PACKET name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_FIELD))
	{
		if(1 < Depth)
			pCurTree = AddFieldBranch(pCurTree, name  , 0 , lenth , format, 0);
		else
			ERR("deep error for <FIELD name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_FILE))
	{
		bFileNode = true;

		if(1 < Depth)
			pCurTree = AddFieldBranch(pCurTree, name  , 0 , lenth , FIELD_FMT_HEX, 0);
		else
			ERR("deep error for <FILE name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_SUBBODY))
	{
		if(1 < Depth)
			pCurTree = AddSubBodyBranch(pCurTree, name  , 0 , lenth);
		else
			ERR("deep error for <SUBBODY name='%s' > node", name.c_str());
	}
	else if(0 == stricmp(el, STRING_RESERVE))
	{
		if(1 < Depth)
			pCurTree = AddReserveBranch(pCurTree  , 0 , lenth);
		else
			ERR("deep error for <RESERVE> node");
	}
	else
	{
		ERR("unrecognized node <%s>, \n", el);
	}

}

void CContentFile::UserStartValue(const char *el, const char **attr)
{
	AddCntTreeBranch(el,  attr);
}

void CContentFile::UserEndValue(const char *el)
{
	if(0 < CurValueLenth)
	{
		CurValuebuf[CurValueLenth] = 0;
		INF("data is:[%s] len = %d .\n", CurValuebuf, CurValueLenth);
		SetCurrentValue(CurValuebuf, CurValueLenth);
	}

	pCurTree = (CContentTree *)(pCurTree->GetFather());

	if(NULL == pCurTree)
		ASSERT(0);
}

/*

*/
void CContentFile::SetCurrentValue(s8 *s, u32 len)
{
#if 1
	if(NULL == pCurTree)
	{
		ASSERT(0);
	}
	CContentTreeNode *pCtn = (CContentTreeNode *) pCurTree->GetNodeType();

	if(CContentTreeNode::FIELD_NODE == pCurTree->GetNodeType())
	{
		CContentFieldNode *pfdn = (CContentFieldNode *)(pCurTree->GetMyNode());
		if(bFileNode) /*xml file node, bute content tree field node. */
		{
			u32 ReadLenth;
			CFile *pFile;
			pFile = new CFile(s);
			if(true ==  pFile->Open(CFile::OPEN_TEXT_READ_ONLY))
			{
				if(pfdn->Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
				{
					ReadLenth = pFile->Read((u8 *)(pfdn->Value), pfdn->Lenth / 8);
				}
				else
				{
					u8 Data[VALUE_MAX_LENGTH_IN_BYTE];
					u8 ldx;
					ReadLenth = pFile->Read((u8 *)Data, pfdn->Lenth / 8);
					pfdn->Value = 0;
					for(ldx = 0; ldx < pfdn->Lenth / 8; ldx ++)
					{
						/* need check , Data[0] is high byte or low byte? ??? */
						pfdn->Value = (pfdn->Value << 8) | Data[ldx];
					}
				}

				pFile->Close();
			}
			else
				ERR("open failed");
			delete pFile;

		}
		else
		{
			switch(pfdn->Format)
			{
				case FIELD_FMT_BIN:
					if(pfdn->Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
					{
					}
					break;
				case FIELD_FMT_DEC:
					pfdn->Value = atoi(s);
					break;
				case FIELD_FMT_HEX:

					while(len)
					{
						u8 cur;
						if((s[0] >= 'a') && (s[0] <= 'f'))
							s[0] -= 0x20;

						if((s[0] >= 'A') && (s[0] <= 'F'))
							cur = 0xa + s[0] - 'A';
						else if((s[0] >= '0') && (s[0] <= '9'))
							cur = s[0] - '0';
						else
							cur = 0;

						if(pfdn->Lenth > (VALUE_MAX_LENGTH_IN_BYTE * 8))
						{
							u32 CharCnt = pfdn->Lenth / 4 + (pfdn->Lenth % 4 ? 1 : 0);
							s32 CharIdx = CharCnt - len  ;
							u8 *pdata = (u8 *)((u32)pfdn->Value);
							pdata[(CharIdx / 2)] = pdata[CharIdx / 2] | (cur << ((1 - (CharIdx % 2)) * 4));
							//printf("value[%d] is:[%x]  .\n" ,(len - 1)/2 ,  pdata[(len - 1)/2] );
						}
						else
						{
							pfdn->Value = (pfdn->Value << 4) | cur;
						}
						len -- ;
						s++;
					}
					//printf("value is:[%x] .\n" , pfdn->Value);
					break;
				case FIELD_FMT_BCD:
					for(int i = pfdn->Lenth - 4 ; i >= 0; i -= 4)
					{
						((((*s) - '0') >> (i)) & 0x0F) ; // may compatible problem
					}

					break;
				case FIELD_FMT_MJD:
					break;
				case FIELD_FMT_STR:
				{
					u8 *pData = (u8 *)((u32)(pfdn->Value));
					u32  cpylen = len;
					u32 MaxLen = pfdn->Lenth / 8 + (pfdn->Lenth % 8 ? 1 : 0);
					if(cpylen > MaxLen)
						cpylen = MaxLen;

					memcpy((void *)pData, (void *)s , cpylen);

					INF("memcpy [%s] lenth %d .\n", pData, cpylen);
				}
					break;
				default:
					break;
			}
		}
	}

#endif
}

void CContentFile::UserInitBeforeLoad()
{
	InitCntTree();
}

