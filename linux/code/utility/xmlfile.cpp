#define _NOINFOM

#include "xmlfile.h"
#include "expat.h"
#include <string.h>

extern "C"
{
	int stricmp(const char *dst, const char *src)
	{
		int ch1, ch2;
		do
		{
			if(((ch1 = (unsigned char)(*(dst++))) >= 'A') && (ch1 <= 'Z'))
				ch1 += 0x20;
			if(((ch2 = (unsigned char)(*(src++))) >= 'A') && (ch2 <= 'Z'))
				ch2 += 0x20;
		}
		while(ch1 && (ch1 == ch2));

		return(ch1 - ch2);
	}
	static void XMLCALL xmlstart(void *data, const char *el, const char **attr)
	{
		CXmlFile *pCf = (CXmlFile *) data;

		pCf->StartValue(el,  attr);
	}

	static void XMLCALL xmlend(void *data, const char *el)
	{
		CXmlFile *pCf = (CXmlFile *) data;

		pCf->EndValue(el);
	}

	/*
	note that: string s is not end with 0, but only length = len
	*/
	static void XMLCALL xmlparsedata(void *userData, const XML_Char *s, int len)
	{
		CXmlFile *pCf = (CXmlFile *) userData;

		if((NULL == userData) || (NULL == s) || (4096 < len))
			return ;
#if 1 /* trim useless char*/
		while((len > 0) &&
				((s[0] == ' ') || (s[0] == '	') || (s[0] == '\r') || (s[0] == '\n')))
		{
			s++;
			len --;
		}
		while((len > 0) &&
				((s[len - 1] == ' ') || (s[len - 1] == '	') || (s[len - 1] == '\r') || (s[len - 1] == '\n')))
		{
			len--;
		}
#endif
		if(len > 0)
		{
			pCf->AddValue((s8 *)s, len);
		}
	}

	static int XMLCALL	XMLEncodingHandle(void *encodingHandlerData,
										  const XML_Char *name,
										  XML_Encoding *info)
	{
		//  map[i]  ֵ����,
		//      >=0 ��ʾ��Ӧ��ֵ�ǵ��ַ�
		//      -1  ��ʾ�Ƿ��ַ�
		//      -2,-3,-4    ��ʾ�Ƕ��ֽ��ַ��ĵ�һ��,�ֱ���2�ֽڡ�3�ֽڡ�4�ֽڵ��ַ�
		//  ʹ��:
		//      expat�����map[(unsigned char)s[i]] ��ֵ�ж�ĳ���ַ���������ٸ��ֽڣ�Ȼ��������Ӧ����Ŀ
		//
		//  info->data    �������ݣ����ֻ��ش���convert��release,expat�ڲ������д���,
		//  expat���������л����convert,����������release(���������ֵ��ΪNULL�Ļ�)
		//  ����ǵ��ֽ��ַ�������convert����ΪNULL,
		//  ����Ƕ��ֽڰ�����,convertӦ����ֵ,����Ѷ�Ӧ������ת����ucs2����

		int i;
		if(!name ||  stricmp(name, "GB2312"))
			return XML_STATUS_ERROR;

		for(i = 0; i < 128; i++)
			info->map[i] = i;
		for(; i < 256; i++)
			info->map[i] = -2;
		//info->convert   = Convert;
		info->convert   =  NULL;
		info->release   = NULL;
		return XML_STATUS_OK;
	}

}

CXmlFile::CXmlFile(const s8 *pFileName)
	: CStreamFile(pFileName)
{
	Depth = 0;
}

CXmlFile::~CXmlFile(void)
{

}


void CXmlFile::StartValue(const char *el, const char **attr)
{
	INF("start element:<%s> deep:%d.\n", el, Depth + 1);
	if(DeepReachMax())
	{
		ERR("DeepReachMax, depth :%d. ", Depth);
		return;
	}
	Depth ++;

	CurValueLenth = 0;
	
	UserStartValue(el, attr);
}

void CXmlFile::EndValue(const char *el)
{
	INF("end element:</%s>deep:%d.\n", el, Depth);
	if(0 == Depth)
	{
		ERR("element stack depth error. ");
		return;
	}
	Depth    --;
	
	UserEndValue(el);


	CurValueLenth = 0;
}

void CXmlFile::AddValue(s8 *s, int len)
{
	ASSERT(VALUE_BUFFER_MAX > (CurValueLenth + len));

	memcpy((void *)(CurValuebuf + CurValueLenth), (void *)s, len);
	CurValueLenth += len;
}



bool CXmlFile::Load(void)
{
	bool ret = true;

	if(true == (ret = Open(CFile::OPEN_TEXT_READ_ONLY)))
	{
		s8 buf[READ_BUFF_SIZE];
		u16 CurLenth;
		XML_Parser parser = XML_ParserCreate(NULL);
		ASSERT(parser);

		UserInitBeforeLoad();
		SeekTo(0);
		Depth = 0;

		//  �����utf-8 us-ascii ucs2 iso8859-1 �ǲ���ʹ���������������
		//  ��ȻҪʹ�ã���ʹ�ûᱨ�ַ�����֧�ֵĴ���,
		//  ��xmlͷ��ʱ����õ�<?xml version="1.0" encoding="gb2312" ?>

		XML_SetUnknownEncodingHandler(parser, XMLEncodingHandle, 0);

		XML_SetElementHandler(parser, xmlstart, xmlend);
		XML_SetUserData(parser, this);
		XML_SetCharacterDataHandler(parser, xmlparsedata);

		while((CurLenth = Read((u8 *)buf, READ_BUFF_SIZE)))
		{
			if(XML_Parse(parser, buf, CurLenth , ReachEnd()) == XML_STATUS_ERROR)
			{
				ERR("%s at line %d.\n", XML_ErrorString(XML_GetErrorCode(parser)),
					(int)(XML_GetCurrentLineNumber(parser)));
				ret = false;
				break;
			}
		}
		XML_ParserFree(parser);

		Close();
	}
	else
		ERR("Open XML file(%s) failed.", GetFileName().c_str());

	return ret;
}


