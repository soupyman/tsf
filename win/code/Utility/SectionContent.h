/*


*/

#pragma once

#include "TypeDefine.h"

class CSectionContent: public CString
{
public:
    CSectionContent();
    ~CSectionContent();
    
      u32 GetContentLengthInBit(int PosStart, int PosEnd);
      void ReadSectionDataTo(u8 *  NewSecData,     u32 &   NewSecLenth);
      bool PreCheckSectionContent();

private:
       void ReversePostion();
      bool PreCheckSubContent(u32 & ParentLenth);
      bool ReadNextLine( CString & pNl);
      bool ReadNextDataLine( CString & pNl);

      bool IsValidLine(CString & pNl);
      CString GetFieldName(CString & pNl);
      u32 GetValueFieldLenth(CString & pNl);

      u32 GetLenth(CString & pNl);
      u64 GetValue(CString & pNl);

public:
        CString       ErrorInfo;

private:
        int             CurPos;
        
};
