#pragma once

/*	portable include  */
#include "typedefine.h"
#include "xmlfile.h"
#include "formatdb.h"

class CFormatFile : public CXmlFile, public CFmtDbInterface
{
		/*          node type string define        */
#define STRING_INCLUDE 	"INCLUDE"
#define STRING_FILE 	        "FILE"
#define STRING_TABLE 	"TABLE"
#define STRING_DESCRIPTOR "DESCRIPTOR"
#define STRING_FIELD 	"FIELD"
#define STRING_RESERVE  "RESERVE"
#define STRING_LOOP         "LOOP"
#define STRING_IF               "IF"
#define STRING_ELSE         "ELSE"

		/*          attr type string define        */
#define STRING_ID       	"id"
#define STRING_NAME 	"name"
#define STRING_LENTH 	"lenth"
#define STRING_CRC       	"crc"
#define STRING_FORMAT 	"format"
#define STRING_CONDITION 	"condition"

		/*          format short name   */
#define STRING_HEX      "hex"
#define STRING_STR      "str"
		/*          bool string name   */
#define STRING_TRUE      "true"
#define STRING_FALSE      "false"

	public:
		/*  temp data when parse    */
		CFormatTree   *pFTree;
		CFormatTree   *pCurTree;/* a temp tree point for parse */
		bool			IncludeFlag;

		/*  attr data   */
		CString name ;
		Format_e format  ;
		u32 		id;
		bool		CrcCheck;
		CFormula *lenth ;
		CFormula *condition;

	public:
		CFormatFile(const s8 *pFileName);
		~CFormatFile(void);
		void ReadAttr(const s8 **attr);
		void ClearFmtTree();
		void StoreFmtTree();
            void ClearAttr();
		void SetValue(const s8 *el,const s8 *vl);
		void UserStartValue(const s8 *el, const s8 **attr);
		void UserEndValue(const s8 *el);

		bool CheckAndTrimFmlStr(CString &FmlStr);
		s32 FindLastOpInTopLevel(CString  FmlStr);
		CFormula *ParseFormula(CString  FmlStr);

};

