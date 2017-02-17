

#include "crc.h"

CRC::CRC(u32 Polynomial)
{
	_Polynomial = Polynomial;
	//InitCrcTable();
}

CRC::~CRC(void)
{
}

void CRC::InitCrcTable()
{
	// init CRC-32 table
	
    // 256 values representing ASCII character codes.
    for (u32 i = 0; i < 0xFF; i++)
    {
        m_Table[i] = ReverseData(i, 8) << 24;
        for (int j = 0; j < 8; j++)
            m_Table[i] = (m_Table[i] << 1) ^ (m_Table[i] & (1 << 31) ? _Polynomial : 0);
        m_Table[i] = ReverseData(m_Table[i], 32);
		//TRACE("m_Table %d build finished",i);
    }
}

u32 CRC::ReverseData(u32 Data, u8 BitLen)
{
    u32 value = 0;

    for(int i = 1; i < (BitLen + 1); i++)
    {
        if (Data & 1)
            value |= 1 << (BitLen - i);
        Data >>= 1;
    }
    return value;
}

u32 CRC::CalCrc32(u8 * data, u16 len)
{
    u32 result;
    int     i,j;
    u8        octet;
    result = 0xFFFFFFFF;
 
    for (i = 0; i < len; i++)
    {
        octet = *(data++);
        for (j = 0; j < 8; j++)
        {
            if ((octet >> 7)^(result >> 31))
                result = (result << 1)^_Polynomial;
            else
                result = result << 1;

            octet <<= 1;
        }
    }

    return result;
}
 

u32 CRC::CalCrc32ShortCut(u8 * data, u16 len)
{
	u32 CRCResult = 0xFFFFFFFF;

	while (len --)
	{
        	CRCResult = (CRCResult >> 8) ^ m_Table[(CRCResult & 0xFF) ^ *data++];
	}
	CRCResult ^= 0xFFFFFFFF;
 
	return CRCResult;
}

bool CRC::CheckCrc32(u8 * data, u16 len)
{
	return (CalCrc32(data, len) == 0);
}

/*return true : little-endian, return false :big-endian*/
bool CRC::checkCPUendian()
{
        union {
             unsigned int a;
             unsigned char b;            
        }c;
        c.a = 1;
        return (c.b == 1);       
   }  

