#ifndef CSTRING_H
#define CSTRING_H
/*
name the file to cstring for avoid overlap with system file string.h
*/

/*
father class string's function:

  append   �����ܽ�ĳ�ַ����������ݴ����ӵ�ĳ�ַ�������
  assign   �����ܽ��ַ����������ݴ����������趨��ĳ�ַ���
  at   �����ܻش��ַ�����λ��ĳ��λ�õ��ַ�
  begin   �������ش����ַ�������ʼ��λ��
  c_str   �����ܽ��ַ��������е����ݣ���C�����ַ����ķ�ʽ���
  capacity   �����ܻش�Ŀǰ���ܿ��ƵĶ���������С
  compare   ��������ش�   �ɿ��Ƶ��ַ�������
  copy   �����ܽ����ݴ������ݸ��Ƶ���һ�����ݴ���
  data   �����ܽ����ݴ��ĵ�һ����ʼָ�봫��
  empty   �������ж����ݴ��Ƿ�Ϊ�մ�
  end   �������ش����ַ�������ֹ��λ��
  erase   �����������е��������
  find   ��������������Ѱ�ҷ��������������ֵ�λ��
  find_first_of   ��������������Ѱ�ҵ�һ�������ض�Ҫ������ݳ��ֵ�λ��
  find_first_not_of   ��������������Ѱ�ҵ�һ���������ض�Ҫ������ݳ��ֵ�λ��
  find_last_of   ��������������Ѱ�����һ�������ض�Ҫ������ݳ��ֵ�λ��
  find_last_not_of   ��������������Ѱ�����һ���������ض�Ҫ������ݳ��ֵ�λ��
  insert   �����ܽ����ݲ��뵽�����е�ĳ���ض�λ��
  length   �����ܼ������ݴ��ĳ���
  replace   �����ܽ����ݲ��뵽�����е�ĳ���ض�λ��
  reserve   �����ܱ�֤capacity�����Ļش�ֵ��һ������ĳ��
  resize   ������ȷ��size�����Ļش�ֵ��һ������ĳ��
  rfind   ��������������Ѱ�����һ�������ض�Ҫ������ݳ��ֵ�λ��
  size   �����ܻش�Ŀǰ���������Ĵ�С
  substr   �����ܽ����ü�Ϊ���ٵĴ�
  swap   �����ܽ������������ݽ���
  max_size   �����ܽ������������ݽ���

  getline   ������ͨ������װ�ã�����������ݴ洢��������
  swap�����ܽ��������ݴ��е�����������
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
