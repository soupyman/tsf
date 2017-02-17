#pragma once

#include "TypeDefine.h"
/*
CRC原理介绍:
 CRC的英文全称为Cyclic Redundancy Check（Code），中文名称为循环冗余校验（码）。
 它是一类重要的线性分组码，编码和解码方法简单，检错和纠错能力强，
 广泛地用于实现差错控制。  

     CRC计算与普通的除法计算有所不同。普通的除法计算是借位相减的，
     而CRC计算则是异或运算。任何一个除法运算都需要选取一个除数，
     在CRC运算中我们称之为poly，而宽度W就是poly最高位的位置。
     比如poly 1001的W是3，而不是4。注意最高位总是1，当你选定一个宽度，
     那么你只需要选择低W各位的值。假如我们想计算一个位串的CRC码，
     并要保证每一位都要被处理，因此我们需要在目标位串后面加上W个0。
     下面举例说明CRC算法的过程。

     在此例中，我们假设位串为110101101。

Poly                     = 10011（宽度W = 4）
Bitstring + W zeros = 110101101 0000 

	    110000101 （我们不关心此运算的商）
          -------------------
10011/1101011010000
           10011|||||||| 
           -----||||||||
             10011|||||||
             10011|||||||
             -----|||||||
               00001||||||
               00000||||||
               -----||||||
                 00010|||||
                 00000|||||
                 -----|||||
                   00101||||
                   00000||||
                   -----||||
                     01010|||
                     00000||| 
                     -----|||
                       10100||
                       10011||
                       -----||
                         01110|
                         00000|
                         -----|
                          11100
                          10011
                          -----
                            1111 -> 余数 -> CRC！

计算过程总结如下：
1. 只有当位串的最高位为1，我们才将它与poly做XOR运算，否则我们只是将位串左移一位。
2. 异或运算的结果实质上是被操作位串与poly的低W位进行运算的结果，因为最高位总为0。　

*/

class CRC
{
	enum CRC_TYPE
	{
	CRC_4,
	CRC_12,
	CRC_16,
	CRC_32,
	CRC_UD
	};
public:
	CRC(u32 Polynomial = 0x04C11DB7 /*the official polynomial used by CRC-32*/);
	~CRC(void);
	void InitCrcTable();
	u32 ReverseData(u32 Data, u8 BitLen);
	u32 CalCrc32(u8 * data , u16 len);
	u32 CalCrc32ShortCut(u8 * data , u16 len);
	bool CheckCrc32(u8 * data, u16 len);
	bool CRC::checkCPUendian();

	
private:
	u32  _Polynomial;
	u32 m_Table[256];
	
};



