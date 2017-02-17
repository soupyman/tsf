
#include "cstring.h"

#include <stdarg.h>

CString::CString(void)
{

}


CString::~CString(void)
{

}

bool CString::Append(const CString &str)
{
	append(str);

	return true;
}

bool CString::AppendFormat(const s8 *pszFormat, ...)
{
	va_list ap;
	s8 str[256];

	va_start(ap, pszFormat);
	/*
	sprintf pass the var param lists.
	vsprintf pass the point of var param lists.
	*/
	vsprintf(str, pszFormat, ap);
	va_end(ap);

	append(str);

	return true;
}

bool CString::Format(const s8 *pszFormat, ...)
{
	va_list ap;
	s8 str[256];

	va_start(ap, pszFormat);
	vsprintf(str, pszFormat, ap);
	va_end(ap);

	clear();
	append(str);
	return true;
}

bool CString::Find(CString &str)
{
	if(string::npos == find(str))
		return false;
	else
		return true;
}

bool CString::Empty()
{
	clear();
	return true;
}

bool CString::IsEmpty()
{

	return empty();
}


u32 CString::GetLength()
{

	return length();
}

bool CString::Delete(u32 start, u32 lenth)
{
	erase(start, lenth);
	return true;
}


const CString &CString::operator= (const CString &str)
{
	clear();
	append(str);

	return *this;
}


const CString &CString::operator= (const s8 *str)
{
	clear();
	append(str);
	return *this;
}

bool CString::IsNumberStr()
{
	const s8 *pDataStr = c_str();
	for(u8 i = 0 ; i < GetLength() ; i ++)
	{
		if(('0' > pDataStr[i]) || ('9' < pDataStr[i]))
			return false;
	}
	return true;
}


CString CString::Right(u8 len)
{
	s8 tmp[256];
	CString str;
	const s8 *pDataStr = c_str();
	if(len > GetLength())
		len = GetLength();

	for(u8 i = 0; i < len; i++)
		tmp[i] = pDataStr[GetLength() - len + i];

	tmp[len] = 0;
	str = tmp;
	return str;
}

CString CString::Left(u8 len)
{
	s8 tmp[256];
	CString str;
	const s8 *pDataStr = c_str();
	if(len > GetLength())
		len = GetLength();

	for(u8 i = 0; i < len; i++)
		tmp[i] = pDataStr[i];

	tmp[len] = 0;
	str = tmp;
	return str;
}

void CString::Trim()
{
	while(' ' == at(0))
		Delete(0, 1);

	while(' ' == at(GetLength() - 1))
		Delete(GetLength() - 1, 1);
}


