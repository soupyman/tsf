#ifndef CSTRING_H
#define CSTRING_H
/*
name the file to cstring for avoid overlap with system file string.h
*/

/*
father class string's function:

  append   函数能将某字符串（或数据串）加到某字符串后面
  assign   函数能将字符串（或数据串）的内容设定给某字符串
  at   函数能回传字符串中位于某个位置的字符
  begin   函数将回传该字符串的起始定位器
  c_str   函数能将字符串对象中的内容，以C语言字符串的方式输出
  capacity   函数能回传目前所能控制的对象容器大小
  compare   函数将会回传   可控制的字符吕容量
  copy   函数能将数据串的内容复制到加一个数据串中
  data   函数能将数据串的第一个起始指针传出
  empty   函数能判断数据串是否为空串
  end   函数将回传该字符串的终止定位器
  erase   函数将容器中的数据清除
  find   函数能在容器中寻找符合求的数据其出现的位置
  find_first_of   函数能在容器中寻找第一个符合特定要求的数据出现的位置
  find_first_not_of   函数能在容器中寻找第一个不符合特定要求的数据出现的位置
  find_last_of   函数能在容器中寻找最后一个符合特定要求的数据出现的位置
  find_last_not_of   函数能在容器中寻找最后一个不符合特定要求的数据出现的位置
  insert   函数能将数据插入到容器中的某个特定位置
  length   函数能计算数据串的长度
  replace   函数能将数据插入到容器中的某个特定位置
  reserve   函数能保证capacity函数的回传值，一定大于某数
  resize   函数能确定size函数的回传值，一定大于某数
  rfind   函数能在容器中寻找最后一个符合特定要求的数据出现的位置
  size   函数能回传目前对象容器的大小
  substr   函数能将串裁减为更少的串
  swap   函数能将两个串的内容交换
  max_size   函数能将两个串的内容交换

  getline   函数能通过输入装置，将输入的数据存储到容器中
  swap函数能将两个数据串中的内容做交换
  */

#include "typedefine.h"

#include <string>
using namespace std;


class CString: public string
{

	public:
		CString(void);
		~CString(void);

		bool Append(const CString &str);
		bool AppendFormat(const s8 *pszFormat, ...);
		bool Format(const s8 *pszFormat, ...);

		bool Find(CString &str);
		bool Empty();
		bool IsEmpty();
		u32 GetLength();
		bool Delete(u32 start, u32 lenth = 1);
		bool IsNumberStr();
		CString Right(u8 len);
		CString Left(u8 len);
		void Trim();

		const CString &operator= (const CString &str);
		const CString &operator= (const s8 *str);

};


#endif
