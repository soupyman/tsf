

#include "StdAfx.h"

#include "Message.h"

CMessageSpace::CMessageSpace(void)
{

}

CMessageSpace::~CMessageSpace(void)
{
}

CMessage::CMessage(void)
{

}

CMessage::~CMessage(void)
{
}

BOOL CMessage::MessageToUI(UINT msg, WPARAM wP, LPARAM lP)
{
	return AfxGetMainWnd()->PostMessage(msg,wP,lP);
}


BOOL CMessage::InformationToUI(LPCSTR pszFormat, ...)
{
	CString * pInfo = new CString;
	va_list ptr; 
	va_start(ptr, pszFormat);
	pInfo->FormatV(pszFormat,ptr);
	va_end(ptr);
	 return MessageToUI(MSG_UI_INFORMATION,	(WPARAM)pInfo);
}

