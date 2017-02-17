#include "StdAfx.h"

#include "SectionContent.h"
#include <assert.h>

CSectionContent::CSectionContent()
{
	ReversePostion();
}

CSectionContent::~CSectionContent()
{
}

void CSectionContent::ReversePostion()
{
	CurPos = 0;
}

bool CSectionContent::ReadNextLine( CString & pNl)
{
	int pos;
	int NextPos;
	pNl.Empty();
	while(CurPos < GetLength())
	{
		if((NextPos = Find("\r\n",CurPos)) >=0)
		{
		}
		else
		{
			NextPos = GetLength();
		}
		pNl = Mid(CurPos,NextPos -CurPos);
		CurPos = NextPos+1;
		
		ErrorInfo = pNl;
		pos = pNl.Find('#');
		if(-1 != pos)
			pNl.Delete(pos,pNl.GetLength() - pos);
		
		pNl.Trim();
		
		if(pNl.GetLength() != 0)
			break;
	}
	
	if(pNl.GetLength() == 0)
	{
		return false;
	}
	else if(!IsValidLine(pNl))
	{
		ErrorInfo.Insert(0,"Error Line found: \r\n");
		return false;
	}
	else
		return true;
}

bool CSectionContent::ReadNextDataLine( CString & pNl)
{
	int FDP,SDP;
	while(ReadNextLine( pNl))
	{
		FDP = pNl.Find(":");
		SDP = pNl.Find(":",FDP+1);
		if( FDP< SDP ) //data format 
			break;
	}
	if(pNl.GetLength() != 0)	
		return true;
	else
		return false;
}

bool CSectionContent::IsValidLine(CString & pNl)
{
	bool ret = false;
	CString Line = pNl;
	CString OldLenthStr,OldValueStr;
	int FDP,SDP,TDP;
	if(Line.CompareNoCase("{") == 0)
	{
		ret=true;
	}
	else if(Line.CompareNoCase("}") == 0)
	{
		ret=true;
	}
	else
	{
		FDP = Line.Find(":");
		SDP = Line.Find(":",FDP+1);
		TDP = Line.Find(":",SDP+1);
		if((FDP >= 0) && (SDP > 0) && (TDP < 0) ) // format correct with two : in line
		{
			if(GetFieldName(Line).GetLength()) //name is valid
			{
				CString NewLenth;
				OldLenthStr = Line.Mid( FDP+1, SDP-(FDP+1));// old length
				OldLenthStr.Trim();
				NewLenth.Format("%d",GetLenth(Line));
				if((GetLenth(Line)>0) && (0 == OldLenthStr.CompareNoCase(NewLenth)) )// lenth is valid
				{
					CString  NewValue;
					u64 Value = GetValue(Line);
					OldValueStr = Line.Right(Line.GetLength() - (SDP+1));//old value
					OldValueStr.Trim();
					
					if((GetValueFieldLenth(Line) <= (VALUE_MAX_LENGTH_IN_BYTE * 2))&& (GetLenth(Line) <= (VALUE_MAX_LENGTH_IN_BYTE * 8))) //normal field
					{
						u64 val_bk = Value;
						u8 byte;
						CString ByteStr;
						NewValue.Empty();
						while(val_bk)
						{
							byte = (u8)(val_bk & 0xFF);
							ByteStr.Format("%02x",byte);
							NewValue.Insert(0 , ByteStr);
							val_bk >>= 8;
						}
						while(NewValue.Left(1) == "0")
						{
							NewValue.Delete(0,1);
						}
						
						if(NewValue.IsEmpty())
						{
							NewValue.Format("0");
						}
						while((OldValueStr.GetLength()>1) && (OldValueStr.Left(1) == "0"))
						{/*befor compare delete 0 in front if there are in old string*/
							OldValueStr.Delete(0,1);
						}
						
						/*max value */
						val_bk = (0xFFFFFFFFFFFFFFFF >> ((VALUE_MAX_LENGTH_IN_BYTE * 8) - GetLenth(Line)) );
						if (( Value <= val_bk)&&(0 == OldValueStr.CompareNoCase(NewValue)))
						{// value is valid in this condition
							ret=true;
						}
						else
						{
							ErrorInfo.Insert(0,"Value invalid. \r\n");
						}
					}
					else
					{// this need fix later
						u32 ByteLenth = GetLenth(Line) / 4;
						if(GetLenth(Line) % 4)
							ByteLenth++;
						
						if(ByteLenth >= GetValueFieldLenth(Line))
						{
							ret=true;
						}
						free((void * )Value);
					}
				}
				else
				{
					ErrorInfo.Insert(0,"length invalid. \r\n");
				}
			}
			else
			{
				ErrorInfo.Insert(0,"name invalid. \r\n");
			}
		}
	}
	return ret;
}

u32 CSectionContent::GetValueFieldLenth(CString & pNl)
{
	CString Line = pNl;
	Line.Delete(0,Line.ReverseFind(':') + 1);
	Line.Trim();
	return(Line.GetLength());
}

CString CSectionContent::GetFieldName(CString & pNl)
{
	CString name = pNl.Left(pNl.Find(":"));
	name.Trim();
	return name;
}


u32 CSectionContent::GetLenth(CString & pNl)
{
	u32 len;
	CString Line = pNl;
	Line.Delete(0,Line.Find(":")+1);
	Line = Line.Left(Line.Find(":"));
	len = atoi(Line);
	return len;
}

/*
note: if byte length > 8, will malloc a mem in side for save this value.
		should free outside
*/


u64 CSectionContent::GetValue(CString & pNl)
{
	u64 Val = 0;
	CString Line = pNl;
	Line.Delete(0,Line.ReverseFind(':') + 1);
	Line.Trim();
	if(Line.GetLength()<= (VALUE_MAX_LENGTH_IN_BYTE *2)) /*ONE BYTE HAS 2 CHAR */
	{
		while(Line.GetLength())
		{
			if(Line.Left(1).CompareNoCase("A") == 0)
				Val = (Val<<4) + 0xA;
			else if(Line.Left(1).CompareNoCase("B") == 0)
				Val = (Val<<4) + 0xB;
			else if(Line.Left(1).CompareNoCase("C") == 0)
				Val = (Val<<4) + 0xC;
			else if(Line.Left(1).CompareNoCase("D") == 0)
				Val = (Val<<4) + 0xD;
			else if(Line.Left(1).CompareNoCase("E") == 0)
				Val = (Val<<4) + 0xE;
			else if(Line.Left(1).CompareNoCase("F") == 0)
				Val = (Val<<4) + 0xF;
			else
				Val = (Val<<4) + atoi(Line.Left(1));
			
			Line.Delete(0,1);
		}
	}
	else
	{
		u8 * pU8 = (u8 *)malloc(Line.GetLength()/2 + 1);
		u8 CC = 0;
		Val = (u64)pU8;
		
		memset(pU8 , 0 , Line.GetLength()/2);
		
		while(Line.GetLength())
		{
			CC ++;
			if(Line.Left(1).CompareNoCase("A") == 0)
				*pU8 = (*pU8<<4) + 0xA;
			else if(Line.Left(1).CompareNoCase("B") == 0)
				*pU8 = (*pU8<<4) + 0xB;
			else if(Line.Left(1).CompareNoCase("C") == 0)
				*pU8 = (*pU8<<4) + 0xC;
			else if(Line.Left(1).CompareNoCase("D") == 0)
				*pU8 = (*pU8<<4) + 0xD;
			else if(Line.Left(1).CompareNoCase("E") == 0)
				*pU8 = (*pU8<<4) + 0xE;
			else if(Line.Left(1).CompareNoCase("F") == 0)
				*pU8 = (*pU8<<4) + 0xF;
			else
				*pU8 = (*pU8<<4) + atoi(Line.Left(1));

			if(0 == (CC%2))
				pU8++;
			
			Line.Delete(0,1);
		}

	}
	return Val;
}

bool CSectionContent::PreCheckSubContent( u32 & ParentLenth)
{
	// check if it's format ok for a integrated section
	CString Line;
	u32 SubLenth = 0 ;
	s32 ShouldLenth = -1;
	int SubStart = CurPos;
	while(ReadNextLine( Line))
	{
		if(Line.CompareNoCase("{") == 0)
		{
			if(PreCheckSubContent(SubLenth) == false)
				return false;

		}
		else if(Line.CompareNoCase("}") == 0)
		{
			// check *8 and and same as should lenth.
			if(((SubLenth % 8) == 0)&&((ShouldLenth == -1)||(ShouldLenth == SubLenth)))
			{
				ParentLenth += SubLenth;
				return true;
			}
			else
			{
				ErrorInfo.Format("Following sub content lenth error: \r\n");
				ErrorInfo.Append(Mid(SubStart-2, CurPos-SubStart+1));
				return false;
			}
		}
		else
		{// now we only collect bit length
			SubLenth += GetLenth(Line);
			/*this is not a best check, but some times may be surprise */
			if((GetFieldName(Line).CompareNoCase("length") == 0)||(GetFieldName(Line).CompareNoCase("section_length") == 0))
			{/*with length without loop,so normaly descriptor length or section length*/
				if((ShouldLenth == -1) && (GetValueFieldLenth(Line) <= (VALUE_MAX_LENGTH_IN_BYTE * 2)))
				{
					ShouldLenth = (s32)(SubLenth + (8 * GetValue(Line)));
				}
				else
				{
					//SubConBk.Insert(0,"Error, find lenth twice in sub: \r\n");
					ErrorInfo.Format("Find length twice in sub... :\r\n");
					ErrorInfo.Append(Mid(SubStart-2, CurPos-SubStart+1));
					return false;
				}
			}
		}
	}
	
	if(Line.GetLength() == 0)
	{
		ErrorInfo.Format("{ and } unpaired");
	}
	return false;
}

bool CSectionContent::PreCheckSectionContent()
{
	// check if it's format ok for a integrated section
	CString Line;
	u32  TotalLenth = 0 ;
	//LastLine.Empty();

	ReversePostion();
	ReadNextLine( Line);
	if(Line.GetLength() == 0)
	{
		ErrorInfo.Format("No valid content line find.");
		return false;
	}
		
	if(Line.CompareNoCase("{") == 0)
	{
		if(PreCheckSubContent(TotalLenth) == false)
		{
			return false;
		}
	}
	else
	{
		ErrorInfo.Format("valid line not start from {");
		return false;
	}

	ReadNextLine(Line);
	if(Line.GetLength() != 0)
	{
		ErrorInfo.Format("Invalid content lines from %s to end",Line);
		return false;
	}

	return true;
}

u32 CSectionContent::GetContentLengthInBit(int PosStart, int PosEnd)
{
	CString Line;
	u32 TotalLenth  = 0 ;

	CurPos = PosStart;

	while(ReadNextDataLine( Line))
	{				
		if(CurPos > (PosEnd+1))
			break;
		
		TotalLenth += GetLenth(Line);
	}
	return (TotalLenth) ;
}

void CSectionContent::ReadSectionDataTo(u8 *  NewSecData,     u32 &   NewSecLenth)
{
		CString Line;
		u32 CurLenth  = 0 ;
		u32 Lenth ;
		u64 Value;
		u8 	ValByt;
		u8 * pData ;

		if(NULL == NewSecData)
			return;
		
		NewSecLenth = (GetContentLengthInBit(0,GetLength()) / 8);

		
		memset(NewSecData, 0, NewSecLenth);

		ReversePostion();
		while(ReadNextDataLine(Line))
		{
			Lenth = GetLenth(Line);
			Value = GetValue(Line);
			pData = NewSecData +	(CurLenth/8);
			
			if(Lenth <= (VALUE_MAX_LENGTH_IN_BYTE * 8))//value
			{
				if((8 - (CurLenth%8) ) >= Lenth) //bits in this byte is enough for this field
				{
					ValByt = (u8)(Value & 0xFF);
					*pData = *pData | (ValByt << (8 - (CurLenth%8) - Lenth));
				}
				else
				{
					Lenth -= (8 - (CurLenth%8) );
					ValByt = (u8)((Value >> Lenth) & 0xFF);
					
					*pData = *pData | ValByt;
					pData ++;
					while(Lenth)
					{
						if(Lenth >= 8)
						{
							Lenth -= 8;
							ValByt = (u8)((Value >> Lenth) & 0xFF);
							*pData =  ValByt;
						}
						else
						{
							ValByt = (u8)((Value<<(8 - Lenth) )& 0xFF);
							*pData =  ValByt;
							Lenth = 0;
						}
						pData ++;
					}
				}
			}
			else //address
			{
				u8 * pU8 = (u8 *)Value;
				if(Lenth % 8)
				{// treat self first
					pU8[Lenth / 8] <<= (8-(Lenth % 8));
				}

				*pData = *pData | (pU8[0] >> (CurLenth%8));
				pData ++;
				Lenth -= (8 -(CurLenth%8));
				while( Lenth >= 8 )
				{
					*pData = (pU8[1] >> (CurLenth%8))| (pU8[0] << (8 - CurLenth%8));
					pData ++;
					pU8 ++;
					Lenth -= 8;
				}
				if(Lenth)
				{
					*pData |=  (pU8[0] << (8 - CurLenth%8));
					Lenth = 0;
				}
				free((u8 *)Value);
			}
			CurLenth += GetLenth(Line);
		}

}
